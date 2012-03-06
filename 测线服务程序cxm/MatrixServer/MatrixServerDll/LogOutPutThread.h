#include "Thread.h"
#include "LogOutPut.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _LogOutPutThread_H
#define   _LogOutPutThread_H
// 日志输出线程
typedef struct LogOutPutThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutTimeDelay;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutErrorCode;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
}m_oLogOutPutThreadStruct;
#endif