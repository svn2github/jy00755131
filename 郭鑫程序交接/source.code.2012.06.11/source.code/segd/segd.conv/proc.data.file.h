/**
@brief	处理命令行传入数据文件
@details	针对--data-file命令传入参数指向的文件,读取数据文件头和AD数据,当前为.h文件,具体参见实现文件(cpp)
@file	proc.conf.file.h
@note	
@see	关于配置文件参看args.conf
@version		1.1.13.45.2011.09.29
@author	kuoxin	
@bug
	1.1.12.40.2011.09.28	初始化版本
	1.1.13.45.2011.09.29	加入分析数据文件头和修改修改配置文件

*/
#ifndef __PROC_DATA_FILE_H__
#define	__PROC_DATA_FILE_H__

/*

@struct	__tagAD_DATA_FILE_HEAD_PRIVATE 
@brief	AD数据文件头信息
@note	typedef struct ___tagAD_DATA_FILE_HEAD_PRIVATE  AD_DATA_FILE_HEAD_PRIVATE , *AD_DATA_FILE_HEAD_PRIVATE_PTR;

typedef struct __tagAD_DATA_FILE_HEAD
{
	char	config_path_[MAX_STRING_SIZE_LOCAL];//!< 配置文件路径
	char	sps_path_[MAX_STRING_SIZE_LOCAL];//!< SPS文件路径
	char	dat_path_[MAX_STRING_SIZE_LOCAL];//!< AD数据文件路径(该文件为通用文件描述)
	char	output_path_[MAX_STRING_SIZE_LOCAL];//!< 输出SEG-D文件路径
	char	remote_ip_[MAX_STRING_SIZE_LOCAL];	//!< 监听ip地址
	unsigned	remote_port_; //!< 端口

	int	shot_number_;			//!< 炮号 1 到 9999
	int	FileNumber_;			//!< 文件编号  1 到 9999

	float source_line_number_;	//!< 测线号
	float source_point_number_;	//!< 震源点号

	int scan_type_number_; //!< 接受线或辅助线数量
	int source_type_;			//!< 29-32 XXXX bin 震源类型0 = 无震源 1 = 脉冲 2 = 可控震源

	int sample_frequency_;		//!< 采样率250、500、1000、2000、4000 微秒
	int sample_length_;			//!< 采集长度1000到128000 毫秒
	int sample_number_in_trace_;	//!< 33-36 XXXX bin 记录道中样本数目1 到 128000
	float tb_window_; //!< TB窗口值
	time_t datetime_; //!< 开始时间
	int tb_time_;	  //内部TB时刻=本地时间，从该时刻起记录数据
}AD_DATA_FILE_HEAD, *AD_DATA_FILE_HEAD_PTR;

@struct	__tagAD_DATA_FILE_HEAD_PRIVATE 
@brief	AD数据文件头信息
@note	typedef struct ___tagAD_DATA_FILE_HEAD_PRIVATE  AD_DATA_FILE_HEAD_PRIVATE , *AD_DATA_FILE_HEAD_PRIVATE_PTR;

typedef struct __tagAD_DATA_SCAN_TYPE_INFO
{
	int trace_number_;	//!< 道数量
	int line_type_;//!< 接受线: 1		辅助线数量: 9

	float resistor_lower_limit_; //!< 电阻下限
	float resistor_higher_limit_;//!< 电阻上限
	float resistor_value_; //!< 电阻值
	float tilt_limit_; //!< 倾斜度界限
	float tilt_value_; //!< 倾斜度值
	bool resistor_deviation_; //!< 电阻误差
	bool tilt_deviation_; //!< 倾斜度误差	
}AD_DATA_SCAN_TYPE_INFO, *AD_DATA_SCAN_TYPE_INFO_PTR;

*/

#define MAX_STRING_SIZE_LOCAL 128

