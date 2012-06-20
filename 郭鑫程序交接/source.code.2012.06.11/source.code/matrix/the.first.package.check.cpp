/**
*	描述：该文件实现定时器定时遍历大线进行退化操作, 定时为CHECK_THE_FIRST_PACKAGE_TIMEOUT_VALUE
*	文件：the.first.package.check.cpp
*	注释：通过主程序在定时器中调用,定时更新大线内IP地址无效的设备,请它们删除
*	参考文献：
*	版本：1.0.5.37.2011.8.12
*	注释：
*	修改：
*		1.0.7.39.2011.8.12	修改check_the_first_package中的错误
*		
*/

#include "..\common\config.h"
#include "..\common\typedef.h"
#include "common.functions.h"
#include "the.first.package.check.h"
#include "matrix.h"

extern LING_ARRAY_ITEM __line_array[MAX_BIG_LINE_SIZE]; //大线数组，标志每条大线上设备

/*
	支持退化处理,如果发现采集站和电源站的首包计数器小于0时,将这个设备删除
*/
void check_the_first_package()
{
	int i = 0;
	EQUIPMENT_ITEM_PTR equipment_ptr = NULL;

#ifdef __MATRIX_DEBUG__
	char dbuf[MAX_STRING_SIZE * 4] = {0};
	output_log(IDS_STRING172, __FILE__, __LINE__, __MATRIX_TEXT("退化处理,如果发现采集站和电源站的首包计数器小于0时,将这个设备删除"));
#endif //__MATRIX_DEBUG__

	for(i = 0; i < MAX_BIG_LINE_SIZE; ++i){
		
		equipment_ptr = __line_array[i].the_first_station_item_left_;
		while(equipment_ptr){

			if(!equipment_ptr->is_valid_ip_){

				--equipment_ptr->the_first_package_count_;
				if(equipment_ptr->the_first_package_count_ < 0){
#ifdef __MATRIX_DEBUG__
					sprintf_s(dbuf, sizeof(dbuf), "将删除在第%d大线A上的SN:%X的设备", i, equipment_ptr->serial_no_);
					output_log(IDS_STRING172, __FILE__, __LINE__, dbuf);
#endif //__MATRIX_DEBUG__
					delete_serial_no(equipment_ptr->serial_no_);
					equipment_ptr =  delete_equipment(equipment_ptr);
					continue;
				}
			}

			equipment_ptr = get_next_fdu_equipment_ptr(equipment_ptr, ROUTE_LEFT);
		}

		equipment_ptr = __line_array[i].the_first_station_item_right_;
		while(equipment_ptr){

			if(!equipment_ptr->is_valid_ip_){

				--equipment_ptr->the_first_package_count_;
				if(equipment_ptr->the_first_package_count_ < 0){

#ifdef __MATRIX_DEBUG__
					sprintf_s(dbuf, sizeof(dbuf), "将删除在第%d大线B上的SN:%X的设备", i, equipment_ptr->serial_no_);
					output_log(IDS_STRING172, __FILE__, __LINE__, dbuf);
#endif //__MATRIX_DEBUG__

					delete_serial_no(equipment_ptr->serial_no_);
					equipment_ptr =  delete_equipment(equipment_ptr);
					continue;
				}
			}

			equipment_ptr = get_next_fdu_equipment_ptr(equipment_ptr, ROUTE_RIGHT);
		}
	}
}

