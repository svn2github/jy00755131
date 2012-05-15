#include "stdafx.h"
#include "MatrixServerDll.h"

// 重置路由信息
void OnRoutReset(m_oRoutStruct* pRout)
{
	if (pRout == NULL)
	{
		return;
	}
	// 仪器是否使用中
	pRout->m_bInUsed = false;
	// 路由方向 1-上；2-下；3-左；4右
	pRout->m_iRoutDirection = 0;
	// 路由头仪器
	pRout->m_pHead = NULL;
	// 路由尾仪器
	pRout->m_pTail = NULL;
	// 路由仪器队列
	pRout->m_olsRoutInstrument.clear();
	// 路由时刻
	pRout->m_uiRoutTime = 0xFFFF0000;
	// 路由是否为交叉线路由
	pRout->m_bRoutLaux = false;
	// 路由是否接收到ADC设置参数应答
	pRout->m_bADCSetReturn = false;
	// 路由是否发送ADC参数设置帧
	pRout->m_bADCSetRout = true;
	// 该路由方向上仪器的个数
	pRout->m_uiInstrumentNum = 0;
}
// 更新路由对象的路由时间
void UpdateRoutTime(m_oRoutStruct* pRout)
{
	if (pRout == NULL)
	{
		return;
	}
	// 路由时刻
	pRout->m_uiRoutTime = GetTickCount();
}
// 路由地址是否已加入索引表
BOOL IfIndexExistInRoutMap(unsigned int uiRoutIP, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	if (pRoutMap == NULL)
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiRoutIP);
	if (iter != pRoutMap->end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}
// 增加一个路由
void AddRout(unsigned int uiIndex, m_oRoutStruct* pRout, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	if ((pRout == NULL) || (pRoutMap == NULL))
	{
		return;
	}
	if (false == IfIndexExistInRoutMap(uiIndex, pRoutMap))
	{
		pRoutMap->insert(hash_map<unsigned int, m_oRoutStruct*>::value_type (uiIndex, pRout));
	}
}
// 根据输入索引号，由索引表得到一个路由指针
m_oRoutStruct* GetRout(unsigned int uiIndex, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	if (pRoutMap == NULL)
	{
		return NULL;
	}
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiIndex);
	return iter->second;
}

// 根据输入索引号，从索引表中删除一个路由
void DeleteRout(unsigned int uiIndex, 
	hash_map<unsigned int, m_oRoutStruct*>* pRoutMap)
{
	if (pRoutMap == NULL)
	{
		return;
	}
	if (pRoutMap->size() == 0)
	{
		return;
	}
	hash_map<unsigned int, m_oRoutStruct*>::iterator iter;
	iter = pRoutMap->find(uiIndex);
	if (iter != pRoutMap->end())
	{
		pRoutMap->erase(iter);
	}
}
// 由路由IP得到路由结构体指针
bool GetRoutByRoutIP(unsigned int uiRoutIP, 
	m_oRoutListStruct* pRoutList, m_oRoutStruct** ppRout)
{
	bool bReturn = false;
	EnterCriticalSection(&pRoutList->m_oSecRoutList);
	// 在路由索引中找到该路由
	if (TRUE == IfIndexExistInRoutMap(uiRoutIP, &pRoutList->m_oRoutMap))
	{
		*ppRout = GetRout(uiRoutIP, &pRoutList->m_oRoutMap);
		bReturn = true;
	}
	LeaveCriticalSection(&pRoutList->m_oSecRoutList);
	return bReturn;
}