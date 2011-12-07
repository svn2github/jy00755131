// ThreadProcDelete.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcDelete.h"


// CThreadProcDelete

IMPLEMENT_DYNCREATE(CThreadProcDelete, CWinThread)

CThreadProcDelete::CThreadProcDelete()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	// 是否空闲状态
	m_bFree = true;
}

CThreadProcDelete::~CThreadProcDelete()
{
}

BOOL CThreadProcDelete::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcDelete::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcDelete, CWinThread)
END_MESSAGE_MAP()

// CThreadProcDelete 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcDelete::Run()
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
		// 处理删除操作
		ProcDelete();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			// 跳出循环
			break;
		}
		// 设置删除线程空闲
		m_bFree = true;
		// 休眠线程
		this->SuspendThread();
		// 设置删除线程忙
		m_bFree = false;
	}
	// 删除仪器或路由处理线程标志为真
	m_pSiteData->m_bProcDeleteClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcDelete::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	// 是否空闲状态
	m_bFree = true;
	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcDelete::OnClose()
{
	// 是否关闭线程
	m_bClose = true;
	// 是否空闲状态
	if(true == m_bFree)
	{
		// 启动时延处理
		ResumeProcDelete();
	}
	return true;
}

/**
* 线程开始工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcDelete::OnWork()
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
bool CThreadProcDelete::OnStop()
{
	// 是否工作状态
	m_bWork = false;
	return true;
}

/**
* 判断线程是否可以进行处理工作的条件
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcDelete::JudgeProcCondition()
{	
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcDeleteStop = true;	// 删除仪器或路由处理线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcDeleteStop = false;	// 删除仪器或路由处理线程工作
	}
	return true;
}

/**
* 判断线程是否空闲
* @param void
* @return bool true：是；false：否
*/
bool CThreadProcDelete::IsThreadFree()
{
	return m_bFree;
}

/**
* 唤醒线程
* @param void
* @return void
*/
void CThreadProcDelete::ResumeProcDelete()
{
	DWORD dwData;	
	while(true)
	{
		dwData = ResumeThread();	// 唤醒线程
		if(1 == dwData)
		{
			break;
		}
		Sleep(50);
	}	
}

/**
* 锁定线程，不允许其他线程使用
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcDelete::Lock()
{
	bool bLock = false;	
	m_pSiteData->m_oCriticalSectionProcDelete.Lock();	// 为删除仪器或路由处理线程准备的线程同步对象锁定
		if(true == m_bFree)	// 删除线程空闲	
		{		
			m_bFree = false;	// 设置删除线程忙
			bLock = true;
		}		
	m_pSiteData->m_oCriticalSectionProcDelete.Unlock();	// 为删除仪器或路由处理线程准备的线程同步对象解锁
	return bLock;
}

/**
* 解锁线程，允许其他线程使用
* @param void
* @return bool true：成功；false：失败
*/
void CThreadProcDelete::Unlock()
{
	m_pSiteData->m_oCriticalSectionProcDelete.Lock();	// 为删除仪器或路由处理线程准备的线程同步对象锁定		
		m_bFree = true;	// 设置删除线程空闲
	m_pSiteData->m_oCriticalSectionProcDelete.Unlock();	// 为删除仪器或路由处理线程准备的线程同步对象解锁
}

/**
* 增加一个需要删除的仪器；加入等待删除的仪器队列
* @param CInstrument* pInstrument 需要删除的仪器指针
* @return void
*/
void CThreadProcDelete::AddInstrumentForDelete(CInstrument* pInstrument)
{
	// 等待删除的仪器队列
	m_olsInstrument.AddTail(pInstrument);
}

/**
* 增加一个需要删除的路由；加入等待删除的路由队列
* @param CRout* pRout 需要删除的路由指针
* @return void
*/
void CThreadProcDelete::AddRoutForDelete(CRout* pRout)
{
	// 等待删除的路由队列
	m_olsRout.AddTail(pRout);
}

