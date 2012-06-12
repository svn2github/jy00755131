/**
@brief	SPS文件数据结构
@details 该文件中描述了有关SPS的数据结构信息
@file	sps.header.h
@note	
@see	有关SPS内容,参见SPS标准(Transcriber 2 SPS Format; e428XL V4.0 VOL.2, 
@version	1.0.0.1.2011.11.16
@author	kuoxin	
@bug
*/

/**
@struct __tagSPS_POINT_RECORD_SPEC
@brief SPS点记录结构用于写入SPS中的S和R文件,该数据为元数据结构
*/
typedef __tagSPS_POINT_RECORD_SPEC_PRIVATE
{
	char	record_id_;			//!< 1 Record identification 1 A1 'R' or 'S' None
	char	line_name_[16 + 1];		//!< 2 Line name (left adj) 2-17 16A1 Free None
	char	point_number_[8 + 1];	//!< 3 Point number (right adj)18-25 8A1 Free None -
	char	point_index_;		//!< 4 Point index 26 A1 '1' to 'A' '1' -
	char	point_code_[2 + 1];		//!< 5 Point code 27-28 A2 see below * None -
	char	static_correction_[4 + 1];	//!< 6 Static correction 29-32 I4 -999 - 999 Blank Msec
	char	point_depth_[4 + 1];		//!< 7 Point depth 33-36 F4.1 0 - 99.9 None Meter
	char	seismic_datum_[4 + 1];		//!< 8 Seismic datum 37-40 I4 -999 - 9999 None Meter
	char	uphole_[2 + 1];				//!< 9 Uphole time 41-42 I2 0 - 99 Blank Msec
	char	water_depth_[5 + 1];		//!< 10 Water depth 43-46 F4.1# 0 - 99.9/999 Blank Meter
	char	map_grid_easting_[9 + 1]	//!< 11 Map grid easting 47-55 F9.1 None -
	char	map_grid_northing_[10 + 1];	//!< 12 Map grid northing 56-65 F10.1 None -
	char	surface_elevation_[6 + 1];	//!< 13 Surface elevation 66-71 F6.1 -999.9 -9999.9 None Meter
	char	day_of_year_[3 + 1];		//!< 14 Day of year 72-74 I3 1 - 999 None -
	char	time_[6 + 1];				//!< 15 Time (hhmmss) 75-80 3I2 000000 - 235959 None
}SPS_POINT_RECORD_SPEC_PRIVATE, *SPS_POINT_RECORD_SPEC_PRIVATE_PTR;

/**
 * @typedef struct __tagSPS_POINT_RECORD_SPEC_PRIVATE SPS_POINT_RECORD_SPEC_PRIVATE, *SPS_POINT_RECORD_SPEC_PRIVATE_PTR;
 * @brief 定义点记录的原始数据结构
*/

/**
@struct __tagSPS_POINT_RECORD_SPEC
@brief sps 点记录结构体,该结构来自于__tagSPS_POINT_RECORD_SPEC_PRIVATE的分析
*/
typedef struct __tagSPS_POINT_RECORD_SPEC
{
	unsigned		line_name_; //!< 测线名
	float			point_number_;//!< 测点号
	unsigned char	index_;	//!< 震源点索引
	char			point_code_[3]; //!< 点代码
	int				static_correction_; //!< 静态纠正
	float			point_depth_; //!< 点深度
	int				seismic_datum_;//!< 地震数据
	unsigned short	uphole_time; //!< 井口时间
	float			water_depth_; //!< 水深
	double			easting_;	//!< 震源偏东距	默认为 GUI 设置，或由放炮或导航系统更新
	double			northing_;	//!< 震源北向纬度差	默认为 GUI 设置，或由放炮或导航系统更新
	float			elevation_;	//!< 震源高程	默认为 GUI 设置，或由放炮或导航系统更新
	int				day_of_year; //!< 儒略日 格式: I3
	unsigned short	time_[3]; //!< 时间,格式: hhmmss
//	__tagSPS_POINT_RECORD_SPEC* next_; //!< 下一个结构体指针,便于使用链表
}SPS_POINT_RECORD_SPEC, *SPS_POINT_RECORD_SPEC_PTR;

/**
 * @typedef struct __tagSPS_POINT_RECORD_SPEC SPS_POINT_RECORD_SPEC, *SPS_POINT_RECORD_SPEC_PTR;
 * @brief 定义点记录
*/

