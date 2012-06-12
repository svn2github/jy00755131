/**
@brief	程序主体实现在该文件中,使用wmain_private代替entry point函数
@details	
@file	segd.conv.private.cpp
@note	
@see	
@version	1.2.46.125.2012.04.26
@author	kuoxin	
@bug
	1.1.5.12.2011.09.07		加入命令行处理,将程序主体基本构建完成
	1.1.10.32.2011.09.15	调整贺玲SPS模块
							修改以前实现的SEG-D文件模块,以便使用在现在的工程中
							根据配置文件信息,写入SEG-D数据头信息
	1.1.20.60.2011.10.14	修改程序中SEGD写入和SPS结合部分,加入进度提示信息等,对程序进行了静态分析
	1.1.21.61.2011.10.14	为了兼容winxp, 去除SetConsoleScreenBufferInfoEx函数
	1.1.30.80.2011.10.26	修改程序中去除segd.api.dll的调用,而直接修改读取信息
	1.1.32.82.2011.10.28	修改进度条和修改SEGD参数错误问题(其中包括sample_length & sample_number_in_trace以毫秒时间为单位,
							并且需要控制sample_number_in_trace最大不能超过128000毫秒,而且sample_number_in_trace等于
							sample_length + 1)
	1.1.33.85.2011.11.03	修改程序中辅助道数据存储
	1.1.34.87.2011.11.04	修改程序中辅助道数据存储错误
	1.1.35.88.2012.04.01	添加数据文件中的结构项目(TB_time);根据传入TB_time参数来获取数据小于该TB_time时间的FDU采集数据内容
	1.1.36.89.2012.04.11	根据TB_time的时间要求, 得到有效数据信息, 数据帧时间<TB_time时, i 可能为小于2或者0情况
	1.1.37.90.2012.04.12	加入--tb-time参数类型,用于传入tb时间参数
	1.2.46.125.2012.04.26	加入如下参数:
								--manufacturer-code
								--output-file
								--rw-segd-file
								--analyse-segd-file
								--make-segd-file
							针对这些参数进行了Release & Debug测试(intel inspector XE 2011) 

*/

#include "segd.conv.h"
#include "proc.conf.file.h"
#include "..\segd.viewer\segd.file.format.h"
#include "proc.sps.file.h"
#include "proc.data.file.h"
#include "merge.config.info.h"
#include "make.segd.h"
#include <float.h>
#include <time.h>


#define LENGTH_PER_SAMPLE_DATA 3 //!< 定义当前数据文件中,每个有效数据的大小

APPLICATION_ENVIRONMENT __application_environment; //!< 定义应用程序全局环境变量

int run_help_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_verbose_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_version_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_data_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_tb_time_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_analysing_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_reading_and_writing_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_output_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg);
int run_set_manufacturer_code_arg_private(APPLICATION_COMMAND_ARG_PTR arg);

APPLICATION_COMMAND_ARG	__app_cmd[] = 
{
	{__TEXT("--verbose"), run_verbose_arg_private, __TEXT(""), 0, 0},//显示输出信息开关,将它打开将可以看到各种罗嗦的显示信息
	{__TEXT("--version"), run_version_arg_private, __TEXT(""), 0, 1},//显示很虚的版本信息
	{__TEXT("--help"), run_help_arg_private, __TEXT(""), 0, 2},//显示帮助信息
	{__TEXT("--tb-time"), run_tb_time_arg_private, __TEXT(""), 1, 1},//指定开始取得数据的TB时间,将从TB时间开始向后的数据将被放入到SEGD文件中
	{__TEXT("--manufacturer-code"), run_set_manufacturer_code_arg_private, __TEXT(""), 1, 2},//根据传入的采样率生成相应的SEGD文件
	{__TEXT("--output-file"), run_output_file_arg_private, __TEXT(""), 1, 1},//输出文件,该文将将根据主参数的不同使用,产生不同的作用
	{__TEXT("--rw-segd-file"), run_reading_and_writing_segd_file_arg_private, __TEXT(""), 1, 2},//读入SEGD文件,然后写出相同的SEGD文件
	{__TEXT("--data-file"), run_data_file_arg_private, __TEXT(""), 1, 3},//输入AD数据文件,根据该文件将生成SEGD文件
	{__TEXT("--analyse-segd-file"), run_analysing_segd_file_arg_private, __TEXT(""), 1, 2},//根据输入的SEGD文件,产生相应的文本SEGD文件,用于分析SEGD
	{__TEXT("--make-segd-file"), run_making_segd_file_arg_private, __TEXT(""), 1, 2},//根据传入的采样率生成相应的SEGD文件
};

static const int __app_cmd_size = sizeof(__app_cmd) / sizeof(__app_cmd[0]);

int get_string(UINT id, char_t* buf, unsigned size)
{	return LoadString((HINSTANCE)__application_environment.instance_, id, buf, size); }

void output_log(unsigned int id, ...)
{
	int size = 0;
	char_t buf[MAX_STRING_SIZE] = {0};
	char_t format[MAX_STRING_SIZE] = {0};
	va_list ap;

	size = get_string(id, format, sizeof(format)/sizeof(format[0]));
	format[size] = 0x0;

	va_start(ap, id);
	__matrix_vsprintf_s(buf, format, ap);
	va_end(ap);

	__application_environment.log_->output_log(buf);
}

void output_log(char_t const* info){	__application_environment.log_->output_log(info);}

int get_last_error(){	return __application_environment.last_error_code_;}
void set_last_error(int error_code){__application_environment.last_error_code_ = error_code;}

/**
@fn	int init_application(char_t const* app_version)
@details 程序初始化
@param[in]	app_version	版本信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
int init_application(char_t const* app_version)
{
	int size = 0;
	char_t buf[MAX_STRING_SIZE] = {0};
	char_t format[MAX_STRING_SIZE] = {0};
	HANDLE console_handler = INVALID_HANDLE_VALUE;
	int error_code = APP_SUCCESS;
	CONSOLE_SCREEN_BUFFER_INFOEX console_screen_buffer_infoex;

	memset(&__application_environment, 0x0, sizeof(__application_environment));
	memset(&console_screen_buffer_infoex, 0x0, sizeof(console_screen_buffer_infoex));

	//initialize socket
	init_socket();

	__application_environment.version_ = app_version;
	__application_environment.manufacturer_code_ = 0x0; //指定当前制造商代码,当前值为通用参数
	__matrix_strcpy_s(__app_cmd[1].value_, sizeof(__app_cmd[1].value_)/sizeof(__app_cmd[1].value_[0]), app_version);

	console_handler = GetStdHandle(STD_OUTPUT_HANDLE);

	if(!SetConsoleTitle(app_version)){	set_last_error(IDS_STRING5002);	return error_code = APP_FAULT;}

	//console_screen_buffer_infoex.cbSize = sizeof(console_screen_buffer_infoex);
	//GetConsoleScreenBufferInfoEx(console_handler, &console_screen_buffer_infoex);
	//console_screen_buffer_infoex.dwSize.X = APP_CONSOLE_SCREEN_BUFFER_X;
	//console_screen_buffer_infoex.dwSize.Y = APP_CONSOLE_SCREEN_BUFFER_Y;
	//console_screen_buffer_infoex.dwMaximumWindowSize.X = console_screen_buffer_infoex.dwSize.X;
	//console_screen_buffer_infoex.dwMaximumWindowSize.Y = APP_CONSOLE_WINDOWS_SIZE_Y;
	//console_screen_buffer_infoex.srWindow.Right = console_screen_buffer_infoex.dwSize.X;
	//console_screen_buffer_infoex.srWindow.Bottom = APP_CONSOLE_WINDOWS_SIZE_Y;
	//SetConsoleScreenBufferInfoEx(console_handler, &console_screen_buffer_infoex);

	__application_environment.log_ = new segd::log;
	__application_environment.instance_ = GetModuleHandle(NULL);

	if(__application_environment.log_)	{	

		__application_environment.log_->init(NULL, segd::log::CONSOLE_TYPE); 

		size = get_string(IDS_STRING5003, format, sizeof(format)/sizeof(format[0]));
		format[size] = 0x0;

		__matrix_sprintf_s(buf, sizeof(buf)/sizeof(buf[0]), format, app_version);
		__application_environment.log_->output_log(buf, segd::log::NULL_INFO);
	}
	else								{	set_last_error(IDS_STRING101);  return error_code = APP_FAULT;}

	return error_code;
}

/**
@fn	void release_application()
@details 程序初始化
@param[in]	app_version	版本信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
void release_application()
{
	if(__application_environment.log_){
		__application_environment.log_->release();
		delete __application_environment.log_;
		__application_environment.log_ = NULL;
	}
	release_socket();
}

/**
@fn	int run_help_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 处理--help命令行参数
@param[in]	arg	有关该命令行的参数信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
int run_help_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	arg = arg;
	int error_code = APP_SUCCESS;
	int size = 0;
	char_t format[MAX_STRING_SIZE* 5] = {0};

	size = get_string(IDS_STRING5005, format, sizeof(format)/sizeof(format[0]));
	format[size] = 0x0;
	__application_environment.log_->output_log(format, segd::log::NULL_INFO);
	return error_code;
}

/**
@fn	int run_verbose_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 处理--verbose命令行参数
@param[in]	arg	有关该命令行的参数信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
int run_verbose_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	arg = arg;
	int error_code = APP_SUCCESS;
	__application_environment.is_verbose_ = true;
	return error_code;
}

void send_progress_info(const SEGD_WRITING_PROGRESS_INFO_PRIVATE_PTR info_ptr)
{
	char buf[MAX_PACK_NET_SIZE] = {0x0};
	int size = 0x0;
	int tmp_value = 0x0;

	tmp_value = htonl(info_ptr->step_position_);
	CopyMemory(buf + size, &tmp_value, sizeof(tmp_value));
	size += sizeof(tmp_value);

	tmp_value = htonl(info_ptr->range_);
	CopyMemory(buf + size, &tmp_value, sizeof(tmp_value));
	size += sizeof(tmp_value);

	tmp_value = wcslen(info_ptr->descrip_) + 1;
	tmp_value = htonl(tmp_value);
	CopyMemory(buf + size, &tmp_value, sizeof(tmp_value));
	size += sizeof(tmp_value);

	tmp_value = wcslen(info_ptr->descrip_) + 1;
	CopyMemory(buf + size, info_ptr->descrip_, sizeof(info_ptr->descrip_[0]) * tmp_value);
	size += sizeof(info_ptr->descrip_[0]) * tmp_value;

	udp_send(&info_ptr->sock_impl_, buf, size);
}

void convert_ints_to_floats(__in__ UCHAR const* in_array, __in__ int in_array_size, 
						 __in_out__ UCHAR* outp_array, __in_out__ int* outp_array_size)
{
	int i = 0x0;
	int size = 0x0;
	float f_val = 0.0f;
	int	val = 0x0;
	UCHAR data[4] = {0x0};
	UCHAR* in_array_ptr = const_cast<UCHAR*>(in_array);
	
	for(i = 0; i < in_array_size; i += LENGTH_PER_SAMPLE_DATA){

		__matrix_zero_Memory(&val, sizeof(val));
		memcpy(&val, in_array_ptr + i, LENGTH_PER_SAMPLE_DATA);

		f_val = 0.0f;
		if(val > 0x800000)	f_val -= (0xffffff - val);
		else				f_val = val;

		memcpy(data, &f_val, sizeof(f_val));
		outp_array[size++] = data[3];
		outp_array[size++] = data[2];
		outp_array[size++] = data[1];
		outp_array[size++] = data[0];
	}

	*outp_array_size = size;
}

/**
@fn	int get_ad_data_per_fdu_private(AD_DATA_FILE_INFO_PTR ad_data_ptr, int frame_number, 
									int current_trace_number, UCHAR* data_ptr)
@details 得到每个采集点数据
@param[in]	ad_data_ptr	当前数据文件结构信息
@param[in]	frame_number 每个采集点总共数据帧数量
@param[in]	current_trace_index 当前采集道所在全部道(包括记录道和辅助道)的索引
@param[in/out]	data_ptr	数据保存的缓冲
@return	采集点的采集数据字节数
@note	
*/
int get_ad_data_per_fdu_private(AD_DATA_FILE_INFO_PTR ad_data_ptr, 
						int frame_number, 
						int trace_number_index,
						UCHAR* data_ptr)
{
	bool b_val = true;

	int i = 0x0,
		j = 0x0,
		trace_number = get_total_trace_size(ad_data_ptr),
		file_header_size = get_file_header_size(),
		data_size_per_trace = SAMPLE_DATA_PACKAGE_SIZE * 3 + TB_TIME_LENGTH,
		data_size_per_frame = trace_number * data_size_per_trace * ad_data_ptr->file_header_.receiver_line_number_;

	int tmp_val = 0x0;
	DWORD read_data_size = 0x0;
	LARGE_INTEGER file_pointer = {0x0};
	UCHAR tmp_buf[TB_TIME_LENGTH] = {0};

	for(i = 0, j = 0; i < frame_number; ++i){

		file_pointer.QuadPart = file_header_size; //file data header size
		file_pointer.QuadPart += (data_size_per_frame * i); // frame size
		file_pointer.QuadPart += trace_number_index * data_size_per_trace; // line size + trace size

		if(b_val){

			SetFilePointerEx(ad_data_ptr->file_handle_, file_pointer, NULL, FILE_BEGIN);
			tmp_val = i * data_size_per_trace;
			ReadFile(ad_data_ptr->file_handle_, tmp_buf, sizeof(tmp_buf), &read_data_size, NULL);
			CopyMemory(&tmp_val,  tmp_buf, sizeof(ad_data_ptr->file_header_.tb_time_));

			//根据TB_time的时间要求, 得到有效数据信息, 数据帧时间<TB_time
			if(ad_data_ptr->file_header_.tb_time_ < tmp_val){
				if(i >2)	i -= 2;
				else		i = 0;		
				b_val = false;
			}
			else if(ad_data_ptr->file_header_.tb_time_ == tmp_val){
				b_val = false;
				if(i > 0)	--i;
				else		i = 0;
			}
		}
		else{

			tmp_val = data_size_per_trace - TB_TIME_LENGTH;// - TB_TIME_LENGTH;
			file_pointer.QuadPart += TB_TIME_LENGTH;
			SetFilePointerEx(ad_data_ptr->file_handle_, file_pointer, NULL, FILE_BEGIN);
			ReadFile(ad_data_ptr->file_handle_, 
				data_ptr + j * tmp_val, //data_ptr偏移量, j第一次使用为0
				tmp_val, &read_data_size, NULL);
			++j;
		}
	}

	return tmp_val = j * (data_size_per_trace - TB_TIME_LENGTH);
}

