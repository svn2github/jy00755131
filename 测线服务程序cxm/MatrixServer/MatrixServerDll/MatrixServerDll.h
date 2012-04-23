// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXSERVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXSERVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef _MatrixServerDll_H
#define _MatrixServerDll_H

#include <list>
#include <map>
#include <hash_map>

using std::list;
using std::string;
using std::wstring;
using std::map;
using stdext::hash_map;

/*
*	if using C++ Compiler to compile the file, adopting C linkage mode
	*/
#ifdef __cplusplus
extern "C" {
#endif

	// according to the control macro, deciding whether export or import functions
#ifdef MATRIXSERVERDLL_EXPORTS
#define MatrixServerDll_API __declspec(dllexport)
#else
#define MatrixServerDll_API __declspec(dllimport)
#endif

	// Macro definitions declarations
	// 日志文件夹
#define LogFolderPath				_T("..\\Log")
	// 系统日志文件夹（包含操作、警告、错误）
#define SysOptLogFolderPath			_T("\\系统运行日志")
	// 时统日志文件夹（包含尾包时刻查询及时统设置应答及结果统计）
#define TimeDelayLogFolderPath		_T("\\时统日志")
	// 误码查询日志文件夹（包含误码查询应答及结果统计）
#define ErrorCodeLogFolderPath		_T("\\误码查询日志")
	// 帧时间和偏移量日志（包含丢帧、重发帧及失效帧结果统计）
#define ADCFrameTimeLogFolderPath	_T("\\采样数据帧时间及偏移量")
	// ADC数据帧
#define ADCDataLogFolderPath		_T("\\采样数据")

	// 输出选择:Debug输出则为0，Release输出则为1
#define OutPutSelect				0
	// 输出错误日志上限
#define OutPutLogErrorLimit			100
	// 日志文件单个文件输出信息条数
#define OutPutLogFileInfoNumLimit	5000
	// 日志输出类型
	enum{LogType, WarningType, ErrorType, ExpandType};
// 日志文件类型
enum{OptLogType, TimeDelayLogType, ErrorCodeLogType, ADCFrameTimeLogType};
// INI文件读取关键字缓冲区大小
#define INIFileStrBufSize			256
typedef int (WINAPI *PFCALLBACK)(int Param1,int Param2) ;
typedef void (CALLBACK* ProSampleDateCallBack)(int _iLineIndex, int _iPointIndex, int *_piData,
	int _iSize, unsigned int _uiSN);

// Resources declarations
// 日志输出结构
typedef struct LogOutPut_Struct
{
	// 日志输出资源同步对象
	CRITICAL_SECTION m_oSecLogFile;
	// 日志输出文件指针
	FILE* m_pFile;
	// 日志输出结构
	list<string> m_oLogOutputList;
	// 错误计数
	unsigned int m_uiErrorCount;
	// 日志输出路径
	string m_SaveLogFilePath;
	// 日志文件序号
	unsigned int m_uiLogFileNb;
	// 日志文件存储信息条数记数
	unsigned int m_uiLogInfoCount;
	// 日志文件类型
	char m_cLogFileType;
}m_oLogOutPutStruct;

// 从INI文件中解析得到的常量
// 该结构体中的变量值只能在INI文件中被修改
// 暂不支持在程序中修改（可能会出现内存冲突的情况）
typedef struct ConstVar_Struct
{
	//____常量设置____
	// 仪器设备个数
	unsigned int m_iInstrumentNum;
	// ADC数据缓冲区个数
	int m_iADCDataCountAll;
	// 施工任务个数
	int m_iOptTaskCountAll;
	// 一次休眠的时间
	int m_iOneSleepTime;
	// 日志输出线程写日志的延时次数
	int m_iLogOutPutSleepTimes;
	// 心跳线程发送心跳帧延时次数
	int m_iHeartBeatSleepTimes;
	// 首包线程接收首包延时次数
	int m_iHeadFrameSleepTimes;
	// IP地址设置线程延时次数
	int m_iIPSetFrameSleepTimes;
	// 尾包线程延时次数
	int m_iTailFrameSleepTimes;
	// 路由监视线程延时次数
	int m_iMonitorSleepTimes;
	// 时统设置线程延时次数
	int m_iTimeDelaySleepTimes;
	// ADC参数设置线程延时次数
	int m_iADCSetSleepTimes;
	// 误码查询线程延时次数
	int m_iErrorCodeSleepTimes;
	// ADC数据接收线程延时次数
	int m_iADCDataRecSleepTimes;
	// ADC数据存储线程延时次数
	int m_iADCDataSaveSleepTimes;
	// 等待线程关闭的延时次数
	int m_iCloseThreadSleepTimes;

	// 首包稳定计数
	int m_iHeadFrameStableTimes;
	// IP地址重设次数
	int m_iIPAddrResetTimes;
	// 尾包稳定次数
	int m_iTailFrameStableTimes;
	// 路由监视稳定时间
	int m_iMonitorStableTime;
	// 测网系统状态-link
	int m_iLineSysStatusLink;
	// 测网系统状态-稳定
	int m_iLineSysStatusStable;
	// 测网系统状态-噪声采集
	int m_iLineSysStatusNoise;
	// 测网系统状态-TB采集
	int m_iLineSysStatusSample;
	// 测网系统达到稳定状态时间
	int m_iLineSysStableTime;
	// ADC参数设置操作序号
	int m_iADCSetOptNb;
	// ADC开始采集操作序号
	int m_iADCStartSampleOptNb;
	// ADC停止采集操作序号
	int m_iADCStopSampleOptNb;
	// 仪器类型-交叉站
	int m_iInstrumentTypeLAUX;
	// 仪器类型-电源站
	int m_iInstrumentTypeLAUL;
	// 仪器类型-采集站
	int m_iInstrumentTypeFDU;
	// 仪器类型-LCI
	int m_iInstrumentTypeLCI;
	// 方向上方
	int m_iDirectionTop;
	// 方向下方
	int m_iDirectionDown;
	// 方向左方
	int m_iDirectionLeft;
	// 方向右方
	int m_iDirectionRight;
	// 方向正中
	int m_iDirectionCenter;
	// IP地址设置的起始地址
	int m_iIPSetAddrStart;
	// IP地址设置的间隔
	int m_iIPSetAddrInterval;
	// 路由地址设置的起始地址
	int m_iRoutSetAddrStart;
	// 路由地址设置的间隔
	int m_iRoutSetAddrInterval;
	// 设置广播端口起始地址
	int m_iBroadcastPortStart;
	// 设置为广播IP
	unsigned int m_uiIPBroadcastAddr;
	// 一个文件内存储单个设备ADC数据帧数
	int m_iADCFrameSaveInOneFileNum;
	// 存储ADC数据的文件头行数
	int m_iADCSaveHeadLineNum;
	// 存储ADC数据的左侧预留信息字节数
	int m_iADCSaveLeftInfoBytes;
	// 存储ADC数据的字节数
	int m_iADCSaveDataBytes;
	// 存储ADC数据之间的间隔字节数
	int m_iADCSaveDataIntervalBytes;
	// 设备ADC数据缓冲区大小
	int m_iADCDataBufSize;

	//____帧格式设置____
	// 帧头长度
	int m_iFrameHeadSize;
	// 同步帧头
	char* m_cpFrameHeadCheck;
	// 命令字长度1字节
	int m_iFrameCmdSize1B;
	// 命令包长度1字节
	int m_iFramePacketSize1B;
	// 命令包长度2字节
	int m_iFramePacketSize2B;
	// 命令包长度4字节
	int m_iFramePacketSize4B;
	// ADC数据所占字节数
	int m_iADCDataSize3B;
	// 一帧内ADC数据个数
	int m_iADCDataInOneFrameNum;
	// ADC数据帧指针偏移量上限
	unsigned short m_usADCFramePointLimit;
	// 命令字个数最大值
	int m_iCommandWordMaxNum;
	// 0x18命令数组包含的最大字节数
	int m_iADCSetCommandMaxByte;
	// 发送帧缓冲区初值设定
	char m_cSndFrameBufInit;
	// 接收的网络数据帧帧长度
	int m_iRcvFrameSize;
	// 发送的网络数据帧帧长度
	int m_iSndFrameSize;
	//____服务器与设备命令字设置____
	// 发送设置命令
	unsigned short m_usSendSetCmd;
	// 发送查询命令
	unsigned short m_usSendQueryCmd;
	// 发送ADC采样数据重发命令
	unsigned short m_usSendADCCmd;
	// 串号
	char m_cCmdSn;
	// 首包时间
	char m_cCmdHeadFrameTime;
	// 本地IP地址
	char m_cCmdLocalIPAddr;
	// 本地系统时间
	char m_cCmdLocalSysTime;
	// 本地时间修正高位
	char m_cCmdLocalTimeFixedHigh;
	// 本地时间修正低位
	char m_cCmdLocalTimeFixedLow;
	// 自动数据返回地址
	char m_cCmdADCDataReturnAddr;
	// 自动数据返回端口和命令
	char m_cCmdADCDataReturnPort;
	// 端口递增下限和上限
	char m_cCmdADCDataReturnPortLimit;
	// 设置网络等待端口和命令
	char m_cCmdBroadCastPortSet;
	// 系统硬件状态拷贝
	char m_cCmdFDUErrorCode;
	// TB时刻高位
	char m_cCmdTBHigh;
	// TB时刻低位
	char m_cCmdTbLow;
	// work_ctrl 交叉站方向
	char m_cCmdLAUXRoutOpenQuery;
	// 路由开关
	char m_cCmdLAUXRoutOpenSet;
	// 尾包接收\发送时刻低位
	char m_cCmdTailRecSndTime;
	// 广播命令等待端口匹配
	char m_cCmdBroadCastPortSeted;
	// 设置ADC控制命令命令字
	char m_cCmdADCSet;
	// 网络时刻
	char m_cCmdNetTime;
	// 交叉站大线尾包接收时刻
	char m_cCmdLineTailRecTimeLAUX;
	// 交叉站交叉线尾包接收时刻
	char m_cCmdLAUTailRecTimeLAUX;
	// 交叉站故障1
	char m_cCmdLAUXErrorCode1;
	// 交叉站故障2
	char m_cCmdLAUXErrorCode2;
	// 交叉站路由分配
	char m_cCmdLAUXSetRout;
	// 返回路由
	char m_cCmdReturnRout;
	// 命令解析结束命令
	char m_cCmdEnd;

	// TB设置延时高位
	unsigned int m_uiTBSleepTimeHigh;
	// TB设置延时低位
	unsigned short m_usTBSleepTimeLow;
	// TB控制，0x0001 启动TB进行ADC数据采集
	unsigned short m_usCmdTBCtrlStartSample;
	// 0x0002 无需TB开关控制ADC数据采集命令
	unsigned short m_usCmdTBLoseCtrlStartSample;
	// 0x0000 TB开关控制ADC数据停止采集命令
	unsigned short m_usCmdTBCtrlStopSample;
	// LED灯灭
	unsigned short m_usCmdCtrlCloseLed;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oConstVarStruct;

// 从XML文件中解析得到的信息
typedef struct InstrumentCommInfo_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecCommInfo;
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	// 源地址
	unsigned int m_uiSrcIP;
	// LCI的IP地址
	unsigned int m_uiAimIP;
	// 自动数据返回地址
	unsigned int m_uiADCDataReturnAddr;
	// LCI接收的端口号
	unsigned short m_usAimPort;
	// 心跳帧返回端口
	unsigned short m_usHeartBeatReturnPort;
	// 首包接收端口
	unsigned short m_usHeadFramePort;
	// IP地址设置返回端口
	unsigned short m_usIPSetReturnPort;
	// 尾包接收端口
	unsigned short m_usTailFramePort;
	// 尾包时刻查询接收端口
	unsigned short m_usTailTimeReturnPort;
	// 时统设置应答端口
	unsigned short m_usTimeDelayReturnPort;
	// ADC参数设置应答端口
	unsigned short m_usADCSetReturnPort;
	// 误码查询返回端口
	unsigned short m_usErrorCodeReturnPort;
	// ADC数据返回端口
	unsigned short m_usADCDataReturnPort;

	//____ADC参数设置____
	// ADC设置正弦波命令大小
	int m_iSetADCSetSineSize;
	// ADC设置正弦波命令
	char* m_cpSetADCSetSine;
	// ADC设置停止采样命令大小
	int m_iSetADCStopSampleSize;
	// ADC设置停止采样命令
	char* m_cpSetADCStopSample;
	// ADC设置打开TB电源低位大小
	int m_iSetADCOpenTBPowerLowSize;
	// ADC设置打开TB电源低位
	char* m_cpSetADCOpenTBPowerLow;
	// ADC设置打开TB电源高位大小
	int m_iSetADCOpenTBPowerHighSize;
	// ADC设置打开TB电源高位
	char* m_cpSetADCOpenTBPowerHigh;
	// ADC设置打开TB开关低位大小
	int m_iSetADCOpenSwitchTBLowSize;
	// ADC设置打开TB开关低位
	char* m_cpSetADCOpenSwitchTBLow;
	// ADC设置打开TB开关高位大小
	int m_iSetADCOpenSwitchTBHighSize;
	// ADC设置打开TB开关高位
	char* m_cpSetADCOpenSwitchTBHigh;
	// ADC设置读寄存器大小
	int m_iSetADCRegisterReadSize;
	// ADC设置读寄存器
	char* m_cpSetADCRegisterRead;
	// ADC设置写寄存器大小
	int m_iSetADCRegisterWriteSize;
	// ADC设置写寄存器
	char* m_cpSetADCRegisterWrite;
	// ADC设置打开TB开关大小
	int m_iSetADCTBSwitchOpenSize;
	// ADC设置打开TB开关
	char* m_cpSetADCTBSwitchOpen;
	// ADC采样设置大小
	int m_iSetADCSampleSize;
	// ADC采样设置
	char* m_cpSetADCSample;
	// ADC设置连续采样大小
	int m_iSetADCReadContinuousSize;
	// ADC设置连续采样
	char* m_cpSetADCReadContinuous;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oInstrumentCommInfoStruct;

// 与设备通讯命令字内容
typedef struct InstrumentCommand_Struct
{
	// LCI的IP地址
	unsigned int m_uiAimIP;
	// LCI接收的端口号
	unsigned short m_usAimPort;
	// 源地址
	unsigned int m_uiSrcIP;
	// 目的地址
	unsigned int m_uiDstIP;
	// 应答端口
	unsigned short m_usReturnPort;
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	unsigned short m_usCommand;
	// SN，低8位为仪器类型，0x01为交叉站，0x02为电源站，0x03为采集站
	unsigned int m_uiSN;
	// 首包时刻
	unsigned int m_uiTimeHeadFrame;
	// 仪器IP地址
	unsigned int m_uiInstrumentIP;
	// 本地时间
	unsigned int m_uiSysTime;
	// 本地时间修正高位
	unsigned int m_uiLocalTimeFixedHigh;
	// 本地时间修正低位
	unsigned int m_uiLocalTimeFixedLow;
	// 自动数据返回地址
	unsigned int m_uiADCDataReturnAddr;
	// 自动数据返回端口
	unsigned short m_usADCDataReturnPort;
	// 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变
	unsigned short m_usADCDataReturnCmd;
	// 当返回命令高位为1时，返回端口自动加1，到高限时返回低限端口
	// 端口递增下限
	unsigned short m_usADCDataReturnPortLimitLow;
	// 端口递增上限
	unsigned short m_usADCDataReturnPortLimitHigh;
	// 设置网络等待端口，指设置接收上位机广播命令的端口
	unsigned int m_uiBroadCastPortSet;
	// 网络数据错误计数
	char m_cFDUErrorCodeDataCount;
	// 命令错误计数
	char m_cFDUErrorCodeCmdCount;
	// 时间报文状态
	char m_cTimeStatus;
	// 控制状态
	char m_cCtrlStatus;
	// TB时刻高位
	unsigned int m_uiTBHigh;
	// TB时刻低位
	unsigned short m_usTBLow;
	// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
	unsigned short m_usTBCtrl;
	// work_ctrl控制交叉站接收和发送命令的方向
	// 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B
	// =0为开，=1为关
	char m_cLAUXRoutOpenQuery;
	// 路由开关
	// 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B
	// =0为开，=1为关
	char m_cLAUXRoutOpenSet;
	// 尾包接收时刻，低14位有效
	unsigned short m_usTailRecTime;
	// 尾包发送时刻//交叉站尾包发送时刻，低14位有效
	unsigned short m_usTailSndTime;
	// 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令
	unsigned int m_uiBroadCastPortSeted;
	// 网络时刻
	unsigned int m_uiNetTime;
	// 交叉站大线A尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLineA;
	// 交叉站大线B尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLineB;
	// 交叉站交叉线A尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLAUXLineA;
	// 交叉站交叉线B尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLAUXLineB;
	// 交叉站大线A数据故障
	char m_cLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	char m_cLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	char m_cLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	char m_cLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	char m_cLAUXErrorCodeCmdCount;
	// 交叉站上方路由IP
	unsigned int m_uiRoutIPTop;
	// 交叉站下方路由IP
	unsigned int m_uiRoutIPDown;
	// 交叉站左方路由IP
	unsigned int m_uiRoutIPLeft;
	// 交叉站右方路由IP
	unsigned int m_uiRoutIPRight;
	// 路由IP地址
	unsigned int m_uiRoutIP;
	// 0x18命令数组
	char* m_cpADCSet;
	// 0x18命令数据个数
	int m_iADCSetNum;
	// ADC数据指针偏移量
	unsigned short m_usADCDataPoint;
	// ADC数据采集时仪器本地时间
	unsigned int m_uiADCSampleSysTime;
	// ADC采样数据缓冲区指针
	int* m_pADCData;
}m_oInstrumentCommandStruct;

// 心跳
typedef struct HeartBeatFrame_Struct
{
	// 心跳帧资源同步对象
	CRITICAL_SECTION m_oSecHeartBeat;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// 心跳命令字集合
	char* m_cpCommandWord;
	// 心跳命令字个数
	unsigned short m_usCommandWordNum;
	// 心跳帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 心跳Socket套接字
	SOCKET m_oHeartBeatSocket;
}m_oHeartBeatFrameStruct;

// 首包
typedef struct HeadFrame_Struct
{
	// 首包帧资源同步对象
	CRITICAL_SECTION m_oSecHeadFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// 首包帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 首包Socket套接字
	SOCKET m_oHeadFrameSocket;
}m_oHeadFrameStruct;

// IP地址设置
typedef struct IPSetFrame_Struct
{
	// IP地址设置帧资源同步对象
	CRITICAL_SECTION m_oSecIPSetFrame;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// IP地址设置命令字集合
	char* m_cpCommandWord;
	// IP地址设置命令字个数
	unsigned short m_usCommandWordNum;
	// IP地址设置帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// IP地址设置应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// IP地址设置Socket套接字
	SOCKET m_oIPSetFrameSocket;
}m_oIPSetFrameStruct;

// 尾包
typedef struct TailFrame_Struct
{
	// 尾包帧资源同步对象
	CRITICAL_SECTION m_oSecTailFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// 尾包帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 尾包Socket套接字
	SOCKET m_oTailFrameSocket;
}m_oTailFrameStruct;

// 尾包时刻
typedef struct TailTimeFrame_Struct
{
	// 尾包时刻帧资源同步对象
	CRITICAL_SECTION m_oSecTailTimeFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// 尾包时刻应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// 尾包时刻查询命令字集合
	char* m_cpCommandWord;
	// 尾包时刻查询命令字个数
	unsigned short m_usCommandWordNum;
	// 尾包时刻发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 尾包时刻Socket套接字
	SOCKET m_oTailTimeFrameSocket;
}m_oTailTimeFrameStruct;

// 时统设置
typedef struct TimeDelayFrame_Struct
{
	// 时统设置帧资源同步对象
	CRITICAL_SECTION m_oSecTimeDelayFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// 时统设置应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// 时统设置命令字集合
	char* m_cpCommandWord;
	// 时统设置命令字个数
	unsigned short m_usCommandWordNum;
	// 时统设置发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 时统设置Socket套接字
	SOCKET m_oTimeDelayFrameSocket;
}m_oTimeDelayFrameStruct;

// ADC参数设置
typedef struct ADCSetFrame_Struct
{
	// ADC参数设置帧资源同步对象
	CRITICAL_SECTION m_oSecADCSetFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// ADC参数设置应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// ADC参数设置命令字集合
	char* m_cpCommandWord;
	// ADC参数设置命令字个数
	unsigned short m_usCommandWordNum;
	// ADC参数设置发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// ADC参数设置Socket套接字
	SOCKET m_oADCSetFrameSocket;
}m_oADCSetFrameStruct;

// 误码查询
typedef struct ErrorCodeFrame_Struct
{
	// ADC参数设置帧资源同步对象
	CRITICAL_SECTION m_oSecErrorCodeFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// 应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// 误码查询命令字集合
	char* m_cpCommandWord;
	// 误码查询命令字个数
	unsigned short m_usCommandWordNum;
	// 发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 误码查询Socket套接字
	SOCKET m_oErrorCodeFrameSocket;
}m_oErrorCodeFrameStruct;

// ADC数据接收
typedef struct ADCDataFrame_Struct
{
	// ADC数据接收帧资源同步对象
	CRITICAL_SECTION m_oSecADCDataFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_cpRcvFrameData;
	// 应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_cpSndFrameData;
	// 发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// ADC数据接收和重发Socket套接字
	SOCKET m_oADCDataFrameSocket;
}m_oADCDataFrameStruct;

// 仪器属性结构体
typedef struct Instrument_Struct 
{
	/** 在仪器数组中的位置*/
	unsigned int m_uiIndex;
	/** 仪器是否使用中*/
	bool m_bInUsed;
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站；4-LCI*/
	int m_iInstrumentType;
	/** 仪器IP地址*/
	unsigned int m_uiIP;
	/** 仪器路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	int m_iRoutDirection;
	/** 路由地址 交叉线方向 上方*/
	unsigned int m_uiRoutIPTop;
	/** 路由地址 交叉线方向 下方*/
	unsigned int m_uiRoutIPDown;
	/** 路由地址 测线线方向 左方*/
	unsigned int m_uiRoutIPLeft;
	/** 路由地址 测线线方向 右方*/
	unsigned int m_uiRoutIPRight;
	/** 路由开关*/
	unsigned char m_ucLAUXRoutOpenSet;
	/** 链接的仪器 上方*/
	Instrument_Struct* m_pInstrumentTop;
	/** 链接的仪器 下方*/
	Instrument_Struct* m_pInstrumentDown;
	/** 链接的仪器 左方*/
	Instrument_Struct* m_pInstrumentLeft;
	/** 链接的仪器 右方*/
	Instrument_Struct* m_pInstrumentRight;
	/** 首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	/** 尾包计数*/
	int m_iTailFrameCount;
	/** 仪器时延*/
	unsigned int m_uiDelayTime;
	/** 仪器本地系统时间*/
	unsigned int m_uiSystemTime;
	/** 仪器网络时间*/
	unsigned int m_uiNetTime;
	/** 仪器网络状态*/
	unsigned int m_uiNetState;
	/** 仪器参数备用1*/
	unsigned int m_uiNetOrder;
	/** 16bits时间，接收时刻低位*/
	unsigned short m_usReceiveTime;	
	/** 16bits时间，发送时刻低位*/
	unsigned short m_usSendTime;
	/** 16bits 测线方向左面尾包接收时刻*/
	unsigned short m_usLineLeftReceiveTime;	
	/** 16bits 测线方向右面尾包接收时刻*/
	unsigned short m_usLineRightReceiveTime;	
	/** 16bits 交叉线方向上面尾包接收时刻*/
	unsigned short m_usCrossTopReceiveTime;	
	/** 16bits 交叉线方下面尾包接收时刻*/
	unsigned short m_usCrossDownReceiveTime;	
	/** 采集站或电源站的广播端口*/
	unsigned int m_uiBroadCastPort;
	/** 测线*/
	int m_iLineIndex;
	/** 测点号*/
	int m_iPointIndex;
	/** 标记线号*/
	unsigned int m_uiLineNb;
	/** 标记点号*/
	unsigned int m_uiPointNb;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 是否跳过道*/
	bool m_bJumpedChannel;
	/** 是否连接检波器*/
	bool m_bSensor;
	/** 是否辅助道*/
	bool m_bAux;
	/** 是否连接爆炸机*/
	bool m_bBlastMachine;
	/** 是否迂回道*/
	bool m_bDetour;
	/** 是否迂回道低端标记点*/
	bool m_bDetourMarkerLow;
	/** 是否迂回道高端标记点*/
	bool m_bDetourMarkerHigh;
	/** 是否迂回道停止标记*/
	bool m_bStopMarking;
	/** 是否标记点*/
	bool m_bMarker;
	/** 是否哑道*/
	bool m_bMute;
	// 首包位置稳定次数
	int m_iHeadFrameStableNum;
	/** 第几次设置IP地址*/
	int m_iIPSetCount;
	/** IP地址设置是否成功*/
	bool m_bIPSetOK;
	/** 第几次尾包时刻查询*/
	int m_iTailTimeQueryCount;
	/** 尾包时刻查询应答次数*/
	int m_iTailTimeReturnCount;
	/** 尾包时刻查询是否成功*/
	bool m_bTailTimeQueryOK;
	/** 本地时间修正高位*/
	unsigned int m_uiTimeHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiTimeLow;
	/** 第几次设置仪器时间*/
	int m_iTimeSetCount;
	/** 仪器时统成功次数*/
	int m_iTimeSetReturnCount;
	/** 仪器时间设置是否成功*/
	bool m_bTimeSetOK;
	// ADC命令设置是否应答
	bool m_bADCSetReturn;
	// 仪器是否进行了ADC参数设置
	bool m_bADCSet;
	// 仪器开始ADC数据采集设置成功
	bool m_bADCStartSample;
	// 仪器停止ADC数据采集设置成功
	bool m_bADCStopSample;
	// 误码查询发送帧数
	unsigned int m_uiErrorCodeQueryNum;
	// 误码查询应答帧数
	unsigned int m_uiErrorCodeReturnNum;
	// 采集站和电源站网络数据错误计数
	int m_iFDUErrorCodeDataCount;
	// 采集站和电源站命令错误计数
	int m_iFDUErrorCodeCmdCount;
	// 采集站和电源站网络数据错误计数
	char m_cFDUErrorCodeDataCountOld;
	// 采集站和电源站命令错误计数
	char m_cFDUErrorCodeCmdCountOld;

	// 交叉站大线A数据故障
	int m_iLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	int m_iLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	int m_iLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	int m_iLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	int m_iLAUXErrorCodeCmdCount;

	// 交叉站大线A数据故障
	char m_cLAUXErrorCodeDataLineACountOld;
	// 交叉站大线B数据故障
	char m_cLAUXErrorCodeDataLineBCountOld;
	// 交叉站交叉线A数据故障
	char m_cLAUXErrorCodeDataLAUXLineACountOld;
	// 交叉站交叉线B数据故障
	char m_cLAUXErrorCodeDataLAUXLineBCountOld;
	// 交叉站命令口故障
	char m_cLAUXErrorCodeCmdCountOld;

	// 实际接收ADC数据帧数（含重发帧）
	unsigned int m_uiADCDataActualRecFrameNum;
	// 重发查询帧得到的应答帧数
	unsigned int m_uiADCDataRetransmissionFrameNum;
	// 应该接收ADC数据帧数（含丢帧）
	unsigned int m_uiADCDataShouldRecFrameNum;
	// ADC数据帧的指针偏移量
	unsigned short m_usADCDataFramePoint;
	// ADC数据帧发送时的本地时间
	unsigned int m_uiADCDataFrameSysTime;
	// ADC数据帧起始帧数
	int m_iADCDataFrameStartNum;
}m_oInstrumentStruct;

// 仪器位置点结构体
typedef struct InstrumentLocation_Struct
{
	InstrumentLocation_Struct(int iLineIndex, int iPointIndex)
	{
		this->m_iLineIndex = iLineIndex;
		this->m_iPointIndex = iPointIndex;
	}
	~InstrumentLocation_Struct()
	{
	}
	bool operator == (const InstrumentLocation_Struct& rhs) const
	{
		return ((m_iLineIndex == rhs.m_iLineIndex) && (m_iPointIndex == rhs.m_iPointIndex));
	}
	bool operator < (const InstrumentLocation_Struct& rhs) const
	{
		if (m_iLineIndex == rhs.m_iLineIndex)
		{
			return (m_iPointIndex < rhs.m_iPointIndex);
		}
		else
		{
			return (m_iLineIndex < rhs.m_iLineIndex);
		}
	}
	// 线号
	int m_iLineIndex;
	// 点号
	int m_iPointIndex;
}m_oInstrumentLocationStruct;

// 仪器队列
typedef struct InstrumentList_Struct
{
	// 仪器队列资源同步对象
	CRITICAL_SECTION m_oSecInstrumentList;
	/** 仪器数组指针*/
	m_oInstrumentStruct* m_pArrayInstrument;
	/** 空闲仪器队列*/
	list<m_oInstrumentStruct*> m_olsInstrumentFree;
	/** 设置IP地址的仪器队列*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPSetMap;
	// 仪器SN索引表
	hash_map<unsigned int, m_oInstrumentStruct*> m_oSNInstrumentMap;
	// 仪器IP地址索引表
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPInstrumentMap;
	// 未完成ADC参数设置的仪器索引
	hash_map<unsigned int, m_oInstrumentStruct*> m_oADCSetInstrumentMap;
	// 仪器位置索引表
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*> m_oInstrumentLocationMap;
	// 测网系统发生变化的时间
	unsigned int m_uiLineChangeTime;
	// 测网状态由不稳定变为稳定
	bool m_bLineStableChange;
	/** 仪器总数*/
	unsigned int m_uiCountAll;
	/** 空闲仪器数量*/
	unsigned int m_uiCountFree;
	// 是否按照路由IP手动设置ADC参数
	bool m_bSetByHand;
}m_oInstrumentListStruct;

// 路由属性结构体
typedef struct Rout_Struct
{
	/** 在路由数组中的位置*/
	unsigned int m_uiIndex;
	/** 路由是否使用中*/
	bool m_bInUsed;
	/** 路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	int m_iRoutDirection;
	/** 路由头仪器*/
	m_oInstrumentStruct* m_pHead;
	/** 路由尾仪器*/
	m_oInstrumentStruct* m_pTail;
	/** 路由仪器队列*/
	list<m_oInstrumentStruct*> m_olsRoutInstrument;
	/** 路由时刻*/
	unsigned int m_uiRoutTime;
	/** 路由是否为交叉线路由*/
	bool m_bRoutLaux;
	/** 路由是否接收到ADC设置参数应答*/
	bool m_bADCSetReturn;
	// 路由是否发送ADC参数设置帧
	bool m_bADCSetRout;
	// 该路由方向上仪器的个数
	unsigned int m_uiInstrumentNum;
}m_oRoutStruct;

// 路由队列结构体
typedef struct RoutList_Struct
{
	// 路由队列资源同步对象
	CRITICAL_SECTION m_oSecRoutList;
	/** 路由数组指针*/
	m_oRoutStruct* m_pArrayRout;
	/** 空闲路由队列*/
	list<m_oRoutStruct*> m_olsRoutFree;
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutMap;
	// 需要删除路由仪器的路由索引表
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutDeleteMap;
	// 仪器时统的任务队列，成员为路由IP
	list<unsigned int> m_olsTimeDelayTaskQueue;
	// 仪器时统的任务中转队列
	list<unsigned int> m_olsTimeDelayTemp;
	// ADC参数设置任务索引
	hash_map<unsigned int, m_oRoutStruct*> m_oADCSetRoutMap;
	/** 路由总数*/
	unsigned int m_uiCountAll;
	/** 空闲路由数量*/
	unsigned int m_uiCountFree;
}m_oRoutListStruct;

// 数据存储缓冲区结构体
typedef struct ADCDataBuf_Struct
{
	// 缓冲区是否使用中
	bool m_bInUsed;
	// ADC数据存储缓冲区
	int* m_pADCDataBuf;
	// 数据存储帧序号，从0开始
	unsigned int m_uiFrameNb;
	// 采样仪器SN
	unsigned int m_uiSN;
	// 帧的本地时间
	unsigned int m_uiSysTime;
	// 缓冲区序号
	unsigned int m_uiIndex;
}m_oADCDataBufStruct;

// 数据存储缓冲区结构体
typedef struct ADCDataBufArray_Struct
{
	// 缓冲区资源同步对象
	CRITICAL_SECTION m_oSecADCDataBufArray;
	/** 缓冲区数组指针*/
	m_oADCDataBufStruct* m_pArrayADCDataBuf;
	/** 缓冲区队列*/
	list<m_oADCDataBufStruct*> m_olsADCDataBufFree;
	// 写入文件的数据缓冲区队列
	list<m_oADCDataBufStruct*> m_olsADCDataToWrite;
	/** 缓冲区总数*/
	unsigned int m_uiCountAll;
	/** 空闲缓冲区数量*/
	unsigned int m_uiCountFree;
}m_oADCDataBufArrayStruct;

// 丢失帧IP地址和偏移量结构体
typedef struct ADCLostFrameKey_Struct
{
	ADCLostFrameKey_Struct(unsigned int uiIP, unsigned short usADCFramePointNb)
	{
		this->m_uiIP = uiIP;
		this->m_usADCFramePointNb = usADCFramePointNb;
	}
	~ADCLostFrameKey_Struct()
	{
	}
	bool operator == (const ADCLostFrameKey_Struct& rhs) const
	{
		return ((m_uiIP == rhs.m_uiIP) && (m_usADCFramePointNb == rhs.m_usADCFramePointNb));
	}
	bool operator < (const ADCLostFrameKey_Struct& rhs) const
	{
		if (m_uiIP == rhs.m_uiIP)
		{
			return (m_usADCFramePointNb < rhs.m_usADCFramePointNb);
		}
		else
		{
			return (m_uiIP < rhs.m_uiIP);
		}
	}
	// 丢帧的指针偏移量
	unsigned short m_usADCFramePointNb;
	// 丢帧的IP地址
	unsigned int m_uiIP;
}m_oADCLostFrameKeyStruct;

// 丢失帧结构体
typedef struct ADCLostFrame_Struct
{
	// 丢失帧重发次数
	unsigned int m_uiCount;
	// 丢帧在文件内的帧序号，从0开始
	unsigned int m_uiFrameNb;
	// 丢失帧的本地时间
	unsigned int m_uiSysTime;
	// 是否已经收到应答
	bool m_bReturnOk;
}m_oADCLostFrameStruct;

// 参与施工的仪器结构体
typedef struct OptInstrument_Struct
{
	// SN
	unsigned int m_uiSN;
	/** 测线号*/
	int m_iLineIndex;
	/** 测点号*/
	int m_iPointIndex;
	bool operator < (const OptInstrument_Struct& rhs) const
	{
		if (m_iLineIndex == rhs.m_iLineIndex)
		{
			return (m_iPointIndex < rhs.m_iPointIndex);
		}
		else
		{
			return (m_iLineIndex < rhs.m_iLineIndex);
		}
	}
}m_oOptInstrumentStruct;

// 施工任务结构体
typedef struct OptTask_Struct
{
	// 任务是否使用中
	bool m_bInUsed;
	// 任务序号
	unsigned int m_uiIndex;
	// 施工任务开始记录的数据帧数
	unsigned int m_uiStartFrame;
	// 施工数据输出文件指针
	FILE* m_pFile;
	// 施工数据输出前一个文件的文件指针
	FILE* m_pPreviousFile;
	// 最新的文件存储序号
	unsigned int m_uiFileSaveNb;
	// 施工数据存储文件路径
	string m_SaveLogFilePath;
	// 施工任务索引表，关键字为SN，内容为行号
	hash_map<unsigned int, unsigned int> m_oSNMap;
	// 参与施工的仪器队列
	list<m_oOptInstrumentStruct> m_olsOptInstrument;
}m_oOptTaskStruct;

// 施工任务结构体数组
typedef struct OptTaskArray_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecOptTaskArray;
	/** 施工任务数组指针*/
	m_oOptTaskStruct* m_pArrayOptTask;
	// 空闲的施工任务队列
	list<m_oOptTaskStruct*> m_olsOptTaskFree;
	// 正在进行的施工任务索引
	hash_map<unsigned int, m_oOptTaskStruct*> m_oOptTaskWorkMap;
	/** 施工任务总数*/
	unsigned int m_uiCountAll;
	/** 空闲任务数量*/
	unsigned int m_uiCountFree;
	// 施工任务数据存储文件夹计数
	unsigned int m_uiADCDataFolderNb;
	// 施工数据存储文件夹路径
	string m_SaveLogFolderPath;
}m_oOptTaskArrayStruct;

// 线程结构体
typedef struct Thread_Struct
{
	// 线程句柄
	HANDLE m_hThread;
	// 线程号
	DWORD m_dwThreadID;
	//是否工作状态
	bool m_bWork;
	// 是否关闭线程
	bool m_bClose;
	// 线程结束事件
	HANDLE m_hThreadClose;
	// 常量指针
	m_oConstVarStruct* m_pConstVar;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oThreadStruct;

// 日志输出线程
typedef struct LogOutPutThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecLogOutPutThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
}m_oLogOutPutThreadStruct;

// 心跳线程
typedef struct HeartBeatThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecHeartBeatThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 心跳帧指针
	m_oHeartBeatFrameStruct* m_pHeartBeatFrame;
}m_oHeartBeatThreadStruct;

