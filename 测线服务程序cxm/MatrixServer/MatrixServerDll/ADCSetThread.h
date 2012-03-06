#include "Thread.h"
#include "ADCSetFrame.h"
#include "InstrumentList.h"
#include "RoutList.h"
#include "LogOutPut.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCSetThread_H
#define   _ADCSetThread_H

// ADC参数设置线程
typedef struct ADCSetThread_Struct
{
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
	// ADC参数设置仪器个数
	unsigned int m_uiADCSetNum;
	// ADC参数设置应答仪器个数
	unsigned int m_uiADCSetReturnNum;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
}m_oADCSetThreadStruct;
#endif