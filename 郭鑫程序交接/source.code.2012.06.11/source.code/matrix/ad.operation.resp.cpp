/**
@brief	处理AD设置命令应答处理
@details	该模块使用发送队列方式,将要发送的数据放在队列中, 在定时从IO接受数据,比较是否存在队列中,如果不在扔掉,存在进行标识, 然后检查发送队列,对于没有发送的数据包进行发送处理,知道队列为空,将结束应答处理
@file	ad.operation.resp.cpp
@note
@see
@version	1.0.8.42.2011.8.18
@author	kuoxin	
@bug	1.0.8.42.2011.8.18	修改数据包验证函数check_legal_package,
							加入add_package_token 加入发送包应答检查码,
							加入compare_package 比较接受包和发送包是否应答匹配,
							修改proc_ad_operation_resp,改变验证匹配方式

*/
#include <time.h>
#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/sockimpl.h"
#include "../common/code.conv.h"
#include "matrix.h"
#include "equipment.comm.order.h"
#include "common.functions.h"
#include "ad.operation.resp.h"

#define __MATRIX_DEBUG__

enum
{
	MATRIX_AD_OPERATION_RESP_PORT = PORT_AD_SET,
	MATRIX_AD_OPERATION_RESP_SND_PORT = DESTINATION_PORT,
	MAX_AD_COUNTER = 0x3,
	MAX_AD_WAIT_COUNTER = 0x3,
	MAX_AD_CMD_PACKAGE_ARRAY_SIZE = 1280,
	AD_OPERATION_RESP_EVENT_ID = 0xc000,

	INPUT_CMD_BUFFER_INDEX = 0x1c,
	OUTPUT_CMD_BUFFER_INDEX = 0x20,
};

typedef struct __tagAD_OPERATION_ITEM
{
	EQUIPMENT_ITEM_PTR equipment_ptr_;
	unsigned char pack_[MAX_OUTPUT_PACKAGE_SIZE];
	__tagAD_OPERATION_ITEM* next_;
}AD_OPERATION_ITEM, *AD_OPERATION_ITEM_PTR;

//ad设置应答帧
typedef struct __tagTHE_AD_SET_REC_PACKAGE_INFO 
{
	unsigned ip_;
	unsigned cmd_word_;
	unsigned char* addr_ptr_;
	unsigned int cmd_length_;
}THE_AD_SET_REC_PACKAGE_INFO, *THE_AD_SET_REC_PACKAGE_INFO_PTR;

typedef stdext::hash_map<unsigned, AD_OPERATION_ITEM_PTR> AD_OPERATION_HASH_MAP;
typedef stdext::hash_map<unsigned, AD_OPERATION_ITEM_PTR>::iterator AD_OPERATION_HASH_MAP_ITERATOR;

//extern global variable
extern stdext_hash_map __route_hash_map;
extern stdext_hash_map __ip_hash_map;
extern APPLICATION_ENVIRONMENT	__application_environment;

//local global variable
SOCKET_IMPL __ad_operation_resp_socket;
AD_OPERATION_ITEM __ad_operation_array[MAX_AD_CMD_PACKAGE_ARRAY_SIZE];
AD_OPERATION_HASH_MAP __ad_operation_hash_map;
AD_OPERATION_ITEM_PTR __free_ad_operation_list;
unsigned int __elapsed_time;
bool __is_ad_operation_running;

void clear_ad_operation_resp();
void start_ad_operation(unsigned int);
void stop_ad_operation();

bool is_ad_operation_running(){return __is_ad_operation_running;}

bool init_ad_operation_resp()
{
	bool is_ok = true;

	if(__ad_operation_resp_socket.socket_)	close_udp_socket(&__ad_operation_resp_socket);
	SecureZeroMemory(&__ad_operation_resp_socket, sizeof(__ad_operation_resp_socket));
	__ad_operation_resp_socket.listen_port_ = MATRIX_AD_OPERATION_RESP_PORT;
	__ad_operation_resp_socket.remote_port_ = MATRIX_AD_OPERATION_RESP_SND_PORT;
	is_ok = create_udp_socket(&__ad_operation_resp_socket, SOCKET_BOARDCAST_ADDRESS);
	if(!is_ok) return is_ok;

	srand( (unsigned)time( NULL ));

	clear_ad_operation_resp();
	return is_ok;
}

void release_ad_operation_resp()
{
	stop_ad_operation();
	__ad_operation_hash_map.clear();
	close_udp_socket(&__ad_operation_resp_socket);
}

void clear_ad_operation_resp()
{
	int i = 0x0;
	int size = sizeof(__ad_operation_array) / sizeof(__ad_operation_array[0]) - 1;
	
	SecureZeroMemory(__ad_operation_array, sizeof(__ad_operation_array));
	for(i = 0; i < size; ++i)	__ad_operation_array[i].next_ = __ad_operation_array + i + 1;

	__free_ad_operation_list = __ad_operation_array;
	__elapsed_time = 0x0;
	__ad_operation_hash_map.clear();
	__is_ad_operation_running = false;
}

