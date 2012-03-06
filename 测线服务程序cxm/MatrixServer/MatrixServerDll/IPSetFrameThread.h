#include "Thread.h"
#include "IPSetFrame.h"
#include "InstrumentList.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _IPSetFrameThread_H
#define   _IPSetFrameThread_H

// IP地址设置线程
typedef struct IPSetFrameThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// IP地址设置帧指针
	m_oIPSetFrameStruct* m_pIPSetFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
}m_oIPSetFrameThreadStruct;
#endif