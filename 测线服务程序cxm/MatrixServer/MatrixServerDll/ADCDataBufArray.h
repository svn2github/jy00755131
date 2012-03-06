#include <list>
#include "ADCDataBuf.h"
using std::list;
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCDataBufArray_H
#define   _ADCDataBufArray_H

// 数据存储缓冲区结构体
typedef struct ADCDataBufArray_Struct
{
	// 缓冲区资源同步对象
	CRITICAL_SECTION m_oSecADCDataBufArray;
	/** 缓冲区数组指针*/
	m_oADCDataBufStruct* m_pArrayADCDataBuf;
	/** 缓冲区队列*/
	list<m_oADCDataBufStruct*> m_olsADCDataBufFree;
	// 写入文件的数据缓冲区队列
	list<m_oADCDataBufStruct*> m_olsADCDataToWrite;
	/** 缓冲区总数*/
	unsigned int m_uiCountAll;
	/** 空闲缓冲区数量*/
	unsigned int m_uiCountFree;
}m_oADCDataBufArrayStruct;
#endif