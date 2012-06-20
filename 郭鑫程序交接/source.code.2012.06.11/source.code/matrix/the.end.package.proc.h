#ifndef __MATRIX_THE_END_PACKAGE_PROC_H__
#define __MATRIX_THE_END_PACKAGE_PROC_H__


typedef struct __tagTHE_END_PACKAGE_INFO
{
	unsigned int cmd_word_;
	unsigned int ip_;
	unsigned int serial_no_;	
	unsigned int system_time_;
	unsigned int route_;
	unsigned int type_;
}THE_END_PACKAGE_INFO, *THE_END_PACKAGE_INFO_PTR;

typedef struct __tagEQUIPMENT_DELETION_ITEM
{
	EQUIPMENT_ITEM_PTR equimpment_ptr_;
	__tagEQUIPMENT_DELETION_ITEM* next_;
}EQUIPMENT_DELETION_ITEM, *EQUIPMENT_DELETION_ITEM_PTR;

bool init_the_end_package_proc(TCHAR const* remote_ip);
BOOL on_receive_the_end_package(WPARAM, LPARAM);
void release_the_end_package_proc();
void send_the_tail_package_requirement_command(EQUIPMENT_ITEM_PTR laux_ptr, EQUIPMENT_ITEM_PTR fdu_ptr, int route);

EQUIPMENT_ITEM_PTR get_master_laux_ptr();

bool is_starting_the_end_package(EQUIPMENT_ITEM_PTR equipment_ptr, int route);
bool is_starting_the_end_package_time_delay(EQUIPMENT_ITEM_PTR equipment_ptr, int route);
bool is_starting_the_end_package_time_delay_setting();
bool is_stoppping_the_end_package();


void start_the_end_package_time_delay(EQUIPMENT_ITEM_PTR equipment_ptr, int route);
void start_the_end_package(EQUIPMENT_ITEM_PTR equipment_ptr, int route);
void start_the_end_package_time_delay_setting();

void stop_the_end_package();


#endif//__MATRIX_THE_FIRST_PACKAGE_PROC_H__