// 首包线程
typedef struct HeadFrameThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecHeadFrameThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 首包帧指针
	m_oHeadFrameStruct* m_pHeadFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oHeadFrameThreadStruct;

// IP地址设置线程
typedef struct IPSetFrameThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecIPSetFrameThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// IP地址设置帧指针
	m_oIPSetFrameStruct* m_pIPSetFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oIPSetFrameThreadStruct;

// 尾包线程
typedef struct TailFrameThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecTailFrameThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 尾包帧指针
	m_oTailFrameStruct* m_pTailFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oTailFrameThreadStruct;

// 时统线程
typedef struct TimeDelayThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecTimeDelayThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 尾包时刻帧指针
	m_oTailTimeFrameStruct* m_pTailTimeFrame;
	// 时统设置帧指针
	m_oTimeDelayFrameStruct* m_pTimeDelayFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
	// ADC开始数据采集标志位
	bool m_bADCStartSample;
	// 计数器
	unsigned int m_uiCounter;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
}m_oTimeDelayThreadStruct;

// ADC参数设置线程
typedef struct ADCSetThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecADCSetThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// ADC参数设置帧指针
	m_oADCSetFrameStruct* m_pADCSetFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
	// ADC命令设置序号
	unsigned int m_uiADCSetOperationNb;
	// 计数器
	unsigned int m_uiCounter;
	// ADC开始数据采集标志位
	bool m_bADCStartSample;
	// ADC停止数据采集标志位
	bool m_bADCStopSample;
	// 仪器的系统时间
	unsigned int m_uiLocalSysTime;
	// 上一次开始采样的采样时间
	unsigned int m_uiTBTimeOld;
	// 从XML文件中解析得到的信息
	m_oInstrumentCommInfoStruct* m_pCommInfo;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
}m_oADCSetThreadStruct;

