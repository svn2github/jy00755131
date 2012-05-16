#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建测线队列结构体
m_oLineListStruct* OnCreateLineList(void)
{
	m_oLineListStruct* pLineList = NULL;
	pLineList = new m_oLineListStruct;
	pLineList->m_pInstrumentList = OnCreateInstrumentList();
	pLineList->m_pRoutList = OnCreateRoutList();
	InitializeCriticalSection(&pLineList->m_oSecLineList);
	return pLineList;
}
// 重置测线队列结构体
void OnResetLineList(m_oLineListStruct* pLineList)
{
	if (pLineList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLineList->m_oSecLineList);
	// 重置仪器队列
	OnResetInstrumentList(pLineList->m_pInstrumentList);
	// 重置路由队列
	OnResetRoutList(pLineList->m_pRoutList);
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 初始化测线队列结构体
void OnInitLineList(m_oLineListStruct* pLineList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pLineList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "OnInitLineList", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pLineList->m_oSecLineList);
	// 初始化仪器队列结构体
	OnInitInstrumentList(pLineList->m_pInstrumentList, pConstVar);
	// 初始化路由队列结构体
	OnInitRoutList(pLineList->m_pRoutList, pConstVar);
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 关闭测线队列结构体
void OnCloseLineList(m_oLineListStruct* pLineList)
{
	if (pLineList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLineList->m_oSecLineList);
	// 释放仪器队列资源
	OnCloseInstrumentList(pLineList->m_pInstrumentList);
	// 释放路由队列资源
	OnCloseRoutList(pLineList->m_pRoutList);
	LeaveCriticalSection(&pLineList->m_oSecLineList);
}
// 释放测线队列结构体
void OnFreeLineList(m_oLineListStruct* pLineList)
{
	if (pLineList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pLineList->m_oSecLineList);
	// 释放仪器队列结构体资源
	OnFreeInstrumentList(pLineList->m_pInstrumentList);
	// 释放路由队列结构体资源
	OnFreeRoutList(pLineList->m_pRoutList);
	LeaveCriticalSection(&pLineList->m_oSecLineList);
	DeleteCriticalSection(&pLineList->m_oSecLineList);
	delete pLineList;
}