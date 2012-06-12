/**
@brief	该文件为log基本操作类的实现文件
@file	log.cpp
@note	具体声明和调用见log.h
@see
@version	1.0.0.0
@author	kuoxin	
@code
	char lp[] = "aaaa";
	char lp1[] = "bbbb";
	char lp2[] = "ccccc";
	char lp3[] = "xxxxx";
	char lp4[] = "dddddd";
	char lp5[] = "eeee";
	char lp6[] = "fffff";
	char lp7[] = "gggg";
	char lp8[] = "hhhhh";
	char lp9[] = "iiiii";
	char lp10[] = "kkkk";

	char* arg[10] = {lp1, lp2, lp3, lp4, lp5, lp6, lp7, lp8, lp9, lp10};

	log_.init();
	log_.output_log(arg, sizeof(arg)/sizeof(arg[0]));
	log_.release();
@endcode
@bug
	2011.08.24	支持中文显示在控制台上，因此必须要注意的是即使在ansi系统上输出到控制台也要用936
				修改文件打开方式由'a' 到 'a+'
				修改打开日志方式,使用共享打开模式.
	2011.09.06	修改该类支持unicode, 加入对于进程类型判断,如果是CUI将直接使用console,否则将创建新的console

*/

#include <windows.h>
#include <time.h>
#include <share.h>
#include <stdio.h>
#include <memory.h>
#include <conio.h>
#include <locale.h>
#include <ImageHlp.h>

#include "config.h"
#include "log.h"

#pragma comment(lib, "Imagehlp.lib")

