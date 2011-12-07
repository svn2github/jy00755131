#include "StdAfx.h"
#include "LineList.h"

CLineList::CLineList()
{
}

CLineList::~CLineList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CLineList::OnInit()
{
	// 生成测线数组
	m_pArrayLine = new CLineSetupData[m_uiCountAll];
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测线数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 测线在测线数组中的位置
		m_pArrayLine[i].m_uiIndex = i;
		// 重置测线
		m_pArrayLine[i].OnReset();
	}
}

/**
* 重新加载
* @param void
* @return void
*/
void CLineList::OnReload()
{
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测线数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置测线
		m_pArrayLine[i].OnReset();
	}
	// 清空索引表
	m_oLineMap.RemoveAll();
}

/**
* 关闭
* @param void
* @return void
*/
void CLineList::OnClose()
{
	// 删除测线数组
	delete[] m_pArrayLine;
	// 清空索引表
	m_oLineMap.RemoveAll();
}

/**
* 得到一个空闲测线
* @param void
* @return CLineSetupData* 空闲测线 NULL：失败
*/
CLineSetupData* CLineList::GetFreeLine()
{
	CLineSetupData* pLine = NULL;

	if(m_uiCountFree > 0)	//有空闲测线
	{
		pLine = &m_pArrayLine[m_uiCountUsed];
		pLine->m_bInUsed = true;	// 设置测线为使用中
		m_uiCountFree--;	// 空闲测线总数减1
		m_uiCountUsed++;	// 使用中测线总数加1
	}
	return pLine;
}

/**
* 增加一个测线到索引表
* @param unsigned int uiIndex 测线标识号
* @param CLineSetupData* pLine 测线指针
* @return void
*/
void CLineList::AddLineToMap(unsigned int uiIndex, CLineSetupData* pLine)
{
	m_oLineMap.SetAt(uiIndex, pLine);
}

/**
* 测线索引号是否已加入索引表
* @param unsigned int uiIndex 索引号
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CLineList::IfIndexExistInMap(unsigned int uiIndex)
{
	CLineSetupData* pLine = NULL;
	return m_oLineMap.Lookup(uiIndex, pLine);
}

/**
* 由索引表得到一个测线
* @param unsigned int uiIndex 索引号
* @param CLineSetupData* &pLine 测线指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CLineList::GetLineFromMap(unsigned int uiIndex, CLineSetupData* &pLine)
{
	return m_oLineMap.Lookup(uiIndex, pLine);
}

/**
* 根据数组索引，在测线数组中得到一个测线
* @param unsigned int uiIndex 数组索引号
* @return CLineSetupData* 测线
*/
CLineSetupData* CLineList::GetLineByIndex(unsigned int uiIndex)
{
	return &m_pArrayLine[uiIndex];
}

/**
* 由索引表得到一个测线
* @param unsigned int uiNbLine 测线号
* @return CLineSetupData* 测线 NULL：失败
*/
CLineSetupData* CLineList::GetLine(unsigned int uiNbLine)
{
	CLineSetupData* pLine = NULL;
	if(TRUE == GetLineFromMap(uiNbLine, pLine))
	{
		return pLine;
	}
	return NULL;
}