#ifndef _Parameter_H
#define _Parameter_H
/** 日志文件夹*/
#define LogFolderPath				_T("..\\Log")
/** 系统日志文件夹（包含操作、警告、错误）*/
#define SysOptLogFolderPath			_T("\\系统运行日志")
/** 时统日志文件夹（包含尾包时刻查询及时统设置应答及结果统计）*/
#define TimeDelayLogFolderPath		_T("\\时统日志")
/** 误码查询日志文件夹（包含误码查询应答及结果统计）*/
#define ErrorCodeLogFolderPath		_T("\\误码查询日志")
/** 帧时间和偏移量日志（包含丢帧、重发帧及失效帧结果统计）*/
#define ADCFrameTimeLogFolderPath	_T("\\采样数据帧时间及偏移量")
/** ADC数据帧*/
#define ADCDataLogFolderPath		_T("\\采样数据")
/** 服务器参数路径*/
#define ParamServerFolderPath		"..\\parameter\\Server"
/** 服务程序配置文件文件名*/
#define ServerXmlFileName			"\\MatrixServer.XML"
/** 服务程序常量文件文件名*/
#define ServerIniFileName			"\\MatrixServerDLL.ini"

/** 输出选择:Debug输出则为0，Release输出则为1*/
#define OutPutSelect				0
/** 输出错误日志上限*/
#define OutPutLogErrorLimit			1000
/** 日志文件单个文件输出信息条数*/
#define OutPutLogFileInfoNumLimit	5000
/** 日志输出类型*/
enum{LogType, WarningType, ErrorType, ExpandType};
/** 日志文件类型*/
enum{OptLogType, TimeDelayLogType, ErrorCodeLogType, ADCFrameTimeLogType};
/** INI文件读取关键字缓冲区大小*/
#define INIFileStrBufSize			256
/** 回调函数-采集数据*/
// typedef void (CALLBACK* ProSampleDateCallBack)(int _iLineIndex, int _iPointIndex, int *_piData,
// 	int _iSize, unsigned int _uiSN);
/** 时统低位修正值上限*/
#define TimeDelayLowLimit	50
/** 时统低位修正默认值*/
#define TimeDelayDefault	32

/** ADC参数设置操作序号*/
#define ADCSetOptNb			1
/** ADC开始采集操作序号*/
#define	ADCStartSampleOptNb	2
/** ADC停止采集操作序号*/
#define ADCStopSampleOptNb	3
/** 仪器类型-交叉站*/
#define InstrumentTypeLAUX	1
/** 仪器类型-电源站*/
#define InstrumentTypeLAUL	2
/** 仪器类型-采集站*/
#define InstrumentTypeFDU	3
/** 仪器类型-LCI*/
#define InstrumentTypeLCI	4
/** 方向上方*/
#define DirectionTop		1
/** 方向下方*/
#define DirectionDown		2
/** 方向左方*/
#define	DirectionLeft		3
/** 方向右方*/
#define DirectionRight		4
/** 方向正中*/
#define DirectionCenter		0
#endif