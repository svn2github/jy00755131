#include "StdAfx.h"
#include "ThreadManage.h"


CThreadManage::CThreadManage(void)
{
}

CThreadManage::~CThreadManage(void)
{
	
}

// 初始化
void CThreadManage::Init()
{
	m_RecThread.Init();
	m_RecThread.CreateThread();

	m_SendThread.Init();
	m_SendThread.CreateThread();

}
void CThreadManage::OnClose()
{
	m_RecThread.OnClose();
	m_SendThread.OnClose();

	while(true)	// 等待线程关闭
	{
		if((true == m_RecThread.m_close)	
			&& (true == m_SendThread.m_close))	
		{
			break;
		}
		Sleep(50);	// 休眠，等待线程处理关闭
	}
}