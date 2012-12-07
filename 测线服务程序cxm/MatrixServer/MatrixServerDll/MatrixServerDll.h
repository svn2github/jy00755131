// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXSERVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXSERVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#ifndef _MatrixServerDll_H
#define _MatrixServerDll_H
#include "ConfigLineXml.h"
#include "ConfigOperationXml.h"
#include "ConfigPcapXml.h"
#include "Segd.h"
#include "Parameter.h"
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
/**
* @struct LogOutPut_Struct
* @brief 日志输出结构
*/
typedef struct LogOutPut_Struct
{
	/** 日志输出资源同步对象*/
	CRITICAL_SECTION m_oSecLogFile;
	/** 日志输出文件指针*/
	FILE* m_pFile;
	/** 日志输出结构*/
	list<string> m_oLogOutputList;
	/** 错误计数*/
	unsigned int m_uiErrorCount;
	/** 日志输出路径*/
	string m_SaveLogFilePath;
	/** 日志文件序号*/
	unsigned int m_uiLogFileNb;
	/** 日志文件存储信息条数记数*/
	unsigned int m_uiLogInfoCount;
	/** 日志文件类型*/
	char m_cLogFileType;
}m_oLogOutPutStruct;

/**
* @struct ConstVar_Struct
* @brief 从INI文件中解析得到的常量
* @note	该结构体中的变量值只能在INI文件中被修改
* @note 暂不支持在程序中修改（可能会出现内存冲突的情况）
*/
typedef struct ConstVar_Struct
{
	/** INI文件路径*/
	string m_strINIFilePath;
	/** 仪器设备个数*/
	unsigned int m_iInstrumentNum;
	/** SEGD数据缓冲区个数*/
	int m_iSEGDDataBufCountAll;
	/** SEGD数据缓冲区大小*/
	int m_iSEGDDataBufSize;
	/** 判断SEGD数据存储完成的时间*/
	int m_iSEGDCheckFinishTime;
	/** 施工任务个数*/
	int m_iOptTaskCountAll;
	/** 一次休眠的时间*/
	int m_iOneSleepTime;
	/** 日志输出线程写日志的延时次数*/
	int m_iLogOutPutSleepTimes;
	/** 心跳线程发送心跳帧延时次数*/
	int m_iHeartBeatSleepTimes;
	/** 首包线程接收首包延时次数*/
	int m_iHeadFrameSleepTimes;
	/** IP地址设置线程延时次数*/
	int m_iIPSetFrameSleepTimes;
	/** 尾包线程延时次数*/
	int m_iTailFrameSleepTimes;
	/** 路由监视线程延时次数*/
	int m_iMonitorSleepTimes;
	/** 时统设置线程延时次数*/
	int m_iTimeDelaySleepTimes;
	/** ADC参数设置线程延时次数*/
	int m_iADCSetSleepTimes;
	/** 误码查询线程延时次数*/
	int m_iErrorCodeSleepTimes;
	/** ADC数据接收线程延时次数*/
	int m_iADCDataRecSleepTimes;
	/** ADC数据存储线程延时次数*/
	int m_iADCDataSaveSleepTimes;
	/** 等待线程关闭的延时次数*/
	int m_iCloseThreadSleepTimes;
	/** 采集站到采集站的时统修正值*/
	int m_iTimeDelayFDUToFDU;
	/** LCI到采集站的时统修正值*/
	int m_iTimeDelayLCIToFDU;
	/** 采集站到LAUL的时统修正值*/
	int m_iTimeDelayFDUToLAUL;
	/** LAUL到采集站的时统修正值*/
	int m_iTimeDelayLAULToFDU;
	/** 首包稳定计数*/
	int m_iHeadFrameStableTimes;
	/** IP地址重设次数*/
	int m_iIPAddrResetTimes;
	/** 尾包稳定次数*/
	int m_iTailFrameStableTimes;
	/** 路由监视稳定时间*/
	int m_iMonitorStableTime;
	/** 测网系统状态-link*/
	int m_iLineSysStatusLink;
	/** 测网系统状态-稳定*/
	int m_iLineSysStatusStable;
	/** 测网系统状态-噪声采集*/
	int m_iLineSysStatusNoise;
	/** 测网系统状态-TB采集*/
	int m_iLineSysStatusSample;
	/** 测网系统达到稳定状态时间*/
	int m_iLineSysStableTime;
// 	/** ADC参数设置操作序号*/
// 	int m_iADCSetOptNb;
// 	/** ADC开始采集操作序号*/
// 	int m_iADCStartSampleOptNb;
// 	/** ADC停止采集操作序号*/
// 	int m_iADCStopSampleOptNb;
	/** ADC参数设置命令数*/
	int m_iADCSetCmdNum;
	/** ADC参数设置起始命令序号*/
	int m_iADCSetCmdBeginNb;
	/** ADC开始采集命令数*/
	int m_iADCStartSampleCmdNum;
	/** ADC开始采集起始命令序号*/
	int m_iADCStartSampleBeginNb;
	/** ADC停止采集命令数*/
	int m_iADCStopSampleCmdNum;
	/** ADC停止采集起始命令序号*/
	int m_iADCStopSampleBeginNb;
// 	/** 仪器类型-交叉站*/
// 	int m_iInstrumentTypeLAUX;
// 	/** 仪器类型-电源站*/
// 	int m_iInstrumentTypeLAUL;
// 	/** 仪器类型-采集站*/
// 	int m_iInstrumentTypeFDU;
// 	/** 仪器类型-LCI*/
// 	int m_iInstrumentTypeLCI;
// 	/** 方向上方*/
// 	int m_iDirectionTop;
// 	/** 方向下方*/
// 	int m_iDirectionDown;
// 	/** 方向左方*/
// 	int m_iDirectionLeft;
// 	/** 方向右方*/
// 	int m_iDirectionRight;
	/** 方向正中*/
	int m_iDirectionCenter;
	/** IP地址设置的起始地址*/
	int m_iIPSetAddrStart;
	/** IP地址设置的间隔*/
	int m_iIPSetAddrInterval;
	/** 路由地址设置的起始地址*/
	int m_iRoutSetAddrStart;
	/** 路由地址设置的间隔*/
	int m_iRoutSetAddrInterval;
	/** 设置广播端口起始地址*/
	int m_iBroadcastPortStart;
	/** 设置为广播IP*/
	unsigned int m_uiIPBroadcastAddr;
// 	/** 一个文件内存储单个设备ADC数据帧数*/
// 	int m_iADCFrameSaveInOneFileNum;
	/** 存储ADC数据的文件头行数*/
	int m_iADCSaveHeadLineNum;
	/** 存储ADC数据的左侧预留信息字节数*/
	int m_iADCSaveLeftInfoBytes;
	/** 存储ADC数据的字节数*/
	int m_iADCSaveDataBytes;
	/** 存储ADC数据之间的间隔字节数*/
	int m_iADCSaveDataIntervalBytes;
	/** 设备ADC数据缓冲区大小*/
	int m_iADCDataBufSize;
	/** 存储用于测试计算的数据个数*/
	unsigned int m_uiSaveTestDataNum;

	/** 帧头长度*/
	int m_iFrameHeadSize;
	/** 同步帧头*/
	char* m_cpFrameHeadCheck;
	/** 命令字长度1字节*/
	int m_iFrameCmdSize1B;
	/** 命令包长度1字节*/
	int m_iFramePacketSize1B;
	/** 命令包长度2字节*/
	int m_iFramePacketSize2B;
	/** 命令包长度4字节*/
	int m_iFramePacketSize4B;
	/** ADC数据所占字节数*/
	int m_iADCDataSize3B;
	/** 一帧内ADC数据个数*/
	int m_iADCDataInOneFrameNum;
	/** ADC数据帧指针偏移量上限*/
	unsigned short m_usADCFramePointLimit;
	/** 命令字个数最大值*/
	int m_iCommandWordMaxNum;
	/** 0x18命令数组包含的最大字节数*/
	int m_iADCSetCommandMaxByte;
	/** 发送帧缓冲区初值设定*/
	char m_cSndFrameBufInit;
	/** 接收的网络数据帧帧长度*/
	int m_iRcvFrameSize;
	/** 发送的网络数据帧帧长度*/
	int m_iSndFrameSize;
	/**____服务器与设备命令字设置____*/
	/** 发送设置命令*/
	unsigned short m_usSendSetCmd;
	/** 发送查询命令*/
	unsigned short m_usSendQueryCmd;
	/** 发送ADC采样数据重发命令*/
	unsigned short m_usSendADCCmd;
	/** 串号*/
	BYTE m_byCmdSn;
	/** 首包时间*/
	BYTE m_byCmdHeadFrameTime;
	/** 本地IP地址*/
	BYTE m_byCmdLocalIPAddr;
	/** 本地系统时间*/
	BYTE m_byCmdLocalSysTime1;
	/** 本地系统时间*/
	BYTE m_byCmdLocalSysTime2;
	/** 本地系统时间*/
	BYTE m_byCmdLocalSysTime3;
	/** 本地时间修正高位*/
	BYTE m_byCmdLocalTimeFixedHigh;
	/** 本地时间修正低位*/
	BYTE m_byCmdLocalTimeFixedLow;
	/** 自动数据返回地址*/
	BYTE m_byCmdADCDataReturnAddr;
	/** 自动数据返回端口和命令*/
	BYTE m_byCmdADCDataReturnPort;
	/** 端口递增下限和上限*/
	BYTE m_byCmdADCDataReturnPortLimit;
	/** 设置网络等待端口和命令*/
	BYTE m_byCmdBroadCastPortSet;
	/** 系统硬件状态拷贝*/
	BYTE m_byCmdFDUErrorCode;
	/** TB时刻高位*/
	BYTE m_byCmdTBHigh;
	/** TB时刻低位*/
	BYTE m_byCmdTbLow;
	/** work_ctrl 交叉站方向*/
	BYTE m_byCmdLAUXRoutOpenQuery;
	/** 路由开关*/
	BYTE m_byCmdLAUXRoutOpenSet;
	/** 仪器设备版本*/
	BYTE m_byCmdVersion;
	/** 尾包接收\发送时刻低位*/
	BYTE m_byCmdTailRecSndTime1;
	/** 尾包接收\发送时刻低位*/
	BYTE m_byCmdTailRecSndTime2;
	/** 尾包接收\发送时刻低位*/
	BYTE m_byCmdTailRecSndTime3;
	/** 广播命令等待端口匹配*/
	BYTE m_byCmdBroadCastPortSeted;
	/** 设置ADC控制命令命令字*/
	BYTE m_byCmdADCSet;
	/** 网络时刻*/
	BYTE m_byCmdNetTime;
	/** 交叉站大线尾包接收时刻*/
	BYTE m_byCmdLineTailRecTimeLAUX;
	/** 交叉站交叉线尾包接收时刻*/
	BYTE m_byCmdLAUTailRecTimeLAUX;
	/** 交叉站故障1*/
	BYTE m_byCmdLAUXErrorCode1;
	/** 交叉站故障2*/
	BYTE m_byCmdLAUXErrorCode2;
	/** 交叉站路由分配*/
	BYTE m_byCmdLAUXSetRout;
	/** 返回路由*/
	BYTE m_byCmdReturnRout;
	/** 命令解析结束命令*/
	BYTE m_byCmdEnd;

	/** TB设置延时高位*/
	unsigned int m_uiTBSleepTimeHigh;
	/** TB设置延时低位*/
	unsigned short m_usTBSleepTimeLow;
	/** FieldOn与上一次FieldOff的时间间隔*/
	unsigned int m_uiFieldOnWaitTimeLimit;
	/** TB控制，0x0001 启动TB进行ADC数据采集*/
	unsigned short m_usCmdTBCtrlStartSample;
	/** 0x0002 无需TB开关控制ADC数据采集命令*/
	unsigned short m_usCmdTBLoseCtrlStartSample;
	/** 0x0000 TB开关控制ADC数据停止采集命令*/
	unsigned short m_usCmdTBCtrlStopSample;
	/** LED灯灭*/
	unsigned short m_usCmdCtrlCloseLed;
}m_oConstVarStruct;