/**
@fn	int get_ad_data_per_fdu(AD_DATA_FILE_INFO_PTR ad_data_ptr, 
							int frame_number, 
							int receiver_line_indx,
							int trace_indx,
							UCHAR* data_ptr)
@details 得到每个采集点数据
@param[in]	
@return	
@note	
*/
int get_ad_data_per_fdu(AD_DATA_FILE_INFO_PTR ad_data_ptr, 
						 int frame_number, 
						 int receiver_line_indx,
						 int trace_indx,
						 UCHAR* data_ptr)
{
	int trace_number = get_trace_size(ad_data_ptr);
	trace_number *= receiver_line_indx;
	trace_number += trace_indx;
	return get_ad_data_per_fdu_private(ad_data_ptr, frame_number, trace_number, data_ptr);
}

bool compare_segd_data_size(AD_DATA_FILE_INFO_PTR ad_data_ptr, unsigned int inp_data_size, unsigned int oup_data_size)
{
	_ASSERTE(ad_data_ptr);
	bool is_ok = true;

	switch(ad_data_ptr->manufacturer_code_){
		case 0://genernal manufacturer code
			if(inp_data_size < oup_data_size) is_ok = false;
				break;
		case 13:// sercel code
			if(inp_data_size < oup_data_size - 1) is_ok = false;
			break;
		default:
			if(inp_data_size < oup_data_size) is_ok = false;
			break;
	}

	return is_ok;
}

int write_segd_data_private(SEGD_CONFIG_FILE_PTR segd_config_info_ptr,
					SEGD_FILE_PTR outp_file_ptr,
					SEGD_WRITING_PROGRESS_INFO_PRIVATE_PTR progress_info_ptr,
					SPS_DATA_INFO_PTR sps_data_info_ptr, 
					AD_DATA_FILE_INFO_PTR ad_data_ptr)
{
	int is_ok = APP_SUCCESS;
	int i = 0x0, 
		trace_number = get_total_trace_size(ad_data_ptr),
		frame_number = get_data_frame_size(ad_data_ptr),
		receiver_line_number = ad_data_ptr->file_header_.receiver_line_number_,
		data_size_per_trace = SAMPLE_DATA_PACKAGE_SIZE * 3 + TB_TIME_LENGTH,
		total_data_size_per_trace = frame_number * data_size_per_trace,
		total_outp_data_size_pertrace = frame_number * SAMPLE_DATA_PACKAGE_SIZE * 4,
		valid_segd_data_size = get_data_size(ad_data_ptr),
		aut_trace_number = get_aux_trace_size(ad_data_ptr),
		j = 0, 
		k = 0,
		tmp_val = 0,
		tmp_val2 = 0;

	SEGD_FILE_TRACE_HEADER trace_header;
	SPS_RELATION_RECORD_SPEC_PTR relation_record_ptr = NULL;
	SPS_POINT_RECORD_SPEC_PTR point_record_ptr = NULL;
	UCHAR* data_ptr = NULL;
	UCHAR* outp_data_ptr = NULL;

	data_ptr = (UCHAR*)malloc(total_data_size_per_trace);
	if(data_ptr == NULL){	set_last_error(IDS_STRING5032); output_log(IDS_STRING5032); is_ok = APP_FAULT; goto GOTO;}
	else __matrix_zero_Memory(data_ptr, total_data_size_per_trace);

	outp_data_ptr = (UCHAR*)malloc(total_outp_data_size_pertrace);
	if(outp_data_ptr == NULL){	set_last_error(IDS_STRING5032); output_log(IDS_STRING5032); is_ok = APP_FAULT; goto GOTO;}
	else __matrix_zero_Memory(outp_data_ptr, total_outp_data_size_pertrace);

	for(i = 0; i < receiver_line_number; ++i, j = 0){

		relation_record_ptr = sps_data_info_ptr->head_relation_records_ptr_;
		//得到接受线信息
		while(relation_record_ptr != NULL){	
			if(j == i){	break;}
			else{relation_record_ptr = relation_record_ptr->next_; ++j;}
		}

		if(relation_record_ptr == NULL){	set_last_error(IDS_STRING5031); output_log(IDS_STRING5031); goto GOTO;}

		//写每一个跟踪点信息到SEGD
		if(trace_number == 0){
			trace_number = (relation_record_ptr->to_channel_ - relation_record_ptr->from_channel_) / relation_record_ptr->channel_increment_;
			++trace_number;
		}

		//测试SPS通道数量是否大于测道数量,如果大于测道数量,否则将报错,并且结束程序
		j = (relation_record_ptr->to_channel_ -  relation_record_ptr->from_channel_) / relation_record_ptr->channel_increment_;
		if(j < (ad_data_ptr->file_header_.trace_total_number_ - 
				(ad_data_ptr->file_header_.auxiliary_line_number_ * 
				ad_data_ptr->file_header_.auxiliary_trace_number_))){

			set_last_error(IDS_STRING5045);
			output_log(get_last_error());
			is_ok = APP_FAULT;
			goto GOTO;			
		}

		for(j = relation_record_ptr->from_channel_; j < relation_record_ptr->to_channel_; 
			j += relation_record_ptr->channel_increment_, k = 0){
			
			tmp_val = (j - relation_record_ptr->from_channel_)/relation_record_ptr->channel_increment_;
			if(tmp_val == (trace_number - aut_trace_number)) break;

			//write trace header
			__matrix_zero_Memory(data_ptr, total_data_size_per_trace);
			__matrix_zero_Memory(&trace_header, sizeof(trace_header));
			CopyMemory(&trace_header, &segd_config_info_ptr->trace_header_, sizeof(trace_header));

			point_record_ptr = relation_record_ptr->head_receiver_point_ptr_;
			while(point_record_ptr != NULL){
				if(k == tmp_val) break;
				else{point_record_ptr = point_record_ptr->next_; ++k;}	
			}

			if(point_record_ptr == NULL){	
				set_last_error(IDS_STRING5041); 
				output_log(IDS_STRING5041); 
				goto GOTO;
			}

			trace_header.demux_trace_header_.trace_number_ = j;
			trace_header.demux_trace_header_.channel_set_number_ = i + 1;
			trace_header.trace_header_extended_1_.receiver_line_number_ = point_record_ptr->line_name_;
			trace_header.trace_header_extended_1_.receiver_point_number_ = point_record_ptr->point_number_;
			trace_header.trace_header_extended_2_.receiver_point_east_ = point_record_ptr->easting_;
			trace_header.trace_header_extended_2_.receiver_point_high_ = point_record_ptr->elevation_;
			trace_header.trace_header_extended_2_.receiver_point_north_ = point_record_ptr->northing_;
			trace_header.trace_header_extended_2_.extended_trace_number_ = j;

			is_ok = segd_file_write_trace_header(outp_file_ptr, &trace_header);
			if(!is_ok){
				set_last_error(GetLastError());
				output_log(get_last_error());
				is_ok = APP_FAULT;
				goto GOTO;
			}			

			//得到每个采集点数据
			tmp_val2 = get_ad_data_per_fdu(ad_data_ptr, frame_number, i, tmp_val, data_ptr);

			//写入数据
			if(!compare_segd_data_size(ad_data_ptr, tmp_val2 / 3, valid_segd_data_size / 4)){
			//if(tmp_val2 < ad_data_ptr->file_header_.sample_length_){
				set_last_error(IDS_STRING5044);
				output_log(get_last_error());
				is_ok = APP_FAULT;
				goto GOTO;
			}

			convert_ints_to_floats(data_ptr, tmp_val2, outp_data_ptr, &total_outp_data_size_pertrace);
			segd_file_write_trace_data(outp_file_ptr, outp_data_ptr, valid_segd_data_size);
			progress_info_ptr->step_position_ += (1000/(trace_number*receiver_line_number));
			send_progress_info(progress_info_ptr);
		}
	}

	//12. 写入SEG-D文件尾
	__matrix_zero_Memory(outp_data_ptr, total_outp_data_size_pertrace);
	for(k = get_trace_size(ad_data_ptr), 
		i = 0; i < ad_data_ptr->file_header_.auxiliary_line_number_; ++i){

		for(j = 0; j < ad_data_ptr->file_header_.auxiliary_trace_number_; ++j, ++k){

			segd_config_info_ptr->extended_header_.auxiliary_trace_number_ = 1;
			__matrix_zero_Memory(&trace_header, sizeof(trace_header));
			CopyMemory(&trace_header, &segd_config_info_ptr->trace_header_, sizeof(trace_header));
			trace_header.demux_trace_header_.channel_set_number_ = ad_data_ptr->file_header_.receiver_line_number_ + i + 1;
			trace_header.demux_trace_header_.trace_number_ = j + 1;
			trace_header.trace_header_extended_2_.extended_trace_number_ = j + 1;
			trace_header.trace_header_extended_1_.receiver_line_number_ = 0xffffff;
			trace_header.trace_header_extended_1_.receiver_point_number_ = 0xffffff;
			is_ok = segd_file_write_trace_header(outp_file_ptr, &trace_header);

			if(!is_ok){	
				set_last_error(GetLastError());	
				output_log(get_last_error()); 
				is_ok = APP_FAULT;
				goto GOTO;
			}
			else{ 
				is_ok = APP_SUCCESS;
			}
		
			//得到每个采集点数据
			tmp_val2 = get_ad_data_per_fdu_private(ad_data_ptr, frame_number, k, data_ptr);

			//写入数据
			if(!compare_segd_data_size(ad_data_ptr, tmp_val2 / 3, valid_segd_data_size / 4)){
			//if(tmp_val2 < ad_data_ptr->file_header_.sample_length_){
				set_last_error(IDS_STRING5044);
				output_log(get_last_error());
				is_ok = APP_FAULT;
				goto GOTO;
			}

			convert_ints_to_floats(data_ptr, tmp_val2, outp_data_ptr, &total_outp_data_size_pertrace);
			segd_file_write_trace_data(outp_file_ptr, outp_data_ptr, valid_segd_data_size);	
		}
	}

	get_string(IDS_STRING5040, progress_info_ptr->descrip_, sizeof(progress_info_ptr->descrip_)/sizeof(progress_info_ptr->descrip_[0]));
	progress_info_ptr->step_position_ = progress_info_ptr->range_;
	send_progress_info(progress_info_ptr);

GOTO:
	if(data_ptr){	free(data_ptr);}
	if(outp_data_ptr){	free(outp_data_ptr);}
	return is_ok;
}

