
#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/sockimpl.h"
#include "../common/code.conv.h"
#include "matrix.h"
#include "ip.setting.resp.h"
#include "common.functions.h"

extern stdext_hash_map __serial_no_hash_map;
extern stdext_hash_map __route_hash_map;
extern int __work_items_size;	//使用设备个数
extern EQUIPMENT_ITEM_PTR __free_items; //没有使用设备
extern EQUIPMENT_ITEM_PTR __work_items;
extern int __work_items_size;	//使用设备个数
extern ip_setting_list __ip_setting_list; //ip响应链表，该结构为实际操作结构
extern IP_SETTING_INFO_PTR __free_ip_setting_list;

extern unsigned long volatile __be_ip_setting_completed_counter; //是否完成首包ip设置，该部分主要是查询ip_setting_list是否为空，如果为空+1,否则= 0

extern stdext_hash_map __ip_hash_map;

typedef struct __tagIP_SETTING_RESP_PACKAGE_INFO
{
	int serial_no_;
	int ip_;
	int deletion_sign_;
	int wait_count_;
	int ip_count_;
	int route_[MAX_ROUTE_SIZE];
	int cmd_word_;
	int type_;
}IP_SETTING_RESP_PACKAGE_INFO, *IP_SETTING_RESP_PACKAGE_INFO_PTR;

enum
{
	MATRIX_IP_SETTING_RESP_PORT = PORT_IP_SET,
	MATRIX_IP_SETTING_RESP_SND_PORT = DESTINATION_PORT,
	MAX_IP_COUNTER = 0x3,
	MAX_WAIT_COUNTER = 0x3,
};

SOCKET_IMPL __ip_setting_resp_socket;


bool init_ip_setting_resp()
{
	bool is_ok = true;

	if(__ip_setting_resp_socket.socket_)	close_udp_socket(&__ip_setting_resp_socket);

	SecureZeroMemory(&__ip_setting_resp_socket, sizeof(__ip_setting_resp_socket)); 

	__ip_setting_resp_socket.listen_port_ = MATRIX_IP_SETTING_RESP_PORT;
	__ip_setting_resp_socket.remote_port_ = MATRIX_IP_SETTING_RESP_SND_PORT;
	
	is_ok = create_udp_socket(&__ip_setting_resp_socket, SOCKET_BOARDCAST_ADDRESS);
	if(!is_ok) return is_ok;

	return is_ok;
}

void release_ip_setting_resp()
{
	close_udp_socket(&__ip_setting_resp_socket);
}

bool check_legal_package(unsigned char* buf, int buf_size, IP_SETTING_RESP_PACKAGE_INFO_PTR ip_rec_pack_info)
{
	buf_size = buf_size;
	unsigned m_sn = 0;
	unsigned m_route[MAX_ROUTE_SIZE];
	unsigned m_ip = 0;
	unsigned m_end = 0;
	unsigned m_end_laux = 0;
	unsigned m_sn_l = 0;
	unsigned m_broadcast_port = 0;
	memset(&m_route[0],0,MAX_ROUTE_SIZE*sizeof(unsigned));

	memcpy(&ip_rec_pack_info->cmd_word_, buf + 0x1a, 2);
	memcpy(&m_sn, buf + 0x1c, 1);
	memcpy(&m_ip, buf + 0x21, 1);
	for (int i = 0; i < MAX_ROUTE_SIZE; i++){

		memcpy(&m_route[i], buf + 0x26 + 5*i, 1);
		memcpy(&ip_rec_pack_info->route_[i], buf + 0x27 + 5*i, 4);
	}
	memcpy(&m_broadcast_port, buf + 0x3a, 1);
	memcpy(&m_end_laux, buf + 0x3f, 1);
	memcpy(&m_end, buf + 0x2b, 1);
	memcpy(&ip_rec_pack_info->serial_no_, buf + 0x1d, 4);
	memcpy(&ip_rec_pack_info->ip_, buf + 0x22, 4);
	memcpy(&m_sn_l, &buf[0x1d], 1);

	if (ip_rec_pack_info->cmd_word_ == CMD_SET && m_sn == CMD_SN && m_ip == CMD_IP){

		if (m_sn_l == LAUX && m_broadcast_port == CMD_BOARDCAST_PORT_W && m_end_laux == CMD_END){

			ip_rec_pack_info->type_ = LAUX;
			for (int j = 0; j < MAX_ROUTE_SIZE; j++){

				if(m_route[j] != CMD_ROUTE_SET){
					MessageBox(NULL,"IP_SET_REC PACKAGE ERROR!","ERROR",MB_OK);
					return false;
				}
			}
		}
		else if (m_sn_l == LAUL && m_end == CMD_END){

			ip_rec_pack_info->type_ = LAUL;
		} 
		else if (m_sn_l == FDU && m_end == CMD_END){

			ip_rec_pack_info->type_ = FDU;
		}
		else{
			MessageBox(NULL,"IP_SET_REC PACKAGE ERROR!","ERROR",MB_OK);
			return false;
		}
	}
	else{

		return false;
	}

	return true;
}

