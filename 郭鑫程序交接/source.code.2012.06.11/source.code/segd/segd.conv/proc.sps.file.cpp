/**
@brief	读取SPS文件,提取SEG-D使用参数
@details	打开相应的sps文件,然后处理相应的参数写入SEG-D配置文件中,
			sps文件包括后缀为x,r,s的三个文件分别从其中读取相关信息
			分析规范:
			//记录标识符“H”（1）头段记录类型（2-3）头段记录类型修改符（4）参数描述（5-32）参数数据（33-80）

			序号	定义段	列	格式	最小到最大	缺省	单位
			1	记录标识	1－1	A1	“R”或“S”	无	－
			2	测线名（左对齐）	2－17	4A4	自由	无	－
			3	点号（右对齐）	18－25	2A4	自由	无	－
			4	点索引	26－26	I1	1－9	1	－
			5	点代码*	27－28	A2	参见下面	无	－
			6	静态校正	29－32	I4	-999－999	空	毫秒
			7	点深度	33－36	F4.1	0－99.9	无	米
			8	地震基准面 	37－40	I4	-999－9999	无	米
			9	井口时间	41－42	I2	0－99	空	毫秒
			10	水深	43－46	F4.1#	0至99.9/999	空	米
			11	物理点横坐标 	47－55	F9.1		无	－
			12	物理点纵坐标 	56－65	F10.1		无	－
			13	地面高程	66－71	F6.1	-999.9－9999.9	无	米
			14	一年的日累计	72－74	I3	1－999	无	－
			15	时间hh mm ss（时分秒）	75－80	3I2	000000－235959	无	－

			序号	定义段	列	格式	最小到最大	缺省	单位
			1	记录标识	1－1	A1	“X”	无	－
			2	野外磁带号（左对齐）	2－7	3A2	自由	无	－
			3	野外记录号	8－11	I4	0－9999	无	－
			4	野外记录增量	12－12	I1	1－9	I	－
			5	仪器代码	13－13	A1	1－9	I	－
			6	测线名（左对齐）	14－29	4A4	没有定义	无	－
			7	点号（右对齐）	30－37	2A4	没有定义	无	－
			8	点索引	38－38	I1	1－9	I	－
			9	起始记录道号	39－42	I4	1－9999	无	―
			10	终止记录道号	43－46	I4	1－9999	无	－
			11	道增量	47－47	I1	1－9	I	－
			12	接收线号（左对齐）	48－63	4A4	没有定义	无	－
			13	起始接收点号（右对齐）	64－71	2A4	没有定义	无	－
			14	终止接收点号（右对齐）	72－79	2A4	没有定义	无	－
			15	接收点索引	80－80	I1	1－9	I	－

@file	proc.sps.file.cpp
@note	
@see	有关SPS内容,参见SPS规范
@version	1.1.11.33.2011.09.26
@author	kuoxin	
@bug
	1.1.11.33.2011.09.26	初始化版本
	1.1.12.40.2011.09.28	加入sps支持,对应震源信息可以查询出接受测点信息
*/

#include "segd.conv.h"
#include "proc.sps.file.h"

/**
@struct	__tagSPS_HEADER_RECORD_SPEC
@brief	sps文件头记录的描述信息,该信息可以出现在文件中也可以忽略,并且在r,x,s文件中,描述都相同
@note	typedef struct __tagSPS_HEADER_RECORD_SPEC SPS_HEADER_RECORD_SPEC, *SPS_HEADER_RECORD_SPEC_PTR
*/
typedef struct __tagSPS_HEADER_RECORD_SPEC
{
	char id_;	//!< 记录标识符“H”
	short type_;	//!< 头段记录类型
	int	 modifier_; //!< 头段记录类型修改符
	char param_descrip_[28 + 1]; //!< 参数描述
	char param_data_[48 + 1]; //!< 参数数据
}SPS_HEADER_RECORD_SPEC, *SPS_HEADER_RECORD_SPEC_PTR;

/**
@struct __tagSPS_POINT_RECORD_SPEC
@brief sps 点记录结构体
@note
	typedef struct __tagSPS_POINT_RECORD_SPEC SPS_POINT_RECORD_SPEC, *SPS_POINT_RECORD_SPEC_PTR;
*/
typedef struct __tagSPS_POINT_RECORD_SPEC_PRIVATE
{
	char	id_[2];//!< 记录标识	1－1	A1	“R”或“S”	无	－
	char	line_name_[16 + 1];//!< 测线名（左对齐）	2－17	4A4	自由	无	－
	char	point_number_[8 + 1];//!< 点号（右对齐）	18－25	2A4	自由	无	－
	char	point_index_[1 + 1];//!< 点索引	26－26	I1	1－9	1	－
	char	point_code_[2 + 1];//!< 点代码*	27－28	A2	参见下面	无	－
	int		static_correction_;//!< 静态校正	29－32	I4	-999－999	空	毫秒
	float	point_depth_;//!< 点深度	33－36	F4.1	0－99.9	无	米
	int		seismic_datum_;//!< 地震基准面 	37－40	I4	-999－9999	无	米
	short	uphole_time_;//!< 井口时间	41－42	I2	0－99	空	毫秒
	float	water_depth_;//!< 水深	43－46	F4.1#	0至99.9/999	空	米
	double	map_grid_easting_;//!< 物理点横坐标 	47－55	F9.1		无	－
	double	map_grid_northing_;//!< 物理点纵坐标 	56－65	F10.1		无	－
	float	surface_elevation_;//!< 地面高程	66－71	F6.1	-999.9－9999.9	无	米
	int		day_of_year_;//!< 一年的日累计	72－74	I3	1－999	无	－
	short	time_[3];//!< 时间hh mm ss（时分秒）	75－80	3I2	000000－235959	无	－
}SPS_POINT_RECORD_SPEC_PRIVATE, *SPS_POINT_RECORD_SPEC_PRIVATE_PTR;

