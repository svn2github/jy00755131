/**
@brief	处理命令行传入文件
@details	针对--data-file命令传入参数指向的文件,读取数据文件头和AD数据,当前为.h文件,具体参见实现文件(cpp)
@file	proc.conf.file.cpp
@note	
@see	关于配置文件参看args.conf
@version	1.1.12.40.2011.09.28
@author	kuoxin	
@bug
	1.1.12.40.2011.09.28	初始化版本
*/

#include "segd.conv.h"
#include "proc.data.file.h"
#include "..\segd.api\segd.api.h"
#include "proc.conf.file.h"
#include <time.h>

extern APPLICATION_ENVIRONMENT __application_environment;

int get_file_header_size(){return sizeof(AD_DATA_FILE_HEAD_PRIVATE);};

unsigned int get_sample_length(HANDLE file_handle, 
							   unsigned int tb_time, 
							   unsigned int frame_number_per_device,
							   unsigned int sample_frequency,
							   unsigned int device_number)
{
	int i = 0;
	unsigned int result = 0x0;
	unsigned int data_size_per_frame_pack = device_number * (SAMPLE_DATA_PACKAGE_SIZE * 3 + TB_TIME_LENGTH);
//	unsigned int total_sample_length = ((frame_number_per_device * SAMPLE_DATA_PACKAGE_SIZE) * 1000) / sample_frequency;
	LARGE_INTEGER file_pointer = {0x0};
	unsigned int tmp_val = 0x0;
	char tmp_buf[TB_TIME_LENGTH] = {0};
	DWORD bytes_size = 0x0;

	for(i = 0; i < frame_number_per_device; ++i){
		
		file_pointer.QuadPart = sizeof(AD_DATA_FILE_HEAD_PRIVATE) + i * data_size_per_frame_pack;
		SetFilePointerEx(file_handle, file_pointer, NULL, FILE_BEGIN);
		ReadFile(file_handle, tmp_buf, TB_TIME_LENGTH, &bytes_size, NULL);
		CopyMemory(&tmp_val, tmp_buf, sizeof(tmp_val));
		if(tb_time < tmp_val){
			i = i > 1 ? --i : 0;
			break;
		}
		else if(tb_time == tmp_val){
			i = i > 0 ? --i : 0;
			break;
		}
	}

	result = (((frame_number_per_device - i) * SAMPLE_DATA_PACKAGE_SIZE)*  1000) / sample_frequency;
	return result = (result > MAX_SAMPLING_TIME) ? (MAX_SAMPLING_TIME - 1) : result;
}