/**
* @struct XMLIPSetupData_Struct
* @brief 从XML文件中解析得到IP地址设置数据
*/
typedef struct XMLIPSetupData_Struct
{
	/** 源地址*/
	unsigned int m_uiSrcIP;
	/** LCI的IP地址*/
	unsigned int m_uiAimIP;
	/** 自动数据返回地址*/
	unsigned int m_uiADCDataReturnAddr;
}m_oXMLIPSetupDataStruct;

/**
* @struct XMLPortSetupData_Struct
* @brief 从XML文件中解析得到端口设置数据
*/ 
typedef struct XMLPortSetupData_Struct
{
	/** LCI接收的端口号*/
	unsigned short m_usAimPort;
	/** 心跳帧返回端口*/
	unsigned short m_usHeartBeatReturnPort;
	/** 首包接收端口*/
	unsigned short m_usHeadFramePort;
	/** IP地址设置返回端口*/
	unsigned short m_usIPSetReturnPort;
	/** 尾包接收端口*/
	unsigned short m_usTailFramePort;
	/** 尾包时刻查询接收端口*/
	unsigned short m_usTailTimeReturnPort;
	/** 时统设置应答端口*/
	unsigned short m_usTimeDelayReturnPort;
	/** ADC参数设置应答端口*/
	unsigned short m_usADCSetReturnPort;
	/** 误码查询返回端口*/
	unsigned short m_usErrorCodeReturnPort;
	/** ADC数据返回端口*/
	unsigned short m_usADCDataReturnPort;
}m_oXMLPortSetupDataStruct;

/**
* @struct XMLADCSetupData_Struct
* @brief 从XML文件中解析得到ADC参数设置信息
*/
typedef struct XMLADCSetupData_Struct
{
	/** ADC设置正弦波命令大小*/
	int m_iSetADCSetSineSize;
	/** ADC设置正弦波命令*/
	char* m_cpSetADCSetSine;
	/** ADC设置停止采样命令大小*/
	int m_iSetADCStopSampleSize;
	/** ADC设置停止采样命令*/
	char* m_cpSetADCStopSample;
	/** ADC设置打开TB电源低位大小*/
	int m_iSetADCOpenTBPowerLowSize;
	/** ADC设置打开TB电源低位*/
	char* m_cpSetADCOpenTBPowerLow;
	/** ADC设置打开TB电源高位大小*/
	int m_iSetADCOpenTBPowerHighSize;
	/** ADC设置打开TB电源高位*/
	char* m_cpSetADCOpenTBPowerHigh;
	/** ADC设置打开TB开关低位大小*/
	int m_iSetADCOpenSwitchTBLowSize;
	/** ADC设置打开TB开关低位*/
	char* m_cpSetADCOpenSwitchTBLow;
	/** ADC设置打开TB开关高位大小*/
	int m_iSetADCOpenSwitchTBHighSize;
	/** ADC设置打开TB开关高位*/
	char* m_cpSetADCOpenSwitchTBHigh;
	/** ADC设置读寄存器大小*/
	int m_iSetADCRegisterReadSize;
	/** ADC设置读寄存器*/
	char* m_cpSetADCRegisterRead;
	/** ADC设置写寄存器大小*/
	int m_iSetADCRegisterWriteSize;
	/** ADC设置写寄存器*/
	char* m_cpSetADCRegisterWrite;
	/** ADC设置打开TB开关大小*/
	int m_iSetADCTBSwitchOpenSize;
	/** ADC设置打开TB开关*/
	char* m_cpSetADCTBSwitchOpen;
	/** ADC采样设置大小*/
	int m_iSetADCSampleSize;
	/** ADC采样设置*/
	char* m_cpSetADCSample;
	/** ADC设置连续采样大小*/
	int m_iSetADCReadContinuousSize;
	/** ADC设置连续采样*/
	char* m_cpSetADCReadContinuous;
	/** 采样率设置*/
	int m_iSampleRate;
	/** 高通滤波器是否开启*/
	bool m_bHPFOpen;
}m_oXMLADCSetupDataStruct;

/**
* @struct XMLParameterSetupData_Struct
* @brief 从XML文件中得到的服务端参数信息
*/
typedef struct XMLParameterSetupData_Struct
{
	CTime m_oTimeFieldOff;
	/** 端口转发程序接收端口偏移量*/
	unsigned short m_usNetRcvPortMove;
}m_oXMLParameterSetupDataStruct;
/** 
* @struct ServerSetupData_Struct
* @brief 从服务程序XML文件中解析得到的信息结构体
*/
typedef struct ServerSetupData_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecCommInfo;
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	/** Server的XML配置文件路径*/
	string m_strServerXMLFilePath;
	/** 从XML文件中解析得到IP地址设置数据*/
	m_oXMLIPSetupDataStruct m_oXMLIPSetupData;
	/** 从XML文件中解析得到端口设置数据*/
	m_oXMLPortSetupDataStruct m_oXMLPortSetupData;
	/** 从XML文件中解析得到ADC参数设置信息*/
	m_oXMLADCSetupDataStruct m_oXMLADCSetupData;
	/** 从XML文件中解析得到的服务端参数信息*/
	m_oXMLParameterSetupDataStruct m_oXMLParameterSetupData;
}m_oServerSetupDataStruct;
/**
* @struct InstrumentCommInfo_Struct
* @brief 从XML文件中解析得到的信息结构体
*/
typedef struct InstrumentCommInfo_Struct
{
	/** 服务端信息*/
	m_oServerSetupDataStruct* m_pServerSetupData;
	/** 测线客户端信息*/
	m_oLineSetupDataStruct* m_pLineSetupData;
	/** 施工客户端信息*/
	m_oOptSetupDataStruct* m_pOptSetupData;
	/** Pcap程序信息*/
	m_oNetPcapSetupDataStruct* m_pPcapSetupData;
}m_oInstrumentCommInfoStruct;

/**
* @struct InstrumentCommand_Struct
* @brief 与设备通讯命令字内容结构体
*/
typedef struct InstrumentCommand_Struct
{
	/** LCI的IP地址*/
	unsigned int m_uiAimIP;
	/** LCI接收的端口号*/
	unsigned short m_usAimPort;
	/** 源地址*/
	unsigned int m_uiSrcIP;
	/** 目的地址*/
	unsigned int m_uiDstIP;
	/** 应答端口*/
	unsigned short m_usReturnPort;
	/** 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发*/
	unsigned short m_usCommand;
	/** SN，低8位为仪器类型，0x01为交叉站，0x02为电源站，0x03为采集站*/
	unsigned int m_uiSN;
	/** 首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	/** 仪器IP地址*/
	unsigned int m_uiInstrumentIP;
	/** 本地时间*/
	unsigned short m_usSysTimeNewLow;
	/** 本地时间*/
	unsigned int m_uiSysTimeNewHigh;
	/** 本地时间*/
	unsigned short m_usSysTimeOldLow;
	/** 本地时间*/
	unsigned int m_uiSysTimeOldHigh;
	/** 本地时间修正高位*/
	unsigned int m_uiLocalTimeFixedHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiLocalTimeFixedLow;
	/** 自动数据返回地址*/
	unsigned int m_uiADCDataReturnAddr;
	/** 自动数据返回端口*/
	unsigned short m_usADCDataReturnPort;
	/** 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变*/
	unsigned short m_usADCDataReturnCmd;
	/** 当返回命令高位为1时，返回端口自动加1，到高限时返回低限端口*/
	/** 端口递增下限*/
	unsigned short m_usADCDataReturnPortLimitLow;
	/** 端口递增上限*/
	unsigned short m_usADCDataReturnPortLimitHigh;
	/** 设置网络等待端口，指设置接收上位机广播命令的端口*/
	unsigned int m_uiBroadCastPortSet;
	/** 网络数据错误计数*/
	char m_cFDUErrorCodeDataCount;
	/** 命令错误计数*/
	char m_cFDUErrorCodeCmdCount;
	/** 时间报文状态*/
	char m_cTimeStatus;
	/** 控制状态*/
	char m_cCtrlStatus;
	/** TB时刻高位*/
	unsigned int m_uiTBHigh;
	/** TB时刻低位*/
	unsigned short m_usTBLow;
	/** TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭*/
	unsigned short m_usTBCtrl;
	/** work_ctrl控制交叉站接收和发送命令的方向*/
	/** 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B*/
	/** =0为开，=1为关*/
	char m_cLAUXRoutOpenQuery;
	/** 路由开关*/
	/** 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B*/
	/** =0为开，=1为关*/
	char m_cLAUXRoutOpenSet;
	/** 硬件设备软件版本号*/
	unsigned int m_uiVersion;
	// @@@时间48位
// 	/** 尾包接收时刻，低14位有效*/
// 	unsigned short m_usTailRecTime;
// 	/** 尾包发送时刻/**交叉站尾包发送时刻，低14位有效*/
// 	unsigned short m_usTailSndTime;
// 	/** 尾包接收时刻低位*/
// 	unsigned short m_usTailRecTimeLow;
	/** 尾包接收时刻高位*/
	unsigned int m_uiTailRecTime;
// 	/** 尾包发送时刻/交叉站尾包发送时刻低位*/
// 	unsigned short m_usTailSndTimeLow;
	/** 尾包发送时刻/交叉站尾包发送时刻高位*/
	unsigned int m_uiTailSndTime;
	/** 低位锁存时间*/
	unsigned int m_uiLowTime;
	/** 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令*/
	unsigned int m_uiBroadCastPortSeted;
	/** 网络时刻*/
	unsigned int m_uiNetTime;
	/** 交叉站大线A尾包接收时刻*/
	unsigned short m_usLAUXTailRecTimeLineA;
	/** 交叉站大线B尾包接收时刻*/
	unsigned short m_usLAUXTailRecTimeLineB;
	/** 交叉站交叉线A尾包接收时刻*/
	unsigned short m_usLAUXTailRecTimeLAUXLineA;
	/** 交叉站交叉线B尾包接收时刻*/
	unsigned short m_usLAUXTailRecTimeLAUXLineB;
	/** 交叉站大线A数据故障*/
	char m_cLAUXErrorCodeDataLineACount;
	/** 交叉站大线B数据故障*/
	char m_cLAUXErrorCodeDataLineBCount;
	/** 交叉站交叉线A数据故障*/
	char m_cLAUXErrorCodeDataLAUXLineACount;
	/** 交叉站交叉线B数据故障*/
	char m_cLAUXErrorCodeDataLAUXLineBCount;
	/** 交叉站命令口故障*/
	char m_cLAUXErrorCodeCmdCount;
	/** 交叉站上方路由IP*/
	unsigned int m_uiRoutIPTop;
	/** 交叉站下方路由IP*/
	unsigned int m_uiRoutIPDown;
	/** 交叉站左方路由IP*/
	unsigned int m_uiRoutIPLeft;
	/** 交叉站右方路由IP*/
	unsigned int m_uiRoutIPRight;
	/** 路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 0x18命令数组*/
	char* m_cpADCSet;
	/** 0x18命令数据个数*/
	int m_iADCSetNum;
	/** ADC数据指针偏移量*/
	unsigned short m_usADCDataPoint;
	/** ADC数据采集时仪器本地时间高位*/
	unsigned int m_uiADCSampleSysTimeHigh;
	/** ADC数据采集时仪器本地时间低位*/
	unsigned short m_usADCSampleSysTimeLow;
	/** ADC采样数据缓冲区指针*/
	char* m_pADCDataBuf;
}m_oInstrumentCommandStruct;

