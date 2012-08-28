// Thread.cpp : 实现文件
//

#include "stdafx.h"
#include "OccupancyCPU.h"
#include "Thread.h"


// CThread

IMPLEMENT_DYNCREATE(CThread, CWinThread)

CThread::CThread()
{
	m_bClose = false;
}

CThread::~CThread()
{
}

BOOL CThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	InitializeCriticalSection(&m_oSec);
	return TRUE;
}

int CThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThread, CWinThread)
END_MESSAGE_MAP()


// CThread 消息处理程序


int CThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类
	bool bClose = false;
	while(1)
	{
		EnterCriticalSection(&m_oSec);
		bClose = m_bClose;
		LeaveCriticalSection(&m_oSec);
		if (bClose == true)
		{
			break;
		}
	}
	DeleteCriticalSection(&m_oSec);
	return CWinThread::Run();
}
void CThread::Close()
{
	EnterCriticalSection(&m_oSec);
	m_bClose = true;
	LeaveCriticalSection(&m_oSec);
}