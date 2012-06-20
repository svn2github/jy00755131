#ifndef __MATRIX_THE_END_PACKAGE_TIME_DELAY_PROC_H__
#define __MATRIX_THE_END_PACKAGE_TIME_DELAY_PROC_H__

//查询尾包时刻
typedef struct __tagTHE_REQUIRE_DELAY_PACKAGE_INFO
{
	unsigned int cmd_word_;
	unsigned int ip_;
	unsigned int send_time_;
	unsigned int rec_time_;
	unsigned int require_delay_cmd_;   //尾包时刻请求命令字
}THE_REQUIRE_DELAY_PACKAGE_INFO, *THE_REQUIRE_DELAY_PACKAGE_INFO_PTR;

bool init_the_end_package_time_delay_proc(TCHAR const*);
BOOL on_receive_the_end_time_delay_package(WPARAM, LPARAM);
void release_the_end_package_time_delay_proc();
bool proc_the_end_package_time_delay_resp(EQUIPMENT_ITEM_PTR equipment_ptr/*发送尾包的设备信息*/, int route);

#endif//__MATRIX_THE_END_PACKAGE_TIME_DELAY_PROC_H__