//加入数据匹配标记,在每个发送数据包中
void add_package_token(unsigned char* pack_ptr//将要发送数据包
					   )
{
	unsigned int id = 0;
	unsigned char* ptr = NULL;

	id = rand();
	ptr	= pack_ptr + OUTPUT_CMD_BUFFER_INDEX;
	while(*ptr) ptr += 5;
	
	*ptr = 0xF;
	memcpy(++ptr, &id, 4);
	ptr += 4;
	*ptr = 0x0;
}

//比较当前接受数据包是否为发送的数据包的应答
bool compare_package(unsigned char* inp_package, //接受数据包
					 unsigned char* outp_package //发送数据包
					 )
{
	_ASSERT(inp_package);
	_ASSERT(outp_package);

	bool is_ok = false;
	unsigned char* inp_ptr = inp_package + INPUT_CMD_BUFFER_INDEX;
	unsigned char* outp_ptr = outp_package + OUTPUT_CMD_BUFFER_INDEX;

	while(*inp_ptr) inp_ptr += 5;
	while(*outp_ptr) outp_ptr += 5;
	
	inp_ptr -= 4;
	outp_ptr -= 4;
	if(memcmp(inp_ptr, outp_ptr, 4) == 0)	is_ok = true;
	return is_ok;
}

bool insert_into_ad_operation_list(EQUIPMENT_ITEM_PTR equipment_ptr, MAKE_AD_CMD_PACKAGE_PTR function_ptr, void* void_ptr)
{
	bool is_ok = true;
	AD_OPERATION_ITEM_PTR ad_setting_ptr = NULL;

	if(__free_ad_operation_list == NULL){

		output_log(IDS_STRING154, 0x0);
		return false;
	}

	ad_setting_ptr = __free_ad_operation_list;
	__free_ad_operation_list = __free_ad_operation_list->next_;

	ad_setting_ptr->equipment_ptr_ = equipment_ptr;
	ad_setting_ptr->next_ = NULL;

	function_ptr(ad_setting_ptr->equipment_ptr_, ad_setting_ptr->pack_, sizeof(ad_setting_ptr->pack_), void_ptr);
	__ad_operation_hash_map[equipment_ptr->ip_] = ad_setting_ptr;
	add_package_token(ad_setting_ptr->pack_);//加上一个标记号, 支持与回应

	return is_ok;
}
void proc_ad_operation_resp();
VOID CALLBACK proc_ad_operation_resp_callback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(__application_environment.main_window_, AD_OPERATION_RESP_EVENT_ID);
	proc_ad_operation_resp();
	SetTimer(__application_environment.main_window_, AD_OPERATION_RESP_EVENT_ID, 
		__elapsed_time, proc_ad_operation_resp_callback);
}

void start_ad_operation(unsigned int elapsed_time)
{
	__elapsed_time = elapsed_time;
	__is_ad_operation_running = true;
	SetTimer(__application_environment.main_window_, AD_OPERATION_RESP_EVENT_ID, 
			__elapsed_time, proc_ad_operation_resp_callback);
}

void stop_ad_operation()
{
	clear_ad_operation_resp();
	__is_ad_operation_running = true;
	KillTimer(__application_environment.main_window_, AD_OPERATION_RESP_EVENT_ID);
}

bool be_ad_operation_completed()
{
	bool is_ok = true;

	if(__is_ad_operation_running){
		if(__ad_operation_hash_map.size() == 0)	is_ok = true;
		else									is_ok = false;
	}

	return is_ok;
}

bool check_legal_package(unsigned char* buf, int buf_size, THE_AD_SET_REC_PACKAGE_INFO_PTR ad_data_rec_pack_info)
{
	unsigned char* ptr = NULL;
	memcpy(&ad_data_rec_pack_info->ip_, &buf[0x10], 4);
	memcpy(&ad_data_rec_pack_info->cmd_word_, &buf[0x1a], 2);
	if(ad_data_rec_pack_info->cmd_word_ == CMD_SET){

		ptr = ad_data_rec_pack_info->addr_ptr_ = buf + 0x1c;
		while(*ptr) ptr += 5;
		ad_data_rec_pack_info->cmd_length_ = ptr - ad_data_rec_pack_info->addr_ptr_;		
		return true;
	}
	else 
		return false;
}

