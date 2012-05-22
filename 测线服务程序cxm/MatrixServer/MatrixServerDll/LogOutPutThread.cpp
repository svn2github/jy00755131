#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建日志输出线程
m_oLogOutPutThreadStruct* OnCreateLogOutPutThread(void)
{
	m_oLogOutPutThreadStruct* pLogOutPutThread = NULL;
	pLogOutPutThread = new m_oLogOutPutThreadStruct;
	pLogOutPutThread->m_pThread = new m_oThreadStruct;
	pLogOutPutThread->m_pLogOutPutTimeDelay = NULL;
	pLogOutPutThread->m_pLogOutPutErrorCode = NULL;
	pLogOutPutThread->m_pLogOutPutADCFrameTime = NULL;
	InitializeCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	return pLogOutPutThread;
}
// 线程等待函数
void WaitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	bool bClose = false;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pLogOutPutThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		bClose = pLogOutPutThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		// 判断是否可以处理的条件
		if(bClose == true)
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(pLogOutPutThread->m_pThread->m_pConstVar->m_iLogOutPutSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return 0;
	}
	bool bClose = false;
	bool bWork = false;
	while(true)
	{
		EnterCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		bClose = pLogOutPutThread->m_pThread->m_bClose;
		bWork = pLogOutPutThread->m_pThread->m_bWork;
		LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		if (bClose == true)
		{
			break;
		}
		if (bWork == true)
		{
			WriteLogOutPutListToFile(pLogOutPutThread->m_pThread->m_pLogOutPut);
			WriteLogOutPutListToFile(pLogOutPutThread->m_pLogOutPutTimeDelay);
			WriteLogOutPutListToFile(pLogOutPutThread->m_pLogOutPutErrorCode);
			WriteLogOutPutListToFile(pLogOutPutThread->m_pLogOutPutADCFrameTime);
		}
		EnterCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		bClose = pLogOutPutThread->m_pThread->m_bClose;
		LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		if (bClose == true)
		{
			break;
		}
		WaitLogOutPutThread(pLogOutPutThread);
	}
	EnterCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pLogOutPutThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	return 1;
}
// 初始化日志输出线程
bool OnInitLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pLogOutPutThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	if (false == OnInitThread(pLogOutPutThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		return false;
	}
	// 设置事件对象为无信号状态
	ResetEvent(pLogOutPutThread->m_pThread->m_hThreadClose);
	// 创建线程
	pLogOutPutThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunLogOutPutThread,
		pLogOutPutThread, 
		0, 
		&pLogOutPutThread->m_pThread->m_dwThreadID);
	if (pLogOutPutThread->m_pThread->m_hThread == NULL)
	{
		LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", 
			"pLogOutPutThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		return false;
	}
	LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnInitLogOutPutThread", 
		"日志输出线程创建成功");
	return true;
}
// 初始化日志输出线程
bool OnInit_LogOutPutThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pLogOutPutThread->m_pLogOutPutTimeDelay = pEnv->m_pLogOutPutTimeDelay;
	pEnv->m_pLogOutPutThread->m_pLogOutPutErrorCode = pEnv->m_pLogOutPutErrorCode;
	pEnv->m_pLogOutPutThread->m_pLogOutPutADCFrameTime = pEnv->m_pLogOutPutADCFrameTime;
	return OnInitLogOutPutThread(pEnv->m_pLogOutPutThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}

// 关闭日志输出线程
bool OnCloseLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	if (false == OnCloseThread(pLogOutPutThread->m_pThread))
	{
		LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
		AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnCloseLogOutPutThread", 
			"日志输出线程强制关闭", WarningType);
		return false;
	}
	LeaveCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	AddMsgToLogOutPutList(pLogOutPutThread->m_pThread->m_pLogOutPut, "OnCloseLogOutPutThread", 
		"日志输出线程成功关闭");
	return true;
}
// 释放日志输出线程
void OnFreeLogOutPutThread(m_oLogOutPutThreadStruct* pLogOutPutThread)
{
	if (pLogOutPutThread == NULL)
	{
		return;
	}
	if (pLogOutPutThread->m_pThread != NULL)
	{
		delete pLogOutPutThread->m_pThread;
		pLogOutPutThread->m_pThread = NULL;
	}
	DeleteCriticalSection(&pLogOutPutThread->m_oSecLogOutPutThread);
	delete pLogOutPutThread;
	pLogOutPutThread = NULL;
}