
#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/sockimpl.h"
#include "../common/code.conv.h"
#include "matrix.h"
#include "the.first.package.proc.h"
#include "common.functions.h"

extern stdext_hash_map __serial_no_hash_map;
extern stdext_hash_map __route_hash_map;
extern int __work_items_size;	//使用设备个数
extern EQUIPMENT_ITEM_PTR __free_items; //没有使用设备
extern EQUIPMENT_ITEM_PTR __work_items;
extern int __work_items_size;	//使用设备个数
extern LING_ARRAY_ITEM __line_array[MAX_BIG_LINE_SIZE]; //大线数组，标志每条大线上设备
extern unsigned long volatile __the_first_package_idled_counter; //没有收到数据，如果收到+1, 否则等于zero
extern APPLICATION_ENVIRONMENT __application_environment;
extern unsigned long volatile __line_no;//标记当前分配的大线数量

IP_SETTING_INFO __meta_ip_setting_array[MAX_IP_SETTING_ARRAY_SIZE];	//初始化ip设置响应队列数组，只用于存储结构
ip_setting_list __ip_setting_list; //ip响应链表，该结构为实际操作结构
IP_SETTING_INFO_PTR __free_ip_setting_list;

enum
{
	MATRIX_THE_FIRST_PACKAGE_PROC_PORT = 0x8000,
};

SOCKET_IMPL __the_first_package_socket;

bool init_the_first_package_proc(TCHAR const* remote_ip)
{
	bool is_ok = true;
	int i = 0, meta_size = 0;
	IP_SETTING_INFO_PTR ip_setting_ptr = NULL;

	if(__the_first_package_socket.socket_)	close_udp_socket(&__the_first_package_socket);

	SecureZeroMemory(&__the_first_package_socket, sizeof(__the_first_package_socket)); 

	__the_first_package_socket.listen_port_ = MATRIX_THE_FIRST_PACKAGE_PROC_PORT;

	is_ok = create_udp_socket(&__the_first_package_socket, remote_ip);
	if(!is_ok) return is_ok;

	SecureZeroMemory(__meta_ip_setting_array, sizeof(__meta_ip_setting_array));
	meta_size = sizeof(__meta_ip_setting_array) / sizeof(__meta_ip_setting_array[0]);

	ip_setting_ptr = __free_ip_setting_list = __meta_ip_setting_array;
	for(i = 1; i < meta_size; ++i){
		ip_setting_ptr->next_ = __meta_ip_setting_array + i;
		ip_setting_ptr = ip_setting_ptr->next_;
	}

	__ip_setting_list.clear();

	return is_ok;
}

void release_the_first_package_proc()
{
	close_udp_socket(&__the_first_package_socket);
}

//
bool check_the_first_package(unsigned char* buf, int buf_size, THE_FIRST_PACKAGE_INFO_PTR fst_pack_info)
{
	buf_size = buf_size;

	unsigned m_sn = 0;
	unsigned m_fst_time = 0;
	unsigned m_route = 0;
	unsigned m_end = 0;
	unsigned m_sn_l = 0;

	memcpy(&m_sn, &buf[0x1c], 1);
	memcpy(&m_fst_time, &buf[0x21], 1);
	memcpy(&m_route, &buf[0x2b], 1);
	memcpy(&m_end, &buf[0x30], 1);
	memcpy(&m_sn_l, &buf[0x1d], 1);

	memcpy(&fst_pack_info->cmd_word_, &buf[0x1a], 2);
	memcpy(&fst_pack_info->serial_no_, &buf[0x1d], 4);
	memcpy(&fst_pack_info->time_, &buf[0x22], 4);

	if (fst_pack_info->cmd_word_ == CMD_SEARCH && m_sn == CMD_SN && m_fst_time == CMD_FST_TIME){
		if(m_route == CMD_ROUTE_REC && m_end == CMD_END){

			memcpy(&fst_pack_info->route_, &buf[0x2c], 4);
			if (m_sn_l == LAUX)			fst_pack_info->type_ = LAUX;			
			else if (m_sn_l == LAUL)	fst_pack_info->type_ = LAUL;
			else if (m_sn_l == FDU)		fst_pack_info->type_ = FDU;
			else {
				MessageBox(NULL,"FIRST PACKAGE ERROR!","ERROR",MB_OK);
				return FALSE;
			}			
		}
		else if (m_route == 0x00 && fst_pack_info->serial_no_ == SN_MAIN_LAUX){	fst_pack_info->type_ = SN_MAIN_LAUX;}
		else{
			MessageBox(NULL,"FIRST PACKAGE ERROR!","ERROR",MB_OK);
			return false;
		}
	}
	else
		return false;

	return true;
}

