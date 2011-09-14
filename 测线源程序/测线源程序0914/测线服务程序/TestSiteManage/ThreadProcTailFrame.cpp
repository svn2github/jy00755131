// ThreadProcTailFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcTailFrame.h"


// CThreadProcTailFrame

IMPLEMENT_DYNCREATE(CThreadProcTailFrame, CWinThread)

CThreadProcTailFrame::CThreadProcTailFrame()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcTailFrame::~CThreadProcTailFrame()
{
}

BOOL CThreadProcTailFrame::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcTailFrame::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcTailFrame, CWinThread)
END_MESSAGE_MAP()

// CThreadProcTailFrame 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcTailFrame::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 处理尾包帧
		ProcTailFrame();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 休眠50毫秒
		Wait(1);
	}
	// 设置尾包处理线程关闭标志为真
	m_pSiteData->m_bProcTailFrameClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTailFrame::OnInit()
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
bool CThreadProcTailFrame::OnClose()
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
bool CThreadProcTailFrame::OnWork()
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
bool CThreadProcTailFrame::OnStop()
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
void CThreadProcTailFrame::Wait(int iWaitStopCount)
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
		if(iWaitStopCount <= iWaitCount)
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
bool CThreadProcTailFrame::JudgeProcCondition()
{	
	// 删除仪器或路由处理线程正在运行，或现场数据输出线程正在运行
	if((true == m_pSiteData->m_bProcDeleteRunning) || (true == m_pSiteData->m_bProcSiteDataOutputRunning))
	{		
		m_pSiteData->m_bProcTailFramePause = true;	// 尾包处理线程暂停
		return false;
	}
	else
	{		
		m_pSiteData->m_bProcTailFramePause = false;	// 尾包处理线程工作
	}
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcTailFrameStop = true;	// 尾包处理线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcTailFrameStop = false;	// 尾包处理线程工作
	}
	return true;
}

/**
* 处理尾包帧
* @note 由网络端口得到野外仪器发来的尾包帧，逐帧处理。
* @note 非处理状态时，读取尾包帧，然后丢弃不处理。
* @note 非处理状态发生在：现场处理停止时；删除仪器或路由处理线程正在运行时；现场数据输出线程正在运行时。
* @param void
* @return void
*/
void CThreadProcTailFrame::ProcTailFrame()
{
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到网络端口尾包帧数量
	iFrameCount = m_pSocketTailFrame->GetFrameCount();
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			// 得到帧数据
			m_pSocketTailFrame->GetFrameData();
			// 判断是否可以处理的条件
			if(true ==JudgeProcCondition())
			{
				// 处理单个尾包帧
				ProcTailFrameOne(&m_pSocketTailFrame->m_oFrameTail);
			}
		}		
	}
}

