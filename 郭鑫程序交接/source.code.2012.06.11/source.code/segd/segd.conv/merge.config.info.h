/**
@brief	sps文件,默认配置文件信息写入segd中
@details	该文件主要包括从sps文件,默认配置文件中抽取选型合并后写入到segd配置头中,该文件为.h,具体实现见cpp
@file	merge.config.info.cpp
@note	该文件将使用wchar_t和char混合的字符集方式
@see	proc.conf.file.h, proc.sps.file.h, proc.data.file.h
@version	1.1.13.45.2011.09.29
@author	kuoxin	
@bug
		1.1.13.45.2011.09.29	初始化版本
*/

#ifndef __MERGE_CONFIG_INFO_CPP__
#define __MERGE_CONFIG_INFO_CPP__

/**
@fn int merge_segd_header_1(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_GENERAL_HEADER_1_PTR header_ptr);
@brief 合并sps信息
@param[in]	sps_data_ptr	sps数据
@param[in/out]	header_ptr	合并后的通用头段1数据
@return 如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int merge_segd_header_1(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
						__in_out__ SEGD_GENERAL_HEADER_1_PTR header_ptr);

/**
@fn int merge_segd_header_2(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_GENERAL_HEADER_2_PTR header_ptr);
@brief 合并sps信息
@param[in]	sps_data_ptr	sps数据
@param[in/out]	header_ptr	合并后的通用头段2数据
@return 如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int merge_segd_header_2(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
						__in_out__ SEGD_GENERAL_HEADER_2_PTR header_ptr);

/**
@fn int merge_segd_header_3(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_GENERAL_HEADER_3_PTR header_ptr);
@brief 合并sps信息
@param[in]	sps_data_ptr	sps数据
@param[in/out]	header_ptr	合并后的通用头段3数据
@return 如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int merge_segd_header_3(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, 
						__in_out__ SEGD_GENERAL_HEADER_3_PTR header_ptr);

/**
@fn int merge_segd_scan_type_header(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_SCAN_TYPE_HEADER_PTR header_ptr);
@brief 合并sps信息
@param[in]	sps_data_ptr	sps数据
@param[in/out]	header_ptr	合并后的扫描类型头段数据
@return 如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int merge_segd_scan_type_header(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_SCAN_TYPE_HEADER_PTR header_ptr);

/**
@fn int merge_segd_extend_header(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_EXTENDED_HEADER_PTR header_ptr);
@brief 合并sps信息
@param[in]	sps_data_ptr	sps数据
@param[in/out]	header_ptr	合并后的扩展类型头段数据
@return 如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int merge_segd_extend_header(__in__ const SPS_DATA_INFO_PTR sps_data_ptr, __in_out__ SEGD_EXTENDED_HEADER_PTR header_ptr);

#endif //__MERGE_CONFIG_INFO_CPP__