/**
@struct __tagSPS_POINT_RECORD_SPEC
@brief sps 关系记录结构体
@note
	typedef struct __tagSPS_RELATION_RECORD_SPEC SPS_RELATION_RECORD_SPEC, *SPS_RELATION_RECORD_SPEC_PTR;
*/
typedef struct __tagSPS_RELATION_RECORD_SPEC_PRIVATE
{
	char	id_[1 + 1];//!< 记录标识	1－1	A1	“X”	无	－
	char	field_tape_number_[6 + 1];//!< 野外磁带号（左对齐）	2－7	3A2	自由	无	－
	int	field_record_number_;//!< 野外记录号	8－11	I4	0－9999	无	－
	int	field_record_increment_;//!< 野外记录增量	12－12	I1	1－9	I	－
	int	instrument_code_;//!< 仪器代码	13－13	A1	1－9	I	－
	char	source_line_name_[16 + 1];//!< 测线名（左对齐）	14－29	4A4	没有定义	无	－
	char	source_point_number_[8 + 1];//!< 点号（右对齐）	30－37	2A4	没有定义	无	－
	int	source_point_index_;//!< 点索引	38－38	I1	1－9	I	－
	int	from_channel_;//!< 起始记录道号	39－42	I4	1－9999	无	―
	int to_channel_;//!< 终止记录道号	43－46	I4	1－9999	无	－
	int	channel_increment_;//!< 道增量	47－47	I1	1－9	I	－
	char	receiver_line_name_[16 + 1];//!< 接收线号（左对齐）	48－63	4A4	没有定义	无	－
	char	from_receiver_[8 + 1];//!< 起始接收点号（右对齐）	64－71	2A4	没有定义	无	－
	char	to_receiver_[8 + 1];//!< 终止接收点号（右对齐）	72－79	2A4	没有定义	无	－
	int	receiver_index_;//!< 接收点索引	80－80	I1	1－9	I	－
}SPS_RELATION_RECORD_SPEC_PRIVATE, *SPS_RELATION_RECORD_SPEC_PRIVATE_PTR;

/**
@struct __tagFUNCTION_PARAM
@brief
@note typedef struct __tagFUNCTION_PARAM FUNCTION_PARAM, *FUNCTION_PARAM_PTR;
*/
typedef struct __tagFUNCTION_PARAM
{
	int								receiver_line_name_; //!< 接受测线名
	float							receive_point_num_;//!< 接受点号
	SPS_RELATION_RECORD_SPEC_PTR	relation_record_ptr_;//!< 关系数据结构的指针
}FUNCTION_PARAM, *FUNCTION_PARAM_PTR;

/**
@typedef bool (*ASSIGN_PARAM_CALLBACK)(__in__ char const* line, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@detail 定义回调函数用于处理读入文件中的每一行信息
@param[in] line 一行字符串缓冲
@param[in/out] sps_data_ptr 保存相应参数信息
@return 如果成功返回true, 否则返回false
@note	当前处理的文件都是char字符
*/
typedef bool (*ASSIGN_PARAM_CALLBACK)(__in__ char const* line, __in_out__ void* data_ptr);

/**
@struct	__tagSPS_FILE_LINE
@brief	保存从SPS文件读取的每一行信息
@note	该类主要用于保存从SPS读取的信息,该数据数据结构保存以文本文件方式的SPS文件(S,r,x扩展名)的自然行信息
		该结构只支持以ANSI格式保存的文本文件,并且最大一行的长度不能超过MAX_STRING_SIZE
*/
typedef struct __tagSPS_FILE_LINE
{
	char line_[MAX_STRING_SIZE];	//!< 一行信息
	size_t size_;					//!< 一行大小
	__tagSPS_FILE_LINE* next_;		//指向下一行信息结构体指针
}SPS_FILE_LINE, *SPS_FILE_LINE_PTR;

/**
@fn bool proc_lines(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr, __in__ ASSIGN_PARAM_CALLBACK callback_function)
@detail 对于打开的文件进行读取,并按照行方式调用callback函数
@param[in]		sps_file_line_ptr	保存sps文件
@param[in/out]	data_ptr	保存相应参数信息
@param[in]		callback_function	行处理回调函数
@return 如果成功返回true, 否则返回false
@note	当前处理的文件都是char字符
*/
bool proc_lines(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr, __in__ ASSIGN_PARAM_CALLBACK callback_function)
{
	_ASSERTE(sps_file_line_ptr != NULL);

	bool is_ok = true;
	SPS_FILE_LINE_PTR tmp_ptr = sps_file_line_ptr;

	while(tmp_ptr != NULL){
		//parse a option
		if(!(callback_function(tmp_ptr->line_, data_ptr))) break;
		tmp_ptr = tmp_ptr->next_;
	}

	return is_ok;
}

