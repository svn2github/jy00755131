/**
@brief	处理命令行传入配置文件
@details	针对--file命令传入参数指向的文件,来检查和生成新的SEGD文件,当前为实现文件
@file	proc.conf.file.cpp
@note	
@see	关于配置文件参看args.conf
@version	1.1.5.12.2011.09.07	
@author	kuoxin	
@bug
	1.1.6.20.2011.09.14	加入检测SEG-D配置文件中,参数和数据的有效性,并返回错误信息
						读取SEG-D配置文件,并写入预定的SEG-D头文件结构中
						保存SEG-D配置文件


*/

#include "segd.conv.h"
#include "proc.conf.file.h"

extern APPLICATION_ENVIRONMENT __application_environment;

SEGD_CONFIG_FILE	__segd_file;
CONFIG_FILE_LINE_INFO	__config_file_lines[MAX_CONFIG_FILE_LINE_SIZE];
unsigned int __config_file_line_size = 0x0;

int assign_ushort(void* value_ptr, const char* value_string);
int assign_string(void* value_ptr, const char* value_string);
int assign_datetime(void* value_ptr, const char* value_string);
int assign_uchar(void* value_ptr, const char* value_string);
int assign_uint(void* value_ptr, const char* value_string);
int assign_float(void* value_ptr, const char* value_string);
int assign_double(void* value_ptr, const char* value_string);
int assign_uint_array(void* value_ptr, const char* value_string);
int assign_ulonglong(void* value_ptr, const char* value_string);
int assign_unicode_string(void* value_ptr, const char* value_string);

