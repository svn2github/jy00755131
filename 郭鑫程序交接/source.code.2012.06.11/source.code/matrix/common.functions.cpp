/*
*	描述：该文件主要实现了通用函数,被多个模块说调用
*	文件：common.functions.cpp
*	注释：
*	参考文献：
*	版本：1.0.5.37.2011.8.12
*	注释：
*	修改：
*		1.0.7.39.2011.8.12	修改delete_equipment中判断错误,加入返回值为下一个设备的指针
*/

#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/code.conv.h"
#include "common.functions.h"
#include "equipment.comm.order.h"
#include "matrix.h"

extern int __work_items_size;	//使用设备个数
extern EQUIPMENT_ITEM_PTR __free_items; //没有使用设备
extern EQUIPMENT_ITEM_PTR __work_items;
extern int __work_items_size;	//使用设备个数
extern ip_setting_list __ip_setting_list; //ip响应链表，该结构为实际操作结构
extern IP_SETTING_INFO_PTR __free_ip_setting_list;

extern stdext_hash_map __ip_hash_map;
extern stdext_hash_map __serial_no_hash_map;
extern stdext_hash_map __route_hash_map;

extern LING_ARRAY_ITEM __line_array[MAX_BIG_LINE_SIZE]; //大线数组，标志每条大线上设备

unsigned long volatile __line_no_;//标记当前分配的大线数量

extern APPLICATION_ENVIRONMENT __application_environment;

bool delete_serial_no(int serial_no)
{
	bool is_ok = false;
	if(__serial_no_hash_map.erase(serial_no) > 0) is_ok = true;
	return is_ok;
}

bool insert_into_ip_setting_resp_list(EQUIPMENT_ITEM_PTR equipment_ptr)
{
	bool is_ok = true;
	IP_SETTING_INFO_PTR ip_setting_ptr = NULL;

	if(__free_ip_setting_list == NULL){

		output_log(IDS_STRING145, 0x0);
		return false;
	}

	ip_setting_ptr = __free_ip_setting_list;
	__free_ip_setting_list = __free_ip_setting_list->next_;

	ip_setting_ptr->equipment_item_ = equipment_ptr;
	ip_setting_ptr->next_ = NULL;

	make_ip_setting_package(ip_setting_ptr->equipment_item_, ip_setting_ptr->pack_, sizeof(ip_setting_ptr->pack_));
	__ip_setting_list.push_back(ip_setting_ptr);

#ifdef __MATRIX_DEBUG__
	char dbuf[MAX_STRING_SIZE * 4] = {0};
	int dsize = 0;
	dsize = bin_to_ansi(ip_setting_ptr->pack_, sizeof(ip_setting_ptr->pack_), dbuf, sizeof(dbuf));
	dbuf[dsize] = 0x0;
	output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("添加数据包到IP设置队列中"), dbuf);
#endif //__MATRIX_DEBUG__

	return is_ok;
}