// 误码查询线程
typedef struct ErrorCodeThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecErrorCodeThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 误码查询帧指针
	m_oErrorCodeFrameStruct* m_pErrorCodeFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
	// 误码查询日志指针
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
}m_oErrorCodeThreadStruct;

// 路由监视线程
typedef struct MonitorThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecMonitorThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
	// 时统线程指针
	m_oTimeDelayThreadStruct* m_pTimeDelayThread;
	// ADC参数设置线程
	m_oADCSetThreadStruct* m_pADCSetThread;
	// 误码查询线程
	m_oErrorCodeThreadStruct* m_pErrorCodeThread;
}m_oMonitorThreadStruct;

// ADC数据接收线程
typedef struct ADCDataRecThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecADCDataRecThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 误码查询帧指针
	m_oADCDataFrameStruct* m_pADCDataFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 误码查询日志指针
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
	// 数据存储缓冲区结构体指针
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	// 上一帧的本地时间
	unsigned int m_uiADCDataFrameSysTime;
	// 存文件数据帧数计数
	int m_iADCFrameCount;
	// 丢帧索引表
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct> m_oADCLostFrameMap;
	// 采样数据回调函数
	ProSampleDateCallBack m_oProSampleDataCallBack;
}m_oADCDataRecThreadStruct;

