#include "Thread.h"
#include "ADCDataBufArray.h"
#include "OptTaskArray.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCDataSaveThread_H
#define   _ADCDataSaveThread_H

// 施工放炮数据存储线程
typedef struct ADCDataSaveThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 数据存储缓冲区结构体指针
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	// 施工任务数组结构体指针
	m_oOptTaskArrayStruct* m_pOptTaskArray;
}m_oADCDataSaveThreadStruct;
#endif