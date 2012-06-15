#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../common/config.h"
#include "../../common/typedef.h"
#include "../../common/log.h"
#include "..\..\common\sockimpl.h"
#include "resource.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "User32")

#define CONF_FILE_NAME	__TEXT("args.conf")

/**
@enum	__enuAPPLICATION_CONST_VALUE
@details	定义全局常量
*/
enum __enuAPPLICATION_CONST_VALUE
{
	APP_CONSOLE_SCREEN_BUFFER_X = 150,	//!< console屏幕缓冲大小X轴 	
	APP_CONSOLE_SCREEN_BUFFER_Y = 9999,	//!< console屏幕缓冲大小Y轴
	APP_CONSOLE_WINDOWS_SIZE_Y = 40,	//!< console window大小Y轴
	MAX_COMMAND_SIZE = 0x19,	//!< 命令行参数最多数量
};

struct __tagAPPLICATION_COMMAND_ARG;
/**
@fn	int (*APP_CMD_FUNC_PTR)(APPLICATION_COMMAND_ARG_PTR); 
@param[in]	制定运行参数
@brief	定义当前命令行传入参数的运行函数
*/
typedef int (*APP_CMD_FUNC_PTR)(__tagAPPLICATION_COMMAND_ARG*); 

/**
@struct	__tagAPPLICATION_COMMAND_ARG
@details	定义命令行传入参数
@note	该数据结构只能针对参数1个的参数有效
*/
typedef struct __tagAPPLICATION_COMMAND_ARG
{
	char_t	arg_name_[MAX_STRING_SIZE];//!< 参数关键字
	APP_CMD_FUNC_PTR	function_ptr; //!<该参数的运行函数
	char_t	value_[MAX_STRING_SIZE];//!< 对应传入命令的参数
	int value_size_; //!< 对应传入命令参数数量
	int priority_;//!< 命令行优先级
}APPLICATION_COMMAND_ARG, *APPLICATION_COMMAND_ARG_PTR;

/**
@struct	__tagSEGD_WRITING_PROGRESS_INFO
@brief	发送进度信息数据结构
*/
typedef struct __tagSEGD_WRITING_PROGRESS_INFO_PRIVATE
{
	SOCKET_IMPL sock_impl_; //!< 使用的socket对象
	int step_position_; //!< 当前进度位置
	int range_;	//!< 进度最大长度
	char_t descrip_[MAX_STRING_SIZE]; //!< 描述信息
}SEGD_WRITING_PROGRESS_INFO_PRIVATE, *SEGD_WRITING_PROGRESS_INFO_PRIVATE_PTR;

/**
@struct	__tagAPPLICATION_ENVIRONMENT
@details	定义全局结构体存放全局变量
*/
typedef struct __tagAPPLICATION_ENVIRONMENT
{
	segd::log*	log_; //!< 日志输出类
	HANDLE	instance_; //!< 当前进程实例句柄
	int		last_error_code_; //!< 最后一个错误编号,可以通过该编号查询到特定字符串
	bool	is_verbose_;//!< 显示输出信息
	char_t	config_file_path_[MAX_STRING_SIZE];//!<如果使用配置文件的话,当前文件的路径
	int		args_[MAX_COMMAND_SIZE];//参数数组,以__app_cmd数组下标
	int		arg_size_;//!<参数个数
	const char_t* version_; //!< 当前版本信息
	unsigned int tb_time_;//!< 当前tb_time信息
	unsigned char manufacturer_code_;//!< 指定制造商代码,sercel为13, 不注明时,通用使用0
	char_t output_log_[MAX_STRING_SIZE]; //!< 输出文件,该文件将根据使用不同的命令来,标定输出文件类型
}APPLICATION_ENVIRONMENT, *APPLICATION_ENVIRONMENT_PTR;

/**
@fn	int main_private(int argc, char_t* argv[],  const char_t* app_version)
@details 实现wmain函数功能
@param[in]	argc	参数个数			
@param[in]	argv	参数变量列表		
@param[in]	app_version	当前版本信息
@return	如果初始化正确,将返回0,否则将返回出错代码, 直接返回给wmain函数,调用程序可以
		通过CreateProcess得到返回信息
@note	如果是字符串,那么要使用wchar_t
*/
int main_private(int argc, char_t* argv[], char_t const* app_version);

/**
@fn	void output_log(char_t const* info)
@details 实现日志输出功能
@param[in]	info	日志字符串
@return	void
@note	要使用char_t
*/
void output_log(char_t const* info);

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
int get_string(UINT id, char_t* buf, unsigned size);

/**
@fn	void send_progress_info(const SEGD_WRITING_PROGRESS_INFO_PTR info_ptr)
@details 发送SEGD文件生成进度
@param[in]	info_ptr	数据结构
@return	
@note	
*/
void send_progress_info(const SEGD_WRITING_PROGRESS_INFO_PRIVATE_PTR info_ptr);
