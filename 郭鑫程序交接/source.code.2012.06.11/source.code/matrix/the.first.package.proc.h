#ifndef __MATRIX_THE_FIRST_PACKAGE_PROC_H__
#define __MATRIX_THE_FIRST_PACKAGE_PROC_H__

enum __enuTHE_FIRST_PACKAGE_PROC_IMPL_ERROR
{
	THE_FIRST_PACKAGE_RECV_SIZE_ERROR,	//接受到首包大小大于规定大小128
	THE_FIRST_PACKAGE_FORMAT_ERROR,		//检测该首包为非法数据包
	THE_FIRST_PACKAGE_TIME_ERROR,		//接受首包时刻与原首包时刻不相等
	FREE_EQUIPMENT_ARRAY_IS_NULL,		//没有可用设备数据存储内存可用，当前大小为：%d
	EQUIPMENT_SERIAL_NO_FIND,			//在索引表中没有发现与该串号相对应的信息
	IP_SETTING_RESP_FULL,				//ip设置应答队列已满，不能分配，等待下次再分配
};

typedef struct __tagTHE_FIRST_PACKAGE_INFO
{
	int serial_no_;
	int ip_;
	int time_;
	int deletion_sign_;
	int wait_count_;
	int ip_count_;
	int route_;
	int type_;
	int cmd_word_;
}THE_FIRST_PACKAGE_INFO, *THE_FIRST_PACKAGE_INFO_PTR;

bool init_the_first_package_proc(TCHAR const* remote_ip);
void the_first_package_proc();
void release_the_first_package_proc();

#endif//__MATRIX_THE_FIRST_PACKAGE_PROC_H__