/**
@brief	兼容428XL格式
@details 实现对于428XL手册中定义的SEGD格式
@file	segd.file.format.428XL.compatible.h
@note	程序中部分实现使用了结构体,用块读取方式实现,因此要注意对齐问题
@see	参看428XLUser's Manual Vol2
@version	1.1.22.45.2011.10.08
@author	kuoxin	
@bug
	1.1.22.45.2011.10.08	调整程序,加入对于外部头段和跟踪到4-7道头的读取,修改错误有关读取外部头段
*/

/**
@brief	该文件中大部分代码都是由python生成
@code

import string, sys

start_str = """	int i = 0x0;
	SEGD_TRACE_HEADER_EXTENSION_x_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_x_PTR ptr = trace_header_extended;

	if(be_reverted_private){
"""

else_str = """	}
	else{
		
"""

end_str = """	}"""

assign_str_1 = "ptr_p->{0[1]} = ptr->{0[1]};\n" 
assign_str_2 = "ptr->{0[1]} = ptr_p->{0[1]};\n"
memcpy_str_1 = "memcpy(ptr_p->{0[1]}, ptr->{0[1]}, {0[2]} * sizeof({0[0]}));\n"
memcpy_str_2 = "memcpy(ptr->{0[1]}, ptr_p->{0[1]}, {0[2]} * sizeof({0[0]}));\n"

ushort_str_1 = "convert_ushort_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
ushort_str_2 = "convert_ushort_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
ushort_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_ushort_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
ushort_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_ushort_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

uint_str_1 = "convert_ulong_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
uint_str_2 = "convert_ulong_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
uint_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_ulong_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
uint_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_ulong_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

float_str_1 = "convert_float_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
float_str_2 = "convert_float_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
float_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_float_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
float_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_float_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

double_str_1 = "convert_double_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
double_str_2 = "convert_double_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
double_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_double_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
double_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_double_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

def format_code():
	new_file = open('d:\\new_file.txt', 'w')
	new_file.write(start_str)
	file_handle = open('d:\\conv.txt', 'r')
	all_lines = file_handle.readlines()
	for line in all_lines:
		token = []
		index = line.find(';')
		if index != -1:
			str = line[:index]
		else:
			str = line
		str = str.strip()
		list = str.split(' ')
		if len(list) < 2:
			continue
		token.append(list[0])
		index = list[1].find('[')
		if index != -1:
			str = list[1].strip()
			list = str.split('[')
			token.append(list[0])
			index = list[1].find(']')
			if index != -1:
				str = list[1]
				list = str.split(']')
				token.append(list[0])
			else:
				print("have a error about ']'")
		else:
			token.append(list[1])			
		if token[0] == 'SEGD_UCHAR':
			if len(token) == 3:
				str1 = memcpy_str_1.format(token)
			else:
				str1 = assign_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_USHORT':
			if len(token) == 3:
				str1 = ushort_for_str_1.format(token)
			else:
				str1 = ushort_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_UINT':
			if len(token) == 3:
				str1 = uint_for_str_1.format(token)
			else:
				str1 = uint_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_FLOAT':
			if len(token) == 3:
				str1 = float_for_str_1.format(token)
			else:
				str1 = float_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_DOUBLE':
			if len(token) == 3:
				str1 = double_for_str_1.format(token)
			else:
				str1 = double_str_1.format(token)
			new_file.write(str1)
	new_file.write(else_str)
	
	for line in all_lines:
		token=[]
		index = line.find(';')
		if index != -1:
			str = line[:index]
		else:
			str = line
		str = str.strip()
		list = str.split(' ')
		if len(list) < 2:
			continue
		token.append(list[0])
		index = list[1].find('[')
		if index != -1:
			str = list[1].strip()
			list = str.split('[')
			token.append(list[0])
			index = list[1].find(']')
			if index != -1:
				str = list[1]
				list = str.split(']')
				token.append(list[0])
			else:
				print("have a error about ']'")
		else:
			token.append(list[1])
		if token[0] == 'SEGD_UCHAR':
			if len(token) == 3:
				str1 = memcpy_str_2.format(token)
			else:
				str1 = assign_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_USHORT':
			if len(token) == 3:
				str1 = ushort_for_str_2.format(token)
			else:
				str1 = ushort_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_UINT':
			if len(token) == 3:
				str1 = uint_for_str_2.format(token)
			else:
				str1 = uint_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_FLOAT':
			if len(token) == 3:
				str1 = float_for_str_2.format(token)
			else:
				str1 = float_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_DOUBLE':
			if len(token) == 3:
				str1 = double_for_str_2.format(token)
			else:
				str1 = double_str_2.format(token)
			new_file.write(str1)	
	new_file.write(end_str)
	new_file.close()
	file_handle.close()
if __name__ == '__main__':
    format_code()

@endcode

*/


void segd_file_convert_general_header_1_private(SEGD_GENERAL_HEADER_1_PTR,
												SEGD_GENERAL_HEADER_1_PRIVATE_PTR, 
												bool	be_reverted_private = false);

void segd_file_convert_general_header_2_private(SEGD_GENERAL_HEADER_2_PTR,
												SEGD_GENERAL_HEADER_2_PRIVATE_PTR, 
												bool	be_reverted_private = false);

void segd_file_convert_general_header_3_private(SEGD_GENERAL_HEADER_3_PTR,
												SEGD_GENERAL_HEADER_3_PRIVATE_PTR, 
												bool	be_reverted_private = false);

void segd_file_convert_scan_type_header_private(SEGD_SCAN_TYPE_HEADER_PTR,
												SEGD_SCAN_TYPE_HEADER_PRIVATE_PTR,
												bool	be_reverted_private = false);

void segd_file_convert_extended_header_private(SEGD_EXTENDED_HEADER_PTR, 
											   SEGD_EXTENDED_HEADER_PRIVATE_PTR,
											   bool	be_reverted_private = false);

void segd_file_convert_external_header_private(SEGD_EXTERNAL_HEADER_PTR,
											   SEGD_EXTERNAL_HEADER_PRIVATE_PTR,
											   bool	be_reverted_private = false);

