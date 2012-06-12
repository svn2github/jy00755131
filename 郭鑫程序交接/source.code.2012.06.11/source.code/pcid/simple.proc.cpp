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

typedef struct __tagSIMPLE_PROC_ENVIRONMENT
{
	HANDLE socket_recv_thread_;
	HANDLE stop_event_;
	APPLICATION_ENVIRONMENT_PTR application_environment_;
} SIMPLE_PROC_ENVIRONMENT, *SIMPLE_PROC_ENVIRONMENT_PTR;

SIMPLE_PROC_ENVIRONMENT __simple_proc_env;

FILE* __recv_file = NULL;
FILE* __snd_file = NULL;

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

	if(output_data_into_parallel_port((unsigned char*)buf, buf_size) != -1)
		_InterlockedIncrement(&env->application_environment_->pci_output_num_);

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
		result = WaitForSingleObject(env->stop_event_, env->application_environment_->recv_thread_waiting_time_);
		if(result == WAIT_OBJECT_0)
			break;
	}

	_endthreadex(0);

	return ret_code;
}

unsigned parallel_thread_callback(unsigned char* buf, unsigned int buf_size)
{
	int size = 0;
	SOCKADDR_IN ip;
	LONG serial_no = 0x0, 
		prev_serial_no = 0x0,
		dispersion = 0x0;

	SYSTEMTIME systm = {0};
	SYSTEMTIME systm2 = {0};

	char bufxx[1024] = {0};
	char error_buf[MAX_STRING_SIZE * 20] = {0};
	int bufxx_size = 0;

	SecureZeroMemory(&ip, sizeof(ip));
	ip.sin_family = AF_INET;

	if((buf[24] == 0x0 && buf[25] == 0x0) &&
		(buf[16] == 0x0 && buf[17] == 0x0 && buf[18] == 0x0 && buf[19] == 0x0) &&
		(buf[20] == 0x0 && buf[21] == 0x0 && buf[22] == 0x0 && buf[23] == 0x0))
		return 0;

	_InterlockedIncrement(&__simple_proc_env.application_environment_->pci_input_num_);

	unsigned short crc16 = 0x0;
	//check crc
	if(__simple_proc_env.application_environment_->enable_to_check_crc_){

		memcpy(&crc16, buf + CRC_BEGIN_POS, sizeof(unsigned short));
		if(!check_crc(buf, buf_size - 4, crc16)){
			return size = 0;
		}
	}


	//get serial.no per-package
	memcpy(&serial_no, buf + RECV_PACK_SERIAL_NO_POS, sizeof(char));

	prev_serial_no = _InterlockedExchange(&__simple_proc_env.application_environment_->serial_no_, serial_no);
	dispersion = serial_no - prev_serial_no;

	if(dispersion != 1){
		if(dispersion < 0){

			dispersion += (RECV_PACK_MAX_SERIAL_NO + 1);
			if(dispersion != 1)	OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
		}
		else{
			OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
		}

		dispersion = interlock_add(&__simple_proc_env.application_environment_->udp_send_serial_no_, dispersion);
	}
	else{

		dispersion = _InterlockedIncrement(&__simple_proc_env.application_environment_->udp_send_serial_no_);
	}

	//set udp serial no
	memcpy(buf + SEND_PACK_SERIAL_NO_POS, &dispersion, SEND_PACK_SERIAL_NO_LENGTH);

	//filter it	
	if(__simple_proc_env.application_environment_->enable_to_transfer_data_){

		if(filter_it(buf, buf_size)){

			memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
			ip.sin_port = htons(ip.sin_port);
			memcpy(&ip.sin_addr.s_addr, buf + DEST_IP_BEGIN_POS, sizeof(ULONG));
			
			size = udp_send((const char*)buf, buf_size, (SOCKADDR*)&ip);
			if(__simple_proc_env.application_environment_->be_all_info_outputted_){
				
				ip.sin_port = ntohs(ip.sin_port);
				if(__simple_proc_env.application_environment_->ignored_port_ != ip.sin_port){

					GetSystemTime(&systm);
					SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
					bufxx_size = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
					bufxx[bufxx_size] = 0x0;
					fprintf(__snd_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
				}
			}
		}
		else{

			memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
			memcpy(&ip.sin_addr.s_addr, buf + DEST_IP_BEGIN_POS, sizeof(ULONG));

			size = bin_to_ansi(buf, buf_size, error_buf, sizeof(error_buf));
			error_buf[size] = 0x0;
			OUTPUT_DEBUG_STRING(IDS_STRING165, inet_ntoa(ip.sin_addr), ip.sin_port, error_buf);

			ip.sin_port = htons(ip.sin_port);
			size = udp_send((const char*)buf, buf_size, (SOCKADDR*)&ip);

			if(__simple_proc_env.application_environment_->be_all_info_outputted_){

				ip.sin_port = ntohs(ip.sin_port);
				if(__simple_proc_env.application_environment_->ignored_port_ != ip.sin_port){

					GetSystemTime(&systm);
					SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
					bufxx_size = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
					bufxx[bufxx_size] = 0x0;
					fprintf(__snd_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
				}
			}
			return size = 0;
		}
	}
	else{
		size = udp_send((const char*)buf, buf_size);
		if(__simple_proc_env.application_environment_->be_all_info_outputted_){

			memcpy(&ip.sin_port, buf + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
			if(__simple_proc_env.application_environment_->ignored_port_ != ip.sin_port){

				GetSystemTime(&systm);
				SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
				bufxx_size = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
				bufxx[bufxx_size] = 0x0;
				fprintf(__snd_file, "[%d:%d:%d:%d] %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
			}
		}
	}

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

	is_ok = create_udp_socket(static_cast<unsigned short>(__simple_proc_env.application_environment_->listen_port_),
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

	release_parallel_port();
	release_socket();

	if(__simple_proc_env.application_environment_->be_all_info_outputted_){
		fclose(__recv_file);
		fclose(__snd_file);
	}
	
	return TRUE;
}

/*
unsigned __stdcall input_thread_proc(void* args)
{
	PARALLEL_PORT_IMPL_PTR ppi_ptr = (PARALLEL_PORT_IMPL_PTR)args;

	BOOLEAN is_Ready = FALSE;
	BYTE rec_buff[DATA_INPUT_PACKAGE_SIZE] = {0};
	I16 ret_code = 0;
	DWORD result = 0x0;
	unsigned i = 0;

	int size = 0;
	SOCKADDR_IN ip;
	LONG serial_no = 0x0, 
		prev_serial_no = 0x0,
		dispersion = 0x0;

	unsigned short status = 0x0;
	U32 count = 0;

	//while(true){

	//	ret_code = DI_ContStatus(__parallel_port.card_id_, &status);

	//	if((ret_code == NoError) && ((status & 0x4) != 0x4)){

	//		//read
	//		while(true){

	//			DI_ContReadPort(__parallel_port.card_id_, 0, rec_buff, 256, 1, ASYNCH_OP);
	//			DI_AsyncCheck (__parallel_port.card_id_, &is_Ready, &count);

	//			if(is_Ready){

	//				DI_AsyncClear (__parallel_port.card_id_, &count);

	//				if(rec_buff[0] == SYN_WORD_ONE && rec_buff[1] == SYN_WORD_TWO &&
	//					rec_buff[2] == SYN_WORD_THREE && rec_buff[3] == SYN_WORD_FOUR){


	//						SecureZeroMemory(&ip, sizeof(ip));
	//						ip.sin_family = AF_INET;

	//						if((rec_buff[24] == 0x0 && rec_buff[25] == 0x0) &&
	//							(rec_buff[16] == 0x0 && rec_buff[17] == 0x0 && rec_buff[18] == 0x0 && rec_buff[19] == 0x0) &&
	//							(rec_buff[20] == 0x0 && rec_buff[21] == 0x0 && rec_buff[22] == 0x0 && rec_buff[23] == 0x0))
	//							continue;

	//						_InterlockedIncrement(&__simple_proc_env.application_environment_->pci_input_num_);

	//						//unsigned short crc16 = 0x0;
	//						////check crc
	//						//if(__simple_proc_env.application_environment_->enable_to_check_crc_){

	//						//	memcpy(&crc16, buf + CRC_BEGIN_POS, sizeof(unsigned short));
	//						//	if(!check_crc(buf, buf_size - 4, crc16)){
	//						//		return size = 0;
	//						//	}
	//						//}

	//						//get serial.no per-package
	//						memcpy(&serial_no, rec_buff + RECV_PACK_SERIAL_NO_POS, sizeof(char));

	//						prev_serial_no = _InterlockedExchange(&__simple_proc_env.application_environment_->serial_no_, serial_no);
	//						dispersion = serial_no - prev_serial_no;

	//						if(dispersion != 1){
	//							if(dispersion < 0){

	//								dispersion += (RECV_PACK_MAX_SERIAL_NO + 1);
	//								if(dispersion != 1)	OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
	//							}
	//							else{
	//								OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
	//							}

	//							dispersion = interlock_add(&__simple_proc_env.application_environment_->udp_send_serial_no_, dispersion);
	//						}
	//						else{

	//							dispersion = _InterlockedIncrement(&__simple_proc_env.application_environment_->udp_send_serial_no_);
	//						}

	//						//set udp serial no
	//						memcpy(rec_buff + SEND_PACK_SERIAL_NO_POS, &dispersion, SEND_PACK_SERIAL_NO_LENGTH);

	//						//filter it	
	//						if(__simple_proc_env.application_environment_->enable_to_transfer_data_){

	//							if(filter_it(rec_buff, sizeof(rec_buff))){

	//								memcpy(&ip.sin_port, rec_buff + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
	//								ip.sin_port = htons(ip.sin_port);
	//								memcpy(&ip.sin_addr.s_addr, rec_buff + DEST_IP_BEGIN_POS, sizeof(ULONG));
	//								//ip.sin_addr.s_addr = htonl(ip.sin_addr.s_addr);
	//								size = udp_send((const char*)rec_buff, sizeof(rec_buff), (SOCKADDR*)&ip);
	//							}
	//							else{

	//								memcpy(&ip.sin_port, rec_buff + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
	//								memcpy(&ip.sin_addr.s_addr, rec_buff + DEST_IP_BEGIN_POS, sizeof(ULONG));
	//								OUTPUT_DEBUG_STRING(IDS_STRING165, inet_ntoa(ip.sin_addr), ip.sin_port);

	//								ip.sin_port = htons(ip.sin_port);
	//								size = udp_send((const char*)rec_buff, sizeof(rec_buff), (SOCKADDR*)&ip);
	//							}

	//						}
	//						else{
	//							size = udp_send((const char*)rec_buff, sizeof(rec_buff));
	//						}

	//						if(static_cast<unsigned int>(size) == sizeof(rec_buff))
	//							_InterlockedIncrement(&__simple_proc_env.application_environment_->upd_send_num_);
	//				}
	//			}
	//			else 
	//				break;
	//		}
	//	}

	//	result = WaitForSingleObject(ppi_ptr->parallel_stop_event_, ppi_ptr->waiting_time_);
	//	if(result == WAIT_OBJECT_0)	break;
	//}


	double tmp_fl = 1.989;
	int save_point = 0;
	U16 buf_id = 0;

	while(true){

		ret_code = DI_AsyncMultiBufferNextReady(ppi_ptr->card_id_, &is_Ready, &buf_id);
		if(ret_code != NoError){

			OUTPUT_DEBUG_STRING(IDS_PARALLEL_GET_NEXT_READY_ERROR, ret_code);
			continue;
		}

		if(is_Ready){

			tmp_fl = 2.998 * 1.99845;//for rising edge & falling edge
			tmp_fl = 2.998;

			if(buf_id < 0 || buf_id > (ppi_ptr->data_input_buffer_num_ - 1)){

				OUTPUT_DEBUG_STRING(IDS_PARALLEL_RETURN_BUF_ID_ERROR);
				continue;
			}

			for(i=0; i< ppi_ptr->data_input_buffer_size_; ++i){

				rec_buff[save_point] = ppi_ptr->input_buffers_[buf_id][i];
				switch(save_point)
				{
				case 0: 
					if(rec_buff[save_point]==SYN_WORD_ONE) save_point ++;
					break;
				case 1: 
					if(rec_buff[save_point]==SYN_WORD_TWO) save_point ++;
					else save_point =0;
					break;
				case 2: 
					if(rec_buff[save_point]==SYN_WORD_THREE) save_point ++;
					else save_point =0;
					break;
				case 3: 
					if(rec_buff[save_point]==SYN_WORD_FOUR) save_point ++;
					else save_point =0;
					break;
				case DATA_INPUT_PACKAGE_SIZE - 1: 
					{
						//ppi_ptr->parallel_thread_callback_(rec_buff, sizeof(rec_buff));

						SecureZeroMemory(&ip, sizeof(ip));
						ip.sin_family = AF_INET;


						if((rec_buff[24] == 0x0 && rec_buff[25] == 0x0) &&
							(rec_buff[16] == 0x0 && rec_buff[17] == 0x0 && rec_buff[18] == 0x0 && rec_buff[19] == 0x0) &&
							(rec_buff[20] == 0x0 && rec_buff[21] == 0x0 && rec_buff[22] == 0x0 && rec_buff[23] == 0x0))
							continue;

						_InterlockedIncrement(&__simple_proc_env.application_environment_->pci_input_num_);

						//unsigned short crc16 = 0x0;
						////check crc
						//if(__simple_proc_env.application_environment_->enable_to_check_crc_){

						//	memcpy(&crc16, buf + CRC_BEGIN_POS, sizeof(unsigned short));
						//	if(!check_crc(buf, buf_size - 4, crc16)){
						//		return size = 0;
						//	}
						//}

						//get serial.no per-package
						memcpy(&serial_no, rec_buff + RECV_PACK_SERIAL_NO_POS, sizeof(char));

						prev_serial_no = _InterlockedExchange(&__simple_proc_env.application_environment_->serial_no_, serial_no);
						dispersion = serial_no - prev_serial_no;

						if(dispersion != 1){
							if(dispersion < 0){

								dispersion += (RECV_PACK_MAX_SERIAL_NO + 1);
								if(dispersion != 1)	OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
							}
							else{
								OUTPUT_DEBUG_STRING(IDS_STRING161, serial_no, prev_serial_no, dispersion);
							}

							dispersion = interlock_add(&__simple_proc_env.application_environment_->udp_send_serial_no_, dispersion);
						}
						else{

							dispersion = _InterlockedIncrement(&__simple_proc_env.application_environment_->udp_send_serial_no_);
						}

						//set udp serial no
						memcpy(rec_buff + SEND_PACK_SERIAL_NO_POS, &dispersion, SEND_PACK_SERIAL_NO_LENGTH);

						//filter it	
						if(__simple_proc_env.application_environment_->enable_to_transfer_data_){

							if(filter_it(rec_buff, sizeof(rec_buff))){

								memcpy(&ip.sin_port, rec_buff + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
								ip.sin_port = htons(ip.sin_port);
								memcpy(&ip.sin_addr.s_addr, rec_buff + DEST_IP_BEGIN_POS, sizeof(ULONG));
								//ip.sin_addr.s_addr = htonl(ip.sin_addr.s_addr);
								size = udp_send((const char*)rec_buff, sizeof(rec_buff), (SOCKADDR*)&ip);
								//WaitForSingleObject(ppi_ptr->parallel_stop_event_, 0);
							}
							else{

								memcpy(&ip.sin_port, rec_buff + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
								memcpy(&ip.sin_addr.s_addr, rec_buff + DEST_IP_BEGIN_POS, sizeof(ULONG));
								OUTPUT_DEBUG_STRING(IDS_STRING165, inet_ntoa(ip.sin_addr), ip.sin_port);

								ip.sin_port = htons(ip.sin_port);
								size = udp_send((const char*)rec_buff, sizeof(rec_buff), (SOCKADDR*)&ip);
								//WaitForSingleObject(ppi_ptr->parallel_stop_event_, 0);
							}

						}
						else{
							size = udp_send((const char*)rec_buff, sizeof(rec_buff));
							//WaitForSingleObject(ppi_ptr->parallel_stop_event_, 0);
						}


						if(static_cast<unsigned int>(size) == sizeof(rec_buff))
							_InterlockedIncrement(&__simple_proc_env.application_environment_->upd_send_num_);
					}

					save_point =0;
					break;
				default: 
					save_point ++;
					break;
				}
			}

		}
       else
			result = WaitForSingleObject(ppi_ptr->parallel_stop_event_, ppi_ptr->waiting_time_);

		if(result == WAIT_OBJECT_0)	break;
	}

	_endthreadex(0);

	return 0;
}

unsigned int __stdcall socket_recv_thread_proc(void* arg)
{
//arg = arg;

DWORD result = 0x0;
char recv_buf[MAX_PACK_NET_SIZE] = {0};
unsigned long ioctl_argp = 0L;
int recv_size = 0;

I16 ret_code = 0x0; 
BOOLEAN stopped = 0x0;
U32 count = 0x0;
U16 status = 0x0;

int i = 0;
unsigned char bufx[128];

while(true){

//receive msg
while(true){

if(ioctlsocket(__socket_impl.socket_, FIONREAD, &ioctl_argp) == 0){

if(ioctl_argp > 0){

recv_size = recv(__socket_impl.socket_, recv_buf, sizeof(recv_buf), 0);

if(recv_size == SOCKET_ERROR){

OUTPUT_DEBUG_STRING(IDS_STRING143, recv_size);
break;
}
else if(recv_size == 0x0){

OUTPUT_DEBUG_STRING(IDS_STRING144);
break;
}
else if(recv_size > MAX_PACK_NET_SIZE){

OUTPUT_DEBUG_STRING(IDS_STRING145);
break;
}
else{

ZeroMemory(bufx, 128);
_InterlockedIncrement(&__application_environment.upd_recv_num_);

do{
//		status = 0x1;	
ret_code = DO_ContStatus(__parallel_port.card_id_, &status);

if(ret_code == NoError)
if((status & 0x4) == 0x4) break;
else WaitForSingleObject(__simple_proc_env.stop_event_, 0);
}while (true);


ret_code = DO_ContWritePort(__parallel_port.card_id_, 0, recv_buf, recv_size, 1, 1, ASYNCH_OP);
if(ret_code != NoError){

OUTPUT_DEBUG_STRING(IDS_STRING141, ret_code);
ret_code = -1;
}

stopped = 0x0;
count = 0;
do{
DO_AsyncCheck(__parallel_port.card_id_, &stopped, &count);
}
while(!stopped);

if(count != recv_size){

OUTPUT_DEBUG_STRING(IDS_STRING166);
ret_code = -1;
}

DO_AsyncClear(__parallel_port.card_id_, &count);

if(ret_code != -1)
_InterlockedIncrement(&__application_environment.pci_output_num_);

//wait
WaitForSingleObject(__simple_proc_env.stop_event_, 0);

for (i=0;i<40;i++){

do{
//		status = 0x1;	
ret_code = DO_ContStatus(__parallel_port.card_id_, &status);
if(ret_code == NoError)
if((status & 0x4) == 0x4) break;
//WaitForSingleObject(__simple_proc_env.stop_event_, 0);
}while (true);


ret_code = DO_ContWritePort(__parallel_port.card_id_, 0, bufx, 128, 1, 1, ASYNCH_OP);
if(ret_code != NoError){

OUTPUT_DEBUG_STRING(IDS_STRING141, ret_code);
ret_code = -1;
}

stopped = 0x0;
count = 0;
do{
DO_AsyncCheck(__parallel_port.card_id_, &stopped, &count);
}
while(!stopped);

DO_AsyncClear(__parallel_port.card_id_, &count);
//WaitForSingleObject(__simple_proc_env.stop_event_, 0);
}
}
}
else
break;
}
else{
break;
}
}

//wait...
result = WaitForSingleObject(__simple_proc_env.stop_event_, __application_environment.recv_thread_waiting_time_);
if(result == WAIT_OBJECT_0)
break;
}

_endthreadex(0);

return 0;
}

*/
