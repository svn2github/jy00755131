// ThreadProcMonitorRout.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcMonitorRout.h"

// CThreadProcMonitorRout

IMPLEMENT_DYNCREATE(CThreadProcMonitorRout, CWinThread)

CThreadProcMonitorRout::CThreadProcMonitorRout()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcMonitorRout::~CThreadProcMonitorRout()
{
}

BOOL CThreadProcMonitorRout::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcMonitorRout::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcMonitorRout, CWinThread)
END_MESSAGE_MAP()

// CThreadProcMonitorRout 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcMonitorRout::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 路由监视
		ProcMonitorRout();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 休眠1000毫秒
		Wait(20);
	}
	// 路由监视线程关闭标志为真
	m_pSiteData->m_bProcMonitorRoutClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcMonitorRout::OnInit()
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
bool CThreadProcMonitorRout::OnClose()
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
bool CThreadProcMonitorRout::OnWork()
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
bool CThreadProcMonitorRout::OnStop()
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
void CThreadProcMonitorRout::Wait(int iWaitStopCount)
{
	// 等待次数
	int iWaitCount = 0;
	while(true)
	{	
		Sleep(50);	// 休眠
		iWaitCount++;	// 等待次数
		// 判断是否可以处理的条件
		if(false == JudgeProcCondition())
		{
			return;
		}
		// 等待次数
		if(iWaitStopCount == iWaitCount)
		{
			return;
		}		
	}
}

/**
* 判断是否可以处理的条件
* @param void
* @return bool true：继续处理；false：停止处理
*/
bool CThreadProcMonitorRout::JudgeProcCondition()
{	
	// 删除仪器或路由处理线程正在运行，或现场数据输出线程正在运行
	if((true == m_pSiteData->m_bProcDeleteRunning) || (true == m_pSiteData->m_bProcSiteDataOutputRunning))
	{		
		m_pSiteData->m_bProcMonitorRoutPause = true;	// 线程暂停
		return false;
	}
	else
	{		
		m_pSiteData->m_bProcMonitorRoutPause = false;	// 线程运行
	}
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcMonitorRoutStop = true;	// 路由监视线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcMonitorRoutStop = false;	// 路由监视线程工作
	}
	return true;
}

/**
* 路由监视
* @param void
* @return void
*/
void CThreadProcMonitorRout::ProcMonitorRout()
{	
	// 判断：不能继续处理
	if(false == JudgeProcCondition())
	{
		return;
	}
	
	bool bResumeProcDelete = false;	// 是否唤醒删除线程	
	bool bDelete = false;	// 是否发生过删除仪器链或删除路由	
	CRout* pRout = NULL;	// 路由对象指针	
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey;	// 索引键

	// 得到索引表起始路由位置
	pos = m_pSiteData->m_oRoutIPRoutMap.m_oRoutMap.GetStartPosition();
	// 循环：得到路由索引表所有路由
	while(NULL != pos)
	{
		pRout = NULL;
		// 得到路由对象
		m_pSiteData->m_oRoutIPRoutMap.m_oRoutMap.GetNextAssoc(pos, uiKey, pRout);
		// 判断：路由指针不为空
		if(NULL != pRout)
		{
			// 为路由监视线程准备的线程同步对象锁定
			m_pSiteData->m_oCriticalSectionProcMonitorRout.Lock();
				// 删除路由IP地址被禁止的路由对象所链接的仪器
				bDelete = DeleteLinkInstrumentRoutWhenRoutIPDisable(pRout);
				// 判断：发生过删除仪器链或删除路由
				if(true == bDelete)	
				{
					// 设置唤醒删除线程标志为真
					bResumeProcDelete = true;
				}
				// 删除路由IP地址被禁止的路由对象
				bDelete = DeleteRoutWhenRoutIPDisable(pRout);
				// 判断：发生过删除仪器链或删除路由
				if(true == bDelete)
				{
					// 设置唤醒删除线程标志为真
					bResumeProcDelete = true;
				}
			// 为路由监视线程准备的线程同步对象解锁
			m_pSiteData->m_oCriticalSectionProcMonitorRout.Unlock();

			// 删除路由时间过期的路由对象所链接的仪器
			bDelete = DeleteRoutLinkInstrumentWhenRoutTimeExpired(pRout);
			// 判断：发生过删除仪器链或删除路由
			if(true == bDelete)
			{
				// 设置唤醒删除线程标志为真
				bResumeProcDelete = true;
			}
		}
	}
	// 判断：发生过删除仪器链或删除路由
	if(true == bResumeProcDelete)
	{
		// 判断：删除线程空闲
		if(true == m_pThreadProcDelete->Lock())
		{
			// 删除的仪器和路由加入删除线程队列
			AddDeleteInstrumentAndRoutToProcDelete();
			// 唤醒删除线程
			m_pThreadProcDelete->ResumeProcDelete();
		}
		else	// 删除线程忙
		{
			// 清空等待删除的仪器队列
			m_olsInstrument.RemoveAll();
			// 清空等待删除的路由队列
			m_olsRout.RemoveAll();
		}
	}
}

