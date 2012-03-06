#include "Thread.h"
#include "HeadFrame.h"
#include "InstrumentList.h"
#include "RoutList.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _HeadFrameThread_H
#define   _HeadFrameThread_H
// 首包线程
typedef struct HeadFrameThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 首包帧指针
	m_oHeadFrameStruct* m_pHeadFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oHeadFrameThreadStruct;
#endif