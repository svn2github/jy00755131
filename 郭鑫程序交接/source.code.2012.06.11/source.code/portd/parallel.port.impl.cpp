/*
*	描述：该文件为log基本操作类的实现文件
*	文件：parallel.port.impl.cpp
*	注释：具体声明和调用见parallel.port.impl.h
*	参考文献：ADLINK PCIS-DASK 文档
*	版本：1.0.0.0
*	注释：
*	修改：
*		1.2.10.150.2011.05.18	将写入并口卡方式由同步写入，转变为异步写入模式，初步测试12小时发现程序，不会卡死现象
*		1.2.16.165.2011.06.20	将并口卡的DO设置修改为20MHZ, WRITE_FIFO方式，在写入代码中加入问询状态
*		1.2.17.170.2011.06.21	加入测试DO性能测试代码,测试结果如下：
								排除个别异常数据（数值特别大，超出精度范围）
								测试环境： 
								HARDWARE: athlon 64 X2 4400+ 2.31HZ, 2G内存
								OS: winxp sp2
								写入包大小：3K
		------------------------------------------------------------------------------------------------------		
								测试量级： 4290次
								查询FIFO是否为空循环消耗： 
									AVG -- 1259微秒
									MAX -- 18873微秒
									总结：生成折线图显示当前的数据抖动频繁，并且差值明显

								WritePort消耗： 
								AVG -- 222微秒
								MAX -- 15687微秒
								总结：生成折线图显示当前的数据存在抖动但出现频率较低，差值明显

								查询是否为写入成功循环消耗： 
								AVG -- 525微秒
								MAX -- 17940微秒
								总结：生成折线图显示基本在很低状况下运行，只是在测试快结束是出现高崎

								------------------------------------------------------------------------------------------------------		
								测试量级： 8920次
								查询FIFO是否为空循环消耗： 
								AVG -- 1306微秒
								MAX -- 24078微秒
								总结：生成折线图显示当前的数据抖动频繁，并且差值明显

								WritePort消耗： 
								AVG -- 305微秒
								MAX -- 18949微秒
								总结：生成折线图显示当前的数据存在抖动但出现频率较低，差值明显

								查询是否为写入成功循环消耗： 
								AVG -- 555微秒
								MAX -- 18895微秒
								总结：生成折线图显示基本在很低状况下运行，只是在测试快结束是出现高崎

		1.2.19.180.2011.06.23	修改写入并口卡时速问题，现在写入为40K，分成3次完成，15k，15k和10k，
								第一次有效数据大小为128字节，放在第一个15k中，其他15k和10k为空数据

		1.2.20.182.2011.06.24	修改1.2.19.180.2011.06.23版本中写方式：不能使用一个缓冲加偏移量方式，
								必须使用变量

		1.2.21.190.2011.06.29	修改些writeport为每次128字节，同时再写40包空数据,在AMD.x64.winxp.sp2.2g.mem
								测试2000条稳定

		1.2.30.195.2011.06.30	修改input_thread_proc线程函数用于支持多缓冲读取时，出现的返回DMA内存id不连续，
								造成的丢包问题

		1.2.33.202.2011.08.12	修改程序中加入调试配置信息, 用来支持全部IO信息输出,可以使用新的设置选项[输出全部日志]和
								使用[忽略端口]来添加不需要记录IO信息,该版本是修改1.2.32.200.2011.08.11的bug
								(bug: 进写入并口卡内容放入到设置选型中)


*/
#include "../common/config.h"
#include "../common/log.h"
#include "../common/code.conv.h"
#include "sockimpl.h"
#include "pcid.h"
#include "parallel.port.impl.h"
#include "Dask.h"
#include "output.queue.h"

#pragma comment(lib, "PCI-Dask")

extern APPLICATION_ENVIRONMENT __application_environment;

#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
#	pragma intrinsic(__rdtsc) 
#	define TIME_VALUE 2310
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)

typedef struct __tagPARALLEL_PORT_IMPL
{
	U16 card_id_;
	U16 card_type_;
	unsigned int data_input_buffer_size_;
	unsigned int data_input_buffer_num_;
	U16 port_width_;

	U16 input_control_Polarity_;
	U16 input_trigger_source_;
	U16 input_wait_status_;
	U16 input_terminator_;

	U16 output_control_Polarity_;
	U16 output_trigger_source_;
	U16 output_terminator_;
	U16 output_wait_status_;

	unsigned waiting_time_;

	unsigned char* input_buffers_[MAX_PARALLE_PORT_INPUT_BUFFER_SIZE];
	U16 input_buffer_id_[MAX_PARALLE_PORT_INPUT_BUFFER_SIZE];

	HANDLE parallel_input_thread_handler_;
	HANDLE parallel_stop_event_;

	bool is_running_;//stop sign

	parallel_inputting_thread_callback parallel_thread_callback_;
	parallel_inputting_callback_function parallel_callback_callback_;
}PARALLEL_PORT_IMPL, *PARALLEL_PORT_IMPL_PTR;