// 施工放炮数据存储线程
typedef struct ADCDataSaveThread_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecADCDataSaveThread;
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 数据存储缓冲区结构体指针
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	// 施工任务数组结构体指针
	m_oOptTaskArrayStruct* m_pOptTaskArray;
}m_oADCDataSaveThreadStruct;

// 环境结构体
typedef struct Environment_Struct
{
	// 从INI文件得到的常量信息
	m_oConstVarStruct* m_pConstVar;
	// 从XML文件得到的与仪器通讯信息结构
	m_oInstrumentCommInfoStruct* m_pInstrumentCommInfo;
	// 心跳帧结构
	m_oHeartBeatFrameStruct* m_pHeartBeatFrame;
	// 首包帧结构
	m_oHeadFrameStruct* m_pHeadFrame;
	// IP地址设置帧结构
	m_oIPSetFrameStruct* m_pIPSetFrame;
	// 尾包帧结构
	m_oTailFrameStruct* m_pTailFrame;
	// 尾包时刻帧结构
	m_oTailTimeFrameStruct* m_pTailTimeFrame;
	// 时统设置帧结构
	m_oTimeDelayFrameStruct* m_pTimeDelayFrame;
	// ADC参数设置帧结构
	m_oADCSetFrameStruct* m_pADCSetFrame;
	// 误码查询帧结构
	m_oErrorCodeFrameStruct* m_pErrorCodeFrame;
	// ADC数据帧结构
	m_oADCDataFrameStruct* m_pADCDataFrame;
	// 操作日志输出结构
	m_oLogOutPutStruct* m_pLogOutPutOpt;
	// 时统日志输出结构
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
	// 误码查询日志输出结构
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
	// 帧时间和偏移量日志输出结构
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
	// 仪器队列结构
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由对列结构
	m_oRoutListStruct* m_pRoutList;
	// 数据存储缓冲区结构体指针
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	// 施工任务数组结构体指针
	m_oOptTaskArrayStruct* m_pOptTaskArray;

	// 日志输出线程
	m_oLogOutPutThreadStruct* m_pLogOutPutThread;
	// 心跳线程
	m_oHeartBeatThreadStruct* m_pHeartBeatThread;
	// 首包接收线程
	m_oHeadFrameThreadStruct* m_pHeadFrameThread;
	// IP地址设置线程
	m_oIPSetFrameThreadStruct* m_pIPSetFrameThread;
	// 尾包接收线程
	m_oTailFrameThreadStruct* m_pTailFrameThread;
	// 路由监视线程
	m_oMonitorThreadStruct* m_pMonitorThread;
	// 时统线程
	m_oTimeDelayThreadStruct* m_pTimeDelayThread;
	// ADC参数设置线程
	m_oADCSetThreadStruct* m_pADCSetThread;
	// 误码查询线程
	m_oErrorCodeThreadStruct* m_pErrorCodeThread;
	// ADC数据接收线程
	m_oADCDataRecThreadStruct* m_pADCDataRecThread;
	// 施工放炮数据存储线程
	m_oADCDataSaveThreadStruct* m_pADCDataSaveThread;
}m_oEnvironmentStruct;

// functions declarations
/************************************************************************/
/* 功能函数                                                             */
/************************************************************************/
// 宽字节字符串转换为多字节字符串
// MatrixServerDll_API string WideCharToMultiChar(wstring str);
// CString转换为String
MatrixServerDll_API void ConvertCStrToStr(CString str, string* strConv);
// 得到帧的帧内容
MatrixServerDll_API void GetFrameInfo(char* pFrameData, int iSize, string* strFrameData);
// 从CString转换为char数组
MatrixServerDll_API void ParseCStringToArray(char** pData, int iSize, CString str);
// 判断文件是否存在
MatrixServerDll_API bool IfFileExist(CString str);
// 校验帧的同步码
MatrixServerDll_API bool CheckInstrumentFrameHead(char* pFrameData, char* pFrameHeadCheck, int iCheckSize);
// 生成帧的同步码
MatrixServerDll_API bool MakeInstrumentFrameHead(char* pFrameData, char* pFrameHeadCheck, int iCheckSize);
// 重置帧内容解析变量
MatrixServerDll_API bool ResetInstrumentFramePacket(m_oInstrumentCommandStruct* pCommand);
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrumentFrame(m_oInstrumentCommandStruct* pCommand, 
	char* pFrameData, m_oConstVarStruct* pConstVar);
// 生成与设备通讯帧
MatrixServerDll_API bool MakeInstrumentFrame(m_oInstrumentCommandStruct* pCommand, 
	m_oConstVarStruct* pConstVar,char* pFrameData, char* pCommandWord = NULL, 
	unsigned short usCommandWordNum = 0);