void segd_file_convert_demux_trace_header_private(SEGD_DEMUX_TRACE_HEADER_PTR,
												  SEGD_DEMUX_TRACE_HEADER_PRIVATE_PTR,
												  bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_1_private(SEGD_TRACE_HEADER_EXTENSION_1_PTR,
														SEGD_TRACE_HEADER_EXTENSION_1_PRIVATE_PTR,
														bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_2_private(SEGD_TRACE_HEADER_EXTENSION_2_PTR,
														SEGD_TRACE_HEADER_EXTENSION_2_PRIVATE_PTR,
														bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_3_private(SEGD_TRACE_HEADER_EXTENSION_3_PTR,
														SEGD_TRACE_HEADER_EXTENSION_3_PRIVATE_PTR,
														bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_4_private(SEGD_TRACE_HEADER_EXTENSION_4_PTR,
														SEGD_TRACE_HEADER_EXTENSION_4_PRIVATE_PTR,
														bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_5_private(SEGD_TRACE_HEADER_EXTENSION_5_PTR,
														SEGD_TRACE_HEADER_EXTENSION_5_PRIVATE_PTR,
														bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_6_private(SEGD_TRACE_HEADER_EXTENSION_6_PTR,
														SEGD_TRACE_HEADER_EXTENSION_6_PRIVATE_PTR,
														bool	be_reverted_private = false);

void segd_file_convert_trace_header_extension_7_private(SEGD_TRACE_HEADER_EXTENSION_7_PTR,	
														SEGD_TRACE_HEADER_EXTENSION_7_PRIVATE_PTR,
														bool	be_reverted_private = false);


void segd_file_convert_general_header_1_private(SEGD_GENERAL_HEADER_1_PTR	general_header,
												SEGD_GENERAL_HEADER_1_PRIVATE_PTR	general_header_private, 
												bool	be_reverted_private)
{
	SEGD_GENERAL_HEADER_1_PTR ptr = general_header;
	SEGD_GENERAL_HEADER_1_PRIVATE_PTR ptr_p = general_header_private;
	unsigned short tmp = 0x0;

	if(be_reverted_private){

		convert_ushort_to_bcd(ptr->file_number_,&ptr->file_number_);
		convert_ushort_to_bcd(ptr->format_code_,&ptr->format_code_);

		convert_ushort_littlend_to_bigend(&ptr->file_number_, &ptr_p->file_number_);		
		convert_ushort_littlend_to_bigend(&ptr->format_code_, &ptr_p->format_code_);

		memcpy(ptr_p->general_constants_, ptr->general_constants_, sizeof(ptr->general_constants_));

		convert_uchar_to_bcd(ptr->year_, &ptr_p->year_);

		ptr_p->additional_blocks_in_general_header_ = ptr->additional_blocks_in_general_header_;
		convert_uchar_to_bcd(ptr_p->additional_blocks_in_general_header_, &ptr_p->additional_blocks_in_general_header_);
		ptr_p->additional_blocks_in_general_header_ <<= 4;

		convert_ushort_to_bcd(ptr->day_, &tmp);
		ptr_p->additional_blocks_in_general_header_ |= static_cast<unsigned char>(tmp >> 8);
		ptr_p->day_ = (tmp & 0xff);

		convert_uchar_to_bcd(ptr->hour_, &ptr_p->hour_);
		convert_uchar_to_bcd(ptr->minute_, &ptr_p->minute_);
		convert_uchar_to_bcd(ptr->second_, &ptr_p->second_);
		convert_uchar_to_bcd(ptr->manufacturer_code_, &ptr_p->manufacturer_code_);

		convert_ushort_to_bcd(ptr->manufacturer_serial_number_, &ptr->manufacturer_serial_number_);
		convert_ushort_littlend_to_uchar_array(ptr->manufacturer_serial_number_, ptr_p->manufacturer_serial_number_);
		memcpy(((unsigned char*)&ptr->bytes_per_scan_) + 1, &ptr_p->bytes_per_scan_, 3);

		convert_uchar_to_bcd(ptr->base_scan_interval_, &ptr_p->base_scan_interval_);
		ptr_p->polarity_ = ptr->polarity_>>4;
		ptr_p->undefined_2_ = ptr->undefined_2_;

		ptr_p->record_type_ = ptr->record_type_; //high
		convert_uchar_to_bcd(ptr_p->record_type_, &ptr_p->record_type_);
		ptr_p->record_type_ <<= 4;
		ptr_p->record_type_ |= 0xf; //low

		ptr_p->record_length_ = 0xff; // record_type_ low 4 bit + record_length_

		convert_uchar_to_bcd(ptr->scan_types_record_, &ptr_p->scan_types_record_);
		convert_uchar_to_bcd(ptr->channal_sets_scan_type_, &ptr_p->channal_sets_scan_type_);
		ptr_p->skew_blocks_ = ptr->skew_blocks_;
		convert_uchar_to_bcd(ptr->extended_header_block_, &ptr_p->extended_header_block_);
		convert_uchar_to_bcd(ptr->external_header_block_, &ptr_p->external_header_block_);
	}
	else{

		convert_ushort_bigend_to_littlend(&ptr_p->file_number_, &ptr->file_number_);
		convert_ushort_bigend_to_littlend(&ptr_p->format_code_, &ptr->format_code_);

		convert_bcd_to_ushort(ptr->file_number_, &ptr->file_number_);
		convert_bcd_to_ushort(ptr->format_code_, &ptr->format_code_);

		memcpy(ptr->general_constants_, ptr_p->general_constants_, sizeof(ptr_p->general_constants_));

		convert_bcd_to_uchar(ptr_p->year_, &ptr->year_);
		convert_bcd_to_uchar(ptr_p->additional_blocks_in_general_header_ >> 4, &ptr->additional_blocks_in_general_header_); //high bit

		ptr->day_ = ptr_p->additional_blocks_in_general_header_ & 0xf;// additional_blk low four bit + day
		ptr->day_ <<= 8;
		ptr->day_ += ptr_p->day_;
		convert_bcd_to_ushort(ptr->day_, &ptr->day_);

		convert_bcd_to_uchar(ptr_p->hour_, &ptr->hour_);
		convert_bcd_to_uchar(ptr_p->minute_, &ptr->minute_);
		convert_bcd_to_uchar(ptr_p->second_, &ptr->second_);
		convert_bcd_to_uchar(ptr_p->manufacturer_code_, &ptr->manufacturer_code_);

		convert_uchar_array_to_ushort_littlend(ptr_p->manufacturer_serial_number_, &ptr->manufacturer_serial_number_);
		convert_bcd_to_ushort(ptr->manufacturer_serial_number_, &ptr->manufacturer_serial_number_);

		memcpy(ptr_p->bytes_per_scan_, &ptr->bytes_per_scan_, 3);

		convert_bcd_to_uchar(ptr_p->base_scan_interval_, &ptr->base_scan_interval_);
		ptr->polarity_ = ptr_p->polarity_>>4;
		ptr->undefined_2_ = ptr_p->undefined_2_;
		convert_bcd_to_uchar(ptr_p->record_type_>>4, &ptr->record_type_);

		ptr->record_length_ = ptr_p->record_type_ & 0xf;
		ptr->record_length_ <<= 8;
		ptr->record_length_ += ptr_p->record_length_; // record_type_ low 4 bit + record_length_

		convert_bcd_to_uchar(ptr_p->scan_types_record_, &ptr->scan_types_record_);
		convert_bcd_to_uchar(ptr_p->channal_sets_scan_type_, &ptr->channal_sets_scan_type_);
		ptr_p->skew_blocks_ = ptr->skew_blocks_;
		convert_bcd_to_uchar(ptr_p->extended_header_block_, &ptr->extended_header_block_);
		convert_bcd_to_uchar(ptr_p->external_header_block_, &ptr->external_header_block_);
	}
}

void segd_file_convert_general_header_2_private(SEGD_GENERAL_HEADER_2_PTR	general_header,
												SEGD_GENERAL_HEADER_2_PRIVATE_PTR	general_header_private, 
												bool	be_reverted_private)
{
	SEGD_GENERAL_HEADER_2_PTR ptr =	general_header;
	SEGD_GENERAL_HEADER_2_PRIVATE_PTR ptr_p = general_header_private;

	if(be_reverted_private){ //private <-- gen

		convert_uint_littlend_to_uchar_array(ptr->expanded_file_number_, ptr_p->expanded_file_number_, 3);
		convert_ushort_littlend_to_uchar_array(ptr->extended_channel_sets_scan_type_, ptr_p->extended_channel_sets_scan_type_);
		convert_ushort_littlend_to_uchar_array(ptr->extended_header_blocks_, ptr_p->extended_header_blocks_);
		convert_ushort_littlend_to_uchar_array(ptr->external_header_blocks_, ptr_p->external_header_blocks_);

		ptr_p->undefined_1_ = ptr->undefined_1_;

		ptr_p->major_segd_revision_number_ = ptr->major_segd_revision_number_;
		ptr_p->minor_segd_revision_number_ = ptr->minor_segd_revision_number_;

		ptr_p->number_of_blocks_of_general_trailer_ = ptr->number_of_blocks_of_general_trailer_;

		convert_uint_littlend_to_uchar_array(ptr->extended_record_length_, ptr_p->extended_record_length_, 3);

		ptr_p->undefined_2_ = ptr->undefined_2_;
		ptr_p->general_header_block_number_ = ptr->general_header_block_number_;
		memcpy(ptr_p->undefined_3_, ptr->undefined_3_, sizeof(ptr_p->undefined_3_));

	}
	else{//private --> gen

		convert_uchar_array_to_uint_littlend(ptr_p->expanded_file_number_, &ptr->expanded_file_number_, 3);
		convert_uchar_array_to_ushort_littlend(ptr_p->extended_channel_sets_scan_type_, &ptr->extended_channel_sets_scan_type_);
		convert_uchar_array_to_ushort_littlend(ptr_p->extended_header_blocks_, &ptr->extended_header_blocks_);
		convert_uchar_array_to_ushort_littlend(ptr_p->external_header_blocks_, &ptr->external_header_blocks_);

		ptr->undefined_1_ = ptr_p->undefined_1_;

		ptr->major_segd_revision_number_ = ptr_p->major_segd_revision_number_;
		ptr->minor_segd_revision_number_ = ptr_p->minor_segd_revision_number_;

		ptr->number_of_blocks_of_general_trailer_ = ptr_p->number_of_blocks_of_general_trailer_;

		convert_uchar_array_to_uint_littlend(ptr_p->extended_record_length_, &ptr->extended_record_length_, 3);

		ptr->undefined_2_ = ptr_p->undefined_2_;
		ptr->general_header_block_number_ = ptr_p->general_header_block_number_;
		memcpy(ptr->undefined_3_, ptr_p->undefined_3_, sizeof(ptr->undefined_3_));
	}
}

void segd_file_convert_general_header_3_private(SEGD_GENERAL_HEADER_3_PTR	general_header,
												SEGD_GENERAL_HEADER_3_PRIVATE_PTR	general_header_private, 
												bool	be_reverted_private)
{
	unsigned int i = 0x0;
	unsigned short f = 0x0;
	SEGD_GENERAL_HEADER_3_PRIVATE_PTR ptr_p = general_header_private;
	SEGD_GENERAL_HEADER_3_PTR ptr = general_header;

	if(be_reverted_private){

		convert_uint_littlend_to_uchar_array(ptr->expanded_file_number_, ptr_p->expanded_file_number_, 3);

		i = static_cast<unsigned int>(ptr->source_line_number_);
		f = static_cast<unsigned short>((ptr->source_line_number_ - i) * 100);

		convert_ushort_littlend_to_uchar_array(f, ptr_p->source_line_number_ + 3);
		convert_uint_littlend_to_uchar_array(i, ptr_p->source_line_number_, 3);

		i = static_cast<unsigned int>(ptr->source_point_number_);
		f = static_cast<unsigned short>((ptr->source_point_number_ - i) * 100);
		convert_uint_littlend_to_uchar_array(i, ptr_p->source_point_number_, 3);
		convert_ushort_littlend_to_uchar_array(f, ptr_p->source_point_number_ + 3);

		ptr_p->source_point_index_ = ptr->source_point_index_;
		ptr_p->phase_control_ = ptr->phase_control_;
		ptr_p->type_vibrator_ = ptr->type_vibrator_;

		convert_ushort_littlend_to_bigend(&ptr->phase_angle_, &ptr_p->phase_angle_);

		ptr_p->general_header_block_number_ = ptr->general_header_block_number_;
		ptr_p->source_set_number_ = ptr->source_set_number_;
		memcpy(ptr_p->undefined_, ptr->undefined_, sizeof(ptr_p->undefined_));
	}
	else{

		convert_uchar_array_to_uint_littlend(ptr_p->expanded_file_number_, &ptr->expanded_file_number_, 3);

		convert_uchar_array_to_uint_littlend(ptr_p->source_line_number_, &i, 3);
		convert_uchar_array_to_ushort_littlend(ptr_p->source_line_number_ + 3, &f);
		ptr->source_line_number_ = i + static_cast<float>(f)/100;

		convert_uchar_array_to_uint_littlend(ptr_p->source_point_number_, &i, 3);
		convert_uchar_array_to_ushort_littlend(ptr_p->source_point_number_ + 3, &f);
		ptr->source_point_number_ = i + static_cast<float>(f)/100;

		ptr->source_point_index_ = ptr_p->source_point_index_;
		ptr->phase_control_ = ptr_p->phase_control_;
		ptr->type_vibrator_ = ptr_p->type_vibrator_;

		convert_ushort_bigend_to_littlend(&ptr_p->phase_angle_, &ptr->phase_angle_);

		ptr->general_header_block_number_ = ptr_p->general_header_block_number_;
		ptr->source_set_number_ = ptr_p->source_set_number_;
		memcpy(ptr->undefined_, ptr_p->undefined_, sizeof(ptr_p->undefined_));
	}

}

void segd_file_convert_scan_type_header_private(SEGD_SCAN_TYPE_HEADER_PTR	scan_type_header,
												SEGD_SCAN_TYPE_HEADER_PRIVATE_PTR	scan_type_header_private,
												bool	be_reverted_private)
{
	SEGD_SCAN_TYPE_HEADER_PRIVATE_PTR ptr_p = scan_type_header_private;
	SEGD_SCAN_TYPE_HEADER_PTR ptr = scan_type_header;

	if(be_reverted_private){

		convert_uchar_to_bcd(ptr->scan_type_number_, &ptr_p->scan_type_number_);
		convert_uchar_to_bcd(ptr->channel_set_number_, &ptr_p->channel_set_number_);

		convert_ushort_littlend_to_bigend( &(ptr->channel_set_start_time_), &(ptr_p->channel_set_start_time_));
		convert_ushort_littlend_to_bigend( &(ptr->channel_set_end_time_), &(ptr_p->channel_set_end_time_));
		convert_ushort_littlend_to_bigend( &(ptr->descale_multiplier_), &(ptr_p->descale_multiplier_));

		convert_ushort_to_bcd(ptr->channels_number_, &ptr->channels_number_);
		convert_ushort_littlend_to_bigend( &(ptr->channels_number_), &(ptr_p->channels_number_));

		ptr_p->channel_type_identification_ = ptr->channel_type_identification_;
		convert_uchar_to_bcd(ptr_p->channel_type_identification_, &ptr_p->channel_type_identification_);
		ptr_p->channel_type_identification_ <<= 4;

		ptr_p->sample_channel_channel_gain_ = ptr->sample_channel_subscans_number_;
		convert_uchar_to_bcd(ptr_p->sample_channel_channel_gain_, &ptr_p->sample_channel_channel_gain_);
		ptr_p->sample_channel_channel_gain_ <<= 4;	

		convert_uchar_to_bcd(ptr->channel_gain_control_, &ptr->channel_gain_control_);
		ptr_p->sample_channel_channel_gain_ |= (ptr->channel_gain_control_ & 0xf);

		convert_ushort_to_bcd(ptr->alias_filter_frequency_, &ptr->alias_filter_frequency_);
		convert_ushort_littlend_to_bigend( &(ptr->alias_filter_frequency_), &(ptr_p->alias_filter_frequency_));

		convert_ushort_to_bcd(ptr->alias_filter_slope_, &ptr->alias_filter_slope_);
		convert_ushort_littlend_to_bigend( &(ptr->alias_filter_slope_), &(ptr_p->alias_filter_slope_));

		convert_ushort_to_bcd(ptr->low_cut_filter_, &ptr->low_cut_filter_);
		convert_ushort_littlend_to_bigend( &(ptr->low_cut_filter_), &(ptr_p->low_cut_filter_));

		convert_ushort_to_bcd(ptr->low_cut_filter_slope_, &ptr->low_cut_filter_slope_);
		convert_ushort_littlend_to_bigend( &(ptr->low_cut_filter_slope_), &(ptr_p->low_cut_filter_slope_));

		convert_ushort_littlend_to_bigend( &(ptr->first_notch_filter_), &(ptr_p->first_notch_filter_));
		convert_ushort_littlend_to_bigend( &(ptr->second_notch_filter_), &(ptr_p->second_notch_filter_));
		convert_ushort_littlend_to_bigend( &(ptr->third_notch_filter_), &(ptr_p->third_notch_filter_));

		ptr_p->vertical_stack_ = ptr->vertical_stack_;
		ptr_p->streamer_number_ = ptr->streamer_number_;
		ptr_p->array_forming_ = ptr->array_forming_;

		convert_ushort_littlend_to_bigend( &(ptr->extended_channel_set_number_), &(ptr_p->extended_channel_set_number_));

		ptr_p->extended_header_flag_trace_header_extension_ = ptr->extended_header_flag_ & 0xf;
		ptr_p->extended_header_flag_trace_header_extension_ <<= 4;
		ptr_p->extended_header_flag_trace_header_extension_ |= (ptr->trace_header_extension_ & 0xf);
	}
	else{

		convert_bcd_to_uchar(ptr_p->scan_type_number_, &ptr->scan_type_number_);
		convert_bcd_to_uchar(ptr_p->channel_set_number_, &ptr->channel_set_number_);

		convert_ushort_bigend_to_littlend( &(ptr_p->channel_set_start_time_), &(ptr->channel_set_start_time_));
		convert_ushort_bigend_to_littlend( &(ptr_p->channel_set_end_time_), &(ptr->channel_set_end_time_));
		convert_ushort_bigend_to_littlend( &(ptr_p->descale_multiplier_), &(ptr->descale_multiplier_));

		convert_ushort_bigend_to_littlend( &(ptr_p->channels_number_), &(ptr->channels_number_));
		convert_bcd_to_ushort(ptr->channels_number_, &ptr->channels_number_);

		ptr->channel_type_identification_ = static_cast<unsigned char>(ptr_p->channel_type_identification_ >> 4);
		convert_bcd_to_uchar(ptr->channel_type_identification_, &ptr->channel_type_identification_);

		ptr->sample_channel_subscans_number_ = static_cast<unsigned char>(ptr_p->sample_channel_channel_gain_ >> 4);
		convert_bcd_to_uchar(ptr->sample_channel_subscans_number_, &ptr->sample_channel_subscans_number_);//non standard
		ptr->channel_gain_control_ = ptr_p->sample_channel_channel_gain_ & 0xf;
		convert_bcd_to_uchar(ptr->channel_gain_control_, &ptr->channel_gain_control_);

		convert_ushort_bigend_to_littlend( &(ptr_p->alias_filter_frequency_), &(ptr->alias_filter_frequency_));
		convert_bcd_to_ushort(ptr->alias_filter_frequency_, &ptr->alias_filter_frequency_);

		convert_ushort_bigend_to_littlend( &(ptr_p->alias_filter_slope_), &(ptr->alias_filter_slope_));
		convert_bcd_to_ushort(ptr->alias_filter_slope_, &ptr->alias_filter_slope_);

		convert_ushort_bigend_to_littlend( &(ptr_p->low_cut_filter_), &(ptr->low_cut_filter_));
		convert_bcd_to_ushort(ptr->low_cut_filter_, &ptr->low_cut_filter_);

		convert_ushort_bigend_to_littlend( &(ptr_p->low_cut_filter_slope_), &(ptr->low_cut_filter_slope_));
		convert_bcd_to_ushort(ptr->low_cut_filter_slope_, &ptr->low_cut_filter_slope_);

		convert_ushort_bigend_to_littlend( &(ptr_p->first_notch_filter_), &(ptr->first_notch_filter_));
		convert_ushort_bigend_to_littlend( &(ptr_p->second_notch_filter_), &(ptr->second_notch_filter_));
		convert_ushort_bigend_to_littlend( &(ptr_p->third_notch_filter_), &(ptr->third_notch_filter_));

		ptr->vertical_stack_ = ptr_p->vertical_stack_;
		ptr->streamer_number_ = ptr_p->streamer_number_;
		ptr->array_forming_ = ptr_p->array_forming_;

		convert_ushort_bigend_to_littlend( &(ptr_p->extended_channel_set_number_), &(ptr->extended_channel_set_number_));

		ptr->extended_header_flag_ = ptr_p->extended_header_flag_trace_header_extension_ >> 4;
		ptr->trace_header_extension_ = ptr_p->extended_header_flag_trace_header_extension_ & 0xf;
	}
}

void segd_file_convert_extended_header_private(SEGD_EXTENDED_HEADER_PTR	extended_header, 
											   SEGD_EXTENDED_HEADER_PRIVATE_PTR	extended_header_private,
											   bool	be_reverted_private)
{
	int i = 0x0;
	SEGD_EXTENDED_HEADER_PTR ptr = extended_header;
	SEGD_EXTENDED_HEADER_PRIVATE_PTR ptr_p = extended_header_private;

	if(be_reverted_private){
		convert_uint_littlend_to_bigend( &(ptr->sample_length_), &(ptr_p->sample_length_));
		convert_uint_littlend_to_bigend( &(ptr->sample_frequency_), &(ptr_p->sample_frequency_));
		convert_uint_littlend_to_bigend( &(ptr->trace_total_number_), &(ptr_p->trace_total_number_));
		convert_uint_littlend_to_bigend( &(ptr->auxiliary_trace_number_), &(ptr_p->auxiliary_trace_number_));
		convert_uint_littlend_to_bigend( &(ptr->trace_number_), &(ptr_p->trace_number_));
		convert_uint_littlend_to_bigend( &(ptr->stopped_trace_number_), &(ptr_p->stopped_trace_number_));
		convert_uint_littlend_to_bigend( &(ptr->active_trace_number_), &(ptr_p->active_trace_number_));
		convert_uint_littlend_to_bigend( &(ptr->source_type_), &(ptr_p->source_type_));
		convert_uint_littlend_to_bigend( &(ptr->sample_number_in_trace_), &(ptr_p->sample_number_in_trace_));
		convert_uint_littlend_to_bigend( &(ptr->shot_number_), &(ptr_p->shot_number_));

		convert_float_littlend_to_bigend( &(ptr->tb_window_), &(ptr_p->tb_window_));

		convert_uint_littlend_to_bigend( &(ptr->test_type_), &(ptr_p->test_type_));
		convert_uint_littlend_to_bigend( &(ptr->line_first_test_), &(ptr_p->line_first_test_));
		convert_uint_littlend_to_bigend( &(ptr->line_first_number_), &(ptr_p->line_first_number_));
		convert_uint_littlend_to_bigend( &(ptr->array_number_), &(ptr_p->array_number_));
		convert_uint_littlend_to_bigend( &(ptr->array_type_), &(ptr_p->array_type_));
		convert_uint_littlend_to_bigend( &(ptr->time_break_signal_), &(ptr_p->time_break_signal_));
		convert_uint_littlend_to_bigend( &(ptr->well_time_), &(ptr_p->well_time_));
		convert_uint_littlend_to_bigend( &(ptr->blaster_serial_number_), &(ptr_p->blaster_serial_number_));
		convert_uint_littlend_to_bigend( &(ptr->blaster_status_), &(ptr_p->blaster_status_));
		convert_uint_littlend_to_bigend( &(ptr->refraction_delay_), &(ptr_p->refraction_delay_));
		convert_uint_littlend_to_bigend( &(ptr->tb_to_t0_time_), &(ptr_p->tb_to_t0_time_));
		convert_uint_littlend_to_bigend( &(ptr->internal_time_break_signal_), &(ptr_p->internal_time_break_signal_));
		convert_uint_littlend_to_bigend( &(ptr->field_unit_prestack_), &(ptr_p->field_unit_prestack_));
		convert_uint_littlend_to_bigend( &(ptr->Noise_elimination_type_), &(ptr_p->Noise_elimination_type_));
		convert_uint_littlend_to_bigend( &(ptr->low_trace_percent_), &(ptr_p->low_trace_percent_));
		convert_uint_littlend_to_bigend( &(ptr->low_trace_value_), &(ptr_p->low_trace_value_));
		convert_uint_littlend_to_bigend( &(ptr->window_number_), &(ptr_p->window_number_));
		convert_uint_littlend_to_bigend( &(ptr->historical_edit_type_), &(ptr_p->historical_edit_type_));
		convert_uint_littlend_to_bigend( &(ptr->noisy_trace_percent_), &(ptr_p->noisy_trace_percent_));
		convert_uint_littlend_to_bigend( &(ptr->historical_range_), &(ptr_p->historical_range_));
		convert_uint_littlend_to_bigend( &(ptr->historical_taper_length_), &(ptr_p->historical_taper_length_));
		convert_uint_littlend_to_bigend( &(ptr->threshold_changed_), &(ptr_p->threshold_changed_));
		convert_uint_littlend_to_bigend( &(ptr->historical_threshold_initial_value_), &(ptr_p->historical_threshold_initial_value_));
		convert_uint_littlend_to_bigend( &(ptr->historical_zeroing_length_), &(ptr_p->historical_zeroing_length_));
		convert_uint_littlend_to_bigend( &(ptr->process_type_), &(ptr_p->process_type_));

		for(i = 0; i < 32; ++i) convert_uint_littlend_to_bigend( &(ptr->sample_type_table_[i]), &(ptr_p->sample_type_table_[i]));
		for(i = 0; i < 32; ++i) convert_uint_littlend_to_bigend( &(ptr->threshold_value_table_[i]), &(ptr_p->threshold_value_table_[i]));

		convert_uint_littlend_to_bigend( &(ptr->overlap_number_), &(ptr_p->overlap_number_));

		memcpy(ptr_p->not_used1_, ptr->not_used1_, 80 * sizeof(SEGD_UCHAR));

		convert_uint_littlend_to_bigend( &(ptr->record_length_), &(ptr_p->record_length_));
		convert_uint_littlend_to_bigend( &(ptr->autocorrelative_peak_time_), &(ptr_p->autocorrelative_peak_time_));
		convert_uint_littlend_to_bigend( &(ptr->not_used2_), &(ptr_p->not_used2_));
		convert_uint_littlend_to_bigend( &(ptr->related_reference_signal_number_), &(ptr_p->related_reference_signal_number_));
		convert_uint_littlend_to_bigend( &(ptr->reference_signal_length_), &(ptr_p->reference_signal_length_));
		convert_uint_littlend_to_bigend( &(ptr->scan_length_), &(ptr_p->scan_length_));
		convert_uint_littlend_to_bigend( &(ptr->sample_number_), &(ptr_p->sample_number_));
		convert_float_littlend_to_bigend( &(ptr->max_auxiliary_channel_max_value_), &(ptr_p->max_auxiliary_channel_max_value_));
		convert_float_littlend_to_bigend( &(ptr->max_channel_max_value_), &(ptr_p->max_channel_max_value_));
		convert_uint_littlend_to_bigend( &(ptr->dump_overlap_number_), &(ptr_p->dump_overlap_number_));
		memcpy(ptr_p->tape_lable_, ptr->tape_lable_, 16 * sizeof(SEGD_UCHAR));
		convert_uint_littlend_to_bigend( &(ptr->tape_number_), &(ptr_p->tape_number_));
		memcpy(ptr_p->version_, ptr->version_, 16 * sizeof(SEGD_UCHAR));
		memcpy(ptr_p->date_, ptr->date_, 12 * sizeof(SEGD_UCHAR));

		convert_double_littlend_to_uchar_array(ptr->source_easting_, ptr_p->source_easting_);
		convert_double_littlend_to_uchar_array(ptr->source_northing_, ptr_p->source_northing_);

		convert_float_littlend_to_bigend( &(ptr->Source_elevation_), &(ptr_p->Source_elevation_));
		convert_uint_littlend_to_bigend( &(ptr->slip_sweep_mode_), &(ptr_p->slip_sweep_mode_));
		convert_uint_littlend_to_bigend( &(ptr->file_number_per_tape_), &(ptr_p->file_number_per_tape_));
		convert_uint_littlend_to_bigend( &(ptr->file_counter_), &(ptr_p->file_counter_));
		memcpy(ptr_p->sample_error_explaination_, ptr->sample_error_explaination_, 160 * sizeof(SEGD_UCHAR));
		convert_uint_littlend_to_bigend( &(ptr->filter_type_), &(ptr_p->filter_type_));
		convert_uint_littlend_to_bigend( &(ptr->stack_dumped_), &(ptr_p->stack_dumped_));
		convert_uint_littlend_to_bigend( &(ptr->stack_symbol_), &(ptr_p->stack_symbol_));
		convert_uint_littlend_to_bigend( &(ptr->prm_tilt_correction_), &(ptr_p->prm_tilt_correction_));
		memcpy(ptr_p->line_name_, ptr->line_name_, 64 * sizeof(SEGD_UCHAR));
		convert_uint_littlend_to_bigend( &(ptr->operation_mode_), &(ptr_p->operation_mode_));
		convert_uint_littlend_to_bigend( &(ptr->reserved2_), &(ptr_p->reserved2_));
		convert_uint_littlend_to_bigend( &(ptr->non_log_), &(ptr_p->non_log_));
		convert_uint_littlend_to_bigend( &(ptr->listen_time_), &(ptr_p->listen_time_));
		convert_uint_littlend_to_bigend( &(ptr->dump_type_), &(ptr_p->dump_type_));
		convert_uint_littlend_to_bigend( &(ptr->reserved3_), &(ptr_p->reserved3_));
		convert_uint_littlend_to_bigend( &(ptr->line_id_), &(ptr_p->line_id_));
		memcpy(ptr_p->undefined_, ptr->undefined_, 152 * sizeof(SEGD_UCHAR));
	}
	else{

		convert_uint_bigend_to_littlend( &(ptr_p->sample_length_), &(ptr->sample_length_));
		convert_uint_bigend_to_littlend( &(ptr_p->sample_frequency_), &(ptr->sample_frequency_));
		convert_uint_bigend_to_littlend( &(ptr_p->trace_total_number_), &(ptr->trace_total_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->auxiliary_trace_number_), &(ptr->auxiliary_trace_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->trace_number_), &(ptr->trace_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->stopped_trace_number_), &(ptr->stopped_trace_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->active_trace_number_), &(ptr->active_trace_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->source_type_), &(ptr->source_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->sample_number_in_trace_), &(ptr->sample_number_in_trace_));
		convert_uint_bigend_to_littlend( &(ptr_p->shot_number_), &(ptr->shot_number_));

		convert_float_bigend_to_littlend( &(ptr_p->tb_window_), &(ptr->tb_window_));//?

		convert_uint_bigend_to_littlend( &(ptr_p->test_type_), &(ptr->test_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->line_first_test_), &(ptr->line_first_test_));
		convert_uint_bigend_to_littlend( &(ptr_p->line_first_number_), &(ptr->line_first_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->array_number_), &(ptr->array_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->array_type_), &(ptr->array_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->time_break_signal_), &(ptr->time_break_signal_));
		convert_uint_bigend_to_littlend( &(ptr_p->well_time_), &(ptr->well_time_));
		convert_uint_bigend_to_littlend( &(ptr_p->blaster_serial_number_), &(ptr->blaster_serial_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->blaster_status_), &(ptr->blaster_status_));
		convert_uint_bigend_to_littlend( &(ptr_p->refraction_delay_), &(ptr->refraction_delay_));
		convert_uint_bigend_to_littlend( &(ptr_p->tb_to_t0_time_), &(ptr->tb_to_t0_time_));
		convert_uint_bigend_to_littlend( &(ptr_p->internal_time_break_signal_), &(ptr->internal_time_break_signal_));
		convert_uint_bigend_to_littlend( &(ptr_p->field_unit_prestack_), &(ptr->field_unit_prestack_));
		convert_uint_bigend_to_littlend( &(ptr_p->Noise_elimination_type_), &(ptr->Noise_elimination_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->low_trace_percent_), &(ptr->low_trace_percent_));
		convert_uint_bigend_to_littlend( &(ptr_p->low_trace_value_), &(ptr->low_trace_value_));
		convert_uint_bigend_to_littlend( &(ptr_p->window_number_), &(ptr->window_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->historical_edit_type_), &(ptr->historical_edit_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->noisy_trace_percent_), &(ptr->noisy_trace_percent_));
		convert_uint_bigend_to_littlend( &(ptr_p->historical_range_), &(ptr->historical_range_));
		convert_uint_bigend_to_littlend( &(ptr_p->historical_taper_length_), &(ptr->historical_taper_length_));
		convert_uint_bigend_to_littlend( &(ptr_p->threshold_changed_), &(ptr->threshold_changed_));
		convert_uint_bigend_to_littlend( &(ptr_p->historical_threshold_initial_value_), &(ptr->historical_threshold_initial_value_));
		convert_uint_bigend_to_littlend( &(ptr_p->historical_zeroing_length_), &(ptr->historical_zeroing_length_));
		convert_uint_bigend_to_littlend( &(ptr_p->process_type_), &(ptr->process_type_));

		for(i = 0; i < 32; ++i) convert_uint_bigend_to_littlend( &(ptr_p->sample_type_table_[i]), &(ptr->sample_type_table_[i]));
		for(i = 0; i < 32; ++i) convert_uint_bigend_to_littlend( &(ptr_p->threshold_value_table_[i]), &(ptr->threshold_value_table_[i]));
		convert_uint_bigend_to_littlend( &(ptr_p->overlap_number_), &(ptr->overlap_number_));

		memcpy(ptr->not_used1_, ptr_p->not_used1_, 80 * sizeof(SEGD_UCHAR));
		convert_uint_bigend_to_littlend( &(ptr_p->record_length_), &(ptr->record_length_));
		convert_uint_bigend_to_littlend( &(ptr_p->autocorrelative_peak_time_), &(ptr->autocorrelative_peak_time_));
		convert_uint_bigend_to_littlend( &(ptr_p->not_used2_), &(ptr->not_used2_));
		convert_uint_bigend_to_littlend( &(ptr_p->related_reference_signal_number_), &(ptr->related_reference_signal_number_));
		convert_uint_bigend_to_littlend( &(ptr_p->reference_signal_length_), &(ptr->reference_signal_length_));
		convert_uint_bigend_to_littlend( &(ptr_p->scan_length_), &(ptr->scan_length_));
		convert_uint_bigend_to_littlend( &(ptr_p->sample_number_), &(ptr->sample_number_));
		convert_float_bigend_to_littlend( &(ptr_p->max_auxiliary_channel_max_value_), &(ptr->max_auxiliary_channel_max_value_));
		convert_float_bigend_to_littlend( &(ptr_p->max_channel_max_value_), &(ptr->max_channel_max_value_));
		convert_uint_bigend_to_littlend( &(ptr_p->dump_overlap_number_), &(ptr->dump_overlap_number_));
		memcpy(ptr->tape_lable_, ptr_p->tape_lable_, 16 * sizeof(SEGD_UCHAR));
		convert_uint_bigend_to_littlend( &(ptr_p->tape_number_), &(ptr->tape_number_));
		memcpy(ptr->version_, ptr_p->version_, 16 * sizeof(SEGD_UCHAR));
		memcpy(ptr->date_, ptr_p->date_, 12 * sizeof(SEGD_UCHAR));

		convert_uchar_array_to_double_littlend(ptr_p->source_easting_, &ptr->source_easting_);
		convert_uchar_array_to_double_littlend(ptr_p->source_northing_, &ptr->source_northing_);

		convert_float_bigend_to_littlend( &(ptr_p->Source_elevation_), &(ptr->Source_elevation_));
		convert_uint_bigend_to_littlend( &(ptr_p->slip_sweep_mode_), &(ptr->slip_sweep_mode_));
		convert_uint_bigend_to_littlend( &(ptr_p->file_number_per_tape_), &(ptr->file_number_per_tape_));
		convert_uint_bigend_to_littlend( &(ptr_p->file_counter_), &(ptr->file_counter_));
		memcpy(ptr->sample_error_explaination_, ptr_p->sample_error_explaination_, 160 * sizeof(SEGD_UCHAR));
		convert_uint_bigend_to_littlend( &(ptr_p->filter_type_), &(ptr->filter_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->stack_dumped_), &(ptr->stack_dumped_));
		convert_uint_bigend_to_littlend( &(ptr_p->stack_symbol_), &(ptr->stack_symbol_));
		convert_uint_bigend_to_littlend( &(ptr_p->prm_tilt_correction_), &(ptr->prm_tilt_correction_));
		memcpy(ptr->line_name_, ptr_p->line_name_, 64 * sizeof(SEGD_UCHAR));
		convert_uint_bigend_to_littlend( &(ptr_p->operation_mode_), &(ptr->operation_mode_));
		convert_uint_bigend_to_littlend( &(ptr_p->reserved2_), &(ptr->reserved2_));
		convert_uint_bigend_to_littlend( &(ptr_p->non_log_), &(ptr->non_log_));
		convert_uint_bigend_to_littlend( &(ptr_p->listen_time_), &(ptr->listen_time_));
		convert_uint_bigend_to_littlend( &(ptr_p->dump_type_), &(ptr->dump_type_));
		convert_uint_bigend_to_littlend( &(ptr_p->reserved3_), &(ptr->reserved3_));
		convert_uint_bigend_to_littlend( &(ptr_p->line_id_), &(ptr->line_id_));
		memcpy(ptr->undefined_, ptr_p->undefined_, 152 * sizeof(SEGD_UCHAR));
	}

}

void segd_file_convert_external_header_private(SEGD_EXTERNAL_HEADER_PTR	external_header,
											   SEGD_EXTERNAL_HEADER_PRIVATE_PTR	external_header_private,
											   bool	be_reverted_private)
{
	SEGD_EXTERNAL_HEADER_PTR ptr = external_header;
	SEGD_EXTERNAL_HEADER_PRIVATE_PTR ptr_p = external_header_private;

	if(be_reverted_private){
		memcpy_s(ptr_p->series_, sizeof(ptr_p->series_), ptr->series_, sizeof(ptr->series_));
	}
	else{
		memcpy_s(ptr->series_, sizeof(ptr->series_), ptr_p->series_, sizeof(ptr_p->series_));
	}	
}

void segd_file_convert_demux_trace_header_private(SEGD_DEMUX_TRACE_HEADER_PTR			demux_trace_header,
												  SEGD_DEMUX_TRACE_HEADER_PRIVATE_PTR	demux_trace_header_private,
												  bool	be_reverted_private)
{
	SEGD_DEMUX_TRACE_HEADER_PTR ptr = demux_trace_header;
	SEGD_DEMUX_TRACE_HEADER_PRIVATE_PTR ptr_p = demux_trace_header_private;

	if(be_reverted_private){

		convert_ushort_to_bcd(ptr->file_number_, &ptr->file_number_);
		convert_ushort_littlend_to_bigend(&ptr->file_number_, &ptr_p->file_number_);

		convert_uchar_to_bcd(ptr->scan_type_number_, &ptr_p->scan_type_number_);
		convert_uchar_to_bcd(ptr->channel_set_number_, &ptr_p->channel_set_number_);

		convert_ushort_to_bcd(ptr->trace_number_, &ptr->trace_number_);
		convert_ushort_littlend_to_bigend(&ptr->trace_number_, &ptr_p->trace_number_);

		convert_uint_littlend_to_uchar_array(ptr->first_timing_word_, ptr_p->first_timing_word_, 3);

		ptr_p->trace_header_extension_ = ptr->trace_header_extension_;
		ptr_p->sample_skew_ = ptr->sample_skew_;
		ptr_p->trace_edit_ = ptr->trace_edit_;

		convert_float_littlend_to_uchar_array(ptr->time_break_window_, ptr_p->time_break_window_, 3);	
		convert_ushort_littlend_to_uchar_array(ptr->extended_channel_set_number_, ptr_p->extended_channel_set_number_);	
		convert_uint_littlend_to_uchar_array(ptr->extended_file_number_, ptr_p->extended_file_number_, 3);
	}
	else{

		convert_ushort_bigend_to_littlend(&ptr_p->file_number_, &ptr->file_number_);
		convert_bcd_to_ushort(ptr->file_number_, &ptr->file_number_);

		convert_bcd_to_uchar(ptr_p->scan_type_number_, &ptr->scan_type_number_);
		convert_bcd_to_uchar(ptr_p->channel_set_number_, &ptr->channel_set_number_);

		convert_ushort_bigend_to_littlend(&ptr_p->trace_number_, &ptr->trace_number_);
		convert_bcd_to_ushort(ptr->trace_number_, &ptr->trace_number_);

		convert_uchar_array_to_uint_littlend(ptr_p->first_timing_word_, &ptr->first_timing_word_, 3);

		ptr->trace_header_extension_ = ptr_p->trace_header_extension_;
		ptr->sample_skew_ = ptr_p->sample_skew_;
		ptr->trace_edit_ = ptr_p->trace_edit_;

		convert_uchar_array_to_float_littlend(ptr_p->time_break_window_, &ptr->time_break_window_, 3);	
		convert_uchar_array_to_ushort_littlend(ptr_p->extended_channel_set_number_, &ptr->extended_channel_set_number_);	
		convert_uchar_array_to_uint_littlend(ptr_p->extended_file_number_, &ptr->extended_file_number_, 3);
	}
}

void segd_file_convert_trace_header_extension_1_private(SEGD_TRACE_HEADER_EXTENSION_1_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_1_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_1_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_1_PTR ptr = trace_header_extended;

	if(be_reverted_private){

		convert_uint_littlend_to_uchar_array(ptr->receiver_line_number_, ptr_p->receiver_line_number_, 3);
		convert_uint_littlend_to_uchar_array(ptr->receiver_point_number_, ptr_p->receiver_point_number_, 3);
		ptr_p->receiver_point_index_ = ptr->receiver_point_index_;
		convert_uint_littlend_to_uchar_array(ptr->number_of_samples_per_trace_, ptr_p->number_of_samples_per_trace_, 3);
		convert_ulonglong_littlend_to_uchar_array(ptr->extended_receiver_line_number_, ptr_p->extended_receiver_line_number_, 5);
		convert_ulonglong_littlend_to_uchar_array(ptr->extended_receiver_point_number_, ptr_p->extended_receiver_point_number_, 5);
		ptr_p->sensor_type_ = ptr->sensor_type_;
		memcpy(ptr->undefined_, ptr_p->undefined_, 11 * sizeof(SEGD_UCHAR));
	}
	else{

		convert_uchar_array_to_uint_littlend(ptr_p->receiver_line_number_, &ptr->receiver_line_number_, 3);
		convert_uchar_array_to_uint_littlend(ptr_p->receiver_point_number_, &ptr->receiver_point_number_, 3);
		ptr->receiver_point_index_ = ptr_p->receiver_point_index_;
		convert_uchar_array_to_uint_littlend(ptr_p->number_of_samples_per_trace_, &ptr->number_of_samples_per_trace_, 3);
		convert_uchar_array_to_ulonglong_littlend(ptr_p->extended_receiver_line_number_, &ptr->extended_receiver_line_number_, 5);
		convert_uchar_array_to_ulonglong_littlend(ptr_p->extended_receiver_point_number_, &ptr->extended_receiver_point_number_, 5);
		ptr->sensor_type_ = ptr_p->sensor_type_;
		memcpy(ptr->undefined_, ptr_p->undefined_, 11 * sizeof(SEGD_UCHAR));
	}
}

void segd_file_convert_trace_header_extension_2_private(SEGD_TRACE_HEADER_EXTENSION_2_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_2_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_2_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_2_PTR ptr = trace_header_extended;

	if(be_reverted_private){
		convert_double_littlend_to_uchar_array(ptr->receiver_point_east_, ptr_p->receiver_point_east_);
		convert_double_littlend_to_uchar_array(ptr->receiver_point_north_,ptr_p->receiver_point_north_);

		convert_float_littlend_to_bigend( &(ptr->receiver_point_high_), &(ptr_p->receiver_point_high_));
		ptr_p->detector_type_number_ = ptr->detector_type_number_;
		memcpy(ptr_p->undefined_, ptr->undefined_, 7 * sizeof(SEGD_UCHAR));
		convert_uint_littlend_to_bigend( &(ptr->extended_trace_number_), &(ptr_p->extended_trace_number_));
	}
	else{

		convert_uchar_array_to_double_littlend(ptr_p->receiver_point_east_, &ptr->receiver_point_east_);
		convert_uchar_array_to_double_littlend(ptr_p->receiver_point_north_, &ptr->receiver_point_north_);
		convert_float_bigend_to_littlend( &(ptr_p->receiver_point_high_), &(ptr->receiver_point_high_));
		ptr->detector_type_number_ = ptr_p->detector_type_number_;
		memcpy(ptr->undefined_, ptr_p->undefined_, 7 * sizeof(SEGD_UCHAR));
		convert_uint_bigend_to_littlend( &(ptr_p->extended_trace_number_), &(ptr->extended_trace_number_));
	}
}

void segd_file_convert_trace_header_extension_3_private(SEGD_TRACE_HEADER_EXTENSION_3_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_3_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_3_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_3_PTR ptr = trace_header_extended;

	if(be_reverted_private){
		convert_float_littlend_to_bigend( &(ptr->resistor_lower_limit_), &(ptr_p->resistor_lower_limit_));
		convert_float_littlend_to_bigend( &(ptr->resistor_higher_limit_), &(ptr_p->resistor_higher_limit_));
		convert_float_littlend_to_bigend( &(ptr->resistor_value_), &(ptr_p->resistor_value_));
		convert_float_littlend_to_bigend( &(ptr->tilt_limit_), &(ptr_p->tilt_limit_));
		convert_float_littlend_to_bigend( &(ptr->tilt_value_), &(ptr_p->tilt_value_));
		ptr_p->resistor_deviation_ = ptr->resistor_deviation_;
		ptr_p->tilt_deviation_ = ptr->tilt_deviation_;
		memcpy(ptr_p->undefined_, ptr->undefined_, 10 * sizeof(SEGD_UCHAR));
	}
	else{

		convert_float_bigend_to_littlend( &(ptr_p->resistor_lower_limit_), &(ptr->resistor_lower_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->resistor_higher_limit_), &(ptr->resistor_higher_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->resistor_value_), &(ptr->resistor_value_));
		convert_float_bigend_to_littlend( &(ptr_p->tilt_limit_), &(ptr->tilt_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->tilt_value_), &(ptr->tilt_value_));
		ptr->resistor_deviation_ = ptr_p->resistor_deviation_;
		ptr->tilt_deviation_ = ptr_p->tilt_deviation_;
		memcpy(ptr->undefined_, ptr_p->undefined_, 10 * sizeof(SEGD_UCHAR));
	}
}

void segd_file_convert_trace_header_extension_4_private(SEGD_TRACE_HEADER_EXTENSION_4_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_4_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_4_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_4_PTR ptr = trace_header_extended;

	if(be_reverted_private){
		convert_float_littlend_to_bigend( &(ptr->capacitance_lower_limit_), &(ptr_p->capacitance_lower_limit_));
		convert_float_littlend_to_bigend( &(ptr->capacitance_higher_limit_), &(ptr_p->capacitance_higher_limit_));
		convert_float_littlend_to_bigend( &(ptr->capacitance_value_), &(ptr_p->capacitance_value_));
		convert_float_littlend_to_bigend( &(ptr->cutoff_lower_limit_), &(ptr_p->cutoff_lower_limit_));
		convert_float_littlend_to_bigend( &(ptr->cutoff_higher_limit_), &(ptr_p->cutoff_higher_limit_));
		convert_float_littlend_to_bigend( &(ptr->cutoff_value_), &(ptr_p->cutoff_value_));
		ptr_p->capacitance_deviation_ = ptr->capacitance_deviation_;
		ptr_p->cutoff_deviation_ = ptr->cutoff_deviation_;
		memcpy(ptr_p->undefined_, ptr->undefined_, 6 * sizeof(SEGD_UCHAR));
	}
	else{

		convert_float_bigend_to_littlend( &(ptr_p->capacitance_lower_limit_), &(ptr->capacitance_lower_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->capacitance_higher_limit_), &(ptr->capacitance_higher_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->capacitance_value_), &(ptr->capacitance_value_));
		convert_float_bigend_to_littlend( &(ptr_p->cutoff_lower_limit_), &(ptr->cutoff_lower_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->cutoff_higher_limit_), &(ptr->cutoff_higher_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->cutoff_value_), &(ptr->cutoff_value_));
		ptr->capacitance_deviation_ = ptr_p->capacitance_deviation_;
		ptr->cutoff_deviation_ = ptr_p->cutoff_deviation_;
		memcpy(ptr->undefined_, ptr_p->undefined_, 6 * sizeof(SEGD_UCHAR));
	}
}

void segd_file_convert_trace_header_extension_5_private(SEGD_TRACE_HEADER_EXTENSION_5_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_5_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_5_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_5_PTR ptr = trace_header_extended;

	if(be_reverted_private){
		convert_float_littlend_to_bigend( &(ptr->leakage_limit_), &(ptr_p->leakage_limit_));
		convert_float_littlend_to_bigend( &(ptr->leakage_value_), &(ptr_p->leakage_value_));
		memcpy(ptr_p->undefined_1_, ptr->undefined_1_, 16 * sizeof(SEGD_UCHAR));
		ptr_p->leakage_deviation_ = ptr->leakage_deviation_;
		memcpy(ptr_p->undefined_2_, ptr->undefined_2_, 7 * sizeof(SEGD_UCHAR));
	}
	else{

		convert_float_bigend_to_littlend( &(ptr_p->leakage_limit_), &(ptr->leakage_limit_));
		convert_float_bigend_to_littlend( &(ptr_p->leakage_value_), &(ptr->leakage_value_));
		memcpy(ptr->undefined_1_, ptr_p->undefined_1_, 16 * sizeof(SEGD_UCHAR));
		ptr->leakage_deviation_ = ptr_p->leakage_deviation_;
		memcpy(ptr->undefined_2_, ptr_p->undefined_2_, 7 * sizeof(SEGD_UCHAR));
	}
}

void segd_file_convert_trace_header_extension_6_private(SEGD_TRACE_HEADER_EXTENSION_6_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_6_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_6_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_6_PTR ptr = trace_header_extended;

	if(be_reverted_private){

		ptr_p->device_type_ = ptr->device_type_;
		convert_uint_littlend_to_uchar_array(ptr->device_serial_number_, ptr_p->device_serial_number_, 3);
		ptr_p->seism_trace_number_ = ptr->seism_trace_number_;
		memcpy(ptr_p->reserved_1_, ptr->reserved_1_, 3 * sizeof(SEGD_UCHAR));
		ptr_p->component_type_ = ptr->component_type_;
		convert_uint_littlend_to_uchar_array(ptr->FDU_DSU_component_serial_number_, ptr_p->FDU_DSU_component_serial_number_, 3);
		ptr_p->FDU_DSU_component_position_ = ptr->FDU_DSU_component_position_;
		memcpy(ptr_p->reserved_2_, ptr->reserved_2_, 3 * sizeof(SEGD_UCHAR));
		ptr_p->FDU_DSU_device_type_ = ptr->FDU_DSU_device_type_;
		ptr_p->seism_trace_type_ = ptr->seism_trace_type_;
		convert_ushort_littlend_to_bigend( &(ptr->reserved_3_), &(ptr_p->reserved_3_));
		convert_float_littlend_to_bigend( &(ptr->detector_Sensitivity_), &(ptr_p->detector_Sensitivity_));
		memcpy(ptr_p->undefined_, ptr->undefined_, 8 * sizeof(SEGD_UCHAR));
	}
	else{

		ptr->device_type_ = ptr_p->device_type_;
		convert_uchar_array_to_uint_littlend(ptr_p->device_serial_number_, &ptr->device_serial_number_, 3);
		ptr->seism_trace_number_ = ptr_p->seism_trace_number_;
		memcpy(ptr->reserved_1_, ptr_p->reserved_1_, 3 * sizeof(SEGD_UCHAR));
		ptr->component_type_ = ptr_p->component_type_;
		convert_uchar_array_to_uint_littlend(ptr_p->FDU_DSU_component_serial_number_, &ptr->FDU_DSU_component_serial_number_, 3);
		ptr->FDU_DSU_component_position_ = ptr_p->FDU_DSU_component_position_;
		memcpy(ptr->reserved_2_, ptr_p->reserved_2_, 3 * sizeof(SEGD_UCHAR));
		ptr->FDU_DSU_device_type_ = ptr_p->FDU_DSU_device_type_;
		ptr->seism_trace_type_ = ptr_p->seism_trace_type_;
		convert_ushort_bigend_to_littlend( &(ptr_p->reserved_3_), &(ptr->reserved_3_));
		convert_float_bigend_to_littlend( &(ptr_p->detector_Sensitivity_), &(ptr->detector_Sensitivity_));
		memcpy(ptr->undefined_, ptr_p->undefined_, 8 * sizeof(SEGD_UCHAR));
	}
}

void segd_file_convert_trace_header_extension_7_private(SEGD_TRACE_HEADER_EXTENSION_7_PTR	trace_header_extended,
														SEGD_TRACE_HEADER_EXTENSION_7_PRIVATE_PTR	trace_header_extended_private,
														bool	be_reverted_private)
{
	SEGD_TRACE_HEADER_EXTENSION_7_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_7_PTR ptr = trace_header_extended;

	if(be_reverted_private){

		ptr_p->control_unit_type_ = ptr->control_unit_type_;
		convert_uint_littlend_to_uchar_array(ptr->control_unit_serial_number_, ptr_p->control_unit_serial_number_, 3);
		ptr_p->channel_gain_scale_ = ptr->channel_gain_scale_;
		ptr_p->channel_filter_ = ptr->channel_filter_;
		ptr_p->channel_data_deviation_ = ptr->channel_data_deviation_;
		ptr_p->channel_edit_status_ = ptr->channel_edit_status_;

		convert_float_littlend_to_bigend( &(ptr->channel_sample_conversion_factor_), &(ptr_p->channel_sample_conversion_factor_));

		ptr_p->number_of_noisy_stack_ = ptr->number_of_noisy_stack_;
		ptr_p->number_of_low_stacks_ = ptr->number_of_low_stacks_;
		ptr_p->channel_type_serial_number_ = ptr->channel_type_serial_number_;
		ptr_p->channel_process_type_ = ptr->channel_process_type_;

		convert_float_littlend_to_bigend( &(ptr->trace_max_value_), &(ptr_p->trace_max_value_));
		convert_uint_littlend_to_bigend( &(ptr->trace_max_time_), &(ptr_p->trace_max_time_));
		convert_uint_littlend_to_bigend( &(ptr->interval_polations_number_), &(ptr_p->interval_polations_number_));
		memcpy(ptr_p->undefined_, ptr->undefined_, 4 * sizeof(SEGD_UCHAR));
	}
	else{

		ptr->control_unit_type_ = ptr_p->control_unit_type_;
		convert_uchar_array_to_uint_littlend(ptr_p->control_unit_serial_number_, &ptr->control_unit_serial_number_, 3);
		ptr->channel_gain_scale_ = ptr_p->channel_gain_scale_;
		ptr->channel_filter_ = ptr_p->channel_filter_;
		ptr->channel_data_deviation_ = ptr_p->channel_data_deviation_;
		ptr->channel_edit_status_ = ptr_p->channel_edit_status_;

		convert_float_bigend_to_littlend( &(ptr_p->channel_sample_conversion_factor_), &(ptr->channel_sample_conversion_factor_));

		ptr->number_of_noisy_stack_ = ptr_p->number_of_noisy_stack_;
		ptr->number_of_low_stacks_ = ptr_p->number_of_low_stacks_;
		ptr->channel_type_serial_number_ = ptr_p->channel_type_serial_number_;
		ptr->channel_process_type_ = ptr_p->channel_process_type_;

		convert_float_bigend_to_littlend( &(ptr_p->trace_max_value_), &(ptr->trace_max_value_));
		convert_uint_bigend_to_littlend( &(ptr_p->trace_max_time_), &(ptr->trace_max_time_));
		convert_uint_bigend_to_littlend( &(ptr_p->interval_polations_number_), &(ptr->interval_polations_number_));
		memcpy(ptr->undefined_, ptr_p->undefined_, 4 * sizeof(SEGD_UCHAR));
	}
}

bool segd_file_read_genernal_header_1(__in__ SEGD_FILE_PTR file_handle, __in_out__ SEGD_GENERAL_HEADER_1_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_GENERAL_HEADER_1_PRIVATE h_p;
//	LARGE_INTEGER distance_move = {0x0};

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	file_handle->file_pointer_.QuadPart = 0x0;
	segd_file_seek_begin_private(file_handle->file_handle_, &file_handle->file_pointer_.QuadPart);
	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_GENERAL_HEADER_1_PRIVATE));
	if(header_size != sizeof(SEGD_GENERAL_HEADER_1_PRIVATE)) return is_ok = false;

//	distance_move.QuadPart = header_size;
//	segd_file_seek_private(file_handle->file_handle_, distance_move.QuadPart, SEGD_FILE_BEGIN, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_general_header_1_private(header, &h_p);

	return is_ok;
}

bool segd_file_write_genernal_header_1(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_GENERAL_HEADER_1_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_GENERAL_HEADER_1_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	file_handle->file_pointer_.QuadPart = 0x0;
	segd_file_convert_general_header_1_private(header, &h_p, true);
	segd_file_seek_begin_private(file_handle->file_handle_, &file_handle->file_pointer_.QuadPart);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_GENERAL_HEADER_1_PRIVATE));
	if(header_size != sizeof(SEGD_GENERAL_HEADER_1_PRIVATE)) return is_ok = false;
//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_BEGIN, &file_handle->file_pointer_.QuadPart);	

	return is_ok;
}

bool segd_file_read_genernal_header_2(__in__ SEGD_FILE_PTR file_handle, __in_out__ SEGD_GENERAL_HEADER_2_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_GENERAL_HEADER_2_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_GENERAL_HEADER_2_PRIVATE));
	if(header_size != sizeof(SEGD_GENERAL_HEADER_2_PRIVATE)) return is_ok = false; 
	segd_file_convert_general_header_2_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_genernal_header_2(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_GENERAL_HEADER_2_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_GENERAL_HEADER_2_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_general_header_2_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_GENERAL_HEADER_2_PRIVATE));
	if(header_size != sizeof(SEGD_GENERAL_HEADER_2_PRIVATE)) return is_ok = false;