//该函数如果是删除交叉站的话,返回值将是空
EQUIPMENT_ITEM_PTR delete_equipment(EQUIPMENT_ITEM_PTR equipment_item_ptr)
{
	int route = 0x0;
	int line_no = 0x0;
	EQUIPMENT_ITEM_PTR tmp_equipment_ptr = NULL;
	EQUIPMENT_ITEM_PTR ret_equipment_ptr = NULL;
	stdext_hash_map_iterator itr;

	//删除该设备ip地址
	if(equipment_item_ptr->is_valid_ip_)	__ip_hash_map.erase(equipment_item_ptr->ip_);

	//according to route and refresh the others
	if(equipment_item_ptr->type_ == FDU || equipment_item_ptr->type_ == LAUL){

		route = equipment_item_ptr->route_[0] & ROUTE_DIRECTION_VALUE_MASK;
		line_no = equipment_item_ptr->route_[0] / ROUTE_BIG_LINE_BASE_NUMBER;

		itr = __route_hash_map.find(equipment_item_ptr->route_[0]);
		if(itr == __route_hash_map.end()){
			output_log(IDS_STRING122, equipment_item_ptr->serial_no_, equipment_item_ptr->route_[0]);
			PostMessage(__application_environment.main_window_, WM_COMMAND,	ID_START_BUTTON,NULL);
			return NULL;			
		}

		if(!(route == ROUTE_LEFT || route == ROUTE_RIGHT)){
			output_log(IDS_STRING120, equipment_item_ptr->serial_no_, equipment_item_ptr->route_[0]);
			PostMessage(__application_environment.main_window_, WM_COMMAND, ID_START_BUTTON, NULL);
			return NULL;
		}

		ret_equipment_ptr = tmp_equipment_ptr = get_next_fdu_equipment_ptr(equipment_item_ptr, route);
		while(tmp_equipment_ptr){

			//delete ip index
			__ip_hash_map.erase(tmp_equipment_ptr->ip_);
			tmp_equipment_ptr->ip_ = get_new_ip_dec(tmp_equipment_ptr->ip_);

			if(tmp_equipment_ptr->is_valid_ip_ || 
				tmp_equipment_ptr->the_first_package_count_ >= ADD_IP_SETTING_LIST_CONDITION){

					if(!insert_into_ip_setting_resp_list(tmp_equipment_ptr)){

						PostMessage(__application_environment.main_window_, WM_COMMAND, 
							ID_START_BUTTON,NULL);
						return NULL;
					}
			}

			tmp_equipment_ptr->is_valid_ip_ = false;
			tmp_equipment_ptr = get_next_fdu_equipment_ptr(tmp_equipment_ptr, route); 
		}

		if(equipment_item_ptr->left_)	equipment_item_ptr->left_->right_ = equipment_item_ptr->right_;
		if(equipment_item_ptr->right_)	equipment_item_ptr->right_->left_ = equipment_item_ptr->left_;

		//更新大线队列
		__line_array[line_no - 1].the_first_station_item_left_ = itr->second->left_;
		__line_array[line_no - 1].the_first_station_item_right_ = itr->second->right_;
	}
	else{//LAUX, MASTER_LAUX

		if(equipment_item_ptr->up_)		equipment_item_ptr->up_->down_ = equipment_item_ptr->down_;
		if(equipment_item_ptr->down_)	equipment_item_ptr->down_->up_ = equipment_item_ptr->up_;

		//更新大线队列
		line_no = equipment_item_ptr->route_[BIG_LINE_A_INDEX] / ROUTE_BIG_LINE_BASE_NUMBER;//对于route取2这个值其实就是指大线A
		SecureZeroMemory(__line_array + line_no - 1, sizeof(__line_array[0]));
		--__line_no_;
	}

	//delete this equipment
	--__work_items_size;
	SecureZeroMemory(equipment_item_ptr, sizeof(EQUIPMENT_ITEM));
	equipment_item_ptr->right_= __free_items;
	__free_items->left_ = equipment_item_ptr;
	__free_items = equipment_item_ptr;	

	return ret_equipment_ptr;
}

unsigned long get_new_ip_inc(unsigned long ip, int route)
{
	unsigned new_ip = ntohl(ip);
	unsigned low_char = new_ip & 0xFF;
	unsigned low_char2 = new_ip & 0x3F00;

	if(++low_char >= 0xFE){

		low_char -= 0xFE;
		low_char2 += 0x100;
	}

	new_ip &= 0xFFFF0000;
	new_ip += (route == ROUTE_RIGHT ? 0xC000 : 0x0);
	new_ip += (low_char2 + low_char);
	new_ip = htonl(new_ip);
	return new_ip;
}

unsigned long get_new_ip_dec(unsigned long ip)
{
	unsigned new_ip = ntohl(ip);
	unsigned low_char = new_ip & 0xFF;
	unsigned low_char2 = new_ip & 0x3F00;

	if(--low_char <= 0x0){

		low_char += 0xFE;
		low_char2 -= 0x100;
	}

	new_ip &= 0xFFFFC000;
	new_ip += (low_char2 + low_char); 	
	new_ip = htonl(new_ip);
	return new_ip;
}

EQUIPMENT_ITEM_PTR find_serial_no(int serial_no)
{
	EQUIPMENT_ITEM_PTR ptr_ret = NULL;
	stdext_hash_map_iterator itr;

	itr = __serial_no_hash_map.find(serial_no);
	if(itr != __serial_no_hash_map.end())	ptr_ret = itr->second;

	return ptr_ret;
}

EQUIPMENT_ITEM_PTR find_ip(int ip)
{
	EQUIPMENT_ITEM_PTR ptr_ret = NULL;
	stdext_hash_map_iterator itr;

	itr = __ip_hash_map.find(ip);
	if(itr != __ip_hash_map.end())	ptr_ret = itr->second;

	return ptr_ret;
}