// 创建CSocket接收端口并绑定端口和IP地址
MatrixServerDll_API SOCKET CreateInstrumentSocket(unsigned short usPort, 
	unsigned int uiIP, m_oLogOutPutStruct* pLogOutPut = NULL);
// 设置广播模式
MatrixServerDll_API void SetInstrumentSocketBroadCast(SOCKET oSocket, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 设置端口接收缓冲区接收帧数量
MatrixServerDll_API void SetRcvBufferSize(SOCKET oSocket, int iRcvBufferSize, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 设置端口发送缓冲区发送帧数量
MatrixServerDll_API void SetSndBufferSize(SOCKET oSocket, int iSndBufferSize, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 得到网络接收缓冲区收到的帧数量
MatrixServerDll_API DWORD GetFrameCount(SOCKET oSocket, int iRcvFrameSize, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 得到帧数据
MatrixServerDll_API bool GetFrameData(SOCKET oSocket, char* pFrameData, 
	int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL);
// 清空接收帧缓冲区
MatrixServerDll_API void ClearRcvFrameBuf(SOCKET oSocket, char* pFrameData, 
	int iRcvFrameSize, m_oLogOutPutStruct* pLogOutPut = NULL);
// 发送帧
MatrixServerDll_API bool SendFrame(SOCKET oSocket, char* pFrameData,int iSndFrameSize, 
	unsigned short usPort, unsigned int uiIP, m_oLogOutPutStruct* pLogOutPut = NULL);
// 初始化套接字库
MatrixServerDll_API void OnInitSocketLib(m_oLogOutPutStruct* pLogOutPut = NULL);
// 释放套接字库
MatrixServerDll_API void OnCloseSocketLib(m_oLogOutPutStruct* pLogOutPut = NULL);
// 关闭Socket套接字
MatrixServerDll_API void OnCloseSocket(SOCKET oSocket);
// 清空接收缓冲区
MatrixServerDll_API void OnClearSocketRcvBuf(SOCKET oSocket, int iRcvFrameSize);
// 得到路由方向上仪器个数
MatrixServerDll_API bool OnGetRoutInstrumentNum(int iLineIndex, int iPointIndex, int iDirection, 
	m_oEnvironmentStruct* pEnv, unsigned int& uiInstrumentNum);

/************************************************************************/
/* 日志输出                                                             */
/************************************************************************/
// 创建日志输出结构体
MatrixServerDll_API m_oLogOutPutStruct* OnCreateLogOutPut();
// 打开日志输出文件
MatrixServerDll_API int OpenLogOutPutFile(m_oLogOutPutStruct* pLogOutPut);
// 加入到日志输出队列
MatrixServerDll_API void AddMsgToLogOutPutList(m_oLogOutPutStruct* pLogOutPut, 
	string pFuncName = "", string pVarName = "", byte byErrorType = LogType, 
	unsigned int uiErrorCode = 0);
// 日志输出写入文件
MatrixServerDll_API void LogOutPutWriteInFile(FILE* pFile, string str, 
	bool bFlush = false);
// 写入日志输出文件
MatrixServerDll_API void WriteLogOutPutListToFile(m_oLogOutPutStruct* pLogOutPut);
// 关闭日志输出文件
MatrixServerDll_API void CloseLogOutPutFile(m_oLogOutPutStruct* pLogOutPut);
// 初始化日志输出结构体
MatrixServerDll_API void OnInitLogOutPut(m_oLogOutPutStruct* pLogOutPut);
// 关闭日志输出结构体
MatrixServerDll_API void OnCloseLogOutPut(m_oLogOutPutStruct* pLogOutPut);
// 释放日志输出结构体
MatrixServerDll_API void OnFreeLogOutPut(m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* 常量信息                                                             */
/************************************************************************/
// 创建常量信息结构体
MatrixServerDll_API m_oConstVarStruct* OnCreateConstVar(void);
// 载入INI文件
MatrixServerDll_API void LoadIniFile(m_oConstVarStruct* pConstVar, string strINIFilePath);
// 初始化常量信息结构体
MatrixServerDll_API void OnInitConstVar(m_oConstVarStruct* pConstVar, 
	string strINIFilePath, m_oLogOutPutStruct* pLogOutPut = NULL);
// 关闭常量信息结构体
MatrixServerDll_API void OnCloseConstVar(m_oConstVarStruct* pConstVar);
// 释放常量信息结构体
MatrixServerDll_API void OnFreeConstVar(m_oConstVarStruct* pConstVar);

/************************************************************************/
/* xml载入                                                              */
/************************************************************************/
// 创建仪器通讯信息结构体
MatrixServerDll_API m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void);
// 打开程序配置文件
MatrixServerDll_API BOOL OpenAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo,
	string strXMLFilePath);
//加载IP地址设置数据
MatrixServerDll_API void LoadIPSetupData(m_oInstrumentCommInfoStruct* pCommInfo);
//加载端口设置数据
MatrixServerDll_API void LoadPortSetupData(m_oInstrumentCommInfoStruct* pCommInfo);
//加载ADC参数设置数据
MatrixServerDll_API void LoadADCSetData(m_oInstrumentCommInfoStruct* pCommInfo);
//加载测线服务器程序设置数据
MatrixServerDll_API void LoadLineServerAppSetupData(m_oInstrumentCommInfoStruct* pCommInfo,
	string strXMLFilePath);
// 初始化仪器通讯信息结构体
MatrixServerDll_API void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo, 
	string strXMLFilePath, m_oLogOutPutStruct* pLogOutPut = NULL);
// 关闭程序配置文件
MatrixServerDll_API void CloseAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo);
// 释放仪器通讯信息结构体
MatrixServerDll_API void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo);

/************************************************************************/
/* 心跳帧                                                               */
/************************************************************************/
// 创建心跳帧信息结构体
MatrixServerDll_API m_oHeartBeatFrameStruct* OnCreateInstrumentHeartBeat(void);
// 初始化心跳
MatrixServerDll_API void OnInitInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭心跳帧信息结构体
MatrixServerDll_API void OnCloseInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame);
// 释放心跳帧信息结构体
MatrixServerDll_API void OnFreeInstrumentHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame);
// 创建并设置心跳端口
MatrixServerDll_API void OnCreateAndSetHeartBeatSocket(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 生成心跳帧
MatrixServerDll_API void MakeInstrumentHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oConstVarStruct* pConstVar);
// 发送心跳帧
MatrixServerDll_API void SendInstrumentHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* 首包帧                                                               */
/************************************************************************/
// 创建首包帧信息结构体
MatrixServerDll_API m_oHeadFrameStruct* OnCreateInstrumentHeadFrame(void);
// 初始化首包
MatrixServerDll_API void OnInitInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭首包帧信息结构体
MatrixServerDll_API void OnCloseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame);
// 释放首包帧信息结构体
MatrixServerDll_API void OnFreeInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame);
// 创建并设置首包端口
MatrixServerDll_API void OnCreateAndSetHeadFrameSocket(m_oHeadFrameStruct* pHeadFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析首包帧
MatrixServerDll_API bool ParseInstrumentHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* IP地址设置帧                                                         */
/************************************************************************/
// 创建IP地址设置帧信息结构体
MatrixServerDll_API m_oIPSetFrameStruct* OnCreateInstrumentIPSetFrame(void);
// 初始化IP地址设置
MatrixServerDll_API void OnInitInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭IP地址设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame);
// 释放IP地址设置帧信息结构体
MatrixServerDll_API void OnFreeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame);
// 创建并设置IP地址设置端口
MatrixServerDll_API void OnCreateAndSetIPSetFrameSocket(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析IP地址设置应答帧
MatrixServerDll_API bool ParseInstrumentIPSetReturnFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar);
// 打开交叉站某一路由方向的电源
MatrixServerDll_API bool OpenLAUXRoutPower(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet, 
	m_oEnvironmentStruct* pEnv);
// 生成IP地址查询帧
MatrixServerDll_API void MakeInstrumentIPQueryFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP);
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument);
// 发送IP地址设置帧
MatrixServerDll_API void SendInstrumentIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* 尾包帧                                                               */
/************************************************************************/
// 创建尾包帧信息结构体
MatrixServerDll_API m_oTailFrameStruct* OnCreateInstrumentTailFrame(void);
// 初始化尾包
MatrixServerDll_API void OnInitInstrumentTailFrame(m_oTailFrameStruct* pTailFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭尾包帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTailFrame(m_oTailFrameStruct* pTailFrame);
// 释放尾包帧信息结构体
MatrixServerDll_API void OnFreeInstrumentTailFrame(m_oTailFrameStruct* pTailFrame);
// 创建并设置尾包端口
MatrixServerDll_API void OnCreateAndSetTailFrameSocket(m_oTailFrameStruct* pTailFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析尾包帧
MatrixServerDll_API bool ParseInstrumentTailFrame(m_oTailFrameStruct* pTailFrame, 
	m_oConstVarStruct* pConstVar);


/************************************************************************/
/* 尾包时刻帧                                                           */
/************************************************************************/
// 创建尾包时刻帧信息结构体
MatrixServerDll_API m_oTailTimeFrameStruct* OnCreateInstrumentTailTimeFrame(void);
// 初始化尾包时刻帧
MatrixServerDll_API void OnInitInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭尾包时刻帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame);
// 释放尾包时刻帧信息结构体
MatrixServerDll_API void OnFreeInstrumentTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame);
// 创建并设置尾包时刻端口
MatrixServerDll_API void OnCreateAndSetTailTimeFrameSocket(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析尾包时刻查询帧
MatrixServerDll_API bool ParseInstrumentTailTimeReturnFrame(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar);
// 按IP地址查询尾包时刻帧
MatrixServerDll_API void MakeInstrumentTailTimeQueryFramebyIP(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP);
// 广播查询尾包时刻帧
MatrixServerDll_API void MakeInstrumentTailTimeQueryFramebyBroadCast(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort);
// 发送尾包时刻查询帧
MatrixServerDll_API void SendInstrumentTailTimeQueryFrame(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* 时统帧                                                               */
/************************************************************************/
// 创建时统设置帧信息结构体
MatrixServerDll_API m_oTimeDelayFrameStruct* OnCreateInstrumentTimeDelayFrame(void);
// 初始化时统设置帧
MatrixServerDll_API void OnInitInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭时统设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame);
// 释放时统设置帧信息结构体
MatrixServerDll_API void OnFreeInstrumentTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame);
// 创建并设置时统设置端口
MatrixServerDll_API void OnCreateAndSetTimeDelayFrameSocket(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析时统设置应答帧
MatrixServerDll_API bool ParseInstrumentTimeDelayReturnFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar);
// 生成时统设置帧
MatrixServerDll_API void MakeInstrumentDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument);
// 发送时统设置帧
MatrixServerDll_API void SendInstrumentDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* ADC参数设置帧                                                        */
/************************************************************************/
// 创建ADC参数设置帧信息结构体
MatrixServerDll_API m_oADCSetFrameStruct* OnCreateInstrumentADCSetFrame(void);
// 初始化ADC参数设置帧
MatrixServerDll_API void OnInitInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭ADC参数设置帧信息结构体
MatrixServerDll_API void OnCloseInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame);
// 释放ADC参数设置帧信息结构体
MatrixServerDll_API void OnFreeInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame);
// 创建并设置ADC参数设置端口
MatrixServerDll_API void OnCreateAndSetADCSetFrameSocket(m_oADCSetFrameStruct* pADCSetFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析ADC参数设置应答帧
MatrixServerDll_API bool ParseInstrumentADCSetReturnFrame(m_oADCSetFrameStruct* pADCSetFrame,
	m_oConstVarStruct* pConstVar);
// 发送ADC参数设置帧
MatrixServerDll_API void SendInstrumentADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* 误码查询帧                                                           */
/************************************************************************/
// 创建误码查询帧信息结构体
MatrixServerDll_API m_oErrorCodeFrameStruct* OnCreateInstrumentErrorCodeFrame(void);
// 初始化误码查询帧
MatrixServerDll_API void OnInitInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭误码查询帧信息结构体
MatrixServerDll_API void OnCloseInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame);
// 释放误码查询帧信息结构体
MatrixServerDll_API void OnFreeInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame);
// 创建并设置误码查询端口
MatrixServerDll_API void OnCreateAndSetErrorCodeFrameSocket(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析误码查询应答帧
MatrixServerDll_API bool ParseInstrumentErrorCodeReturnFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oConstVarStruct* pConstVar);
// 广播查询误码
MatrixServerDll_API void MakeInstrumentErrorCodeQueryFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort);
// 发送误码查询帧
MatrixServerDll_API void SendInstrumentErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* ADC数据帧                                                            */
/************************************************************************/
// 创建ADC数据帧信息结构体
MatrixServerDll_API m_oADCDataFrameStruct* OnCreateInstrumentADCDataFrame(void);
// 初始化ADC数据帧
MatrixServerDll_API void OnInitInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭ADC数据帧信息结构体
MatrixServerDll_API void OnCloseInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame);
// 释放ADC数据帧信息结构体
MatrixServerDll_API void OnFreeInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame);
// 创建并设置ADC数据帧端口
MatrixServerDll_API void OnCreateAndSetADCDataFrameSocket(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析ADC数据接收帧
MatrixServerDll_API bool ParseInstrumentADCDataRecFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar);
// 生成ADC数据查询帧
MatrixServerDll_API void MakeInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiIP, unsigned short usDataPoint);
// 发送ADC数据查询帧
MatrixServerDll_API void SendInstrumentADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar);