//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);	
	return is_ok;
}

bool segd_file_read_genernal_header_3(__in__ SEGD_FILE_PTR file_handle, __in_out__ SEGD_GENERAL_HEADER_3_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_GENERAL_HEADER_3_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_GENERAL_HEADER_3_PRIVATE));
	if(header_size != sizeof(SEGD_GENERAL_HEADER_3_PRIVATE)) return is_ok = false; 

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_general_header_3_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_genernal_header_3(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_GENERAL_HEADER_3_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_GENERAL_HEADER_3_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_general_header_3_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_GENERAL_HEADER_3_PRIVATE));
	if(header_size != sizeof(SEGD_GENERAL_HEADER_3_PRIVATE)) return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);	

	return is_ok;
}

bool segd_file_read_scan_type_header(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_SCAN_TYPE_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_SCAN_TYPE_HEADER_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_SCAN_TYPE_HEADER_PRIVATE));
	if(header_size != sizeof(SEGD_SCAN_TYPE_HEADER_PRIVATE))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_scan_type_header_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_scan_type_header(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_SCAN_TYPE_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_SCAN_TYPE_HEADER_PRIVATE h_p;
	SEGD_SCAN_TYPE_HEADER header_private;

	__matrix_zero_Memory(&h_p, sizeof(h_p));
	memcpy(&header_private, header, sizeof(header_private));

	segd_file_convert_scan_type_header_private(&header_private, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_SCAN_TYPE_HEADER_PRIVATE));
	if(header_size != sizeof(SEGD_SCAN_TYPE_HEADER_PRIVATE)) return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);	
	return is_ok;
}

