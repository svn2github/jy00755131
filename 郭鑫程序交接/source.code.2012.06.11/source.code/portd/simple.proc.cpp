/*@
*	描述：该文件为log基本操作类的实现文件
*	文件：parallel.port.impl.cpp
*	注释：具体声明和调用见parallel.port.impl.h
*	参考文献：ADLINK PCIS-DASK 文档
*	版本：1.0.0.0
*	注释：
*	修改：
*		1.2.13.154.2011.05.19	加入根据并口接收数据包内的端口为发送端口功能
*		1.2.15.161.2011.06.15	加入根据并口接收数据包内的端口和ip信息分发功能，同时提供是否使用该功能设置选项
*		1.2.16.165.2011.06.20	在Parallel_thread_callback中，调整过滤和分发
*
*		1.2.18.175.2011.06.22	修改Parallel_thread_callback的第一个传入参数由const unsigned char* 变为 unsigned char*,
*								这样便于将调试流水号写入报文中
*								接收并口卡数据包，发送修改后的带流水号报文
*								
*	
*/

#include "../common/config.h"
#include "../common/utility.h"
#include "../common/code.conv.h"
#include "pcid.h"

#include "sockimpl.h"
#include "parallel.port.impl.h"
#include "filter.optr.h"
#include "output.queue.h"
//#include <vector>
//#include <list>

SIMPLE_PROC_ENVIRONMENT __simple_proc_env;

FILE* __recv_file = NULL;
FILE* __snd_file = NULL;

unsigned long __sn = 0x0;

//std::vector<ptRECV_PAK> __recv_pak_vector;
//std::list<ptRECV_PAK> __recv_pak_list;

bool push_parallel_queue(const unsigned char* buf)
{
	return parallel_output_queue_push((PARALLEL_OUTPUT_QUEUE_ITEM_PTR)buf);
}