/************************************************************************/
/* 仪器结构体                                                           */
/************************************************************************/
// 仪器信息重置
MatrixServerDll_API void OnInstrumentReset(m_oInstrumentStruct* pInstrument, bool bSetByHand);
// 判断仪器索引号是否已加入索引表
MatrixServerDll_API BOOL IfIndexExistInMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap);
// 增加一个IP地址设置仪器
MatrixServerDll_API void AddInstrumentToMap(unsigned int uiIndex, m_oInstrumentStruct* pInstrument, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap);
// 根据输入索引号，由索引表得到仪器指针
MatrixServerDll_API m_oInstrumentStruct* GetInstrumentFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap);
// 从索引表删除索引号指向的仪器指针
MatrixServerDll_API BOOL DeleteInstrumentFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oInstrumentStruct*>* pMap);
// 判断仪器位置索引号是否已加入索引表
MatrixServerDll_API BOOL IfLocationExistInMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 增加对象到索引表
MatrixServerDll_API void AddLocationToMap(int iLineIndex, int iPointIndex, 
	m_oInstrumentStruct* pInstrument, map<m_oInstrumentLocationStruct, 
	m_oInstrumentStruct*>* pMap);
// 根据输入索引号，由索引表得到仪器指针
MatrixServerDll_API m_oInstrumentStruct* GetInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 根据仪器的位置得到仪器的SN号
MatrixServerDll_API unsigned int GetInstrumentSnFromLocationMap(int iLineIndex, int iPointIndex, 
	m_oInstrumentListStruct* pInstrumentList);
// 从索引表删除索引号指向的仪器指针
MatrixServerDll_API BOOL DeleteInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 得到仪器在某一方向上的路由IP
MatrixServerDll_API bool GetRoutIPBySn(unsigned int uiSN, int iDirection, 
	m_oInstrumentListStruct* pInstrumentList, m_oConstVarStruct* pConstVar, 
	unsigned int& uiRoutIP);

/**
* 根据链接方向，得到连接的下一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的下一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetNextInstrument(int iRoutDirection, 
m_oInstrumentStruct* pInstrument, m_oConstVarStruct* pConstVar);
/**
* 根据链接方向，得到连接的前一个仪器
* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的前一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetPreviousInstrument(int iRoutDirection, 
m_oInstrumentStruct* pInstrument, m_oConstVarStruct* pConstVar);



/************************************************************************/
/* 仪器队列                                                             */
/************************************************************************/
// 创建仪器队列结构体
MatrixServerDll_API m_oInstrumentListStruct* OnCreateInstrumentList(void);
// 重置仪器队列结构体
MatrixServerDll_API void OnResetInstrumentList(m_oInstrumentListStruct* pInstrumentList);
// 初始化仪器队列结构体
MatrixServerDll_API void OnInitInstrumentList(m_oInstrumentListStruct* pInstrumentList,
	m_oConstVarStruct* pConstVar);
// ADC参数设置改为手动设置
MatrixServerDll_API void OnSetADCSetByHand(m_oInstrumentListStruct* pInstrumentList);
// 关闭仪器队列结构体
MatrixServerDll_API void OnCloseInstrumentList(m_oInstrumentListStruct* pInstrumentList);
// 释放仪器队列结构体
MatrixServerDll_API void OnFreeInstrumentList(m_oInstrumentListStruct* pInstrumentList);
// 得到一个空闲仪器
MatrixServerDll_API m_oInstrumentStruct* GetFreeInstrument(m_oInstrumentListStruct* pInstrumentList);
// 增加一个空闲仪器
MatrixServerDll_API void AddFreeInstrument(m_oInstrumentStruct* pInstrument, 
	m_oInstrumentListStruct* pInstrumentList);
// 更新上次测网系统变化时刻
MatrixServerDll_API void UpdateLineChangeTime(m_oInstrumentListStruct* pInstrumentList);

/************************************************************************/
/* 路由结构体                                                           */
/************************************************************************/
// 重置路由信息
MatrixServerDll_API void OnRoutReset(m_oRoutStruct* pRout);
// 更新路由对象的路由时间
MatrixServerDll_API void UpdateRoutTime(m_oRoutStruct* pRout);
// 路由地址是否已加入索引表
MatrixServerDll_API BOOL IfIndexExistInRoutMap(unsigned int uiRoutIP, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap);
// 增加一个路由
MatrixServerDll_API void AddRout(unsigned int uiIndex, m_oRoutStruct* pRout, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap);
// 根据输入索引号，由索引表得到一个路由指针
MatrixServerDll_API m_oRoutStruct* GetRout(unsigned int uiIndex, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap);
// 根据输入索引号，从索引表中删除一个路由
MatrixServerDll_API void DeleteRout(unsigned int uiIndex, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap);
// 由路由IP得到路由结构体指针
MatrixServerDll_API bool GetRoutByRoutIP(unsigned int uiRoutIP, 
	m_oRoutListStruct* pRoutList, m_oRoutStruct** ppRout);

/************************************************************************/
/* 路由队列                                                             */
/************************************************************************/
// 创建路由队列结构体
MatrixServerDll_API m_oRoutListStruct* OnCreateRoutList(void);
// 重置路由队列结构体
MatrixServerDll_API void OnResetRoutList(m_oRoutListStruct* pRoutList);
// 初始化路由队列结构体
MatrixServerDll_API void OnInitRoutList(m_oRoutListStruct* pRoutList,
	m_oConstVarStruct* pConstVar);
// 关闭路由队列结构体
MatrixServerDll_API void OnCloseRoutList(m_oRoutListStruct* pRoutList);
// 释放路由队列结构体
MatrixServerDll_API void OnFreeRoutList(m_oRoutListStruct* pRoutList);
// 得到一个空闲路由
MatrixServerDll_API m_oRoutStruct* GetFreeRout(m_oRoutListStruct* pRoutList);
// 增加一个空闲路由
MatrixServerDll_API void AddFreeRout(m_oRoutStruct* pRout, m_oRoutListStruct* pRoutList);

/************************************************************************/
/* ADC数据存储缓冲                                                      */
/************************************************************************/
// 重置数据存储缓冲区信息
MatrixServerDll_API void OnADCDataBufReset(m_oADCDataBufStruct* pADCDataBuf);

/************************************************************************/
/* ADC数据存储缓冲区数组                                                */
/************************************************************************/
// 创建数据存储缓冲区结构体
MatrixServerDll_API m_oADCDataBufArrayStruct* OnCreateADCDataBufArray(void);
// 重置数据存储缓冲区结构体
MatrixServerDll_API void OnResetADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray);
// 初始化数据存储缓冲区结构体
MatrixServerDll_API void OnInitADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray,
	m_oConstVarStruct* pConstVar);
// 关闭数据存储缓冲区结构体
MatrixServerDll_API void OnCloseADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray);
// 释放数据存储缓冲区结构体
MatrixServerDll_API void OnFreeADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray);
// 得到一个空闲数据存储缓冲区
MatrixServerDll_API m_oADCDataBufStruct* GetFreeADCDataBuf(m_oADCDataBufArrayStruct* pADCDataBufArray);
// 增加一个空闲数据存储缓冲区
MatrixServerDll_API void AddFreeADCDataBuf(m_oADCDataBufStruct* pADCDataBuf, 
	m_oADCDataBufArrayStruct* pADCDataBufArray);

/************************************************************************/
/* 丢失帧结构体                                                         */
/************************************************************************/
// 判断索引号是否已加入索引表
MatrixServerDll_API BOOL IfIndexExistInADCFrameLostMap(unsigned int uiIP, unsigned short usFramePointNb,
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap);
// 增加到索引
MatrixServerDll_API void AddToADCFrameLostMap(unsigned int uiIP, unsigned short usFramePointNb, 
	m_oADCLostFrameStruct oLostFrame, map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap);
// 根据输入索引号，由索引表得到仪器指针
MatrixServerDll_API m_oADCLostFrameStruct* GetFromADCFrameLostMap(unsigned int uiIP, unsigned short usFramePointNb, 
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap);

/************************************************************************/
/* 施工任务                                                             */
/************************************************************************/
// 重置施工任务
MatrixServerDll_API void OnOptTaskReset(m_oOptTaskStruct* pOptTask);
// 判断索引号是否已加入施工任务索引表
MatrixServerDll_API BOOL IfIndexExistInOptTaskMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap);
// 判断仪器SN号是否已加入施工任务仪器索引表
MatrixServerDll_API BOOL IfIndexExistInOptTaskSNMap(unsigned int uiIndex,
	hash_map<unsigned int, unsigned int>* pMap);
// 从施工任务仪器索引表中得到仪器的行号
MatrixServerDll_API unsigned int GetLineNbFromOptTaskSNMap(unsigned int uiIndex,
	hash_map<unsigned int, unsigned int>* pMap);
// 向施工任务仪器索引表中加入仪器
MatrixServerDll_API void AddToOptTaskSNMap(unsigned int uiIndex, unsigned int uiLineNb,
	hash_map<unsigned int, unsigned int>* pMap);
// 增加一个施工任务
MatrixServerDll_API void AddOptTaskToMap(unsigned int uiIndex, m_oOptTaskStruct* pOptTask, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap);
// 根据输入索引号，由索引表得到施工任务指针
MatrixServerDll_API m_oOptTaskStruct* GetOptTaskFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap);
// 从索引表删除索引号指向的施工任务指针
MatrixServerDll_API BOOL DeleteOptTaskFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap);

