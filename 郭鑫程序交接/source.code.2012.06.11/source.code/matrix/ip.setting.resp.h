#ifndef __MATRIX_IP_SETTING_RESP_PROC_H__
#define __MATRIX_IP_SETTING_RESP_PROC_H__

enum __enuIP_SETTING_RESP_ERROR
{
	IP_SETTING_RECV_SIZE_ERROR, //接受到得ip响应数据包大小不正确
	IP_SETTING_FORMAT_ERROR,	//检测该ip应答包为非法数据包
	IP_SETTING_SEND_ERROR,		//发送ip设置数据包失败，将其保留在ip设置响应队列中，等待下次发送
};

bool init_ip_setting_resp();
void proc_ip_setting_resp();
void release_ip_setting_resp();

#endif//__MATRIX_IP_SETTING_RESP_PROC_H__