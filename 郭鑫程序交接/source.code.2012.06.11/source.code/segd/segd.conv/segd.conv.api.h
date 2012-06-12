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

typedef int (*INIT_MODULE)(wchar_t*);
typedef int (*SET_MODULE_OPTION)(wchar_t*, wchar_t const*);
typedef void (*RELEASE_MODULE)(void);
typedef const wchar_t* (*GET_ERROR_INFO)(int*);

extern "C"{

/**
@fn	int init_module(wchar_t* config_filename)
@details	初始化当前功能模块,加载配置文件信息
@param[in]	config_filename 传入segd配置路径信息,要求使用全路径
@return	如果初始化正确,将返回0,否则将返回出错代码, 可以调用scGetErrorInfo来获得出错描述
@note
*/
int init_module(WCHAR* config_filename);

/**
@fn	int set_module_option(wchar_t* option_name, wchar_t const* option_value)
@details 修改指定option_name参数,使用option_value
@param[in]	option_name		指定设置项的名称
@param[in]	option_value	修改值,该值类型将根据使用参数,该参数使用字符串,不论的关键字是哪种类型
@return	如果初始化正确,将返回0,否则将返回出错代码, 可以调用get_error_info来获得出错描述
@note	如果是字符串,那么要使用wchar_t
*/
int set_module_option(wchar_t* option_name, wchar_t const* option_value);

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
const wchar_t* get_error_info(int* error_code);
/**
@fn void __debug_hook()
@details 这是一个调试函数,目前尚未使用
@param
@return
@note
*/
int __debug_hook();
}
