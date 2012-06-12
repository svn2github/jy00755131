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
#include "pcid.h"
#include "parallel.port.impl.h"
#include "Dask.h"


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

	__parallel_port.is_running_ = false;

	ret_code = Register_Card(__parallel_port.card_type_,0);
	if( ret_code < 0 || ret_code > 31 ){
		
		OUTPUT_DEBUG_STRING(IDS_PARALLEL_REGISTER_ERROR, ret_code);
		return false;
	}
	else{

		__parallel_port.card_id_ = ret_code;
	}	

	//configure input part
	ret_code = DI_7300B_Config(__parallel_port.card_id_, 
								__parallel_port.port_width_, 
								__parallel_port.input_trigger_source_, 
								__parallel_port.input_wait_status_, 
								__parallel_port.input_terminator_, 
								__parallel_port.input_control_Polarity_, TRUE, TRUE);
	if(ret_code != 0){

		OUTPUT_DEBUG_STRING(IDS_PARALLEL_INPUT_ARG_ERROR, ret_code);
		return false;
	}

	//configure output part
	ret_code = DO_7300B_Config(__parallel_port.card_id_, 
								__parallel_port.port_width_, 
								__parallel_port.output_trigger_source_, 
								__parallel_port.output_wait_status_, 
								__parallel_port.output_terminator_, 
								__parallel_port.output_control_Polarity_, 0);
	if(ret_code != 0){

		OUTPUT_DEBUG_STRING(IDS_PARALLEL_ONPUT_ARG_ERROR, ret_code);
		return false;
	}

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
	I16 ret_code = 0x0;

	if(callback == NULL) return	false;
	else __parallel_port.parallel_callback_callback_ = callback;

	for(unsigned i = 0; i < __parallel_port.data_input_buffer_num_; ++i){

		__parallel_port.input_buffers_[i] = (unsigned char*)malloc(__parallel_port.data_input_buffer_size_);
		ret_code = DI_ContMultiBufferSetup(__parallel_port.card_id_, __parallel_port.input_buffers_ + i, __parallel_port.data_input_buffer_size_, 
			&__parallel_port.input_buffer_id_[i]);
		if(ret_code != 0){

			OUTPUT_DEBUG_STRING(IDS_PARALLEL_MULTI_BUF_SET_ERROR, ret_code);
			return false;
		}
	}

	//add callback function
	DI_EventCallBack(__parallel_port.card_id_, 1, DIEnd, (U32)callback);

	ret_code = DI_ContMultiBufferStart(__parallel_port.card_id_, 0, 1);
	if(ret_code != 0){

		OUTPUT_DEBUG_STRING(IDS_PARALLEL_START_ERROR, ret_code);
		return false;
	}

	return is_ok;
}

