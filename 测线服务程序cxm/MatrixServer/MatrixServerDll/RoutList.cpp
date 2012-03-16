#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建路由队列结构体
m_oRoutListStruct* OnCreateRoutList(void)
{
	m_oRoutListStruct* pRoutList = NULL;
	pRoutList = new m_oRoutListStruct;
	InitializeCriticalSection(&pRoutList->m_oSecRoutList);
	pRoutList->m_pArrayRout = NULL;
	return pRoutList;
}
// 重置路由队列结构体
void OnResetRoutList(m_oRoutListStruct* pRoutList)
{
	if (pRoutList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 清空路由地址索引
	pRoutList->m_oRoutMap.clear();
	// 清空需要删除的路由地址索引
	pRoutList->m_oRoutDeleteMap.clear();
	// 清空ADC参数设置任务索引
	pRoutList->m_oADCSetRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 清空仪器时统的任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
	// 空闲路由总数
	pRoutList->m_uiCountFree = pRoutList->m_uiCountAll;
	// 加入空闲路由队列
	for(unsigned int i = 0; i < pRoutList->m_uiCountAll; i++)
	{
		// 重置路由
		OnRoutReset(&pRoutList->m_pArrayRout[i]);
		// 路由加在空闲路由队列尾部
		pRoutList->m_olsRoutFree.push_back(&pRoutList->m_pArrayRout[i]);
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
}
// 初始化路由队列结构体
void OnInitRoutList(m_oRoutListStruct* pRoutList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pRoutList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitRoutList", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 清空路由地址索引
	pRoutList->m_oRoutMap.clear();
	// 清空需要删除的路由地址索引
	pRoutList->m_oRoutDeleteMap.clear();
	// 清空ADC参数设置任务索引
	pRoutList->m_oADCSetRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 清空仪器时统的任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
	// 路由队列中路由个数
	pRoutList->m_uiCountAll = pConstVar->m_iInstrumentNum;
	// 空闲路由总数
	pRoutList->m_uiCountFree = pRoutList->m_uiCountAll;
	// 生成路由数组
	if (pRoutList->m_pArrayRout != NULL)
	{
		delete[] pRoutList->m_pArrayRout;
	}
	pRoutList->m_pArrayRout = new m_oRoutStruct[pRoutList->m_uiCountAll];

	// 加入空闲路由队列
	for(unsigned int i = 0; i < pRoutList->m_uiCountAll; i++)
	{
		// 设置路由IP地址
		pRoutList->m_pArrayRout[i].m_uiRoutIP = pConstVar->m_iRoutSetAddrStart 
			+ i * pConstVar->m_iRoutSetAddrInterval;
		// 路由在路由数组中的位置
		pRoutList->m_pArrayRout[i].m_uiIndex = i;
		// 重置路由
		OnRoutReset(&pRoutList->m_pArrayRout[i]);
		// 路由加在空闲路由队列尾部
		pRoutList->m_olsRoutFree.push_back(&pRoutList->m_pArrayRout[i]);
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
}
// 关闭路由队列结构体
void OnCloseRoutList(m_oRoutListStruct* pRoutList)
{
	if (pRoutList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 清空路由地址索引
	pRoutList->m_oRoutMap.clear();
	// 清空需要删除的路由地址索引
	pRoutList->m_oRoutDeleteMap.clear();
	// 清空ADC参数设置任务索引
	pRoutList->m_oADCSetRoutMap.clear();
	// 清空空闲路由队列
	pRoutList->m_olsRoutFree.clear();
	// 清空仪器时统的任务队列
	pRoutList->m_olsTimeDelayTaskQueue.clear();
	pRoutList->m_olsTimeDelayTemp.clear();
	// 删除路由数组
	if (pRoutList->m_pArrayRout != NULL)
	{
		delete[] pRoutList->m_pArrayRout;
		pRoutList->m_pArrayRout = NULL;
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
}
// 释放路由队列结构体
void OnFreeRoutList(m_oRoutListStruct* pRoutList)
{
	if (pRoutList == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pRoutList->m_oSecRoutList);
	delete pRoutList;
}

// 得到一个空闲路由
m_oRoutStruct* GetFreeRout(m_oRoutListStruct* pRoutList)
{
	if (pRoutList == NULL)
	{
		return NULL;
	}
	m_oRoutStruct* pRout = NULL;
	list <m_oRoutStruct*>::iterator iter;
	if(pRoutList->m_uiCountFree > 0)	//有空闲路由
	{
		// 从空闲路由队列头部得到一个空闲路由
		iter = pRoutList->m_olsRoutFree.begin();
		pRout = *iter;
		pRoutList->m_olsRoutFree.pop_front();	
		// 路由是否使用中
		pRout->m_bInUsed = true;	
		// 空闲路由计数减1
		pRoutList->m_uiCountFree--;
	}
	return pRout;
}
// 增加一个空闲路由
void AddFreeRout(m_oRoutStruct* pRout, m_oRoutListStruct* pRoutList)
{
	if ((pRout == NULL) || (pRoutList == NULL))
	{
		return;
	}
	//初始化路由
	OnRoutReset(pRout);
	//加入空闲队列
	pRoutList->m_olsRoutFree.push_back(pRout);
	// 空闲路由计数加1
	pRoutList->m_uiCountFree++;
}