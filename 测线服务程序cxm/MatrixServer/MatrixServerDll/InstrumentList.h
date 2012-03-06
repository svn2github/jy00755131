
#include <list>
#include <hash_map>
#include "Instrument.h"
using std::list;
using stdext::hash_map;

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _InstrumentList_H
#define   _InstrumentList_H
// 仪器队列
typedef struct InstrumentList_Struct
{
	// 仪器队列资源同步对象
	CRITICAL_SECTION m_oSecInstrumentList;
	/** 仪器数组指针*/
	m_oInstrumentStruct* m_pArrayInstrument;
	/** 空闲仪器队列*/
	list<m_oInstrumentStruct*> m_olsInstrumentFree;
	/** 设置IP地址的仪器队列*/
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPSetMap;
	// 仪器SN索引表
	hash_map<unsigned int, m_oInstrumentStruct*> m_oSNInstrumentMap;
	// 仪器IP地址索引表
	hash_map<unsigned int, m_oInstrumentStruct*> m_oIPInstrumentMap;
	// 未完成ADC参数设置的仪器队列
	hash_map<unsigned int, m_oInstrumentStruct*> m_oADCSetInstrumentMap;
	// 测网系统发生变化的时间
	unsigned int m_uiLineChangeTime;
	// 测网状态由不稳定变为稳定
	bool m_bLineStableChange;
	/** 仪器总数*/
	unsigned int m_uiCountAll;
	/** 空闲仪器数量*/
	unsigned int m_uiCountFree;
}m_oInstrumentListStruct;
#endif