/**
@fn bool H402_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H402标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H402Sample int.,Record Len.     1,1MSEC,6SEC;                                   
*/
bool H402_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	bool is_ok = true;
	char* context = NULL;
	char* token = NULL;
	char* tmp_ptr = NULL;
	
	token = strtok_s(header_record_spec->param_data_, ",", &context);
	while(token != NULL){
		
		tmp_ptr = StrStrA(token, "MSEC");
		if(tmp_ptr != NULL){
			
			token[tmp_ptr - token] = NULL;
			sps_data_ptr->sample_frequency_ = static_cast<int>(atof(token) * 1000);
			switch((int)(atof(token)*100)){
				case 25: {sps_data_ptr->base_scan_interval_ = 4; break;}
				case 50: {sps_data_ptr->base_scan_interval_ = 8; break;}
				case 100: {sps_data_ptr->base_scan_interval_ = 10; break;}
				case 200: {sps_data_ptr->base_scan_interval_ = 20; break;}
				case 400: {sps_data_ptr->base_scan_interval_ = 40; break;}
			}
		}
		else{
			tmp_ptr = StrStrA(token, "SEC");
			if(tmp_ptr != NULL){
				token[tmp_ptr - token] = NULL;
				sps_data_ptr->sample_length_ = atoi(token) * 1000;	
				sps_data_ptr->extended_record_length_ = sps_data_ptr->sample_length_;
				sps_data_ptr->listen_time_ = sps_data_ptr->extended_record_length_;
				sps_data_ptr->sample_number_in_trace_ = sps_data_ptr->sample_length_;
			}
		}

		token = strtok_s(NULL, ",", &context);
	}
	return is_ok; 
}

/**
@fn bool H403_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H403标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H403Number of channels          1,200;                                          
*/
bool H403_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	header_record_spec = header_record_spec;
	sps_data_ptr = sps_data_ptr;
	return true; 
}

/**
@fn bool H404_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H404标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H404Tape type,format,density    1,Cartridge,SEGD,5745171;                       
*/
bool H404_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	header_record_spec = header_record_spec;
	sps_data_ptr = sps_data_ptr;
	return true; 
}

/**
@fn bool H405_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H405标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H405Filter_alias Hz,dB pnt,slope1,None;                                         
*/
bool H405_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	bool is_ok = true;
	char* context = NULL;
	char* token = NULL;
	int num = 0x0;
	double tmp_value = 0x0;

	token = strtok_s(header_record_spec->param_data_, ",", &context);
	while(token != NULL){

		if(num == 0){ ++num;}
		else{
			StrTrimA(token, " ;");
			if(num == 1){
				if(StrCmpA(token, "None") == 0){ break;}
				else{
					tmp_value = atof(token);
				}
			}
			else{ //2,3...
				if(num == 2){
					if(StrCmpA(token, "3DB") == 0){
						switch((int)tmp_value * 100){
						case 25:	sps_data_ptr->alias_filter_frequency_ = 1600;	break;
						case 50:	sps_data_ptr->alias_filter_frequency_ = 800;	break;
						case 100:	sps_data_ptr->alias_filter_frequency_ = 400;	break;
						case 200:	sps_data_ptr->alias_filter_frequency_ = 200;	break;
						case 400:	sps_data_ptr->alias_filter_frequency_ = 100;	break;
						}
					}
				}
			}
		}
		token = strtok_s(NULL, ",", &context);
	}
	return is_ok; 
}

/**
@fn bool H406_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H406标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H406Filter_notch Hz,-3dB points 1,None;                                         
*/
bool H406_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	header_record_spec = header_record_spec;
	sps_data_ptr = sps_data_ptr;
	return true; 
}

/**
@fn bool H407_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H407标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H407Filter_low Hz,dB pnt,slope  1,0.0Hz,0.0DB,0.0DB/OCT;   
*/
bool H407_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	bool is_ok = true;
	char* context = NULL;
	char* token = NULL;
	char* tmp_ptr = NULL;
	int num = 0x0;

	token = strtok_s(header_record_spec->param_data_, ",", &context);
	while(token != NULL){
		if(num == 0){ ++num;}
		else{
			StrTrimA(token, " ;");
			tmp_ptr = StrStrA(token, "Hz");
			if(tmp_ptr != NULL){
				token[tmp_ptr-token] = NULL;
				sps_data_ptr->low_cut_filter_ = atoi(token);
			}
			else{
				tmp_ptr = StrStrA(token, "DB/OCT");
				if(tmp_ptr != NULL){
					token[tmp_ptr-token] = NULL;
					sps_data_ptr->low_cut_filter_slope_ = atoi(token);				
				}
			}			
		}
		token = strtok_s(NULL, ",", &context);
	}
	return is_ok; 
}

/**
@fn bool H600_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
@brief 处理H402标识
@param[in]	header_record_spec	sps头段记录
@param[in/out]	sps_data_ptr	保存到sps数据结构中
@return 如果成功返回true, 否则返回false
@note
H600Type,model,polarity         G1,G_Land,SN7C-10Hz,SEG;                        
*/
bool H600_function(__in__ SPS_HEADER_RECORD_SPEC_PTR header_record_spec, __in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	header_record_spec = header_record_spec;
	sps_data_ptr = sps_data_ptr;
	return true; 
}

