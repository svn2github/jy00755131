/**
@brief	处理命令行传入配置文件
@details	针对--file命令传入参数指向的文件,来检查和生成新的SEGD文件,当前为.h文件,具体参见实现文件(cpp)
@file	proc.conf.file.h
@note	
@see	关于配置文件参看args.conf
@version	1.1.30.80.2011.10.26	
@author	kuoxin	
@bug
	1.1.5.12.2011.09.07		初始化版本	
	1.1.30.80.2011.10.26	修改定义MAX_CONFIG_FILE_LINE_SIZE常量由0x200改为0x104,
							并修改结构体__tagSEGD_OPTION_ITEM和 __tagCONFIG_FILE_LINE_INFO中的key_name大小
							为MAX_STRING_SIZE/4
*/

#include "../segd.viewer/segd.file.format.h"

/**
@enum __enuPROC_CONF_FILE_VALUE
@brief 定义本模块使用的常量
*/
enum __enuPROC_CONF_FILE_VALUE
{
	MAX_CONFIG_FILE_LINE_SIZE = 0x104,
};

/**
@typedef int (*ASSIGN_VALUE_FUNCTION)(void* value_ptr, const char* value_string)
@brief	定义ASSIGN_VALUE_FUNCTION用于转化配置信息到SEGD_FILE结构体中参数来生成SEGD文件
@param[in]	value_ptr	SEGD_FILE结构体中,变量指针
@param[in]	value_string	配置文件中的字符串值
@return	返回成功APP_SUCCESS, 失败返回APP_FAULT
*/
typedef int (*ASSIGN_VALUE_FUNCTION)(void* value_ptr, const char* value_string);

/**
@struct	__tagCONFIG_FILE_LINE_INFO
@details	定义配置文件参数
@note	当前使用char
*/
typedef struct __tagSEGD_OPTION_ITEM
{
	char	key_name_[MAX_STRING_SIZE/4];//!< 参数关键字
	void*	value_ptr_; //!< 参数指针
	ASSIGN_VALUE_FUNCTION	function_ptr_;//!< 参数赋值函数
}SEGD_OPTION_ITEM, SEGD_OPTION_ITEM_PTR;

/**
@struct	__tagCONFIG_FILE_LINE_INFO
@details	定义配置文件参数
@note	当前使用char
*/
typedef struct __tagCONFIG_FILE_LINE_INFO
{
	char	key_name_[MAX_STRING_SIZE/4];//!< 参数关键字
	char	value_[MAX_STRING_SIZE];//!< 对应传入命令的参数
	char	line_[MAX_STRING_SIZE]; //!< 当前行信息
}CONFIG_FILE_LINE_INFO, *CONFIG_FILE_LINE_INFO_PTR;

/**
@struct	__tagSEGD_CONFIG_FILE
@details	SEGD配置文件数据结构
@note	当前使用char_t
*/
typedef struct __tagSEGD_CONFIG_FILE
{
	char_t					config_path_[MAX_STRING_SIZE];//!< 配置文件路径
	char_t					sps_path_[MAX_STRING_SIZE];//!< SPS文件路径
	char_t					dat_path_[MAX_STRING_SIZE];//!< AD数据文件路径(该文件为通用文件描述)
	char_t					output_path_[MAX_STRING_SIZE];//!< 输出SEG-D文件路径
	char_t					remote_ip_[MAX_STRING_SIZE];	//!< 监听ip地址
	unsigned short			remote_port_; //!< 端口

	SEGD_GENERAL_HEADER_1	header_1_;//!< SEG-D头1
	SEGD_GENERAL_HEADER_2	header_2_;//!< SEG-D头2
	SEGD_GENERAL_HEADER_3	header_3_;//!< SEG-D头3
	SEGD_SCAN_TYPE_HEADER	scan_type_header_; //!< SEG-D 扫描头
	SEGD_EXTENDED_HEADER	extended_header_; //!< SEG-D扩展头

	SEGD_FILE_TRACE_HEADER	trace_header_;//!< SEG-D道头
	SEGD_FILE_TRACE_HEADER	aux_trace_header_;//!< SEG-D附加道头
}SEGD_CONFIG_FILE, *SEGD_CONFIG_FILE_PTR;


/**
@fn	int load_config_file(char_t const* config_path, APPLICATION_ENVIRONMENT_PTR app_env_ptr)
@details	检查并加载配置文件信息到应用环境中
@param[in]		config_path	配置文件
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int load_config_file(char_t const* config_path);

/**
@fn	const SEGD_CONFIG_FILE_PTR get_config_info_ptr()
@details	返回当前SEG-D配置文件信息
@return	返回配置文件结构体指针,该指针不能被修改
@note
*/
const SEGD_CONFIG_FILE_PTR get_segd_config_info_ptr();

/**
@fn	int check_config_file(char_t const* config_path)
@details	检查并配置文件信息
@param[in]		config_path	配置文件
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int check_config_file(char_t const* config_path);

/**
@fn	int check_segd_option(char_t const* option, char const* option_value, unsigned option_value_size)
@details	检查指定参数的数据是否正确
@param[in]	option	配置指令
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int check_segd_option(char_t const* option/*, char const* option_value, unsigned option_value_size*/);

/**
@fn	int modify_segd_option(char_t const* option, char const* option_value, unsigned option_value_size)
@details	修改指定参数的数据
@param[in]	option	配置指令
@param[in]	option_value	配置指令数值,所有的传入变量,都必须转化成字符串,对于字符串来说要加入双引号的
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
	例如: 	sample_error_explaination "this is a demo"	#605-764 	XXXX 	asc	采集错误说明	ASCII 文本，160 个字符
*/
int modify_segd_option(char_t const* option, char const* option_value/*, unsigned option_value_size*/);

/**
@fn	int save_config_file(char_t const* config_path)
@details	保存应用环境中数据到配置文件
@param[in]		config_path	配置文件
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int save_config_file(char_t const* config_path);



