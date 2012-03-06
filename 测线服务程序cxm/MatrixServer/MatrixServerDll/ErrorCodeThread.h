#include "Thread.h"
#include "ErrorCodeFrame.h"
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

#ifndef   _ErrorCodeThread_H
#define   _ErrorCodeThread_H

// 误码查询线程
typedef struct ErrorCodeThread_Struct
{
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
#endif