//
void proc_ip_setting_resp()
{
	char recv_buf[MAX_INPUT_PACKAGE_SIZE] = {0};
	char error_buf[MAX_INPUT_PACKAGE_SIZE * 6] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	int i = 0;
	EQUIPMENT_ITEM_PTR eqp_item_ptr = NULL;
	IP_SETTING_RESP_PACKAGE_INFO pack;
	ip_setting_list_iterator itr;
	IP_SETTING_INFO_PTR ip_setting_info_ptr = NULL;

	SOCKADDR_IN remote_ip;
	
	SecureZeroMemory(&remote_ip, sizeof(remote_ip));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_port = htons(MATRIX_IP_SETTING_RESP_SND_PORT);
	remote_ip.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	while(true){

		if(ioctlsocket(__ip_setting_resp_socket.socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = recv(__ip_setting_resp_socket.socket_, recv_buf, sizeof(recv_buf), 0);

				if(recv_size == SOCKET_ERROR)	break;
				else if(recv_size == 0x0){

					WSASetLastError(REMOTE_CLOSE_CONNECTION_GRACEFULLY);
					break;
				}
				else if(recv_size > MAX_INPUT_PACKAGE_SIZE){

					WSASetLastError(IP_SETTING_RECV_SIZE_ERROR);
					recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
					output_log(IDS_STRING116, error_buf);
					continue;
				}
				else{

					//process ip-setting-resp package
					SecureZeroMemory(&pack, sizeof(pack));
					if(!check_legal_package((unsigned char*)recv_buf, recv_size, &pack)){

						SetLastError(IP_SETTING_FORMAT_ERROR);
						recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
						output_log(IDS_STRING116, error_buf);
						continue;
					}
					
					for(itr = __ip_setting_list.begin(); itr != __ip_setting_list.end(); ++itr){

						ip_setting_info_ptr = *itr;
						if(ip_setting_info_ptr->equipment_item_->serial_no_ == pack.serial_no_){

							if(ip_setting_info_ptr->equipment_item_->ip_ == pack.ip_)	{	ip_setting_info_ptr->pack_[DELETION_TOKEN_INDEX] = 1;}
							else														{	ip_setting_info_ptr->pack_[DELETION_TOKEN_INDEX] = (unsigned char)-1;}
						}
					}
				}
			}
			else	break;
		}
		else	break;
	}
	
	itr = __ip_setting_list.begin();
	while(itr != __ip_setting_list.end()){			

		ip_setting_info_ptr = *itr;
		if(ip_setting_info_ptr->pack_[DELETION_TOKEN_INDEX] == 1){

			eqp_item_ptr = ip_setting_info_ptr->equipment_item_;
			itr = __ip_setting_list.erase(itr);
				
			//add a free ip setting list
			ip_setting_info_ptr->next_ = __free_ip_setting_list;
			__free_ip_setting_list = ip_setting_info_ptr;

			//add ip index
			eqp_item_ptr->is_valid_ip_ = true;
			__ip_hash_map[eqp_item_ptr->ip_] = eqp_item_ptr;

			//add route index
			if(eqp_item_ptr->type_ == SN_MAIN_LAUX || eqp_item_ptr->type_ == LAUX)
				for(i = 0; i < MAX_ROUTE_SIZE; ++i)
					if(eqp_item_ptr->route_[i] != 0)
						__route_hash_map[eqp_item_ptr->route_[i]] = eqp_item_ptr;
		}
		else if(ip_setting_info_ptr->pack_[DELETION_TOKEN_INDEX] == (unsigned char)-1){ //invalidate

			eqp_item_ptr = ip_setting_info_ptr->equipment_item_;
			itr = __ip_setting_list.erase(itr);

			//add a free ip setting list
			ip_setting_info_ptr->next_ = __free_ip_setting_list;
			__free_ip_setting_list = ip_setting_info_ptr;
		}
		else{//do not find it!

			//ip counter > 3
			if(ip_setting_info_ptr->pack_[PROC_COUNT_TOKEN_INDEX] < MAX_IP_COUNTER){

				if(ip_setting_info_ptr->pack_[WAIT_COUNT_TOKEN_INDEX] >= MAX_WAIT_COUNTER){
					
					recv_size = udp_send(&__ip_setting_resp_socket, (char*)ip_setting_info_ptr->pack_, 
										sizeof(ip_setting_info_ptr->pack_));

					if(recv_size != sizeof(ip_setting_info_ptr->pack_)){ //send failed 

						WSASetLastError(IP_SETTING_SEND_ERROR);
						output_log(IDS_STRING117, ip_setting_info_ptr->equipment_item_->serial_no_);
					}
					else{//send successsfully
						ip_setting_info_ptr->pack_[WAIT_COUNT_TOKEN_INDEX] = 0x0;
						++ip_setting_info_ptr->pack_[PROC_COUNT_TOKEN_INDEX];
					}
				}
				else	++ip_setting_info_ptr->pack_[WAIT_COUNT_TOKEN_INDEX];

				++itr;				
			}
			else{ //log & delete work-list, sn index and ip setting list
				
				recv_size = bin_to_ansi((unsigned char*)ip_setting_info_ptr->pack_, sizeof(ip_setting_info_ptr->pack_), 
												error_buf, sizeof(error_buf));

				output_log(IDS_STRING118, ip_setting_info_ptr->pack_[PROC_COUNT_TOKEN_INDEX], 
							ip_setting_info_ptr->equipment_item_->serial_no_, error_buf);

				//delete from ip setting list
				eqp_item_ptr = ip_setting_info_ptr->equipment_item_;
				itr = __ip_setting_list.erase(itr);

				//add a free ip setting list
				ip_setting_info_ptr->next_ = __free_ip_setting_list;
				__free_ip_setting_list = ip_setting_info_ptr;
				delete_serial_no(eqp_item_ptr->serial_no_);
				delete_equipment(eqp_item_ptr);
			}
		}
	}
}

