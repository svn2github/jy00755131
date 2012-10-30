#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建数据存储缓冲区结构体
m_oADCDataBufArrayStruct* OnCreateADCDataBufArray(void)
{
	m_oADCDataBufArrayStruct* pADCDataBufArray = NULL;
	pADCDataBufArray = new m_oADCDataBufArrayStruct;
	InitializeCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	pADCDataBufArray->m_pArrayADCDataBuf = NULL;
	return pADCDataBufArray;
}
// 重置数据存储缓冲区结构体
void OnResetADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray)
{
	ASSERT(pADCDataBufArray != NULL);
	EnterCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	// 清空数据缓冲区索引
	pADCDataBufArray->m_oADCDataBufWorkMap.clear();
	// 清空空闲数据缓冲区队列
	pADCDataBufArray->m_olsADCDataBufFree.clear();
	// 空闲数据缓冲区总数
	pADCDataBufArray->m_uiCountFree = pADCDataBufArray->m_uiCountAll;
	// 加入空闲数据缓冲区队列
	for(unsigned int i = 0; i < pADCDataBufArray->m_uiCountAll; i++)
	{
		// 重置数据缓冲区
		OnADCDataBufReset(&pADCDataBufArray->m_pArrayADCDataBuf[i]);
		// 数据缓冲区加在空闲数据缓冲区队列尾部
		pADCDataBufArray->m_olsADCDataBufFree.push_back(&pADCDataBufArray->m_pArrayADCDataBuf[i]);
	}
	LeaveCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
}
// 初始化数据存储缓冲区结构体
void OnInitADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray, m_oConstVarStruct* pConstVar)
{
	ASSERT(pADCDataBufArray != NULL);
	ASSERT(pConstVar != NULL);
	EnterCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	// 清空数据缓冲区队列
	pADCDataBufArray->m_oADCDataBufWorkMap.clear();
	// 清空空闲数据缓冲区队列
	pADCDataBufArray->m_olsADCDataBufFree.clear();

	// 数据缓冲区队列中路由个数
	pADCDataBufArray->m_uiCountAll = pConstVar->m_iSEGDDataBufCountAll;
	// 空闲数据缓冲区总数
	pADCDataBufArray->m_uiCountFree = pADCDataBufArray->m_uiCountAll;
	// 生成数据缓冲区数组
	if (pADCDataBufArray->m_pArrayADCDataBuf != NULL)
	{
		for(unsigned int i = 0; i < pADCDataBufArray->m_uiCountAll; i++)
		{
			delete[] pADCDataBufArray->m_pArrayADCDataBuf[i].m_pADCDataBuf;
			pADCDataBufArray->m_pArrayADCDataBuf[i].m_pADCDataBuf = NULL;
		}
		delete[] pADCDataBufArray->m_pArrayADCDataBuf;
		pADCDataBufArray->m_pArrayADCDataBuf = NULL;
	}
	pADCDataBufArray->m_pArrayADCDataBuf = new m_oADCDataBufStruct[pADCDataBufArray->m_uiCountAll];

	// 加入空闲数据缓冲区队列
	for(unsigned int i = 0; i < pADCDataBufArray->m_uiCountAll; i++)
	{
		// 缓冲区在缓冲区数组中的位置
		pADCDataBufArray->m_pArrayADCDataBuf[i].m_uiIndex = i;
		pADCDataBufArray->m_pArrayADCDataBuf[i].m_pADCDataBuf = new char[pConstVar->m_iSEGDDataBufSize];
		pADCDataBufArray->m_pArrayADCDataBuf[i].m_uiBufLength = pConstVar->m_iSEGDDataBufSize;
		// 重置数据缓冲区
		OnADCDataBufReset(&pADCDataBufArray->m_pArrayADCDataBuf[i]);
		// 数据缓冲区加在空闲数据缓冲区队列尾部
		pADCDataBufArray->m_olsADCDataBufFree.push_back(&pADCDataBufArray->m_pArrayADCDataBuf[i]);
	}
	LeaveCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
}
// 关闭数据存储缓冲区结构体
void OnCloseADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray)
{
	ASSERT(pADCDataBufArray != NULL);
	EnterCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	// 清空数据缓冲区队列
	pADCDataBufArray->m_oADCDataBufWorkMap.clear();
	// 清空空闲数据缓冲区队列
	pADCDataBufArray->m_olsADCDataBufFree.clear();
	// 删除数据缓冲区数组
	if (pADCDataBufArray->m_pArrayADCDataBuf != NULL)
	{
		for(unsigned int i = 0; i < pADCDataBufArray->m_uiCountAll; i++)
		{
			if (pADCDataBufArray->m_pArrayADCDataBuf[i].m_pADCDataBuf != NULL)
			{
				delete[] pADCDataBufArray->m_pArrayADCDataBuf[i].m_pADCDataBuf;
				pADCDataBufArray->m_pArrayADCDataBuf[i].m_pADCDataBuf = NULL;
			}
		}
		delete[] pADCDataBufArray->m_pArrayADCDataBuf;
		pADCDataBufArray->m_pArrayADCDataBuf = NULL;
	}
	LeaveCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
}
// 释放数据存储缓冲区结构体
void OnFreeADCDataBufArray(m_oADCDataBufArrayStruct* pADCDataBufArray)
{
	ASSERT(pADCDataBufArray != NULL);
	DeleteCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	delete pADCDataBufArray;
	pADCDataBufArray = NULL;
}
// 得到一个空闲数据存储缓冲区
m_oADCDataBufStruct* GetFreeADCDataBuf(m_oADCDataBufArrayStruct* pADCDataBufArray)
{
	ASSERT(pADCDataBufArray != NULL);
	m_oADCDataBufStruct* pADCDataBuf = NULL;
	EnterCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	if(pADCDataBufArray->m_uiCountFree > 0)	//有空闲数据存储缓冲区
	{
		// 从空闲数据存储缓冲区队列头部得到一个空闲数据存储缓冲区
		pADCDataBuf = *pADCDataBufArray->m_olsADCDataBufFree.begin();
		// 数据存储缓冲区是否使用中
		pADCDataBuf->m_bInUsed = true;
		pADCDataBufArray->m_olsADCDataBufFree.pop_front();
		// 空闲数据存储缓冲区计数减1
		pADCDataBufArray->m_uiCountFree--;
	}
	LeaveCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	return pADCDataBuf;
}
// 增加一个空闲数据存储缓冲区
void AddFreeADCDataBuf(m_oADCDataBufStruct* pADCDataBuf, 
	m_oADCDataBufArrayStruct* pADCDataBufArray)
{
	ASSERT(pADCDataBuf != NULL);
	ASSERT(pADCDataBufArray != NULL);
	EnterCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
	//初始化数据存储缓冲区
	OnADCDataBufReset(pADCDataBuf);
	//加入空闲队列
	pADCDataBufArray->m_olsADCDataBufFree.push_back(pADCDataBuf);
	// 空闲数据存储缓冲区计数加1
	pADCDataBufArray->m_uiCountFree++;
	LeaveCriticalSection(&pADCDataBufArray->m_oSecADCDataBufArray);
}
// 判断索引号是否已加入数据存储缓冲索引表
BOOL IfIndexExistInADCDataBufMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oADCDataBufStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter != pMap->end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}
// 向数据存储缓冲区索引表中加入缓冲区指针
void AddToADCDataBufMap(unsigned int uiIndex, m_oADCDataBufStruct* pADCDataBuf,
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	if (FALSE == IfIndexExistInADCDataBufMap(uiIndex, pMap))
	{
		pMap->insert(hash_map<unsigned int, m_oADCDataBufStruct*>::value_type (uiIndex, pADCDataBuf));
	}
}
// 根据输入索引号，由索引表得到数据存储缓冲区指针
m_oADCDataBufStruct* GetADCDataBufFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	hash_map<unsigned int, m_oADCDataBufStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}
// 从索引表删除索引号指向的数据存储缓冲区指针
BOOL DeleteADCDataBufFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oADCDataBufStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oADCDataBufStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter != pMap->end())
	{
		pMap->erase(iter);
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}