/**
* 删除路由IP地址被禁止的路由对象所链接的仪器
* @param CRout* pRout 路由对象指针
* @return bool true：删除成功；false：没有要删除的仪器
*/
bool CThreadProcMonitorRout::DeleteLinkInstrumentRoutWhenRoutIPDisable(CRout* pRout)
{
	// 是否发生删除路由链接的仪器操作
	bool bDelete = false;

	// 判断路由方向是否链接了仪器
	if(NULL == pRout->m_pTail)	// 路由方向没有链接仪器
	{
		return bDelete;
	}

	// 判断路由方向
	switch(pRout->m_uiRoutDirection)
	{
	case 1:	// 上
		{
			if(0 == pRout->m_pHead->m_uiRoutIPTop)
			{
				// 删除路由链接的仪器
				bDelete = DeleteRoutLinkInstrument(pRout);
			}
			break;
		}
	case 2:	// 下
		{
			if(0 == pRout->m_pHead->m_uiRoutIPDown)
			{
				// 删除路由链接的仪器
				bDelete = DeleteRoutLinkInstrument(pRout);
			}
			break;
		}
	case 3:	// 左
		{
			if(0 == pRout->m_pHead->m_uiRoutIPLeft)
			{
				// 删除路由链接的仪器
				bDelete = DeleteRoutLinkInstrument(pRout);
			}
			break;
		}
	case 4:	// 右
		{
			if(0 == pRout->m_pHead->m_uiRoutIPRight)
			{
				// 删除路由链接的仪器
				bDelete = DeleteRoutLinkInstrument(pRout);
			}
			break;
		}
	}

	return bDelete;
}

/**
* 删除路由IP地址被禁止的路由
* @param CRout* pRout 路由指针
* @return bool true：删除成功；false：没有要删除的路由
*/
bool CThreadProcMonitorRout::DeleteRoutWhenRoutIPDisable(CRout* pRout)
{
	// 是否发生删除路由对象操作
	bool bDelete = false;

	// 判断路由方向
	switch(pRout->m_uiRoutDirection)
	{
	case 1:	// 上
		{
			if(0 == pRout->m_pHead->m_uiRoutIPTop)
			{
				// 增加一个需要设置删除的路由
				m_olsRout.AddTail(pRout);
				bDelete = true;
			}
			break;
		}
	case 2:	// 下
		{
			if(0 == pRout->m_pHead->m_uiRoutIPDown)
			{
				// 增加一个需要设置删除的路由
				m_olsRout.AddTail(pRout);
				bDelete = true;
			}
			break;
		}
	case 3:	// 左
		{
			if(0 == pRout->m_pHead->m_uiRoutIPLeft)
			{
				// 增加一个需要设置删除的路由
				m_olsRout.AddTail(pRout);
				bDelete = true;
			}
			break;
		}
	case 4:	// 右
		{
			if(0 == pRout->m_pHead->m_uiRoutIPRight)
			{
				// 增加一个需要设置删除的路由
				m_olsRout.AddTail(pRout);
				bDelete = true;
			}
			break;
		}
	}
	return bDelete;
}

