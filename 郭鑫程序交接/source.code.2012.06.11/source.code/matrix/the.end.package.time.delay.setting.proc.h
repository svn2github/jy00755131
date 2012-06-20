#ifndef __MATRIX_THE_END_PACKAGE_TIME_DELAY_SETTING_PROC_H__
#define __MATRIX_THE_END_PACKAGE_TIME_DELAY_SETTING_PROC_H__

//首包设置ip地址响应队列数据结构
typedef struct __tagTIME_DELAY_SETTING_INFO
{
	EQUIPMENT_ITEM_PTR equipment_item_;
	unsigned char pack_[MAX_OUTPUT_PACKAGE_SIZE]; //发送数据包内容
	__tagTIME_DELAY_SETTING_INFO* next_;
}TIME_DELAY_SETTING_INFO, *TIME_DELAY_SETTING_INFO_PTR;

//设置时延
typedef struct __tagTHE_SET_DELAY_PACKAGE_INFO                
{
	unsigned ip_;
	unsigned cmd_word_;
	unsigned modify_time_high_;
	unsigned modify_time_low_;
}THE_SET_DELAY_PACKAGE_INFO,*THE_SET_DELAY_PACKAGE_INFO_PTR;

bool init_the_end_package_time_delay_setting_proc();
void release_the_end_package_time_delay_setting_proc();
bool proc_the_tail_package_time_delay_setting();
bool insert_into_time_delay_setting_resp_list(EQUIPMENT_ITEM_PTR equipment_ptr);

#endif//__MATRIX_THE_END_PACKAGE_TIME_DELAY_SETTING_PROC_H__