/**
@fn	int run_tb_time_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 处理--tb-time命令行参数
@param[in]	arg	有关该命令行的参数信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
int run_tb_time_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	int error_code = APP_SUCCESS;
	__application_environment.tb_time_ = _tstoi(arg->value_);
	return error_code;
}

/**
@fn	int run_data_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 处理--data-file命令行参数
@param[in]	arg	有关该命令行的参数信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
int run_data_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	int error_code = APP_SUCCESS, i = 0x0, trace_group_value = 0x0;
	SEGD_CONFIG_FILE_PTR segd_config_info_ptr = NULL;
	SEGD_FILE_PTR outp_file_ptr = NULL;
	SEGD_WRITING_PROGRESS_INFO_PRIVATE progress_info;
	SPS_DATA_INFO sps_data_info;
	AD_DATA_FILE_INFO ad_data;

	__matrix_zero_Memory(&progress_info, sizeof(progress_info));
	__matrix_zero_Memory(&sps_data_info, sizeof(sps_data_info));

	progress_info.range_ = 1000;

	if(load_data_file(arg->value_, &ad_data) != APP_SUCCESS){	error_code = APP_FAULT;	goto GOTO;}

	if(strlen(ad_data.file_header_.config_path_) == 0){
		
		char_t tmp_buf[MAX_STRING_SIZE] = {0};
		char_t tmp_driver_buf[MAX_STRING_SIZE] = {0};
		char_t tmp_dir_buf[MAX_STRING_SIZE] = {0};
		DWORD tmp_buf_size = 0x0;

		tmp_buf_size = GetModuleFileName(NULL, tmp_buf, sizeof(tmp_buf)/sizeof(tmp_buf[0]));
		tmp_buf[tmp_buf_size] = 0x0;

		__matrix_splitpath_s(tmp_buf, tmp_driver_buf, sizeof(tmp_driver_buf)/sizeof(tmp_driver_buf[0]),
							tmp_dir_buf, sizeof(tmp_dir_buf)/sizeof(tmp_dir_buf[0]), NULL, NULL, NULL, NULL);

		__matrix_sprintf_s(__application_environment.config_file_path_, 
							sizeof(__application_environment.config_file_path_)/sizeof(__application_environment.config_file_path_[0]), 
							__TEXT("%s%s%s"), tmp_driver_buf, tmp_dir_buf, CONF_FILE_NAME);
	}
	else{
		size_t mbcs_size = 0x0;

		__matrix_zero_Memory(__application_environment.config_file_path_, sizeof(__application_environment.config_file_path_));
		mbstowcs_s(&mbcs_size, NULL, 0, ad_data.file_header_.config_path_, 0);
		mbstowcs_s(&mbcs_size, __application_environment.config_file_path_, mbcs_size, ad_data.file_header_.config_path_, mbcs_size);
		__application_environment.config_file_path_[mbcs_size] = 0x0;
	}

	ad_data.file_header_.tb_time_ = __application_environment.tb_time_;
	ad_data.manufacturer_code_ = __application_environment.manufacturer_code_;

	//0.根据数据文件,修改配置文件
	//modify_config_info(__application_environment.config_file_path_, &ad_data);

	//1. 检测配置文件是否正确
	//if(check_config_file(__application_environment.config_file_path_) != APP_SUCCESS){	error_code = APP_FAULT;	goto GOTO;}

	//2. 读取配置文件
	if(load_config_file(__application_environment.config_file_path_) != APP_SUCCESS){	error_code = APP_FAULT;	goto GOTO;}

	//3. 得到配置信息 & 创建socket
	segd_config_info_ptr = get_segd_config_info_ptr();

	//根据数据文件,修改配置文件
	modify_config_info(&ad_data, segd_config_info_ptr);
	
	progress_info.sock_impl_.remote_port_ = segd_config_info_ptr->remote_port_;
	progress_info.sock_impl_.send_buffer_size_ = 0x80000;
	if(!create_udp_socket(&progress_info.sock_impl_, segd_config_info_ptr->remote_ip_)){
		set_last_error(IDS_STRING5015);
		output_log(IDS_STRING5015);
		error_code = APP_FAULT;
		goto GOTO;
	}

	//4. 获取SPS数据信息
	progress_info.range_ = 1000;
	progress_info.step_position_ = 0;

	get_string(IDS_STRING5033, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);

	sps_data_info.sps_path_ptr_ = segd_config_info_ptr->sps_path_;
	sps_data_info.source_line_number_ = ad_data.file_header_.source_line_number_;
	sps_data_info.source_point_number_ = ad_data.file_header_.source_point_number_;
	load_sps_file(&sps_data_info);

	get_string(IDS_STRING5034, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);

	//5. 创建SEG-D文件
	get_string(IDS_STRING5035, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);

	outp_file_ptr = segd_file_create(segd_config_info_ptr->output_path_, SEGD_FILE_CREATE | SEGD_FILE_IO_NON_OVERLAPPED);
	if(outp_file_ptr == NULL){
		set_last_error(IDS_STRING5014);
		output_log(IDS_STRING5014, segd_config_info_ptr->output_path_);
		error_code = APP_FAULT;
		goto GOTO;
	}

	//6. 写入普通头段数据块1
	get_string(IDS_STRING5036, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);

	merge_segd_header_1(&sps_data_info, &segd_config_info_ptr->header_1_);
	memcpy_s(&outp_file_ptr->header_1_, sizeof(outp_file_ptr->header_1_), &segd_config_info_ptr->header_1_, sizeof(segd_config_info_ptr->header_1_));
	if(!segd_file_write_genernal_header_1(outp_file_ptr, &outp_file_ptr->header_1_)){

		set_last_error(GetLastError());
		output_log(get_last_error());
		error_code = APP_FAULT;
		goto GOTO;
	}

	//7. 写入普通头段数据块2
	merge_segd_header_2(&sps_data_info, &segd_config_info_ptr->header_2_);
	memcpy_s(&outp_file_ptr->header_2_, sizeof(outp_file_ptr->header_2_), &segd_config_info_ptr->header_2_, sizeof(segd_config_info_ptr->header_2_));
	if(!segd_file_write_genernal_header_2(outp_file_ptr, &outp_file_ptr->header_2_)){

		set_last_error(GetLastError());
		output_log(get_last_error());
		error_code = APP_FAULT;
		goto GOTO;
	}

	//8. 写入普通头段数据块3
	merge_segd_header_3(&sps_data_info, &segd_config_info_ptr->header_3_);
	memcpy_s(&outp_file_ptr->header_3_, sizeof(outp_file_ptr->header_3_), &segd_config_info_ptr->header_3_, sizeof(segd_config_info_ptr->header_3_));
	if(segd_config_info_ptr->header_1_.additional_blocks_in_general_header_ == 2){

		if(!segd_file_write_genernal_header_3(outp_file_ptr, &outp_file_ptr->header_3_)){

			set_last_error(GetLastError());
			output_log(get_last_error());
			error_code = APP_FAULT;
			goto GOTO;
		}
	}

	//9. 写入扫描类型头段
	get_string(IDS_STRING5037, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);

	if((ad_data.file_header_.receiver_line_number_ + ad_data.file_header_.auxiliary_line_number_) 
		> segd_config_info_ptr->header_1_.channal_sets_scan_type_){	

		set_last_error(IDS_STRING5044); output_log(get_last_error());error_code = APP_FAULT; goto GOTO;
	}
	else{
		i = sizeof(SEGD_SCAN_TYPE_HEADER) * segd_config_info_ptr->header_1_.channal_sets_scan_type_;
		outp_file_ptr->scan_type_header_ = (SEGD_SCAN_TYPE_HEADER_PTR)malloc(i);
		__matrix_zero_Memory(outp_file_ptr->scan_type_header_, i);
	}

	//9.1写入记录道组信息
	for(trace_group_value = 1, i = 0; i< ad_data.file_header_.receiver_line_number_; ++i, ++trace_group_value){

		//__matrix_zero_Memory(&segd_scan_type_header, sizeof(segd_scan_type_header));
		if(i < sps_data_info.relation_records_size_){

			merge_segd_scan_type_header(&sps_data_info, &segd_config_info_ptr->scan_type_header_);
			memcpy(outp_file_ptr->scan_type_header_ + (trace_group_value - 1), 
				&segd_config_info_ptr->scan_type_header_, sizeof(SEGD_SCAN_TYPE_HEADER));

			//merge_segd_scan_type_header(&sps_data_info, &segd_config_info_ptr->scan_type_header_);
			//memcpy(&segd_scan_type_header, &segd_config_info_ptr->scan_type_header_, sizeof(segd_scan_type_header));
		}
		//segd_scan_type_header.channel_set_number_ = trace_group_value;
		outp_file_ptr->scan_type_header_[trace_group_value - 1].channel_set_number_ = trace_group_value;
		outp_file_ptr->scan_type_header_[trace_group_value - 1].channels_number_ = 
			ad_data.file_header_.trace_total_number_ - ad_data.file_header_.auxiliary_trace_number_ * ad_data.file_header_.auxiliary_line_number_;

		if(!segd_file_write_scan_type_header(outp_file_ptr, &outp_file_ptr->scan_type_header_[trace_group_value - 1])){
			set_last_error(GetLastError());	output_log(get_last_error()); error_code = APP_FAULT; goto GOTO;}
	}

	//9.2写入辅助道组信息
	for(i = 0; i< ad_data.file_header_.auxiliary_line_number_; ++i, ++trace_group_value){

		memcpy(outp_file_ptr->scan_type_header_ + trace_group_value - 1, outp_file_ptr->scan_type_header_, sizeof(SEGD_SCAN_TYPE_HEADER));
		outp_file_ptr->scan_type_header_[trace_group_value - 1].channels_number_ = ad_data.file_header_.auxiliary_trace_number_;
		outp_file_ptr->scan_type_header_[trace_group_value - 1].channel_type_identification_ = 9;
		outp_file_ptr->scan_type_header_[trace_group_value - 1].channel_set_number_ = trace_group_value;
		if(!segd_file_write_scan_type_header(outp_file_ptr, &outp_file_ptr->scan_type_header_[trace_group_value - 1])){
			set_last_error(GetLastError());	output_log(get_last_error()); error_code = APP_FAULT; goto GOTO;}
	}

	trace_group_value = ad_data.file_header_.receiver_line_number_ + ad_data.file_header_.auxiliary_line_number_;
	for(i = trace_group_value; i < segd_config_info_ptr->header_1_.channal_sets_scan_type_; ++i){

		__matrix_zero_Memory(outp_file_ptr->scan_type_header_ + i, sizeof(SEGD_SCAN_TYPE_HEADER));
		if(!segd_file_write_scan_type_header(outp_file_ptr, &outp_file_ptr->scan_type_header_[i])){
			set_last_error(GetLastError());	output_log(get_last_error()); error_code = APP_FAULT; goto GOTO;}
	}

	//10. 写入扩展头段
	get_string(IDS_STRING5038, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);

	if(segd_config_info_ptr->header_1_.extended_header_block_ == 32){

		merge_segd_extend_header(&sps_data_info, &segd_config_info_ptr->extended_header_);
		memcpy_s(&outp_file_ptr->extended_header_, sizeof(outp_file_ptr->extended_header_), &segd_config_info_ptr->extended_header_, sizeof(segd_config_info_ptr->extended_header_));
		if(!segd_file_write_extended_header(outp_file_ptr, &outp_file_ptr->extended_header_)){

			set_last_error(GetLastError());
			output_log(get_last_error());
			error_code = APP_FAULT;
			goto GOTO;
		}
	}

	//11. 写入记录道数据块 + 写入AD数据
	progress_info.range_ = 1000;
	progress_info.step_position_ = 0;
	get_string(IDS_STRING5039, progress_info.descrip_, sizeof(progress_info.descrip_)/sizeof(progress_info.descrip_[0]));
	send_progress_info(&progress_info);
	
	write_segd_data_private(segd_config_info_ptr, outp_file_ptr, &progress_info, &sps_data_info, &ad_data);
GOTO:
	if(error_code == APP_FAULT){

		progress_info.range_ = 0;
		progress_info.step_position_ = 0;
		__matrix_sprintf_s(progress_info.descrip_, sizeof(progress_info.descrip_) / sizeof(char_t), __TEXT("error: %d"), get_last_error());
		send_progress_info(&progress_info);
	}
	free_sps_data_memory(&sps_data_info);
	CloseHandle(ad_data.file_handle_);
	segd_file_close(outp_file_ptr);
	close_udp_socket(&progress_info.sock_impl_);
	return error_code;
}

int run_version_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	int error_code = APP_SUCCESS;
	int size = 0;
	char_t format[MAX_STRING_SIZE] = {0};
	char_t buf[MAX_STRING_SIZE] = {0};

	size = get_string(IDS_STRING5003, format, sizeof(format)/sizeof(format[0]));
	format[size] = 0x0;
	__matrix_sprintf_s(buf, sizeof(buf)/sizeof(buf[0]), format, arg->value_);
	__application_environment.log_->output_log(buf, segd::log::NULL_INFO);
	return error_code;
}

int read_segd_file_private(SEGD_FILE_PTR read_segd_ptr)
{
	bool is_ok = true;
	DWORD err = 0x0;
	int size = 0x0, i = 0;
	char_t buf[MAX_STRING_SIZE] = {0x0};

	//general header #1
	is_ok = segd_file_read_genernal_header_1(read_segd_ptr, &read_segd_ptr->header_1_);
	if(!is_ok){
		err = GetLastError();
		_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
		__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
		return APP_FAULT;
	}

	//general header #2
	is_ok = segd_file_read_genernal_header_2(read_segd_ptr, &read_segd_ptr->header_2_);
	if(!is_ok){
		err = GetLastError();
		_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
		__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
		return APP_FAULT;
	}

	//general header #3
	if(read_segd_ptr->header_1_.additional_blocks_in_general_header_ == 2){

		is_ok = segd_file_read_genernal_header_3(read_segd_ptr, &read_segd_ptr->header_3_);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	//scan type header #
	if(read_segd_ptr->header_1_.channal_sets_scan_type_ > 0){

		size = read_segd_ptr->header_1_.channal_sets_scan_type_ * sizeof(SEGD_SCAN_TYPE_HEADER);
		read_segd_ptr->scan_type_header_ = (SEGD_SCAN_TYPE_HEADER_PTR)malloc(size);
		__matrix_zero_Memory(read_segd_ptr->scan_type_header_, size);

		for(i = 0; i < read_segd_ptr->header_1_.channal_sets_scan_type_; ++i){

			is_ok = segd_file_read_scan_type_header(read_segd_ptr, read_segd_ptr->scan_type_header_ + i);
			if(!is_ok){
				err = GetLastError();
				_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
				__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
				return APP_FAULT;
			}
		}
	}

	//extended header
	if(read_segd_ptr->header_1_.extended_header_block_ == 32){

		//general header
		is_ok = segd_file_read_extended_header(read_segd_ptr, &read_segd_ptr->extended_header_);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	//external header
	if(read_segd_ptr->header_1_.external_header_block_ > 0){

		size = read_segd_ptr->header_1_.external_header_block_ * sizeof(SEGD_UCHAR) * 32;
		read_segd_ptr->external_header_.series_ = (SEGD_UCHAR*)malloc(size);
		__matrix_zero_Memory(read_segd_ptr->external_header_.series_, size);

		//read it
		is_ok = segd_file_read_external_header(read_segd_ptr, size, &read_segd_ptr->external_header_);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf,segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	if(read_segd_ptr->header_1_.extended_header_block_ == 32){

		//trace header + data
		if(read_segd_ptr->extended_header_.trace_number_ > 0){

			i = read_segd_ptr->extended_header_.trace_number_ * sizeof(SEGD_FILE_TRACE_HEADER);
			read_segd_ptr->trace_headers_ = (SEGD_FILE_TRACE_HEADER_PTR)malloc(i);
			__matrix_zero_Memory(read_segd_ptr->trace_headers_, i);
		}

		for(i = 0; i < read_segd_ptr->extended_header_.trace_number_; ++i){

			is_ok = segd_file_read_trace_header(read_segd_ptr, read_segd_ptr->trace_headers_ + i);
			if(!is_ok){
				err = GetLastError();
				_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
				__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
				return APP_FAULT;
			}

			//segd data
			if(read_segd_ptr->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

				size = segd_file_get_trace_data_size(read_segd_ptr);
				read_segd_ptr->trace_headers_[i].trace_data_ = (SEGD_UCHAR*)malloc(size);
				segd_file_read_trace_data(read_segd_ptr, read_segd_ptr->trace_headers_[i].trace_data_, size);
			}
		}

		//aux trace header + data
		if(read_segd_ptr->extended_header_.auxiliary_trace_number_ > 0){

			size = read_segd_ptr->extended_header_.auxiliary_trace_number_ * sizeof(SEGD_FILE_TRACE_HEADER);
			read_segd_ptr->aux_trace_headers_ = (SEGD_FILE_TRACE_HEADER_PTR)malloc(size);
			__matrix_zero_Memory(read_segd_ptr->aux_trace_headers_, size);

			for(i = 0; i < read_segd_ptr->extended_header_.auxiliary_trace_number_; ++i){

				is_ok = segd_file_read_trace_header(read_segd_ptr, read_segd_ptr->aux_trace_headers_ + i);
				if(!is_ok){
					err = GetLastError();
					_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
					__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
					return APP_FAULT;
				}

				//trace data
				if(read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

					size = segd_file_get_trace_data_size(read_segd_ptr);
					read_segd_ptr->aux_trace_headers_[i].trace_data_ = (SEGD_UCHAR*)malloc(size);
					segd_file_read_trace_data(read_segd_ptr, read_segd_ptr->aux_trace_headers_[i].trace_data_, size);
				}
			}			
		}
	}

	return APP_SUCCESS; 
}

int write_segd_ptr_private(SEGD_FILE_PTR write_segd_ptr, SEGD_FILE_PTR segd_file)
{
	bool is_ok = true;
	DWORD err = 0x0;
	int i = 0x0, size = 0x0;
	char_t buf[MAX_STRING_SIZE] = {0x0};

	is_ok = segd_file_write_genernal_header_1(write_segd_ptr, &segd_file->header_1_);
	if(!is_ok){
		err = GetLastError();
		_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
		__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
		return APP_FAULT;
	}

	is_ok = segd_file_write_genernal_header_2(write_segd_ptr, &segd_file->header_2_);
	if(!is_ok){
		err = GetLastError();
		_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
		__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
		return APP_FAULT;
	}

	if(segd_file->header_1_.additional_blocks_in_general_header_ == 2){

		is_ok = segd_file_write_genernal_header_3(write_segd_ptr, &segd_file->header_3_);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	for(i = 0; i < segd_file->header_1_.channal_sets_scan_type_; ++i){

		is_ok = segd_file_write_scan_type_header(write_segd_ptr, segd_file->scan_type_header_ + i);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	//extended header
	if(segd_file->header_1_.extended_header_block_ == 32){

		is_ok = segd_file_write_extended_header(write_segd_ptr, &segd_file->extended_header_);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	//external header
	if(segd_file->header_1_.external_header_block_ > 0){

		is_ok = segd_file_write_external_header(write_segd_ptr, 
			segd_file->header_1_.external_header_block_ * sizeof(UCHAR) * 32, 
			&segd_file->external_header_);
		if(!is_ok){
			err = GetLastError();
			_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
			__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
			return APP_FAULT;
		}
	}

	if(segd_file->header_1_.extended_header_block_ == 32){

		//trace header + data
		for(i = 0; i < segd_file->extended_header_.trace_number_; ++i){

			is_ok = segd_file_write_trace_header(write_segd_ptr, segd_file->trace_headers_ + i);
			if(!is_ok){
				err = GetLastError();
				_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
				__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
				return APP_FAULT;
			}

			//segd data
			if(segd_file->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

				size = segd_file_get_trace_data_size(segd_file);
				segd_file_write_trace_data(write_segd_ptr, segd_file->trace_headers_[i].trace_data_, size);
			}
		}

		//aux trace header + data
		if(segd_file->extended_header_.auxiliary_trace_number_ > 0){

			for(i = 0; i < segd_file->extended_header_.auxiliary_trace_number_; ++i){

				is_ok = segd_file_write_trace_header(write_segd_ptr, segd_file->aux_trace_headers_ + i);
				if(!is_ok){
					err = GetLastError();
					_stprintf_s(buf, sizeof(buf), __TEXT("spawn a error: %d"), err);
					__application_environment.log_->output_log(buf, segd::log::ERROR_INFO);
					return APP_FAULT;
				}

				if(segd_file->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_ > 0){

					size = segd_file_get_trace_data_size(segd_file);
					segd_file_write_trace_data(write_segd_ptr, segd_file->aux_trace_headers_[i].trace_data_, size);
				}
			}			
		}
	}

	return APP_SUCCESS;
}

/**
@fn	int run_reading_and_writing_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 读取并分析SEGD, 并生成新文件
@param[in]	argc	参数个数
@param[in]	argv	参数数组
@return	如果失败返回false,否则返回true
@note	当前使用char_t
*/
int run_reading_and_writing_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	int error_code = APP_SUCCESS;
	char_t path_name[MAX_STRING_SIZE] = {0};
	SEGD_FILE_PTR read_segd_ptr = NULL;
	SEGD_FILE_PTR write_segd_ptr = NULL;

	StrCpy(path_name, arg->value_);
	read_segd_ptr = segd_file_create(path_name, SEGD_FILE_OPEN_EXISTING | SEGD_FILE_IO_OVERLAPPED);

	if(_tcslen(__application_environment.output_log_) == 0){
		StrCpy(__application_environment.output_log_, __TEXT("write.segd.file.sgd"));
	}

	write_segd_ptr = segd_file_create(__application_environment.output_log_, SEGD_FILE_CREATE);

	if(read_segd_file_private(read_segd_ptr) == APP_FAULT)	return error_code = APP_FAULT;
	if(write_segd_ptr_private(write_segd_ptr, read_segd_ptr) == APP_FAULT)	return error_code = APP_FAULT;

	segd_file_close(read_segd_ptr);
	segd_file_close(write_segd_ptr);

	return error_code;
}

