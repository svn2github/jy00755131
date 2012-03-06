
#include "LogOutPut.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef    _ConstVar_H
#define    _ConstVar_H

// ini参数文件相对路径
#define INIFilePath					_T("..\\parameter\\MatrixServerDLL.ini")
// INI文件读取关键字缓冲区大小
#define INIFileStrBufSize			256
// 从INI文件中解析得到的常量
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
	int m_iIPBroadcastAddr;
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
	char* m_pFrameHeadCheck;
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
	char m_bySndFrameBufInit;
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
	char m_byCmdSn;
	// 首包时间
	char m_byCmdHeadFrameTime;
	// 本地IP地址
	char m_byCmdLocalIPAddr;
	// 本地系统时间
	char m_byCmdLocalSysTime;
	// 本地时间修正高位
	char m_byCmdLocalTimeFixedHigh;
	// 本地时间修正低位
	char m_byCmdLocalTimeFixedLow;
	// 自动数据返回地址
	char m_byCmdADCDataReturnAddr;
	// 自动数据返回端口和命令
	char m_byCmdADCDataReturnPort;
	// 端口递增下限和上限
	char m_byCmdADCDataReturnPortLimit;
	// 设置网络等待端口和命令
	char m_byCmdBroadCastPortSet;
	// 系统硬件状态拷贝
	char m_byCmdFDUErrorCode;
	// TB时刻高位
	char m_byCmdTBHigh;
	// TB时刻低位
	char m_byCmdTbLow;
	// work_ctrl 交叉站方向
	char m_byCmdLAUXRoutOpenQuery;
	// 路由开关
	char m_byCmdLAUXRoutOpenSet;
	// 尾包接收\发送时刻低位
	char m_byCmdTailRecSndTime;
	// 广播命令等待端口匹配
	char m_byCmdBroadCastPortSeted;
	// 设置ADC控制命令命令字
	char m_byCmdADCSet;
	// 网络时刻
	char m_byCmdNetTime;
	// 交叉站大线尾包接收时刻
	char m_byCmdLineTailRecTimeLAUX;
	// 交叉站交叉线尾包接收时刻
	char m_byCmdLAUTailRecTimeLAUX;
	// 交叉站故障1
	char m_byCmdLAUXErrorCode1;
	// 交叉站故障2
	char m_byCmdLAUXErrorCode2;
	// 交叉站路由分配
	char m_byCmdLAUXSetRout;
	// 返回路由
	char m_byCmdReturnRout;
	// 命令解析结束命令
	char m_byCmdEnd;

	//____ADC参数设置____
	// ADC设置正弦波命令大小
	int m_iSetADCSetSineSize;
	// ADC设置正弦波命令
	char* m_pSetADCSetSine;
	// ADC设置停止采样命令大小
	int m_iSetADCStopSampleSize;
	// ADC设置停止采样命令
	char* m_pSetADCStopSample;
	// ADC设置打开TB电源低位大小
	int m_iSetADCOpenTBPowerLowSize;
	// ADC设置打开TB电源低位
	char* m_pSetADCOpenTBPowerLow;
	// ADC设置打开TB电源高位大小
	int m_iSetADCOpenTBPowerHighSize;
	// ADC设置打开TB电源高位
	char* m_pSetADCOpenTBPowerHigh;
	// ADC设置打开TB开关低位大小
	int m_iSetADCOpenSwitchTBLowSize;
	// ADC设置打开TB开关低位
	char* m_pSetADCOpenSwitchTBLow;
	// ADC设置打开TB开关高位大小
	int m_iSetADCOpenSwitchTBHighSize;
	// ADC设置打开TB开关高位
	char* m_pSetADCOpenSwitchTBHigh;
	// ADC设置读寄存器大小
	int m_iSetADCRegisterReadSize;
	// ADC设置读寄存器
	char* m_pSetADCRegisterRead;
	// ADC设置写寄存器大小
	int m_iSetADCRegisterWriteSize;
	// ADC设置写寄存器
	char* m_pSetADCRegisterWrite;
	// ADC设置打开TB开关大小
	int m_iSetADCTBSwitchOpenSize;
	// ADC设置打开TB开关
	char* m_pSetADCTBSwitchOpen;
	// ADC采样设置大小
	int m_iSetADCSampleSize;
	// ADC采样设置
	char* m_pSetADCSample;
	// ADC设置连续采样大小
	int m_iSetADCReadContinuousSize;
	// ADC设置连续采样
	char* m_pSetADCReadContinuous;
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

#endif