/**
* 处理单个尾包帧
* @param CFrameTail* pFrameTail 尾包帧指针
* @return void
*/
void CThreadProcTailFrame::ProcTailFrameOne(CFrameTail* pFrameTail)
{
	// 设置仪器指针为空
	CInstrument* pInstrument = NULL;
	// 判断：根据仪器SN找到仪器对象
	if(TRUE == m_pSiteData->m_oSNInstrumentMap.GetInstrument(pFrameTail->m_uiSN, pInstrument))
	{
		// 仪器尾包计数加1
		pInstrument->m_uiTailFrameCount++;
		// 判断：仪器尾包计数小于3
		if(pInstrument->m_uiTailFrameCount < 3 )
		{
			return;
		}
	}
	else
	{
		return;
	}
	pFrameTail->m_uiRoutIP=0x0f & pFrameTail->m_uiRoutIP;		// add test by zl 04.12
	// 判断：主交叉站发出的尾包
	if((0 == pFrameTail->m_uiRoutIP) && (1 == pFrameTail->m_uiInstrumentType))
	{
		// 更新野外设备系统时间		//因取m_pSiteData->m_uiSystemTimeSite错误取消，换到采集站读取 04.12
//		unsigned int uiSystemTimeSite = (unsigned int)(((double)pFrameTail->m_uiTimeSystem / 4096.0) * 1000.0);
/*		m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
			//m_pSiteData->m_uiSystemTimeSite = uiSystemTimeSite;
			m_pSiteData->m_uiSystemTimeSite =pFrameTail->m_uiTimeSystem;
		m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
*///		TRACE1("系统时间%d\r\n", m_pSiteData->m_uiSystemTimeSite);
		return;
	}

	// 判断：交叉站发出的尾包
	if((0 != pFrameTail->m_uiRoutIP) && (1 == pFrameTail->m_uiInstrumentType))
	{
		// 设置路由指针为空
		CRout* pRout = NULL;
		// 判断：根据路由IP地址找到路由对象
		if(TRUE == m_pSiteData->m_oRoutIPRoutMap.GetRout(pFrameTail->m_uiRoutIP, pRout))
		{
			// 判断：尾包来自交叉线方向
			if((1 == pRout->m_uiRoutDirection) || (2 == pRout->m_uiRoutDirection))
			{
				// 更新路由时间
				pRout->UpdateRoutTime();	
				// 更新时统
				ProcTimeDelay(pRout);
			}
			// 判断：尾包来自大线方向
			else if((3 == pRout->m_uiRoutDirection) || (4 == pRout->m_uiRoutDirection))
			{
				// 设置仪器指针为空
				CInstrument* pInstrument = NULL;
				// 判断：根据仪器SN找到仪器对象
				if(TRUE == m_pSiteData->m_oSNInstrumentMap.GetInstrument(pFrameTail->m_uiSN, pInstrument))
				{
					// 判断：路由方向仪器链有仪器增减
					if((pRout->m_pTail != NULL) && (pRout->m_pTail != pInstrument))
					{
						// 当仪器增减时候清除尾包计数	add by zl 2011.9.5
						CInstrument* pInstrument1 = pRout->m_pHead;
						while(true)
						{
							// 得到下一仪器
							pInstrument1 = m_pThreadProcTimeDelay->GetNextInstrument(pInstrument1);
							if(pInstrument1==NULL)
								break;
							pInstrument1->m_uiTailFrameCount=0;
							// 无下一链接仪器
							if(NULL == pInstrument1)
							{
								break;
							}
						}
						// 判断：路由方向向左，仪器左指针不为空
						if((3 == pRout->m_uiRoutDirection) && (NULL != pInstrument->m_pInstrumentLeft))
						{
							// 判断：删除线程空闲
							if(true == m_pThreadProcDelete->Lock())
							{
								// 删除方向来自右边
								pInstrument->m_pInstrumentLeft->m_uiDeleteDirection = 4;
								// 增加一个需要设置删除的仪器
								m_pThreadProcDelete->AddInstrumentForDelete(pInstrument->m_pInstrumentLeft);
								// 唤醒删除线程
								m_pThreadProcDelete->ResumeProcDelete();
							}
						}
						// 判断：路由方向向右，仪器右指针不为空
						if((4 == pRout->m_uiRoutDirection) && (NULL != pInstrument->m_pInstrumentRight))
						{
							// 判断：删除线程空闲
							if(true == m_pThreadProcDelete->Lock())
							{
								// 删除方向来自左边
								pInstrument->m_pInstrumentRight->m_uiDeleteDirection = 3;
								// 增加一个需要设置删除的仪器
								m_pThreadProcDelete->AddInstrumentForDelete(pInstrument->m_pInstrumentRight);
								// 唤醒删除线程
								m_pThreadProcDelete->ResumeProcDelete();
							}
						}
					}
				}
			}
		}
		// 根据路由IP地址没有找到路由对象
		else
		{	
			CString strDesc;
			strDesc.Format("Can not find Rout In RoutMap. RoutIP=%d", pFrameTail->m_uiRoutIP);
			// 记录错误到日志文件
			m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcTailFrame", "ProcTailFrameOne", strDesc);
		}
		return;
	}
	// 判断：采集站或电源站发出的尾包
	if((0 != pFrameTail->m_uiRoutIP) && (1 != pFrameTail->m_uiInstrumentType))
	{
		// 设置路由指针为空
		CRout* pRout = NULL;
		// 判断：根据路由IP地址找到路由对象
		if(TRUE == m_pSiteData->m_oRoutIPRoutMap.GetRout(pFrameTail->m_uiRoutIP, pRout))
		{
			//增加尾包时刻读取
			//unsigned int uiSystemTimeSite = (unsigned int)(((double)pFrameTail->m_uiTimeSystem / 4096.0) * 1000.0);
			m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
				m_pSiteData->m_uiSystemTimeSite =pFrameTail->m_uiTimeSystem;
			m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();

			// 更新路由时间
			pRout->UpdateRoutTime();
			// 更新时统
			ProcTimeDelay(pRout);
			// 设置仪器指针为空
			CInstrument* pInstrument = NULL;
			// 判断：根据仪器SN找到仪器对象
			if(TRUE == m_pSiteData->m_oSNInstrumentMap.GetInstrument(pFrameTail->m_uiSN, pInstrument))
			{
				// 判断：路由方向仪器链有仪器增减
				if((pRout->m_pTail != NULL) && (pRout->m_pTail != pInstrument))
				{
					// 当仪器增减时候清除尾包计数	add by zl 2011.9.5
					CInstrument* pInstrument1 = pRout->m_pHead;
					while(true)
					{
						// 得到下一仪器
						pInstrument1 = m_pThreadProcTimeDelay->GetNextInstrument(pInstrument1);
						if(pInstrument1==NULL)
							break;
						pInstrument1->m_uiTailFrameCount=0;
						// 无下一链接仪器
						if(NULL == pInstrument1)
						{
							break;
						}
					}
					// 判断：路由方向向左，仪器左指针不为空
					if((3 == pRout->m_uiRoutDirection) && (NULL != pInstrument->m_pInstrumentLeft))
					{
						// 判断：删除线程空闲
						if(true == m_pThreadProcDelete->Lock())
						{
							// 删除方向来自右边
							pInstrument->m_pInstrumentLeft->m_uiDeleteDirection = 4;
							// 增加一个需要设置删除的仪器
							m_pThreadProcDelete->AddInstrumentForDelete(pInstrument->m_pInstrumentLeft);
							// 唤醒删除线程
							m_pThreadProcDelete->ResumeProcDelete();
						}
					}
					// 判断：路由方向向右，仪器右指针不为空
					if((4 == pRout->m_uiRoutDirection) && (NULL != pInstrument->m_pInstrumentRight))
					{
						// 判断：删除线程空闲
						if(true == m_pThreadProcDelete->Lock())
						{
							// 删除方向来自左边
							pInstrument->m_pInstrumentRight->m_uiDeleteDirection = 3;
							// 增加一个需要设置删除的仪器
							m_pThreadProcDelete->AddInstrumentForDelete(pInstrument->m_pInstrumentRight);
							// 唤醒删除线程
							m_pThreadProcDelete->ResumeProcDelete();
						}
					}
				}
			}
		}
		// 根据路由IP地址没有找到路由对象
		else
		{	
			CString strDesc;
			strDesc.Format("Can not find Rout In RoutMap. RoutIP=%d", pFrameTail->m_uiRoutIP);
			// 记录错误到日志文件
			m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcTailFrame", "ProcTailFrameOne", strDesc);
		}
	}
}

/**
* 更新路由时统
* @param CRout* pRout 路由指针
* @return void
*/
void CThreadProcTailFrame::ProcTimeDelay(CRout* pRout)
{
	// 时延处理线程空闲
	if(true == m_pThreadProcTimeDelay->IsThreadFree())
	{
		// 判断路由是否是最久没有时延修正的路由
		if(true == m_pSiteData->m_oRoutIPRoutMap.JudgeRoutIsLongestTime(pRout))
		{
			// 更新上次时统处理时刻
			pRout->UpdateDelayProcTime();
			// 时延处理即将工作
			m_pThreadProcTimeDelay->m_bFree = false;
			// 设置路由对象
			m_pThreadProcTimeDelay->SetRout(pRout);
			// 启动时延处理
			m_pThreadProcTimeDelay->ResumeProcTimeDelay();
//			TRACE1("时延处理-仪器SN号：%d\r\n", pRout->m_uiSNTail);
		}
	}
}