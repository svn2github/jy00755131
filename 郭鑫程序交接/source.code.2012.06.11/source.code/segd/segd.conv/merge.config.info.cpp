/**
@brief	sps文件,默认配置文件信息写入segd中
@details	该文件主要包括从sps文件,默认配置文件中抽取选型合并后写入到segd配置头中
@file	merge.config.info.cpp
@note	该文件将使用wchar_t和char混合的字符集方式
@see	proc.conf.file.h, proc.sps.file.h, proc.data.file.h
@version	1.1.13.45.2011.09.29
@author	kuoxin	
@bug
		1.1.13.45.2011.09.29	初始化版本
*/

#include "segd.conv.h"
#include "proc.conf.file.h"
#include "proc.sps.file.h"
#include "..\segd.viewer\segd.file.format.h"
#include "merge.config.info.h"


int merge_segd_header_1(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
						__in_out__ SEGD_GENERAL_HEADER_1_PTR header_ptr)

{
	int is_ok = APP_SUCCESS;
	if(sps_data_ptr->base_scan_interval_ != 0) header_ptr->base_scan_interval_ = sps_data_ptr->base_scan_interval_;
	return is_ok;
}

int merge_segd_header_2(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
						__in_out__ SEGD_GENERAL_HEADER_2_PTR header_ptr)
{
	int is_ok = APP_SUCCESS;
	if(sps_data_ptr->extended_record_length_ != 0) header_ptr->extended_record_length_ = sps_data_ptr->extended_record_length_;
	return is_ok;
}

int merge_segd_header_3(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
						__in_out__ SEGD_GENERAL_HEADER_3_PTR header_ptr)
{
	int is_ok = APP_SUCCESS;
	header_ptr->source_line_number_ = sps_data_ptr->source_line_number_;
	header_ptr->source_point_number_ = sps_data_ptr->source_point_number_;
	header_ptr->source_point_index_ = sps_data_ptr->shot_point_.index_;
	return is_ok;
}

int merge_segd_scan_type_header(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
								__in_out__ SEGD_SCAN_TYPE_HEADER_PTR header_ptr)
{
	int i = 0, size = 0x0;
	int is_ok = APP_SUCCESS;
	SPS_RELATION_RECORD_SPEC_PTR relation_record_ptr = NULL;
	header_ptr->alias_filter_frequency_ = sps_data_ptr->alias_filter_frequency_;
	header_ptr->alias_filter_slope_ = sps_data_ptr->alias_filter_slope_;
	if(header_ptr->channels_number_ == 0){
		
		relation_record_ptr = sps_data_ptr->head_relation_records_ptr_;
		size = sps_data_ptr->relation_records_size_ + 1;
		for(i = 1; i < size; ++i){
			if(i == header_ptr->channel_set_number_)	break;
			relation_record_ptr	= relation_record_ptr->next_;
		}

		if(relation_record_ptr != NULL)
			header_ptr->channels_number_ = relation_record_ptr->to_channel_ - relation_record_ptr->from_channel_ + 1;
	}

	header_ptr->low_cut_filter_ = sps_data_ptr->low_cut_filter_;
	header_ptr->low_cut_filter_slope_ = sps_data_ptr->low_cut_filter_slope_;
	return is_ok;
}

int merge_segd_extend_header(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_EXTENDED_HEADER_PTR header_ptr)
{
	int is_ok = APP_SUCCESS;
	SPS_RELATION_RECORD_SPEC_PTR relation_record_ptr = NULL;

	if(header_ptr->sample_frequency_ == 0)
		header_ptr->sample_frequency_ = sps_data_ptr->sample_frequency_;

	if(header_ptr->sample_length_ == 0)
		header_ptr->sample_length_ = sps_data_ptr->sample_length_;

	if(header_ptr->trace_number_ == 0){
		relation_record_ptr = sps_data_ptr->head_relation_records_ptr_;
		while(relation_record_ptr){
			header_ptr->trace_number_ += relation_record_ptr->receiver_point_size_;
			relation_record_ptr = relation_record_ptr->next_;
		}
	}

	header_ptr->well_time_ = sps_data_ptr->shot_point_.uphole_time_;
	header_ptr->source_easting_ = sps_data_ptr->shot_point_.easting_;
	header_ptr->Source_elevation_ = sps_data_ptr->shot_point_.elevation_;
	header_ptr->source_northing_ = sps_data_ptr->shot_point_.northing_;

	return is_ok;
}