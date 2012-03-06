#include "Thread.h"
#include "TailFrame.h"
#include "InstrumentList.h"
#include "RoutList.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _TailFrameThread_H
#define   _TailFrameThread_H

// 尾包线程
typedef struct TailFrameThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 尾包帧指针
	m_oTailFrameStruct* m_pTailFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 路由队列结构体指针
	m_oRoutListStruct* m_pRoutList;
}m_oTailFrameThreadStruct;

#endif
