#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建施工任务数组结构体
m_oOptTaskArrayStruct* OnCreateOptTaskArray(void)
{
	m_oOptTaskArrayStruct* pOptTaskArray = NULL;
	pOptTaskArray = new m_oOptTaskArrayStruct;
	pOptTaskArray->m_pArrayOptTask = NULL;
	InitializeCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	return pOptTaskArray;
}
// 重置施工任务数组结构体
void OnResetOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray)
{
	ASSERT(pOptTaskArray != NULL);
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	// 清空施工任务索引
	pOptTaskArray->m_oOptTaskWorkMap.clear();
	// 清空空闲施工任务队列
	pOptTaskArray->m_olsOptTaskFree.clear();
	// 空闲施工任务总数
	pOptTaskArray->m_uiCountFree = pOptTaskArray->m_uiCountAll;
	// 加入空闲施工任务队列
	for(unsigned int i = 0; i < pOptTaskArray->m_uiCountAll; i++)
	{
		// 重置施工任务
		OnOptTaskReset(&pOptTaskArray->m_pArrayOptTask[i]);
		// 施工任务加在空闲施工任务队列尾部
		pOptTaskArray->m_olsOptTaskFree.push_back(&pOptTaskArray->m_pArrayOptTask[i]);
	}
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
}
// 初始化施工任务数组结构体
void OnInitOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray, m_oConstVarStruct* pConstVar)
{
	ASSERT(pOptTaskArray != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	// 清空施工任务索引
	pOptTaskArray->m_oOptTaskWorkMap.clear();
	// 清空空闲施工任务队列
	pOptTaskArray->m_olsOptTaskFree.clear();

	// 施工任务队列中路由个数
	pOptTaskArray->m_uiCountAll = pConstVar->m_iOptTaskCountAll;
	// 空闲施工任务总数
	pOptTaskArray->m_uiCountFree = pOptTaskArray->m_uiCountAll;
	// 施工任务数据存储文件夹计数
	pOptTaskArray->m_uiOptTaskNb = 0;
	// 生成施工任务数组
	if (pOptTaskArray->m_pArrayOptTask != NULL)
	{
		delete[] pOptTaskArray->m_pArrayOptTask;
		pOptTaskArray->m_pArrayOptTask = NULL;
	}
	pOptTaskArray->m_pArrayOptTask = new m_oOptTaskStruct[pOptTaskArray->m_uiCountAll];

	// 加入空闲施工任务队列
	for(unsigned int i = 0; i < pOptTaskArray->m_uiCountAll; i++)
	{
// 		// 施工任务在施工任务数组中的位置
// 		pOptTaskArray->m_pArrayOptTask[i].m_uiIndex = i;
		// 重置施工任务
		OnOptTaskReset(&pOptTaskArray->m_pArrayOptTask[i]);
		// 施工任务加在空闲施工任务队列尾部
		pOptTaskArray->m_olsOptTaskFree.push_back(&pOptTaskArray->m_pArrayOptTask[i]);
	}
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
}
// 关闭施工任务数组结构体
void OnCloseOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray)
{
	ASSERT(pOptTaskArray != NULL);
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	// 清空施工任务索引
	pOptTaskArray->m_oOptTaskWorkMap.clear();
	// 清空空闲施工任务队列
	pOptTaskArray->m_olsOptTaskFree.clear();
	// 删除施工任务数组
	if (pOptTaskArray->m_pArrayOptTask != NULL)
	{
		delete[] pOptTaskArray->m_pArrayOptTask;
		pOptTaskArray->m_pArrayOptTask = NULL;
	}
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
}
// 释放施工任务数组结构体
void OnFreeOptTaskArray(m_oOptTaskArrayStruct* pOptTaskArray)
{
	ASSERT(pOptTaskArray != NULL);
	DeleteCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	delete pOptTaskArray;
	pOptTaskArray = NULL;
}
// 得到一个空闲施工任务
m_oOptTaskStruct* GetFreeOptTask(m_oOptTaskArrayStruct* pOptTaskArray)
{
	ASSERT(pOptTaskArray != NULL);
	m_oOptTaskStruct* pOptTask = NULL;
	list <m_oOptTaskStruct*>::iterator iter;
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	if(pOptTaskArray->m_uiCountFree > 0)	//有空闲施工任务
	{
		// 从空闲施工任务队列头部得到一个空闲施工任务
		iter = pOptTaskArray->m_olsOptTaskFree.begin();
		pOptTask = *iter;
		pOptTaskArray->m_olsOptTaskFree.pop_front();
		// 施工任务是否使用中
		pOptTask->m_bInUsed = true;
		// 空闲施工任务计数减1
		pOptTaskArray->m_uiCountFree--;
	}
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	return pOptTask;
}
// 增加一个空闲施工任务
void AddFreeOptTask(m_oOptTaskStruct* pOptTask, m_oOptTaskArrayStruct* pOptTaskArray)
{
	ASSERT(pOptTask != NULL);
	ASSERT(pOptTaskArray != NULL);
	EnterCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
	//初始化施工任务
	OnOptTaskReset(pOptTask);
	//加入空闲队列
	pOptTaskArray->m_olsOptTaskFree.push_back(pOptTask);
	// 空闲施工任务计数加1
	pOptTaskArray->m_uiCountFree++;
	LeaveCriticalSection(&pOptTaskArray->m_oSecOptTaskArray);
}