/**
@fn	bool check_cmd_args(int argc, char_t* argv[])
@details 检查命令行参数有效性
@param[in]	argc	参数个数
@param[in]	argv	参数数组
@return	如果失败返回false,否则返回true
@note	当前使用char_t
*/
bool check_cmd_args(int argc, char_t* argv[])
{
	bool is_ok = true;
	int i = 0, j = 0, k = 0, l = 0;
	
	__application_environment.last_error_code_ = IDS_STRING5004;
	memset(&__application_environment.args_, 0x0, sizeof(__application_environment.args_));
	__application_environment.arg_size_ = 0x0;
	
	//处理参数
	while(i < argc){		
		for(j = 0; j < __app_cmd_size; ++j){
			if(__matrix_stricmp(argv[i], __app_cmd[j].arg_name_) == 0){

				__application_environment.args_[__application_environment.arg_size_] = j;
				for(k = 0; k < __app_cmd[j].value_size_; ++k){

					if(__matrix_strstr(argv[i + k + 1], __TEXT("--")) != NULL){	
						set_last_error(IDS_STRING5006);
						output_log(IDS_STRING5006, argv[i]);
						is_ok = false;
						goto GOTO;
					}
					__matrix_strcpy_s(__app_cmd[j].value_, sizeof(__app_cmd[j].value_)/sizeof(__app_cmd[j].value_[0]), argv[i + k + 1]);
					++i;
				}//end for
				++__application_environment.arg_size_;
				break;
			}//end if
		}//end for
		++i;
	}//end while

	//命令行优先级处理排序
	for(i = 0; i < __application_environment.arg_size_; ++i){
		for(j = i + 1; j < __application_environment.arg_size_; ++j){
			
			k = __application_environment.args_[i];
			l = __application_environment.args_[j];
			if(__app_cmd[k].priority_ > __app_cmd[l].priority_){ 
				__application_environment.args_[i] = l; 
				__application_environment.args_[j] = k;
			}
			else if(__app_cmd[k].priority_ == __app_cmd[l].priority_){
				set_last_error(IDS_STRING5007);
				output_log(IDS_STRING5007, __app_cmd[k].arg_name_, __app_cmd[l].arg_name_);
				is_ok = false;
				goto GOTO;
			}
		}
	}

GOTO:

	return is_ok;
}

