#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建心跳线程
m_oHeartBeatThreadStruct* OnCreateHeartBeatThread(void)
{
	m_oHeartBeatThreadStruct* pHeartBeatThread = NULL;
	pHeartBeatThread = new m_oHeartBeatThreadStruct;
	pHeartBeatThread->m_pThread = new m_oThreadStruct;
	pHeartBeatThread->m_pHeartBeatFrame = NULL;
	InitializeCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	return pHeartBeatThread;
}
// 线程等待函数
void WaitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return;
	}
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(pHeartBeatThread->m_pThread->m_pConstVar->m_iOneSleepTime);
		// 等待次数加1
		iWaitCount++;
		EnterCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		// 判断是否可以处理的条件
		if(pHeartBeatThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
			// 返回
			return;
		}
		LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		// 判断等待次数是否大于等于最多等待次数
		if(pHeartBeatThread->m_pThread->m_pConstVar->m_iHeartBeatSleepTimes == iWaitCount)
		{
			// 返回
			return;
		}		
	}
}
// 线程函数
DWORD WINAPI RunHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return 0;
	}
	while(true)
	{
		EnterCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		if (pHeartBeatThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
			break;
		}
		if (pHeartBeatThread->m_pThread->m_bWork == true)
		{
			MakeInstrumentHeartBeatFrame(pHeartBeatThread->m_pHeartBeatFrame, 
				pHeartBeatThread->m_pThread->m_pConstVar);
			SendInstrumentHeartBeatFrame(pHeartBeatThread->m_pHeartBeatFrame, 
				pHeartBeatThread->m_pThread->m_pConstVar);
		}
		if (pHeartBeatThread->m_pThread->m_bClose == true)
		{
			LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
			break;
		}
		LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		WaitHeartBeatThread(pHeartBeatThread);
	}
	EnterCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	SetEvent(pHeartBeatThread->m_pThread->m_hThreadClose);
	LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	return 1;
}
// 初始化心跳线程
bool OnInitHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread, 
	m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pHeartBeatThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	if (false == OnInitThread(pHeartBeatThread->m_pThread, pLogOutPut, pConstVar))
	{
		LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		return false;
	}
	ResetEvent(pHeartBeatThread->m_pThread->m_hThreadClose);	// 设置事件对象为无信号状态
	// 创建线程
	pHeartBeatThread->m_pThread->m_hThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 
		0,
		(LPTHREAD_START_ROUTINE)RunHeartBeatThread,
		pHeartBeatThread, 
		0, 
		&pHeartBeatThread->m_pThread->m_dwThreadID);
	if (pHeartBeatThread->m_pThread->m_hThread == NULL)
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnInitHeartBeatThread", 
			"pHeartBeatThread->m_pThread->m_hThread", ErrorType, IDS_ERR_CREATE_THREAD);
		LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		return false;
	}
	AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnInitHeartBeatThread", 
		"心跳线程创建成功");
	LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	return true;
}
// 初始化心跳线程
bool OnInit_HeartBeatThread(m_oEnvironmentStruct* pEnv)
{
	if (pEnv == NULL)
	{
		return false;
	}
	pEnv->m_pHeartBeatThread->m_pHeartBeatFrame = pEnv->m_pHeartBeatFrame;
	return OnInitHeartBeatThread(pEnv->m_pHeartBeatThread, pEnv->m_pLogOutPutOpt, pEnv->m_pConstVar);
}
// 关闭心跳线程
bool OnCloseHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return false;
	}
	EnterCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	if(false == OnCloseThread(pHeartBeatThread->m_pThread))
	{
		AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnCloseHeartBeatThread", 
			"心跳线程强制关闭", WarningType);
		LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
		return false;
	}
	AddMsgToLogOutPutList(pHeartBeatThread->m_pThread->m_pLogOutPut, "OnCloseHeartBeatThread", 
		"心跳线程成功关闭");
	LeaveCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	return true;
}
// 释放心跳线程
void OnFreeHeartBeatThread(m_oHeartBeatThreadStruct* pHeartBeatThread)
{
	if (pHeartBeatThread == NULL)
	{
		return;
	}
	if (pHeartBeatThread->m_pThread != NULL)
	{
		delete pHeartBeatThread->m_pThread;
	}
	DeleteCriticalSection(&pHeartBeatThread->m_oSecHeartBeatThread);
	delete pHeartBeatThread;
}