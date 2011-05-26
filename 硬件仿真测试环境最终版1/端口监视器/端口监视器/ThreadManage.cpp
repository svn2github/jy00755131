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
// 	m_ceRecThreadClose = CEvent(false, false, NULL, NULL);
// 	m_ceSendThreadClose = CEvent(false, false, NULL, NULL);

	m_RecThread.Init();
	m_RecThread.CreateThread();

	m_SendThread.Init();
	m_SendThread.CreateThread();
}
void CThreadManage::OnClose()
{
	int iResult = 0;
	m_RecThread.OnClose();
	iResult = ::WaitForSingleObject(m_RecThread.m_hRecThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("接收线程在100ms内未能正常结束！"));
	}
	m_SendThread.OnClose();
	iResult = ::WaitForSingleObject(m_SendThread.m_hSendThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("发送线程在100ms内未能正常结束！"));
	}
}