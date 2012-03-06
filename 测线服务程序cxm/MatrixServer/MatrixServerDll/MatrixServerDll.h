// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MATRIXSERVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MATRIXSERVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#include <list>
#include <hash_map>
#include <map>
#include "Resource.h"

#include "LogOutPut.h"
#include "ConstVar.h"
#include "xml.h"
#include "InstrumentList.h"
#include "RoutList.h"
#include "HeartBeatFrame.h"
#include "HeadFrame.h"
#include "IPSetFrame.h"
#include "TailFrame.h"
#include "TailTimeFrame.h"
#include "TimeDelayFrame.h"
#include "ADCSetFrame.h"
#include "ErrorCodeFrame.h"
#include "ADCDataFrame.h"
#include "LogOutPutThread.h"
#include "HeartBeatThread.h"
#include "HeadFrameThread.h"
#include "IPSetFrameThread.h"
#include "TailFrameThread.h"
#include "TimeDelayThread.h"
#include "ADCSetThread.h"
#include "ErrorCodeThread.h"
#include "ADCDataRecThread.h"
#include "MonitorThread.h"
#include "ADCDataSaveThread.h"
using std::list;
using stdext::hash_map;
using std::map;
using std::string;
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

// 此类是从 MatrixServerDll.dll 导出的
class MATRIXSERVERDLL_API CMatrixServerDll {
public:
	CMatrixServerDll(void);
	// TODO: 在此添加您的方法。
};

extern MATRIXSERVERDLL_API int nMatrixServerDll;

MATRIXSERVERDLL_API int fnMatrixServerDll(void);

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

// 创建实例，并返回实例指针
typedef m_oEnvironmentStruct* (*Create_Instance)(void);
// 释放实例资源
typedef void (*Free_Instance)(m_oEnvironmentStruct* pEnv);
// 校验与设备通讯帧的同步码
typedef bool (*Instrument_CheckFrameHead)(char* pFrameData);
// 生成帧的同步码
typedef void (*Instrument_MakeFrameHead)(char* pFrameData);
// 重置帧内容解析变量
typedef void (*Instrument_ResetFramePacket)(m_oInstrumentCommandStruct* pCommand);
// 解析与设备通讯接收帧内容
typedef bool (*Instrument_ParseFrame)(m_oInstrumentCommandStruct* pCommand, char* pFrameData);
// 生成与设备通讯帧
typedef void (*Instrument_MakeFrame)(m_oInstrumentCommandStruct* pCommand, char* pFrameData);
// 创建CSocket接收端口并绑定端口和IP地址,必须先执行初始化该模块后才能创建该模块端口
typedef SOCKET (*Instrument_CreateSocket)(unsigned short usPort, unsigned int uiIP);
// 设置广播模式
typedef void (*Instrument_SetSocketBroadCast)(SOCKET oSocket);
// 设置端口接收缓冲区接收帧数量
typedef void (*Set_RcvBufferSize)(SOCKET oSocket, int iFrameCount);
// 设置端口发送缓冲区发送帧数量
typedef void (*Set_SndBufferSize)(SOCKET oSocket, int iFrameCount);
// 得到网络接收缓冲区收到的帧数量
typedef DWORD (*Get_FrameCount)(SOCKET oSocket);
// 得到帧数据
typedef bool (*Get_FrameData)(SOCKET oSocket, char* pFrameData);
// 发送帧
typedef void (*Send_Frame)(SOCKET oSocket, char* pFrameData, unsigned short usPort, unsigned int uiIP);