int main_private(int argc, char_t* argv[], char_t const* app_version)
{
	int i = 0, index = 0x0;
	int error_code = 0x0;

	error_code = init_application(app_version);
	if(error_code == APP_FAULT)	return error_code = __application_environment.last_error_code_;
	
	if(!check_cmd_args(argc, argv)){
		output_log(get_last_error());
		goto GOTO;
	}

	//run
	for(i = 0; i < __application_environment.arg_size_; ++i){
		index = __application_environment.args_[i];
		if(__app_cmd[index].function_ptr(__app_cmd + index) != APP_SUCCESS){
			error_code = get_last_error();
			break;
		}
	}

GOTO:
	release_application();
	return error_code;
}

/**
@fn	int run_output_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 输出日志文本文件
@param[in]	argc	参数个数
@param[in]	argv	参数数组
@return	如果失败返回false,否则返回true
@note	当前使用char_t
*/
int run_output_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	StrCpy(__application_environment.output_log_, arg->value_);
	return APP_SUCCESS;
}

void output_segd_log(SEGD_FILE_PTR read_segd_ptr, char_t const* output_log)
{
	int j = 0,i = 0, k = 0;
	HANDLE handle = 0x0;
	DWORD buf_size_dat = 0x0;
	char buf_file_name[MAX_PATH] = {0x0};
	float f_val = 0.0f;
	UCHAR data[4] = {0x0};
	char buf[MAX_PATH] = {0x0};
	int size = 0x0;

	wcstombs(buf_file_name, output_log, wcslen(output_log) + 1);
	handle = CreateFileA(buf_file_name, FILE_ALL_ACCESS, FILE_SHARE_WRITE, NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL, NULL);

	wcstombs(buf_file_name, read_segd_ptr->path_name_, wcslen(read_segd_ptr->path_name_) + 1);
	sprintf_s(buf, sizeof(buf), "SEGD File: %s\n", buf_file_name);
	WriteFile(handle, buf, strlen(buf) + 1, &buf_size_dat, NULL);

	strcpy(buf,"SEGD_GENERAL_HEADER 1:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "file_number: %d\n", read_segd_ptr->header_1_.file_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "format_code: %d\n", read_segd_ptr->header_1_.format_code_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "general_constants: %s\n", read_segd_ptr->header_1_.general_constants_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "year: %d\n", read_segd_ptr->header_1_.year_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "additional_blocks_in_general_header: %d\n", read_segd_ptr->header_1_.additional_blocks_in_general_header_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "day: %d\n", read_segd_ptr->header_1_.day_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "hour: %d\n", read_segd_ptr->header_1_.hour_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "minute: %d\n", read_segd_ptr->header_1_.minute_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "second: %d\n", read_segd_ptr->header_1_.second_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "manufacturer_code: %d\n", read_segd_ptr->header_1_.manufacturer_code_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "manufacturer_serial_number: %d\n", read_segd_ptr->header_1_.manufacturer_serial_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "bytes_per_scan: %d\n", read_segd_ptr->header_1_.bytes_per_scan_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "base_scan_interval: %d\n", read_segd_ptr->header_1_.base_scan_interval_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "polarity: %d\n", read_segd_ptr->header_1_.polarity_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "record_type: %d\n", read_segd_ptr->header_1_.record_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "record_length: %d\n", read_segd_ptr->header_1_.record_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "scan_types_record: %d\n", read_segd_ptr->header_1_.scan_types_record_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "channal_sets_scan_type: %d\n", read_segd_ptr->header_1_.channal_sets_scan_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "skew_blocks: %d\n", read_segd_ptr->header_1_.skew_blocks_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "extended_header_block: %d\n", read_segd_ptr->header_1_.extended_header_block_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "external_header_block: %d\n", read_segd_ptr->header_1_.external_header_block_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	//header2
	strcpy(buf,"\nSEGD_GENERAL_HEADER 2:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "expanded_file_number: %d\n", read_segd_ptr->header_2_.expanded_file_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "extended_channel_sets_scan_type: %d\n", read_segd_ptr->header_2_.extended_channel_sets_scan_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "major_segd_revision_number: %d\n", read_segd_ptr->header_2_.major_segd_revision_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "minor_segd_revision_number: %d\n", read_segd_ptr->header_2_.minor_segd_revision_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "number_of_blocks_of_general_trailer: %d\n", read_segd_ptr->header_2_.number_of_blocks_of_general_trailer_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "extended_record_length: %d\n", read_segd_ptr->header_2_.extended_record_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "general_header_block_number: %d\n", read_segd_ptr->header_2_.general_header_block_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	//header3
	strcpy(buf,"\nSEGD_GENERAL_HEADER 3:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "expanded_file_number: %d\n", read_segd_ptr->header_3_.expanded_file_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_line_number: %f\n", read_segd_ptr->header_3_.source_line_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_point_number: %f\n", read_segd_ptr->header_3_.source_point_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_point_index: %d\n", read_segd_ptr->header_3_.source_point_index_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_point_index: %d\n", read_segd_ptr->header_3_.source_point_index_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "phase_control: %d\n", read_segd_ptr->header_3_.phase_control_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "type_vibrator: %d\n", read_segd_ptr->header_3_.type_vibrator_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "phase_angle: %d\n", read_segd_ptr->header_3_.phase_angle_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "general_header_block_number: %d\n", read_segd_ptr->header_3_.general_header_block_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_set_number: %d\n", read_segd_ptr->header_3_.source_set_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	//scan type header
	for(i = 0; i < read_segd_ptr->header_1_.channal_sets_scan_type_; ++i){

		sprintf_s(buf, sizeof(buf), "\nSEGD_SCAN_TYPE_HEADER #%d :\n", i + 1);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "scan_type_number: %d\n", (read_segd_ptr->scan_type_header_ + i)->scan_type_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_number: %d\n", (read_segd_ptr->scan_type_header_ + i)->channel_set_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_start_time: %d\n", (read_segd_ptr->scan_type_header_ + i)->channel_set_start_time_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_set_end_time: %d\n", (read_segd_ptr->scan_type_header_ + i)->channel_set_end_time_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "descale_multiplier: %d\n", (read_segd_ptr->scan_type_header_ + i)->descale_multiplier_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channels_number: %d\n", (read_segd_ptr->scan_type_header_ + i)->channels_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_type_identification: %d\n", (read_segd_ptr->scan_type_header_ + i)->channel_type_identification_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "sample_channel_subscans_number: %d\n", (read_segd_ptr->scan_type_header_ + i)->sample_channel_subscans_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "channel_gain_control: %d\n", (read_segd_ptr->scan_type_header_ + i)->channel_gain_control_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "alias_filter_frequency: %d\n", (read_segd_ptr->scan_type_header_ + i)->alias_filter_frequency_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "alias_filter_slope: %d\n", (read_segd_ptr->scan_type_header_ + i)->alias_filter_slope_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "low_cut_filter: %d\n", (read_segd_ptr->scan_type_header_ + i)->low_cut_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "low_cut_filter_slope: %d\n", (read_segd_ptr->scan_type_header_ + i)->low_cut_filter_slope_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "first_notch_filter: %d\n", (read_segd_ptr->scan_type_header_ + i)->first_notch_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "second_notch_filter: %d\n", (read_segd_ptr->scan_type_header_ + i)->second_notch_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "third_notch_filter: %d\n", (read_segd_ptr->scan_type_header_ + i)->third_notch_filter_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "extended_channel_set_number: %d\n", (read_segd_ptr->scan_type_header_ + i)->extended_channel_set_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "extended_header_flag: %d\n", (read_segd_ptr->scan_type_header_ + i)->extended_header_flag_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "trace_header_extension: %d\n", (read_segd_ptr->scan_type_header_ + i)->trace_header_extension_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "vertical_stack: %d\n", (read_segd_ptr->scan_type_header_ + i)->vertical_stack_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "streamer_number: %d\n", (read_segd_ptr->scan_type_header_ + i)->streamer_number_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

		sprintf_s(buf, sizeof(buf), "array_forming: %d\n", (read_segd_ptr->scan_type_header_ + i)->array_forming_);
		WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
	}

	strcpy(buf,"\nSEGD_EXTENDED_HEADER:\n");
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_length: %d\n", read_segd_ptr->extended_header_.sample_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_frequency: %d\n", read_segd_ptr->extended_header_.sample_frequency_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "trace_total_number: %d\n", read_segd_ptr->extended_header_.trace_total_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "auxiliary_trace_number: %d\n", read_segd_ptr->extended_header_.auxiliary_trace_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "trace_number: %d\n", read_segd_ptr->extended_header_.trace_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "active_trace_number: %d\n", read_segd_ptr->extended_header_.active_trace_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_type: %d\n", read_segd_ptr->extended_header_.source_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_number_in_trace: %d\n", read_segd_ptr->extended_header_.sample_number_in_trace_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "shot_number: %d\n", read_segd_ptr->extended_header_.shot_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tb_window: %d\n", read_segd_ptr->extended_header_.tb_window_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "test_type: %d\n", read_segd_ptr->extended_header_.test_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_first_test: %d\n", read_segd_ptr->extended_header_.line_first_test_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_first_number: %d\n", read_segd_ptr->extended_header_.line_first_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "array_number: %d\n", read_segd_ptr->extended_header_.array_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "array_type: %d\n", read_segd_ptr->extended_header_.array_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "time_break_signal: %d\n", read_segd_ptr->extended_header_.time_break_signal_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "well_time: %d\n", read_segd_ptr->extended_header_.well_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "blaster_serial_number: %d\n", read_segd_ptr->extended_header_.blaster_serial_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "blaster_status: %d\n", read_segd_ptr->extended_header_.blaster_status_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "refraction_delay: %d\n", read_segd_ptr->extended_header_.refraction_delay_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tb_to_t0_time: %d\n", read_segd_ptr->extended_header_.tb_to_t0_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "internal_time_break_signal: %d\n", read_segd_ptr->extended_header_.internal_time_break_signal_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "field_unit_prestack: %d\n", read_segd_ptr->extended_header_.field_unit_prestack_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "Noise_elimination_type: %d\n", read_segd_ptr->extended_header_.Noise_elimination_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "low_trace_percent: %d\n", read_segd_ptr->extended_header_.low_trace_percent_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "low_trace_value: %d\n", read_segd_ptr->extended_header_.low_trace_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "window_number: %d\n", read_segd_ptr->extended_header_.window_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_edit_type: %d\n", read_segd_ptr->extended_header_.historical_edit_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "noisy_trace_percent: %d\n", read_segd_ptr->extended_header_.noisy_trace_percent_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_range: %d\n", read_segd_ptr->extended_header_.historical_range_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_taper_length: %d\n", read_segd_ptr->extended_header_.historical_taper_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "threshold_changed: %d\n", read_segd_ptr->extended_header_.threshold_changed_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_threshold_initial_value: %d\n", read_segd_ptr->extended_header_.historical_threshold_initial_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "historical_zeroing_length: %d\n", read_segd_ptr->extended_header_.historical_zeroing_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "process_type: %d\n", read_segd_ptr->extended_header_.process_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "overlap_number: %d\n", read_segd_ptr->extended_header_.overlap_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "record_length: %d\n", read_segd_ptr->extended_header_.record_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "autocorrelative_peak_time: %d\n", read_segd_ptr->extended_header_.autocorrelative_peak_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "related_reference_signal_number: %d\n", read_segd_ptr->extended_header_.related_reference_signal_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "reference_signal_length: %d\n", read_segd_ptr->extended_header_.reference_signal_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "scan_length: %d\n", read_segd_ptr->extended_header_.scan_length_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_number: %d\n", read_segd_ptr->extended_header_.sample_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "max_auxiliary_channel_max_value: %f\n", read_segd_ptr->extended_header_.max_auxiliary_channel_max_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "max_channel_max_value: %f\n", read_segd_ptr->extended_header_.max_channel_max_value_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "dump_overlap_number: %d\n", read_segd_ptr->extended_header_.dump_overlap_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tape_lable: %s\n", read_segd_ptr->extended_header_.tape_lable_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "tape_number_: %d\n", read_segd_ptr->extended_header_.tape_number_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "version: %d\n", read_segd_ptr->extended_header_.version_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "date: %d\n", read_segd_ptr->extended_header_.date_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_easting: %f\n", read_segd_ptr->extended_header_.source_easting_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "source_northing_: %f\n", read_segd_ptr->extended_header_.source_northing_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "Source_elevation: %d\n", read_segd_ptr->extended_header_.Source_elevation_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "slip_sweep_mode: %d\n", read_segd_ptr->extended_header_.slip_sweep_mode_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "file_number_per_tape: %d\n", read_segd_ptr->extended_header_.file_number_per_tape_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "file_counter: %d\n", read_segd_ptr->extended_header_.file_counter_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "sample_error_explaination: %d\n", read_segd_ptr->extended_header_.sample_error_explaination_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "filter_type: %d\n", read_segd_ptr->extended_header_.filter_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "stack_dumped: %d\n", read_segd_ptr->extended_header_.stack_dumped_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "stack_symbol: %d\n", read_segd_ptr->extended_header_.stack_symbol_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "prm_tilt_correction: %d\n", read_segd_ptr->extended_header_.prm_tilt_correction_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_name: %s\n", read_segd_ptr->extended_header_.line_name_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "operation_mode: %d\n", read_segd_ptr->extended_header_.operation_mode_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "non_log: %d\n", read_segd_ptr->extended_header_.non_log_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "listen_time: %d\n", read_segd_ptr->extended_header_.listen_time_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "dump_type: %d\n", read_segd_ptr->extended_header_.dump_type_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	sprintf_s(buf, sizeof(buf), "line_id: %d\n", read_segd_ptr->extended_header_.line_id_);
	WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

	for(k = 0; k < read_segd_ptr->header_1_.channal_sets_scan_type_; ++k){
		
		switch(read_segd_ptr->scan_type_header_[k].channel_type_identification_){
			case 1://地震道
				{

					for(i = 0; i < read_segd_ptr->extended_header_.trace_number_; ++i){

						sprintf_s(buf, sizeof(buf), "\n记录道数据块#%d :\n", i);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						strcpy(buf, "\n多路解码道头:\n");
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "file_number: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.file_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "scan_type_number: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.scan_type_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "channel_set_number: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.channel_set_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "trace_number: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.trace_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "first_timing_word: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.first_timing_word_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "trace_header_extension: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.trace_header_extension_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "sample_skew: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.sample_skew_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "trace_edit: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.trace_edit_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "time_break_window: %f\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.time_break_window_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_channel_set_number: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.extended_channel_set_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						sprintf_s(buf, sizeof(buf), "extended_file_number: %d\n", read_segd_ptr->trace_headers_[i].demux_trace_header_.extended_file_number_);
						WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

						for(j = 0; j < read_segd_ptr->scan_type_header_[0].trace_header_extension_; ++j){

							sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							switch(j){
								case 0:
									{

										sprintf_s(buf, sizeof(buf), "receiver_line_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.receiver_line_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "receiver_point_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.receiver_point_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "receiver_point_index: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.receiver_point_index_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "number_of_samples_per_trace: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "extended_receiver_line_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.extended_receiver_line_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "extended_receiver_point_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.extended_receiver_point_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "sensor_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_1_.sensor_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

									}
									break;
								case 1:
									{
										sprintf_s(buf, sizeof(buf), "receiver_point_east_: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_2_.receiver_point_east_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "receiver_point_north: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_2_.receiver_point_north_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "receiver_point_high: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_2_.receiver_point_high_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "detector_type_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_2_.detector_type_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										sprintf_s(buf, sizeof(buf), "extended_trace_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_2_.extended_trace_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
									}
									break;
								case 2:
									{

										sprintf_s(buf, sizeof(buf), "resistor_lower_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.resistor_lower_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


										sprintf_s(buf, sizeof(buf), "resistor_higher_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.resistor_higher_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


										sprintf_s(buf, sizeof(buf), "resistor_value: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.resistor_value_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


										sprintf_s(buf, sizeof(buf), "tilt_limit: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.tilt_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


										sprintf_s(buf, sizeof(buf), "tilt_value_: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.tilt_value_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


										sprintf_s(buf, sizeof(buf), "resistor_deviation: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.resistor_deviation_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);


										sprintf_s(buf, sizeof(buf), "tilt_deviation: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_3_.tilt_deviation_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
									}
									break;
								case 3:
									{
										sprintf_s(buf, sizeof(buf), "capacitance_lower_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.capacitance_lower_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "capacitance_higher_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.capacitance_higher_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "capacitance_value: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.capacitance_value_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "cutoff_lower_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.cutoff_lower_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "cutoff_higher_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.cutoff_higher_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "cutoff_value: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.cutoff_value_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "capacitance_deviation: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.capacitance_deviation_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "cutoff_deviation: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_4_.cutoff_deviation_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
									}
									break;
								case 4:
									{
										
										sprintf_s(buf, sizeof(buf), "leakage_limit: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_5_.leakage_limit_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "leakage_value: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_5_.leakage_value_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "leakage_deviation: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_5_.leakage_deviation_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

									}
									break;
								case 5:
									{
										sprintf_s(buf, sizeof(buf), "device_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.device_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "device_serial_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.device_serial_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "seism_trace_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.seism_trace_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "component_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.component_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "FDU_DSU_component_serial_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.FDU_DSU_component_serial_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "FDU_DSU_component_position: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.FDU_DSU_component_position_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "FDU_DSU_device_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.FDU_DSU_device_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "seism_trace_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.seism_trace_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "detector_Sensitivity: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_6_.detector_Sensitivity_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

									}
									break;
								case 6:
									{
										sprintf_s(buf, sizeof(buf), "control_unit_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.control_unit_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "control_unit_serial_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.control_unit_serial_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_gain_scale: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_gain_scale_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_filter: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_filter_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_data_deviation: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_data_deviation_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_edit_status: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_edit_status_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_sample_conversion_factor: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_sample_conversion_factor_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "number_of_noisy_stack: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.number_of_noisy_stack_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "number_of_low_stacks: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.number_of_low_stacks_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_type_serial_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_type_serial_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "channel_process_type: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.channel_process_type_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "trace_max_value: %f\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.trace_max_value_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "trace_max_time: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.trace_max_time_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										
										sprintf_s(buf, sizeof(buf), "interval_polations_number: %d\n", read_segd_ptr->trace_headers_[i].trace_header_extended_7_.interval_polations_number_);
										WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
									}
									break;
							}
						}

						//read data
						size = segd_file_get_trace_data_size(read_segd_ptr);
						for(j = 0; j < size; j += CURRENT_SEGD_MINI_DATA_LENGTH){

							data[3] = *(read_segd_ptr->trace_headers_[i].trace_data_ + j);
							data[2] = *(read_segd_ptr->trace_headers_[i].trace_data_ + j + 1);
							data[1] = *(read_segd_ptr->trace_headers_[i].trace_data_ + j + 2);
							data[0] = *(read_segd_ptr->trace_headers_[i].trace_data_ + j + 3);
							memcpy(&f_val, data, sizeof(UCHAR) * CURRENT_SEGD_MINI_DATA_LENGTH);
							memset(buf_file_name, 0x0, sizeof(buf_file_name));
							sprintf_s(buf_file_name, sizeof(buf_file_name), "%f,", f_val);
							WriteFile(handle, buf_file_name, strlen(buf_file_name), &buf_size_dat, NULL);
						}
					}
				}
				break;
			case 9://辅助道
				{
					if(read_segd_ptr->extended_header_.auxiliary_trace_number_ > 0){

						for(i = 0; i < read_segd_ptr->extended_header_.auxiliary_trace_number_; ++i){

							sprintf_s(buf, sizeof(buf), "\n辅助记录道数据块#%d :\n", i);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							strcpy(buf, "\n多路解码道头:\n");
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "file_number: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.file_number_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "scan_type_number: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.scan_type_number_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "channel_set_number: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.channel_set_number_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "trace_number: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.trace_number_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "first_timing_word: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.first_timing_word_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "trace_header_extension: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.trace_header_extension_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "sample_skew: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.sample_skew_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "trace_edit: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.trace_edit_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "time_break_window: %f\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.time_break_window_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "extended_channel_set_number: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.extended_channel_set_number_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							sprintf_s(buf, sizeof(buf), "extended_file_number: %d\n", read_segd_ptr->aux_trace_headers_[i].demux_trace_header_.extended_file_number_);
							WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

							for(j = 0; j < read_segd_ptr->scan_type_header_[0].trace_header_extension_; ++j){

								sprintf_s(buf, sizeof(buf), "\n道头扩展#%d :\n", j + 1);
								WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

								switch(j){
									case 0:
										{
											sprintf_s(buf, sizeof(buf), "receiver_line_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.receiver_line_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "receiver_point_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.receiver_point_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "receiver_point_index: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.receiver_point_index_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "number_of_samples_per_trace: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.number_of_samples_per_trace_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "extended_receiver_line_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.extended_receiver_line_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "extended_receiver_point_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.extended_receiver_point_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "sensor_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_1_.sensor_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										}
										break;
									case 1:
										{
											sprintf_s(buf, sizeof(buf), "receiver_point_east_: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_2_.receiver_point_east_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "receiver_point_north: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_2_.receiver_point_north_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "receiver_point_high: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_2_.receiver_point_high_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "detector_type_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_2_.detector_type_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "extended_trace_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_2_.extended_trace_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										}
										break;
									case 2:
										{
											sprintf_s(buf, sizeof(buf), "resistor_lower_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.resistor_lower_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "resistor_higher_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.resistor_higher_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "resistor_value: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.resistor_value_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "tilt_limit: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.tilt_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "tilt_value_: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.tilt_value_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "resistor_deviation: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.resistor_deviation_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "tilt_deviation: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_3_.tilt_deviation_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										}
										break;
									case 3:
										{
											sprintf_s(buf, sizeof(buf), "capacitance_lower_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.capacitance_lower_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "capacitance_higher_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.capacitance_higher_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "capacitance_value: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.capacitance_value_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "cutoff_lower_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.cutoff_lower_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "cutoff_higher_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.cutoff_higher_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "cutoff_value: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.cutoff_value_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "capacitance_deviation: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.capacitance_deviation_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "cutoff_deviation: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_4_.cutoff_deviation_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										}
										break;
									case 4:
										{
											sprintf_s(buf, sizeof(buf), "leakage_limit: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_5_.leakage_limit_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "leakage_value: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_5_.leakage_value_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "leakage_deviation: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_5_.leakage_deviation_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										}
										break;
									case 5:
										{
											sprintf_s(buf, sizeof(buf), "device_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.device_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "device_serial_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.device_serial_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "seism_trace_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.seism_trace_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "component_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.component_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "FDU_DSU_component_serial_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.FDU_DSU_component_serial_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "FDU_DSU_component_position: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.FDU_DSU_component_position_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "FDU_DSU_device_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.FDU_DSU_device_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "seism_trace_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.seism_trace_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "detector_Sensitivity: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_6_.detector_Sensitivity_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

										}
										break;
									case 6:
										{
											sprintf_s(buf, sizeof(buf), "control_unit_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.control_unit_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "control_unit_serial_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.control_unit_serial_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_gain_scale: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_gain_scale_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_filter: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_filter_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_data_deviation: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_data_deviation_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_edit_status: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_edit_status_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_sample_conversion_factor: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_sample_conversion_factor_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "number_of_noisy_stack: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.number_of_noisy_stack_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "number_of_low_stacks: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.number_of_low_stacks_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_type_serial_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_type_serial_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "channel_process_type: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.channel_process_type_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "trace_max_value: %f\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.trace_max_value_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "trace_max_time: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.trace_max_time_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);

											sprintf_s(buf, sizeof(buf), "interval_polations_number: %d\n", read_segd_ptr->aux_trace_headers_[i].trace_header_extended_7_.interval_polations_number_);
											WriteFile(handle, buf, strlen(buf), &buf_size_dat, NULL);
										}
										break;

								}
							}

							//read data
							size = segd_file_get_trace_data_size(read_segd_ptr);
							for(j = 0; j < size; j += CURRENT_SEGD_MINI_DATA_LENGTH){

								data[3] = *(read_segd_ptr->aux_trace_headers_[i].trace_data_ + j);
								data[2] = *(read_segd_ptr->aux_trace_headers_[i].trace_data_ + j + 1);
								data[1] = *(read_segd_ptr->aux_trace_headers_[i].trace_data_ + j + 2);
								data[0] = *(read_segd_ptr->aux_trace_headers_[i].trace_data_ + j + 3);
								memcpy(&f_val, data, sizeof(UCHAR) * CURRENT_SEGD_MINI_DATA_LENGTH);
								memset(buf_file_name, 0x0, sizeof(buf_file_name));
								sprintf_s(buf_file_name, sizeof(buf_file_name), "%f,", f_val);
								WriteFile(handle, buf_file_name, strlen(buf_file_name), &buf_size_dat, NULL);
							}
						}
					}
				}
				break;
		}
	}
	CloseHandle(handle);
}

/**
@fn	int run_analysing_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 分析SEGD文件,并且输出相应的文本文件
@param[in]	argc	参数个数
@param[in]	argv	参数数组
@return	如果失败返回false,否则返回true
@note	当前使用char_t
*/
int run_analysing_segd_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	int error_code = APP_SUCCESS;
	char_t path_name[MAX_STRING_SIZE] = {0};
	SEGD_FILE_PTR read_segd_ptr = NULL;

	StrCpy(path_name, arg->value_);
	if(_tcslen(__application_environment.output_log_) == 0)
		StrCpy(__application_environment.output_log_, __TEXT("output.segd.log"));

	read_segd_ptr = segd_file_create(path_name, SEGD_FILE_OPEN_EXISTING | SEGD_FILE_IO_OVERLAPPED);
	if(read_segd_file_private(read_segd_ptr) == APP_FAULT)	return error_code = APP_FAULT;

	output_segd_log(read_segd_ptr, __application_environment.output_log_);

	segd_file_close(read_segd_ptr);

	return error_code;
}

/**
@fn	int run_set_manufacturer_code_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 根据传入的采样率生成相应的SEGD文件
@param[in]	argc	参数个数
@param[in]	argv	参数数组
@return	如果失败返回false,否则返回true
@note	当前使用char_t
*/
int run_set_manufacturer_code_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
{
	__application_environment.manufacturer_code_ = _ttoi(arg->value_);
	return APP_SUCCESS;
}

/**
@fn	int run_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 处理--file命令行参数
@param[in]	arg	有关该命令行的参数信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
//int run_file_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
//{
//	bool is_ok;
//	int error_code = 0x0, i = 0x0, size = 0x0;
//	SEGD_CONFIG_FILE_PTR segd_config_info_ptr = NULL;
//	SEGD_FILE_PTR outp_file_ptr = NULL;
//	SEGD_SCAN_TYPE_HEADER segd_scan_type_header;
//	SEGD_FILE_TRACE_HEADER trace_header;
//	SEGD_WRITING_PROGRESS_INFO_PRIVATE progress_info;
//	SPS_DATA_INFO sps_data_info; 
//
//	__matrix_zero_Memory(&progress_info, sizeof(progress_info));
//	
//	//1. 检测配置文件是否正确
//	if(check_config_file(arg->value_) != APP_SUCCESS){	error_code = APP_FAULT;	goto GOTO;}
//	
//	//2. 读取配置文件
//	if(load_config_file(arg->value_) != APP_SUCCESS){	error_code = APP_FAULT;	goto GOTO;}
//
//	//3. 得到配置信息 & 创建socket
//	segd_config_info_ptr = get_segd_config_info_ptr();
//
//	progress_info.sock_impl_.remote_port_ = segd_config_info_ptr->remote_port_;
//	if(!create_udp_socket(&progress_info.sock_impl_, segd_config_info_ptr->remote_ip_)){
//		set_last_error(IDS_STRING5015);
//		output_log(IDS_STRING5015);
//		error_code = APP_FAULT;
//		goto GOTO;
//	}
//
//	//4. 获取SPS数据信息
//	__matrix_zero_Memory(&sps_data_info, sizeof(sps_data_info));
//	sps_data_info.sps_path_ptr_ = segd_config_info_ptr->sps_path_;
//	load_sps_file(&sps_data_info);
//
//	//5. 创建SEG-D文件
//	outp_file_ptr = segd_file_create(segd_config_info_ptr->output_path_, SEGD_FILE_CREATE);
//
//	if(outp_file_ptr == NULL){
//		set_last_error(IDS_STRING5014);
//		output_log(IDS_STRING5014, segd_config_info_ptr->output_path_);
//		error_code = APP_FAULT;
//		goto GOTO;
//	}
//
//	//6. 写入普通头段数据块1
//	memcpy_s(&outp_file_ptr->header_1_, sizeof(outp_file_ptr->header_1_), &segd_config_info_ptr->header_1_, sizeof(segd_config_info_ptr->header_1_));
//	is_ok = segd_file_write_genernal_header_1(outp_file_ptr, &outp_file_ptr->header_1_);
//	if(!is_ok){
//
//		set_last_error(GetLastError());
//		output_log(get_last_error());
//		error_code = APP_FAULT;
//		goto GOTO;
//	}
//
//	//7. 写入普通头段数据块2
//	memcpy_s(&outp_file_ptr->header_2_, sizeof(outp_file_ptr->header_2_), &segd_config_info_ptr->header_2_, sizeof(segd_config_info_ptr->header_2_));
//	is_ok = segd_file_write_genernal_header_2(outp_file_ptr, &outp_file_ptr->header_2_);
//	if(!is_ok){
//
//		set_last_error(GetLastError());
//		output_log(get_last_error());
//		error_code = APP_FAULT;
//		goto GOTO;
//	}
//
//	//8. 写入普通头段数据块3
//	memcpy_s(&outp_file_ptr->header_3_, sizeof(outp_file_ptr->header_3_), &segd_config_info_ptr->header_3_, sizeof(segd_config_info_ptr->header_3_));
//	if(segd_config_info_ptr->header_1_.additional_blocks_in_general_header_ == 2){
//
//		is_ok = segd_file_write_genernal_header_3(outp_file_ptr, &outp_file_ptr->header_3_);
//		if(!is_ok){
//
//			set_last_error(GetLastError());
//			output_log(get_last_error());
//			error_code = APP_FAULT;
//			goto GOTO;
//		}
//	}
//
//	//9. 写入扫描类型头段
//	__matrix_zero_Memory(&segd_scan_type_header, sizeof(segd_scan_type_header));
//	memcpy(&segd_scan_type_header, &segd_config_info_ptr->scan_type_header_, sizeof(segd_scan_type_header));
//	for(i = 0; i < segd_config_info_ptr->header_1_.channal_sets_scan_type_ ; ++i){
//		
//		__matrix_zero_Memory(&segd_scan_type_header, sizeof(segd_scan_type_header));
//		memcpy(&segd_scan_type_header, &segd_config_info_ptr->scan_type_header_, sizeof(segd_scan_type_header));
//		segd_scan_type_header.channel_set_number_ = i + 1;
//		is_ok = segd_file_write_scan_type_header(outp_file_ptr, &segd_scan_type_header);
//		if(!is_ok){
//
//			set_last_error(GetLastError());
//			output_log(get_last_error());
//			error_code = APP_FAULT;
//			goto GOTO;
//		}
//	}
//
//	//10. 写入扩展头段
//	if(segd_config_info_ptr->header_1_.extended_header_block_ == 32){
//		
//		memcpy_s(&outp_file_ptr->extended_header_, sizeof(outp_file_ptr->extended_header_), &segd_config_info_ptr->extended_header_, sizeof(segd_config_info_ptr->extended_header_));
//		is_ok = segd_file_write_extended_header(outp_file_ptr, &outp_file_ptr->extended_header_);
//		if(!is_ok){
//
//			set_last_error(GetLastError());
//			output_log(get_last_error());
//			error_code = APP_FAULT;
//			goto GOTO;
//		}
//	}
//
//	//11. 写入记录道数据块 + 写入AD数据
//	if(segd_config_info_ptr->header_1_.extended_header_block_ == 32){
//
//		//trace header + data
//		for(i = 0; i < segd_config_info_ptr->extended_header_.trace_number_; ++i){
//
//			//modify this value for multiple trace header.
//			__matrix_zero_Memory(&trace_header, sizeof(trace_header));
//			CopyMemory(&trace_header, &segd_config_info_ptr->trace_header_, sizeof(trace_header));
//			trace_header.demux_trace_header_.trace_number_ = i + 1;
//			++trace_header.trace_header_extended_1_.extended_receiver_point_number_;
//			is_ok = segd_file_write_trace_header(outp_file_ptr, &trace_header);
//			if(!is_ok){
//
//				set_last_error(GetLastError());
//				output_log(get_last_error());
//				error_code = APP_FAULT;
//				goto GOTO;
//			}
//
//			//segd data
//			if(trace_header.trace_header_extended_1_.number_of_samples_per_trace_ > 0){
//
//				size = segd_file_get_trace_data_size(trace_header);
//				segd_file_write_trace_data(outp_file_ptr, trace_header.trace_data_, size);
//			}
//		}
//
//		//12. 写入SEG-D文件尾
//		segd_config_info_ptr->extended_header_.auxiliary_trace_number_ = 1;
//		__matrix_zero_Memory(&trace_header, sizeof(trace_header));
//		CopyMemory(&trace_header, &segd_config_info_ptr->trace_header_, sizeof(trace_header));
//		trace_header.demux_trace_header_.channel_set_number_ = 0x10;
//		trace_header.trace_header_extended_1_.receiver_line_number_ = 0xffffff;
//		trace_header.trace_header_extended_1_.receiver_point_number_ = 0xffffff;
//		is_ok = segd_file_write_trace_header(outp_file_ptr, &trace_header);
//		if(!is_ok){
//
//			set_last_error(GetLastError());
//			output_log(get_last_error());
//			error_code = APP_FAULT;
//			goto GOTO;
//		}
//
//		if(trace_header.trace_header_extended_1_.number_of_samples_per_trace_ > 0){
//
//			size = segd_file_get_trace_data_size(trace_header);
//			segd_file_write_trace_data(outp_file_ptr, trace_header.trace_data_, size);
//		}
//	}
//
//GOTO:
//	segd_file_close(outp_file_ptr);
//	close_udp_socket(&progress_info.sock_impl_);
//	return error_code;
//}
//
/**
@fn	int run_version_arg_private(APPLICATION_COMMAND_ARG_PTR arg)
@details 处理--version命令行参数
@param[in]	arg	有关该命令行的参数信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