/*
	路由分配规则：测线号*1000+方向（1=大线左，2=大线右，3=交叉线下）例如：1001——第一条测线左侧
	IP分配规则：192. 测线号. Xx. Xx

	当前用况为： 只有主交叉线上存在交叉站，即只有一条交叉线存在

	当前使用的ip分配方式为：
		1.主交叉站为192.0.0.0 这是默认分配
		2.交叉站来说，默认起始分配为 192.B.XXX.XXX 将交叉站信息存放在B位置存放信息如下：
			a. 所在大线信息 第6位存放
			b. 所在交叉线信息 高2位存放 
					交叉线A（主交叉线上方）00
					交叉线B（主交叉线下方）11		
		3.采集站和电源站 默认起始分配为 192.xxx.c.d 将使用c, d 来访方式信息
			c高两位存放大线A（交叉站左） 00
			c高两位存放大线B（交叉站右） 11

	路由分配方式为
		1. 主交叉站	默认方式为 ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_XXXX
		2. 交叉站		默认使用全局变量来记录当前线号 * ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_XXXX
	广播端口分配方式：
		1. 主交叉站	默认初始化端口BASE_BOARDCAST_PORT
		2. 交叉站		BASE_BOARDCAST_PORT + 路由方向
		3. 采集站		大线编号 * BASE_BOARDCAST_PORT + 大线A,B广播端口
*/
void set_master_laux_equipment_info(EQUIPMENT_ITEM_PTR equipment_ptr, 
									THE_FIRST_PACKAGE_INFO_PTR the_first_pack)
{
	SecureZeroMemory(equipment_ptr, sizeof(EQUIPMENT_ITEM));
	equipment_ptr->ip_ = inet_addr(LCI_IP_ADDRESS);
	equipment_ptr->route_[CROSS_LINE_A_INDEX] = ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_UP;//cross line A
	equipment_ptr->route_[CROSS_LINE_B_INDEX] = ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_DOWN;//cross line B
	equipment_ptr->route_[BIG_LINE_A_INDEX] = ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_LEFT;//Bing Line A
	equipment_ptr->route_[BIG_LINE_B_INDEX] = ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_RIGHT;//Big Line B

	equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX] = BASE_BOARDCAST_PORT;
	equipment_ptr->boardcast_port_[BIG_LINE_A_BOARDCAST_PORT_INDEX] = BASE_BOARDCAST_PORT + BIG_LINE_A_BOARDCAST_PORT;
	equipment_ptr->boardcast_port_[BIG_LINE_B_BOARDCAST_PORT_INDEX] = BASE_BOARDCAST_PORT + BIG_LINE_B_BOARDCAST_PORT;

	equipment_ptr->is_valid_ip_ = false;
	equipment_ptr->serial_no_ = the_first_pack->serial_no_;
	equipment_ptr->the_first_package_time_ = the_first_pack->time_;
	equipment_ptr->type_ = the_first_pack->type_;
	equipment_ptr->the_first_package_count_ += THE_FIRST_PACKAGE_INC_COUNTER;
}

