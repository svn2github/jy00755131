#include "StdAfx.h"
#include "ShotSpreads.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*
CShotSpreadEx::CShotSpreadEx(void) : CShotSpread()
{
}
CShotSpreadEx::~CShotSpreadEx(void) 
{
	m_arrSpreadDesc.RemoveAll();
}
*/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
 * @brief 构造函数
 */
CShotSpreads::CShotSpreads(void)
{
}
/**
 * @brief 析构函数
 */
CShotSpreads::~CShotSpreads(void)
{
	RemoveAll();
}
/**
 * @brief 删除所有采集排列对象，释放内存
 * @note  删除所有采集排列对象，释放内存
 * @param void
 * @return void
 */
void CShotSpreads::RemoveAll()
{
	int i,nCount = m_AllSpread.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllSpread[i])
		{
			delete m_AllSpread[i];
			m_AllSpread[i] = NULL;
		}		
	}
	m_AllSpread.RemoveAll();
}
/**
 * @brief 获得指定位置的采集排列对象
 * @note  返回采集排列表中指定位置的排列对象
 * @param int iIndex，索引值，索引从0开始
 * @return 成功返回震源指针，失败则返回NULL
 */
CShotSpread* CShotSpreads::GetShotSpread(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount)
		return NULL;

	return m_AllSpread[iIndex];
}
/**
 * @brief 计算总数
 * @note  计算采集排列表中定义的采集排列总数
 * @param void
 * @return 返回总的采集排列数
 */
int CShotSpreads::GetCount(void)
{
	return m_AllSpread.GetCount();
}

/**
 * @brief 增加一个采集排列
 * @note  向采集排列表的尾端增加一个采集排列对象
 * @param CShotSpread* pSrc，采集排列对象
 * @return 返回在数组中的位置
 */
int CShotSpreads::Add(CShotSpread* pSrc)
{
	return m_AllSpread.Add(pSrc);
}

/**
 * @brief 通过采集排列编号查找对象
 * @note  通过采集排列编号查找对象，返回采集排列对象
 * @param DWORD dwNb，采集排列编号
 * @return 成功则返回采集排列指针，失败则返回NULL
 */
CShotSpread* CShotSpreads::GetShotSpreadBySN(DWORD dwSN)
{
	CShotSpread* pSpread=NULL;
	int i,nCount = GetCount();
	for(i=0;i<nCount;i++)
	{
		pSpread = m_AllSpread[i];
		if(pSpread->m_dwNb == dwSN)
			return pSpread;
	}
	return NULL;
}