EQUIPMENT_ITEM_PTR get_prev_laux_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr/*必须是交叉站*/, int route)
{	
	EQUIPMENT_ITEM_PTR ret_equipment_ptr = NULL;

	if(equipment_ptr != NULL){
		if(equipment_ptr->type_ != SN_MAIN_LAUX){

			if(route == ROUTE_UP)			ret_equipment_ptr = equipment_ptr->down_;
			else if(route == ROUTE_DOWN)	ret_equipment_ptr = equipment_ptr->up_;
		}
	}

	return ret_equipment_ptr;
}

EQUIPMENT_ITEM_PTR get_next_laux_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr/*必须是交叉站*/, int route)
{	
	EQUIPMENT_ITEM_PTR ret_equipment_ptr = NULL;

	if(equipment_ptr != NULL){
		if(equipment_ptr->type_ != SN_MAIN_LAUX){

			if(route == ROUTE_UP)			ret_equipment_ptr = equipment_ptr->up_;
			else if(route == ROUTE_DOWN)	ret_equipment_ptr = equipment_ptr->down_;
		}
	}

	return ret_equipment_ptr;
}

EQUIPMENT_ITEM_PTR get_next_fdu_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr, int route)
{
	EQUIPMENT_ITEM_PTR ret_equipment_ptr = NULL;

	if(equipment_ptr != NULL){

		if(route == ROUTE_LEFT)			ret_equipment_ptr = equipment_ptr->left_;
		else if(route == ROUTE_RIGHT)	ret_equipment_ptr = equipment_ptr->right_;

		if(ret_equipment_ptr != NULL)
			if(ret_equipment_ptr->type_ == LAUX || ret_equipment_ptr->type_ == SN_MAIN_LAUX) 
				ret_equipment_ptr = NULL;
	}

	return ret_equipment_ptr;
}

EQUIPMENT_ITEM_PTR get_prev_fdu_equipment_ptr(EQUIPMENT_ITEM_PTR equipment_ptr, int route)
{
	EQUIPMENT_ITEM_PTR ret_equipment_ptr = NULL;

	if(equipment_ptr != NULL){

		if(equipment_ptr->type_ == LAUX || equipment_ptr->type_ == SN_MAIN_LAUX){ ret_equipment_ptr = NULL;	}
		else{

			if(route == ROUTE_LEFT)			ret_equipment_ptr = equipment_ptr->right_;
			else if(route == ROUTE_RIGHT)	ret_equipment_ptr = equipment_ptr->left_;

			if(ret_equipment_ptr != NULL)
				if(ret_equipment_ptr->type_ == LAUX || ret_equipment_ptr->type_ == SN_MAIN_LAUX) 
					ret_equipment_ptr = NULL;
		}
	}

	return ret_equipment_ptr;
}

//更新测线数组,主要是保存尾包数据到__line_array该结构中,注意是一个数组和下标的使用
void refresh_lines_array(int route/*当前测线的路由信息*/)
{
	if(route <= 0){	output_log(IDS_STRING153, route); return; }

	bool is_ok = true;
	int route_direction = route & ROUTE_DIRECTION_VALUE_MASK;
	unsigned int line_no = route / ROUTE_BIG_LINE_BASE_NUMBER;
	EQUIPMENT_ITEM_PTR	laux_ptr = NULL, 
						fdu_ptr = NULL,
						fdu_ptr2 = NULL;
	stdext_hash_map_iterator itr;

	if(line_no <= 0){	output_log(IDS_STRING153, route); return; }

	itr = __route_hash_map.find(route);
	laux_ptr = itr->second;
	fdu_ptr = get_next_fdu_equipment_ptr(laux_ptr, route_direction);
	if(fdu_ptr){

		if(route_direction == ROUTE_LEFT)	__line_array[line_no - 1].the_first_station_item_left_ = fdu_ptr;
		else								__line_array[line_no - 1].the_first_station_item_right_ = fdu_ptr; 

		while(fdu_ptr){
			if(!fdu_ptr->is_valid_equipment_)	is_ok = false;
			fdu_ptr2 = fdu_ptr;
			fdu_ptr = get_next_fdu_equipment_ptr(fdu_ptr, route_direction);
		}

		if(route_direction == ROUTE_LEFT){
			__line_array[line_no - 1].be_the_left_line_time_delay_setting_completed_ = is_ok;
			__line_array[line_no - 1].the_end_station_item_left_ = fdu_ptr2;
		}
		else{
			__line_array[line_no - 1].be_the_right_line_time_delay_setting_completed_ = is_ok;
			__line_array[line_no - 1].the_end_station_item_right_ = fdu_ptr2;
		}		
	}
}