int load_data_file(char_t const* data_path, AD_DATA_FILE_INFO_PTR ad_data_ptr)
{
	int is_ok = APP_SUCCESS;
	DWORD size = 0x0;
//	LARGE_INTEGER tmp_value = {0x0};
	char tmp_buf[MAX_STRING_SIZE_LOCAL] = {0};

	if(ad_data_ptr == NULL) return is_ok = APP_FAULT;
	ad_data_ptr->file_handle_ = CreateFile(data_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
											NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(ad_data_ptr->file_handle_ == INVALID_HANDLE_VALUE){
		set_last_error(IDS_STRING5027);
		output_log(IDS_STRING5027, get_last_error());
		is_ok = APP_FAULT;
		goto GOTO;
	}

	if(!ReadFile(ad_data_ptr->file_handle_, &ad_data_ptr->file_header_, sizeof(ad_data_ptr->file_header_), &size, NULL)){
		set_last_error(IDS_STRING5028);
		output_log(IDS_STRING5028, get_last_error());
		is_ok = APP_FAULT;
		goto GOTO;

	}

	if(size != sizeof(ad_data_ptr->file_header_)){
		set_last_error(IDS_STRING5028);
		output_log(IDS_STRING5028, get_last_error());
		is_ok = APP_FAULT;
		goto GOTO;
	}

	wcstombs_s((size_t*)&size, ad_data_ptr->file_header_.dat_path_, sizeof(ad_data_ptr->file_header_.dat_path_), 
				data_path, sizeof(ad_data_ptr->file_header_.dat_path_));


	if(!GetFileSizeEx(ad_data_ptr->file_handle_, &ad_data_ptr->data_size_)){
		set_last_error(IDS_STRING5030);
		output_log(IDS_STRING5030, get_last_error());
		is_ok = APP_FAULT;
		goto GOTO;
	}

	ad_data_ptr->file_header_.trace_total_number_ += (ad_data_ptr->file_header_.auxiliary_line_number_ * ad_data_ptr->file_header_.auxiliary_trace_number_);//需要加上尾部的辅助道数据
	ad_data_ptr->data_size_.QuadPart -= sizeof(ad_data_ptr->file_header_);
	ad_data_ptr->frame_number_per_fdu_ = ad_data_ptr->data_size_.QuadPart / ((SAMPLE_DATA_PACKAGE_SIZE * 3 + TB_TIME_LENGTH) * ad_data_ptr->file_header_.trace_total_number_);
	
	if(ad_data_ptr->file_header_.sample_length_ == 0x0 || ad_data_ptr->file_header_.sample_number_in_trace_ == 0x0){

		//得到真实有效的sample_length & sample_number_in_trace -- 该数据为tb_time后的采样数据长度
		ad_data_ptr->file_header_.sample_length_ = get_sample_length(ad_data_ptr->file_handle_, 
																		__application_environment.tb_time_, 
																		ad_data_ptr->frame_number_per_fdu_, 
																		ad_data_ptr->file_header_.sample_frequency_,
																		ad_data_ptr->file_header_.trace_total_number_);
		ad_data_ptr->file_header_.sample_number_in_trace_ = ad_data_ptr->file_header_.sample_length_;
	}
	
	sprintf_s(tmp_buf, sizeof(tmp_buf), "%s\\%d.sgd", ad_data_ptr->file_header_.output_path_, ad_data_ptr->file_header_.FileNumber_);
	StrCpyA(ad_data_ptr->file_header_.output_path_, tmp_buf);

#ifdef _DEBUG
//	strcpy(ad_data_ptr->file_header_.sps_path_, "Z:\\works\\sps\\jo\\jo");
//	strcpy(ad_data_ptr->file_header_.output_path_, "Z:\\works\\1.sgd");
#endif

GOTO:
	if(is_ok == APP_FAULT){
		if(ad_data_ptr->file_handle_ != INVALID_HANDLE_VALUE)
			CloseHandle(ad_data_ptr->file_handle_);	
	}

	return is_ok;
}


int modify_config_info(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr, __in_out__ void* void_ptr)
{
	int is_ok = APP_SUCCESS;
	size_t mbcs_size = 0x0;
	char_t tmp_buf[MAX_STRING_SIZE] = {0x0};
	char_t tmp_buf2[MAX_STRING_SIZE] = {0x0};
	struct tm* tmp_tm_ptr = NULL;
	SEGD_CONFIG_FILE_PTR segd_config_info_ptr = reinterpret_cast<SEGD_CONFIG_FILE_PTR>(void_ptr);

	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.dat_path_, 0);
	mbstowcs_s(&mbcs_size, segd_config_info_ptr->dat_path_, mbcs_size, ad_data_ptr->file_header_.dat_path_, mbcs_size);
	segd_config_info_ptr->dat_path_[mbcs_size] = 0x0;

	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.sps_path_, 0);
	mbstowcs_s(&mbcs_size, segd_config_info_ptr->sps_path_, mbcs_size, ad_data_ptr->file_header_.sps_path_, mbcs_size);
	segd_config_info_ptr->sps_path_[mbcs_size] = 0x0;

	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.output_path_, 0);
	mbstowcs_s(&mbcs_size, segd_config_info_ptr->output_path_, mbcs_size, ad_data_ptr->file_header_.output_path_, mbcs_size);
	segd_config_info_ptr->output_path_[mbcs_size] = 0x0;

	__matrix_zero_Memory(tmp_buf2, sizeof(tmp_buf2));
	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.remote_ip_, 0);
	mbstowcs_s(&mbcs_size, segd_config_info_ptr->remote_ip_, mbcs_size, ad_data_ptr->file_header_.remote_ip_, mbcs_size);
	segd_config_info_ptr->remote_ip_[mbcs_size] = 0x0;
	segd_config_info_ptr->remote_port_ = static_cast<unsigned short>(ad_data_ptr->file_header_.remote_port_);
	segd_config_info_ptr->extended_header_.shot_number_ = ad_data_ptr->file_header_.shot_number_;
	segd_config_info_ptr->header_1_.file_number_ = static_cast<unsigned short>(ad_data_ptr->file_header_.FileNumber_);
	segd_config_info_ptr->trace_header_.demux_trace_header_.file_number_ = static_cast<unsigned short>(ad_data_ptr->file_header_.FileNumber_);
	segd_config_info_ptr->trace_header_.demux_trace_header_.extended_file_number_ = ad_data_ptr->file_header_.FileNumber_;
	segd_config_info_ptr->header_2_.expanded_file_number_ = ad_data_ptr->file_header_.FileNumber_;
	segd_config_info_ptr->header_3_.expanded_file_number_ = ad_data_ptr->file_header_.FileNumber_;
	segd_config_info_ptr->header_3_.source_line_number_ = ad_data_ptr->file_header_.source_line_number_;
	segd_config_info_ptr->header_3_.source_point_number_ = ad_data_ptr->file_header_.source_point_number_;

	segd_config_info_ptr->extended_header_.sample_frequency_ = ad_data_ptr->file_header_.sample_frequency_;
	segd_config_info_ptr->extended_header_.sample_length_ = ad_data_ptr->file_header_.sample_length_;
	segd_config_info_ptr->header_2_.extended_record_length_ = ad_data_ptr->file_header_.sample_length_;
	segd_config_info_ptr->scan_type_header_.channel_set_end_time_ = static_cast<unsigned short>(ad_data_ptr->file_header_.sample_length_);
	segd_config_info_ptr->extended_header_.listen_time_ = ad_data_ptr->file_header_.sample_length_;
	segd_config_info_ptr->extended_header_.trace_total_number_ = ad_data_ptr->file_header_.trace_total_number_;
	segd_config_info_ptr->extended_header_.source_type_ = ad_data_ptr->file_header_.source_type_;
	segd_config_info_ptr->extended_header_.sample_number_in_trace_ = ad_data_ptr->file_header_.sample_number_in_trace_;
	segd_config_info_ptr->trace_header_.trace_header_extended_1_.number_of_samples_per_trace_ = ad_data_ptr->file_header_.sample_number_in_trace_;
	segd_config_info_ptr->header_3_.source_set_number_ = static_cast<unsigned short>(ad_data_ptr->file_header_.receiver_line_number_);
	segd_config_info_ptr->extended_header_.record_length_ = ad_data_ptr->file_header_.sample_length_;

	segd_config_info_ptr->scan_type_header_.channels_number_ = 	
		static_cast<unsigned short>(ad_data_ptr->file_header_.trace_total_number_ - 
									ad_data_ptr->file_header_.auxiliary_trace_number_ * ad_data_ptr->file_header_.auxiliary_line_number_);

	segd_config_info_ptr->extended_header_.trace_number_ = segd_config_info_ptr->scan_type_header_.channels_number_;
	segd_config_info_ptr->extended_header_.active_trace_number_ = segd_config_info_ptr->extended_header_.trace_number_;
	segd_config_info_ptr->extended_header_.auxiliary_trace_number_ = ad_data_ptr->file_header_.auxiliary_line_number_ * ad_data_ptr->file_header_.auxiliary_trace_number_;
	segd_config_info_ptr->extended_header_.tb_window_ = ad_data_ptr->file_header_.tb_window_;

	segd_config_info_ptr->trace_header_.trace_header_extended_3_.resistor_lower_limit_ = ad_data_ptr->file_header_.resistor_lower_limit_;
	segd_config_info_ptr->trace_header_.trace_header_extended_3_.resistor_higher_limit_ = ad_data_ptr->file_header_.resistor_higher_limit_;
	segd_config_info_ptr->trace_header_.trace_header_extended_3_.resistor_deviation_ = ad_data_ptr->file_header_.resistor_deviation_;
	segd_config_info_ptr->trace_header_.trace_header_extended_3_.resistor_value_ = ad_data_ptr->file_header_.resistor_value_;
	segd_config_info_ptr->trace_header_.trace_header_extended_3_.tilt_limit_ = ad_data_ptr->file_header_.tilt_limit_;
	segd_config_info_ptr->trace_header_.trace_header_extended_3_.tilt_value_ = ad_data_ptr->file_header_.tilt_value_;
	segd_config_info_ptr->trace_header_.trace_header_extended_3_.tilt_deviation_ = ad_data_ptr->file_header_.tilt_deviation_;

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	tmp_tm_ptr = gmtime(&ad_data_ptr->file_header_.datetime_);

	sprintf((char*)segd_config_info_ptr->extended_header_.date_, "\"%d %d %d\"", 
			tmp_tm_ptr->tm_mon, tmp_tm_ptr->tm_mday, tmp_tm_ptr->tm_year + 1900);

	segd_config_info_ptr->header_1_.year_ = tmp_tm_ptr->tm_year % 100;
	segd_config_info_ptr->header_1_.day_ = static_cast<unsigned short>(tmp_tm_ptr->tm_yday + 1);
	segd_config_info_ptr->header_1_.hour_ = static_cast<unsigned char>(tmp_tm_ptr->tm_hour);
	segd_config_info_ptr->header_1_.minute_ = static_cast<unsigned char>(tmp_tm_ptr->tm_min);
	segd_config_info_ptr->header_1_.second_ = static_cast<unsigned char>(tmp_tm_ptr->tm_sec);
	segd_config_info_ptr->header_1_.manufacturer_code_ = ad_data_ptr->manufacturer_code_;//制造商代码

	return is_ok;
}