PARALLEL_PORT_IMPL __parallel_port;
FILE* __write_port_file;
FILE* __read_port_file;
FILE* __buf_id_file;

//extern std::vector<ptRECV_PAK> __recv_pak_vector;
//extern std::list<ptRECV_PAK> __recv_pak_list;
extern SIMPLE_PROC_ENVIRONMENT __simple_proc_env;

bool init_parallel_port(unsigned input_buffer_num, unsigned input_buffer_size, unsigned poll_time, 
						unsigned output_buffer_size)
{
	
	output_buffer_size = output_buffer_size;

	I16 ret_code = 0x0;

	if(input_buffer_size > 0)	__parallel_port.data_input_buffer_size_ = input_buffer_size;
	else						__parallel_port.data_input_buffer_size_ = 81920;

	if(input_buffer_num > 0)	__parallel_port.data_input_buffer_num_ = input_buffer_num;
	else						__parallel_port.data_input_buffer_num_ = 2;

	if(poll_time > 0)	__parallel_port.waiting_time_ = poll_time;
	else				__parallel_port.waiting_time_ = 0x0;

	__parallel_port.card_type_ = PCI_7300A_RevB;
	__parallel_port.card_id_ = 32;
	__parallel_port.port_width_ = 8;
	__parallel_port.input_control_Polarity_ = P7300_DIREQ_NEG|P7300_DIACK_NEG|P7300_DITRIG_NEG;
	__parallel_port.input_trigger_source_ = TRIG_EXT_STROBE;
	__parallel_port.input_wait_status_ = P7300_WAIT_NO;
	__parallel_port.input_terminator_ = P7300_TERM_ON;


	__parallel_port.output_control_Polarity_ = P7300_DOREQ_NEG|P7300_DOACK_NEG|P7300_DOTRIG_NEG;
	__parallel_port.output_trigger_source_ = TRIG_CLK_20MHZ;
	__parallel_port.output_terminator_ = P7300_TERM_OFF;
	__parallel_port.output_wait_status_ = P7300_WAIT_FIFO;

	memset(__parallel_port.input_buffers_, 0x0, sizeof(__parallel_port.input_buffers_));
	memset(__parallel_port.input_buffer_id_, 0x0, sizeof(__parallel_port.input_buffer_id_));

	__parallel_port.parallel_input_thread_handler_ = NULL;
	__parallel_port.parallel_stop_event_ = NULL;

	__parallel_port.parallel_thread_callback_ = NULL;
	__parallel_port.parallel_callback_callback_ = NULL;

	parallel_output_queue_init();

	__parallel_port.is_running_ = false;

	__parallel_port.parallel_stop_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);

	if(__application_environment.be_all_info_outputted_){
		__write_port_file = fopen("write.port.file.log", "w");
		__read_port_file = fopen("read.port.file.log", "w");
		__buf_id_file = fopen("buf.id.log", "w");
	}
	
	return true;
}

bool start_inputting_callback(parallel_inputting_callback_function callback)
{
	bool is_ok = true;
	return is_ok;
}

void stop_inputting_callback(){}

void stop_inputting_thread()
{
	DWORD result = 0x0;

	__parallel_port.is_running_ = false;
	SetEvent(__parallel_port.parallel_stop_event_);

	result = WaitForSingleObject(__parallel_port.parallel_input_thread_handler_, 100);
	if(result != WAIT_OBJECT_0){

		TerminateThread(__parallel_port.parallel_input_thread_handler_, 0x1);
		CloseHandle(__parallel_port.parallel_input_thread_handler_);
		__parallel_port.parallel_input_thread_handler_ = NULL;
	}

	stop_inputting_callback();
}

unsigned __stdcall input_thread_proc(void* args)
{

	//	WaitForSingleObject(ppi_ptr->parallel_stop_event_, ppi_ptr->waiting_time_);
	PARALLEL_PORT_IMPL_PTR ppi_ptr = (PARALLEL_PORT_IMPL_PTR)args;
	DWORD result = 0x0;
	PARALLEL_OUTPUT_QUEUE_ITEM item;
	LARGE_INTEGER  litmp; 
	LONGLONG       QPart1,QPart2;;
	double         minute, freq, tim; 
	int i = 0;

	while(ppi_ptr->is_running_){

		QueryPerformanceFrequency(&litmp); 
		freq = (double)litmp.QuadPart; 
		QueryPerformanceCounter(&litmp); 
		QPart1 = litmp.QuadPart; 

		do{
			result = WaitForSingleObject(ppi_ptr->parallel_stop_event_, 0);
			if(result == WAIT_OBJECT_0)	goto END;

			QueryPerformanceCounter(&litmp); 
			QPart2 = litmp.QuadPart;
			minute = (double)(QPart2-QPart1); 
			tim = minute / freq; 
		}while(tim<0.01);

		for(i = 0; i < 4; ++i){
			if(parallel_output_queue_pop(&item)){
				if(output_data_into_parallel_port(item.buf_, sizeof(item.buf_)) != -1){
					_InterlockedIncrement(&__simple_proc_env.application_environment_->pci_output_num_);
				}
				else{
					OUTPUT_DEBUG_STRING("Write a pallerl error!");
				}
			}
		}

	}
END:

	_endthreadex(0);
	return 0;
}