/**
* 处理删除操作
* @param void
* @return void
*/
void CThreadProcDelete::ProcDelete()
{
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		// 返回
		return;
	}
	// 删除仪器或路由处理线程正在运行标志为真
	m_pSiteData->m_bProcDeleteRunning = true;
		// 判断等待其他线程暂停是否成功
		if(true == WaitOtherThreadProcPause())
		{
			// 处理删除仪器操作
			ProcDeleteInstrument();
			// 处理删除路由操作
			ProcDeleteRout();
		}
		// 等待超时
		else
		{
			// 清空等待删除的仪器队列
			m_olsInstrument.RemoveAll();
			// 清空等待删除的路由队列
			m_olsRout.RemoveAll();
		}
	// 删除仪器或路由处理线程停止运行标志为假
	m_pSiteData->m_bProcDeleteRunning = false;
	// 更新上次现场数据变化时刻
	m_pSiteData->UpdateSiteDataChangedTime();

	// 设置运行状态数据日志数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcDelete", "ProcDelete", "Delete Operation");
}

/**
* 等待其他线程暂停
* @note 删除线程工作时，需要暂停的线程：首包处理线程、尾包处理线程、路由监视线程、仪器IP地址分配线程、仪器IP地址设置线程、现场数据输出线程、时延处理线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcDelete::WaitOtherThreadProcPause()
{
	// 等待次数
	int iWaitCount = 0;
	while(true)
	{	
		if((true == m_pSiteData->m_bProcHeadFramePause)	// 首包处理线程暂停
			&& (true == m_pSiteData->m_bProcTailFramePause)	// 尾包处理线程暂停
			&& (true == m_pSiteData->m_bProcMonitorRoutPause)	// 路由监视线程暂停
			&& (true == m_pSiteData->m_bProcIPDistributePause)	// 仪器IP地址分配线程暂停
			&& (true == m_pSiteData->m_bProcIPSetPause)	// 仪器IP地址设置线程暂停
			&& (true == m_pSiteData->m_bProcSiteDataOutputPause))	// 现场数据输出线程暂停
		{
			//取消 时延处理线程对象空闲 2011.11.24 by zl
			//break;
			// 时延处理线程对象空闲
			if(true == m_pThreadProcTimeDelay->IsThreadFree())
			{
				break;
			}			
		}
		// 等待次数
		iWaitCount++;
		// 休眠
		Sleep(50);
		// 等待次数
		if(40 == iWaitCount)
		{
			return false;
		}
	}
	return true;
}

/**
* 处理删除仪器操作
* @param void
* @return void
*/
void CThreadProcDelete::ProcDeleteInstrument()
{
	// 等待删除的仪器指针
	CInstrument* pInstrument = NULL;
	// 等待删除的仪器队列不为空
	while(FALSE == m_olsInstrument.IsEmpty())
	{
		// 得到仪器
		pInstrument = m_olsInstrument.RemoveHead();
		if(NULL != pInstrument)
		{
		CString strDesc;
		strDesc.Format("DeleteInstrumentLink Sn=%x", pInstrument->m_uiSN);
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcDelete", "ProcDeleteInstrument ", strDesc);
		//特殊处理电源站 2011.11.24 by zl
		//if(pInstrument->m_uiInstrumentType==2)
		//	return;
		TRACE1("删除仪器Sn=%x \r\n",pInstrument->m_uiSN); 
			// 删除仪器串
			m_pSiteData->DeleteInstrumentLink(pInstrument, pInstrument->m_uiDeleteDirection);
			// 仪器被删除时的删除方向 1-上；2-下；3-左；4右
			pInstrument->m_uiDeleteDirection = 0;
			pInstrument = NULL;
		}
	}
}

/**
* 处理删除路由操作
* @param void
* @return void
*/
void CThreadProcDelete::ProcDeleteRout()
{
	// 等待删除的路由指针
	CRout* pRout = NULL;
	// 等待删除的路由队列不为空
	while(FALSE == m_olsRout.IsEmpty())
	{
		// 得到路由对象
		pRout = m_olsRout.RemoveHead();
		// 路由对象不为空
		if(NULL != pRout)
		{
			// 删除路由
			m_pSiteData->DeleteRout(pRout);
			pRout = NULL;
		}
	}
}