void set_laux_equipment_info(EQUIPMENT_ITEM_PTR equipment_ptr, 
							 EQUIPMENT_ITEM_PTR prev_equipment_ptr,
							 THE_FIRST_PACKAGE_INFO_PTR the_first_pack)
{
	int route = 0x0;

	SecureZeroMemory(equipment_ptr, sizeof(EQUIPMENT_ITEM));
	equipment_ptr->ip_ = inet_addr(LCI_IP_ADDRESS);
	equipment_ptr->ip_ = ntohl(equipment_ptr->ip_);
	route = the_first_pack->route_ & ROUTE_DIRECTION_VALUE_MASK;
	equipment_ptr->ip_ >>= 22;

	if(route == ROUTE_UP){

		equipment_ptr->route_[CROSS_LINE_A_INDEX] = __line_no * ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_DOWN; //cross A
		equipment_ptr->route_[CROSS_LINE_B_INDEX] = ROUTE_UP; //cross B

		prev_equipment_ptr->up_ = equipment_ptr;
		equipment_ptr->down_ = prev_equipment_ptr;
		equipment_ptr->ip_ += 0x0;
		equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX] = BASE_BOARDCAST_PORT + ROUTE_UP;
	} 
	else if(route == ROUTE_DOWN){

		equipment_ptr->route_[CROSS_LINE_B_INDEX] = ROUTE_UP; //cross B
		equipment_ptr->route_[CROSS_LINE_A_INDEX] = __line_no * ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_DOWN; //cross A

		prev_equipment_ptr->down_ = equipment_ptr;
		equipment_ptr->up_ = prev_equipment_ptr;
		equipment_ptr->ip_ += 0x3;
		equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX] = BASE_BOARDCAST_PORT + ROUTE_DOWN;
	}

	equipment_ptr->ip_ <<= 6;
	equipment_ptr->ip_ += __line_no;
	equipment_ptr->ip_ <<= 16;

	equipment_ptr->ip_ = htonl(equipment_ptr->ip_);

	equipment_ptr->route_[BIG_LINE_A_INDEX] = __line_no * ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_LEFT; // line a
	equipment_ptr->route_[BIG_LINE_B_INDEX] = __line_no * ROUTE_BIG_LINE_BASE_NUMBER + ROUTE_RIGHT; // line b

	equipment_ptr->boardcast_port_[BIG_LINE_A_BOARDCAST_PORT_INDEX] = __line_no * BASE_BOARDCAST_PORT + BIG_LINE_A_BOARDCAST_PORT;
	equipment_ptr->boardcast_port_[BIG_LINE_B_BOARDCAST_PORT_INDEX] = __line_no * BASE_BOARDCAST_PORT + BIG_LINE_B_BOARDCAST_PORT;

	equipment_ptr->is_valid_ip_ = false;
	equipment_ptr->serial_no_ = the_first_pack->serial_no_;
	equipment_ptr->the_first_package_time_ = the_first_pack->time_;
	equipment_ptr->type_ = the_first_pack->type_;
	equipment_ptr->the_first_package_count_ += THE_FIRST_PACKAGE_INC_COUNTER;
}

void set_fdu_equipment_info(EQUIPMENT_ITEM_PTR equipment_ptr, 
					  EQUIPMENT_ITEM_PTR prev_equipment_ptr,
					  THE_FIRST_PACKAGE_INFO_PTR the_first_pack)
{

	int route = the_first_pack->route_ & ROUTE_DIRECTION_VALUE_MASK;
	SecureZeroMemory(equipment_ptr, sizeof(EQUIPMENT_ITEM));

	equipment_ptr->route_[0] = the_first_pack->route_;
	if(route == ROUTE_LEFT){
	
		equipment_ptr->left_ = prev_equipment_ptr->left_;
		prev_equipment_ptr->left_ = equipment_ptr;
		equipment_ptr->right_ = prev_equipment_ptr;
		equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX] = __line_no * BASE_BOARDCAST_PORT + BIG_LINE_A_BOARDCAST_PORT;
	}
	else if(route == ROUTE_RIGHT){
		equipment_ptr->right_ = prev_equipment_ptr->right_;
		prev_equipment_ptr->right_ = equipment_ptr;
		equipment_ptr->left_ = prev_equipment_ptr;
		equipment_ptr->boardcast_port_[CURRENT_EQUIPMENT_BOARDCAST_INDEX] = __line_no * BASE_BOARDCAST_PORT + BIG_LINE_B_BOARDCAST_PORT;
	}

	equipment_ptr->ip_ = get_new_ip_inc(prev_equipment_ptr->ip_, route);

	equipment_ptr->is_valid_ip_ = false;
	equipment_ptr->serial_no_ = the_first_pack->serial_no_;
	equipment_ptr->the_first_package_time_ = the_first_pack->time_;
	equipment_ptr->type_ = the_first_pack->type_;
	equipment_ptr->the_first_package_count_ += THE_FIRST_PACKAGE_INC_COUNTER;
}