SEGD_OPTION_ITEM __segd_option_items[] = 
{
	{"input_ad_dat_file", &__segd_file.dat_path_, assign_unicode_string},
	{"input_sps_file",  &__segd_file.sps_path_, assign_unicode_string},
	{"output_segd_file", &__segd_file.output_path_, assign_unicode_string},
	{"remote_ip", &__segd_file.remote_ip_, assign_unicode_string},
	{"remote_port", &__segd_file.remote_port_, assign_uint},

	//general header 1
	{"file_number", &__segd_file.header_1_.file_number_, assign_ushort},
	{"format_code",  &__segd_file.header_1_.format_code_, assign_ushort},
	{"general_constants", __segd_file.header_1_.general_constants_, assign_string},
	{"additional_blocks_in_general_header", &__segd_file.header_1_.additional_blocks_in_general_header_, assign_ushort},
	{"datetime", &__segd_file.header_1_, assign_datetime},
	{"manufacturer_code", &__segd_file.header_1_.manufacturer_code_, assign_ushort},
	{"manufacturer_serial_number", &__segd_file.header_1_.manufacturer_serial_number_, assign_ushort},
	{"bytes_per_scan", &__segd_file.header_1_.bytes_per_scan_, assign_uint},
	{"base_scan_interval", &__segd_file.header_1_.base_scan_interval_, assign_ushort},
	{"polarity", &__segd_file.header_1_.polarity_, assign_ushort},
	{"record_type", &__segd_file.header_1_.record_type_, assign_ushort},
	{"record_length", &__segd_file.header_1_.record_length_, assign_ushort},
	{"scan_types_record", &__segd_file.header_1_.scan_types_record_, assign_ushort},
	{"channal_sets_scan_type", &__segd_file.header_1_.channal_sets_scan_type_, assign_ushort},
	{"skew_blocks", &__segd_file.header_1_.skew_blocks_, assign_ushort},
	{"extended_header_block", &__segd_file.header_1_.extended_header_block_, assign_ushort},
	{"external_header_block", &__segd_file.header_1_.external_header_block_, assign_ushort},
	
	//general header 2
	{"expanded_file_number2", &__segd_file.header_2_.expanded_file_number_, assign_uint}, 
	{"extended_channel_sets_scan_type", &__segd_file.header_2_.extended_channel_sets_scan_type_, assign_ushort}, 
	{"extended_header_blocks", &__segd_file.header_2_.extended_header_blocks_, assign_ushort}, 
	{"external_header_blocks", &__segd_file.header_2_.external_header_blocks_, assign_ushort}, 
	{"major_segd_revision_number", &__segd_file.header_2_.major_segd_revision_number_, assign_ushort}, 
	{"minor_segd_revision_number", &__segd_file.header_2_.minor_segd_revision_number_, assign_ushort}, 
	{"number_of_blocks_of_general_trailer", &__segd_file.header_2_.number_of_blocks_of_general_trailer_, assign_ushort}, 
	{"extended_record_length", &__segd_file.header_2_.extended_record_length_, assign_uint}, 
	{"general_header_block_number2", &__segd_file.header_2_.general_header_block_number_, assign_ushort},

	//genernal header3
	{"expanded_file_number3", &__segd_file.header_3_.expanded_file_number_, assign_uint}, 
	{"source_line_number", &__segd_file.header_3_.source_line_number_, assign_float}, 
	{"source_point_number", &__segd_file.header_3_.source_point_number_, assign_float}, 
	{"source_point_index", &__segd_file.header_3_.source_point_index_, assign_ushort}, 
	{"phase_control", &__segd_file.header_3_.phase_control_, assign_ushort}, 
	{"type_vibrator", &__segd_file.header_3_.type_vibrator_, assign_ushort}, 
	{"phase_angle", &__segd_file.header_3_.phase_angle_, assign_ushort}, 
	{"general_header_block_number3", &__segd_file.header_3_.general_header_block_number_, assign_ushort}, 
	{"source_set_number", &__segd_file.header_3_.source_set_number_, assign_ushort}, 

	//scan type header
	{"scan_type_number", &__segd_file.scan_type_header_.scan_type_number_, assign_ushort}, 
	{"scan_channel_set_number", &__segd_file.scan_type_header_.channel_set_number_, assign_ushort}, 
	{"channel_set_start_time", &__segd_file.scan_type_header_.channel_set_start_time_, assign_ushort}, 
	{"channel_set_end_time", &__segd_file.scan_type_header_.channel_set_end_time_, assign_ushort}, 
	{"descale_multiplier", &__segd_file.scan_type_header_.descale_multiplier_, assign_ushort}, 
	{"channels_number", &__segd_file.scan_type_header_.channels_number_, assign_ushort}, 
	{"channel_type_identification", &__segd_file.scan_type_header_.channel_type_identification_, assign_ushort}, 
	{"sample_channel_subscans_number", &__segd_file.scan_type_header_.sample_channel_subscans_number_, assign_ushort}, 
	{"channel_gain_control", &__segd_file.scan_type_header_.channel_gain_control_, assign_ushort}, 
	{"alias_filter_frequency", &__segd_file.scan_type_header_.alias_filter_frequency_, assign_ushort}, 
	{"alias_filter_slope", &__segd_file.scan_type_header_.alias_filter_slope_, assign_ushort}, 
	{"low_cut_filter", &__segd_file.scan_type_header_.low_cut_filter_, assign_ushort}, 
	{"low_cut_filter_slope", &__segd_file.scan_type_header_.low_cut_filter_slope_, assign_ushort}, 
	{"first_notch_filter", &__segd_file.scan_type_header_.first_notch_filter_, assign_ushort}, 
	{"second_notch_filter", &__segd_file.scan_type_header_.second_notch_filter_, assign_ushort}, 
	{"third_notch_filter", &__segd_file.scan_type_header_.third_notch_filter_, assign_ushort}, 
	{"extended_channel_set_number", &__segd_file.scan_type_header_.extended_channel_set_number_, assign_ushort}, 
	{"extended_header_flag", &__segd_file.scan_type_header_.extended_header_flag_, assign_ushort}, 
	{"scan_trace_header_extension", &__segd_file.scan_type_header_.trace_header_extension_, assign_ushort}, 
	{"vertical_stack", &__segd_file.scan_type_header_.vertical_stack_, assign_ushort}, 
	{"streamer_number", &__segd_file.scan_type_header_.streamer_number_, assign_ushort}, 
	{"array_forming", &__segd_file.scan_type_header_.array_forming_, assign_ushort}, 

	//extend header
	{"sample_length", &__segd_file.extended_header_.sample_length_, assign_uint}, 
	{"sample_frequency", &__segd_file.extended_header_.sample_frequency_, assign_uint}, 
	{"trace_total_number", &__segd_file.extended_header_.trace_total_number_, assign_uint}, 
	{"auxiliary_trace_number", &__segd_file.extended_header_.auxiliary_trace_number_, assign_uint}, 
	{"extend_trace_number", &__segd_file.extended_header_.trace_number_, assign_uint}, 
	{"stopped_trace_number", &__segd_file.extended_header_.stopped_trace_number_, assign_uint}, 
	{"active_trace_number", &__segd_file.extended_header_.active_trace_number_, assign_uint}, 
	{"source_type", &__segd_file.extended_header_.source_type_, assign_uint}, 
	{"sample_number_in_trace", &__segd_file.extended_header_.sample_number_in_trace_, assign_uint}, 
	{"shot_number", &__segd_file.extended_header_.shot_number_, assign_uint}, 
	{"tb_window", &__segd_file.extended_header_.tb_window_, assign_float}, 
	{"test_type", &__segd_file.extended_header_.test_type_, assign_uint}, 
	{"line_first_test", &__segd_file.extended_header_.line_first_test_, assign_uint}, 
	{"line_first_number", &__segd_file.extended_header_.line_first_number_, assign_uint}, 
	{"array_number", &__segd_file.extended_header_.array_number_, assign_uint}, 
	{"array_type", &__segd_file.extended_header_.array_type_, assign_uint}, 
	{"time_break_signal", &__segd_file.extended_header_.time_break_signal_, assign_uint}, 
	{"well_time", &__segd_file.extended_header_.well_time_, assign_uint}, 
	{"blaster_serial_number", &__segd_file.extended_header_.blaster_serial_number_, assign_uint}, 
	{"blaster_status", &__segd_file.extended_header_.blaster_status_, assign_uint}, 
	{"refraction_delay", &__segd_file.extended_header_.refraction_delay_, assign_uint}, 
	{"tb_to_t0_time", &__segd_file.extended_header_.tb_to_t0_time_, assign_uint}, 
	{"internal_time_break_signal", &__segd_file.extended_header_.internal_time_break_signal_, assign_uint}, 
	{"field_unit_prestack", &__segd_file.extended_header_.field_unit_prestack_, assign_uint}, 
	{"Noise_elimination_type", &__segd_file.extended_header_.Noise_elimination_type_, assign_uint}, 
	{"low_trace_percent", &__segd_file.extended_header_.low_trace_percent_, assign_uint}, 
	{"low_trace_value", &__segd_file.extended_header_.low_trace_value_, assign_uint}, 
	{"window_number", &__segd_file.extended_header_.window_number_, assign_uint}, 
	{"historical_edit_type", &__segd_file.extended_header_.historical_edit_type_, assign_uint}, 
	{"noisy_trace_percent", &__segd_file.extended_header_.noisy_trace_percent_, assign_uint}, 
	{"historical_range", &__segd_file.extended_header_.historical_range_, assign_uint}, 
	{"historical_taper_length", &__segd_file.extended_header_.historical_taper_length_, assign_uint}, 
	{"threshold_changed", &__segd_file.extended_header_.threshold_changed_, assign_uint}, 
	{"historical_threshold_initial_value", &__segd_file.extended_header_.historical_threshold_initial_value_, assign_uint}, 
	{"historical_zeroing_length", &__segd_file.extended_header_.historical_zeroing_length_, assign_uint}, 
	{"process_type", &__segd_file.extended_header_.process_type_, assign_uint}, 
	{"sample_type_table", &__segd_file.extended_header_.sample_type_table_, assign_uint_array}, 
	{"threshold_value_table", &__segd_file.extended_header_.threshold_value_table_, assign_uint_array}, 
	{"overlap_number", &__segd_file.extended_header_.overlap_number_, assign_uint}, 
	{"extend_record_length", &__segd_file.extended_header_.record_length_, assign_uint}, 
	{"autocorrelative_peak_time", &__segd_file.extended_header_.autocorrelative_peak_time_, assign_uint}, 
	{"related_reference_signal_number", &__segd_file.extended_header_.related_reference_signal_number_, assign_uint}, 
	{"reference_signal_length", &__segd_file.extended_header_.reference_signal_length_, assign_uint}, 
	{"scan_length", &__segd_file.extended_header_.scan_length_, assign_uint}, 
	{"sample_number", &__segd_file.extended_header_.sample_number_, assign_uint}, 
	{"max_auxiliary_channel_max_value", &__segd_file.extended_header_.max_auxiliary_channel_max_value_, assign_float}, 
	{"max_channel_max_value", &__segd_file.extended_header_.max_channel_max_value_, assign_float}, 
	{"dump_overlap_number", &__segd_file.extended_header_.dump_overlap_number_, assign_uint}, 
	{"tape_lable", &__segd_file.extended_header_.tape_lable_, assign_string}, 
	{"tape_number", &__segd_file.extended_header_.tape_number_, assign_uint}, 
	{"version", &__segd_file.extended_header_.version_, assign_string}, 
	{"date", &__segd_file.extended_header_.date_, assign_string}, 
	{"source_easting", &__segd_file.extended_header_.source_easting_, assign_double}, 
	{"source_northing", &__segd_file.extended_header_.source_northing_, assign_double}, 
	{"Source_elevation", &__segd_file.extended_header_.Source_elevation_, assign_float}, 
	{"slip_sweep_mode", &__segd_file.extended_header_.slip_sweep_mode_, assign_uint}, 
	{"file_number_per_tape", &__segd_file.extended_header_.file_number_per_tape_, assign_uint}, 
	{"file_counter", &__segd_file.extended_header_.file_counter_, assign_uint}, 
	{"sample_error_explaination", &__segd_file.extended_header_.sample_error_explaination_, assign_string}, 
	{"filter_type", &__segd_file.extended_header_.filter_type_, assign_uint}, 
	{"stack_dumped", &__segd_file.extended_header_.stack_dumped_, assign_uint}, 
	{"stack_symbol", &__segd_file.extended_header_.stack_symbol_, assign_uint}, 
	{"prm_tilt_correction", &__segd_file.extended_header_.prm_tilt_correction_, assign_uint}, 
	{"line_name", &__segd_file.extended_header_.line_name_, assign_string}, 
	{"operation_mode", &__segd_file.extended_header_.operation_mode_, assign_uint}, 
	{"non_log", &__segd_file.extended_header_.non_log_, assign_uint}, 
	{"listen_time", &__segd_file.extended_header_.listen_time_, assign_uint}, 
	{"dump_type", &__segd_file.extended_header_.dump_type_, assign_uint}, 
	{"line_id", &__segd_file.extended_header_.line_id_, assign_uint}, 

	//demux trace
	{"trace_file_number", &__segd_file.trace_header_.demux_trace_header_.file_number_, assign_ushort}, 
	{"trace_scan_type_number", &__segd_file.trace_header_.demux_trace_header_.scan_type_number_, assign_ushort}, 
	{"trace_channel_set_number", &__segd_file.trace_header_.demux_trace_header_.channel_set_number_, assign_ushort}, 
	{"trace_number", &__segd_file.trace_header_.demux_trace_header_.trace_number_, assign_ushort}, 
	{"first_timing_word", &__segd_file.trace_header_.demux_trace_header_.first_timing_word_, assign_uint}, 
	{"trace_header_extension", &__segd_file.trace_header_.demux_trace_header_.trace_header_extension_, assign_ushort}, 
	{"sample_skew", &__segd_file.trace_header_.demux_trace_header_.sample_skew_, assign_ushort}, 
	{"trace_edit", &__segd_file.trace_header_.demux_trace_header_.trace_edit_, assign_ushort}, 
	{"time_break_window", &__segd_file.trace_header_.demux_trace_header_.time_break_window_, assign_float}, 
	{"trace_extended_channel_set_number", &__segd_file.trace_header_.demux_trace_header_.extended_channel_set_number_, assign_ushort}, 
	{"extended_file_number", &__segd_file.trace_header_.demux_trace_header_.extended_file_number_, assign_uint}, 

	//TRACE_HEADER_EXTENSION_1
	{"receiver_line_number", &__segd_file.trace_header_.trace_header_extended_1_.receiver_line_number_, assign_uint}, 
	{"receiver_point_number", &__segd_file.trace_header_.trace_header_extended_1_.receiver_point_number_, assign_uint}, 
	{"receiver_point_index", &__segd_file.trace_header_.trace_header_extended_1_.receiver_point_index_, assign_ushort}, 
	{"number_of_samples_per_trace", &__segd_file.trace_header_.trace_header_extended_1_.number_of_samples_per_trace_, assign_uint}, 
	{"extended_receiver_line_number", &__segd_file.trace_header_.trace_header_extended_1_.extended_receiver_line_number_, assign_ulonglong}, 
	{"extended_receiver_point_number", &__segd_file.trace_header_.trace_header_extended_1_.extended_receiver_point_number_, assign_ulonglong}, 
	{"sensor_type", &__segd_file.trace_header_.trace_header_extended_1_.sensor_type_, assign_ushort}, 


	//TRACE_HEADER_EXTENSION_2
	{"receiver_point_east", &__segd_file.trace_header_.trace_header_extended_2_.receiver_point_east_, assign_double}, 
	{"receiver_point_north", &__segd_file.trace_header_.trace_header_extended_2_.receiver_point_north_, assign_double}, 
	{"receiver_point_high", &__segd_file.trace_header_.trace_header_extended_2_.receiver_point_high_, assign_float}, 
	{"detector_type_number", &__segd_file.trace_header_.trace_header_extended_2_.detector_type_number_, assign_uchar}, 
	{"extended_trace_number", &__segd_file.trace_header_.trace_header_extended_2_.extended_trace_number_, assign_uint}, 

	//TRACE_HEADER_EXTENSION_3
	{"resistor_lower_limit", &__segd_file.trace_header_.trace_header_extended_3_.resistor_lower_limit_, assign_float}, 
	{"resistor_higher_limit", &__segd_file.trace_header_.trace_header_extended_3_.resistor_higher_limit_, assign_float}, 
	{"resistor_value", &__segd_file.trace_header_.trace_header_extended_3_.resistor_value_, assign_float}, 
	{"tilt_limit", &__segd_file.trace_header_.trace_header_extended_3_.tilt_limit_, assign_float}, 
	{"tilt_value", &__segd_file.trace_header_.trace_header_extended_3_.tilt_value_, assign_float}, 
	{"resistor_deviation", &__segd_file.trace_header_.trace_header_extended_3_.resistor_deviation_, assign_ushort}, 
	{"tilt_deviation", &__segd_file.trace_header_.trace_header_extended_3_.tilt_deviation_, assign_ushort}, 

};