/************************************************************************/
/* 施工任务数组                                                         */
/************************************************************************/
// 创建施工任务数组结构体
MatrixServerDll_API m_oOptTaskArrayStruct* OnCreateOptTaskArray(void);
// 重置施工任务数组结构体
MatrixServerDll_API void OnResetOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray);
// 初始化施工任务数组结构体
MatrixServerDll_API void OnInitOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray,
	m_oConstVarStruct* pConstVar);
// 关闭施工任务数组结构体
MatrixServerDll_API void OnCloseOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray);
// 释放施工任务数组结构体
MatrixServerDll_API void OnFreeOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray);
// 得到一个空闲施工任务
MatrixServerDll_API m_oOptTaskStruct* GetFreeOptTask(m_oOptTaskArrayStruct* pOptTaskArray);
// 增加一个空闲施工任务
MatrixServerDll_API void AddFreeOptTask(m_oOptTaskStruct* pOptTask, m_oOptTaskArrayStruct* pOptTaskArray);

/************************************************************************/
/* 线程结构体                                                           */
/************************************************************************/
// 初始化线程函数
MatrixServerDll_API bool OnInitThread(m_oThreadStruct* pThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 关闭线程函数的句柄
MatrixServerDll_API void OnCloseThreadHandle(m_oThreadStruct* pThread);
// 关闭线程函数
MatrixServerDll_API bool OnCloseThread(m_oThreadStruct* pThread);

/************************************************************************/
/* 日志输出线程                                                         */
/************************************************************************/
// 创建日志输出线程
MatrixServerDll_API m_oLogOutPutThreadStruct* OnCreateLogOutPutThread(void);
// 线程等待函数
MatrixServerDll_API void WaitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread);
// 初始化日志输出线程
MatrixServerDll_API bool OnInitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化日志输出线程
MatrixServerDll_API bool OnInit_LogOutPutThread(m_oEnvironmentStruct* pEnv);
// 关闭日志输出线程
MatrixServerDll_API bool OnCloseLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread);
// 释放日志输出线程
MatrixServerDll_API void OnFreeLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread);

/************************************************************************/
/* 心跳线程                                                             */
/************************************************************************/
// 创建心跳线程
MatrixServerDll_API m_oHeartBeatThreadStruct* OnCreateHeartBeatThread(void);
// 线程等待函数
MatrixServerDll_API void WaitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread);
// 初始化心跳线程
MatrixServerDll_API bool OnInitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化心跳线程
MatrixServerDll_API bool OnInit_HeartBeatThread(m_oEnvironmentStruct* pEnv);
// 关闭心跳线程
MatrixServerDll_API bool OnCloseHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread);
// 释放心跳线程
MatrixServerDll_API void OnFreeHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread);

/************************************************************************/
/* 首包线程                                                             */
/************************************************************************/
// 创建首包接收线程
MatrixServerDll_API m_oHeadFrameThreadStruct* OnCreateHeadFrameThread(void);
// 仪器位置按照首包时刻排序
MatrixServerDll_API void InstrumentLocationSort(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar);
/**
* 设置交叉站某个方向的路由
	* @param CInstrument* &pInstrument 仪器指针
* @param unsigned int uiRoutDirection 路由方向
* @return void
*/
MatrixServerDll_API void SetCrossRout(m_oInstrumentStruct* pInstrument, int iRoutDirection, 
m_oConstVarStruct* pConstVar, m_oRoutListStruct* pRoutList);
// 处理单个首包帧
MatrixServerDll_API void ProcHeadFrameOne(m_oHeadFrameThreadStruct* pHeadFrameThread);
// 处理首包帧
MatrixServerDll_API void ProcHeadFrame(m_oHeadFrameThreadStruct* pHeadFrameThread);
// 线程等待函数
MatrixServerDll_API void WaitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread);
// 初始化首包接收线程
MatrixServerDll_API bool OnInitHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化首包接收线程
MatrixServerDll_API bool OnInit_HeadFrameThread(m_oEnvironmentStruct* pEnv);
// 关闭首包接收线程
MatrixServerDll_API bool OnCloseHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread);
// 释放首包接收线程
MatrixServerDll_API void OnFreeHeadFrameThread(m_oHeadFrameThreadStruct* pHeadFrameThread);

/************************************************************************/
/* IP地址设置线程                                                       */
/************************************************************************/
// 创建IP地址设置线程
MatrixServerDll_API m_oIPSetFrameThreadStruct* OnCreateIPSetFrameThread(void);
// 处理单个IP地址设置应答帧
MatrixServerDll_API void  ProcIPSetReturnFrameOne(m_oIPSetFrameThreadStruct* pIPSetFrameThread);
// 处理IP地址设置应答帧
MatrixServerDll_API void ProcIPSetReturnFrame(m_oIPSetFrameThreadStruct* pIPSetFrameThread);
// 按照IP地址设置索引发送IP地址设置帧
MatrixServerDll_API void ProcIPSetFrame(m_oIPSetFrameThreadStruct* pIPSetFrameThread);
// 线程等待函数
MatrixServerDll_API void WaitIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread);
// 初始化IP地址设置线程
MatrixServerDll_API bool OnInitIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化IP地址设置线程
MatrixServerDll_API bool OnInit_IPSetFrameThread(m_oEnvironmentStruct* pEnv);
// 关闭IP地址设置线程
MatrixServerDll_API bool OnCloseIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread);
// 释放IP地址设置线程
MatrixServerDll_API void OnFreeIPSetFrameThread(m_oIPSetFrameThreadStruct* pIPSetFrameThread);

/************************************************************************/
/* 尾包线程                                                             */
/************************************************************************/
// 创建尾包接收线程
MatrixServerDll_API m_oTailFrameThreadStruct* OnCreateTailFrameThread(void);
// 线程等待函数
MatrixServerDll_API void WaitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread);
// 清除与该仪器路由方向相同的之前仪器的尾包计数
MatrixServerDll_API void OnClearSameRoutTailCount(m_oInstrumentStruct* pInstrument, 
	m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar);
// 回收一个路由
MatrixServerDll_API void FreeRoutFromMap(unsigned int uiRoutIP, m_oRoutListStruct* pRoutList);
// 回收一个仪器
MatrixServerDll_API void FreeInstrumentFromMap(m_oInstrumentStruct* pInstrument, 
	m_oInstrumentListStruct* pInstrumentList, m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar);
// 删除该路由方向指定仪器之后的仪器
MatrixServerDll_API void DeleteInstrumentAlongRout(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, 
	m_oInstrumentListStruct* pInstrumentList, m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar);
// 在路由方向上删除该仪器之后的全部仪器
MatrixServerDll_API void DeleteAllInstrumentAlongRout(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, 
	m_oInstrumentListStruct* pInstrumentList, m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar,
	m_oLogOutPutStruct* pLogOutPut);
// 处理单个尾包帧
MatrixServerDll_API void ProcTailFrameOne(m_oTailFrameThreadStruct* pTailFrameThread);
// 处理尾包帧
MatrixServerDll_API void ProcTailFrame(m_oTailFrameThreadStruct* pTailFrameThread);
// 初始化尾包接收线程
MatrixServerDll_API bool OnInitTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化尾包接收线程
MatrixServerDll_API bool OnInit_TailFrameThread(m_oEnvironmentStruct* pEnv);
// 关闭尾包接收线程
MatrixServerDll_API bool OnCloseTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread);
// 释放尾包接收线程
MatrixServerDll_API void OnFreeTailFrameThread(m_oTailFrameThreadStruct* pTailFrameThread);

/************************************************************************/
/* 时统线程                                                             */
/************************************************************************/
// 创建时统线程
MatrixServerDll_API m_oTimeDelayThreadStruct* OnCreateTimeDelayThread(void);
// 线程等待函数
MatrixServerDll_API void WaitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread);
// 尾包时刻查询准备工作
MatrixServerDll_API void PrepareTailTimeFrame(m_oRoutStruct* pRout, m_oConstVarStruct* pConstVar);
// 处理尾包时刻查询
MatrixServerDll_API void ProcTailTimeFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread);
// 处理单个尾包时刻查询应答帧
MatrixServerDll_API void ProcTailTimeReturnFrameOne(m_oRoutStruct* pRout, 
	m_oTimeDelayThreadStruct* pTimeDelayThread);
// 处理尾包时刻查询应答
MatrixServerDll_API void ProcTailTimeReturnFrame(m_oRoutStruct* pRout, 
	m_oTimeDelayThreadStruct* pTimeDelayThread);
// 检查尾包时刻查询结果是否接收完全
MatrixServerDll_API bool CheckTailTimeReturn(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread);
// 处理时统设置
MatrixServerDll_API void ProcTimeDelayFrame(m_oRoutStruct* pRout, m_oTimeDelayThreadStruct* pTimeDelayThread);
// 处理单个时统设置应答
MatrixServerDll_API void ProcTimeDelayReturnFrameOne(m_oTimeDelayThreadStruct* pTimeDelayThread);
// 处理时统设置应答
MatrixServerDll_API void ProcTimeDelayReturnFrame(m_oTimeDelayThreadStruct* pTimeDelayThread);
// 初始化时统设置线程
MatrixServerDll_API bool OnInitTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化时统设置线程
MatrixServerDll_API bool OnInit_TimeDelayThread(m_oEnvironmentStruct* pEnv);
// 关闭时统设置线程
MatrixServerDll_API bool OnCloseTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread);
// 释放时统设置线程
MatrixServerDll_API void OnFreeTimeDelayThread(m_oTimeDelayThreadStruct* pTimeDelayThread);

/************************************************************************/
/* ADC参数设置线程                                                      */
/************************************************************************/
// 创建ADC参数设置线程
MatrixServerDll_API m_oADCSetThreadStruct* OnCreateADCSetThread(void);
// 发送ADC命令设置帧
MatrixServerDll_API void OnSelectADCSetCmd(m_oADCSetThreadStruct* pADCSetThread, bool bRout, 
	unsigned int uiDstIP);
// 手动发送ADC参数设置帧
MatrixServerDll_API bool OnSetADCSetFrameByHand(int iLineIndex, int iPointIndex, int iDirection, bool bRout, 
	char* cpADCSet, int iADCSetNum, m_oEnvironmentStruct* pEnv);