bool create_new_equipment(THE_FIRST_PACKAGE_INFO_PTR the_first_pack)
{
	bool is_ok = true;
	int route = 0x0, line_no = 0x0;
	EQUIPMENT_ITEM_PTR equipment_ptr = NULL;
	EQUIPMENT_ITEM_PTR tmp_equipment_ptr = NULL, tmp_equipment_ptr2 = NULL;
	stdext_hash_map_iterator itr;

	if(__free_items == NULL){

		WSASetLastError(FREE_EQUIPMENT_ARRAY_IS_NULL);
		output_log(IDS_STRING113, 0);
		return is_ok = false;
	}

	equipment_ptr = __free_items;
	__free_items = __free_items->right_;

	if(the_first_pack->type_ == SN_MAIN_LAUX){ //主交叉站
		
		//add new equipment
		set_master_laux_equipment_info(equipment_ptr, the_first_pack);
		__line_no = 1;
		__work_items = equipment_ptr;
	}
	else if(the_first_pack->type_ == LAUX){ //交叉站
		
		itr = __route_hash_map.find(the_first_pack->route_);
		if(itr != __route_hash_map.end()){

			route = the_first_pack->route_ & ROUTE_DIRECTION_VALUE_MASK;
			tmp_equipment_ptr = itr->second;
			set_laux_equipment_info(equipment_ptr, tmp_equipment_ptr, the_first_pack);
			++__line_no;
		}
		else{

			WSASetLastError(EQUIPMENT_SERIAL_NO_FIND);
			output_log(IDS_STRING123, the_first_pack->serial_no_);
			return is_ok = false;			
		}
	}
	else{

		itr = __route_hash_map.find(the_first_pack->route_);
		if(itr != __route_hash_map.end()){

			route = the_first_pack->route_ & ROUTE_DIRECTION_VALUE_MASK;
			line_no = the_first_pack->route_ / ROUTE_BIG_LINE_BASE_NUMBER;
			tmp_equipment_ptr2 = tmp_equipment_ptr = itr->second;

			if(route == ROUTE_LEFT){
				
				while(tmp_equipment_ptr->left_ != NULL){

					if(tmp_equipment_ptr->left_->the_first_package_time_ < the_first_pack->time_)
						tmp_equipment_ptr = tmp_equipment_ptr->left_;
					else	break;
				}
			}
			else if(route == ROUTE_RIGHT){

				while(tmp_equipment_ptr->right_ != NULL){

					if(tmp_equipment_ptr->right_->the_first_package_time_ < the_first_pack->time_)	
						tmp_equipment_ptr = tmp_equipment_ptr->right_;
					else	break;
				}
			}

			set_fdu_equipment_info(equipment_ptr, tmp_equipment_ptr, the_first_pack);

			if(route == ROUTE_LEFT){

				tmp_equipment_ptr = equipment_ptr->left_;
				while(tmp_equipment_ptr != NULL){//设置其后的所有站计数器为0

					tmp_equipment_ptr->the_first_package_count_ = 0;
					tmp_equipment_ptr->ip_ = get_new_ip_inc(tmp_equipment_ptr->ip_, ROUTE_LEFT);

					if(tmp_equipment_ptr->is_valid_ip_ ||  
						tmp_equipment_ptr->the_first_package_count_ >= ADD_IP_SETTING_LIST_CONDITION){

						if(!insert_into_ip_setting_resp_list(tmp_equipment_ptr)){

							PostMessage(__application_environment.main_window_, WM_COMMAND, 
										ID_START_BUTTON,NULL);
							return false;
						}

						tmp_equipment_ptr->is_valid_ip_ = false;
					}
					
					tmp_equipment_ptr = tmp_equipment_ptr->left_;
				}
			}
			else if(route == ROUTE_RIGHT){

				tmp_equipment_ptr = equipment_ptr->right_;
				while(tmp_equipment_ptr != NULL){//设置其后的所有站计数器为0

					tmp_equipment_ptr->the_first_package_count_ = 0;
					tmp_equipment_ptr->ip_ = get_new_ip_inc(tmp_equipment_ptr->ip_, ROUTE_RIGHT);

					if(tmp_equipment_ptr->is_valid_ip_ ||  
						tmp_equipment_ptr->the_first_package_count_ >= ADD_IP_SETTING_LIST_CONDITION){

						if(!insert_into_ip_setting_resp_list(tmp_equipment_ptr)){

							PostMessage(__application_environment.main_window_, WM_COMMAND, 
										ID_START_BUTTON,NULL);
							return false;
						}

						tmp_equipment_ptr->is_valid_ip_ = false;
					}

					tmp_equipment_ptr = tmp_equipment_ptr->right_;
				}
			}

			//refresh line-array
			__line_array[line_no - 1].the_first_station_item_left_ = tmp_equipment_ptr2->left_;
			__line_array[line_no - 1].the_first_station_item_right_ = tmp_equipment_ptr2->right_;
		}
		else{//not find it

			WSASetLastError(EQUIPMENT_SERIAL_NO_FIND);
			output_log(IDS_STRING114, the_first_pack->serial_no_);
			return is_ok = false;
		}
	}		

	__serial_no_hash_map[the_first_pack->serial_no_] = equipment_ptr;
	++__work_items_size;
	
	return is_ok;
}

