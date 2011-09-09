#include "StdAfx.h"
#include "ShotComment.h"

CShotComment::CShotComment(void)
: m_dwCommentNb(0)
, m_strLabel(_T(""))
, m_strComment(_T(""))
{
}

CShotComment::~CShotComment(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

CShotComments::CShotComments(void)
{

}

CShotComments::~CShotComments(void)
{
	RemoveAll();
}
/**
 * @brief 删除所有注释对象
 * @note  删除所有注释对象
 * @param void
 * @return void
 */
void   CShotComments::RemoveAll(void)
{
	int i,nCount = m_AllComments.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_AllComments[i])
		{
			delete m_AllComments[i];
			m_AllComments[i] = NULL;
		}	
	}
	m_AllComments.RemoveAll();
}
/**
 * @brief 获得指定位置的对象
 * @note  返回注释表表中指定位置的注释对象
 * @param int iIndex，索引值，索引从0开始
 * @return 成功返回指针，失败则返回NULL
 */
CShotComment* CShotComments::GetShotComment(int nIndex)
{
	int nCount = GetCount();
	if(nIndex>=nCount)
		return NULL;
	
	return m_AllComments[nIndex];
}