/**
* @struct HeartBeatFrame_Struct
* @brief 心跳帧结构体
*/
typedef struct HeartBeatFrame_Struct
{
	/** 心跳帧资源同步对象*/
	CRITICAL_SECTION m_oSecHeartBeat;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** 心跳命令字集合*/
	BYTE* m_pbyCommandWord;
	/** 心跳命令字个数*/
	unsigned short m_usCommandWordNum;
	/** 心跳帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStruct;
	/** 心跳Socket套接字*/
	SOCKET m_oHeartBeatSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oHeartBeatFrameStruct;

/**
* @struct HeadFrame_Struct
* @brief 首包帧结构体
*/
typedef struct HeadFrame_Struct
{
	/** 首包帧资源同步对象*/
	CRITICAL_SECTION m_oSecHeadFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** 首包帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStruct;
	/** 首包Socket套接字*/
	SOCKET m_oHeadFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oHeadFrameStruct;

/**
* @struct IPSetFrame_Struct
* @brief IP地址设置帧结构体
*/
typedef struct IPSetFrame_Struct
{
	/** IP地址设置帧资源同步对象*/
	CRITICAL_SECTION m_oSecIPSetFrame;
	/** 网络端口发送缓冲区大小*/
	unsigned int m_uiSndBufferSize;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** IP地址设置命令字集合*/
	BYTE* m_pbyCommandWord;
	/** IP地址设置命令字个数*/
	unsigned short m_usCommandWordNum;
	/** IP地址设置帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** IP地址设置应答帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	/** IP地址设置Socket套接字*/
	SOCKET m_oIPSetFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oIPSetFrameStruct;

/**
* @struct TailFrame_Struct
* @brief 尾包帧结构体
*/
typedef struct TailFrame_Struct
{
	/** 尾包帧资源同步对象*/
	CRITICAL_SECTION m_oSecTailFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** 尾包帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStruct;
	/** 尾包Socket套接字*/
	SOCKET m_oTailFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oTailFrameStruct;

/**
* @struct TailTimeFrame_Struct
* @brief 尾包时刻帧结构体
*/
typedef struct TailTimeFrame_Struct
{
	/** 尾包时刻帧资源同步对象*/
	CRITICAL_SECTION m_oSecTailTimeFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** 尾包时刻应答帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	/** 网络端口发送缓冲区大小*/
	unsigned int m_uiSndBufferSize;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** 尾包时刻查询命令字集合*/
	BYTE* m_pbyCommandWord;
	/** 尾包时刻查询命令字个数*/
	unsigned short m_usCommandWordNum;
	/** 尾包时刻发送帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	/** 尾包时刻Socket套接字*/
	SOCKET m_oTailTimeFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oTailTimeFrameStruct;

/**
* @struct TimeDelayFrame_Struct
* @brief 时统设置帧结构体
*/
typedef struct TimeDelayFrame_Struct
{
	/** 时统设置帧资源同步对象*/
	CRITICAL_SECTION m_oSecTimeDelayFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** 时统设置应答帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	/** 网络端口发送缓冲区大小*/
	unsigned int m_uiSndBufferSize;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** 时统设置命令字集合*/
	BYTE* m_pbyCommandWord;
	/** 时统设置命令字个数*/
	unsigned short m_usCommandWordNum;
	/** 时统设置发送帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	/** 时统设置Socket套接字*/
	SOCKET m_oTimeDelayFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oTimeDelayFrameStruct;

/**
* @struct ADCSetFrame_Struct
* @brief ADC参数设置帧结构体
*/
typedef struct ADCSetFrame_Struct
{
	/** ADC参数设置帧资源同步对象*/
	CRITICAL_SECTION m_oSecADCSetFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** ADC参数设置应答帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	/** 网络端口发送缓冲区大小*/
	unsigned int m_uiSndBufferSize;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** ADC参数设置命令字集合*/
	BYTE* m_pbyCommandWord;
	/** ADC参数设置命令字个数*/
	unsigned short m_usCommandWordNum;
	/** ADC参数设置发送帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	/** ADC参数设置Socket套接字*/
	SOCKET m_oADCSetFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oADCSetFrameStruct;

/**
* @struct ErrorCodeFrame_Struct
* @brief 误码查询帧结构体
*/
typedef struct ErrorCodeFrame_Struct
{
	/** ADC参数设置帧资源同步对象*/
	CRITICAL_SECTION m_oSecErrorCodeFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** 应答帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	/** 网络端口发送缓冲区大小*/
	unsigned int m_uiSndBufferSize;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** 误码查询命令字集合*/
	BYTE* m_pbyCommandWord;
	/** 误码查询命令字个数*/
	unsigned short m_usCommandWordNum;
	/** 发送帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	/** 误码查询Socket套接字*/
	SOCKET m_oErrorCodeFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oErrorCodeFrameStruct;

/**
* @struct ADCDataFrame_Struct
* @brief ADC数据接收帧结构体
*/
typedef struct ADCDataFrame_Struct
{
	/** ADC数据接收帧资源同步对象*/
	CRITICAL_SECTION m_oSecADCDataFrame;
	/** 网络端口接收缓冲区大小*/
	unsigned int m_uiRcvBufferSize;
	/** 接收帧缓冲区*/
	char* m_cpRcvFrameData;
	/** 应答帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	/** 网络端口发送缓冲区大小*/
	unsigned int m_uiSndBufferSize;
	/** 发送帧缓冲区*/
	char* m_cpSndFrameData;
	/** 发送帧命令*/
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	/** ADC数据接收和重发Socket套接字*/
	SOCKET m_oADCDataFrameSocket;
	/** 接收端口偏移量*/
	unsigned short m_usPortMove;
}m_oADCDataFrameStruct;

/**
* @struct Instrument_Struct
* @brief 仪器属性结构体
*/
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
	/** 仪器软件版本号*/
	unsigned int m_uiVersion;
	/** 尾包计数*/
	int m_iTailFrameCount;
	/** 仪器时延*/
	unsigned int m_uiDelayTime;
	// @@@时间48位
// 	/** 仪器本地系统时间*/
// 	unsigned int m_uiSystemTime;
// 	/** 仪器网络时间*/
// 	unsigned int m_uiNetTime;
	/** 仪器网络状态*/
	unsigned int m_uiNetState;
	/** 仪器参数备用1*/
	unsigned int m_uiNetOrder;
// 	/** 16bits时间，接收时刻低位*/
// 	unsigned short m_usReceiveTime;	
// 	/** 16bits时间，发送时刻低位*/
// 	unsigned short m_usSendTime;
// 	/** 16bits时间，接收时刻低位*/
// 	unsigned short m_usReceiveTimeLow;
	/** 32bits时间，接收时刻高位*/
	unsigned int m_uiReceiveTime;
// 	/** 16bits时间，发送时刻低位*/
// 	unsigned short m_usSendTimeLow;
	/** 32bits时间，发送时刻高位*/
	unsigned int m_uiSendTime;
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
	/** 首包位置稳定次数*/
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
	/** ADC命令设置是否应答*/
	bool m_bADCSetReturn;
	/** 仪器是否进行了ADC参数设置*/
	bool m_bADCSet;
	/** 仪器开始ADC数据采集设置成功*/
	bool m_bADCStartSample;
	/** 仪器停止ADC数据采集设置成功*/
	bool m_bADCStopSample;
	/** 误码查询发送帧数*/
	unsigned int m_uiErrorCodeQueryNum;
	/** 误码查询应答帧数*/
	unsigned int m_uiErrorCodeReturnNum;
	/** 采集站和电源站网络数据错误计数*/
	int m_iFDUErrorCodeDataCount;
	/** 采集站和电源站命令错误计数*/
	int m_iFDUErrorCodeCmdCount;
	/** 采集站和电源站网络数据错误计数*/
	char m_cFDUErrorCodeDataCountOld;
	/** 采集站和电源站命令错误计数*/
	char m_cFDUErrorCodeCmdCountOld;

	/** 交叉站大线A数据故障*/
	int m_iLAUXErrorCodeDataLineACount;
	/** 交叉站大线B数据故障*/
	int m_iLAUXErrorCodeDataLineBCount;
	/** 交叉站交叉线A数据故障*/
	int m_iLAUXErrorCodeDataLAUXLineACount;
	/** 交叉站交叉线B数据故障*/
	int m_iLAUXErrorCodeDataLAUXLineBCount;
	/** 交叉站命令口故障*/
	int m_iLAUXErrorCodeCmdCount;

	/** 交叉站大线A数据故障*/
	char m_cLAUXErrorCodeDataLineACountOld;
	/** 交叉站大线B数据故障*/
	char m_cLAUXErrorCodeDataLineBCountOld;
	/** 交叉站交叉线A数据故障*/
	char m_cLAUXErrorCodeDataLAUXLineACountOld;
	/** 交叉站交叉线B数据故障*/
	char m_cLAUXErrorCodeDataLAUXLineBCountOld;
	/** 交叉站命令口故障*/
	char m_cLAUXErrorCodeCmdCountOld;

	/** 实际接收ADC数据帧数（含重发帧）*/
	unsigned int m_uiADCDataActualRecFrameNum;
	/** 重发查询帧得到的应答帧数*/
	unsigned int m_uiADCDataRetransmissionFrameNum;
	/** 应该接收ADC数据帧数（含丢帧）*/
	unsigned int m_uiADCDataShouldRecFrameNum;
	/** ADC采样数据（取每帧的第一个数据用于计算）*/
	list<int> m_olsADCDataSave;
	/** ADC数据帧的指针偏移量*/
	unsigned short m_usADCDataFramePointNow;
	/** ADC数据帧发送时的本地时间*/
	unsigned int m_uiADCDataFrameSysTimeNow;
	/** ADC数据帧的指针偏移量*/
	unsigned short m_usADCDataFramePointOld;
	/** ADC数据帧发送时的本地时间*/
	unsigned int m_uiADCDataFrameSysTimeOld;
	/** 仪器开始采样的TB时刻高位*/
	unsigned int m_uiTBHigh;
	/** 检查是否晚或者没有收到ADC数据帧*/
	bool m_bCheckADCFrameLate;
	/** 仪器存活时间*/
	unsigned int m_uiActiveTime;
	/** 本地系统时间高位*/
	unsigned int m_uiSysTimeHigh;
	/** 本地系统时间低位*/
	unsigned short m_usSysTimeLow;
}m_oInstrumentStruct;

/**
* @struct InstrumentLocation_Struct
* @brief 仪器位置点结构体
*/
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
	/** 线号*/
	int m_iLineIndex;
	/** 点号*/
	int m_iPointIndex;
}m_oInstrumentLocationStruct;

/**
* @struct ADCLostFrameKey_Struct
* @brief 丢失帧IP地址和偏移量结构体
*/
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
	/** 丢帧的指针偏移量*/
	unsigned short m_usADCFramePointNb;
	/** 丢帧的IP地址*/
	unsigned int m_uiIP;
}m_oADCLostFrameKeyStruct;

/**
* @struct ADCLostFrame_Struct
* @brief 丢失帧结构体
*/
typedef struct ADCLostFrame_Struct
{
	/** 丢失帧重发次数*/
	unsigned int m_uiCount;
// 	/** 丢帧在文件内的帧序号，从0开始*/
// 	unsigned int m_uiFrameNb;
	/** 丢失帧的本地时间*/
	unsigned int m_uiSysTime;
	/** 是否已经收到应答*/
	bool m_bReturnOk;
}m_oADCLostFrameStruct;

/**
* @struct InstrumentList_Struct
* @brief 仪器队列结构体
*/
typedef struct InstrumentList_Struct
{
	/** 仪器数组指针*/
	m_oInstrumentStruct* m_pArrayInstrument;
	/** 空闲仪器队列*/
	list<m_oInstrumentStruct*> m_olsInstrumentFree;
	/** 设置IP地址的仪器队列*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPSetInstrumentMap;
	/** 仪器SN索引表*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oSNInstrumentMap;
	/** 仪器IP地址索引表*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPInstrumentMap;
	/** 未完成ADC参数设置的仪器索引*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oADCSetInstrumentMap;
	/** 仪器位置索引表*/
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*> m_oInstrumentLocationMap;
	/** 参与施工采集的仪器索引表*/
	// @@@暂未使用
	hash_map<unsigned int, m_oInstrumentStruct*> m_oOptInstrumentMap;
	/** 采样丢帧索引表*/
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct> m_oADCLostFrameMap;
	/** 仪器总数*/
	unsigned int m_uiCountAll;
	/** 空闲仪器数量*/
	unsigned int m_uiCountFree;
	/** 是否按照路由IP手动设置ADC参数*/
	bool m_bSetByHand;
}m_oInstrumentListStruct;

/**
* @struct Rout_Struct
* @brief 路由属性结构体
*/
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
	/** 路由是否为交叉线路由*/
	bool m_bRoutLaux;
	/** 路由是否接收到ADC设置参数应答*/
	bool m_bADCSetReturn;
	/** 路由是否发送ADC参数设置帧*/
	bool m_bADCSetRout;
	/** 该路由方向上仪器的个数*/
	unsigned int m_uiInstrumentNum;
}m_oRoutStruct;

/**
* @struct RoutList_Struct
* @brief 路由队列结构体
*/
typedef struct RoutList_Struct
{
	/** 路由数组指针*/
	m_oRoutStruct* m_pArrayRout;
	/** 空闲路由队列*/
	list<m_oRoutStruct*> m_olsRoutFree;
	/** 仪器路由地址索引表*/
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutMap;
	/** 需要删除路由仪器的路由索引表*/
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutDeleteMap;
// 	/** 仪器时统的任务队列，成员为路由IP*/
// 	list<unsigned int> m_olsTimeDelayTaskQueue;
// 	/** 仪器时统的任务中转队列*/
// 	list<unsigned int> m_olsTimeDelayTemp;
	/** ADC参数设置任务索引*/
	hash_map<unsigned int, m_oRoutStruct*> m_oADCSetRoutMap;
	/** 路由总数*/
	unsigned int m_uiCountAll;
	/** 空闲路由数量*/
	unsigned int m_uiCountFree;
}m_oRoutListStruct;

/**
* @struct LineList_Struct
* @brief 测线队列结构体
*/
typedef struct LineList_Struct
{
	/** 测线队列资源同步对象*/
	CRITICAL_SECTION m_oSecLineList;
	/** 仪器队列结构体指针*/
	m_oInstrumentListStruct* m_pInstrumentList;
	/** 路由队列结构体*/
	m_oRoutListStruct* m_pRoutList;
	/** 测网状态由不稳定变为稳定*/
	bool m_bLineStableChange;
	/** 测网系统发生变化的时间*/
	unsigned int m_uiLineChangeTime;
	/** TB时间高位*/
	unsigned int m_uiTBHigh;
	/** LCI的本地系统时间*/
	unsigned int m_uiLocalSysTime;
}m_oLineListStruct;

/**
* @struct OptInstrument_Struct
* @brief 参与施工的仪器结构体
*/
typedef struct OptInstrument_Struct
{
	/** 仪器物理IP*/
	unsigned int m_uiIP;
	/** 标记线号*/
	int m_iLineIndex;
	/** 标记点号*/
	int m_iPointIndex;
	/** 采集站仪器是否用作辅助道*/
	bool m_bAuxiliary;
	/** 仪器存储成SEGD的位置号*/
	unsigned int m_uiLocation;
}m_oOptInstrumentStruct;

/**
* @struct OptTask_Struct
* @brief 施工任务结构体
*/
typedef struct OptTask_Struct
{
	/** 任务是否使用中*/
	bool m_bInUsed;
	/** 施工炮号*/
	unsigned int m_uiOptNo;
	/** 施工任务开始记录的时间*/
	unsigned int m_uiTB;
	/** 施工任务停止记录的时间*/
	unsigned int m_uiTS;
	/** 采样率*/
	int m_iSampleRate;
// 	/** 存储一帧所需时间*/
// 	unsigned int m_uiOneFrameTime;
	/** 施工数据输出文件指针*/
	FILE* m_pFile;
// 	/** 施工数据输出前一个文件的文件指针*/
// 	FILE* m_pPreviousFile;
// 	/** 最新的文件存储序号*/
// 	unsigned int m_uiFileSaveNb;
	/** 施工数据存储文件路径*/
	string m_SaveFilePath;
	/** 施工任务索引表，关键字为IP，内容为参与施工的仪器结构体*/
	hash_map<unsigned int, m_oOptInstrumentStruct*> m_oIPMap;
	/** 分配存储单元标志位*/
	bool m_bSaveBuf;
	/** 分配存储单元序号*/
	unsigned int m_uiSaveBufNo;
// 	/** 参与施工的仪器队列*/
// 	list<m_oOptInstrumentStruct> m_olsOptInstrument;
}m_oOptTaskStruct;

/**
* @struct OptTaskArray_Struct
* @brief 施工任务结构体数组
*/
typedef struct OptTaskArray_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecOptTaskArray;
	/** 施工任务数组指针*/
	m_oOptTaskStruct* m_pArrayOptTask;
	/** 空闲的施工任务队列*/
	list<m_oOptTaskStruct*> m_olsOptTaskFree;
	/** 正在进行的施工任务索引*/
	hash_map<unsigned int, m_oOptTaskStruct*> m_oOptTaskWorkMap;
	/** 施工任务总数*/
	unsigned int m_uiCountAll;
	/** 空闲任务数量*/
	unsigned int m_uiCountFree;
	/** 施工任务数据存储文件夹计数*/
	unsigned int m_uiOptTaskNb;
	/** 施工数据存储文件夹路径*/
	string m_SaveFolderPath;
}m_oOptTaskArrayStruct;
/**
* @struct ADCDataBuf_Struct
* @brief 数据存储缓冲区结构体
*/
typedef struct ADCDataBuf_Struct
{
	/** 缓冲区是否使用中*/
	bool m_bInUsed;
	/** ADC数据存储缓冲区*/
	char* m_pADCDataBuf;
	/** 缓冲区长度*/
	unsigned int m_uiBufLength;
	/** 每个站存储的数据点数*/
	unsigned int m_uiSavePointNum;
	/** 采样时间*/
	unsigned int m_uiSampleTime;
	/** 判断是否写入SEGD文件标志位*/
	bool m_bSaveInSegd;
	/** 参与施工的采集站个数*/
	unsigned int m_uiAcqTraceNum;
	/** 辅助道数目*/
	unsigned int m_uiAuxTraceNum;
	/** 施工炮号*/
	unsigned int m_uiOptNo;
	/** 施工数据存储文件路径*/
	string m_SaveFilePath;
	/** 采样率*/
	int m_iSampleRate;
	// 	/** 每个采集站数据存储帧数*/
	// 	unsigned int m_uiFrameNb;
	// 	/** SEGD协议头长度*/
	// 	unsigned int m_uiSEGDHeaderLen;
	// 	/** 采集站数据头长度*/
	// 	unsigned int m_uiDataHeaderLen;
	/** 施工任务索引表指针，关键字为IP，内容为参与施工的Segd数据头结构体*/
	list<m_oSegdDataHeaderStruct*> m_olsSegdDataHeader;
	/** 缓冲区序号*/
	unsigned int m_uiIndex;
}m_oADCDataBufStruct;

/**
* @struct ADCDataBufArray_Struct
* @brief 数据存储缓冲区结构体
*/
typedef struct ADCDataBufArray_Struct
{
	/** 缓冲区资源同步对象*/
	CRITICAL_SECTION m_oSecADCDataBufArray;
	/** 缓冲区数组指针*/
	m_oADCDataBufStruct* m_pArrayADCDataBuf;
	/** 缓冲区队列*/
	list<m_oADCDataBufStruct*> m_olsADCDataBufFree;
	/** 写入文件的数据缓冲区索引*/
	hash_map<unsigned int, m_oADCDataBufStruct*> m_oADCDataBufWorkMap;
	/** 缓冲区总数*/
	unsigned int m_uiCountAll;
	/** 空闲缓冲区数量*/
	unsigned int m_uiCountFree;
}m_oADCDataBufArrayStruct;

/**
* @struct Thread_Struct
* @brief 线程结构体
*/
typedef struct Thread_Struct
{
	/** 线程句柄*/
	HANDLE m_hThread;
	/** 线程号*/
	DWORD m_dwThreadID;
	/**是否工作状态*/
	bool m_bWork;
	/** 是否关闭线程*/
	bool m_bClose;
	/** 线程结束事件*/
	HANDLE m_hThreadClose;
	/** 常量指针*/
	m_oConstVarStruct* m_pConstVar;
	/** 输出日志指针*/
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oThreadStruct;

/**
* @struct LogOutPutThread_Struct
* @brief 日志输出线程结构体
*/
typedef struct LogOutPutThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecLogOutPutThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 输出日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
	/** 输出日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
	/** 输出日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
}m_oLogOutPutThreadStruct;

/**
* @struct HeartBeatThread_Struct
* @brief 心跳线程结构体
*/
typedef struct HeartBeatThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecHeartBeatThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 心跳帧指针*/
	m_oHeartBeatFrameStruct* m_pHeartBeatFrame;
}m_oHeartBeatThreadStruct;

/**
* @struct HeadFrameThread_Struct
* @brief 首包线程结构体
*/
typedef struct HeadFrameThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecHeadFrameThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 首包帧指针*/
	m_oHeadFrameStruct* m_pHeadFrame;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** 首包计数*/
	unsigned int m_uiHeadFrameCount;
}m_oHeadFrameThreadStruct;

/**
* @struct IPSetFrameThread_Struct
* @brief IP地址设置线程结构体
*/
typedef struct IPSetFrameThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecIPSetFrameThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** IP地址设置帧指针*/
	m_oIPSetFrameStruct* m_pIPSetFrame;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
}m_oIPSetFrameThreadStruct;

/**
* @struct TimeDelayThread_Struct
* @brief 时统线程结构体
*/
typedef struct TimeDelayThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecTimeDelayThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 尾包时刻帧指针*/
	m_oTailTimeFrameStruct* m_pTailTimeFrame;
	/** 时统设置帧指针*/
	m_oTimeDelayFrameStruct* m_pTimeDelayFrame;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** ADC开始数据采集标志位*/
	bool m_bADCStartSample;
	/** 计数器*/
//	unsigned int m_uiCounter;
	/** 输出日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
}m_oTimeDelayThreadStruct;

/**
* @struct TailFrameThread_Struct
* @brief 尾包线程结构体
*/
typedef struct TailFrameThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecTailFrameThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 尾包帧指针*/
	m_oTailFrameStruct* m_pTailFrame;
	/** 仪器队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** 尾包计数*/
	unsigned int m_uiTailFrameCount;
	/** 时统线程指针*/
	m_oTimeDelayThreadStruct* m_pTimeDelayThread;
}m_oTailFrameThreadStruct;
/**
* @struct ADCSetThread_Struct
* @brief ADC参数设置线程结构体
*/
typedef struct ADCSetThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecADCSetThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** ADC参数设置帧指针*/
	m_oADCSetFrameStruct* m_pADCSetFrame;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** ADC命令设置序号*/
	int m_iADCSetOperationNb;
	/** 计数器*/
	unsigned int m_uiCounter;
	/** ADC开始数据采集标志位*/
	bool m_bADCStartSample;
	/** ADC停止数据采集标志位*/
	bool m_bADCStopSample;