void the_first_package_proc()
{
	char recv_buf[MAX_INPUT_PACKAGE_SIZE] = {0};
	char error_buf[MAX_INPUT_PACKAGE_SIZE * 6] = {0};
	unsigned long ioctl_argp = 0L;
	int recv_size = 0;
	THE_FIRST_PACKAGE_INFO pack;
	EQUIPMENT_ITEM_PTR eqp_item_ptr = NULL;

#ifdef __MATRIX_DEBUG__
	char dbuf[MAX_STRING_SIZE * 4] = {0};
#endif //__MATRIX_DEBUG__

	while(true){

		if(ioctlsocket(__the_first_package_socket.socket_, FIONREAD, &ioctl_argp) == 0){

			if(ioctl_argp > 0){

				recv_size = recv(__the_first_package_socket.socket_, recv_buf, sizeof(recv_buf), 0);

				if(recv_size == SOCKET_ERROR)	break;
				else if(recv_size == 0x0){

					WSASetLastError(REMOTE_CLOSE_CONNECTION_GRACEFULLY);
					output_log(IDS_STRING119, __TEXT("!"));
					return;
				}
				else if(recv_size > MAX_INPUT_PACKAGE_SIZE){

					WSASetLastError(THE_FIRST_PACKAGE_RECV_SIZE_ERROR);
					recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
					output_log(IDS_STRING107, error_buf, recv_size);
					continue;
				}
				else{
#ifdef __MATRIX_DEBUG__
					output_log(IDS_STRING172, __FILE__, __FUNCTION__, __MATRIX_TEXT("接受到首包数据,开始检查首包有效性"), __MATRIX_TEXT("NULL"));
#endif //__MATRIX_DEBUG__

					//process the-first-package 
					SecureZeroMemory(&pack, sizeof(pack));
					if(!check_the_first_package((unsigned char*)recv_buf, recv_size, &pack)){

						WSASetLastError(THE_FIRST_PACKAGE_FORMAT_ERROR);
						recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));

						output_log(IDS_STRING108, error_buf, recv_size);
						continue;
					}

					__the_first_package_idled_counter = 0x0;

					//调用sn查询函数,判断是否存在
#ifdef __MATRIX_DEBUG__
					sprintf_s(dbuf, sizeof(dbuf), "SN: %X, route: %X, time: %X", pack.serial_no_, pack.route_, pack.time_);
					output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("通过SN查询当前设备"), dbuf);
#endif //__MATRIX_DEBUG__
					eqp_item_ptr = find_serial_no(pack.serial_no_);
					if(eqp_item_ptr != NULL){
						
						//判断首包时间是否相等
						if(eqp_item_ptr->the_first_package_time_ == pack.time_){

							eqp_item_ptr->the_first_package_count_ += THE_FIRST_PACKAGE_INC_COUNTER;
							//???
							if(eqp_item_ptr->the_first_package_count_ >= ADD_IP_SETTING_LIST_CONDITION){
								
#ifdef __MATRIX_DEBUG__
								sprintf_s(dbuf, sizeof(dbuf), "SN: %X, route: %X, time: %X", pack.serial_no_, pack.route_, pack.time_);
								output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("添加数据包到IP设置队列中"), dbuf);
#endif //__MATRIX_DEBUG__
								if(!insert_into_ip_setting_resp_list(eqp_item_ptr)){

									PostMessage(__application_environment.main_window_, WM_COMMAND, 
												ID_START_BUTTON,NULL);
									return;
								}
							}
						}
						else{//不相等

							WSASetLastError(THE_FIRST_PACKAGE_TIME_ERROR);
							recv_size = bin_to_ansi((unsigned char*)recv_buf, recv_size, error_buf, sizeof(error_buf));
							output_log(IDS_STRING112, error_buf, recv_size);
#ifdef __MATRIX_DEBUG__
							sprintf_s(dbuf, sizeof(dbuf), "SN: %X, route: %X, time: %X", pack.serial_no_, pack.route_, pack.time_);
							output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("删除SN和设备"), dbuf);
#endif //__MATRIX_DEBUG__
							delete_serial_no(pack.serial_no_);
							delete_equipment(eqp_item_ptr);

							continue;
						}						
					}
					else{
						if(create_new_equipment(&pack))//successful
							output_log(IDS_STRING110, pack.serial_no_);
						else //have a error adding a new equipment 
							output_log(IDS_STRING111, pack.serial_no_);
					}
				}
			}
			else{
				if(__the_first_package_idled_counter <= THE_FIRST_PACKAGE_OPTR_COMPELETED_COUNTER){
					++__the_first_package_idled_counter;
#ifdef __MATRIX_DEBUG__
					sprintf_s(dbuf, sizeof(dbuf), "首包闲置计数器加1为 %X ", __the_first_package_idled_counter);
					output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("首包处理定时器运行"), dbuf);
#endif //__MATRIX_DEBUG__
				}
				break;
			}
		}
		else	break;
	}
}


