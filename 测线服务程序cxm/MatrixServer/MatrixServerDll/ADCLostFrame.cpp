#include "stdafx.h"
#include "MatrixServerDll.h"

// 判断索引号是否已加入索引表
BOOL IfIndexExistInADCFrameLostMap(unsigned int uiIP, unsigned short usFramePointNb, 
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap)
{
	if (pMap == NULL)
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	m_oADCLostFrameKeyStruct Key(uiIP, usFramePointNb);
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>::iterator iter;
	iter = pMap->find(Key);
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
// 增加到索引
void AddToADCFrameLostMap(unsigned int uiIP, unsigned short usFramePointNb, 
	m_oADCLostFrameStruct oLostFrame, map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap)
{
	if (pMap == NULL)
	{
		return;
	}
	m_oADCLostFrameKeyStruct Key(uiIP, usFramePointNb);
	if (false == IfIndexExistInADCFrameLostMap(uiIP, usFramePointNb, pMap))
	{
		pMap->insert(map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>::value_type (Key, oLostFrame));
	}
}
// 根据输入索引号，由索引表得到仪器指针
ADCLostFrame_Struct* GetFromADCFrameLostMap(unsigned int uiIP, unsigned short usFramePointNb, 
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap)
{
	if (pMap == NULL)
	{
		return NULL;
	}
	m_oADCLostFrameKeyStruct Key(uiIP, usFramePointNb);
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>::iterator iter;
	iter = pMap->find(Key);
	return &iter->second;
}
