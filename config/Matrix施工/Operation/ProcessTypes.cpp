#include "StdAfx.h"
#include "ProcessTypes.h"
/**
 * @brief 构造函数
 */
CProcessTypes::CProcessTypes(void)
{
}
/**
 * @brief 析构函数
 */
CProcessTypes::~CProcessTypes(void)
{
	int i,nCount = m_AllProcess.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllProcess[i])
		{
			delete m_AllProcess[i];
			m_AllProcess[i] = NULL;
		}	
	}
	m_AllProcess.RemoveAll();
}
/**
 * @brief 删除所有的处理类型对象
 * @note  删除该对象中保存的处理类型对象，并清空对象指针数组。
 * @param void
 * @return void
 */
void CProcessTypes:: RemoveAll(void)
{
	int i,nCount = m_AllProcess.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllProcess[i])
		{
			delete m_AllProcess[i];
			m_AllProcess[i] = NULL;
		}	
	}
	m_AllProcess.RemoveAll();
}

/**
 * @brief 获得处理类型对象
 * @note  返回处理类型表中指定位置的处理类型对象
 * @param int iIndex，索引值，索引从0开始
 * @return 成功返回处理类型对象指针，失败则返回NULL
 */
CProcessType* CProcessTypes::GetProcessType(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount)
		return NULL;
	return m_AllProcess[iIndex];
}
/**
 * @brief 计算总数
 * @note  计算处理类型对象的总数
 * @param void
 * @return 返回类型对象的总数
 */
int CProcessTypes::GetCount(void)
{
	return m_AllProcess.GetCount();
}
/**
 * @brief 增加一个处理类型对象
 * @note  向处理类型对象表的尾端增加一个处理类型对象
 * @param CProcessType* pProcess，处理类型对象
 * @return 返回在数组中的位置
 */
int CProcessTypes::Add(CProcessType* pProcess)
{
	return m_AllProcess.Add(pProcess);
}

/**
 * @brief 通过处理类型编号查找对象
 * @note  通过处理类型编号查找对象，返回处理类型对象
 * @param DWORD dwNb，处理类型编号
 * @return 成功则返回处理类型指针，失败则返回NULL
 */
CProcessType* CProcessTypes::GetProcessTypeByNb(DWORD dwNb)
{
	int i,nCount = m_AllProcess.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllProcess[i]->m_dwProcessNb == dwNb)
		{			
			return m_AllProcess[i];
		}	
	}
	return NULL;
}
/**
 * @brief 通过处理类型编号查找对象
 * @note  通过处理类型编号查找对象，返回处理类型对象的位置
 * @param DWORD dwNb，处理类型编号
 * @return 数字中的索引位置，失败则返回-1
 */
int  CProcessTypes::GetProcessIndexByNb(DWORD dwNb)
{
	int i,nCount = m_AllProcess.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllProcess[i]->m_dwProcessNb == dwNb)
		{			
			return i;
		}	
	}
	return -1;
}