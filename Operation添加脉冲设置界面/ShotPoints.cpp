#include "StdAfx.h"
#include "ShotPoints.h"
/**
 * @brief CShotPoints构造函数
 */
CShotPoints::CShotPoints(void)
{
}
/**
 * @brief CShotPoints析构函数
 *
 * 先释放每个炮号对象，再清空放炮表。
 */
CShotPoints::~CShotPoints(void)
{
	RemoveAll();
}
/**
 * @brief 删除所有放炮对象
 * @note  删除所有放炮对象
 * @param void
 * @return void
 */
void   CShotPoints::RemoveAll(void)
{
	int i,nCount = m_AllVP.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllVP[i])
		{
			delete m_AllVP[i];
			m_AllVP[i] = NULL;
		}	
	}
	m_AllVP.RemoveAll();
}
/**
 * @brief 拷贝放炮表
 * @note  从一个对象中拷贝全部放炮表，仅仅是拷贝内存中放炮表指针数组，
 并不是将放炮表数据拷贝到该对象中。调用该函数后，本地对象和pPoints对象指向内存
 中同一块的放炮表区域。
 * @param CShotPoints* pPoints-放炮表
 * @return void
 */
void   CShotPoints::Copy(CShotPoints*  pPoints)
{
	m_AllVP.Copy(pPoints->m_AllVP);
}
/**
 * @brief 增加一个炮号
 * @note  向放炮表的尾端增加一个炮号对象
 * @param CShotPoint* pVP，炮号对象
 * @return 返回在数组中的位置
 */
int CShotPoints::Add(CShotPoint* pVP)
{
	return m_AllVP.Add(pVP);
}
/**
 * @brief 计算总数
 * @note  计算放炮表中总的炮号数
 * @param void
 * @return 返回总的炮号数
 */
int CShotPoints::GetCount(void)
{
	return m_AllVP.GetCount();
}

/**
 * @brief 获得指定位置的炮号对象
 * @note  返回放炮表中指定位置的炮号对象
 * @param int iIndex，索引值，索引从0开始
 * @return 成功返回炮号指针，失败则返回NULL
 */
CShotPoint* CShotPoints::GetShotPoint(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount)
		return NULL;

	return m_AllVP[iIndex];

}
/**
 * @brief 通过炮点编号查找对象
 * @note  通过炮点编号查找对象，返回炮号对象
 * @param DWORD dwNb，炮点编号
 * @return 成功则返回炮号指针，失败则返回NULL
 */
CShotPoint* CShotPoints::GetShotPointByNb(DWORD dwNb)
{
	int i,nCount = m_AllVP.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllVP[i]->m_dwShotNb == dwNb)
		{			
			return m_AllVP[i];
		}	
	}
	return NULL;
}