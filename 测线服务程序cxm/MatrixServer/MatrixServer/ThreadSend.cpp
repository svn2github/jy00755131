#include "StdAfx.h"
#include "ThreadSend.h"


CThreadSend::CThreadSend(void)
	: m_dwThreadID(0)
	, m_bClose(false)
	, m_hThread(INVALID_HANDLE_VALUE)
	, m_hThreadClose(INVALID_HANDLE_VALUE)
{
}


CThreadSend::~CThreadSend(void)
{
}

// 线程函数
DWORD WINAPI RunThread(CThreadSend* pClass)
{
	return pClass->ThreadRunFunc();
}
// 初始化
void CThreadSend::OnInit(void)
{
	m_hThreadClose = CreateEvent(false, false, NULL, NULL);
	ResetEvent(m_hThreadClose);
	m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunThread,
		this, 
		0, 
		&m_dwThreadID);
}


// 关闭
void CThreadSend::OnClose(void)
{
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}
	m_bClose = true;
	int iResult = WaitForSingleObject(m_hThreadClose, WaitForCloseSendThreadTimes);
	if (iResult != WAIT_OBJECT_0)
	{
		TerminateThread(m_hThread, 0);
	}
	CloseHandle(m_hThread);
	CloseHandle(m_hThreadClose);
}
// 线程函数
DWORD CThreadSend::ThreadRunFunc(void)
{
	while (1)
	{
		if (m_bClose == true)
		{
			break;
		}

		if (m_bClose == true)
		{
			break;
		}
		Sleep(WaitForSendThreadSleepTimes);
	}
	SetEvent(m_hThreadClose);
	return 1;
}