/**
@fn bool proc_header_record_callback(char * line, __SPS_DATA_INFO_PTR sps_data_ptr)
@detail callback函数
@note	ASSIGN_PARAM_CALLBACK
*/
bool proc_header_record_callback(__in__ char const* line, __in__ void* data_ptr)
{
	bool is_ok = true;
	SPS_DATA_INFO_PTR sps_data_ptr = (SPS_DATA_INFO_PTR)data_ptr;
	char tmp_buff[MAX_STRING_SIZE] = {0};
	SPS_HEADER_RECORD_SPEC header_rec;

	__matrix_zero_Memory(&header_rec, sizeof(header_rec));

	header_rec.id_ = line[0];

	__matrix_zero_Memory(tmp_buff, sizeof(tmp_buff));
	CopyMemory(tmp_buff, line+1, 2);
	header_rec.type_ = atoi(tmp_buff);

	__matrix_zero_Memory(tmp_buff, sizeof(tmp_buff));
	tmp_buff[0] = line[3];
	header_rec.modifier_ = atoi(tmp_buff);

	CopyMemory(header_rec.param_descrip_, line + 4, 28);
	CopyMemory(header_rec.param_data_, line + 32, 48);

	if(header_rec.type_ == 40){
		switch(header_rec.modifier_){
		case 2: H402_function(&header_rec, sps_data_ptr); break;
		case 3: H403_function(&header_rec, sps_data_ptr); break;
		case 4: H404_function(&header_rec, sps_data_ptr); break;
		case 5: H405_function(&header_rec, sps_data_ptr); break;
		case 6: H406_function(&header_rec, sps_data_ptr); break;
		case 7: H407_function(&header_rec, sps_data_ptr); break;
		}
	}
	else if(header_rec.type_ == 60){
		if(header_rec.modifier_ == 0) H600_function(&header_rec, sps_data_ptr);
	}

	return is_ok; 
}


/**
@fn	int read_hxxx_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
@details	读取Hxxx信息
@param[in]		sps_file_line_ptr	sps行信息链表
@param[in/out]	sps_data_ptr	从sps文件中得到的数据结构
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
	将读取:
	H402Sample int.,Record Len.     1,1MSEC,6SEC;                                   
	H403Number of channels          1,200;                                          
	H404Tape type,format,density    1,Cartridge,SEGD,5745171;                       
	H405Filter_alias Hz,dB pnt,slope1,None;                                         
	H406Filter_notch Hz,-3dB points 1,None;                                         
	H407Filter_low Hz,dB pnt,slope  1,0.0Hz,0.0DB,0.0DB/OCT;   
	H600Type,model,polarity         G1,G_Land,SN7C-10Hz,SEG;                        
*/
int read_hxxx_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
{
	int is_ok = APP_SUCCESS;

	if(!proc_lines(sps_file_line_ptr, data_ptr, proc_header_record_callback))	is_ok = APP_FAULT;

	return is_ok;
}

bool s_function(__in__ char const* line, __in_out__ void* data_ptr)
{
	bool is_ok = true;
	SPS_POINT_RECORD_SPEC_PRIVATE point_record;
	SPS_DATA_INFO_PTR sps_data_ptr = (SPS_DATA_INFO_PTR)data_ptr;
	float tmp_value = 0x0;
	char tmp_buf[80 + 1] = {0};

	__matrix_zero_Memory(&point_record, sizeof(point_record));

	point_record.id_[0] = line[0];

	CopyMemory(point_record.line_name_, line+1, sizeof(point_record.line_name_) - 1);
//	StrTrimA(point_record.line_name_, " ");

	tmp_value = atof(point_record.line_name_);
	if(tmp_value != sps_data_ptr->source_line_number_) goto GOTO;

	CopyMemory(point_record.point_number_, line + 17, sizeof(point_record.point_number_) - 1);
//	StrTrimA(point_record.point_number_, " ");
	tmp_value = atof(point_record.point_number_);
	if(tmp_value != sps_data_ptr->source_point_number_) goto GOTO;

	point_record.point_index_[0] = line[25];
	CopyMemory(point_record.point_code_, line + 26, sizeof(point_record.point_code_) - 1);

		
	CopyMemory(tmp_buf, line + 28, sizeof(point_record.static_correction_));
//	StrTrimA(tmp_buf, " ");
	point_record.static_correction_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 32, sizeof(point_record.point_depth_));
//	StrTrimA(tmp_buf, " ");
	point_record.point_depth_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 36, sizeof(point_record.seismic_datum_));
//	StrTrimA(tmp_buf, " ");
	point_record.seismic_datum_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 40, sizeof(point_record.uphole_time_));
//	StrTrimA(tmp_buf, " ");
	point_record.uphole_time_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 42, sizeof(point_record.water_depth_));
//	StrTrimA(tmp_buf, " ");
	point_record.water_depth_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 46, 9);
//	StrTrimA(tmp_buf, " ");
	point_record.map_grid_easting_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 55, 10);
//	StrTrimA(tmp_buf, " ");
	point_record.map_grid_northing_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 65, 6);
//	StrTrimA(tmp_buf, " ");
	point_record.surface_elevation_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 71, 3);
//	StrTrimA(tmp_buf, " ");
	point_record.day_of_year_ = atof(tmp_buf);

	sps_data_ptr->shot_point_.index_ = atoi(point_record.point_index_);
	sps_data_ptr->shot_point_.uphole_time_ = point_record.uphole_time_;
	sps_data_ptr->shot_point_.easting_ = point_record.map_grid_easting_;
	sps_data_ptr->shot_point_.northing_ = point_record.map_grid_northing_;
	sps_data_ptr->shot_point_.elevation_ = point_record.surface_elevation_;
	sps_data_ptr->shot_point_.seismic_datum_ = point_record.seismic_datum_;
	sps_data_ptr->shot_point_.point_number_ = atoi(point_record.point_number_);
	sps_data_ptr->shot_point_.line_name_ = atoi(point_record.line_name_);
