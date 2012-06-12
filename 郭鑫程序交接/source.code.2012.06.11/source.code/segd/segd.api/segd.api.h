/**
@brief		修改segd配置文件功能导出函数
@details	这些导出函数,存在于该进程中,需要调用者使用动态加载该执行文件,然后调用相应的函数地址
@file	segd.conv.cpp
@note	该头文件中使用unicode编码,调用者考虑
@see
@code
@endcode
@version	1.1.1.5.2011.09.02
@author	kuoxin	
@bug
*/

#pragma once

#include "targetver.h"

#define __in__
#define __out__
#define __in_out__
#define __in_opt__
#define __out_opt__
#define __in_out_opt__

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

/**
@struct SEGD_WRITING_PROGRESS_INFO
@detail	UDP接受到的SEG-D写入进度通知消息结构体
*/
typedef struct __tagSEGD_WRITING_PROGRESS_INFO
{
	int step_position_; //!< 进度,该值步进位置
	int range_;			//!< 最大范围
	int descrip_size_; //!< descrip_描述信息的大小
	wchar_t*	descrip_; //!< 描述信息,可以是指定正在读取那个文件或其他信息
}SEGD_WRITING_PROGRESS_INFO, *SEGD_WRITING_PROGRESS_INFO_PTR;

/**
@struct	__tagSEGD_API_INFO
@detail	定义传入该模块初始化参数
*/
typedef struct __tagSEGD_API_INFO
{
	wchar_t* config_path_; //!< ,要求使用全路径
	unsigned short listen_port_; //!< 定义socket 监听端口
}SEGD_API_INFO, *SEGD_API_INFO_PTR;

typedef int (*INIT_MODULE)(SEGD_API_INFO_PTR);
typedef int (*SET_MODULE_OPTION)(wchar_t*, wchar_t const*);
typedef void (*RELEASE_MODULE)(void);
typedef const wchar_t* (*GET_ERROR_INFO)(int*);
typedef int (*SHOW_WRITING_PROGRESS)(HWND, wchar_t const*);

extern "C"{

/**
@fn	int init_module(__in__ SEGD_API_INFO_PTR config_info_ptr)
@details	初始化当前功能模块,加载配置文件信息
@param[in]	config_info 传入segd配置信息
@return	如果初始化正确,将返回APP_SUCCESS,否则将返回APP_FAULT, 可以调用get_error_info来获得出错描述
@note
*/
int init_module(__in__ SEGD_API_INFO_PTR config_info_ptr);

/**
@fn	int set_module_option(wchar_t const* option_name, wchar_t const* option_value)
@details 修改指定option_name参数,使用option_value
@param[in]	option_name		指定设置项的名称
@param[in]	option_value	修改值,该值类型将根据使用参数,该参数使用字符串,不论的关键字是哪种类型
@return	如果初始化正确,将返回0,否则将返回出错代码, 可以调用get_error_info来获得出错描述
@note	如果是字符串,那么要使用wchar_t
*/
int set_module_option(__in__ wchar_t const* option_name, __in__ wchar_t const* option_value);

/**
@fn	void release_module();
@details	释放当前模块
@return void
@note
*/
void release_module();

/**
@fn	const wchar_t* get_error_info(int* error_code)
@details 返回出错信息描述
@param[in/out]	error_code		出错信息编码
@return	得到返回的字符串,如果没有方向将返回NULL
@note	
*/
__out__ const wchar_t* get_error_info(__in_out__ int* error_code);
/**
@fn void __debug_hook()
@details 这是一个调试函数,目前尚未使用
@return
@note
*/
int __debug_hook();

/**
@fn __out__ int show_writing_progress(__in_opt__ HWND parent_window_handler, __in_opt__ wchar_t const* window_title)
@details 弹出一个对话框用来显示当前SEGD文件的写入进度情况
@param[in]	parent_window_handler	可选使用,指定当前的弹出对话框的父窗体
@param[in]	window_title			可选使用,指定当前的弹出对话框的标题		
@return	如果创建窗体成功返回APP_SUCCESS,否则返回APP_FAULT,可以通过get_error_info得到错误信息
@note
*/
__out__ int show_writing_progress(__in_opt__ HWND parent_window_handler, __in_opt__ wchar_t const* window_title);
}

/**
@fn	void output_log(char_t const* info)
@details 实现日志输出功能
@param[in]	info	日志字符串
@return	void
@note	要使用char_t
*/
void output_log(wchar_t const* info);

/**
@fn	void output_log(unsigned int id, ...)
@details 实现多参数日志输出功能
@param[in]	id	资源中字符串表中编号
@param[in]	...	根据资源中字符串表字符串的参数来确定
@return	void
@note	要使用char_t, 该函数本身将不检查传参数量
*/
void output_log(unsigned int id, ...);

/**
@fn	int get_last_error()
@details 得到当前最后错误代码
@return	int 返回错误信息
@note	
*/
int get_last_error();

/**
@fn	void set_last_error(int error_code)
@details	设置当前最后的错误代码编号 
@param[in]	error_code	错误代码编号 
@return	void
@note	
*/
void set_last_error(int error_code);

/**
@fn	int get_string(UINT id, char_t* buf, unsigned size)
@details	根据资源ID得到其字符串
@param[in]		id		资源ID
@param[in/out]	buf		得到的字符串
@param[in]		size	buf大小
@return	返回值参考LoadString (Win32 API)
@note	
*/
int get_string(UINT id, wchar_t* buf, unsigned size);
