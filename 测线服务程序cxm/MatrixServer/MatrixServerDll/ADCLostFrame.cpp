#include "stdafx.h"
#include "MatrixServerDll.h"

// 判断索引号是否已加入索引表
BOOL IfIndexExistInADCFrameLostMap(m_oADCLostFrameKeyStruct Key, 
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap)
{
	if (pMap == NULL)
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
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
void AddToADCFrameLostMap(m_oADCLostFrameKeyStruct Key, 
	m_oADCLostFrameStruct oLostFrame, map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap)
{
	if (pMap == NULL)
	{
		return;
	}
	if (false == IfIndexExistInADCFrameLostMap(Key, pMap))
	{
		pMap->insert(map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>::value_type (Key, oLostFrame));
	}
}
// 根据输入索引号，由索引表得到仪器指针
ADCLostFrame_Struct* GetFromADCFrameLostMap(m_oADCLostFrameKeyStruct Key, 
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>* pMap)
{
	if (pMap == NULL)
	{
		return NULL;
	}
	map<m_oADCLostFrameKeyStruct, m_oADCLostFrameStruct>::iterator iter;
	iter = pMap->find(Key);
	return &iter->second;
}