bool segd_file_read_extended_header(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_EXTENDED_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_EXTENDED_HEADER_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_EXTENDED_HEADER_PRIVATE));
	if(header_size != sizeof(SEGD_EXTENDED_HEADER_PRIVATE))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_extended_header_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_extended_header(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_EXTENDED_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_EXTENDED_HEADER_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_extended_header_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(SEGD_EXTENDED_HEADER_PRIVATE));
	if(header_size != sizeof(SEGD_EXTENDED_HEADER_PRIVATE))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_external_header(__in__ SEGD_FILE_PTR file_handle, __in__ int external_header_size, __out__ SEGD_EXTERNAL_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;

	header_size = segd_file_read_private(file_handle, header->series_, external_header_size);
	if(header_size != external_header_size)	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_write_external_header(__in__ SEGD_FILE_PTR file_handle, __in__ int external_header_size, __in__ SEGD_EXTERNAL_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;

	header_size = segd_file_write_private(file_handle, header->series_,	external_header_size);
	if(header_size != external_header_size)	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_FILE_TRACE_HEADER_PTR header)
{
	bool is_ok = true;
	is_ok = segd_file_read_demux_trace_header_private(file_handle, &header->demux_trace_header_);
	if(!is_ok) return is_ok;

	for(int i = 1; i <= header->demux_trace_header_.trace_header_extension_; ++i){

		switch(i){
			case 1:
				is_ok = segd_file_read_trace_header_EXTENDED_1_private(file_handle, &header->trace_header_extended_1_);
				if(!is_ok) return is_ok;
				break;
			case 2:
				is_ok = segd_file_read_trace_header_EXTENDED_2_private(file_handle, &header->trace_header_extended_2_);
				if(!is_ok) return is_ok;
				break;
			case 3:
				is_ok = segd_file_read_trace_header_EXTENDED_3_private(file_handle, &header->trace_header_extended_3_);
				if(!is_ok) return is_ok;
				break;
			case 4:
				is_ok = segd_file_read_trace_header_EXTENDED_4_private(file_handle, &header->trace_header_extended_4_);
				if(!is_ok) return is_ok;
				break;
			case 5:
				is_ok = segd_file_read_trace_header_EXTENDED_5_private(file_handle, &header->trace_header_extended_5_);
				if(!is_ok) return is_ok;
				break;
			case 6:
				is_ok = segd_file_read_trace_header_EXTENDED_6_private(file_handle, &header->trace_header_extended_6_);
				if(!is_ok) return is_ok;
				break;
			case 7:
				is_ok = segd_file_read_trace_header_EXTENDED_7_private(file_handle, &header->trace_header_extended_7_);
				if(!is_ok) return is_ok;
				break;
		}
	}

	return is_ok;
}

bool segd_file_write_trace_header(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_FILE_TRACE_HEADER_PTR header)
{
	bool is_ok = true;
	is_ok = segd_file_write_demux_trace_header_private(file_handle, &header->demux_trace_header_);
	if(!is_ok) return is_ok;

	for(int i = 1; i <= header->demux_trace_header_.trace_header_extension_; ++i){

		switch(i){
			case 1:
				is_ok = segd_file_write_trace_header_EXTENDED_1_private(file_handle, &header->trace_header_extended_1_);
				if(!is_ok) return is_ok;
				break;
			case 2:
				is_ok = segd_file_write_trace_header_EXTENDED_2_private(file_handle, &header->trace_header_extended_2_);
				if(!is_ok) return is_ok;
				break;
			case 3:
				is_ok = segd_file_write_trace_header_EXTENDED_3_private(file_handle, &header->trace_header_extended_3_);
				if(!is_ok) return is_ok;
				break;
			case 4:
				is_ok = segd_file_write_trace_header_EXTENDED_4_private(file_handle, &header->trace_header_extended_4_);
				if(!is_ok) return is_ok;
				break;
			case 5:
				is_ok = segd_file_write_trace_header_EXTENDED_5_private(file_handle, &header->trace_header_extended_5_);
				if(!is_ok) return is_ok;
				break;
			case 6:
				is_ok = segd_file_write_trace_header_EXTENDED_6_private(file_handle, &header->trace_header_extended_6_);
				if(!is_ok) return is_ok;
				break;
			case 7:
				is_ok = segd_file_write_trace_header_EXTENDED_7_private(file_handle, &header->trace_header_extended_7_);
				if(!is_ok) return is_ok;
				break;
		}
	}

	return is_ok;
}

bool segd_file_read_demux_trace_header_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_DEMUX_TRACE_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_DEMUX_TRACE_HEADER_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_demux_trace_header_private(header, &h_p);
	return is_ok;

}

