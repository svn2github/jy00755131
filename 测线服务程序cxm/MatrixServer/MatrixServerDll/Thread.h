#include "ConstVar.h"
#include "LogOutPut.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _Thread_H
#define   _Thread_H
// 线程结构体
typedef struct Thread_Struct
{
	// 线程句柄
	HANDLE m_hThread;
	// 线程号
	DWORD m_dwThreadID;
	//是否工作状态
	bool m_bWork;
	// 是否关闭线程
	bool m_bClose;
	// 线程结束事件
	HANDLE m_hThreadClose;
	// 常量指针
	m_oConstVarStruct* m_pConstVar;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oThreadStruct;
#endif