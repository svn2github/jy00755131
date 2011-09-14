#include "StdAfx.h"

#include "IndexRoutMap.h"

CIndexRoutMap::CIndexRoutMap()
{
}

CIndexRoutMap::~CIndexRoutMap()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CIndexRoutMap::OnInit()
{

}

/**
* 关闭
* @param void
* @return void
*/
void CIndexRoutMap::OnClose()
{
	// 删除所有路由
	DeleteRoutAll();
}

/**
* 重置
* @param void
* @return void
*/
void CIndexRoutMap::OnReset()
{
	// 删除所有路由
	DeleteRoutAll();
}

/**
* 增加一个路由
* @param unsigned int uiIndex 索引号
* @param CRout* pRout	路由指针
* @return void
*/
// 
void CIndexRoutMap::AddRout(unsigned int uiIndex, CRout* pRout)
{
	m_oRoutMap.SetAt(uiIndex, pRout);
}

/**
* 根据输入索引号，由索引表得到一个路由指针
* @param unsigned int uiIndex 索引号
* @param CRout* &pRout	路由指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CIndexRoutMap::GetRout(unsigned int uiIndex, CRout* &pRout)
{
	return m_oRoutMap.Lookup(uiIndex, pRout);
}

/**
* 根据输入索引号，从索引表中删除一个路由
* @param unsigned int uiIndex 索引号
* @return void
*/
void CIndexRoutMap::DeleteRout(unsigned int uiIndex)
{
	m_oRoutMap.RemoveKey(uiIndex);
}

/**
* 删除索引表中所有路由
* @param void
* @return void
*/
void CIndexRoutMap::DeleteRoutAll()
{
	m_oRoutMap.RemoveAll();
}

/**
* 路由地址是否已加入索引表
* @param unsigned int uiRoutIP 路由地址
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CIndexRoutMap::IfIndexExistInMap(unsigned int uiRoutIP)
{
	CRout* pRout = NULL;
	return m_oRoutMap.Lookup(uiRoutIP, pRout);
}

/**
* 判断路由是否是最久没有时延修正的路由
* @param CRout* &pRout	路由指针
* @return BOOL TRUE：是；FALSE：否
*/
bool CIndexRoutMap::JudgeRoutIsLongestTime(CRout* pRout)
{
	if(0 == pRout->m_uiDelayProcTime)
	{
		return true;
	}

	CRout* pRoutFind = NULL;	// 路由对象指针
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey;	// 索引键	
	pos = m_oRoutMap.GetStartPosition();	// 得到索引表起始路由位置
	while(NULL != pos)
	{
		pRoutFind = NULL;		
		m_oRoutMap.GetNextAssoc(pos, uiKey, pRoutFind);	// 得到路由对象
		if(NULL != pRoutFind)	// 得到路由对象
		{
			if(NULL != pRoutFind->m_pTail)	// 链接有仪器
			{
				if(0 != pRoutFind->m_pTail->m_uiIP)	// 尾仪器已分配IP地址
				{
					if(pRout->m_uiDelayProcTime > pRoutFind->m_uiDelayProcTime)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}