bool segd_file_write_demux_trace_header_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_DEMUX_TRACE_HEADER_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_DEMUX_TRACE_HEADER_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_demux_trace_header_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_1_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_1_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_1_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_1_private(header, &h_p);
	return is_ok;

}

bool segd_file_write_trace_header_EXTENDED_1_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_1_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_1_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_1_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p)) return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_2_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_2_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_2_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_2_private(header, &h_p);
	return is_ok;

}

bool segd_file_write_trace_header_EXTENDED_2_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_2_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_2_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_2_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_3_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_3_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_3_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_3_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_trace_header_EXTENDED_3_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_3_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_3_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_3_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_4_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_4_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_4_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_4_private(header, &h_p);
	return is_ok;

}

bool segd_file_write_trace_header_EXTENDED_4_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_4_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_4_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_4_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_5_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_5_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_5_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_5_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_trace_header_EXTENDED_5_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_5_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_5_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_5_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_6_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_6_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_6_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_6_private(header, &h_p);
	return is_ok;

}

bool segd_file_write_trace_header_EXTENDED_6_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_6_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_6_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_6_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_header_EXTENDED_7_private(__in__ SEGD_FILE_PTR file_handle, __out__ SEGD_TRACE_HEADER_EXTENSION_7_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_7_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	header_size = segd_file_read_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	segd_file_convert_trace_header_extension_7_private(header, &h_p);
	return is_ok;
}

