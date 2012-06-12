/**
@brief	各模块参数文件导入接口
@details	该头文件中提供了参数文件操作接口函数说明,其中包括config, export, line. log, operation, 
			plotter, positioning, vibrator
@file	params.io.h
@note	
@see	
@version	1.0.0.1.2011.11.16
@author	kuoxin	
@bug
*/

#ifndef __PARAMS_IO_H__
#define __PARAMS_IO_H__

#include "params.header.h"

/**
@enum __enuPARAMS_STATUS
@brief 接口函数返回状态
*/
enum __enuPARAMS_IO_STATUS
{
	PARAMS_IO_SUCCESS = 0, //!< 成功
	PARAMS_IO_FAULT = -1, //!< 失败
};

/**
@enum __enuPARAMS_IO_SET_TYPE
@brief 参数集合类型
*/
enum __enuPARAMS_IO_SET_TYPE
{
	LOG_PARAMS_IO_SET_TYPE = 1, //!< log参数集合
	CONFIG_PARAMS_IO_SET_TYPE, //!< config参数集合
	LINE_PARAMS_IO_SET_TYPE, //!< line参数集合
	EXPORT_PARAMS_IO_SET_TYPE, //!< export参数集合
	OPTERATION_PARAMS_IO_SET_TYPE, //!< operation参数集合
	PLOTTER_PARAMS_IO_SET_TYPE, //!< plotter参数集合
	POSITIONING_PARAMS_IO_SET_TYPE, //!< positioning参数集合
	VIBRATOR_PARAMS_IO_SET_TYPE, //!< vibrator参数集合
};

/**
@typedef int (*INIT_PARAMS_IO)()
@brief 定义params import 初始化函数
@see 
*/
typedef int (*INIT_PARAMS_IO)();

/**
@typedef int (*RELEASE_PARAMS_IO)()
@brief 定义params import 释放函数
@see int release_params_io()
*/
typedef int (*RELEASE_PARAMS_IO)();

/**
@typedef int (*PARAMS_GET_LAST_ERROR_PROC)(char*, int*)
@brief 定义params import 函数
@see int get_last_error(char*, int*)
*/
typedef int (*PARAMS_GET_LAST_ERROR_PROC)(char*, int*);

/**
@typedef int (*IMPORT_PARAMS_PROC)(char const*, enum __enuPARAMS_IO_SET_TYPE, void*) 
@brief 定义params import 导入函数
@see int import_params(char const* param_path, enum __enuPARAMS_IO_SET_TYPE type, void* params)
*/
typedef int (*IMPORT_PARAMS_PROC)(char const*, enum __enuPARAMS_IO_SET_TYPE, void*);

/**
@typedef typedef int (EXPORT_PARAMS_PROC)(char const*, enum __enuPARAMS_IO_SET_TYPE, void const*)
@brief 定义params import 导入函数
@see int export_params(char const* params_path, enum __enuPARAMS_IO_SET_TYPE type, void const* params)
*/
typedef int (*EXPORT_PARAMS_PROC)(char const*, enum __enuPARAMS_IO_SET_TYPE, void const*);

/**
 @fn int init_params_io();
 @brief SPS import/extport 初始化函数
 @return 如果成功返回PARAMS_IO_SUCCESS,否则返回PARAMS_IO_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int init_params_io();

/**
 @fn int import_params(char const* params_path, enum __enuPARAMS_IO_SET_TYPE type, void* params_list)
 @brief 导入指定的类型参数文件的参数列表
 @param[in] params_path 输入路径
 @param[in] type 参数集合类型,参见enum __enuPARAMS_IO_SET_TYPE
 @param[in/out] params 数据集合信息
 @return 如果成功返回PARAMS_IO_SUCCESS,否则返回PARAMS_IO_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int import_params(char const* params_path, enum __enuPARAMS_IO_SET_TYPE type, void* params);

/**
 @fn int export_params(char const* params_path, enum __enuPARAMS_IO_SET_TYPE type, void const* params_list)
 @brief 导出指定的类型参数文件的参数列表
 @param[in] params_path 输入路径
 @param[in] type 参数集合类型,参见enum __enuPARAMS_IO_SET_TYPE
 @param[in] params 数据集合信息
 @return 如果成功返回PARAMS_IO_SUCCESS,否则返回PARAMS_IO_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int export_params(char const* params_path, enum __enuPARAMS_IO_SET_TYPE type, void const* params);

/**
 @fn int release_params_io()
 @brief 释放模块
 @return 如果成功返回PARAMS_IO_SUCCESS,否则返回PARAMS_IO_FAULT, 通过调用get_last_error()返回错误信息
 @remark
*/
int release_params_io();

/**
 @fn int get_last_error(char* error_info, int* error_info_size)
 @brief 得到当前程序模块错误信息
 @param[in/out] error_info 传入有效缓冲区,返回当前错误缓冲
 @param[in/out] error_info_size 传入error_info大小,返回缓冲实际填充大小
 @return 返回当前的错误编号,如果返回PARAMS_IO_SUCCESS说明没有错误
 @remark
*/
int get_last_error(char* error_info, int* error_info_size);

#endif /*__PARAMS_IO_H__*/

