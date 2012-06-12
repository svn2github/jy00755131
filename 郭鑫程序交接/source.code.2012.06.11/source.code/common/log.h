/*
*	描述：该文件为log基本操作类的声明文件
*	文件：log.h
*	注释：具体功能实现现实见log.cpp
*	参考文献：
*	版本：1.0.0.0
*	完成人：
*	example:
*/
#ifndef __LOG_H__
#define __LOG_H__

namespace segd{

class log
{
public:
	enum{
		MAX_BUFFER_SIZE = 2048,
	};

public:
	typedef unsigned int size_t;
	typedef enum __enuLOG_TYPE{
		LOG_FILE_TYPE,
		DBGVIEW_TYPE,
		CONSOLE_TYPE,
		UDP_TYPE,
	}LOG_TYPE;

	typedef enum __enuLOG_INFO_TYPE{
		FATAL_INFO = 0, 
		ERROR_INFO = 1,
		WARN_INFO = 2,
		INFO_INFO = 3,
		DEBUG_INFO = 4,
		NULL_INFO = 5,
	} LOG_INFO_TYPE;

	typedef struct __tagLOG_CALLBACK_ARG
	{
		log* log_;
		char_t const* buf_;
		log::size_t size_;
	}LOG_CALLBACK_ARG, *ptLOG_CALLBACK_ARG;

public:
	log();
	~log();

public:
	bool init(const char_t* file_name, log::LOG_TYPE type = LOG_FILE_TYPE, log::size_t max_buffer_size = MAX_BUFFER_SIZE);
	void release();	
	bool output_log(char_t const** arg_array, log::size_t arg_array_size, log::LOG_INFO_TYPE type = log::DEBUG_INFO);
	bool output_log(const char_t* info, log::LOG_INFO_TYPE type = log::DEBUG_INFO);

protected:
	bool output_log_private(char_t const** arg_array, log::size_t arg_array_size, bool (*output_log_address)(ptLOG_CALLBACK_ARG), 
							log::LOG_INFO_TYPE type = log::DEBUG_INFO);

private:
	
	bool alloc_console_private();
	bool free_console_private();

	static bool output_log_file_private(ptLOG_CALLBACK_ARG);
	static bool output_console_private(ptLOG_CALLBACK_ARG);
	static bool output_dbgview_private(ptLOG_CALLBACK_ARG);

	log::size_t make_string_private(char_t const ** arg_array, log::size_t arg_array_size, log::LOG_INFO_TYPE type,
		char_t* out_buf, log::size_t out_buf_size);

private:
	FILE* file_handle_;
	log::LOG_TYPE type_;
	log::LOG_INFO_TYPE filter_;//if value of log-info-type is bigger than the filer, do not output it!
	CRITICAL_SECTION critical_section_;
	log::size_t max_buffer_size_;
	char_t* buffer_internal_;
	HANDLE	std_output_handler_;

	static const char_t log_info_token_[][MAX_STRING_SIZE];
};
};
#endif /*__LOG_H__*/
