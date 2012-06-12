/**
@brief	显示segd文件写入进度情况	
@details	见segd.api.dialog.cpp实现文件	
@file	segd.api.dialog.h
@note	该头文件中使用unicode编码,调用者考虑
@see	
@code	
@endcode
@version	1.0.2.20.2011.09.19
@author	kuoxin	
@bug
*/

#ifndef __SEGD_API_DIALOG_H__
#define __SEGD_API_DIALOG_H__

/**
@fn	int show_writing_progress_private(unsigned short listen_port, HWND parent_window_handler, wchar_t const* window_title)
@details 
@param[in]	listen_port	
@param[in]	parent_window_handler	参数变量列表		
@param[in]	window_title	当前版本信息
@return	如果初始化正确,将返回APP_SUCCESS,否则将返回APP_FAULT, 可以调用get_last_error得到出错代码
@note	如果是字符串,那么要使用wchar_t
*/
int show_writing_progress_private(__in__ unsigned int listen_port, __in_opt__ HWND parent_window_handler, __in_opt__ wchar_t const* window_title);


#endif //__SEGD_API_DIALOG_H__