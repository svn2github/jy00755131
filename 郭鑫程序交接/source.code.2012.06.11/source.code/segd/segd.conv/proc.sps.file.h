/**
@brief	读取SPS文件,提取SEG-D使用参数
@details 打开响应的sps文件,然后处理相应的参数写入SEG-D配置中
@file	proc.sps.file.h
@note	实现见proc.sps.file.cpp
@see	有关SPS内容,参见SPS标准
@version	1.1.6.20.2011.09.14
@author	kuoxin	
@bug
*/

/**
@struct __tagSPS_POINT_RECORD_SPEC
@brief sps 点记录结构体
@note
	typedef struct __tagSPS_POINT_RECORD_SPEC SPS_POINT_RECORD_SPEC, *SPS_POINT_RECORD_SPEC_PTR;
*/
typedef struct __tagSPS_POINT_RECORD_SPEC
{
	unsigned	line_name_; //!< 测线名
	float		point_number_;//!< 测点号
	unsigned	index_;	//!< 震源点索引
	double		easting_;	//!< 震源偏东距	默认为 GUI 设置，或由放炮或导航系统更新
	double		northing_;	//!< 震源北向纬度差	默认为 GUI 设置，或由放炮或导航系统更新
	float		elevation_;	//!< 震源高程	默认为 GUI 设置，或由放炮或导航系统更新
	int			seismic_datum_;//!< 地震基准面
	short		uphole_time_;//!< 井口时间
	__tagSPS_POINT_RECORD_SPEC* next_; //!< 下一个结构体指针,便于使用链表
}SPS_POINT_RECORD_SPEC, *SPS_POINT_RECORD_SPEC_PTR;

/**
@struct __tagSPS_POINT_RECORD_SPEC
@brief sps 关系记录结构体
@note
	typedef struct __tagSPS_RELATION_RECORD_SPEC SPS_RELATION_RECORD_SPEC, *SPS_RELATION_RECORD_SPEC_PTR;
*/
typedef struct __tagSPS_RELATION_RECORD_SPEC
{
	int	field_record_number_;//!< 野外记录号
	int	field_record_increment_;//!< 野外记录增量
	int	instrument_code_;//!< 仪器代码
	int	from_channel_;//!< 起始记录道号
	int to_channel_;//!< 终止记录道号
	int	channel_increment_;//!< 道增量
	int	receiver_line_name_;//!< 接收线号（左对齐）
	int	from_receiver_;//!< 起始接收点号（右对齐）
	int	to_receiver_;//!< 终止接收点号（右对齐）
	int	receiver_index_;//!< 接收点索引
	int	receiver_point_size_;//!< 分配数据大小
	SPS_POINT_RECORD_SPEC_PTR	head_receiver_point_ptr_;//!< 接受点数据信息,该值为指针,内存的分配主要是由分析成完成,使用后,需要删除
	SPS_POINT_RECORD_SPEC_PTR	tail_receiver_point_ptr_;//!< 接受点数据信息,该值为指针,内存的分配主要是由分析成完成,使用后,需要删除
	__tagSPS_RELATION_RECORD_SPEC* next_; //!< 下一个结构体指针,便于使用链表
}SPS_RELATION_RECORD_SPEC, *SPS_RELATION_RECORD_SPEC_PTR;

/**
@struct	__tagSPS_DATA_INFO
@brief	保存SPS信息
@note
	使用到h信息:
	H402Sample int.,Record Len.     1,1MSEC,6SEC;                                   
	H403Number of channels          1,200;                                          
	H404Tape type,format,density    1,Cartridge,SEGD,5745171;                       
	H405Filter_alias Hz,dB pnt,slope1,None;                                         
	H406Filter_notch Hz,-3dB points 1,None;                                         
	H407Filter_low Hz,dB pnt,slope  1,0.0Hz,0.0DB,0.0DB/OCT;   
	H600Type,model,polarity         G1,G_Land,SN7C-10Hz,SEG;                        

	s文件中的点信息:
	S691              23329.51V1                   509494.8 4782327.81269.9
	
	r文件中的点信息:
	R691                233291G1                   509481.8 4782309.61269.4

	x文件中的点信息:
	X          11691              23329.51   1 6001691                23030   236291
*/
typedef struct __tagSPS_DATA_INFO
{
	__in__ char_t const* sps_path_ptr_; //!< sps文件路径
	__in__ float source_line_number_;	//!< 测线号
	__in__ float source_point_number_;	//!< 震源点号

	//来自Hxxx信息
	__out__ unsigned	base_scan_interval_; //!< 基本扫描间隔
	__out__ unsigned	extended_record_length_;//!< 扩展记录长度
	__out__ unsigned	alias_filter_frequency_; //!< 混叠滤波器频率 
	__out__ unsigned	alias_filter_slope_; //!< 混叠滤波器斜率 
	__out__ unsigned	low_cut_filter_;  //!< 低截滤波器频率
	__out__ unsigned	low_cut_filter_slope_;  //!< 低截滤波器斜率
	__out__ unsigned	sample_length_;			//!< 采集长度1000 到 128000毫秒
	__out__ unsigned	sample_frequency_;		//!< 采样率250、500、1000、2000、4000 微秒
	__out__ unsigned	sample_number_in_trace_;//!< 记录道中样本数目1 到 128000
	__out__ unsigned	listen_time_;			//!< 监听时间	100 到 128000 毫秒
	__out__ unsigned	scan_length_;			//!< 扫描长度	1000 到 128000 毫秒
	__out__ unsigned	trace_total_number_;	//!< 记录道总数1 到 100000
	__out__ unsigned	auxiliary_trace_number_;//!< 辅助道数目1 到 100000
	__out__ unsigned	trace_number_;			//!< 地震记录道数目1 到 100000

	__out__ SPS_POINT_RECORD_SPEC shot_point_;//!< 震源信息
	__out__ unsigned relation_records_size_; //!< 测线关系记录大小
	__out__ SPS_RELATION_RECORD_SPEC_PTR head_relation_records_ptr_; //!< 第一个测线关系记录指针
	__out__ SPS_RELATION_RECORD_SPEC_PTR tail_relation_records_ptr_; //!< 最后一个测线关系记录指针
}SPS_DATA_INFO, *SPS_DATA_INFO_PTR;

/**
@fn	int load_sps_file(__in_out__ SPS_DATA_INFO_PTR sps_data_ptr);
@brief	检查并加载配置文件信息到应用环境中
@param[in/out]	sps_data_ptr		从sps文件中得到的数据结构
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int load_sps_file(__in_out__ SPS_DATA_INFO_PTR sps_data_ptr);


/**
@fn	int free_sps_data_memory(__in__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 释放SPS_DATA_INFO数据结构分配的内存 	
@param[in/out]	sps_data_ptr	从sps文件中得到的数据结构
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
int free_sps_data_memory(__in__ SPS_DATA_INFO_PTR sps_data_ptr);
