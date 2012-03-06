#include <list>
#include <hash_map>
#include "OptTask.h"
using std::list;
using stdext::hash_map;

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _OptTaskArray_H
#define   _OptTaskArray_H

// 施工任务结构体数组
typedef struct OptTaskArray_Struct
{
	// 资源同步对象
	CRITICAL_SECTION m_oSecOptTaskArray;
	/** 施工任务数组指针*/
	m_oOptTaskStruct* m_pArrayOptTask;
	// 空闲的施工任务队列
	list<m_oOptTaskStruct*> m_olsOptTaskFree;
	// 正在进行的施工任务索引
	hash_map<unsigned int, m_oOptTaskStruct*> m_oOptTaskWorkMap;
	/** 施工任务总数*/
	unsigned int m_uiCountAll;
	/** 空闲任务数量*/
	unsigned int m_uiCountFree;
	// 施工任务数据存储文件夹计数
	unsigned int m_uiADCDataFolderNb;
	// 施工数据存储文件夹路径
	string m_SaveLogFolderPath;
}m_oOptTaskArrayStruct;
#endif