// 发送ADC命令设置帧
MatrixServerDll_API void OnSendADCSetCmd(m_oADCSetThreadStruct* pADCSetThread);
// 处理ADC参数设置应答帧
MatrixServerDll_API void ProcADCSetReturnFrameOne(m_oADCSetThreadStruct* pADCSetThread);
// 输出接收和发送帧的统计结果
MatrixServerDll_API void OnOutPutADCDataRecResult(m_oADCSetThreadStruct* pADCSetThread);
// 命令应答接收完全后的操作
MatrixServerDll_API void OnADCSetNextOpt(m_oADCSetThreadStruct* pADCSetThread);
// 判断ADC参数设置应答是否接收完全
MatrixServerDll_API bool CheckADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread);
// 处理ADC参数设置应答帧
MatrixServerDll_API void ProcADCSetReturnFrame(m_oADCSetThreadStruct* pADCSetThread);
// 线程等待函数
MatrixServerDll_API void WaitADCSetThread(m_oADCSetThreadStruct* pADCSetThread);
// 初始化ADC参数设置线程
MatrixServerDll_API bool OnInitADCSetThread(m_oADCSetThreadStruct* pADCSetThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化ADC参数设置线程
MatrixServerDll_API bool OnInit_ADCSetThread(m_oEnvironmentStruct* pEnv);
// 关闭ADC参数设置线程
MatrixServerDll_API bool OnCloseADCSetThread(m_oADCSetThreadStruct* pADCSetThread);
// 释放ADC参数设置线程
MatrixServerDll_API void OnFreeADCSetThread(m_oADCSetThreadStruct* pADCSetThread);

/************************************************************************/
/* 误码查询线程                                                         */
/************************************************************************/
// 创建误码查询线程
MatrixServerDll_API m_oErrorCodeThreadStruct* OnCreateErrorCodeThread(void);
// 线程等待函数
MatrixServerDll_API void WaitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread);
// 处理单个误码查询应答帧
MatrixServerDll_API void ProcErrorCodeReturnFrameOne(m_oErrorCodeThreadStruct* pErrorCodeThread);
// 处理误码查询应答帧
MatrixServerDll_API void ProcErrorCodeReturnFrame(m_oErrorCodeThreadStruct* pErrorCodeThread);
// 发送误码查询帧
MatrixServerDll_API void ProcErrorCodeQueryFrame(m_oErrorCodeThreadStruct* pErrorCodeThread);
// 初始化误码查询线程
MatrixServerDll_API bool OnInitErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化误码查询线程
MatrixServerDll_API bool OnInit_ErrorCodeThread(m_oEnvironmentStruct* pEnv);
// 关闭误码查询线程
MatrixServerDll_API bool OnCloseErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread);
// 释放误码查询线程
MatrixServerDll_API void OnFreeErrorCodeThread(m_oErrorCodeThreadStruct* pErrorCodeThread);

/************************************************************************/
/* 监视线程                                                             */
/************************************************************************/
// 创建路由监视线程
MatrixServerDll_API m_oMonitorThreadStruct* OnCreateMonitorThread(void);
// 线程等待函数
MatrixServerDll_API void WaitMonitorThread(m_oMonitorThreadStruct* pMonitorThread);
// 沿着路由方向得到时统任务
MatrixServerDll_API void GetTimeDelayTaskAlongRout(m_oRoutStruct* pRout, 
	m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar);
// 得到时统任务
MatrixServerDll_API void GetTimeDelayTask(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar);
// 生成时统任务队列
MatrixServerDll_API void GenTimeDelayTaskQueue(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar);
// ADC参数设置线程开始工作
MatrixServerDll_API void OnADCSetThreadWork(int iOpt, m_oADCSetThreadStruct* pADCSetThread);
// 清除ADC参数设置任务索引
MatrixServerDll_API void OnClearADCSetMap(m_oADCSetThreadStruct* pADCSetThread);
// 将仪器加入ADC参数设置索引表
MatrixServerDll_API void GetADCTaskQueueBySN(m_oADCSetThreadStruct* pADCSetThread, 
	m_oInstrumentStruct* pInstrument, int iOpt);
// 判断路由方向上是否有采集站
MatrixServerDll_API void GetADCTaskQueueByRout(m_oADCSetThreadStruct* pADCSetThread, 
	m_oRoutStruct* pRout, int iOpt);
// 生成ADC参数设置任务队列
MatrixServerDll_API void GetADCTaskQueue(m_oADCSetThreadStruct* pADCSetThread, int iOpt);
// 自动开始ADC参数设置
MatrixServerDll_API void OnADCCmdAuto(m_oADCSetThreadStruct* pADCSetThread, int iOpt);
// ADC参数设置
MatrixServerDll_API void OnADCSetAuto(m_oADCSetThreadStruct* pADCSetThread);
// ADC开始采集命令
MatrixServerDll_API void OnADCStartSampleAuto(m_oADCSetThreadStruct* pADCSetThread);
// ADC停止采集命令
MatrixServerDll_API void OnADCStopSampleAuto(m_oADCSetThreadStruct* pADCSetThread);
// 监视过期路由及仪器
MatrixServerDll_API void MonitorRoutAndInstrument(m_oMonitorThreadStruct* pMonitorThread);
// 检查时统设置应答是否接收完全
MatrixServerDll_API bool CheckTimeDelayReturnByRout(m_oRoutStruct* pRout, 
	m_oTimeDelayThreadStruct* pTimeDelayThread, bool bSample);
// 检查仪器是否都完成了时统
MatrixServerDll_API bool CheckTimeDelayReturn(m_oTimeDelayThreadStruct* pTimeDelayThread);
// 监视时统
MatrixServerDll_API void MonitorTimeDelay(m_oTimeDelayThreadStruct* pTimeDelayThread);
// 监视仪器的ADC参数设置
MatrixServerDll_API void MonitorADCSet(m_oADCSetThreadStruct* pADCSetThread);
// 监视仪器的误码
MatrixServerDll_API void MonitorErrorCode(m_oErrorCodeThreadStruct* pErrorCodeThread);
// 处理路由监视
MatrixServerDll_API void ProcMonitor(m_oMonitorThreadStruct* pMonitorThread);
// 初始化路由监视线程
MatrixServerDll_API bool OnInitMonitorThread(m_oMonitorThreadStruct* pMonitorThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化路由监视线程
MatrixServerDll_API bool OnInit_MonitorThread(m_oEnvironmentStruct* pEnv);
// 关闭路由监视线程
MatrixServerDll_API bool OnCloseMonitorThread(m_oMonitorThreadStruct* pMonitorThread);
// 释放路由监视线程
MatrixServerDll_API void OnFreeMonitorThread(m_oMonitorThreadStruct* pMonitorThread);

/************************************************************************/
/* ADC数据接收线程                                                      */
/************************************************************************/
// 创建ADC数据接收线程
MatrixServerDll_API m_oADCDataRecThreadStruct* OnCreateADCDataRecThread(void);
// 线程等待函数
MatrixServerDll_API void WaitADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread);
// 将ADC数据加入到任务列表
MatrixServerDll_API void AddToADCDataWriteFileList(int iLineIndex, int iPointIndex, 
	unsigned int uiFrameNb, unsigned int uiSN, unsigned int uiSysTime, 
	m_oADCDataRecThreadStruct* pADCDataRecThread);
// 采样数据回调函数
MatrixServerDll_API void GetProSampleDateCallBack(m_oADCDataRecThreadStruct* pADCDataRecThread, 
	ProSampleDateCallBack pCallBack);
// 处理单个ADC数据帧
MatrixServerDll_API void ProcADCDataRecFrameOne(m_oADCDataRecThreadStruct* pADCDataRecThread);
// 处理ADC数据接收帧
MatrixServerDll_API void ProcADCDataRecFrame(m_oADCDataRecThreadStruct* pADCDataRecThread);
// 处理ADC数据重发
MatrixServerDll_API void ProcADCRetransmission(m_oADCDataRecThreadStruct* pADCDataRecThread);
// 初始化ADC数据接收线程
MatrixServerDll_API bool OnInitADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化ADC数据接收线程
MatrixServerDll_API bool OnInit_ADCDataRecThread(m_oEnvironmentStruct* pEnv);
// 关闭ADC数据接收线程
MatrixServerDll_API bool OnCloseADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread);
// 释放ADC数据接收线程
MatrixServerDll_API void OnFreeADCDataRecThread(m_oADCDataRecThreadStruct* pADCDataRecThread);
/************************************************************************/
/* 施工放炮数据存储线程                                                 */
/************************************************************************/
// 创建施工放炮数据存储线程
MatrixServerDll_API m_oADCDataSaveThreadStruct* OnCreateADCDataSaveThread(void);
// 线程等待函数
MatrixServerDll_API void WaitADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread);
// 保存ADC数据到施工文件
MatrixServerDll_API void WriteADCDataInOptTaskFile(m_oADCDataBufStruct* pADCDataBuf, 
	m_oOptTaskStruct* pOptTask, unsigned int uiLineIndex, m_oConstVarStruct* pConstVar);
// 关闭所有的施工文件
MatrixServerDll_API void CloseAllADCDataSaveInFile(m_oOptTaskArrayStruct* pOptTaskArray);
// 保存ADC数据到施工文件
MatrixServerDll_API void ProcADCDataSaveInFile(m_oADCDataSaveThreadStruct* pADCDataSaveThread);
// 初始化施工放炮数据存储线程
MatrixServerDll_API bool OnInitADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread, 
	m_oLogOutPutStruct* pLogOutPut = NULL, m_oConstVarStruct* pConstVar = NULL);
// 初始化施工放炮数据存储线程
MatrixServerDll_API bool OnInit_ADCDataSaveThread(m_oEnvironmentStruct* pEnv);
// 关闭施工放炮数据存储线程
MatrixServerDll_API bool OnCloseADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread);
// 释放施工放炮数据存储线程
MatrixServerDll_API void OnFreeADCDataSaveThread(m_oADCDataSaveThreadStruct* pADCDataSaveThread);

/************************************************************************/
/* 服务程序                                                             */
/************************************************************************/
// 产生一个施工任务
MatrixServerDll_API void GenOneOptTask(unsigned int uiIndex, unsigned int uiStartFrame, 
	m_oOptTaskArrayStruct* pOptTaskArray, m_oInstrumentListStruct* pInstrumentList,
	m_oConstVarStruct* pConstVar);
// 释放一个施工任务
MatrixServerDll_API void FreeOneOptTask(unsigned int uiIndex, 
	m_oOptTaskArrayStruct* pOptTaskArray);
// 按SN重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLableBySN(m_oInstrumentStruct* pInstrument, int iOpt, 
	m_oInstrumentListStruct* pInstrumentList, m_oConstVarStruct* pConstVar);
// 按路由重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLableByRout(m_oRoutStruct* pRout, int iOpt, 
	m_oConstVarStruct* pConstVar);
// 按照路由地址重置ADC参数设置标志位
MatrixServerDll_API void OnSetADCByLAUXSN(int iLineIndex, int iPointIndex, int iDirection, int iOpt, 
	m_oEnvironmentStruct* pEnv, bool bOnly = true, bool bRout = true);
// 重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLable(m_oRoutListStruct* pRoutList, int iOpt, 
	m_oConstVarStruct* pConstVar);
// ADC参数设置
MatrixServerDll_API void OnADCSet(m_oEnvironmentStruct* pEnv);
// ADC开始采集命令
MatrixServerDll_API void OnADCStartSample(m_oEnvironmentStruct* pEnv);
// ADC停止采集命令
MatrixServerDll_API void OnADCStopSample(m_oEnvironmentStruct* pEnv);
// 输出接收和发送帧的统计结果
MatrixServerDll_API void OnOutPutResult(m_oEnvironmentStruct* pEnv);
// 创建实例，并返回实例指针
MatrixServerDll_API m_oEnvironmentStruct* OnCreateInstance(void);
// 初始化实例
MatrixServerDll_API void OnInit(m_oEnvironmentStruct* pEnv, 
	char* pcXMLFilePath, char* pcINIFilePath);
// 关闭
MatrixServerDll_API void OnClose(m_oEnvironmentStruct* pEnv);
// 工作
MatrixServerDll_API void OnWork(m_oEnvironmentStruct* pEnv);
// 停止
MatrixServerDll_API void OnStop(m_oEnvironmentStruct* pEnv);
// 释放实例资源
MatrixServerDll_API void OnFreeInstance(m_oEnvironmentStruct* pEnv);



#undef MatrixServerDll_API
#ifdef __cplusplus
}
#endif

#endif
