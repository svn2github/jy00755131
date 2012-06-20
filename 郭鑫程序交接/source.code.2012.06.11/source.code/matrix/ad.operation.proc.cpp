/*
*	描述:	该文件实现采集设备的AD操作基本功能
*	文件:	ad.operation.proc.cpp
*	注释:	具体声明和调用见ad.operation.proc.h
*			该部分实现方法是逐个调用AD命令数组中的每一个命令,根据等待时间来唤醒TIMER, 
*			其中将针对每一条大线逐个执行,直到所有大线都执行完成.
*	版本:	1.0.2.30.2011.7.29.BASE
*	完成人:	kuoxin
*	参考文献:	
*	修改:	
*			1.0.8.42.2011.8.18	修改proc_ad_operation 程序流程,以前流程难于理解
*/

#include "../common/config.h"
#include "../common/typedef.h"
#include "../common/sockimpl.h"
#include "matrix.h"
#include "common.functions.h"
#include "equipment.comm.order.h"
#include "ad.operation.resp.h"

#define __MATRIX_DEBUG__

enum
{
	MAX_AD_OPERATION_EQUIPMENTS_SIZE = 0x100,
};

//定义该结构主要保存大线A,B上交叉站后的第一个采集站
typedef struct __tagAD_OPERATION_EQUIPMENTS
{
	EQUIPMENT_ITEM_PTR	equipment_ptr_;
	int					route_;
}AD_OPERATION_EQUIPMENTS, *AD_OPERATION_EQUIPMENTS_PTR;

//extern global variable
extern stdext_hash_map __route_hash_map;
extern stdext_hash_map __ip_hash_map;
extern APPLICATION_ENVIRONMENT	__application_environment;
extern THE_HEART_BEAT_PACKAGE_INFO __heartbeat_data;
extern LING_ARRAY_ITEM __line_array[MAX_BIG_LINE_SIZE]; //大线数组，标志每条大线上设备

//local global variable
unsigned int __current_command_index;//指明当前AD设置命令在命令集合中的下标
unsigned int __current_ad_operation_equipments_array_index; //当前__ad_operation_equipments_array的下标索引
unsigned int __current_ad_operation_equipments_array_size; //当前__ad_operation_equipments_array大小
bool __be_ad_operation_proc_completed; //整个AD设置是否完成
AD_OPERATION_EQUIPMENTS __ad_operation_equipments_array[MAX_AD_OPERATION_EQUIPMENTS_SIZE]; // 存放AD设置大线A,B的第一个采集站

bool init_ad_operation_proc()
{
	bool is_ok = true;
	int i = 0;
	int size = sizeof(__line_array) / sizeof(__line_array[0]);
	AD_OPERATION_EQUIPMENTS_PTR ptr = NULL;

	SecureZeroMemory(__ad_operation_equipments_array, sizeof(__ad_operation_equipments_array));
	__current_command_index = 0;
	__current_ad_operation_equipments_array_index = 0;
	__current_ad_operation_equipments_array_size = 0;
	__be_ad_operation_proc_completed = false;

	for(i = 0; i < size; ++i){
		
		ptr = __ad_operation_equipments_array + __current_ad_operation_equipments_array_size;
		ptr->equipment_ptr_ = __line_array[i].the_first_station_item_left_;
		if(ptr->equipment_ptr_){	ptr->route_ = ROUTE_LEFT;	++__current_ad_operation_equipments_array_size;}

		ptr = __ad_operation_equipments_array + __current_ad_operation_equipments_array_size;
		ptr->equipment_ptr_ = __line_array[i].the_first_station_item_right_;
		if(ptr->equipment_ptr_){	ptr->route_ = ROUTE_RIGHT;	++__current_ad_operation_equipments_array_size;}
	}

	size = sizeof(__ad_operation_equipments_array)/sizeof(__ad_operation_equipments_array[0]);
	if(__current_ad_operation_equipments_array_size > size)	{
		output_log(IDS_STRING178, size, __current_ad_operation_equipments_array_size);	
		return is_ok = false;
	}

	init_ad_operation_resp();

	return is_ok;
}

void release_ad_operation_proc(){	release_ad_operation_resp();}