namespace segd{

const char_t log::log_info_token_[][MAX_STRING_SIZE] = {
	__TEXT("FATAL"),
	__TEXT("ERROR"),
	__TEXT("WARN"),
	__TEXT("INFO"),
	__TEXT("DEBUG")
};

log::log():max_buffer_size_(MAX_BUFFER_SIZE), buffer_internal_(NULL), file_handle_(NULL), std_output_handler_(NULL)
{
	InitializeCriticalSection(&critical_section_);
}

log::~log()
{
	release();
	DeleteCriticalSection(&critical_section_);
}

bool log::alloc_console_private()
{
	char image_path[MAX_STRING_SIZE] = {0};
	bool is_ok = true;
	PLOADED_IMAGE loaded_image_ptr = NULL;

	GetModuleFileNameA(NULL, image_path, sizeof(image_path));
	loaded_image_ptr = ImageLoad(image_path, NULL);

	switch(loaded_image_ptr->FileHeader->OptionalHeader.Subsystem){
	   case IMAGE_SUBSYSTEM_UNKNOWN:
			is_ok = false;
		   break;
	   case IMAGE_SUBSYSTEM_NATIVE:
	   case IMAGE_SUBSYSTEM_WINDOWS_GUI:
	   case IMAGE_SUBSYSTEM_OS2_CUI:
	   case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
	   case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
		   if(AllocConsole() == FALSE) is_ok = false;
		   break;
	   case IMAGE_SUBSYSTEM_WINDOWS_CUI:
	   case IMAGE_SUBSYSTEM_POSIX_CUI:
		   break;
	}

	ImageUnload(loaded_image_ptr);
	std_output_handler_ = GetStdHandle(STD_OUTPUT_HANDLE);
	__matrix_setlocale(LC_ALL, __TEXT("chs"));
	return is_ok;
}

bool log::free_console_private()
{
	bool is_ok = true;
	if(FreeConsole() == FALSE) is_ok = false;
	return is_ok;
}

bool log::init(const char_t* file_name, log::__enuLOG_TYPE type, log::size_t max_buffer_size)
{
	size_t size = 0;
	
	switch(type){
		case LOG_FILE_TYPE:
			{
				type_ = type;
				file_handle_= __matrix_fsopen(file_name, __TEXT("a+"), _SH_DENYWR);
				if(file_handle_ == NULL){

					OutputDebugString(__TEXT("can not open a log file!"));
					return false;
				}
			}
			break;
		case DBGVIEW_TYPE:
			type_ = type;
			break;
		case CONSOLE_TYPE:
			type_ = type;
			if(!alloc_console_private()){

				OutputDebugString(__TEXT("can not allocate a console window!"));
				return false;
			}
			break;
		default:
			if(file_name != NULL){

				size = __matrix_strnlen_s(file_name, MAX_STRING_SIZE);
				if(size > 0){
					file_handle_= __matrix_fsopen(file_name, __TEXT("a+"), _SH_DENYWR);
					type_ = file_handle_ == NULL ? log::DBGVIEW_TYPE : log::LOG_FILE_TYPE;
				}
			}
			break;		
	}
	
	if(max_buffer_size >= max_buffer_size_) max_buffer_size_ = max_buffer_size + 4;

	if(buffer_internal_){

		free(buffer_internal_);
		buffer_internal_ = NULL;
	}
	else{

		buffer_internal_ = (char_t*)malloc(max_buffer_size_* sizeof(char_t));
		memset(buffer_internal_, 0x0, sizeof(char_t) * max_buffer_size_);
	}
		
	return true;
}

void log::release()
{
	if(buffer_internal_){

		free(buffer_internal_);
		buffer_internal_ = NULL;
	}

	if(file_handle_){

		fflush(file_handle_);
		fclose(file_handle_);
		file_handle_ = NULL;
	}
}

bool log::output_log(const char_t* info, log::LOG_INFO_TYPE type)
{
	const char_t* arg_arry[] = {(char_t*)info};
	return output_log(arg_arry, 1, type);
}

bool log::output_log(char_t const** arg_array, log::size_t arg_array_size, log::LOG_INFO_TYPE type)
{
	bool is_ok = true;

	switch(type_){
		case LOG_FILE_TYPE:
			is_ok = output_log_private(arg_array, arg_array_size, &log::output_log_file_private, type);
			break;
		case CONSOLE_TYPE:
			is_ok = output_log_private(arg_array, arg_array_size, &log::output_console_private, type);
			break;
		case DBGVIEW_TYPE:
			is_ok = output_log_private(arg_array, arg_array_size, &log::output_dbgview_private, type);
			break;
	}
	return is_ok;
}

bool log::output_log_private(char_t const** arg_array, log::size_t arg_array_size, 
							 bool (*output_log_address)(ptLOG_CALLBACK_ARG), log::LOG_INFO_TYPE type)
{
	LOG_CALLBACK_ARG arg;
	arg.buf_ = buffer_internal_;
	arg.log_ = this;

	arg.size_ = make_string_private(arg_array, arg_array_size, type, buffer_internal_, max_buffer_size_);
	
	return output_log_address(&arg);
};

bool log::output_console_private(ptLOG_CALLBACK_ARG arg)
{

//#if defined(_UNICODE) || defined(UNICODE)
//
//	log::size_t count = 0;
//	WCHAR* tmp_ptr = NULL;
//	count = mbstowcs(NULL, arg->buf_, 0);
//	if(count == size_t(-1)) return false;
//	tmp_ptr = (WCHAR*)malloc((++count) * sizeof(WCHAR));
//	if(tmp_ptr == NULL) return false;
//	count = mbstowcs(tmp_ptr, arg->buf_, count);
//	tmp_ptr[count] = 0x0;
//#endif //defined(_UNICODE) || defined(UNICODE)

	EnterCriticalSection(&arg->log_->critical_section_);

	WriteConsole(arg->log_->std_output_handler_, arg->buf_, arg->size_, NULL, NULL);
//#if defined(_UNICODE) || defined(UNICODE)
//	_cwprintf(L"%s", tmp_ptr);
//#else //MBCS
//	_cprintf("%s", arg->buf_);
//#endif 

	LeaveCriticalSection(&arg->log_->critical_section_);

//#if defined(_UNICODE) || defined(UNICODE)
//	if(tmp_ptr)	free(tmp_ptr);
//#endif

	return true;
}	

bool log::output_dbgview_private(ptLOG_CALLBACK_ARG arg)
{
	OutputDebugString(arg->buf_);
	return true;
}

bool log::output_log_file_private(ptLOG_CALLBACK_ARG arg)
{

	if(arg->log_->file_handle_ == NULL) return false;

	EnterCriticalSection(&arg->log_->critical_section_);

	fwrite(arg->buf_, sizeof(char_t), arg->size_, arg->log_->file_handle_);
	fflush(arg->log_->file_handle_);

	LeaveCriticalSection(&arg->log_->critical_section_);

	return true;
}

log::size_t log::make_string_private(char_t const** arg_array, log::size_t arg_array_size, log::LOG_INFO_TYPE type,
									 char_t* out_buf, log::size_t out_buf_size)
{
	log::size_t i = 0;
	log::size_t result = 0x0;

	if(arg_array == NULL || arg_array_size == 0 || out_buf == NULL || out_buf_size == 0)
		return false;

	memset(out_buf, 0x0, out_buf_size * sizeof(char_t));

	if(arg_array[0] != NULL){
		if(type == NULL_INFO)	__matrix_sprintf_s(out_buf, out_buf_size, __TEXT("%s "), arg_array[0]);
		else					__matrix_sprintf_s(out_buf, out_buf_size, __TEXT("[%s] %s "), log_info_token_[type], arg_array[0]);
	}
	else{
		if(type != NULL_INFO)	__matrix_sprintf_s(out_buf, out_buf_size, __TEXT("[%s] "), log_info_token_[type]);
	}

	for(i = 1; i < arg_array_size; ++i){

		if(arg_array[i] != NULL){
			__matrix_strcat_s(out_buf, out_buf_size, __TEXT(" "));
			__matrix_strcat_s(out_buf, out_buf_size, arg_array[i]);
		}
	}

	result = __matrix_strnlen_s(out_buf, out_buf_size);
	if(out_buf[result-1] == __TEXT(' ')){
		out_buf[result - 1] = __TEXT('\n');
		out_buf[result] = 0x0;
	}
	else{
		out_buf[result++] = __TEXT('\n');
		out_buf[result] = 0x0;
	}

	return result;
}
}