GOTO:
	return is_ok; 
}

/**
@fn	int read_shot_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
@details	读取Hxxx信息
@param[in]		sps_file_line_ptr	sps行信息链表
@param[in/out]	sps_data_ptr	从sps文件中得到的数据结构
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
	s文件中的点信息:
	S691              23329.51V1                   509494.8 4782327.81269.9
*/
int read_shot_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
{
	int is_ok = APP_SUCCESS;

	if(!proc_lines(sps_file_line_ptr, data_ptr, s_function))	is_ok = APP_FAULT;
	return is_ok;
}

bool r_function(__in__ char const* line, __in_out__ void* data_ptr)
{
	bool is_ok = true;
	SPS_POINT_RECORD_SPEC_PRIVATE point_record;
	FUNCTION_PARAM_PTR param_ptr = (FUNCTION_PARAM_PTR)data_ptr;
	SPS_RELATION_RECORD_SPEC_PTR relation_record_ptr = param_ptr->relation_record_ptr_;
	float tmp_value = 0x0;
	char tmp_buf[80 + 1];
	SPS_POINT_RECORD_SPEC_PTR receive_record_ptr = NULL;

	memset(&point_record, 0x0, sizeof(point_record));

	point_record.id_[0] = line[0];

	CopyMemory(point_record.line_name_, line+1, sizeof(point_record.line_name_) - 1);

//	StrTrimA(point_record.line_name_, " ");

	tmp_value = atof(point_record.line_name_);
	if(tmp_value != param_ptr->receiver_line_name_) goto GOTO;

	CopyMemory(point_record.point_number_, line + 17, sizeof(point_record.point_number_) - 1);
	//StrTrimA(point_record.point_number_, " ");
	tmp_value = atof(point_record.point_number_);
	if(tmp_value != param_ptr->receive_point_num_) goto GOTO;

	point_record.point_index_[0] = line[25];
	CopyMemory(point_record.point_code_, line + 26, sizeof(point_record.point_code_) - 1);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 28, sizeof(point_record.static_correction_));
	//StrTrimA(tmp_buf, " ");
	point_record.static_correction_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 32, sizeof(point_record.point_depth_));
	//StrTrimA(tmp_buf, " ");
	point_record.point_depth_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 36, sizeof(point_record.seismic_datum_));
	//StrTrimA(tmp_buf, " ");
	point_record.seismic_datum_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 40, sizeof(point_record.uphole_time_));
	//StrTrimA(tmp_buf, " ");
	point_record.uphole_time_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 42, sizeof(point_record.water_depth_));
	//StrTrimA(tmp_buf, " ");
	point_record.water_depth_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 46, 9);
	//StrTrimA(tmp_buf, " ");
	point_record.map_grid_easting_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 55, 10);
	//StrTrimA(tmp_buf, " ");
	point_record.map_grid_northing_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 65, 6);
	//StrTrimA(tmp_buf, " ");
	point_record.surface_elevation_ = atof(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 71, 3);
	//StrTrimA(tmp_buf, " ");
	point_record.day_of_year_ = atof(tmp_buf);

	receive_record_ptr = (SPS_POINT_RECORD_SPEC_PTR)malloc(sizeof(SPS_POINT_RECORD_SPEC));
	if(receive_record_ptr == NULL) {is_ok = false; goto GOTO;}
	__matrix_zero_Memory(receive_record_ptr, sizeof(SPS_POINT_RECORD_SPEC));

	if(relation_record_ptr->head_receiver_point_ptr_ == NULL)	relation_record_ptr->head_receiver_point_ptr_ = receive_record_ptr;
	
	if(relation_record_ptr->tail_receiver_point_ptr_ != NULL){	relation_record_ptr->tail_receiver_point_ptr_->next_ = receive_record_ptr;}
	relation_record_ptr->tail_receiver_point_ptr_ = receive_record_ptr;
	++relation_record_ptr->receiver_point_size_;

	receive_record_ptr->easting_ = point_record.map_grid_easting_;
	receive_record_ptr->elevation_ = point_record.surface_elevation_;
	receive_record_ptr->index_ = atoi(point_record.point_index_);
	receive_record_ptr->line_name_ = atoi(point_record.line_name_);
	receive_record_ptr->northing_ = point_record.map_grid_northing_;
	receive_record_ptr->point_number_ = atoi(point_record.point_number_);
	receive_record_ptr->seismic_datum_ = point_record.seismic_datum_;
	receive_record_ptr->uphole_time_ = point_record.uphole_time_;
GOTO:
	return is_ok; 
}

/**
@fn	int read_receiver_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
@details	读取Hxxx信息
@param[in]		sps_file_line_ptr	sps行信息链表
@param[in/out]	sps_data_ptr		从sps文件中得到的数据结构
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
	r文件中的点信息:
	R691                233291G1                   509481.8 4782309.61269.4
*/
int read_receiver_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
{
	int is_ok = APP_SUCCESS;

	if(!proc_lines(sps_file_line_ptr, data_ptr, r_function))	is_ok = APP_FAULT;
	return is_ok;
}

