#include "StdAfx.h"

#include "Rout.h"

CRout::CRout()
{
}

CRout::~CRout()
{
}

/**
* 重置
* @param void
* @return void
*/
void CRout::OnReset()
{
	// 仪器是否使用中
	m_bInUsed = false;

	// 路由方向 1-上；2-下；3-左；4右
	m_uiRoutDirection = 0;
	// 路由头仪器
	m_pHead = NULL;
	// 路由尾仪器
	m_pTail = NULL;
	// 路由头仪器
	m_uiSNHead = 0;
	// 路由尾仪器
	m_uiSNTail = 0;
	// 路由头仪器
	m_iIndexHead = -1;
	// 路由尾仪器
	m_iIndexTail = -1;
	// 路由时刻
	m_uiRoutTime = 0;
	// 上次时统处理时刻
	m_uiDelayProcTime = 0;
}

/**
* 更新路由时间
* @param void
* @return void
*/
void CRout::UpdateRoutTime()
{
	// 路由时刻
	m_uiRoutTime = GetTickCount();
}

/**
* 更新上次时统处理时刻
* @param void
* @return void
*/
void CRout::UpdateDelayProcTime()
{
	// 上次时统处理时刻
	m_uiDelayProcTime = GetTickCount();
}