// 	/** 上一次开始采样的采样时间*/
// 	unsigned int m_uiLocalSysTimeOld;
	/** 从XML文件中解析得到的信息*/
	m_oInstrumentCommInfoStruct* m_pCommInfo;
	/** 输出日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
}m_oADCSetThreadStruct;

/**
* @struct ErrorCodeThread_Struct
* @brief 误码查询线程结构体
*/
typedef struct ErrorCodeThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecErrorCodeThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 误码查询帧指针*/
	m_oErrorCodeFrameStruct* m_pErrorCodeFrame;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** 误码查询日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
}m_oErrorCodeThreadStruct;

/**
* @struct MonitorThread_Struct
* @brief 路由监视线程结构体
*/
typedef struct MonitorThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecMonitorThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** 时统线程指针*/
	m_oTimeDelayThreadStruct* m_pTimeDelayThread;
	/** ADC参数设置线程*/
	m_oADCSetThreadStruct* m_pADCSetThread;
	/** 误码查询线程*/
	m_oErrorCodeThreadStruct* m_pErrorCodeThread;
	/** IP地址设置帧结构*/
	m_oIPSetFrameStruct* m_pIPSetFrame;
}m_oMonitorThreadStruct;

/**
* @struct ADCDataRecThread_Struct
* @brief ADC数据接收线程结构体
*/
typedef struct ADCDataRecThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecADCDataRecThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 误码查询帧指针*/
	m_oADCDataFrameStruct* m_pADCDataFrame;
	/** 测线队列结构体指针*/
	m_oLineListStruct* m_pLineList;
	/** 误码查询日志指针*/
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
	/** 数据存储缓冲区数组结构体指针*/
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	/** 施工任务数组结构体指针*/
	m_oOptTaskArrayStruct* m_pOptTaskArray;
	/** 采样率设置*/
	int m_iADCSampleRate;