bool x_function(__in__ char const* line, __in_out__ void* data_ptr)
{
	bool is_ok = true;
	SPS_DATA_INFO_PTR sps_data_ptr = (SPS_DATA_INFO_PTR)data_ptr;
	char tmp_buf[80 + 1] = {0};
	int tmp_value = 0x0;
	float tmp_float_value = 0x0;
	SPS_RELATION_RECORD_SPEC_PRIVATE relation_record;
	SPS_RELATION_RECORD_SPEC_PTR relation_record_ptr = NULL;

	__matrix_zero_Memory(&relation_record, sizeof(relation_record));

	relation_record.id_[0] = line[0];

	CopyMemory(relation_record.field_tape_number_, line + 1, sizeof(relation_record.field_tape_number_) - 1);
	CopyMemory(tmp_buf, line + 7, sizeof(relation_record.field_record_number_));
//	StrTrimA(tmp_buf, " ");
	relation_record.field_record_number_ = atoi(tmp_buf);
	
	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	tmp_buf[0] = line[11];
	relation_record.field_record_increment_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	tmp_buf[0] = line[12];
	relation_record.instrument_code_ = atoi(tmp_buf);

	CopyMemory(relation_record.source_line_name_, line + 13, sizeof(relation_record.source_line_name_) - 1);
//	StrTrimA(relation_record.source_line_name_, " ");
	tmp_value = atoi(relation_record.source_line_name_);
	if(tmp_value != sps_data_ptr->source_line_number_) goto GOTO;

	CopyMemory(relation_record.source_point_number_, line + 29, sizeof(relation_record.source_point_number_) - 1);
//	StrTrimA(relation_record.source_point_number_, " ");
	tmp_float_value = atof(relation_record.source_point_number_);
	if(tmp_float_value != sps_data_ptr->source_point_number_) goto GOTO;

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	tmp_buf[0] = line[37];
	relation_record.source_point_index_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 38, sizeof(int));
	relation_record.from_channel_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	CopyMemory(tmp_buf, line + 42, sizeof(int));
	relation_record.to_channel_ = atoi(tmp_buf);

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	tmp_buf[0] = line[46];
	relation_record.channel_increment_ = atoi(tmp_buf);

	CopyMemory(relation_record.receiver_line_name_, line + 47, sizeof(relation_record.receiver_line_name_) - 1);
//	StrTrimA(relation_record.receiver_line_name_, " ");

	CopyMemory(relation_record.from_receiver_, line + 63, sizeof(relation_record.from_receiver_) - 1);
//	StrTrimA(relation_record.from_receiver_, " ");

	CopyMemory(relation_record.to_receiver_, line + 71, sizeof(relation_record.to_receiver_) - 1);
//	StrTrimA(relation_record.to_receiver_, " ");

	__matrix_zero_Memory(tmp_buf, sizeof(tmp_buf));
	tmp_buf[0] = line[79];
	relation_record.receiver_index_ = atoi(tmp_buf);

	relation_record_ptr = (SPS_RELATION_RECORD_SPEC_PTR)malloc(sizeof(SPS_RELATION_RECORD_SPEC));
	if(!relation_record_ptr) {is_ok = false; goto GOTO;}
	__matrix_zero_Memory(relation_record_ptr, sizeof(SPS_RELATION_RECORD_SPEC));

	if(sps_data_ptr->head_relation_records_ptr_ == NULL)	sps_data_ptr->head_relation_records_ptr_ = relation_record_ptr;

	if(sps_data_ptr->tail_relation_records_ptr_ == NULL){
		sps_data_ptr->tail_relation_records_ptr_ = relation_record_ptr;
	}
	else{
		sps_data_ptr->tail_relation_records_ptr_->next_ = relation_record_ptr;
		sps_data_ptr->tail_relation_records_ptr_ = relation_record_ptr;
	}

	++sps_data_ptr->relation_records_size_;

	relation_record_ptr->channel_increment_ = relation_record.channel_increment_;
	relation_record_ptr->field_record_increment_ = relation_record.field_record_increment_;
	relation_record_ptr->field_record_number_ = relation_record.field_record_number_;
	relation_record_ptr->from_channel_ = relation_record.from_channel_;
	relation_record_ptr->from_receiver_ = atoi(relation_record.from_receiver_);
	relation_record_ptr->instrument_code_ = relation_record.instrument_code_;
	relation_record_ptr->receiver_index_ = relation_record.receiver_index_;
	relation_record_ptr->receiver_line_name_ = atoi(relation_record.receiver_line_name_);
	relation_record_ptr->receiver_point_size_ = 0;
	relation_record_ptr->to_channel_ = relation_record.to_channel_;
	relation_record_ptr->to_receiver_ = atoi(relation_record.to_receiver_);

GOTO:
	return is_ok; 
}

/**
@fn	int read_cross_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, SPS_DATA_INFO_PTR sps_data_ptr)
@details	读取Hxxx信息
@param[in]		sps_file_line_ptr	sps行信息链表
@param[in/out]	sps_data_ptr	从sps文件中得到的数据结构
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
	x文件中的点信息:
	X          11691              23329.51   1 6001691                23030   236291
*/
int read_cross_option(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr, __in_out__ void* data_ptr)
{
	int is_ok = APP_SUCCESS;

	if(!proc_lines(sps_file_line_ptr, data_ptr, x_function))	is_ok = APP_FAULT;
	return is_ok;
}