/**
* 删除路由时间过期的路由对象所链接的仪器
* @param CRout* pRout 路由指针
* @return bool true：删除成功；false：没有要删除的仪器
*/
bool CThreadProcMonitorRout::DeleteRoutLinkInstrumentWhenRoutTimeExpired(CRout* pRout)
{
	// 是否发生删除路由链接的仪器操作
	bool bDelete = false;

	// 判断路由方向是否链接了仪器
	if(NULL == pRout->m_pTail)	// 路由方向没有链接仪器
	{
		return bDelete;
	}

	// 得到当前系统时间
	unsigned int uiTimeNow = GetTickCount();
	// 判断路由对象的路由时间是否过期
	if(7000 < (uiTimeNow - pRout->m_uiRoutTime))	// 过期
	//if(3000 < (uiTimeNow - pRout->m_uiRoutTime))	// 过期
	{
		m_pSiteData->m_oCriticalSectionSiteDataChanged.Lock();
		m_pSiteData->HeadminTime=0xffffffff;
		m_pSiteData->HeadCount=0;
		m_pSiteData->m_oCriticalSectionSiteDataChanged.Unlock();
		// 删除路由链接的仪器
		bDelete = DeleteRoutLinkInstrument(pRout);
	}
	return bDelete;
}

/**
* 删除路由链接的仪器
* @param CRout* pRout 路由指针
* @return bool true：删除成功；false：没有要删除的仪器
*/
bool CThreadProcMonitorRout::DeleteRoutLinkInstrument(CRout* pRout)
{
	bool bDelete = false;

	// 判断路由方向是否链接仪器
	if(NULL == pRout->m_pTail)	// 路由方向没有链接仪器
	{
		return bDelete;
	}

	// 判断路由方向
	switch(pRout->m_uiRoutDirection)
	{
	case 1:	// 上
		{
			// 删除方向
			pRout->m_pHead->m_pCrossTop->m_uiDeleteDirection = 2;
			// 增加一个需要设置删除的仪器
			m_olsInstrument.AddTail(pRout->m_pHead->m_pCrossTop);			
			bDelete = true;
			break;
		}
	case 2:	// 下
		{
			// 删除方向
			pRout->m_pHead->m_pCrossDown->m_uiDeleteDirection = 1;
			// 增加一个需要设置删除的仪器
			m_olsInstrument.AddTail(pRout->m_pHead->m_pCrossDown);
			bDelete = true;
			break;
		}
	case 3:	// 左
		{
			// 删除方向
			pRout->m_pHead->m_pInstrumentLeft->m_uiDeleteDirection = 4;
			// 增加一个需要设置删除的仪器
			m_olsInstrument.AddTail(pRout->m_pHead->m_pInstrumentLeft);
			bDelete = true;
			break;
		}
	case 4:	// 右
		{
			// 删除方向
			pRout->m_pHead->m_pInstrumentRight->m_uiDeleteDirection = 3;
			// 增加一个需要设置删除的仪器
			m_olsInstrument.AddTail(pRout->m_pHead->m_pInstrumentRight);
			bDelete = true;
			break;
		}
	}
	return bDelete;
}

/**
* 删除的仪器和路由加入删除线程队列
* @param void
* @return void
*/
void CThreadProcMonitorRout::AddDeleteInstrumentAndRoutToProcDelete()
{
	// 等待删除的仪器指针
	CInstrument* pInstrument = NULL;
	// 等待删除的仪器队列不为空
	while(FALSE == m_olsInstrument.IsEmpty())
	{
		// 得到仪器
		pInstrument = m_olsInstrument.RemoveHead();
		// 增加一个需要设置删除的仪器
		m_pThreadProcDelete->AddInstrumentForDelete(pInstrument);
	}

	// 等待删除的路由指针
	CRout* pRout = NULL;
	// 等待删除的路由队列不为空
	while(FALSE == m_olsRout.IsEmpty())
	{
		// 得到路由对象
		pRout = m_olsRout.RemoveHead();
		// 增加一个需要设置删除的路由
		m_pThreadProcDelete->AddRoutForDelete(pRout);	
	}
}