//	/** 上一帧的本地时间*/
//	unsigned int m_uiADCDataFrameSysTime;
//	/** 存文件数据帧数计数*/
//	int m_iADCFrameCount;
// 	/** 采样数据回调函数*/
// 	ProSampleDateCallBack m_oProSampleDataCallBack;
}m_oADCDataRecThreadStruct;

/**
* @struct ADCDataSaveThread_Struct
* @brief 施工放炮数据存储线程结构体
*/
typedef struct ADCDataSaveThread_Struct
{
	/** 资源同步对象*/
	CRITICAL_SECTION m_oSecADCDataSaveThread;
	/** 线程结构体指针*/
	m_oThreadStruct* m_pThread;
	/** 数据存储缓冲区结构体指针*/
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
}m_oADCDataSaveThreadStruct;
/**
* @struct Environment_Struct
* @brief 资源环境结构体
*/
typedef struct Environment_Struct
{
	/** 从INI文件得到的常量信息*/
	m_oConstVarStruct* m_pConstVar;
	/** 从XML文件得到的与仪器通讯信息结构*/
	m_oInstrumentCommInfoStruct* m_pInstrumentCommInfo;
	/** 心跳帧结构*/
	m_oHeartBeatFrameStruct* m_pHeartBeatFrame;
	/** 首包帧结构*/
	m_oHeadFrameStruct* m_pHeadFrame;
	/** IP地址设置帧结构*/
	m_oIPSetFrameStruct* m_pIPSetFrame;
	/** 尾包帧结构*/
	m_oTailFrameStruct* m_pTailFrame;
	/** 尾包时刻帧结构*/
	m_oTailTimeFrameStruct* m_pTailTimeFrame;
	/** 时统设置帧结构*/
	m_oTimeDelayFrameStruct* m_pTimeDelayFrame;
	/** ADC参数设置帧结构*/
	m_oADCSetFrameStruct* m_pADCSetFrame;
	/** 误码查询帧结构*/
	m_oErrorCodeFrameStruct* m_pErrorCodeFrame;
	/** ADC数据帧结构*/
	m_oADCDataFrameStruct* m_pADCDataFrame;
	/** 操作日志输出结构*/
	m_oLogOutPutStruct* m_pLogOutPutOpt;
	/** 时统日志输出结构*/
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
	/** 误码查询日志输出结构*/
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
	/** 帧时间和偏移量日志输出结构*/
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
	/** 测线队列结构体*/
	m_oLineListStruct* m_pLineList;
	/** 数据存储缓冲区结构体指针*/
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	/** 施工任务数组结构体指针*/
	m_oOptTaskArrayStruct* m_pOptTaskArray;

	/** 日志输出线程*/
	m_oLogOutPutThreadStruct* m_pLogOutPutThread;
	/** 心跳线程*/
	m_oHeartBeatThreadStruct* m_pHeartBeatThread;
	/** 首包接收线程*/
	m_oHeadFrameThreadStruct* m_pHeadFrameThread;
	/** IP地址设置线程*/
	m_oIPSetFrameThreadStruct* m_pIPSetFrameThread;
	/** 尾包接收线程*/
	m_oTailFrameThreadStruct* m_pTailFrameThread;
	/** 路由监视线程*/
	m_oMonitorThreadStruct* m_pMonitorThread;
	/** 时统线程*/
	m_oTimeDelayThreadStruct* m_pTimeDelayThread;
	/** ADC参数设置线程*/
	m_oADCSetThreadStruct* m_pADCSetThread;
	/** 误码查询线程*/
	m_oErrorCodeThreadStruct* m_pErrorCodeThread;
	/** ADC数据接收线程*/
	m_oADCDataRecThreadStruct* m_pADCDataRecThread;
	/** 施工放炮数据存储线程*/
	m_oADCDataSaveThreadStruct* m_pADCDataSaveThread;
	/** Field On*/
	bool m_bFieldOn;
	/** Field Off*/
	bool m_bFieldOff;
	/** Netd程序的进程信息*/
	PROCESS_INFORMATION m_piNetd;
}m_oEnvironmentStruct;

/** functions declarations*/
/************************************************************************/
/* 功能函数                                                             */
/************************************************************************/
/**
* @fn void OnInitSocketLib(void)
* @detail 初始化套接字库
*/
MatrixServerDll_API	void OnInitSocketLib(void);
/**
* @fn void OnCloseSocketLib(void)
* @detail 释放套接字库
*/
MatrixServerDll_API	void OnCloseSocketLib(void);
// 得到帧的帧内容
MatrixServerDll_API void GetFrameInfo(char* pFrameData, int iSize, string* strFrameData);
// 从CString转换为char数组
MatrixServerDll_API void ParseCStringToArray(char** pData, int iSize, CString str);
// 判断文件是否存在
MatrixServerDll_API bool IfFileExist(CString str);
// 校验帧的同步码
MatrixServerDll_API bool CheckInstrFrameHead(char* pFrameData, char* pFrameHeadCheck, int iCheckSize);
// 生成帧的同步码
MatrixServerDll_API bool MakeInstrFrameHead(char* pFrameData, char* pFrameHeadCheck, int iCheckSize);
// 重置帧内容解析变量
MatrixServerDll_API bool ResetInstrFramePacket(m_oInstrumentCommandStruct* pCommand);
// 解析与设备通讯接收帧内容
MatrixServerDll_API bool ParseInstrFrame(m_oInstrumentCommandStruct* pCommand, 
	char* pFrameData, m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);
// 生成与设备通讯帧
MatrixServerDll_API bool MakeInstrFrame(m_oInstrumentCommandStruct* pCommand, 
	m_oConstVarStruct* pConstVar, char* pFrameData, BYTE* pCommandWord = NULL, 
	unsigned short usCommandWordNum = 0);
// 创建CSocket接收端口并绑定端口和IP地址
MatrixServerDll_API SOCKET CreateInstrSocket(unsigned short usPort, 
	unsigned int uiIP, m_oLogOutPutStruct* pLogOutPut = NULL);
// 设置广播模式
MatrixServerDll_API void SetInstrSocketBroadCast(SOCKET oSocket, 
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
// 关闭Socket套接字
MatrixServerDll_API void OnCloseSocket(SOCKET oSocket);
// 清空接收缓冲区
MatrixServerDll_API void OnClearSocketRcvBuf(SOCKET oSocket, int iRcvFrameSize);
// 得到路由方向上仪器个数
MatrixServerDll_API bool OnGetRoutInstrNum(int iLineIndex, int iPointIndex, int iDirection, 
	m_oEnvironmentStruct* pEnv, unsigned int& uiInstrumentNum);
// 计算测试数据的算术均方根
MatrixServerDll_API double CalMeanSquare(m_oInstrumentStruct* pInstrument);

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
MatrixServerDll_API bool LoadIniFile(m_oConstVarStruct* pConstVar);
// 初始化常量信息结构体
MatrixServerDll_API bool OnInitConstVar(m_oConstVarStruct* pConstVar);
// 关闭常量信息结构体
MatrixServerDll_API void OnCloseConstVar(m_oConstVarStruct* pConstVar);
// 释放常量信息结构体
MatrixServerDll_API void OnFreeConstVar(m_oConstVarStruct* pConstVar);

/************************************************************************/
/* xml载入                                                              */
/************************************************************************/
// 创建仪器通讯信息结构体
MatrixServerDll_API m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void);
// 初始化仪器通讯信息结构体
MatrixServerDll_API void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo);
// 释放仪器通讯信息结构体
MatrixServerDll_API void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo);