/**
@fn	void free_sps_line_list(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr)
@details	释放读取的sps信息链表
@param[in]	sps_file_line_ptr	将从这个参数节点开始释放链表信息
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note
*/
void free_sps_line_list(__in__ SPS_FILE_LINE_PTR sps_file_line_ptr)
{
	if(sps_file_line_ptr == NULL) return;
	SPS_FILE_LINE_PTR tmp_ptr = sps_file_line_ptr;
	SPS_FILE_LINE_PTR tmp_ptr2 = NULL;

	while(tmp_ptr != NULL){
		tmp_ptr2 = tmp_ptr->next_;
		tmp_ptr->next_ = NULL;
		free(tmp_ptr);
		tmp_ptr = tmp_ptr2;
	}
}

/**
@fn	int read_lines(__in__ HANDLE file_handle, __in_out__ SPS_FILE_LINE_PTR sps_file_line_ptr)
@details	按行读取已经打开的SPS文件,将行信息保存到链表中
@param[in]	file_handle	打开的文件句柄,文件可以是X,R,S文件
@param[in/out]	sps_file_line_ptr	保存在该链表中
@return	如果成功返回APP_SUCCESS, 否则返回APP_FAULT
@note	当前使用编码为ANSI格式
*/
int read_lines(__in__ HANDLE file_handle, __in_out__ SPS_FILE_LINE_PTR sps_file_line_ptr)
{
	int is_ok = APP_SUCCESS;
	char buf[MAX_STRING_SIZE] = {0};
	char line[MAX_STRING_SIZE] = {0};
	DWORD size = 0x0, i = 0, cur_line_size = 0x0;
	SPS_FILE_LINE_PTR tmp_ptr = sps_file_line_ptr;
	SPS_FILE_LINE_PTR tmp_ptr2 = NULL;

	if(sps_file_line_ptr == NULL){	set_last_error(IDS_STRING5043);	output_log(IDS_STRING5043);	is_ok = APP_FAULT; 	goto GOTO;}

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);
	while(ReadFile(file_handle, buf, sizeof(buf), &size,NULL)){

		if(size == 0) break;
		for(i = 0; i < size; ++i){
			if(buf[i] == 0x0d){

				if(cur_line_size == 0) continue;
				StrTrimA(line, " 	");
				cur_line_size = strlen(line);
				if(cur_line_size == 0) continue;

				//save this line
				StrCpyA(tmp_ptr->line_, line);
				tmp_ptr->size_ = cur_line_size;
				tmp_ptr->line_[cur_line_size] = NULL;
				if(tmp_ptr->next_ == NULL){
					tmp_ptr->next_ = (SPS_FILE_LINE_PTR)malloc(sizeof(SPS_FILE_LINE));
					if(tmp_ptr->next_ == NULL){
						set_last_error(IDS_STRING5042); 
						is_ok = APP_FAULT;	goto GOTO;
					}
					__matrix_zero_Memory(tmp_ptr->next_, sizeof(SPS_FILE_LINE));
				}

				cur_line_size = 0x0;
				__matrix_zero_Memory(line, sizeof(line));
				tmp_ptr2 = tmp_ptr;
				tmp_ptr = tmp_ptr->next_;
			}
			else{
				if(buf[i] != 0xa){	line[cur_line_size++] = buf[i];}
			}
		}
	}

	//释放多余的行结构信息
	if(tmp_ptr != NULL){	if(tmp_ptr2)	tmp_ptr2->next_ = NULL;}
	free_sps_line_list(tmp_ptr);
GOTO:
	return is_ok;
}

