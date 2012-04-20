#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建仪器队列结构体
m_oInstrumentListStruct* OnCreateInstrumentList(void)
{
	m_oInstrumentListStruct* pInstrumentList = NULL;
	pInstrumentList = new m_oInstrumentListStruct;
	InitializeCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	pInstrumentList->m_pArrayInstrument = NULL;
	pInstrumentList->m_bSetByHand = false;
	return pInstrumentList;
}
// 重置仪器队列结构体
void OnResetInstrumentList(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	// 清空SN仪器索引表
	pInstrumentList->m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	pInstrumentList->m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	pInstrumentList->m_olsInstrumentFree.clear();
	// 清空IP地址设置仪器索引表
	pInstrumentList->m_oIPSetMap.clear();
	// 清空未完成ADC参数设置的仪器队列
	pInstrumentList->m_oADCSetInstrumentMap.clear();
	// 清空仪器位置索引表
	pInstrumentList->m_oInstrumentLocationMap.clear();
	// 测网系统发生变化的时间
	pInstrumentList->m_uiLineChangeTime = 0;
	// 测网状态为不稳定
	pInstrumentList->m_bLineStableChange = false;

	// 空闲仪器数量
	pInstrumentList->m_uiCountFree = pInstrumentList->m_uiCountAll;
	for(unsigned int i = 0; i < pInstrumentList->m_uiCountAll; i++)
	{
		// 重置仪器
		OnInstrumentReset(&pInstrumentList->m_pArrayInstrument[i], pInstrumentList->m_bSetByHand);
		// 仪器加在空闲仪器队列尾部
		pInstrumentList->m_olsInstrumentFree.push_back(&pInstrumentList->m_pArrayInstrument[i]);
	}
	LeaveCriticalSection(&pInstrumentList->m_oSecInstrumentList);
}
// ADC参数设置改为手动设置
void OnSetADCSetByHand(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	pInstrumentList->m_bSetByHand = true;
}
// 初始化仪器队列结构体
void OnInitInstrumentList(m_oInstrumentListStruct* pInstrumentList, m_oConstVarStruct* pConstVar)
{
	if (pConstVar == NULL)
	{
		return;
	}
	if (pInstrumentList == NULL)
	{
		AddMsgToLogOutPutList(pConstVar->m_pLogOutPut, "InitInstrumentList", "",
			ErrorType, IDS_ERR_PTRISNULL);
		return;
	}
	EnterCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	// 清空SN仪器索引表
	pInstrumentList->m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	pInstrumentList->m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	pInstrumentList->m_olsInstrumentFree.clear();
	// 清空IP地址设置仪器索引表
	pInstrumentList->m_oIPSetMap.clear();
	// 清空未完成ADC参数设置的仪器索引
	pInstrumentList->m_oADCSetInstrumentMap.clear();
	// 清空仪器位置索引表
	pInstrumentList->m_oInstrumentLocationMap.clear();

	// 测网系统发生变化的时间
	pInstrumentList->m_uiLineChangeTime = 0;
	// 测网状态为不稳定
	pInstrumentList->m_bLineStableChange = false;
	// 仪器队列中仪器个数
	pInstrumentList->m_uiCountAll = pConstVar->m_iInstrumentNum;
	// 生成仪器数组
	if (pInstrumentList->m_pArrayInstrument != NULL)
	{
		delete[] pInstrumentList->m_pArrayInstrument;
	}
	pInstrumentList->m_pArrayInstrument = new m_oInstrumentStruct[pInstrumentList->m_uiCountAll];
	// 空闲仪器数量
	pInstrumentList->m_uiCountFree = pInstrumentList->m_uiCountAll;
	for(unsigned int i = 0; i < pInstrumentList->m_uiCountAll; i++)
	{
		// 仪器在仪器数组中的位置
		pInstrumentList->m_pArrayInstrument[i].m_uiIndex = i;
		pInstrumentList->m_pArrayInstrument[i].m_uiIP = pConstVar->m_iIPSetAddrStart 
			+ i * pConstVar->m_iIPSetAddrInterval;
		// 重置仪器
		OnInstrumentReset(&pInstrumentList->m_pArrayInstrument[i], pInstrumentList->m_bSetByHand);
		// 仪器加在空闲仪器队列尾部
		pInstrumentList->m_olsInstrumentFree.push_back(&pInstrumentList->m_pArrayInstrument[i]);
	}
	LeaveCriticalSection(&pInstrumentList->m_oSecInstrumentList);
}
// 关闭仪器队列结构体
void OnCloseInstrumentList(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	EnterCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	// 清空SN仪器索引表
	pInstrumentList->m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	pInstrumentList->m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	pInstrumentList->m_olsInstrumentFree.clear();
	// 清空IP地址设置仪器索引表
	pInstrumentList->m_oIPSetMap.clear();
	// 清空未完成ADC参数设置的仪器队列
	pInstrumentList->m_oADCSetInstrumentMap.clear();
	// 清空仪器位置索引表
	pInstrumentList->m_oInstrumentLocationMap.clear();
	// 删除仪器数组
	if (pInstrumentList->m_pArrayInstrument != NULL)
	{
		delete[] pInstrumentList->m_pArrayInstrument;
		pInstrumentList->m_pArrayInstrument = NULL;
	}
	LeaveCriticalSection(&pInstrumentList->m_oSecInstrumentList);
}
// 释放仪器队列结构体
void OnFreeInstrumentList(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	DeleteCriticalSection(&pInstrumentList->m_oSecInstrumentList);
	delete pInstrumentList;
}
// 得到一个空闲仪器
m_oInstrumentStruct* GetFreeInstrument(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return NULL;
	}
	m_oInstrumentStruct* pInstrument = NULL;
	list <m_oInstrumentStruct*>::iterator iter;
	if(pInstrumentList->m_uiCountFree > 0)	//有空闲仪器
	{
		// 从空闲队列中得到一个仪器
		iter = pInstrumentList->m_olsInstrumentFree.begin();
		pInstrument = *iter;
		pInstrumentList->m_olsInstrumentFree.pop_front();	
		pInstrument->m_bInUsed = true;	// 设置仪器为使用中
		pInstrumentList->m_uiCountFree--;	// 空闲仪器总数减1
	}
	return pInstrument;
}
// 增加一个空闲仪器
void AddFreeInstrument(m_oInstrumentStruct* pInstrument, m_oInstrumentListStruct* pInstrumentList)
{
	if ((pInstrument == NULL) || (pInstrumentList == NULL))
	{
		return;
	}
	//初始化仪器
	OnInstrumentReset(pInstrument, pInstrumentList->m_bSetByHand);
	//加入空闲队列
	pInstrumentList->m_olsInstrumentFree.push_back(pInstrument);
	pInstrumentList->m_uiCountFree++;	// 空闲仪器总数加1
}
// 更新上次测网系统变化时刻
void UpdateLineChangeTime(m_oInstrumentListStruct* pInstrumentList)
{
	if (pInstrumentList == NULL)
	{
		return;
	}
	// 上次测网系统变化时刻
	pInstrumentList->m_uiLineChangeTime = GetTickCount();
}