// 创建常量信息结构体
typedef void (*Create_ConstVar)(m_oEnvironmentStruct* pEnv);
// 初始化常量信息结构体
typedef void (*Init_ConstVar)(m_oEnvironmentStruct* pEnv);
// 释放常量信息结构体
typedef void (*Free_ConstVar)(m_oEnvironmentStruct* pEnv);
// 创建仪器通讯信息结构体
typedef void (*Instrument_CreateCommInfo)(m_oEnvironmentStruct* pEnv);
// 初始化仪器通讯信息结构体
typedef void (*Instrument_InitCommInfo)(m_oEnvironmentStruct* pEnv);
// 释放仪器通讯信息结构体
typedef void (*Instrument_FreeCommInfo)(m_oEnvironmentStruct* pEnv);
// 创建心跳帧信息结构体
typedef void (*Instrument_CreateHeartBeat)(m_oEnvironmentStruct* pEnv);
// 初始化心跳
typedef void (*Instrument_InitHeartBeat)(m_oEnvironmentStruct* pEnv);
// 释放心跳帧信息结构体
typedef void (*Instrument_FreeHeartBeat)(m_oEnvironmentStruct* pEnv);
// 创建首包帧信息结构体
typedef void (*Instrument_CreateHeadFrame)(m_oEnvironmentStruct* pEnv);
// 初始化首包
typedef void (*Instrument_InitHeadFrame)(m_oEnvironmentStruct* pEnv);
// 释放首包帧信息结构体
typedef void (*Instrument_FreeHeadFrame)(m_oEnvironmentStruct* pEnv);
// 创建IP地址设置帧信息结构体
typedef void (*Instrument_CreateIPSetFrame)(m_oEnvironmentStruct* pEnv);
// 初始化IP地址设置
typedef void (*Instrument_InitIPSetFrame)(m_oEnvironmentStruct* pEnv);
// 释放IP地址设置帧信息结构体
typedef void (*Instrument_FreeIPSetFrame)(m_oEnvironmentStruct* pEnv);
// 创建线程处理标志位信息结构体
typedef void (*Create_ThreadProcFlag)(m_oEnvironmentStruct* pEnv);
// 初始化线程处理标志位
typedef void (*Init_ThreadProcFlag)(m_oEnvironmentStruct* pEnv);
// 释放线程处理标志位信息结构体
typedef void (*Free_ThreadProcFlag)(m_oEnvironmentStruct* pEnv);
// 创建并设置心跳端口
typedef void (*CreateAndSet_HeartBeatSocket)(m_oEnvironmentStruct* pEnv);
// 创建并设置首包端口
typedef void (*CreateAndSet_HeadFrameSocket)(m_oEnvironmentStruct* pEnv);
// 创建并设置IP地址设置端口
typedef void (*CreateAndSet_IPSetFrameSocket)(m_oEnvironmentStruct* pEnv);

// 初始化实例
// 包括：
// 创建并初始化仪器通讯信息结构体
// 创建并初始化心跳帧信息结构体
// 创建并初始化首包帧信息结构体
// 创建并初始化IP地址设置帧信息结构体
// 创建并初始化线程处理标志位信息结构体
// 创建并设置心跳端口
// 创建并设置首包端口
// 创建并设置IP地址设置端口
typedef void (*Init_Instance)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Init)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Work)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Stop)(m_oEnvironmentStruct* pEnv);
typedef void (*On_Close)(m_oEnvironmentStruct* pEnv);
typedef void (*On_StartSample)(m_oEnvironmentStruct* pEnv);
typedef void (*On_StopSample)(m_oEnvironmentStruct* pEnv);
// 解析首包帧
typedef bool (*Instrument_ParseHeadFrame)(m_oEnvironmentStruct* pEnv);
// 解析IP地址设置应答帧
typedef bool (*Instrument_ParseIPSetReturnFrame)(m_oEnvironmentStruct* pEnv);
// 生成心跳帧
typedef void (*Instrument_MakeHeartBeatFrame)(m_oEnvironmentStruct* pEnv);
// 生成IP地址设置帧
typedef void (*Instrument_MakeIPSetFrame)(m_oEnvironmentStruct* pEnv);

// 将日志信息加入到日志文件输出队列
typedef void (*Add_LogFileOutPutList)(int iType, unsigned int uiErrorCodeNb, string strFuncName, string strLogInfo);