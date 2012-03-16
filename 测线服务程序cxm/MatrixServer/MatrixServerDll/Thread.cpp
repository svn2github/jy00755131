#include "stdafx.h"
#include "MatrixServerDll.h"

// 初始化线程函数
bool OnInitThread(m_oThreadStruct* pThread, m_oLogOutPutStruct* pLogOutPut, m_oConstVarStruct* pConstVar)
{
	if (pThread == NULL)
	{
		return false;
	}
	pThread->m_bClose = false;
	pThread->m_bWork = false;
	pThread->m_dwThreadID = 0;
	pThread->m_pLogOutPut = pLogOutPut;
	pThread->m_pConstVar = pConstVar;
	pThread->m_hThread = INVALID_HANDLE_VALUE;
	pThread->m_hThreadClose = INVALID_HANDLE_VALUE;
	// 创建事件对象
	pThread->m_hThreadClose = CreateEvent(false, false, NULL, NULL);
	if (pThread->m_hThreadClose == NULL)
	{
		AddMsgToLogOutPutList(pLogOutPut, "OnInitThread", 
			"pThread->m_hThreadClose", ErrorType, IDS_ERR_CREATE_EVENT);
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddMsgToLogOutPutList(pThread->m_pLogOutPut, "OnInitThread", 
			"pThread->m_hThreadClose", ErrorType, ERROR_ALREADY_EXISTS);
		return false;
	}
	return true;
}
// 关闭线程函数的句柄
void OnCloseThreadHandle(m_oThreadStruct* pThread)
{
	if (pThread == NULL)
	{
		return;
	}
	if (FALSE == CloseHandle(pThread->m_hThread))
	{
		AddMsgToLogOutPutList(pThread->m_pLogOutPut, "OnCloseThread", 
			"pThread->m_hThread", ErrorType, IDS_ERR_CLOSEHANDLE_FAILED);
	}
	if (FALSE == CloseHandle(pThread->m_hThreadClose))
	{
		AddMsgToLogOutPutList(pThread->m_pLogOutPut, "OnCloseThread", 
			"pThread->m_hThreadClose", ErrorType, IDS_ERR_CLOSEHANDLE_FAILED);
	}
}
// 关闭线程函数
bool OnCloseThread(m_oThreadStruct* pThread)
{
	if (pThread == NULL)
	{
		return false;
	}
	int iResult = WaitForSingleObject(pThread->m_hThreadClose, 
		pThread->m_pConstVar->m_iCloseThreadSleepTimes 
		* pThread->m_pConstVar->m_iOneSleepTime);
	if (iResult != WAIT_OBJECT_0)
	{
		TerminateThread(pThread->m_hThread, 0);
		OnCloseThreadHandle(pThread);
		return false;
	}
	else
	{
		OnCloseThreadHandle(pThread);
		return true;
	}
}