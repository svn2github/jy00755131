#include "StdAfx.h"
#include "ClientCommThread.h"


CClientCommThread::CClientCommThread(void)
	: m_dwThreadID(0)
	, m_hThread(INVALID_HANDLE_VALUE)
	, m_hThreadClose(INVALID_HANDLE_VALUE)
	, m_hThreadRun(INVALID_HANDLE_VALUE)
{
}


CClientCommThread::~CClientCommThread(void)
{
}

// 线程函数
DWORD WINAPI RunThread(CClientCommThread* pClientCommThread)
{
	return pClientCommThread->ThreadRunFunc();
}
// 初始化
void CClientCommThread::OnInit(void)
{
	m_hThreadClose = CreateEvent(false, false, NULL, NULL);
	m_hThreadRun = CreateEvent(false, false, NULL, NULL);
	ResetEvent(m_hThreadClose);
	ResetEvent(m_hThreadRun);
	m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunThread,
		this, 
		0, 
		&m_dwThreadID);
}


// 关闭
void CClientCommThread::OnClose(void)
{
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}
	SetEvent(m_hThreadRun);
	if (WaitForSingleObject(m_hThreadClose, CloseClientCommThreadTimes) != WAIT_OBJECT_0)
	{
		TerminateThread(m_hThread, 0);
	}
	else
	{
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}
	CloseHandle(m_hThreadRun);
	m_hThreadRun = INVALID_HANDLE_VALUE;
	CloseHandle(m_hThreadClose);
	m_hThreadClose = INVALID_HANDLE_VALUE;
}


// 线程函数
DWORD CClientCommThread::ThreadRunFunc(void)
{
	while (1)
	{
		OnProc();
		if(WaitForSingleObject(m_hThreadRun, RunClientCommThreadTimes) == WAIT_OBJECT_0) 
		{
			break;
		}
	}
	SetEvent(m_hThreadClose);
	return 1;
}
// 处理函数
void CClientCommThread::OnProc(void)
{
}