bool udp_recv_callback_impl(void* args, const char* buf, unsigned buf_size)
{
	SYSTEMTIME systm = {0};
	SYSTEMTIME systm2 = {0};
	char bufxx[1024] = {0};
	int bufxx_size = 0;
	SIMPLE_PROC_ENVIRONMENT_PTR env = (SIMPLE_PROC_ENVIRONMENT_PTR)args;

	if(env->application_environment_->be_all_info_outputted_){

		GetSystemTime(&systm);
		SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
		bufxx_size = bin_to_ansi((unsigned char*)buf, buf_size, bufxx, sizeof(bufxx));
		bufxx[bufxx_size] = 0x0;
		fprintf(__recv_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
	}

	_InterlockedIncrement(&env->application_environment_->upd_recv_num_);
	push_parallel_queue((const unsigned char*)buf);
	//parallel_thread_callback(buf, buf_size);
	return true;
}

unsigned int __stdcall socket_recv_thread_proc(void* arg)
{
	SIMPLE_PROC_ENVIRONMENT_PTR env = (SIMPLE_PROC_ENVIRONMENT_PTR)arg;

	unsigned int ret_code = 0;
	DWORD result = 0x0;

	while(true){

		//recv...
		udp_recv(udp_recv_callback_impl, env);

		//wait...
//		result = WaitForSingleObject(env->stop_event_, env->application_environment_->recv_thread_waiting_time_);
//		if(result == WAIT_OBJECT_0)
//			break;
	}

	_endthreadex(0);

	return ret_code;
}

unsigned parallel_thread_callback(unsigned char* buf, unsigned int buf_size)
{
	int size = 0, i = 0;
	SOCKADDR_IN ip;
	unsigned short crc16 = 0x0;
	LONG serial_no = 0x0, 
		prev_serial_no = 0x0,
		dispersion = 0x0;

	SYSTEMTIME systm = {0};
	SYSTEMTIME systm2 = {0};

	char bufxx[1024] = {0};
	char fdu_buf[256] = {0};
	char error_buf[MAX_STRING_SIZE * 20] = {0};
	int bufxx_size = 0;

	SecureZeroMemory(&ip, sizeof(ip));
	ip.sin_family = AF_INET;

	if((buf[24] == 0x0 && buf[25] == 0x0) &&
		(buf[16] == 0x0 && buf[17] == 0x0 && buf[18] == 0x0 && buf[19] == 0x0) &&
		(buf[20] == 0x0 && buf[21] == 0x0 && buf[22] == 0x0 && buf[23] == 0x0))
		return 0;

	_InterlockedIncrement(&__simple_proc_env.application_environment_->upd_recv_num_);

	//check crc
	if(__simple_proc_env.application_environment_->enable_to_check_crc_){

		memcpy(&crc16, buf + CRC_BEGIN_POS, sizeof(unsigned short));
		if(!check_crc(buf, buf_size - 4, crc16)){
			return size = 0;
		}
	}

	unsigned long source_addr = 0x0;
	unsigned long target_addr = 0x0;
	unsigned long data = 0x0;
	unsigned char cmd = 0x0;

	DEVICE_INFO_PTR di = NULL;

	memcpy(&source_addr, buf + 10, sizeof(unsigned long));
	memcpy(&target_addr, buf + 14, sizeof(unsigned long));
	cmd = buf[0x1c];
	
	memcpy(&data, buf + 0x1d, sizeof(unsigned long));

	if((source_addr == 0x0) && (target_addr == 0xffffffff) && cmd == 0x01){

		if((data & 0x03) == 0x03){

			di = new DEVICE_INFO;
			di->ip_ = 0x0;
			di->is_be_lci_device_ = false;
			di->is_be_virtual_fdu_ = false;
			di->sn_ = data;
			__simple_proc_env.application_environment_->device_info_list_->push_back(di);

			CopyMemory(fdu_buf, buf, buf_size); 
			//send virtual fdu
			__sn &= 0xFFF00000;
			__sn |= data;
			for(i = 0; i < __simple_proc_env.application_environment_->parallel_port_data_input_buffer_num_; ++i){

				__sn += (i * 0x100000);
				CopyMemory(fdu_buf + 0x1d, &__sn, sizeof(__sn));
				di = new DEVICE_INFO;
				di->ip_ = 0x0;
				di->is_be_lci_device_ = false;
				di->is_be_virtual_fdu_ = true;
				di->sn_ = __sn;
				__simple_proc_env.application_environment_->device_info_list_->push_back(di);

				if(__simple_proc_env.application_environment_->enable_to_transfer_data_){
					memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
					memcpy(&ip.sin_addr.s_addr, buf + DEST_IP_BEGIN_POS, sizeof(unsigned long));
					ip.sin_port = htons(ip.sin_port);
					size = udp_send((const char*)buf, buf_size, (sockaddr*)&ip);
				}
			}
		}
		else if((data & 0x01) == 0x01){

			di = new DEVICE_INFO;
			di->ip_ = 0x0;
			di->is_be_lci_device_ = true;
			di->is_be_virtual_fdu_ = false;
			di->sn_ = data;
			__simple_proc_env.application_environment_->device_info_list_->push_back(di);
		}

		if(__simple_proc_env.application_environment_->enable_to_transfer_data_){
			memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
			memcpy(&ip.sin_addr.s_addr, buf + DEST_IP_BEGIN_POS, sizeof(unsigned long));
			ip.sin_port = htons(ip.sin_port);
			size = udp_send((const char*)buf, buf_size, (sockaddr*)&ip);
		}
	}


	//get serial.no per-package
	//memcpy(&serial_no, buf + RECV_PACK_SERIAL_NO_POS, sizeof(char));

	//prev_serial_no = _InterlockedExchange(&__simple_proc_env.application_environment_->serial_no_, serial_no);
	//dispersion = serial_no - prev_serial_no;

	//if(dispersion != 1){
	//	if(dispersion < 0){

	//		dispersion += (RECV_PACK_MAX_SERIAL_NO + 1);
	//		if(dispersion != 1)	OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
	//	}
	//	else{
	//		OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
	//	}

	//	dispersion = interlock_add(&__simple_proc_env.application_environment_->udp_send_serial_no_, dispersion);
	//}
	//else{

	//	dispersion = _InterlockedIncrement(&__simple_proc_env.application_environment_->udp_send_serial_no_);
	//}

	////set udp serial no
	//memcpy(buf + SEND_PACK_SERIAL_NO_POS, &dispersion, SEND_PACK_SERIAL_NO_LENGTH);

	//filter it	
	//if(__simple_proc_env.application_environment_->enable_to_transfer_data_){

	//	if(filter_it(buf, buf_size)){

	//		memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
	//		ip.sin_port = htons(ip.sin_port);
	//		memcpy(&ip.sin_addr.s_addr, buf + DEST_IP_BEGIN_POS, sizeof(ULONG));
	//		
	//		size = udp_send((const char*)buf, buf_size, (SOCKADDR*)&ip);
	//		if(__simple_proc_env.application_environment_->be_all_info_outputted_){
	//			
	//			ip.sin_port = ntohs(ip.sin_port);
	//			if(__simple_proc_env.application_environment_->ignored_port_ != ip.sin_port){

	//				GetSystemTime(&systm);
	//				SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
	//				bufxx_size = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
	//				bufxx[bufxx_size] = 0x0;
	//				fprintf(__snd_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
	//			}
	//		}
	//	}
	//	else{

	//		memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
	//		memcpy(&ip.sin_addr.s_addr, buf + DEST_IP_BEGIN_POS, sizeof(ULONG));

	//		size = bin_to_ansi(buf, buf_size, error_buf, sizeof(error_buf));
	//		error_buf[size] = 0x0;
	//		OUTPUT_DEBUG_STRING(IDS_STRING165, inet_ntoa(ip.sin_addr), ip.sin_port, error_buf);

	//		ip.sin_port = htons(ip.sin_port);
	//		size = udp_send((const char*)buf, buf_size, (SOCKADDR*)&ip);

	//		if(__simple_proc_env.application_environment_->be_all_info_outputted_){

	//			ip.sin_port = ntohs(ip.sin_port);
	//			if(__simple_proc_env.application_environment_->ignored_port_ != ip.sin_port){

	//				GetSystemTime(&systm);
	//				SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
	//				bufxx_size = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
	//				bufxx[bufxx_size] = 0x0;
	//				fprintf(__snd_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
	//			}
	//		}
	//		return size = 0;
	//	}
	//}
	//else{
	//	size = udp_send((const char*)buf, buf_size);
	//	if(__simple_proc_env.application_environment_->be_all_info_outputted_){

	//		memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
	//		if(__simple_proc_env.application_environment_->ignored_port_ != ip.sin_port){

	//			GetSystemTime(&systm);
	//			SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
	//			bufxx_size = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
	//			bufxx[bufxx_size] = 0x0;
	//			fprintf(__snd_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
	//		}
	//	}
	//}

	if(static_cast<unsigned int>(size) == buf_size)
		_InterlockedIncrement(&__simple_proc_env.application_environment_->upd_send_num_);

	return size;
}

BOOL start_simple_proc(APPLICATION_ENVIRONMENT_PTR app_env)
{
	bool is_ok = false;

	if(app_env->be_all_info_outputted_){

		__recv_file = fopen("recv.log", "w");
		__snd_file = fopen("snd.log", "w");
	}

	__simple_proc_env.application_environment_ = app_env;
	InterlockedExchange(&__simple_proc_env.application_environment_->serial_no_, 0x0);

	is_ok = init_socket();
	if(!is_ok)	return FALSE;

	is_ok = init_parallel_port(app_env->parallel_port_data_input_buffer_num_,
								app_env->parallel_port_data_input_buffer_size_,								
								app_env->parallel_port_input_waiting_time_,
								app_env->parallel_port_output_buffer_size_);
	if(!is_ok)	return FALSE;

	is_ok = create_udp_socket(__simple_proc_env.application_environment_->listen_ip_,
								static_cast<unsigned short>(__simple_proc_env.application_environment_->listen_port_),
								__simple_proc_env.application_environment_->remote_ip_,
								static_cast<unsigned short>(__simple_proc_env.application_environment_->remote_port_),
								__simple_proc_env.application_environment_->recv_buff_size_,
								__simple_proc_env.application_environment_->send_buff_size_);

	if(!is_ok)	return FALSE;

	__simple_proc_env.stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
	__simple_proc_env.socket_recv_thread_ = (HANDLE)_beginthreadex(NULL, 0, 
																	socket_recv_thread_proc, 
																	&__simple_proc_env, 0 , NULL);

	start_inputting_thread(&parallel_thread_callback);


	return TRUE;
}

BOOL stop_simple_proc()
{
	DWORD result = 0x0;

	stop_inputting_thread();

	SetEvent(__simple_proc_env.stop_event_);
	result = WaitForSingleObject(__simple_proc_env.socket_recv_thread_, 100);
	if(result != WAIT_OBJECT_0) TerminateThread(__simple_proc_env.socket_recv_thread_, 0x1);

	CloseHandle(__simple_proc_env.socket_recv_thread_);
	__simple_proc_env.socket_recv_thread_ = NULL;

	CloseHandle(__simple_proc_env.stop_event_);
	__simple_proc_env.stop_event_ = NULL;

	release_socket();

	if(__simple_proc_env.application_environment_->be_all_info_outputted_){
		fclose(__recv_file);
		fclose(__snd_file);
	}

	return TRUE;
}