/**
@struct __tagSPS_RELATION_RECORD_SPEC_PRIVATE
@brief sps 关系记录结构体的原始数据结构
*/
typedef struct __tagSPS_RELATION_RECORD_SPEC_PRIVATE
{
	char	record_id_;							//!< 1 Record identification 1 A1 'X' None -
	char	field_tape_number_[6 + 1];			//!< 2 Field tape number 2-7 6A1 Free None -
	char	field_record_number_[4 + 1];		//!< 3 Field record number 8-11 I4 0 - 9999 None -
	unsigned char	field_record_increment_;	//4 Field record increment 12 A1 '1' to 'Z' '1' -
	unsigned char	instrument_code_;			//!< 5 Instrument code 13 A1 '1' to 'A' '1' -
	char	shot_line_name_[16 + 1];			//!< 6 Line name (left adj) 14-29 16A1 Free None -
	char	point_number_[8 + 1];				//!< 7 Point number (right adj) 30-37 8A1 Free None -
	char	point_index_;						//!< 8 Point index 38 A1 '1' to 'Z' '1' -
	char	from_channel_[4 + 1];				//!< 9 From channel 39-42 I4 0 - 9999 None -
	char	to_channel_[4 + 1];					//!< 10 To channel 43-46 I4 0 - 9999 None -
	char	channel_increment_;					//!< 11 Channel increment 47 I1 1-9 1 -
	char	receiver_line_name_[16 + 1];		//!< 12 Line name (left adj) 48-63 16A1 Free None -
	char	from_receiver_[8 + 1];				//!< 13 From receiver (right adj) 64-71 8A1 Free None -
	char	to_receiver_[8 + 1];				//!< 14 To receiver (right adj) 72-79 8A1 Free None -
	char	receiver_index_;					//!< 15 Receiver index 80 A1 '1' to 'A' '1' -
}SPS_RELATION_RECORD_SPEC_PRIVATE, *SPS_RELATION_RECORD_SPEC_PRIVATE_PTR;

/**
 * @typedef struct __tagSPS_RELATION_RECORD_SPEC_PRIVATE SPS_RELATION_RECORD_SPEC_PRIVATE, *SPS_RELATION_RECORD_SPEC_PRIVATE_PTR
 * @brief 定义sps关系记录元数据结构
*/

/**
@struct __tagSPS_RELATION_RECORD_SPEC
@brief sps关系记录结构体
*/
typedef struct __tagSPS_RELATION_RECORD_SPEC
{
	char	field_tape_number_[6 + 1]; //!< 区域磁带号
	int	field_record_number_;//!< 野外记录号
	int	field_record_increment_;//!< 野外记录增量
	int	instrument_code_;//!< 仪器代码
	char	shot_line_name_[16 + 1]; //!< 炮点线号名称	
	char	point_number_[8 + 1];	//!< 炮点号
	int	point_index_;				//!< 炮点索引
	int	from_channel_;				//!< 起始记录道号
	int to_channel_;				//!< 终止记录道号
	int	channel_increment_;			//!< 道增量
	char	receiver_line_name_[16 + 1];//!< 接收线号（左对齐）
	int	from_receiver_;//!< 起始接收点号（右对齐）
	int	to_receiver_;//!< 终止接收点号（右对齐）
	int	receiver_index_;//!< 接收点索引
//	int	receiver_point_size_;//!< 分配数据大小
//	SPS_POINT_RECORD_SPEC_PTR	head_receiver_point_ptr_;//!< 接受点数据信息,该值为指针,内存的分配主要是由分析成完成,使用后,需要删除
//	SPS_POINT_RECORD_SPEC_PTR	tail_receiver_point_ptr_;//!< 接受点数据信息,该值为指针,内存的分配主要是由分析成完成,使用后,需要删除
//	__tagSPS_RELATION_RECORD_SPEC* next_; //!< 下一个结构体指针,便于使用链表
}SPS_RELATION_RECORD_SPEC, *SPS_RELATION_RECORD_SPEC_PTR;

/**
 * @typedef struct __tagSPS_RELATION_RECORD_SPEC SPS_RELATION_RECORD_SPEC, *SPS_RELATION_RECORD_SPEC_PTR
 * @brief 定义sps关系记录结构体
*/

