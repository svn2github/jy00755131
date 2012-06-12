/**
@brief	该文件主要实现导出配置文件设置功能
@details 将导出如下函数:
			init_module
			release_module
			set_module_option
			get_error_info
			__debug_hook
		具体参考每个函数说明
@file	segd.api.cpp
@note	
@see	
@version	1.0.13.45.2011.10.28
@author	kuoxin	
@bug
	1.0.2.20.2011.09.19		加入对于UDP通知支持
	1.0.12.44.2011.10.20	修改程序dllmain中swith语句低级错误,导致的socket提前释放问题
	1.0.13.45.2011.10.28	修改进度条重画跳动问题
*/
#include "..\..\common\config.h"
#include "segd.api.h"
#include "..\segd.conv\proc.conf.file.h"
#include <shlwapi.h>
#include <commctrl.h>
#include "resource.h"
#include "..\..\common\sockimpl.h"
#include "segd.api.dialog.h"

#pragma comment(lib, "comctl32")

int __last_error_code = 0x0;
int __listen_port = 0x0;
char_t __config_file_path[MAX_STRING_SIZE];
char_t __last_error_info[MAX_STRING_SIZE];
HINSTANCE __instance = NULL;

INT_PTR CALLBACK dialog_proc(HWND, UINT,	WPARAM,	LPARAM);

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	UNREFERENCED_PARAMETER(lpReserved);
	switch(ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
		 InitCommonControls();
		 init_socket();
		__instance = hModule;
		break;
	case DLL_THREAD_ATTACH:		break;
	case DLL_THREAD_DETACH:		break;
	case DLL_PROCESS_DETACH:
		release_socket();
		break;
	}
	return TRUE;
}

int get_string(UINT id, char_t* buf, unsigned size)
{	return LoadString(__instance, id, buf, size); }

void output_log(unsigned int id, ...)
{
	int size = 0;
	char_t format[MAX_STRING_SIZE] = {0};
	va_list ap;

	size = get_string(id, format, sizeof(format)/sizeof(format[0]));
	format[size] = 0x0;

	va_start(ap, id);
	__matrix_vsprintf_s(__last_error_info, format, ap);
	va_end(ap);
}

void output_log(char_t const* info){	StrCpy(__last_error_info, info);}

int get_last_error(){	return __last_error_code;}

void set_last_error(int error_code){__last_error_code = error_code;}

int init_module(__in__ SEGD_API_INFO_PTR config_info_ptr)
{
	int error_code = APP_SUCCESS;

	memset(__config_file_path, 0x0, sizeof(__config_file_path));
	StrCpy(__config_file_path, config_info_ptr->config_path_);
	error_code = check_config_file(__config_file_path);
	if(error_code == APP_FAULT){
		__last_error_code = get_last_error();
		output_log(IDS_STRING5017, __last_error_code);
		__last_error_code = IDS_STRING5017;
		return error_code = APP_FAULT;
	}

	if(config_info_ptr->listen_port_ == 0x0){	
		output_log(__last_error_code = IDS_STRING5018);		
		error_code = APP_FAULT;	
	}
	else{	__listen_port = config_info_ptr->listen_port_;}

	return error_code;
}

int set_module_option(wchar_t* option_name, wchar_t const* option_value)
{
	char tmp_option_value[MAX_STRING_SIZE * 2] = {0};
	int error_code =0x0;
	size_t size = 0x0;
	errno_t errno;

	error_code = load_config_file(__config_file_path);
	if(error_code == APP_FAULT){	__last_error_code = get_last_error();	goto GOTO;}

	error_code = check_segd_option(option_name);
	if(error_code == APP_FAULT){	__last_error_code = get_last_error();	goto GOTO;}

	errno = wcstombs_s(&size, tmp_option_value, sizeof(tmp_option_value), option_value, sizeof(tmp_option_value));
	if(errno != 0){	__last_error_code = IDS_STRING5015; 	goto GOTO;}

	tmp_option_value[size] = 0x0;
	error_code = modify_segd_option(option_name, tmp_option_value);
	if(error_code == APP_FAULT){	__last_error_code = get_last_error();	goto GOTO;}

	error_code = save_config_file(__config_file_path);
	if(error_code == APP_FAULT){	__last_error_code = get_last_error();}

GOTO:
	return error_code;
}

void release_module()
{
	memset(__config_file_path, 0x0, sizeof(__config_file_path));
	__last_error_code = 0x0;
}

const wchar_t* get_error_info(int* error_code)
{
	*error_code = __last_error_code;
	memset(__last_error_info, 0x0, sizeof(__last_error_info));
	get_string(__last_error_code, __last_error_info, sizeof(__last_error_info) / sizeof(__last_error_info));
	return __last_error_info;
}

int __debug_hook()
{
	int error_code = 0;
	return error_code;
}

int show_writing_progress(HWND parent_window_handler, wchar_t const* window_title)
{	return show_writing_progress_private(__listen_port, parent_window_handler, window_title);}