void proc_ad_operation_resp()
{
	char recv_buf[MAX_INPUT_PACKAGE_SIZE] = {0};
	char error_buf[MAX_INPUT_PACKAGE_SIZE * 6] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	THE_AD_SET_REC_PACKAGE_INFO pack;
	AD_OPERATION_HASH_MAP_ITERATOR itr;
	AD_OPERATION_ITEM_PTR ad_setting_ptr = NULL;

	SOCKADDR_IN remote_ip;
	
	SecureZeroMemory(&remote_ip, sizeof(remote_ip));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_port = htons(MATRIX_AD_OPERATION_RESP_SND_PORT);
	remote_ip.sin_addr.s_addr = htonl(INADDR_BROADCAST);

#ifdef __MATRIX_DEBUG__
	char dbuf[MAX_STRING_SIZE * 4] = {0};
	int dsize = 0;
#endif //__MATRIX_DEBUG__

	while(true){

		if(ioctlsocket(__ad_operation_resp_socket.socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = recv(__ad_operation_resp_socket.socket_, recv_buf, sizeof(recv_buf), 0);

				if(recv_size == SOCKET_ERROR)	break;
				else if(recv_size == 0x0){

					WSASetLastError(REMOTE_CLOSE_CONNECTION_GRACEFULLY);
					break;
				}
				else if(recv_size > MAX_INPUT_PACKAGE_SIZE){

					WSASetLastError(AD_SETTING_RECV_SIZE_ERROR);
					recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
					output_log(IDS_STRING155, recv_size, error_buf);
					continue;
				}
				else{

#ifdef __MATRIX_DEBUG__
					dsize = bin_to_ansi((unsigned char*)recv_buf, recv_size, dbuf, sizeof(dbuf));
					dbuf[dsize] = 0x0;
					output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("接收AD命令应答并检查数据包"), dbuf);
#endif //__MATRIX_DEBUG__

					//process ip-setting-resp package
					SecureZeroMemory(&pack, sizeof(pack));
					if(!check_legal_package((unsigned char*)recv_buf, recv_size, &pack)){
						SetLastError(AD_SETTING_FORMAT_ERROR);
						recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
						output_log(IDS_STRING156, error_buf);
						continue;
					}

					//find this equipment
					itr = __ad_operation_hash_map.find(pack.ip_);
					if(itr != __ad_operation_hash_map.end()){

#ifdef __MATRIX_DEBUG__
						output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("比较接受AD命令响应数据包是否与发送相等"), __MATRIX_TEXT("NULL"));
#endif //__MATRIX_DEBUG__
						ad_setting_ptr = itr->second;
						if(compare_package((unsigned char*)recv_buf, ad_setting_ptr->pack_))
							ad_setting_ptr->pack_[DELETION_TOKEN_INDEX] = 1;
						else{

							recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
							output_log(IDS_STRING160, pack.ip_, error_buf);
						}
					}
					else{						
						recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
						output_log(IDS_STRING157, pack.ip_, error_buf);
					}
				}
			}
			else	break;
		}
		else	break;
	}
	
	itr = __ad_operation_hash_map.begin();
	while(itr != __ad_operation_hash_map.end()){			

		ad_setting_ptr = itr->second;
		if(ad_setting_ptr->pack_[DELETION_TOKEN_INDEX] == 1){

			itr = __ad_operation_hash_map.erase(itr);
				
			//add a free ip setting list
			ad_setting_ptr->next_ = __free_ad_operation_list;
			__free_ad_operation_list = ad_setting_ptr;
		}
		else{//do not find it!

			//ad counter > 3
			if(ad_setting_ptr->pack_[PROC_COUNT_TOKEN_INDEX] < MAX_AD_COUNTER){

				if(ad_setting_ptr->pack_[WAIT_COUNT_TOKEN_INDEX] >= MAX_AD_WAIT_COUNTER){
					
					recv_size = udp_send(&__ad_operation_resp_socket, (char*)ad_setting_ptr->pack_, 
										sizeof(ad_setting_ptr->pack_));

					if(recv_size != sizeof(ad_setting_ptr->pack_)){ //send failed 

						WSASetLastError(AD_SETTING_SEND_ERROR);
						output_log(IDS_STRING159, ad_setting_ptr->equipment_ptr_->ip_);
					}
					else{//send successsfully
						ad_setting_ptr->pack_[WAIT_COUNT_TOKEN_INDEX] = 0x0;
						++ad_setting_ptr->pack_[PROC_COUNT_TOKEN_INDEX];
#ifdef __MATRIX_DEBUG__
						dsize = bin_to_ansi(ad_setting_ptr->pack_, sizeof(ad_setting_ptr->pack_), dbuf, sizeof(dbuf));
						dbuf[dsize] = 0x0;
						output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("发送AD命令数据包"), dbuf);
#endif //__MATRIX_DEBUG__

					}
				}
				else	++ad_setting_ptr->pack_[WAIT_COUNT_TOKEN_INDEX];

				++itr;
			}
			else{ //stop this operations, output a error info
				
				recv_size = bin_to_ansi((unsigned char*)ad_setting_ptr->pack_, sizeof(ad_setting_ptr->pack_), 
												error_buf, sizeof(error_buf));

				output_log(IDS_STRING158, ad_setting_ptr->pack_[PROC_COUNT_TOKEN_INDEX], ad_setting_ptr->equipment_ptr_->ip_, error_buf);

				ad_setting_ptr->equipment_ptr_->is_valid_equipment_ = false;
				itr = __ad_operation_hash_map.erase(itr);

				//add a free ip setting list
				ad_setting_ptr->next_ = __free_ad_operation_list;
				__free_ad_operation_list = ad_setting_ptr;
			}
		}
	}
}