int load_sps_file(__in_out__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	int is_ok = APP_SUCCESS;
	int i = 0, loop = 0x0;
	size_t size = 0;
	HANDLE file_handle = INVALID_HANDLE_VALUE;
	char_t* tmp_ptr = NULL;
	errno_t err_no = 0;
	char_t tmp_value[MAX_STRING_SIZE] = {0};
	char_t path[MAX_STRING_SIZE] = {0};
	SPS_RELATION_RECORD_SPEC_PTR relation_record_ptr = NULL;
	FUNCTION_PARAM param;
	SPS_FILE_LINE_PTR sps_file_line_ptr = NULL;

	size = __matrix_strnlen_s(sps_data_ptr->sps_path_ptr_, MAX_STRING_SIZE);
	if(size == 0){
		set_last_error(IDS_STRING5024);	output_log(IDS_STRING5024);	
		is_ok = APP_FAULT; 	goto GOTO;
	}

	CopyMemory(path, sps_data_ptr->sps_path_ptr_, size * sizeof(path[0]));

	tmp_ptr = StrStr(path, __TEXT("."));
	if(tmp_ptr != NULL){//在传入路径中发现有可能存在文件后缀
		__matrix_zero_Memory(tmp_value, sizeof(tmp_value));
		err_no = __matrix_splitpath_s(sps_data_ptr->sps_path_ptr_, NULL, 0, NULL,0,
									NULL, 0, tmp_value, sizeof(tmp_value)/sizeof(tmp_value[0]));
		if(err_no != 0){
			set_last_error(IDS_STRING5025);	output_log(IDS_STRING5025);
			is_ok = APP_FAULT; 	goto GOTO;			
		}
		
		size = __matrix_strnlen_s(tmp_value, sizeof(tmp_value)/sizeof(tmp_value[0]));
		if(size != 0){//去除文件后缀
			tmp_ptr = StrStr(path, tmp_value);
			size = tmp_ptr - tmp_value + 1;
			path[size] = NULL;
		}
	}

	//打开s文件
	__matrix_zero_Memory(tmp_value, sizeof(tmp_value));
	size = __matrix_sprintf_s(tmp_value, __TEXT("%s.s"), path);
	tmp_value[size] = 0x0; 
	file_handle = CreateFile(tmp_value, GENERIC_READ, FILE_SHARE_READ, NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if(file_handle == INVALID_HANDLE_VALUE)
	{	set_last_error(IDS_STRING5026); output_log(IDS_STRING5026, tmp_value);	is_ok = APP_FAULT;	goto GOTO;}
	else{	
		sps_file_line_ptr = (SPS_FILE_LINE_PTR)malloc(sizeof(SPS_FILE_LINE)); 
		if(sps_file_line_ptr == NULL){
			set_last_error(IDS_STRING5042); 
			output_log(IDS_STRING5042, tmp_value);	
			is_ok = APP_FAULT;	goto GOTO;
		}
		__matrix_zero_Memory(sps_file_line_ptr, sizeof(SPS_FILE_LINE));
		is_ok = read_lines(file_handle, sps_file_line_ptr);
		if(is_ok == APP_FAULT) goto GOTO;
	}	

	CloseHandle(file_handle);//关闭s文件
	is_ok = read_hxxx_option(sps_file_line_ptr, sps_data_ptr);
	if(is_ok == APP_FAULT){	
		if(get_last_error() == IDS_STRING5042)	output_log(IDS_STRING5042, tmp_value);
		goto GOTO;
	}

	is_ok = read_shot_option(sps_file_line_ptr, sps_data_ptr);
	if(is_ok == APP_FAULT){
		if(get_last_error() == IDS_STRING5042)	output_log(IDS_STRING5042, tmp_value);
		goto GOTO;
	}

	//打开x文件
	__matrix_zero_Memory(tmp_value, sizeof(tmp_value));
	size = __matrix_sprintf_s(tmp_value, __TEXT("%s.x"), path);
	tmp_value[size] = 0x0; 
	file_handle = CreateFile(tmp_value, GENERIC_READ, FILE_SHARE_READ, NULL, 
							OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if(file_handle == INVALID_HANDLE_VALUE)
	{	set_last_error(IDS_STRING5026); output_log(IDS_STRING5026, tmp_value);	is_ok = APP_FAULT;	goto GOTO;}
	else{
		is_ok = read_lines(file_handle, sps_file_line_ptr);
		if(is_ok == APP_FAULT) goto GOTO;
	}

	CloseHandle(file_handle);//关闭x文件
	is_ok = read_cross_option(sps_file_line_ptr, sps_data_ptr);
	if(is_ok == APP_FAULT){	
		if(get_last_error() == IDS_STRING5042)	output_log(IDS_STRING5042, tmp_value);
		goto GOTO;
	}

	//打开r文件
	__matrix_zero_Memory(tmp_value, sizeof(tmp_value));
	size = __matrix_sprintf_s(tmp_value, __TEXT("%s.r"), path);
	tmp_value[size] = 0x0; 
	file_handle = CreateFile(tmp_value, GENERIC_READ, FILE_SHARE_READ, NULL, 
							OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if(file_handle == INVALID_HANDLE_VALUE)
	{	set_last_error(IDS_STRING5026); output_log(IDS_STRING5026, tmp_value);	is_ok = APP_FAULT;	goto GOTO;}
	else{	is_ok = read_lines(file_handle, sps_file_line_ptr);	if(is_ok == APP_FAULT) goto GOTO;}
	
	relation_record_ptr = sps_data_ptr->head_relation_records_ptr_;
	while(relation_record_ptr != NULL){
		
		loop = (relation_record_ptr->to_receiver_ - relation_record_ptr->from_receiver_) / 
				(relation_record_ptr->to_channel_ - relation_record_ptr->from_channel_ -1);
		for(i = relation_record_ptr->from_receiver_; i <= relation_record_ptr->to_receiver_; i += loop){

			param.receiver_line_name_ = relation_record_ptr->receiver_line_name_;
			param.receive_point_num_ = i;
			param.relation_record_ptr_ = relation_record_ptr;
			is_ok = read_receiver_option(sps_file_line_ptr, &param);
			if(is_ok == APP_FAULT)	goto GOTO;
		}
		relation_record_ptr = relation_record_ptr->next_;
	}

GOTO:
	free_sps_line_list(sps_file_line_ptr);
	if(file_handle != INVALID_HANDLE_VALUE)	CloseHandle(file_handle);

	return is_ok;
}

int free_sps_data_memory(__in__ SPS_DATA_INFO_PTR sps_data_ptr)
{
	int is_ok = APP_SUCCESS;
	SPS_RELATION_RECORD_SPEC_PTR	relation_record_ptr = NULL,
									tmp_relation_record_ptr = NULL;

	SPS_POINT_RECORD_SPEC_PTR		point_record_ptr = NULL,
									tmp_point_record_ptr = NULL;

	relation_record_ptr = sps_data_ptr->head_relation_records_ptr_;
	while(relation_record_ptr != NULL){

		tmp_relation_record_ptr = relation_record_ptr;
		point_record_ptr = relation_record_ptr->head_receiver_point_ptr_;
		while(point_record_ptr != NULL){
			tmp_point_record_ptr = point_record_ptr;
			point_record_ptr = point_record_ptr->next_;
			free(tmp_point_record_ptr);
		}

		relation_record_ptr = relation_record_ptr->next_;
		free(tmp_relation_record_ptr);
	}	

	return is_ok;
}

