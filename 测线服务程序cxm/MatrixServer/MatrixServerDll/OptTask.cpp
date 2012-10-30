#include "stdafx.h"
#include "MatrixServerDll.h"

// 重置施工任务
void OnOptTaskReset(m_oOptTaskStruct* pOptTask)
{
	ASSERT(pOptTask != NULL);
	m_oOptInstrumentStruct* pOptInstr = NULL;
	// 任务是否使用中
	pOptTask->m_bInUsed = false;
	// 施工任务开始记录的时间
	pOptTask->m_uiTB = 0;
	// 施工任务停止记录的时间
	pOptTask->m_uiTS = 0;
	/** 每个站存储的数据点数*/
	pOptTask->m_uiSavePointNum = 0;
	/** 参与施工的采集站个数*/
	pOptTask->m_uiOptInstrNum = 0;
	/** 判断是否写入SEGD文件标志位*/
	pOptTask->m_bSaveInSegd = false;
// 	// 存储一帧所需时间
// 	pOptTask->m_uiOneFrameTime = 0;
	// 施工数据输出文件指针
	pOptTask->m_pFile = NULL;
// 	// 施工数据输出前一个文件的文件指针
// 	pOptTask->m_pPreviousFile = NULL;
// 	// 最新的文件存储序号
// 	pOptTask->m_uiFileSaveNb = 0;
	// 施工任务IP索引表，关键字为IP，内容为施工仪器指针
	while(pOptTask->m_oIPMap.size() != 0)
	{
		pOptInstr = pOptTask->m_oIPMap.begin()->second;
		delete pOptInstr;
		pOptInstr = NULL;
		pOptTask->m_oIPMap.erase(pOptTask->m_oIPMap.begin());
	}
	// 分配存储单元标志位
	pOptTask->m_bSaveBuf = false;
	// 分配存储单元序号
	pOptTask->m_uiSaveBufNo = 0;
// 	// 参与施工的仪器队列
// 	pOptTask->m_olsOptInstrument.clear();
}

// 判断索引号是否已加入施工任务索引表
BOOL IfIndexExistInOptTaskMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oOptTaskStruct*>::iterator iter;
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
// 判断仪器SN号是否已加入施工任务仪器索引表
BOOL IfIndexExistInOptTaskIPMap(unsigned int uiIndex,
	hash_map<unsigned int, m_oOptInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oOptInstrumentStruct*>::iterator iter;
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
// 从施工任务仪器索引表中得到仪器的行号
// unsigned int GetLineNbFromOptTaskSNMap(unsigned int uiIndex,
// 	hash_map<unsigned int, unsigned int>* pMap)
// {
// 	ASSERT(pMap != NULL);
// 	hash_map<unsigned int, unsigned int>::iterator iter;
// 	iter = pMap->find(uiIndex);
// 	return iter->second;
// }
// 向施工任务仪器索引表中加入仪器
void AddToOptTaskIPMap(unsigned int uiIndex, m_oOptInstrumentStruct* pOptInstr,
	hash_map<unsigned int, m_oOptInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	if (FALSE == IfIndexExistInOptTaskIPMap(uiIndex, pMap))
	{
		pMap->insert(hash_map<unsigned int, m_oOptInstrumentStruct*>::value_type (uiIndex, pOptInstr));
	}
}
// 增加一个施工任务
void AddOptTaskToMap(unsigned int uiIndex, m_oOptTaskStruct* pOptTask, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap)
{
	ASSERT(pOptTask != NULL);
	ASSERT(pMap != NULL);
	if (FALSE == IfIndexExistInOptTaskMap(uiIndex, pMap))
	{
		pMap->insert(hash_map<unsigned int, m_oOptTaskStruct*>::value_type (uiIndex, pOptTask));
	}
}
// 根据输入索引号，由索引表得到施工任务指针
m_oOptTaskStruct* GetOptTaskFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	hash_map<unsigned int, m_oOptTaskStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}
// 根据输入索引号，由索引表得到施工仪器指针
m_oOptInstrumentStruct* GetOptTaskFromIPMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptInstrumentStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	hash_map<unsigned int, m_oOptInstrumentStruct*>::iterator iter;
	iter = pMap->find(uiIndex);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}
// 从索引表删除索引号指向的施工任务指针
BOOL DeleteOptTaskFromMap(unsigned int uiIndex, 
	hash_map<unsigned int, m_oOptTaskStruct*>* pMap)
{
	ASSERT(pMap != NULL);
	BOOL bResult = FALSE;
	hash_map<unsigned int, m_oOptTaskStruct*>::iterator iter;
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