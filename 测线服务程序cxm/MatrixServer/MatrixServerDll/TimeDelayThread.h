#include "Thread.h"
#include "TailTimeFrame.h"
#include "TimeDelayFrame.h"
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

#ifndef   _TimeDelayThread_H
#define   _TimeDelayThread_H
// 时统线程
typedef struct TimeDelayThread_Struct
{
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
#endif