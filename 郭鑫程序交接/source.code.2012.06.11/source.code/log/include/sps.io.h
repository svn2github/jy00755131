/**
@brief	SPS文件导入导出接口
@details 该头文件中提供了SPS文件操作接口函数说明
@file	sps.io.h
@note	
@see	有关SPS内容,参见SPS标准(Transcriber 2 SPS Format; e428XL V4.0 VOL.2, 
		关于数据结构参考sps.header.h
@version	1.0.0.1.2011.11.16
@author	kuoxin	
@bug
*/

#ifndef __SPS_IO_H__
#define __SPS_IO_H__

#include "sps.header.h"

enum 
{
	SPS_SUCCESS = 0,
	SPS_FAULT = -1,
};

typedef struct __tagSHOT_POINT_DATA
{
	SPS_POINT_RECORD_SPEC data_;	//!< 点记录信息
	SPS_POINT_RECORD_SPEC_PTR next_;//!< 点记录信息指针链表
}SHOT_POINT_DATA, *SHOT_POINT_DATA_PTR, RECEIVER_POINT_DATA, *RECEIVER_POINT_DATA_PTR;

typedef struct __tagRELATION_RECORD_SPEC
{
	SPS_RELATION_RECORD_SPEC data_;	//!< 点记录信息
	int	receiver_point_size_;//!< 分配数据大小
	RECEIVER_POINT_DATA_PTR	head_receiver_point_ptr_;//!< 接受点数据信息,该值为指针,内存的分配主要是由分析成完成,使用后,需要删除
	RECEIVER_POINT_DATA_PTR	tail_receiver_point_ptr_;//!< 接受点数据信息,该值为指针,内存的分配主要是由分析成完成,使用后,需要删除
	RELATION_RECORD_SPEC_PTR next_; //!< 下一个结构体指针,便于使用链表
}RELATION_RECORD_SPEC, *RELATION_RECORD_SPEC_PTR;

/**
@struct __tagSPS_DATA
@brief 保存s,x,r文件信息数据结构
@see sps.header.h
*/
typedef struct __tagSPS_DATA
{
	SHOT_POINT_DATA	shot_pointers_;			//!< 炮点数据链表
	RECEIVER_POINT_DATA	receiver_pointers_;	//!< 接受点数据链表
	RELATION_RECORD_SPEC relation_records_;	//!< 关系数据链表
}SPS_DATA, *SPS_DATA_PTR;

/**
@typedef int (*INIT_SPS_IO_PROC)();
@brief 定义SPS import/export 初始化函数
@see int init_sps_io()
*/
typedef int (*INIT_SPS_IO_PROC)();

/**
@typedef int (*RELEASE_SPS_IO_PROC)();
@brief 定义SPS import/export 释放函数
@see int release_sps_io()
*/
typedef int (*RELEASE_SPS_IO_PROC)();

/**
@typedef int (*SPS_GET_LAST_ERROR_PROC)(char*, int*)
@brief 定义SPS import/export 函数
@see int get_last_error(char*, int*)
*/
typedef int (*SPS_GET_LAST_ERROR_PROC)(char*, int*);

/**
@typedef int (SPS_IMPORT)(char const*, SPS_DATA_PTR)
@brief 定义SPS import/export 导入函数
@see int import_sps(char const* sps_path, SPS_DATA_PTR sps_data);
*/
typedef int (SPS_IMPORT)(char const*, SPS_DATA_PTR);

/**
@typedef int (SPS_EXPORT)(char const*, SPS_DATA_PTR)
@brief 定义SPS import/export 导入函数
@see int export_sps(char const* sps_path, SPS_DATA_PTR sps_data);
*/
typedef int (SPS_EXPORT)(char const*, SPS_DATA_PTR);

/**
 @fn int init_sps_io()
 @brief SPS import/extport 初始化函数
 @return 如果成功返回SPS_SUCCESS,否则返回SPS_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int init_sps_io();

/**
 @fn int import_sps(char const* sps_path, SPS_DATA_PTR sps_data)
 @brief 输入sps文件,得到SPS数据
 @param[in] sps_path 输入路径,将打开后缀s, x, r 文件
 @param[in/out] sps_data 数据信息
 @return 如果成功返回SPS_SUCCESS,否则返回SPS_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int import_sps(char const* sps_path, SPS_DATA_PTR sps_data);

/**
 @fn int export_sps(char const* sps_path, const SPS_DATA_PTR sps_data)
 @brief 输出SPS文件
 @param[in] sps_path 输入路径,将文件后缀加入s, x, r 
 @param[in] sps_data 数据信息
 @return 如果成功返回SPS_SUCCESS,否则返回SPS_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int export_sps(char const* sps_path, const SPS_DATA_PTR sps_data);

/**
 @fn int release_sps_io()
 @brief 释放模块
 @return 如果成功返回SPS_SUCCESS,否则返回SPS_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int release_sps_io();

/**
 @fn int get_last_error(char* error_info, int* error_info_size)
 @brief 得到当前程序模块错误信息
 @param[in/out] error_info 传入有效缓冲区,返回当前错误缓冲
 @param[in/out] error_info_size 传入error_info大小,返回缓冲实际填充大小
 @return 返回当前的错误编号,如果返回SPS_SUCCESS说明没有错误
 @remark
*/
int get_last_error(char* error_info, int* error_info_size);

#endif /*__SPS_IO_H__*/