// 初始化测线客户程序设置信息
MatrixServerDll_API void OnInitLineClientXMLSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 重置Survery
MatrixServerDll_API void OnResetSurveryList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Point Code
MatrixServerDll_API void OnResetPointCodeList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Sensor
MatrixServerDll_API void OnResetSensorList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Marker
MatrixServerDll_API void OnResetMarkerList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Aux
MatrixServerDll_API void OnResetAuxList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Detour
MatrixServerDll_API void OnResetDetourList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Mute
MatrixServerDll_API void OnResetMuteList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置BlastMachine
MatrixServerDll_API void OnResetBlastMachineList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Absolute
MatrixServerDll_API void OnResetAbsoluteMap(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Generic
MatrixServerDll_API void OnResetGenericList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置FormLine
MatrixServerDll_API void OnResetFormLineList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Instrument_SensorTestBase
MatrixServerDll_API void OnResetInstrument_SensorTestBaseList(bool bInstrument, m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Instrument_SensorTestLimit
MatrixServerDll_API void OnResetInstrument_SensorTestLimitList(bool bInstrument, m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Instrument Test
MatrixServerDll_API void OnResetInstrumentTestList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Sensor Test
MatrixServerDll_API void OnResetSensorTestList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Multiple Test
MatrixServerDll_API void OnResetMultipleTestMap(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置SeisMonitor
MatrixServerDll_API void OnResetSeisMonitor(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置测线客户端信息
MatrixServerDll_API void OnResetLineClientXMLSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 打开测线客户端程序配置文件
MatrixServerDll_API BOOL OpenLineClientXMLFile(m_oLineSetupDataStruct* pLineSetupData);
// 关闭测线客户端程序配置文件
MatrixServerDll_API void CloseLineClientXMLFile(m_oLineSetupDataStruct* pLineSetupData);
// 加载Survery设置数据
MatrixServerDll_API void LoadSurverySetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Survery设置数据
MatrixServerDll_API void SaveSurverySetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Survery设置数据
MatrixServerDll_API void SetSurverySetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 SurveyXML 文件信息
MatrixServerDll_API void QuerySurverySetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Point Code设置数据
MatrixServerDll_API void LoadPointCodeSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Point Code设置数据
MatrixServerDll_API void SavePointCodeSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Point Code设置数据
MatrixServerDll_API void SetPointCodeSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 PointCode XML文件信息
MatrixServerDll_API void QueryPointCodeSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Sensor设置数据
MatrixServerDll_API void LoadSensorSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Sensor设置数据
MatrixServerDll_API void SaveSensorSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Sensor设置数据
MatrixServerDll_API void SetSensorSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Sensor XML文件信息
MatrixServerDll_API void QuerySensorSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Marker设置数据
MatrixServerDll_API void LoadMarkerSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Marker设置数据
MatrixServerDll_API void SaveMarkerSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Marker设置数据
MatrixServerDll_API void SetMarkerSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Marker XML文件信息
MatrixServerDll_API void QueryMarkerSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Aux设置数据
MatrixServerDll_API void LoadAuxSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Aux设置数据
MatrixServerDll_API void SaveAuxSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Aux设置数据
MatrixServerDll_API void SetAuxSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Aux XML文件信息
MatrixServerDll_API void QueryAuxSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Detour设置数据
MatrixServerDll_API void LoadDetourSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Detour设置数据
MatrixServerDll_API void SaveDetourSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Detour设置数据
MatrixServerDll_API void SetDetourSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Detour XML文件信息
MatrixServerDll_API void QueryDetourSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Mute设置数据
MatrixServerDll_API void LoadMuteSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Mute设置数据
MatrixServerDll_API void SaveMuteSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Mute设置数据
MatrixServerDll_API void SetMuteSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Mute XML文件信息
MatrixServerDll_API void QueryMuteSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载BlastMachine设置数据
MatrixServerDll_API void LoadBlastMachineSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存BlastMachine设置数据
MatrixServerDll_API void SaveBlastMachineSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置BlastMachine设置数据
MatrixServerDll_API void SetBlastMachineSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 BlastMachine XML文件信息
MatrixServerDll_API void QueryBlastMachineSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Absolute设置数据
MatrixServerDll_API void LoadAbsoluteSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Absolute设置数据
MatrixServerDll_API void SaveAbsoluteSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Absolute设置数据
MatrixServerDll_API void SetAbsoluteSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Absolute XML文件信息
MatrixServerDll_API void QueryAbsoluteSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Generic设置数据
MatrixServerDll_API void LoadGenericSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Generic设置数据
MatrixServerDll_API void SaveGenericSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Generic设置数据
MatrixServerDll_API void SetGenericSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Generic XML文件信息
MatrixServerDll_API void QueryGenericSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Look设置数据
MatrixServerDll_API void LoadLookSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Look设置数据
MatrixServerDll_API void SaveLookSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Look设置数据
MatrixServerDll_API void SetLookSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Look XML文件信息
MatrixServerDll_API void QueryLookSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载LAULeakage设置数据
MatrixServerDll_API void LoadLAULeakageSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存LAULeakage设置数据
MatrixServerDll_API void SaveLAULeakageSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置LAULeakage设置数据
MatrixServerDll_API void SetLAULeakageSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 LAULeakage XML文件信息
MatrixServerDll_API void QueryLAULeakageSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载FormLine设置数据
MatrixServerDll_API void LoadFormLineSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存FormLine设置数据
MatrixServerDll_API void SaveFormLineSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置FormLine设置数据
MatrixServerDll_API void SetFormLineSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 FormLine XML文件信息
MatrixServerDll_API void QueryFormLineSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Instrument_SensorTestBase设置数据
MatrixServerDll_API void LoadInstrument_SensorTestBaseSetupData(bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
// 保存Instrument_SensorTestBase设置数据
MatrixServerDll_API void SaveInstrument_SensorTestBaseSetupData(bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
// 设置Instrument_SensorTestBase设置数据
MatrixServerDll_API void SetInstrument_SensorTestBaseSetupData(char* pChar, unsigned int uiSize, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 Instrument_SensorTestBase XML文件信息
MatrixServerDll_API void QueryInstrument_SensorTestBaseSetupData(char* cProcBuf, int& iPos, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
// 加载Instrument_SensorTestLimit设置数据
MatrixServerDll_API void LoadInstrument_SensorTestLimitSetupData(bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
// 保存Instrument_SensorTestLimit设置数据
MatrixServerDll_API void SaveInstrument_SensorTestLimitSetupData(bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
// 设置Instrument_SensorTestLimit设置数据
MatrixServerDll_API void SetInstrument_SensorTestLimitSetupData(char* pChar, unsigned int uiSize, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 InstrumentTestLimit XML文件信息
MatrixServerDll_API void QueryInstrument_SensorTestLimitSetupData(char* cProcBuf, int& iPos, bool bInstrument, m_oLineSetupDataStruct* pLineSetupData);
// 加载Instrument Test设置数据
MatrixServerDll_API void LoadInstrumentTestSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Instrument Test设置数据
MatrixServerDll_API void SaveInstrumentTestSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Instrument Test设置数据
MatrixServerDll_API void SetInstrumentTestSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 InstrumentTest XML文件信息
MatrixServerDll_API void QueryInstrumentTestSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Sensor Test设置数据
MatrixServerDll_API void LoadSensorTestSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Sensor Test设置数据
MatrixServerDll_API void SaveSensorTestSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Sensor Test设置数据
MatrixServerDll_API void SetSensorTestSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 SensorTest XML文件信息
MatrixServerDll_API void QuerySensorTestSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载Multiple Test设置数据
MatrixServerDll_API void LoadMultipleTestSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存Multiple Test设置数据
MatrixServerDll_API void SaveMultipleTestSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置Multiple Test设置数据
MatrixServerDll_API void SetMultipleTestSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 MultipleTest XML文件信息
MatrixServerDll_API void QueryMultipleTestSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 加载SeisMonitor设置数据
MatrixServerDll_API void LoadSeisMonitorSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存SeisMonitor设置数据
MatrixServerDll_API void SaveSeisMonitorSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 设置SeisMonitor设置数据
MatrixServerDll_API void SetSeisMonitorSetupData(char* pChar, unsigned int uiSize, m_oLineSetupDataStruct* pLineSetupData, bool bSave = true);
// 查询 SeisMonitorTest XML文件信息
MatrixServerDll_API void QuerySeisMonitorSetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData);
// 从XML配置文件得到测试数据限制值
MatrixServerDll_API float QueryTestDataLimit(bool bInstrument, string str, m_oLineSetupDataStruct* pLineSetupData);
// 创建测线客户端通讯信息结构体
MatrixServerDll_API m_oLineSetupDataStruct* OnCreateLineAppSetupData(void);
// 加载测线客户端程序设置数据
MatrixServerDll_API void LoadLineAppSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 保存测线客户端程序设置数据
MatrixServerDll_API void SaveLineAppSetupData(m_oLineSetupDataStruct* pLineSetupData);
// 释放测线客户端参数设置信息结构体缓冲区
MatrixServerDll_API void OnFreeLineXMLSetupData(m_oLineSetupDataStruct* pLineSetupData);

// 初始化施工客户程序设置信息
MatrixServerDll_API void OnInitOptClientXMLSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 重置炮点队列
MatrixServerDll_API void OnResetOptSourceShotList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Explo震源类型队列
MatrixServerDll_API void OnResetOptExploList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置Vibro震源类型队列
MatrixServerDll_API void OnResetOptVibroList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置处理类型Aux队列
MatrixServerDll_API void OnResetOptProcessAuxList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置处理类型Acq队列
MatrixServerDll_API void OnResetOptProcessAcqList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置处理类型队列
MatrixServerDll_API void OnResetOptProcessTypeList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置注释队列
MatrixServerDll_API void OnResetOptCommentList(m_oInstrumentCommInfoStruct* pCommInfo);
// 重置施工客户端信息
MatrixServerDll_API void OnResetOptClientXMLSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 打开施工客户端程序配置文件
MatrixServerDll_API BOOL OpenOptClientXMLFile(m_oOptSetupDataStruct* pOptSetupData);
// 关闭施工客户端程序配置文件
MatrixServerDll_API void CloseOptClientXMLFile(m_oOptSetupDataStruct* pOptSetupData);
// 加载Delay设置数据
MatrixServerDll_API void LoadOptDelaySetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存Delay设置数据
MatrixServerDll_API void SaveDelaySetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置Delay设置数据
MatrixServerDll_API void SetDelaySetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 Delay XML文件信息
MatrixServerDll_API void QueryDelaySetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载SourceShot设置数据
MatrixServerDll_API void LoadSourceShotSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存SourceShot设置数据
MatrixServerDll_API void SaveSourceShotSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置SourceShot设置数据
MatrixServerDll_API void SetSourceShotSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 SourceShot XML文件信息
MatrixServerDll_API void QuerySourceShotSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载Explo设置数据
MatrixServerDll_API void LoadExploSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存Explo设置数据
MatrixServerDll_API void SaveExploSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置Explo设置数据
MatrixServerDll_API void SetExploSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 Explo XML文件信息
MatrixServerDll_API void QueryExploSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载Vibro设置数据
MatrixServerDll_API void LoadVibroSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存Vibro设置数据
MatrixServerDll_API void SaveVibroSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置Vibro设置数据
MatrixServerDll_API void SetVibroSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 Vibro XML文件信息
MatrixServerDll_API void QueryVibroSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载ProcessRecord设置数据
MatrixServerDll_API void LoadOptProcessRecordSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存ProcessRecord设置数据
MatrixServerDll_API void SaveProcessRecordSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessRecord设置数据
MatrixServerDll_API void SetProcessRecordSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 ProcessRecord XML文件信息
MatrixServerDll_API void QueryProcessRecordSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载ProcessAux设置数据
MatrixServerDll_API void LoadProcessAuxSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存ProcessAux设置数据
MatrixServerDll_API void SaveProcessAuxSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessAux设置数据
MatrixServerDll_API void SetProcessAuxSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 ProcessAux XML文件信息
MatrixServerDll_API void QueryProcessAuxSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载ProcessAcq设置数据
MatrixServerDll_API void LoadProcessAcqSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存ProcessAcq设置数据
MatrixServerDll_API void SaveProcessAcqSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessAcq设置数据
MatrixServerDll_API void SetProcessAcqSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 ProcessAcq XML文件信息
MatrixServerDll_API void QueryProcessAcqSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载ProcessType设置数据
MatrixServerDll_API void LoadProcessTypeSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存ProcessType设置数据
MatrixServerDll_API void SaveProcessTypeSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessType设置数据
MatrixServerDll_API void SetProcessTypeSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 ProcessType XML文件信息
MatrixServerDll_API void QueryProcessTypeSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 加载ProcessComments设置数据
MatrixServerDll_API void LoadProcessCommentsSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存ProcessComments设置数据
MatrixServerDll_API void SaveProcessCommentsSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessComments设置数据
MatrixServerDll_API void SetProcessCommentsSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
// 查询 ProcessComments XML文件信息
MatrixServerDll_API void QueryProcessCommentsSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 创建施工客户端通讯信息结构体
MatrixServerDll_API m_oOptSetupDataStruct* OnCreateOptAppSetupData(void);
// 加载施工客户端程序设置数据
MatrixServerDll_API void LoadOptAppSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 保存施工客户端程序设置数据
MatrixServerDll_API void SaveOptAppSetupData(m_oOptSetupDataStruct* pOptSetupData);
// 释放施工客户端参数设置信息结构体缓冲区
MatrixServerDll_API void OnFreeOptXMLSetupData(m_oOptSetupDataStruct* pOptSetupData);

// 初始化服务程序设置信息
MatrixServerDll_API void OnInitServerXMLSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 打开服务程序配置文件
MatrixServerDll_API BOOL OpenServerXMLFile(m_oServerSetupDataStruct* pServerSetupData);
// 关闭服务程序配置文件
MatrixServerDll_API void CloseServerXMLFile(m_oServerSetupDataStruct* pServerSetupData);
// 加载IP地址设置数据
MatrixServerDll_API void LoadServerIPSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 加载端口设置数据
MatrixServerDll_API void LoadServerPortSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 加载ADC参数设置数据
MatrixServerDll_API void LoadServerADCSetSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 加载服务器端参数设置数据
MatrixServerDll_API void LoadServerParameterSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 保存服务器端参数设置数据
MatrixServerDll_API void SaveServerParameterSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 创建服务端通讯信息结构体
MatrixServerDll_API m_oServerSetupDataStruct* OnCreateServerAppSetupData(void);
// 加载服务端程序设置数据
MatrixServerDll_API void LoadServerAppSetupData(m_oServerSetupDataStruct* pServerSetupData);
// 初始化Pcap程序配置信息结构体
MatrixServerDll_API void OnInitPcapXMLSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 创建Pcap程序信息结构体
MatrixServerDll_API m_oNetPcapSetupDataStruct* OnCreatePcapAppSetupData(void);
// 打开Pcap程序配置文件
MatrixServerDll_API BOOL OpenPcapXMLFile(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 关闭Pcap程序配置文件
MatrixServerDll_API void ClosePcapXMLFile(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 加载PcapIP设置数据
MatrixServerDll_API void LoadPcapIPSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 加载Pcap端口设置数据
MatrixServerDll_API void LoadPcapPortSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 加载Pcap参数设置数据
MatrixServerDll_API void LoadPcapParamSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 加载Pcap程序配置信息
MatrixServerDll_API void LoadPcapAppSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 释放Pcap程序配置信息结构体缓冲区
MatrixServerDll_API void OnFreePcapXMLSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData);
// 释放服务端参数设置信息结构体缓冲区
MatrixServerDll_API void OnFreeServerXMLSetupData(m_oServerSetupDataStruct* pServerSetupData);

// 得到测线接收区域
MatrixServerDll_API void GetLineRevSection(unsigned int& uiLineNum, unsigned int& uiColumnNum, m_oLineSetupDataStruct* pLineSetupData);
/************************************************************************/
/* 心跳帧                                                               */
/************************************************************************/
// 创建心跳帧信息结构体
MatrixServerDll_API m_oHeartBeatFrameStruct* OnCreateInstrHeartBeat(void);
// 初始化心跳
MatrixServerDll_API void OnInitInstrHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭心跳帧信息结构体
MatrixServerDll_API void OnCloseInstrHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame);
// 释放心跳帧信息结构体
MatrixServerDll_API void OnFreeInstrHeartBeat(m_oHeartBeatFrameStruct* pHeartBeatFrame);
// 创建并设置心跳端口
MatrixServerDll_API void OnCreateAndSetHeartBeatSocket(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 生成心跳帧
MatrixServerDll_API void MakeInstrHeartBeatFrame(m_oHeartBeatFrameStruct* pHeartBeatFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* 首包帧                                                               */
/************************************************************************/
// 创建首包帧信息结构体
MatrixServerDll_API m_oHeadFrameStruct* OnCreateInstrHeadFrame(void);
// 初始化首包
MatrixServerDll_API void OnInitInstrHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭首包帧信息结构体
MatrixServerDll_API void OnCloseInstrHeadFrame(m_oHeadFrameStruct* pHeadFrame);
// 释放首包帧信息结构体
MatrixServerDll_API void OnFreeInstrHeadFrame(m_oHeadFrameStruct* pHeadFrame);
// 创建并设置首包端口
MatrixServerDll_API void OnCreateAndSetHeadFrameSocket(m_oHeadFrameStruct* pHeadFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析首包帧
MatrixServerDll_API bool ParseInstrHeadFrame(m_oHeadFrameStruct* pHeadFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* IP地址设置帧                                                         */
/************************************************************************/
// 创建IP地址设置帧信息结构体
MatrixServerDll_API m_oIPSetFrameStruct* OnCreateInstrIPSetFrame(void);
// 初始化IP地址设置
MatrixServerDll_API void OnInitInstrIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭IP地址设置帧信息结构体
MatrixServerDll_API void OnCloseInstrIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame);
// 释放IP地址设置帧信息结构体
MatrixServerDll_API void OnFreeInstrIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame);
// 创建并设置IP地址设置端口
MatrixServerDll_API void OnCreateAndSetIPSetFrameSocket(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析IP地址设置应答帧
MatrixServerDll_API bool ParseInstrIPSetReturnFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);
// 打开交叉站某一路由方向的电源
MatrixServerDll_API bool OpenLAUXRoutPower(int iLineIndex, int iPointIndex, unsigned char ucLAUXRoutOpenSet, 
	m_oEnvironmentStruct* pEnv);
// 生成IP地址查询帧
MatrixServerDll_API void MakeInstrIPQueryFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP, m_oLogOutPutStruct* pLogOutPut);
// 生成IP地址设置帧
MatrixServerDll_API void MakeInstrIPSetFrame(m_oIPSetFrameStruct* pIPSetFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* 尾包帧                                                               */
/************************************************************************/
// 创建尾包帧信息结构体
MatrixServerDll_API m_oTailFrameStruct* OnCreateInstrTailFrame(void);
// 初始化尾包
MatrixServerDll_API void OnInitInstrTailFrame(m_oTailFrameStruct* pTailFrame, 
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭尾包帧信息结构体
MatrixServerDll_API void OnCloseInstrTailFrame(m_oTailFrameStruct* pTailFrame);
// 释放尾包帧信息结构体
MatrixServerDll_API void OnFreeInstrTailFrame(m_oTailFrameStruct* pTailFrame);
// 创建并设置尾包端口
MatrixServerDll_API void OnCreateAndSetTailFrameSocket(m_oTailFrameStruct* pTailFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析尾包帧
MatrixServerDll_API bool ParseInstrTailFrame(m_oTailFrameStruct* pTailFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);


/************************************************************************/
/* 尾包时刻帧                                                           */
/************************************************************************/
// 创建尾包时刻帧信息结构体
MatrixServerDll_API m_oTailTimeFrameStruct* OnCreateInstrTailTimeFrame(void);
// 初始化尾包时刻帧
MatrixServerDll_API void OnInitInstrTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭尾包时刻帧信息结构体
MatrixServerDll_API void OnCloseInstrTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame);
// 释放尾包时刻帧信息结构体
MatrixServerDll_API void OnFreeInstrTailTimeFrame(m_oTailTimeFrameStruct* pTailTimeFrame);
// 创建并设置尾包时刻端口
MatrixServerDll_API void OnCreateAndSetTailTimeFrameSocket(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析尾包时刻查询帧
MatrixServerDll_API bool ParseInstrTailTimeReturnFrame(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);
// 按IP地址查询尾包时刻帧
MatrixServerDll_API void MakeInstrTailTimeQueryFramebyIP(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiInstrumentIP, m_oLogOutPutStruct* pLogOutPut);
// 广播查询尾包时刻帧
MatrixServerDll_API void MakeInstrTailTimeQueryFramebyBroadCast(m_oTailTimeFrameStruct* pTailTimeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* 时统帧                                                               */
/************************************************************************/
// 创建时统设置帧信息结构体
MatrixServerDll_API m_oTimeDelayFrameStruct* OnCreateInstrTimeDelayFrame(void);
// 初始化时统设置帧
MatrixServerDll_API void OnInitInstrTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭时统设置帧信息结构体
MatrixServerDll_API void OnCloseInstrTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame);
// 释放时统设置帧信息结构体
MatrixServerDll_API void OnFreeInstrTimeDelayFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame);
// 创建并设置时统设置端口
MatrixServerDll_API void OnCreateAndSetTimeDelayFrameSocket(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析时统设置应答帧
MatrixServerDll_API bool ParseInstrTimeDelayReturnFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);
// 生成时统设置帧
MatrixServerDll_API void MakeInstrDelayTimeFrame(m_oTimeDelayFrameStruct* pTimeDelayFrame, 
	m_oConstVarStruct* pConstVar, m_oInstrumentStruct* pInstrument, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* ADC参数设置帧                                                        */
/************************************************************************/
// 创建ADC参数设置帧信息结构体
MatrixServerDll_API m_oADCSetFrameStruct* OnCreateInstrADCSetFrame(void);
// 初始化ADC参数设置帧
MatrixServerDll_API void OnInitInstrADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭ADC参数设置帧信息结构体
MatrixServerDll_API void OnCloseInstrADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame);
// 释放ADC参数设置帧信息结构体
MatrixServerDll_API void OnFreeInstrADCSetFrame(m_oADCSetFrameStruct* pADCSetFrame);
// 创建并设置ADC参数设置端口
MatrixServerDll_API void OnCreateAndSetADCSetFrameSocket(m_oADCSetFrameStruct* pADCSetFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析ADC参数设置应答帧
MatrixServerDll_API bool ParseInstrADCSetReturnFrame(m_oADCSetFrameStruct* pADCSetFrame,
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* 误码查询帧                                                           */
/************************************************************************/
// 创建误码查询帧信息结构体
MatrixServerDll_API m_oErrorCodeFrameStruct* OnCreateInstrErrorCodeFrame(void);
// 初始化误码查询帧
MatrixServerDll_API void OnInitInstrErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭误码查询帧信息结构体
MatrixServerDll_API void OnCloseInstrErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame);
// 释放误码查询帧信息结构体
MatrixServerDll_API void OnFreeInstrErrorCodeFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame);
// 创建并设置误码查询端口
MatrixServerDll_API void OnCreateAndSetErrorCodeFrameSocket(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析误码查询应答帧
MatrixServerDll_API bool ParseInstrErrorCodeReturnFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);
// 广播查询误码
MatrixServerDll_API void MakeInstrErrorCodeQueryFrame(m_oErrorCodeFrameStruct* pErrorCodeFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiBroadCastPort, m_oLogOutPutStruct* pLogOutPut);

/************************************************************************/
/* ADC数据帧                                                            */
/************************************************************************/
// 创建ADC数据帧信息结构体
MatrixServerDll_API m_oADCDataFrameStruct* OnCreateInstrADCDataFrame(void);
// 初始化ADC数据帧
MatrixServerDll_API void OnInitInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame,
	m_oInstrumentCommInfoStruct* pCommInfo, m_oConstVarStruct* pConstVar);
// 关闭ADC数据帧信息结构体
MatrixServerDll_API void OnCloseInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame);
// 释放ADC数据帧信息结构体
MatrixServerDll_API void OnFreeInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame);
// 创建并设置ADC数据帧端口
MatrixServerDll_API void OnCreateAndSetADCDataFrameSocket(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oLogOutPutStruct* pLogOutPut = NULL);
// 解析ADC数据接收帧
MatrixServerDll_API bool ParseInstrADCDataRecFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar, m_oLogOutPutStruct* pLogOutPut);
// 生成ADC数据查询帧
MatrixServerDll_API void MakeInstrADCDataFrame(m_oADCDataFrameStruct* pADCDataFrame, 
	m_oConstVarStruct* pConstVar, unsigned int uiIP, unsigned short usDataPoint, 
	m_oLogOutPutStruct* pLogOutPut);

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
// 从索引表删除索引号指向的仪器指针
MatrixServerDll_API BOOL DeleteInstrumentFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oInstrumentLocationStruct, m_oInstrumentStruct*>* pMap);
// 得到仪器在某一方向上的路由IP
MatrixServerDll_API bool GetRoutIPBySn(unsigned int uiSN, int iDirection, 
	m_oInstrumentListStruct* pInstrumentList, unsigned int& uiRoutIP);
// 更新仪器的存活时间
MatrixServerDll_API void UpdateInstrActiveTime(m_oInstrumentStruct* pInstrument);
/**
* 根据链接方向，得到连接的下一个仪器
	* @param unsigned int uiRoutDirection 方向 1-上；2-下；3-左；4右
* @return CInstrument* 仪器指针 NLLL：连接的下一个仪器不存在
	*/
MatrixServerDll_API m_oInstrumentStruct* GetNextInstrAlongRout(m_oInstrumentStruct* pInstrument, int iRoutDirection);
/**
* 根据链接方向，得到连接的前一个仪器
* @return CInstrument* 仪器指针 NLLL：连接的前一个仪器不存在
*/
MatrixServerDll_API m_oInstrumentStruct* GetPreviousInstr(m_oInstrumentStruct* pInstrument);



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
MatrixServerDll_API void UpdateLineChangeTime(m_oLineListStruct* pLineList);

/************************************************************************/
/* 路由结构体                                                           */
/************************************************************************/
// 重置路由信息
MatrixServerDll_API void OnRoutReset(m_oRoutStruct* pRout);
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
MatrixServerDll_API void OnInitRoutList(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar);
// 关闭路由队列结构体
MatrixServerDll_API void OnCloseRoutList(m_oRoutListStruct* pRoutList);
// 释放路由队列结构体
MatrixServerDll_API void OnFreeRoutList(m_oRoutListStruct* pRoutList);
// 得到一个空闲路由
MatrixServerDll_API m_oRoutStruct* GetFreeRout(m_oRoutListStruct* pRoutList);
// 增加一个空闲路由
MatrixServerDll_API void AddFreeRout(m_oRoutStruct* pRout, m_oRoutListStruct* pRoutList);

/************************************************************************/
/* 测线队列                                                             */
/************************************************************************/
// 创建测线队列结构体
MatrixServerDll_API m_oLineListStruct* OnCreateLineList(void);
// 重置测线队列结构体
MatrixServerDll_API void OnResetLineList(m_oLineListStruct* pLineList);
// 初始化测线队列结构体
MatrixServerDll_API void OnInitLineList(m_oLineListStruct* pLineList, m_oConstVarStruct* pConstVar);
// 关闭测线队列结构体
MatrixServerDll_API void OnCloseLineList(m_oLineListStruct* pLineList);
// 释放测线队列结构体
MatrixServerDll_API void OnFreeLineList(m_oLineListStruct* pLineList);
// 得到在线仪器位置
MatrixServerDll_API void QueryInstrumentLocation(char* pChar, int& iPos, m_oLineListStruct* pLineList);
// 更新底层仪器的本地系统时间
MatrixServerDll_API void UpdateLocalSysTime(unsigned int uiSysTime, m_oLineListStruct* pLineList);
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
// 判断索引号是否已加入数据存储缓冲索引表
MatrixServerDll_API BOOL IfIndexExistInADCDataBufMap(unsigned int uiIndex, hash_map<unsigned int, m_oADCDataBufStruct*>* pMap);
// 向数据存储缓冲区索引表中加入缓冲区指针
MatrixServerDll_API void AddToADCDataBufMap(unsigned int uiIndex, m_oADCDataBufStruct* pADCDataBuf,
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap);
// 根据输入索引号，由索引表得到数据存储缓冲区指针
MatrixServerDll_API m_oADCDataBufStruct* GetADCDataBufFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap);
// 从索引表删除索引号指向的数据存储缓冲区指针
MatrixServerDll_API BOOL DeleteADCDataBufFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap);
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
MatrixServerDll_API BOOL IfIndexExistInOptTaskIPMap(unsigned int uiIndex,
	hash_map<unsigned int, m_oOptInstrumentStruct*>* pMap);
// 从施工任务仪器索引表中得到仪器的行号
// MatrixServerDll_API m_oOptInstrumentStruct GetLineNbFromOptTaskSNMap(unsigned int uiIndex,
// 	hash_map<unsigned int, m_oOptInstrumentStruct>* pMap);
// 向施工任务仪器索引表中加入仪器
MatrixServerDll_API void AddToOptTaskIPMap(unsigned int uiIndex, m_oOptInstrumentStruct* pOptInstr,
	hash_map<unsigned int, m_oOptInstrumentStruct*>* pMap);
// 增加一个施工任务
MatrixServerDll_API void AddOptTaskToMap(unsigned int uiIndex, m_oOptTaskStruct* pOptTask, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap);
// 根据输入索引号，由索引表得到施工任务指针
MatrixServerDll_API m_oOptTaskStruct* GetOptTaskFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap);
// 根据输入索引号，由索引表得到施工仪器指针
MatrixServerDll_API m_oOptInstrumentStruct* GetOptTaskFromIPMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptInstrumentStruct*>* pMap);
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
MatrixServerDll_API void InstrumentLocationSort(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout);
/**
* 设置交叉站某个方向的路由
	* @param CInstrument* &pInstrument 仪器指针
	* @param unsigned int uiRoutDirection 路由方向
* @return void
*/
MatrixServerDll_API void SetCrossRout(m_oInstrumentStruct* pInstrument, int iRoutDirection, m_oRoutListStruct* pRoutList);
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
// 回收一个路由
MatrixServerDll_API void FreeRoutFromMap(unsigned int uiRoutIP, m_oRoutListStruct* pRoutList);
// 回收一个仪器
MatrixServerDll_API void FreeInstrumentFromMap(m_oInstrumentStruct* pInstrument, 
	m_oLineListStruct* pLineList, m_oLogOutPutStruct* pLogOutPut);
// 删除该路由方向指定仪器之后的仪器
MatrixServerDll_API void DeleteInstrumentAlongRout(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, 
	m_oLineListStruct* pLineList, m_oLogOutPutStruct* pLogOutPut);
// 在路由方向上删除该仪器之后的全部仪器
MatrixServerDll_API void DeleteAllInstrumentAlongRout(m_oInstrumentStruct* pInstrument, m_oRoutStruct* pRout, 
	m_oLineListStruct* pLineList, m_oLogOutPutStruct* pLogOutPut);
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
MatrixServerDll_API void PrepareTailTimeFrame(m_oRoutStruct* pRout);
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
	unsigned int uiDstIP, unsigned int uiTBHigh);
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
// MatrixServerDll_API void GetTimeDelayTaskAlongRout(m_oRoutStruct* pRout, m_oRoutListStruct* pRoutList);
// 得到时统任务
// MatrixServerDll_API void GetTimeDelayTask(m_oRoutListStruct* pRoutList, m_oLogOutPutStruct* pLogOutPut);
// 生成时统任务队列
// MatrixServerDll_API void GenTimeDelayTaskQueue(m_oRoutListStruct* pRoutList, m_oLogOutPutStruct* pLogOutPut);
// ADC参数设置线程开始工作
MatrixServerDll_API void OnADCSetThreadWork(int iOpt, m_oADCSetThreadStruct* pADCSetThread);
// 清除ADC参数设置任务索引
MatrixServerDll_API void OnClearADCSetMap(m_oLineListStruct* pLineList);
// 将仪器加入ADC参数设置索引表
MatrixServerDll_API void GetADCTaskQueueBySN(bool bADCStartSample, bool bADCStopSample, 
	m_oLineListStruct* pLineList, m_oLogOutPutStruct* pLogOutPut, m_oInstrumentStruct* pInstrument, int iOpt);
// 判断路由方向上是否有采集站
MatrixServerDll_API void GetADCTaskQueueByRout(bool bADCStartSample, bool bADCStopSample, 
	m_oLineListStruct* pLineList, m_oLogOutPutStruct* pLogOutPut, m_oRoutStruct* pRout, int iOpt);
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
// 将ADC数据加入到任务缓冲区
MatrixServerDll_API void AddToADCDataBuf(unsigned int uiIP, unsigned int uiTime, double dPointTime, char* pBuf, 
	unsigned int uiLen, m_oADCDataRecThreadStruct* pADCDataRecThread);
// 采样数据回调函数
// MatrixServerDll_API void GetProSampleDateCallBack(m_oADCDataRecThreadStruct* pADCDataRecThread, 
// 	ProSampleDateCallBack pCallBack);
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
// 初始化SEGD文件标准头
MatrixServerDll_API void OnInitSegdSH(m_oSegdStandardHeaderStruct* pSegdSH);
// 初始化SEGD文件数据道头
MatrixServerDll_API void OnInitSegdTH(m_oSegdTraceHeaderStruct* pSegdTH);
// 保存到Segd文件，成功返回true，失败返回false
MatrixServerDll_API bool SaveSegdFile(m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessGeneralHeaderBlock1BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessGeneralHeaderBlock2BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessGeneralHeaderBlock3BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessScanTypeHeaderBackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessExtendedHeaderBackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessExternalHeaderBackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderBackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock1BackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock2BackNew(FILE* pFile, int iChannel, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock3BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock4BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock5BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock6BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceHeaderExtensionBlock7BackNew(FILE* pFile, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ProcessTraceDataBackNew(FILE* pFile, int iInstrNo, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API int ReadInitToWriteSegdNew_BCD(FILE* pFile, int iCountByte, const char* pParameter);
MatrixServerDll_API int ReadInitToWriteSegdNew_BIN(FILE* pFile, int iCountByte, const char* pParameter);
MatrixServerDll_API void ReadInitToWriteSegdNew_BCD_addLow(FILE* pFile, int iCountByte, const char* pParameter, string strHigh);
MatrixServerDll_API void ReadInitToWriteSegdNew_BCD_addHigh(const char* pParameter, string* pstr);
MatrixServerDll_API void ReadInitToWriteSegdNew_FLOAT(FILE* pFile, int iCountByte, const char* pParameter);
MatrixServerDll_API void ReadInitToWriteSegdNew_DOUBLE(FILE* pFile, int iCountByte, const char* pParameter);
MatrixServerDll_API void ReadInitToWriteSegdNew_ASC(FILE* pFile, int iCountByte, const char* pParameter);
MatrixServerDll_API void ReadArrayToWriteSegdData_FLOAT(FILE* pFile, int iInstrNo, int iDataNo, m_oSegdFileStruct* pSegdFileStruct);
MatrixServerDll_API void ReadParameterToSegd_BIN(FILE* pFile, int iCountByte, int iValue);
MatrixServerDll_API void ReadParameterToSegd_BCD(FILE* pFile, int iCountByte, int iValue);
// 关闭所有的施工文件
// MatrixServerDll_API void CloseAllADCDataSaveInFile(m_oOptTaskArrayStruct* pOptTaskArray);
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
// 产生参与施工仪器索引表
MatrixServerDll_API void GenOptInstrMap(m_oLineListStruct* pLineList, m_oOptTaskArrayStruct* pOptTaskArray);
// 产生一个施工任务
MatrixServerDll_API void GenOptTaskList(unsigned int uiStartOptNo, unsigned int uiOptNoInterval, unsigned int uiOptNum, 
	unsigned int uiTBWindow, unsigned int uiTSample, int iSampleRate, m_oOptTaskArrayStruct* pOptTaskArray, 
	m_oLineListStruct* pLineList, m_oConstVarStruct* pConstVar);
// 释放一个施工任务
// MatrixServerDll_API void FreeOneOptTask(unsigned int uiIndex, m_oOptTaskArrayStruct* pOptTaskArray, m_oADCDataBufArrayStruct* pADCDataBufArray);
// 按SN重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLableBySN(m_oInstrumentStruct* pInstrument, int iOpt);
// 按路由重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLableByRout(m_oRoutStruct* pRout, int iOpt);
// 按照路由地址重置ADC参数设置标志位
MatrixServerDll_API void OnSetADCByLAUXSN(int iLineIndex, int iPointIndex, int iDirection, int iOpt, 
	m_oEnvironmentStruct* pEnv, bool bOnly = true, bool bRout = true);
// 重置ADC参数设置标志位
MatrixServerDll_API void OnResetADCSetLable(m_oRoutListStruct* pRoutList, int iOpt);
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
MatrixServerDll_API void OnInit(m_oEnvironmentStruct* pEnv);
// 关闭
MatrixServerDll_API void OnClose(m_oEnvironmentStruct* pEnv);
// 工作
MatrixServerDll_API unsigned int OnWork(m_oEnvironmentStruct* pEnv);
// 停止
MatrixServerDll_API void OnStop(m_oEnvironmentStruct* pEnv);
// 释放实例资源
MatrixServerDll_API void OnFreeInstance(m_oEnvironmentStruct* pEnv);



#undef MatrixServerDll_API
#ifdef __cplusplus
}
#endif

#endif