/**
@struct	__tagAD_DATA_FILE_HEAD_PRIVATE 
@brief	AD数据文件头信息
@note	typedef struct ___tagAD_DATA_FILE_HEAD_PRIVATE  AD_DATA_FILE_HEAD_PRIVATE , *AD_DATA_FILE_HEAD_PRIVATE_PTR;
*/
typedef struct __tagAD_DATA_FILE_HEAD_PRIVATE 
{
	char	config_path_[MAX_STRING_SIZE_LOCAL];//!< 配置文件路径
	char	sps_path_[MAX_STRING_SIZE_LOCAL];//!< SPS文件路径
	char	dat_path_[MAX_STRING_SIZE_LOCAL];//!< AD数据文件路径(该文件为通用文件描述)
	char	output_path_[MAX_STRING_SIZE_LOCAL];//!< 输出SEG-D文件路径
	char	remote_ip_[MAX_STRING_SIZE_LOCAL];	//!< 监听ip地址
	unsigned	remote_port_; //!< 端口

	int	shot_number_;			//!< 炮号 1 到 9999
	int	FileNumber_;			//!< 文件编号  1 到 9999

	float source_line_number_;	//!< 测线号
	float source_point_number_;	//!< 震源点号

	int sample_frequency_;		//!< 采样率250、500、1000、2000、4000 微秒
	int sample_length_;			//!< 采集长度1000到128000 毫秒

	int receiver_line_number_; //!< 接受线数量
	int	auxiliary_line_number_;		//!< 辅助线数量
	int	auxiliary_trace_number_;	//!< 辅助记录道数量	
	int trace_total_number_;	//!< 记录道总数1到100000
	int source_type_;			//!< 29-32 XXXX bin 震源类型0 = 无震源 1 = 脉冲 2 = 可控震源
	int sample_number_in_trace_;	//!< 33-36 XXXX bin 记录道中样本数目1 到 128000

	float tb_window_; //!< TB窗口值
	time_t datetime_; //!< 开始时间
	int tb_time_;	  //内部TB时刻=本地时间，从该时刻起记录数据

	float resistor_lower_limit_; //!< 电阻下限
	float resistor_higher_limit_;//!< 电阻上限
	float resistor_value_; //!< 电阻值
	float tilt_limit_; //!< 倾斜度界限
	float tilt_value_; //!< 倾斜度值
	bool resistor_deviation_; //!< 电阻误差
	bool tilt_deviation_; //!< 倾斜度误差
}AD_DATA_FILE_HEAD_PRIVATE, *AD_DATA_FILE_HEAD__PRIVATE_PTR;

/**
@struct	__tagAD_DATA_FILE_INFO
@brief	AD数据文件头信息
@note	typedef struct __tagAD_DATA_FILE_INFO AD_DATA_FILE_INFO, *AD_DATA_FILE_INFO_PTR;
*/
typedef struct __tagAD_DATA_FILE_INFO
{
	HANDLE	file_handle_; //!< 数据文件句柄
	LARGE_INTEGER data_size_; //!< 数据大小;
	ULONG	frame_number_per_fdu_; //!< 每个设备采集帧数对于整个DAT文件
	UCHAR	manufacturer_code_;//!< 指定制造代码,sercel为13, 不注明时,通用使用0
	AD_DATA_FILE_HEAD_PRIVATE file_header_;
}AD_DATA_FILE_INFO, *AD_DATA_FILE_INFO_PTR;


/**
@fn int load_data_file(char_t const* data_path, AD_DATA_FILE_INFO_PTR ad_data_ptr);
@brief 读取数据文件
@param[in]		data_path	数据文件路径
@param[in/out]	ad_data_ptr	数据头文件结构
@return 如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int load_data_file(char_t const* data_path, AD_DATA_FILE_INFO_PTR ad_data_ptr);

/**
@fn	int modify_config_info(__in__ char_t const* config_path, const AD_DATA_FILE_INFO_PTR ad_data_ptr)
@details 通过数据文件信息修改配置文件
@param[in]	config_path	配置文件目录
@param[in]	ad_data_ptr	数据文件中的配置信息
@return	如果失败返回APP_FAULT,否则返回APP_SUCCESS
@note	
*/
int modify_config_info(__in__ char_t const* config_path, __in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr);
int modify_config_info(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr, __in_out__ VOID* segd_config_info_ptr);
/**
@fn	int get_trace_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr);
@details 得到跟踪道数量,其值为trace_total_number - 1
@param[in]	ad_data_ptr	数据文件中的配置信息
@return	返回跟踪道数量
@note	
*/
inline int get_total_trace_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr){return ad_data_ptr->file_header_.trace_total_number_;};
inline int get_aux_trace_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr)
	{return ad_data_ptr->file_header_.auxiliary_trace_number_ * ad_data_ptr->file_header_.auxiliary_line_number_;};
inline int get_trace_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr)
	{return get_total_trace_size(ad_data_ptr) - get_aux_trace_size(ad_data_ptr);};

/**
@fn	int get_data_frame_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr);
@details 数据采集文件中的数据帧数量
@param[in]	ad_data_ptr	数据文件中的配置信息
@return	返回当前数据帧数量
@note	
*/
inline int get_data_frame_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr){	return ad_data_ptr->frame_number_per_fdu_; };

/**
@fn	int get_data_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr);
@details 每个FDU采集的数据能放入SEGD文件中的数据大小
@param[in]	ad_data_ptr	数据文件中的配置信息
@return	返回每个FDU采集的数据能放入SEGD文件中的数据大小
@note	
*/
int get_data_size(__in__ const AD_DATA_FILE_INFO_PTR ad_data_ptr);

/**
@fn	int get_file_header_size()
@details 
@return	返回当前数据帧数量
@note	
*/
int get_file_header_size();

#endif //__PROC_DATA_FILE_H__