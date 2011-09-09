#include "StdAfx.h"
#include "ShotSources.h"
/**
 * @brief 构造函数
 */
CShotSources::CShotSources(void)
{
}
/**
 * @brief 析构函数
 */
CShotSources::~CShotSources(void)
{
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllSrc[i])
		{
			delete m_AllSrc[i];
			m_AllSrc[i] = NULL;
		}		
	}
	m_AllSrc.RemoveAll();
}
/**
 * @brief 删除所有震源对象
 * @note  删除所有震源对象
 * @param void
 * @return void
 */
void CShotSources::RemoveAll(void)
{
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllSrc[i])
		{
			delete m_AllSrc[i];
			m_AllSrc[i] = NULL;
		}		
	}
	m_AllSrc.RemoveAll();
}

/**
 * @brief 拷贝震源表数组
 * @note  从一个对象中拷贝全部震源表，仅仅是拷贝内存中震源表指针数组，
 并不是将震源表数据拷贝到该对象中。调用该函数后，本地对象和pShotSrcs对象指向内存
 中同一块的震源表区域。
 * @param CShotPoints* pPoints-放炮表
 * @return void
 */
void CShotSources::Copy(CShotSources* pShotSrcs)
{
	m_AllSrc.Copy(pShotSrcs->m_AllSrc);
}
/**
 * @brief 获得指定位置的震源对象
 * @note  返回震源表中指定位置的震源对象
 * @param int iIndex，索引值，索引从0开始
 * @return 成功返回震源指针，失败则返回NULL
 */
CShotSource* CShotSources::GetShotSource(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount)
		return NULL;

	return m_AllSrc[iIndex];
}

/**
 * @brief 计算总数
 * @note  计算震源表中总的震源数
 * @param void
 * @return 返回总的震源数
 */
int CShotSources::GetCount(void)
{
	return (int)m_AllSrc.GetCount();
}
/**
 * @brief 计算脉冲震源的总数
 * @note  计算震源表中总的脉冲震源数
 * @param void
 * @return 返回总的脉冲震源数
 */
int CShotSources::GetExploCount(void)
{
	int nExploCount=0;
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllSrc[i]->m_dwSourceType == SHOTTYPE_EXPLO)
		{
			nExploCount++;
		}		
	}
	return nExploCount;
}
/**
 * @brief 计算可控震源的总数
 * @note  计算震源表中总的可控震源数
 * @param void
 * @return 返回总的可控震源数
 */
int CShotSources::GetVibroCount(void)
{
	int nVibroCount=0;
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllSrc[i]->m_dwSourceType == SHOTTYPE_VIBRO)
		{
			nVibroCount++;
		}		
	}
	return nVibroCount;
}
/**
 * @brief 增加一个震源
 * @note  向震源表的尾端增加一个震源对象
 * @param CShotSource* pSrc，震源对象
 * @return 返回在数组中的位置
 */
int CShotSources::Add(CShotSource* pSrc)
{
	return (int)m_AllSrc.Add(pSrc);
}

/**
 * @brief 通过震源编号查找对象
 * @note  通过震源编号查找对象，返回震源对象
 * @param DWORD dwNb，震源编号
 * @return 成功则返回震源指针，失败则返回NULL
 */
CShotSource* CShotSources::GetShotSourceByNb(DWORD dwNb)
{
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllSrc[i]->m_dwSourceNb == dwNb)
		{			
			return m_AllSrc[i];
		}	
	}
	return NULL;
}
/**
 * @brief 设置指定位置的对象
 * @note  设置指定索引位置的对象，如果对象已经存在，则删除对象然后再添加；
 如果位置超过数组的范围，则动态增加数组，并设置指定位置的值。
 * @param DWORD dwNb，震源编号
 * @return 成功则返回震源指针，失败则返回NULL
 */
void CShotSources::SetAt(int nIndex,CShotSource* pShotSrc)
{
	int nCount = GetCount();
	if(nIndex<nCount)
	{
		delete m_AllSrc[nIndex];
		m_AllSrc[nIndex] = pShotSrc;
	}
	else
	{
		m_AllSrc.SetAtGrow(nIndex,pShotSrc);
	}

}