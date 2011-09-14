#include "StdAfx.h"
#include "PointList.h"

CPointList::CPointList()
{
}

CPointList::~CPointList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CPointList::OnInit()
{
	// 生成测点数组
	m_pArrayPoint = new CPointSetupData[m_uiCountAll];
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测点数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 测点在测点数组中的位置
		m_pArrayPoint[i].m_uiIndex = i;
		// 重置测点
		m_pArrayPoint[i].OnReset();
	}
}

/**
* 重新加载
* @param void
* @return void
*/
void CPointList::OnReload()
{
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测点数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置测点
		m_pArrayPoint[i].OnReset();
	}
	// 清空索引表
	m_oPointMap.RemoveAll();
}

/**
* 关闭
* @param void
* @return void
*/
void CPointList::OnClose()
{
	// 删除测点数组
	delete[] m_pArrayPoint;
	// 清空索引表
	m_oPointMap.RemoveAll();
}

/**
* 得到一个空闲测点
* @param void
* @return CChannelSetupData* 空闲测点 NULL：没有空闲测点
*/
CPointSetupData* CPointList::GetFreePoint()
{
	CPointSetupData* pPoint = NULL;

	if(m_uiCountFree > 0)	//有空闲测点
	{
		pPoint = &m_pArrayPoint[m_uiCountUsed];
		pPoint->m_bInUsed = true;	// 设置测点为使用中
		m_uiCountFree--;	// 空闲测点总数减1
		m_uiCountUsed++;	// 使用中测线总数加1
	}
	return pPoint;
}

/**
* 测道加入测点索引表
* @param unsigned int uiIndex 索引号
* @param CPointSetupData* pPoint 测点指针
* @return void
*/
void CPointList::AddPointToMap(unsigned int uiIndex, CPointSetupData* pPoint)
{
	m_oPointMap.SetAt(uiIndex, pPoint);
}

/**
* 测点索引号是否已加入索引表
* @param unsigned int uiIndex 索引号
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CPointList::IfIndexExistInMap(unsigned int uiIndex)
{
	CPointSetupData* pPoint = NULL;
	return m_oPointMap.Lookup(uiIndex, pPoint);
}

/**
* 由索引表得到一个测点
* @param unsigned int uiIndex 索引号
* @param CPointSetupData* &pPoint 测点指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CPointList::GetPointFromMap(unsigned int uiIndex, CPointSetupData* &pPoint)
{
	return m_oPointMap.Lookup(uiIndex, pPoint);
}

/**
* 根据数组索引，在测道数组中得到一个测点
* @param unsigned int uiIndex 数组索引号
* @return CPointSetupData* 测点
*/
CPointSetupData* CPointList::GetPointByIndex(unsigned int uiIndex)
{
	return &m_pArrayPoint[uiIndex];
}

/**
* 根据测线号、测点号，在索引表中得到测点
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @return CPointSetupData* 测点 NULL：没有测点
*/
CPointSetupData* CPointList::GetPoint(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	CPointSetupData* pPoint = NULL;
	unsigned int m_uiIndexPoint = CPointSetupData::CreateIndexPoint(uiNbLine, uiNbPoint);
	if(TRUE == GetPointFromMap(m_uiIndexPoint, pPoint))
	{
		return pPoint;
	}
	return NULL;
}