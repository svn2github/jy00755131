#include "StdAfx.h"

#include "RoutList.h"

CRoutList::CRoutList()
{
}

CRoutList::~CRoutList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CRoutList::OnInit()
{
	// 路由总数
	m_uiCountAll = 2048;
	// 空闲路由总数
	m_uiCountFree = m_uiCountAll;
	// 生成路由数组
	m_pArrayRout = new CRout[m_uiCountAll];
	// 生成路由数组，为现场数据输出准备
	m_pArrayRoutOutput = new CRout[m_uiCountAll];

	// 加入空闲路由队列
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 设置路由IP地址
		m_pArrayRout[i].m_uiRoutIP = i + 1;
		// 路由在路由数组中的位置
		m_pArrayRout[i].m_uiIndex = i;
		// 重置路由
		m_pArrayRout[i].OnReset();
		// 路由加在空闲路由队列尾部
		m_olsRoutFree.AddTail(&m_pArrayRout[i]);
	}
}

/**
* 关闭
* @param void
* @return void
*/
void CRoutList::OnClose()
{
	// 清空空闲路由队列
	m_olsRoutFree.RemoveAll();
	// 删除路由数组
	delete[] m_pArrayRout;
	// 删除路由数组，为现场数据输出准备
	delete[] m_pArrayRoutOutput;
}

/**
* 重置
* @param void
* @return void
*/
void CRoutList::OnReset()
{
	// 清空空闲路由队列
	m_olsRoutFree.RemoveAll();
	// 空闲路由总数
	m_uiCountFree = m_uiCountAll;
	// 加入空闲路由队列
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置路由
		m_pArrayRout[i].OnReset();
		// 路由加在空闲路由队列尾部
		m_olsRoutFree.AddTail(&m_pArrayRout[i]);
	}
}

/**
* 得到一个空闲路由
* @param void
* @return CRout* 空闲路由 NULL：没有空闲路由
*/
CRout* CRoutList::GetFreeRout()
{
	CRout* pRout = NULL;

	if(m_uiCountFree > 0)	//有空闲路由
	{
		// 从空闲路由队列头部得到一个空闲路由
		pRout = m_olsRoutFree.RemoveHead();
		// 路由是否使用中
		pRout->m_bInUsed = true;	
		// 空闲路由计数减1
		m_uiCountFree--;
	}
	return pRout;
}

/**
* 增加一个空闲路由
* @param CRout* pRout 路由指针
* @return void
*/
void CRoutList::AddFreeRout(CRout* pRout)
{
	//初始化路由
	pRout->OnReset();
	//加入空闲队列
	m_olsRoutFree.AddTail(pRout);
	// 空闲路由计数加1
	m_uiCountFree++;
}

/**
* 复制仪器数组到输出数组
* @param void
* @return void
*/
void CRoutList::CopyRoutArrayForOutput()
{
	memcpy(m_pArrayRoutOutput, m_pArrayRout, sizeof(CRout) * m_uiCountAll);
}