bool segd_file_write_trace_header_EXTENDED_7_private(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_TRACE_HEADER_EXTENSION_7_PTR header)
{
	bool is_ok = true;
	int header_size = 0;
	SEGD_TRACE_HEADER_EXTENSION_7_PRIVATE h_p;

	__matrix_zero_Memory(&h_p, sizeof(h_p));

	segd_file_convert_trace_header_extension_7_private(header, &h_p, true);
	header_size = segd_file_write_private(file_handle, (SEGD_UCHAR*)&h_p, sizeof(h_p));
	if(header_size != sizeof(h_p))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, header_size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_read_trace_data(__in__ SEGD_FILE_PTR file_handle, __in_out__ SEGD_UCHAR* data, __in__ SEGD_ULONG data_size)
{
	bool is_ok = true;
	int size = 0;

	__matrix_zero_Memory(data, data_size);

	size = segd_file_read_private(file_handle, data, data_size);
	if(size != static_cast<int>(data_size))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}

bool segd_file_write_trace_data(__in__ SEGD_FILE_PTR file_handle, __in__ SEGD_UCHAR* data, __in__ SEGD_ULONG data_size)
{
	bool is_ok = true;
	int size = 0;

	size = segd_file_write_private(file_handle, data, data_size);
	if(size != static_cast<int>(data_size))	return is_ok = false;

//	segd_file_seek_private(file_handle->file_handle_, size, SEGD_FILE_CURRENT, &file_handle->file_pointer_.QuadPart);
	return is_ok;
}
