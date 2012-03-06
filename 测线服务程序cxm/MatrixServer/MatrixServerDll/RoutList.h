#include <list>
#include <hash_map>
#include "Rout.h"
using std::list;
using stdext::hash_map;

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _RoutList_H
#define   _RoutList_H

// 路由队列结构体
typedef struct RoutList_Struct
{
	// 路由队列资源同步对象
	CRITICAL_SECTION m_oSecRoutList;
	/** 路由数组指针*/
	m_oRoutStruct* m_pArrayRout;
	/** 空闲路由队列*/
	list<m_oRoutStruct*> m_olsRoutFree;
	// 仪器路由地址索引表
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutMap;
	// 需要删除路由仪器的路由索引表
	hash_map<unsigned int, m_oRoutStruct*> m_oRoutDeleteMap;
	// 仪器时统的任务队列，成员为路由IP
	list<unsigned int> m_olsTimeDelayTaskQueue;
	// 仪器时统的任务中转队列
	list<unsigned int> m_olsTimeDelayTemp;
	// ADC参数设置任务索引
	hash_map<unsigned int, m_oRoutStruct*> m_oADCSetRoutMap;
	/** 路由总数*/
	unsigned int m_uiCountAll;
	/** 空闲路由数量*/
	unsigned int m_uiCountFree;
}m_oRoutListStruct;

#endif