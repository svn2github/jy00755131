// ThreadProcHeartBeat.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "ThreadProcHeartBeat.h"
#include "Parameter.h"

// CThreadProcHeartBeat

IMPLEMENT_DYNCREATE(CThreadProcHeartBeat, CWinThread)

CThreadProcHeartBeat::CThreadProcHeartBeat()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcHeartBeat::~CThreadProcHeartBeat()
{
}

BOOL CThreadProcHeartBeat::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcHeartBeat::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcHeartBeat, CWinThread)
END_MESSAGE_MAP()

// CThreadProcHeartBeat 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcHeartBeat::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			// 跳出循环
			break;
		}
		// 发送心跳帧
		SendHeartBeatFrame();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			// 跳出循环
			break;
		}
		// 休眠250ms
		Wait(HertBeatSleepTimes);
	}
	// 心跳处理线程关闭标志为真
	m_pSiteData->m_bProcHeartBeatClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcHeartBeat::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcHeartBeat::OnClose()
{
	// 是否关闭线程
	m_bClose = true;
	return true;
}

/**
* 线程开始工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcHeartBeat::OnWork()
{
	// 是否工作状态
	m_bWork = true;
	return true;
}

/**
* 线程停止工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcHeartBeat::OnStop()
{
	// 是否工作状态
	m_bWork = false;
	return true;
}

/**
* 线程等待
* @param int iWaitStopCount 最多等待次数；每次50毫秒
* @return void
*/
void CThreadProcHeartBeat::Wait(int iWaitStopCount)
{
	// 初始化等待次数为0
	int iWaitCount = 0;
	// 循环
	while(true)
	{	// 休眠50毫秒
		Sleep(OneSleepTime);
		// 等待次数加1
		iWaitCount++;
		// 判断是否可以处理的条件
		if(false == JudgeProcCondition())
		{
			// 返回
			return;
		}
		// 判断等待次数是否大于等于最多等待次数
		if(iWaitStopCount <= iWaitCount)
		{
			// 返回
			return;
		}		
	}
}

/**
* 判断是否可以处理的条件
* @param void
* @return bool true：继续处理；false：停止处理
*/
bool CThreadProcHeartBeat::JudgeProcCondition()
{	
	if(false == m_bWork)	// 非工作状态
	{		
		m_pSiteData->m_bProcHeartBeatStop = true;	// 心跳处理线程停止
		return false;
	}
	else	// 工作状态
	{
		m_pSiteData->m_bProcHeartBeatStop = false;	// 心跳处理线程工作
		return true;
	}
}

/**
* 发送心跳帧
* @param void
* @return void
*/
void CThreadProcHeartBeat::SendHeartBeatFrame()
{
	if(false == m_bWork)	// 是否工作状态
	{
		return;
	}
	// 发送心跳帧
	m_pSocketHeartBeatFrame->SendIHeartBeatFrame();
}