int get_data_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr)
{
	int size = 0x0;
	size = segd_file_get_trace_data_size_base(ad_data_ptr->manufacturer_code_, ad_data_ptr->file_header_.sample_number_in_trace_,
											ad_data_ptr->file_header_.sample_length_, ad_data_ptr->file_header_.sample_frequency_);
	return size;
};


//int modify_config_info(__in__ char_t const* config_path, const AD_DATA_FILE_INFO_PTR ad_data_ptr)
//{
//	int is_ok = APP_SUCCESS;
//	HMODULE module_handle = NULL;
//	SEGD_API_INFO segd_api;
//	int error_code = 0x0;
//	size_t mbcs_size = 0x0;
//	const char_t* error_info = NULL;
//	char_t tmp_buf[MAX_STRING_SIZE] = {0x0};
//	char_t tmp_buf2[MAX_STRING_SIZE] = {0x0};
//	struct tm* tmp_tm_ptr = NULL;
//
//	INIT_MODULE init_module_ptr = NULL;
//	SET_MODULE_OPTION set_module_option_ptr = NULL;
//	RELEASE_MODULE rel_module_ptr = NULL;
//	GET_ERROR_INFO get_error_info_ptr = NULL;
//
//	module_handle = LoadLibrary(__TEXT("segd.api.dll"));
//	if(module_handle == NULL) {set_last_error(IDS_STRING5029); output_log(IDS_STRING5029, GetLastError()); goto GOTO;}
//
//	init_module_ptr = (INIT_MODULE)GetProcAddress(module_handle, "init_module");
//	set_module_option_ptr = (SET_MODULE_OPTION)GetProcAddress(module_handle, "set_module_option");
//	rel_module_ptr = (RELEASE_MODULE)GetProcAddress(module_handle, "release_module");
//	get_error_info_ptr = (GET_ERROR_INFO)GetProcAddress(module_handle, "get_error_info");
//
//	segd_api.config_path_ = const_cast<char_t*>(config_path);
//	segd_api.listen_port_ = 0;
//
//	if(init_module_ptr(&segd_api) != APP_FAULT){
//		error_info = get_error_info_ptr(&error_code);
//		output_log(error_info);
//		is_ok = APP_FAULT;
//		set_last_error(error_code);
//		goto GOTO;
//	}
//
//	__matrix_zero_Memory(tmp_buf2, sizeof(tmp_buf2));
//	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.dat_path_, 0);
//	mbstowcs_s(&mbcs_size, tmp_buf, mbcs_size, ad_data_ptr->file_header_.dat_path_, mbcs_size);
//	__matrix_sprintf_s(tmp_buf2, __TEXT("\"%s\""), tmp_buf);
//	set_module_option_ptr(__TEXT("input_ad_dat_file"), tmp_buf2);
//
//	__matrix_zero_Memory(tmp_buf2, sizeof(tmp_buf2));
//	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.sps_path_, 0);
//	mbstowcs_s(&mbcs_size, tmp_buf, mbcs_size, ad_data_ptr->file_header_.sps_path_, mbcs_size);
//	__matrix_sprintf_s(tmp_buf2, __TEXT("\"%s\""), tmp_buf);
//	set_module_option_ptr(__TEXT("input_sps_file"), tmp_buf2);
//
//	__matrix_zero_Memory(tmp_buf2, sizeof(tmp_buf2));
//	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.output_path_, 0);
//	mbstowcs_s(&mbcs_size, tmp_buf, mbcs_size, ad_data_ptr->file_header_.output_path_, mbcs_size);
//	__matrix_sprintf_s(tmp_buf2, __TEXT("\"%s\""), tmp_buf);
//	set_module_option_ptr(__TEXT("output_segd_file"), tmp_buf2);
//
//	__matrix_zero_Memory(tmp_buf2, sizeof(tmp_buf2));
//	mbstowcs_s(&mbcs_size, NULL, 0, ad_data_ptr->file_header_.remote_ip_, 0);
//	mbstowcs_s(&mbcs_size, tmp_buf, mbcs_size, ad_data_ptr->file_header_.remote_ip_, mbcs_size);
//	__matrix_sprintf_s(tmp_buf2, __TEXT("\"%s\""), tmp_buf);
//	set_module_option_ptr(__TEXT("remote_ip"), tmp_buf2);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.remote_port_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("remote_port"), tmp_buf);
//	
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.shot_number_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("shot_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.FileNumber_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("file_number"), tmp_buf);
//	set_module_option_ptr(__TEXT("trace_file_number"), tmp_buf);
//	set_module_option_ptr(__TEXT("expanded_file_number2"), tmp_buf);	
//	set_module_option_ptr(__TEXT("expanded_file_number3"), tmp_buf);
//	set_module_option_ptr(__TEXT("extended_file_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%d"), (int)ad_data_ptr->file_header_.source_line_number_);
//	set_module_option_ptr(__TEXT("source_line_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%.1f"), ad_data_ptr->file_header_.source_point_number_);
//	set_module_option_ptr(__TEXT("source_point_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.sample_frequency_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("sample_frequency"), tmp_buf);// 采样率250、500、1000、2000、4000 微秒
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.sample_length_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("sample_length"), tmp_buf);// 采集长度1000 到 128000 毫秒
//	set_module_option_ptr(__TEXT("extended_record_length"), tmp_buf);
//	set_module_option_ptr(__TEXT("channel_set_end_time"), tmp_buf);
//	set_module_option_ptr(__TEXT("listen_time"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.trace_total_number_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("trace_total_number"), tmp_buf);// 记录道总数1 到 100000
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.source_type_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("source_type"), tmp_buf);// 震源类型0 = 无震源 1 = 脉冲 2 = 可控震源
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.sample_number_in_trace_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("sample_number_in_trace"), tmp_buf);// 记录道中样本数目1 到 128000
//	set_module_option_ptr(__TEXT("number_of_samples_per_trace"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.receiver_line_number_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("source_set_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.trace_total_number_ - ad_data_ptr->file_header_.receiver_line_number_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("channels_number"), tmp_buf);
//	set_module_option_ptr(__TEXT("extend_trace_number"), tmp_buf);
//	set_module_option_ptr(__TEXT("active_trace_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	_itow(ad_data_ptr->file_header_.receiver_line_number_, tmp_buf, 10);
//	set_module_option_ptr(__TEXT("auxiliary_trace_number"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%.1f"), ad_data_ptr->file_header_.tb_window_);
//	set_module_option_ptr(__TEXT("tb_window"), tmp_buf);// TB窗口值
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%f"), ad_data_ptr->file_header_.resistor_lower_limit_);
//	set_module_option_ptr(__TEXT("resistor_lower_limit"), tmp_buf);// 电阻下限
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%f"), ad_data_ptr->file_header_.resistor_higher_limit_);
//	set_module_option_ptr(__TEXT("resistor_higher_limit"), tmp_buf);// 电阻上限
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%f"), ad_data_ptr->file_header_.resistor_value_);
//	set_module_option_ptr(__TEXT("resistor_value"), tmp_buf); // 电阻值
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%f"), ad_data_ptr->file_header_.tilt_limit_);
//	set_module_option_ptr(__TEXT("tilt_limit"), tmp_buf);// 倾斜度界限
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%f"), ad_data_ptr->file_header_.tilt_value_);
//	set_module_option_ptr(__TEXT("tilt_value"), tmp_buf);//!< 倾斜度值
//
//	set_module_option_ptr(__TEXT("resistor_deviation"), ad_data_ptr->file_header_.resistor_deviation_ ? __TEXT("1") : __TEXT("0"));// 电阻误差
//	set_module_option_ptr(__TEXT("tilt_deviation"), ad_data_ptr->file_header_.tilt_deviation_ ? __TEXT("1") : __TEXT("0"));// 倾斜度误差
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	tmp_tm_ptr = gmtime(&ad_data_ptr->file_header_.datetime_);
//
//	__matrix_sprintf_s(tmp_buf, __TEXT("\"%d %d %d\""), tmp_tm_ptr->tm_mon, tmp_tm_ptr->tm_mday, tmp_tm_ptr->tm_year + 1900);
//	set_module_option_ptr(__TEXT("date"), tmp_buf);
//
//	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
//	__matrix_sprintf_s(tmp_buf, __TEXT("%d.%d.%d.%d.%d.%d"), (tmp_tm_ptr->tm_year % 100), tmp_tm_ptr->tm_yday + 1, 
//						tmp_tm_ptr->tm_mday, tmp_tm_ptr->tm_hour, tmp_tm_ptr->tm_min, tmp_tm_ptr->tm_sec);
//	set_module_option_ptr(__TEXT("datetime"), tmp_buf);
//
//	rel_module_ptr();
//
//GOTO:
//	if(module_handle)	FreeLibrary(module_handle);	
//	return is_ok;
//}
//