void stop_inputting_callback()
{
	I16 ret_code = 0;
	U32 access_count = 0;

	ret_code = DI_AsyncClear(__parallel_port.card_id_, &access_count);

	if(ret_code != 0){

		OUTPUT_DEBUG_STRING(IDS_PARALLEL_CLEAR_ERROR, ret_code);
	}

	//delete all buffer
	for(unsigned i = 0; i < __parallel_port.data_input_buffer_num_; ++i){
		if(__parallel_port.input_buffers_[i] != NULL){

			free(__parallel_port.input_buffers_[i]);
			__parallel_port.input_buffers_[i] = NULL;
		}
	}
}

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
	PARALLEL_PORT_IMPL_PTR ppi_ptr = (PARALLEL_PORT_IMPL_PTR)args;

	U16 buf_id = 0, 
		prev_buf_id = ppi_ptr->data_input_buffer_num_ - 1;

	int	buf_size = 0,i = 0, j = 0;

	BOOLEAN is_Ready = FALSE;
	int save_point = 0;
	BYTE rec_buff[DATA_INPUT_PACKAGE_SIZE] = {0};
	I16 ret_code = 0;
	double tmp_fl = 1.998;

	unsigned short port = 0x0;
	int bufxx_size = 0;
	char bufxx[1280] = {0};
	SYSTEMTIME systm = {0};
	SYSTEMTIME systm2 = {0};

	while(ppi_ptr->is_running_){

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

			if(__application_environment.be_all_info_outputted_){
				GetSystemTime(&systm);
				SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
				fprintf(__buf_id_file, "[%d:%d:%d:%d] %d\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, buf_id);
			}

			buf_size = buf_id - prev_buf_id;
			if(buf_size <= 0)	buf_size += ppi_ptr->data_input_buffer_num_;

			for(j = 0, ++prev_buf_id; j < buf_size; ++j, ++prev_buf_id){

				if(prev_buf_id == ppi_ptr->data_input_buffer_num_) prev_buf_id = 0;

				for(i=0; i< ppi_ptr->data_input_buffer_size_; ++i){

					rec_buff[save_point] = ppi_ptr->input_buffers_[prev_buf_id][i];
					switch(save_point)
					{
					case 0: 
						if(rec_buff[save_point]==SYN_WORD_ONE) ++save_point;
						break;
					case 1: 
						if(rec_buff[save_point]==SYN_WORD_TWO) ++save_point;
						else save_point =0;
						break;
					case 2: 
						if(rec_buff[save_point]==SYN_WORD_THREE) ++save_point;
						else save_point =0;
						break;
					case 3: 
						if(rec_buff[save_point]==SYN_WORD_FOUR) ++save_point;
						else save_point =0;
						break;
					case DATA_INPUT_PACKAGE_SIZE - 1: 
						if(__application_environment.be_all_info_outputted_){

							memcpy(&port, rec_buff + DEST_PORT_BEGIN_POS, sizeof(unsigned short));
							if(__application_environment.ignored_port_ != port){

								GetSystemTime(&systm);
								SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm2);
								bufxx_size = bin_to_ansi(rec_buff, sizeof(rec_buff), bufxx, sizeof(bufxx));
								bufxx[bufxx_size] = 0x0;
								fprintf(__read_port_file, "[%d:%d:%d:%d] data: %s\n", systm2.wHour, systm2.wMinute, systm2.wSecond, systm2.wMilliseconds, bufxx);
							}
						}

						ppi_ptr->parallel_thread_callback_(rec_buff, sizeof(rec_buff));					
						save_point =0;
						break;
					default: 
						++save_point;
						break;
					}
				}
			}
			prev_buf_id = buf_id;
		}
		else//check signal event and stop this thread
			WaitForSingleObject(ppi_ptr->parallel_stop_event_, ppi_ptr->waiting_time_);
	}

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

	for(unsigned i = 0; i < __parallel_port.data_input_buffer_num_; ++i){

		__parallel_port.input_buffers_[i] = (unsigned char*)malloc(__parallel_port.data_input_buffer_size_);

		ret_code = DI_ContMultiBufferSetup(__parallel_port.card_id_, __parallel_port.input_buffers_[i], 
			__parallel_port.data_input_buffer_size_, &__parallel_port.input_buffer_id_[i]);

		if(ret_code != 0){
			OUTPUT_DEBUG_STRING(IDS_PARALLEL_MULTI_BUF_SET_ERROR, ret_code);
			return false;
		}
	}

	ret_code = DI_ContMultiBufferStart(__parallel_port.card_id_, 0, 1);
	if(ret_code != 0){
		OUTPUT_DEBUG_STRING(IDS_PARALLEL_START_ERROR, ret_code);
		return false;
	}

	ResetEvent(__parallel_port.parallel_stop_event_);
	__parallel_port.is_running_ = true;

	//create input thread
	__parallel_port.parallel_input_thread_handler_ = (HANDLE)_beginthreadex(NULL, 0, input_thread_proc, &__parallel_port, 0, NULL);

	return true;
}