const SEGD_CONFIG_FILE_PTR get_segd_config_info_ptr(){return & __segd_file;}

int load_config_file(char_t const* config_path)
{
	int is_ok = APP_SUCCESS;
	HANDLE file_handle = NULL;
	char buf[MAX_STRING_SIZE] = {0};
	char line[MAX_STRING_SIZE * 2] = {0};
	char *ptr = NULL;
	DWORD size = 0x0, i = 0, cur_line_size = 0x0, j = 0, arg_size = 0;

	__matrix_zero_Memory(&__segd_file, sizeof(__segd_file));
	__matrix_zero_Memory(&__config_file_lines, sizeof(__config_file_lines));
	__config_file_line_size = 0x0;

	__matrix_strcpy_s(__segd_file.config_path_, sizeof(__segd_file.config_path_)/sizeof(__segd_file.config_path_[0]), config_path);

	file_handle = CreateFile(config_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
										NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(file_handle == INVALID_HANDLE_VALUE){
		set_last_error(IDS_STRING5008);
		output_log(IDS_STRING5008, get_last_error());
		is_ok = APP_FAULT;
		goto GOTO;
	}

	//read all lines
	while(ReadFile(file_handle, buf, sizeof(buf), &size,NULL)){
		
		if(size == 0) break;
		for(i = 0; i < size; ++i){
			if(buf[i] == 0x0d){
			
				if(cur_line_size == 0) continue;

				memcpy(__config_file_lines[__config_file_line_size].line_, line, cur_line_size);
				__config_file_lines[__config_file_line_size].line_[cur_line_size] = NULL;

				StrTrimA(line, " 	");
				cur_line_size = strlen(line);
				if(cur_line_size == 0) continue;

				//parse all option
				ptr = __config_file_lines[__config_file_line_size].key_name_;
				for(arg_size = 1, j = 0; j < cur_line_size; ++j){

					if(line[j] == '#'){	break;}
					else if(line[j] == ' '){

						if(line[j] == line[j+1]) continue;
						if(arg_size == 2){ *ptr++ = line[j];}
						else{
							*++ptr = NULL;
							ptr = __config_file_lines[__config_file_line_size].value_;
							++arg_size;
						}
					}
					else{	*ptr++ = line[j];	}
				}

				cur_line_size = 0x0;
				memset(line, 0x0, sizeof(line));
				++__config_file_line_size;
			}
			else{

				if(buf[i] != 0xa){
					line[cur_line_size] = buf[i];
					++cur_line_size;
				}
			}
		}
	}

	size = sizeof(__segd_option_items) / sizeof(__segd_option_items[0]);
	for(i = 0; i < __config_file_line_size; ++i){

		if(strlen(__config_file_lines[i].key_name_) == 0) continue;
		for(j = 0; j < size; ++j){			
			if(strcmp(__config_file_lines[i].key_name_, __segd_option_items[j].key_name_) == 0){				
				__segd_option_items[j].function_ptr_(__segd_option_items[j].value_ptr_, __config_file_lines[i].value_);
				break;
			}
		}
	}
GOTO:
	if(file_handle != INVALID_HANDLE_VALUE)	CloseHandle(file_handle);
	return is_ok;
}

int check_config_file(char_t const* config_path)
{
	int i_ret = APP_SUCCESS, is_ok = APP_SUCCESS;
	HANDLE file_handle = NULL;
	char buf[MAX_STRING_SIZE] = {0};
	char line[MAX_STRING_SIZE * 2] = {0};
	char *ptr = NULL;
	DWORD size = 0x0, i = 0, cur_line_size = 0x0, j = 0, arg_size = 0;

	wchar_t tmp_value[MAX_STRING_SIZE] = {0};
	size_t mbcs_size = 0x0;

	__matrix_zero_Memory(&__config_file_lines, sizeof(__config_file_lines));
	__config_file_line_size = 0x0;

	file_handle = CreateFile(config_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(file_handle == INVALID_HANDLE_VALUE){

		set_last_error(IDS_STRING5008);
		output_log(IDS_STRING5008, get_last_error());
		return APP_FAULT;
	}

	//read all lines
	while(ReadFile(file_handle, buf, sizeof(buf), &size,NULL)){

		if(size == 0) break;
		for(i = 0; i < size; ++i){
			if(buf[i] == 0x0d){

				if(cur_line_size == 0) continue;

				memcpy(__config_file_lines[__config_file_line_size].line_, line, cur_line_size);
				__config_file_lines[__config_file_line_size].line_[cur_line_size] = NULL;

				StrTrimA(line, " 	");
				cur_line_size = strlen(line);
				if(cur_line_size == 0) continue;

				//parse all option
				ptr = __config_file_lines[__config_file_line_size].key_name_;
				for(arg_size = 1, j = 0; j < cur_line_size; ++j){

					if(line[j] == '#'){	break;}
					else if(line[j] == ' '){

						if(line[j] == line[j+1]) continue;
						if(arg_size == 2){ *ptr++ = line[j];}
						else{
							*++ptr = NULL;
							ptr = __config_file_lines[__config_file_line_size].value_;
							++arg_size;
						}
					}
					else{	*ptr++ = line[j];	}
				}

				cur_line_size = 0x0;
				memset(line, 0x0, sizeof(line));
				++__config_file_line_size;
			}
			else{

				if(buf[i] != 0xa){
					line[cur_line_size] = buf[i];
					++cur_line_size;
				}
			}
		}
	}

	if(file_handle != INVALID_HANDLE_VALUE)	CloseHandle(file_handle);

	arg_size = sizeof(__segd_option_items) / sizeof(__segd_option_items[0]);
	for(i = 0; i < __config_file_line_size; ++i){

		is_ok = APP_FAULT;
		if(strlen(__config_file_lines[i].key_name_) == 0) continue;
		for(j = 0; j < arg_size; ++j){			
			if(strcmp(__config_file_lines[i].key_name_, __segd_option_items[j].key_name_) == 0){
				is_ok = APP_SUCCESS;
				break;
			}
		}

		if(is_ok == APP_FAULT){
			set_last_error(IDS_STRING5013);
			mbstowcs_s(&mbcs_size, NULL, 0, __config_file_lines[i].key_name_, 0);
			mbstowcs_s(&mbcs_size, tmp_value, mbcs_size, __config_file_lines[i].key_name_, mbcs_size);
			tmp_value[mbcs_size] = 0x0;
			output_log(IDS_STRING5013, tmp_value);
			if(i_ret == APP_SUCCESS && is_ok == APP_FAULT)  i_ret = is_ok;
		}
	}

	__config_file_line_size = 0x0;
	__matrix_zero_Memory(__config_file_lines, sizeof(__config_file_lines));

	return i_ret;
}

int check_segd_option(char_t const* option/*, char const* option_value, unsigned option_value_size*/)
{
	int i_ret = APP_FAULT;
	int i = 0;
	size_t size = 0;
	char tmp_buf[MAX_STRING_SIZE*2] = {0}; 	
	errno_t err_code = 0x0;

#if defined(UNICODE) || defined(_UNICODE)
	
	err_code = wcstombs_s(&size, tmp_buf, sizeof(tmp_buf), option, MAX_STRING_SIZE);
	if(err_code != 0){
		set_last_error(IDS_STRING5010);
		output_log(IDS_STRING5010, option);
		return i_ret;
	}

	tmp_buf[size] = 0x0;
#else 
	strcpy(tmp_buf, option, strlen(option) + 1);
#endif //defined(UNICODE) || defined(_UNICODE)

	size = sizeof(__segd_option_items)/sizeof(__segd_option_items[0]);

	for(i = 0; i < size; ++i){
		if(_stricmp(tmp_buf, __segd_option_items[i].key_name_) == 0)
		{	i_ret = APP_SUCCESS; break; }
	}

	if(i_ret != APP_SUCCESS){
		set_last_error(IDS_STRING5009);
		output_log(IDS_STRING5009, option);
	}
	return i_ret;
}

int modify_segd_option(char_t const* option, char const* option_value/*, unsigned option_value_size*/)
{
	int i_ret = APP_SUCCESS;
	int i = 0;
	size_t size = 0;

	char tmp_buf[MAX_STRING_SIZE*2] = {0}; 	
	errno_t err_code = 0x0;

#if defined(UNICODE) || defined(_UNICODE)
	err_code = wcstombs_s(&size, tmp_buf, sizeof(tmp_buf), option, MAX_STRING_SIZE);
	if(err_code != 0){
		set_last_error(IDS_STRING5010);
		output_log(IDS_STRING5010, option);
		return i_ret = APP_FAULT;
	}

#else 
	strcpy(tmp_buf, option);
#endif //defined(UNICODE) || defined(_UNICODE)

	tmp_buf[size] = 0x0;
	size = sizeof(__segd_option_items)/sizeof(__segd_option_items[0]);

	for(i = 0; i < size; ++i)	if(strcmp(tmp_buf, __segd_option_items[i].key_name_) == 0)	break;

	i_ret = __segd_option_items[i].function_ptr_(__segd_option_items[i].value_ptr_, option_value);
	if(i_ret != APP_SUCCESS){	set_last_error(IDS_STRING5009);		output_log(IDS_STRING5009, option);	return i_ret;}

	for(i = 0; i < __config_file_line_size; ++i)
		if(strcmp(tmp_buf, __config_file_lines[i].key_name_) == 0) break;

	
	StrCpyA(__config_file_lines[i].value_, option_value);
	wnsprintfA(__config_file_lines[i].line_, sizeof(__config_file_lines[i].line_), "%s %s", tmp_buf, option_value);

	return i_ret;
}

int save_config_file(char_t const* config_path)
{
	int i = 0;
	int size = 0;
	HANDLE file_handle = NULL;
	char tmp_buf[MAX_STRING_SIZE] = {0};

	file_handle	= CreateFile(config_path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(file_handle == INVALID_HANDLE_VALUE){

		set_last_error(IDS_STRING5008);
		output_log(IDS_STRING5008, get_last_error());
		return APP_FAULT;
	}

	for(i = 0; i < __config_file_line_size; ++i){
		strcpy(tmp_buf, __config_file_lines[i].line_);
		size = strlen(tmp_buf);
		tmp_buf[size++] = 0xd;
		tmp_buf[size++] = 0xa;
		tmp_buf[size] = NULL;
		if(!WriteFile(file_handle, tmp_buf, size, (DWORD*)&size, NULL)){
			set_last_error(IDS_STRING5011);
			output_log(IDS_STRING5011, get_last_error());
		}
	}

	if(file_handle != INVALID_HANDLE_VALUE) CloseHandle(file_handle);
	return APP_SUCCESS;
}

int assign_ushort(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
	StrTrimA(tmp_value, " 	");

	*((unsigned short*)value_ptr) = static_cast<unsigned short>(StrToIntA(tmp_value));

	return i_ret;
}

/**
@fn int assign_string(void* value_ptr, const char* value_string)
@note 对于value_string来说必须使用双引号,例如: tape_lable "matrixAL28"
*/
int assign_string(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};
	char* ch_ptr = tmp_value;

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
	while(*ch_ptr)
		if(*ch_ptr == '\"') *ch_ptr++ = ' ';
		else				++ch_ptr;

	StrTrimA(tmp_value, " 	");
	
	
	StrCpyA((char*)value_ptr, tmp_value);
	return i_ret;
}

/**
@fn int assign_datetime(void* value_ptr, const char* value_string)
@note	datetime 11.112.16.02.44 #格式要求XX.XXX.XX.XX.XX不足补零
*/
int assign_datetime(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	SEGD_GENERAL_HEADER_1_PTR header = (SEGD_GENERAL_HEADER_1_PTR)value_ptr;
	char tmp_value[MAX_STRING_SIZE] = {0};
	char tmp_value2[24] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
	StrTrimA(tmp_value, " 	");

	memcpy(tmp_value2, tmp_value, 2);
	tmp_value2[2] = 0x0;
	header->year_ = atoi(tmp_value2);
	
	memcpy(tmp_value2, tmp_value + 3, 3);
	tmp_value2[3] = 0x0;
	header->day_ = atoi(tmp_value2);

	memcpy(tmp_value2, tmp_value + 7, 2);
	tmp_value2[2] = 0x0;
	header->hour_ = atoi(tmp_value2);

	memcpy(tmp_value2, tmp_value + 10, 2);
	tmp_value2[2] = 0x0;
	header->minute_ = atoi(tmp_value2);

	memcpy(tmp_value2, tmp_value + 13, 2);
	tmp_value2[2] = 0x0;
	header->second_ = atoi(tmp_value2);

	return i_ret;
}

int assign_uint_array(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	int size = 0x0;
	int* i_ptr = NULL;
	char* ptr = NULL;
	char tmp_value[MAX_STRING_SIZE] = {0};
	char tmp_value2[24] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
	StrTrimA(tmp_value, " 	");
	ptr = tmp_value;
	i_ptr = (int*)value_ptr;

	while(*ptr){
		
		if(*ptr == ','){
			*i_ptr = atoi(tmp_value2);
			++i_ptr;
			size = 0;
			__matrix_zero_Memory(tmp_value2, sizeof(tmp_value2));
		}
		else{
			tmp_value2[size++] = *ptr;
		}
		
		++ptr;
	}

	if(size > 0){	*i_ptr = atoi(tmp_value2);	}

	return i_ret;
}

int assign_uchar(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE];
	
	__matrix_zero_Memory(tmp_value, sizeof(tmp_value));
	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
	StrTrimA(tmp_value, " 	");
	*(char*)value_ptr = tmp_value[0];

	return i_ret;
}

int assign_uint(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
//	StrTrimA(tmp_value, " 	");

	*((unsigned int*)value_ptr) = StrToIntA(tmp_value);

	return i_ret;
}

int assign_float(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
//	StrTrimA(tmp_value, " 	");

	*((float*)value_ptr) = atof(tmp_value);

	return i_ret;
}

int assign_double(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
//	StrTrimA(tmp_value, " 	");

	*((double*)value_ptr) = strtod(tmp_value, NULL);

	return i_ret;
}

int assign_ulonglong(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);
//	StrTrimA(tmp_value, " 	");

	*((__int64*)value_ptr) = _atoi64(tmp_value);
	return i_ret;
}

int assign_unicode_string(void* value_ptr, const char* value_string)
{
	int i_ret = APP_SUCCESS;
	char tmp_value[MAX_STRING_SIZE] = {0};
	char* ch_ptr = tmp_value;
	size_t size = 0;
	wchar_t wc_tmp_value[MAX_STRING_SIZE] = {0};

	strcpy_s(tmp_value, MAX_STRING_SIZE - 1, value_string);

	while(*ch_ptr)
		if(*ch_ptr == '\"') *ch_ptr++ = ' ';
		else				++ch_ptr;

	StrTrimA(tmp_value, " 	");
	mbstowcs_s(&size, NULL, 0, tmp_value, 0);
	mbstowcs_s(&size, wc_tmp_value, size, tmp_value, size);
	wc_tmp_value[size] = 0x0;

	if(size == (size_t) -1){
		set_last_error(IDS_STRING5012);
		output_log(IDS_STRING5012);
		return i_ret = APP_FAULT;
	}

	StrCpy((wchar_t*)value_ptr, wc_tmp_value);
	return i_ret;
}