bool proc_ad_operation(AD_COMMAND_ITEM_PTR ad_commands, unsigned int ad_commands_size)
{
	int route = 0x0;
	EQUIPMENT_ITEM_PTR equipment_ptr = NULL;
#ifdef __MATRIX_DEBUG__
	char dbuf[MAX_STRING_SIZE * 4] = {0};
#endif //__MATRIX_DEBUG__

	if(__current_ad_operation_equipments_array_index < __current_ad_operation_equipments_array_size){

		if(__current_command_index < ad_commands_size){

#ifdef __MATRIX_DEBUG__
			sprintf_s(dbuf, sizeof(dbuf), __MATRIX_TEXT("ln-idx %X; ln-sz %X; cmd-idx %X; cmd-sz %X"), 
					__current_ad_operation_equipments_array_index, __current_ad_operation_equipments_array_size,
					__current_command_index, ad_commands_size);
			output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("AD操作中当前命令索引小于AD命令总数"), dbuf);
#endif //__MATRIX_DEBUG__
			if(is_ad_operation_running()){

#ifdef __MATRIX_DEBUG__
				output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("AD命令操作正在在运行中"), __MATRIX_TEXT("NULL"));
#endif //__MATRIX_DEBUG__
				if(be_ad_operation_completed()){

#ifdef __MATRIX_DEBUG__
					output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("AD命令操作已经完成,将关闭应答TIMER"), __MATRIX_TEXT("NULL"));
#endif //__MATRIX_DEBUG__
					stop_ad_operation();
					if(++__current_command_index == ad_commands_size) goto GOTO;

					equipment_ptr = __ad_operation_equipments_array[__current_ad_operation_equipments_array_index].equipment_ptr_;
					route = __ad_operation_equipments_array[__current_ad_operation_equipments_array_index].route_ & ROUTE_DIRECTION_VALUE_MASK;
					while(equipment_ptr){

						if(equipment_ptr->is_valid_equipment_)
							insert_into_ad_operation_list(equipment_ptr, (ad_commands + __current_command_index)->function_ptr_, &__heartbeat_data);
						else
							output_log(IDS_STRING176, equipment_ptr->serial_no_, equipment_ptr->ip_);

						equipment_ptr = get_next_fdu_equipment_ptr(equipment_ptr, route);
					}

#ifdef __MATRIX_DEBUG__
					sprintf_s(dbuf, sizeof(dbuf), __MATRIX_TEXT("AD-CMD index %X; elapsed-time %X"), __current_command_index, 
						(ad_commands+__current_command_index)->elapsed_time_);
					output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("打开AD命令操作应答TIMER"), dbuf);
#endif //__MATRIX_DEBUG__
					start_ad_operation((ad_commands + __current_command_index)->elapsed_time_);
				}
#ifdef __MATRIX_DEBUG__
				else{
					output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("当前AD命令操作还在进行中"), __MATRIX_TEXT("NULL"));
				}
#endif //__MATRIX_DEBUG__
			}
			else{

				stop_ad_operation();
				equipment_ptr = __ad_operation_equipments_array[__current_ad_operation_equipments_array_index].equipment_ptr_;
				route = __ad_operation_equipments_array[__current_ad_operation_equipments_array_index].route_ & ROUTE_DIRECTION_VALUE_MASK;
				while(equipment_ptr){

					if(equipment_ptr->is_valid_equipment_)
						insert_into_ad_operation_list(equipment_ptr, (ad_commands+__current_command_index)->function_ptr_, &__heartbeat_data);
					else
						output_log(IDS_STRING176, equipment_ptr->serial_no_, equipment_ptr->ip_);

					equipment_ptr = get_next_fdu_equipment_ptr(equipment_ptr, route);
				}
#ifdef __MATRIX_DEBUG__
				output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("将该大线中的所有设备都加入到AD操作应答队列中"), __MATRIX_TEXT("NULL"));
				sprintf_s(dbuf, sizeof(dbuf), __MATRIX_TEXT("AD-CMD index %X; elapsed-time %X"), __current_command_index, 
					(ad_commands+__current_command_index)->elapsed_time_);
				output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("打开AD命令操作应答TIMER"), dbuf);
#endif //__MATRIX_DEBUG__			
				start_ad_operation((ad_commands+__current_command_index)->elapsed_time_);
			}
		}
		else{
GOTO:
#ifdef __MATRIX_DEBUG__
			sprintf_s(dbuf, sizeof(dbuf), __MATRIX_TEXT("line-index %X"), __current_ad_operation_equipments_array_index);
			output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("所有AD命令操作在大线上运行完成"), __MATRIX_TEXT("NULL"));
#endif //__MATRIX_DEBUG__
			++__current_ad_operation_equipments_array_index;
			__current_command_index = 0;
		}
	}
	else{ 
		output_log(IDS_STRING173, __FILE__, __FUNCTION__, __MATRIX_TEXT("所有AD命令操作在所有大线上运行完成"), __MATRIX_TEXT("NULL"));
		__be_ad_operation_proc_completed = true;
	}

	return __be_ad_operation_proc_completed;
}