bool start_inputting_thread(parallel_inputting_thread_callback callback)
{
	I16 ret_code = 0x0;

	if(callback == NULL){			
		OUTPUT_DEBUG_STRING(IDS_STRING140);
		return false;
	}
	else{
		__parallel_port.parallel_thread_callback_ = callback;
	}

	ResetEvent(__parallel_port.parallel_stop_event_);
	__parallel_port.is_running_ = true;

	//create input thread
	__parallel_port.parallel_input_thread_handler_ = (HANDLE)_beginthreadex(NULL, 0, input_thread_proc, &__parallel_port, 0, NULL);

	return true;
}


void release_parallel_port()
{
	//ptRECV_PAK pak_ptr = NULL;
	//for(std::vector<ptRECV_PAK>::iterator it_v = __recv_pak_vector.begin(); it_v != __recv_pak_vector.end(); ++it_v){
	//	pak_ptr = *it_v;
	//	if(pak_ptr) delete pak_ptr;
	//}

	//for(std::list<ptRECV_PAK>::iterator it_l = __recv_pak_list.begin(); it_l != __recv_pak_list.end(); ++it_l){
	//	pak_ptr = *it_l;
	//	if(pak_ptr) delete pak_ptr;	
	//}

	CloseHandle(__parallel_port.parallel_stop_event_);
	__parallel_port.parallel_stop_event_ = NULL;
//	Release_Card(__parallel_port.card_id_);

	parallel_output_queue_destroy();

	if(__application_environment.be_all_info_outputted_){
		fclose(__write_port_file);
		fclose(__read_port_file);
		fclose(__buf_id_file);
	}
}

int output_data_into_parallel_port(unsigned char* buf, unsigned buf_size)
{
	int ret_code = 0x0;
	unsigned char cmd = 0x0;
	unsigned long ip_addr = 0x0;
	APPLICATION_ENVIRONMENT::IP_TABLE_MAP_ITER itr;
	APPLICATION_ENVIRONMENT::DEVICE_INFO_LIST::iterator di_itr;
	unsigned char virtual_buf[256] = {0};
	SOCKADDR_IN ip = {0};

	memcpy(&ip_addr, buf + 16, sizeof(unsigned long));
	
	itr = __application_environment.virtual_ip_table_map_->find(ip_addr);
	if(itr != __application_environment.virtual_ip_table_map_->end()){

		_InterlockedIncrement(&__application_environment.upd_send_num_);
		udp_send((const char*)buf, buf_size);
	}
	else{

		//主机IP设置广播 & 响应主机IP设置
		cmd = *(buf + 0x20);
		memcpy(&ip_addr, buf + 21, sizeof(unsigned long));
		if(cmd == 0x1){
			for(di_itr = __application_environment.device_info_list_->begin();
				di_itr != __application_environment.device_info_list_->end(); ++di_itr){
				
				if((*di_itr)->sn_ == ip_addr) break;
			}

			if(di_itr == __application_environment.device_info_list_->end()) return 0;

			if(!(*di_itr)->is_be_virtual_fdu_){	(*di_itr)->ip_ = 	ip_addr;}
			else{
				cmd = *(buf + 25);
				if(cmd == 0x03){
					memcpy(&(*di_itr)->ip_, buf + 26, sizeof(unsigned long));
					__application_environment.virtual_ip_table_map_->insert(APPLICATION_ENVIRONMENT::IP_TABLE_PAIR((*di_itr)->ip_, *di_itr));
					//send respon
					CopyMemory(virtual_buf, buf, sizeof(virtual_buf));
					SecureZeroMemory(virtual_buf + 10, 4);
					CopyMemory(virtual_buf + 14, virtual_buf + 26, 4);
					ip.sin_family = AF_INET;
					ip.sin_port = htons(__application_environment.remote_port_);
					ip.sin_addr.s_addr = inet_addr(__application_environment.remote_ip_);
					udp_send((char*)virtual_buf, sizeof(virtual_buf), (SOCKADDR*)&ip);
				}
			}
		}

		_InterlockedIncrement(&__application_environment.pci_output_num_);
		return ret_code;
	}

	return ret_code;
}



