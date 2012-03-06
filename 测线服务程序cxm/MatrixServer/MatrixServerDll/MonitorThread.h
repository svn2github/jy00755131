#include "Thread.h"
#include "InstrumentList.h"
#include "RoutList.h"
#include "ADCSetThread.h"
#include "TimeDelayThread.h"
#include "ErrorCodeThread.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _MonitorThread_H
#define   _MonitorThread_H
// 路由监视线程
typedef struct MonitorThread_Struct
{
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
#endif