void release_parallel_port()
{
	CloseHandle(__parallel_port.parallel_stop_event_);
	__parallel_port.parallel_stop_event_ = NULL;
	Release_Card(__parallel_port.card_id_);

	if(__application_environment.be_all_info_outputted_){
		fclose(__write_port_file);
		fclose(__read_port_file);
		fclose(__buf_id_file);
	}
}

int output_data_into_parallel_port_private(unsigned char* buf, unsigned buf_size)
{
	I16 ret_code = 0x0; 
	BOOLEAN stopped = 0x0;
	U32 count = 0x0;
	U16 status = 0x0;

#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
	unsigned __int64 dbg_count[5] = {0x0};
	TCHAR dbg_count_info[128]={0x0};

	dbg_count[0] = __rdtsc();
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMAN

	do{
		ret_code = DO_ContStatus(__parallel_port.card_id_, &status);
		if(ret_code == NoError)
			if((status & 0x4) == 0x4) break;
	}while (true);

#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
	dbg_count[1] = __rdtsc();
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMAN

	ret_code = DO_ContWritePort(__parallel_port.card_id_, 0, buf, buf_size, 1, 1, ASYNCH_OP);
	if(ret_code != NoError){

		OUTPUT_DEBUG_STRING(IDS_STRING141, ret_code);
		ret_code = -1;
	}

#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
	dbg_count[2] = __rdtsc();
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMAN

#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
	dbg_count[3] = __rdtsc();
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMAN

	do{
		DO_AsyncCheck(__parallel_port.card_id_, &stopped, &count);
	}
	while(!stopped);

	if(count != buf_size){

		OUTPUT_DEBUG_STRING(IDS_STRING166);
		ret_code = -1;
	}

	DO_AsyncClear(__parallel_port.card_id_, &count);


#if defined(__DEBUG_PCI_WRITE_PORT_PERFORMANCE__)
	dbg_count[4] = __rdtsc();
	sprintf_s(dbg_count_info, sizeof(dbg_count_info), "%I64d %I64d %I64d", 
		static_cast<__int64>((dbg_count[1] - dbg_count[0]) / TIME_VALUE),
		static_cast<__int64>((dbg_count[2] - dbg_count[1]) / TIME_VALUE),
		static_cast<__int64>((dbg_count[4] - dbg_count[3]) / TIME_VALUE));
	__application_environment.pci_writing_performance_log_->output_log(dbg_count_info);
#endif //defined(__DEBUG_PCI_WRITE_PORT_PERFORMAN

	return ret_code;
}

int output_data_into_parallel_port(unsigned char* buf, unsigned buf_size)
{

	int i = 0;
	int ret_code = 0x0;	
	unsigned char bufx[128];
	char bufxx[1024];
	SYSTEMTIME systm = {0};
	SYSTEMTIME systm2 = {0};
	
	SYSTEMTIME systm3 = {0};
	SYSTEMTIME systm4 = {0};

	if(__application_environment.be_all_info_outputted_)	GetSystemTime(&systm);

	ret_code = output_data_into_parallel_port_private(buf, buf_size);

	if(__application_environment.be_all_info_outputted_){

		GetSystemTime(&systm2);
		ret_code = bin_to_ansi(buf, buf_size, bufxx, sizeof(bufxx));
		bufxx[ret_code] = 0x0;
		SystemTimeToTzSpecificLocalTime(NULL, &systm, &systm3);
		SystemTimeToTzSpecificLocalTime(NULL, &systm2, &systm4);
		fprintf(__write_port_file, "beg:[%d:%d:%d:%d] end:[%d:%d:%d:%d] data: %s\n", 
			systm3.wHour, systm3.wMinute, systm3.wSecond, systm3.wMilliseconds,
			systm4.wHour, systm4.wMinute, systm4.wSecond, systm4.wMilliseconds, bufxx);
	}

	SecureZeroMemory(bufx, sizeof(bufx));
	for (i=0;i<40;i++)	output_data_into_parallel_port_private(bufx, sizeof(bufx));

	return ret_code;
}



