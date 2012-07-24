#include "stdafx.h"
#include "MatrixCommDll.h"

CCommThread::CCommThread(void)
	: m_dwThreadID(0)
	, m_hThread(INVALID_HANDLE_VALUE)
	, m_hThreadClose(INVALID_HANDLE_VALUE)
	, m_bClose(false)
{
}

CCommThread::~CCommThread(void)
{
}

/**
* @fn DWORD WINAPI RunThread(CCommThread* pClientCommThread)
* @detail 线程函数
* @param[in] pClientCommThread 通讯线程类指针	
* @return DWORD
*/
DWORD WINAPI RunThread(CCommThread* pClientCommThread)
{
	return pClientCommThread->ThreadRunFunc();
}

// 初始化
void CCommThread::OnInit(void)
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
void CCommThread::OnClose(void)
{
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}
	m_bClose = true;
	if (WaitForSingleObject(m_hThreadClose, CloseClientCommThreadTimes) != WAIT_OBJECT_0)
	{
		TerminateThread(m_hThread, 0);
	}
	else
	{
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}
	CloseHandle(m_hThreadClose);
	m_hThreadClose = INVALID_HANDLE_VALUE;
}

// 线程等待函数
void CCommThread::WaitForThread(void)
{
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(ClientCommThreadWaitTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(m_bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(iWaitCount == ClientCommThreadWaitTimes)
		{
			// 返回
			return;
		}		
	}
}

// 线程函数
DWORD CCommThread::ThreadRunFunc(void)
{
	while (1)
	{
		if (m_bClose == true)
		{
			break;
		}
		OnProc();
		if (m_bClose == true)
		{
			break;
		}
		WaitForThread();
	}
	SetEvent(m_hThreadClose);
	return 1;
}

// 处理函数
void CCommThread::OnProc(void)
{
}