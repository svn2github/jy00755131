#include <map>
#include "Thread.h"
#include "ADCDataFrame.h"
#include "InstrumentList.h"
#include "LogOutPut.h"
#include "ADCDataBufArray.h"
#include "ADCLostFrame.h"
using std::map;

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCDataRecThread_H
#define   _ADCDataRecThread_H

// ADC数据接收线程
typedef struct ADCDataRecThread_Struct
{
	// 线程结构体指针
	m_oThreadStruct* m_pThread;
	// 误码查询帧指针
	m_oADCDataFrameStruct* m_pADCDataFrame;
	// 仪器队列结构体指针
	m_oInstrumentListStruct* m_pInstrumentList;
	// 误码查询日志指针
	m_oLogOutPutStruct* m_pLogOutPutADCFrameTime;
	// 数据存储缓冲区结构体指针
	m_oADCDataBufArrayStruct* m_pADCDataBufArray;
	// 上一帧的本地时间
	unsigned int m_uiADCDataFrameSysTime;
	// 存文件数据帧数计数
	int m_iADCFrameCount;
	// 丢帧索引表
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct> m_oADCLostFrameMap;
}m_oADCDataRecThreadStruct;
#endif