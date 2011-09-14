// ThreadProcTimeDelay.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcTimeDelay.h"
#include "Matrixline.h"

// CThreadProcTimeDelay

IMPLEMENT_DYNCREATE(CThreadProcTimeDelay, CWinThread)

CThreadProcTimeDelay::CThreadProcTimeDelay()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	// 是否空闲状态
	m_bFree = true;
}

CThreadProcTimeDelay::~CThreadProcTimeDelay()
{
}

BOOL CThreadProcTimeDelay::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcTimeDelay::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcTimeDelay, CWinThread)
END_MESSAGE_MAP()

// CThreadProcTimeDelay 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcTimeDelay::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 时延修正处理
		ProcTimeDelay();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 是否空闲状态
		m_bFree = true;
		// 休眠线程
		this->SuspendThread();
		// 是否空闲状态
		m_bFree = false;
	}
	// 设置仪器时延处理线程关闭标志为真
	m_pSiteData->m_bProcTimeDelayClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	// 是否空闲状态
	m_bFree = true;
	itmp=0;
	times_Delay=0;
	ifrist=0;
	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::OnClose()
{
	// 是否关闭线程
	m_bClose = true;
	// 是否空闲状态
	if(true == m_bFree)
	{
		// 启动时延处理
		ResumeProcTimeDelay();
	}
	return true;
}

/**
* 线程开始工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::OnWork()
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
bool CThreadProcTimeDelay::OnStop()
{
	// 是否工作状态
	m_bWork = false;
	return true;
}

/**
* 判断是否可以处理的条件
* @param void
* @return bool true：继续处理；false：停止处理
*/
bool CThreadProcTimeDelay::JudgeProcCondition()
{	
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcTimeDelayStop = true;	// 仪器时延处理线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcTimeDelayStop = false;	// 仪器时延处理线程工作
	}
	return true;
}

/**
* 判断线程是否空闲
* @param void
* @return bool true：是；false：否
*/
bool CThreadProcTimeDelay::IsThreadFree()
{
	return m_bFree;
}

/**
* 唤醒线程
* @param void
* @return void
*/
void CThreadProcTimeDelay::ResumeProcTimeDelay()
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
* 设置需要进行时延修正的路由
* @param CRout* pRout 路由指针
* @return void
*/
void CThreadProcTimeDelay::SetRout(CRout* pRout)
{
	m_pRout = pRout;
}

/**
* 时延修正处理
* @param void
* @return void
*/
void CThreadProcTimeDelay::ProcTimeDelay()
{
	// 判断：不能继续处理
	if(false == JudgeProcCondition())
	{
		return;
	}

/*	if(times_Delay>5)
		return;
	times_Delay++;
*/
	// 得到需要时延修正的仪器，加入队列和索引表
	GetInstrumentForTailTimeQuery();
	// 判断：只有一个仪器
	if(1 == m_olsInstrument.GetCount())
	{
		return;
	}
	// 读空尾包时刻查询应答帧网络接收缓冲区
	m_pSocketTailTimeFrame->RemoveFrameDataAll();
	Calc_skip=0;
	// 处理尾包时刻查询
	if(ProcTailTimeQuery()==false){
		CString strDesc;
		strDesc.Format("RoutIP=%d", m_pRout->m_uiRoutIP);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTimeDelay", "ProcTimeDelay 1 find wait fail", strDesc);
		return;
	}
	// 判断：不能继续处理
	if(false == JudgeProcCondition())
	{
		return;
	}
	if(Calc_skip!=0){
		CString strDesc;
		strDesc.Format("RoutIP=%d", m_pRout->m_uiRoutIP);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTimeDelay", "ProcTimeDelay 2 find wait fail", strDesc);
		return;
	}
	//m_pSiteData->Calc_skip=Calc_skip;
	// 计算时延
	//ProcTimeDelayCalculate();
	if(ProcTimeDelayCalculate_sys()==false){
		CString strDesc;
		strDesc.Format("RoutIP=%d", m_pRout->m_uiRoutIP);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTimeDelay", "ProcTimeDelay 3 find wait fail", strDesc);
		return;
		}		
	// 设置时延
	ProcTimeDelaySet();

	CString strDesc;
	strDesc.Format("RoutIP=%d", m_pRout->m_uiRoutIP);
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTimeDelay", "ProcTimeDelay", strDesc);
}

/**
* 得到需要时延修正的仪器，加入队列和索引表
* @param void
* @return void
*/
void CThreadProcTimeDelay::GetInstrumentForTailTimeQuery()
{
	// 清空仪器队列
	m_olsInstrument.RemoveAll();
	// 清空仪器索引表
	m_oInstrumentMap.RemoveAll();

	// 需要进行时延修正的仪器加入仪器队列和索引表
	CInstrument* pInstrument = m_pRout->m_pHead;
	while(true)
	{
		// 第几次尾包时刻查询
		pInstrument->m_iTailTimeQueryCount = 0;
		// 尾包时刻查询是否成功
		pInstrument->m_bTailTimeQueryOK = false;
		// 第几次设置仪器时间
		pInstrument->m_iTimeSetCount = 0;
		// 仪器时间设置是否成功
		pInstrument->m_bTimeSetOK = false;	
		// 加入队列
		m_olsInstrument.AddTail(pInstrument);
		// 加入索引表
		m_oInstrumentMap.SetAt(pInstrument->m_uiIP, pInstrument);
		// 得到下一仪器
		pInstrument = GetNextInstrument(pInstrument);
		// 无下一链接仪器
		if(NULL == pInstrument)
		{
			break;
		}
	}
}

/**
* 得到仪器在路由方向上连接的下一个仪器
* @param CInstrument* pInstrument 仪器指针
* @return CInstrument* 连接的下一个仪器 NULL：无连接仪器
*/
CInstrument* CThreadProcTimeDelay::GetNextInstrument(CInstrument* pInstrument)
{
	CInstrument* pInstrumentNext = NULL;

	// 路由链的最后一个仪器
	if(m_pRout->m_pTail == pInstrument)
	{
		return NULL;
	}
	// 得到路由方向的下一个仪器
	pInstrumentNext = pInstrument->GetNextInstrument(m_pRout->m_uiRoutDirection);
	// 下一个仪器存在
	if(NULL != pInstrumentNext)
	{
		// 下一个仪器没有设置IP地址
		if(0 == pInstrumentNext->m_uiIP)
		{
			pInstrumentNext = NULL;
		}
	}
	return pInstrumentNext;
}

/**
* 向仪器发送尾包时刻查询帧
* @param void
* @return void
*/
void CThreadProcTimeDelay::SendTailTimeFrame()
{	
	CInstrument* pInstrument = NULL;	// 仪器	
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey;	// 索引键
	int count=0;

	CString strDesc;
	strDesc.Format("SendTailTimeFrame"); 
	//m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "SendTailTimeFrame", strDesc);

	// 得到索引表起始仪器位置
	pos = m_oInstrumentMap.GetStartPosition();
	while(NULL != pos)
	{
		pInstrument = NULL;
		// 得到仪器
		m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);
		if(NULL != pInstrument)	// 得到仪器
		{
			if(pInstrument->m_uiInstrumentType==1){	
				// 生成仪器尾包时刻查询帧
				m_pSocketTailTimeFrame->MakeFrameData(pInstrument);
				// 发送仪器尾包时刻查询帧
				m_pSocketTailTimeFrame->SendTailTimeFrame();
				count=count|0x1;
			}
			else{
				if((count&0x2)==0){
					count=count|0x2;
					// 生成仪器尾包时刻查询帧
					m_pSocketTailTimeFrame->MakeFrameData(pInstrument);
					// 发送仪器尾包时刻查询帧
					m_pSocketTailTimeFrame->SendTailTimeFrame();
				}
			}
			if(count==0x3)
				break;
		}
	}

	/*
	// 得到索引表起始仪器位置
	pos = m_oInstrumentMap.GetStartPosition();
	// 当前位置有仪器
	while(NULL != pos)
	{
		pInstrument = NULL;
		// 得到仪器
		m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);
		if(NULL != pInstrument)	// 得到仪器
		{
			// 第几次尾包时刻查询
			pInstrument->m_iTailTimeQueryCount++;
			// 生成仪器尾包时刻查询帧
			m_pSocketTailTimeFrame->MakeFrameData(pInstrument);
			// 发送仪器尾包时刻查询帧
			m_pSocketTailTimeFrame->SendTailTimeFrame();
			//TRACE1("SendTailTimeFrame IP=%d \r\n", pInstrument->m_uiIP);
			/*
			if(pInstrument->m_uiInstrumentType==1 && itmp==0){
				for(int j=0;j<10;j++){
					for(int i=0;i<4000;i++)
						m_pSocketTailTimeFrame->SendTailTimeFrame();
					Sleep(1000);
					}
				itmp=1;
			}
			
			if(pInstrument->m_uiSN==8451 && itmp==0){
				for(int j=0;j<1;j++){
					for(int i=0;i<50;i++)
						m_pSocketTailTimeFrame->SendTailTimeFrame();
					Sleep(1);
					}
				itmp=1;
			}
			Sleep(1);
			*/
/*			Sleep(0);
		}
	}
	*/
}

/**
* 得到仪器尾包时刻查询应答帧
* @param void
* @return void
*/
void CThreadProcTimeDelay::GetTailTimeFrame()
{
	int iFrameCount = 0;
	while(true)	// 读空接收缓冲区
	{
		// 得到网络端口尾包时刻查询应答帧数量
		iFrameCount = m_pSocketTailTimeFrame->GetFrameCount();
		if(0 == iFrameCount)	//无仪器IP设置应答帧
		{
			break;
		}
		if(iFrameCount > 0)	// 有尾包时刻查询应答帧
		{
			for(int i = 0; i < iFrameCount; i++)	// 依次得到尾包时刻查询应答帧
			{
				// 得到帧数据
				m_pSocketTailTimeFrame->GetFrameData();
				// 仪器
				CInstrument* pInstrument = NULL;
				// 根据仪器IP地址，由索引表得到仪器
				if(TRUE == GetInstrumentByIP(m_pSocketTailTimeFrame->m_oFrameTailTime.m_uiIPSource, pInstrument))
				{
					// 设置仪器尾包时刻查询结果
					//SetInstrumentTimeDelayData(pInstrument, &m_pSocketTailTimeFrame->m_oFrameTailTime);
					if(SetInstrumentTimeDelayData1(pInstrument, &m_pSocketTailTimeFrame->m_oFrameTailTime)==false)
						Calc_skip=1;
					//TRACE2("GetTailTimeFrame IP=%d TBH=%d\r\n", pInstrument->m_uiIP,pInstrument->m_uiTimeSystem);
					// 重新设置交叉站尾包接收时刻
					pInstrument->ResetInstrumentCrossTailFrameReceiveTime(m_pRout->m_uiRoutDirection);
					// 重新设置尾包接收、发送时刻，屏蔽高2位
					pInstrument->ResetInstrumentTailFrameTime();
/*
					TRACE1("时延查询应答帧-仪器SN号：%d\r\n", pInstrument->m_uiSN);
					TRACE1("时延查询应答帧-m_usReceiveTime：%d\r\n", pInstrument->m_usReceiveTime);
					TRACE1("时延查询应答帧-m_usSendTime：%d\r\n", pInstrument->m_usSendTime);
					TRACE1("时延查询应答帧-m_usLineLeftReceiveTime：%d\r\n", pInstrument->m_usLineLeftReceiveTime);
					TRACE1("时延查询应答帧-m_usLineRightReceiveTime：%d\r\n", pInstrument->m_usLineRightReceiveTime);
					TRACE1("时延查询应答帧-m_usCrossTopReceiveTime：%d\r\n", pInstrument->m_usCrossTopReceiveTime);
					TRACE1("时延查询应答帧-m_usCrossDownReceiveTime：%d\r\n", pInstrument->m_usCrossDownReceiveTime);
*/
					// 尾包时刻查询是否成功
					pInstrument->m_bTailTimeQueryOK = true;
					//TRACE2("时延查询应答帧-仪器SN号：%d IP=%d\r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
					// 从索引表中清除仪器
					m_oInstrumentMap.RemoveKey(pInstrument->m_uiIP);
				}else
					TRACE1("时延应答帧错误- IP=%d\r\n", m_pSocketTailTimeFrame->m_oFrameTailTime.m_uiIPSource);
			}		
		}
	}
	CString strDesc;
	strDesc.Format("GetTailTimeFrame"); 
	//m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "GetTailTimeFrame", strDesc);
}

/**
* 根据仪器IP地址，由索引表得到一个仪器；以指针引用方式得到仪器
* @param unsigned int uiIP 仪器IP地址
* @param CInstrument* &pInstrument 仪器指针，引用调用
* @return BOOL TRUE：得到仪器成功；FALSE：得到仪器失败
*/
BOOL CThreadProcTimeDelay::GetInstrumentByIP(unsigned int uiIP, CInstrument* &pInstrument)
{
	return m_oInstrumentMap.Lookup(uiIP, pInstrument);
}

/**
* 处理尾包时刻查询
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::ProcTailTimeQuery()
{
	int iTimeDelay = 100;
	// 交叉线方向
	if((1 == m_pRout->m_uiRoutDirection) || (2 == m_pRout->m_uiRoutDirection))
	{
		iTimeDelay = 50;
	}
	// 第1次处理尾包时刻查询
	if(true == ProcTailTimeQueryOnce(iTimeDelay))
	{
		return true;
	}
	iTimeDelay = 151;
	// 交叉线方向
	if((1 == m_pRout->m_uiRoutDirection) || (2 == m_pRout->m_uiRoutDirection))
	{
		iTimeDelay = 25;
	}
	// 第2次处理尾包时刻查询
	if(true == ProcTailTimeQueryOnce(iTimeDelay))
	{
		return true;
	}
	// 第3次处理尾包时刻查询
	if(true == ProcTailTimeQueryOnce(iTimeDelay))
	{
		return true;
	}

	return true;
}

/**
* 处理一次尾包时刻查询
* @param int iTimeDelay 发送查询帧和接收查询帧之间的时间间隔；单位毫秒
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::ProcTailTimeQueryOnce(int iTimeDelay)
{
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return false;
	}
	// 向仪器发送尾包时刻查询帧
	if(iTimeDelay!=151)
		SendTailTimeFrame();
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return false;
	}
	// 休眠
	Sleep(iTimeDelay);
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return false;
	}
	// 得到仪器尾包时刻查询应答帧
	GetTailTimeFrame();
	// 收到所有应答帧
	if(TRUE == m_oInstrumentMap.IsEmpty())
	{
		return true;
	}
	return false;
}

/**
* 计算时延
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::ProcTimeDelayCalculate()
{
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return false;
	}
	// 清空仪器队列
	m_olsInstrument.RemoveAll();
	// 清空仪器索引表
	m_oInstrumentMap.RemoveAll();

	CInstrument* pInstrument = NULL;
	CInstrument* pInstrumentNext = NULL;
	pInstrument = m_pRout->m_pHead;
	while(true)
	{
		// 得到下一仪器
		pInstrumentNext = GetNextInstrument(pInstrument);
		// 判断：当前仪器尾包时刻查询失败，或无下一链接仪器，或下一链接仪器未设置IP地址，或下一链接仪器尾包时刻查询失败
		if((False == pInstrument->m_bTailTimeQueryOK) || (NULL == pInstrumentNext) || (0 == pInstrumentNext->m_uiIP) || (False == pInstrumentNext->m_bTailTimeQueryOK))
		{
			break;
		}
		// 计算时延
		
		if(pInstrument->m_usReceiveTime > pInstrumentNext->m_usSendTime)
		{
			pInstrumentNext->m_uiTimeDelay = pInstrument->m_uiTimeDelay + (pInstrument->m_usReceiveTime - pInstrumentNext->m_usSendTime) / 2;
		}
		else
		{
			pInstrumentNext->m_uiTimeDelay = pInstrument->m_uiTimeDelay + (TLOffset + pInstrument->m_usReceiveTime - pInstrumentNext->m_usSendTime) / 2;
		}
		// 本地时间修正高位
		pInstrumentNext->m_uiTimeHigh = pInstrumentNext->m_uiTimeDelay / 0x10000;
		// 本地时间修正低位
		pInstrumentNext->m_uiTimeLow = pInstrumentNext->m_uiTimeDelay % 0x10000;
/*
		TRACE1("时延设置帧-仪器SN号：%d\r\n", pInstrument->m_uiSN);
		TRACE1("时延设置帧-m_usReceiveTime：%d\r\n", pInstrument->m_usReceiveTime);
		TRACE1("时延设置帧-m_uiTimeDelay：%d\r\n", pInstrument->m_uiTimeDelay);
		TRACE1("时延设置帧-NEXT-仪器SN号：%d\r\n", pInstrumentNext->m_uiSN);
		TRACE1("时延设置帧-NEXT-m_usReceiveTime：%d\r\n", pInstrumentNext->m_usReceiveTime);
		TRACE1("时延设置帧-NEXT-m_usSendTime：%d\r\n", pInstrumentNext->m_usSendTime);
		TRACE1("时延设置帧-NEXT-m_uiTimeDelay：%d\r\n", pInstrumentNext->m_uiTimeDelay);
		TRACE1("时延设置帧-NEXT-m_uiTimeHigh：%d\r\n", pInstrumentNext->m_uiTimeHigh);
		TRACE1("时延设置帧-NEXT-m_uiTimeLow：%d\r\n", pInstrumentNext->m_uiTimeLow);
*/
//		TRACE1("时延设置帧-NEXT-仪器SN号：%d\r\n", pInstrumentNext->m_uiSN);
//		TRACE1("时延设置帧-NEXT-m_uiTimeDelay：%d\r\n", pInstrumentNext->m_uiTimeDelay);
		// 加入队列
		m_olsInstrument.AddTail(pInstrumentNext);
		// 加入索引表
		m_oInstrumentMap.SetAt(pInstrument->m_uiIP, pInstrumentNext);
		// 路由尾仪器
		if(pInstrumentNext == m_pRout->m_pTail)
		{
			break;
		}
		pInstrument = pInstrumentNext;
	}
	return true;
}

/**
* 计算时延
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::ProcTimeDelayCalculate_sys()
{
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return false;
	}
	// 清空仪器队列
	m_olsInstrument.RemoveAll();
	// 清空仪器索引表
	m_oInstrumentMap.RemoveAll();

	CInstrument* pInstrument = NULL;
	CInstrument* pInstrumentNext = NULL;
	pInstrument = m_pRout->m_pHead;
	unsigned int m_uiSystemTimeSite=0;

	//取交叉站的时间
	while(true)
	{	 
		if((False == pInstrument->m_bTailTimeQueryOK) || pInstrument->m_bIPSetOK==false )
		{
			break;
		}
		if(pInstrument->m_uiInstrumentType==3){		// by zl
			if(time_init0==0){
				time_init0=pInstrument->m_uiNetTime;
				m_uiSystemTimeSite=pInstrument->m_uiNetTime;
				break;
				//m_pSiteData->m_uiSystemTimeSite=pInstrument->m_uiNetTime;
			}
			else{
				if(pInstrument->m_uiNetTime==m_uiSystemTimeSite)
					break;
					//return false;
				m_uiSystemTimeSite=pInstrument->m_uiNetTime;
				//m_pSiteData->m_uiSystemTimeSite=pInstrument->m_uiNetTime;
			}
		}
		// 得到下一仪器
		pInstrumentNext = GetNextInstrument(pInstrument);
		if(pInstrumentNext == m_pRout->m_pTail)
		{
			break;
		}
		pInstrument = pInstrumentNext;
	}
	
	pInstrument = NULL;
	pInstrumentNext = NULL;
	pInstrument = m_pRout->m_pHead;
    int iTimeDelayH =0,iTimeDelayHH=0;
	int iTimeDelayL=0;
	int inDelaylast=0,Delayinit0=4112;

	//ifrist=0;
	int iDelayL=0;

	while(true)
	{	 
		if((False == pInstrument->m_bTailTimeQueryOK) || pInstrument->m_bIPSetOK==false )
		{
			break;
		}
		// 得到下一仪器
		pInstrumentNext = GetNextInstrument(pInstrument);
		// 判断：当前仪器尾包时刻查询失败，或无下一链接仪器，或下一链接仪器未设置IP地址，或下一链接仪器尾包时刻查询失败
		if((False == pInstrument->m_bTailTimeQueryOK) || (NULL == pInstrumentNext) || (0 == pInstrumentNext->m_uiIP) || (False == pInstrumentNext->m_bTailTimeQueryOK))
		{
			break;
		}
		// 计算时延

		//if(ifrist<10 && ifrist>5){
		if(ifrist==5){
			// 本地时间修正高位
			//iTimeDelayH =iTimeDelayH+(m_uiSystemTimeSite-pInstrument->m_uiNetTime);
			iTimeDelayH =pInstrument->m_uiTimeSystem-pInstrument->m_uiNetTime;
			iTimeDelayHH=iTimeDelayH;
			m_pSiteData->m_bProcTimeDelayON_stop=true;
			//iTimeDelayH =pInstrument->m_uiTimeSystem;
		}else{
			iTimeDelayH =0;
			//iTimeDelayH =iTimeDelayH+(m_uiSystemTimeSite-pInstrument->m_uiNetTime);
			iTimeDelayHH=pInstrument->m_uiTimeSystem-pInstrument->m_uiNetTime;
			m_pSiteData->m_bProcTimeDelayON_stop=false;
			//ifrist=20;
		}
/*		if(pInstrument->m_uiInstrumentType==1){
			iTimeDelayH=0;
			pInstrument->m_uiTimeHigh =0;
			pInstrument->m_uiTimeLow =0;
			inDelaylast=0;
		}
*/	//	iTimeDelayH =pInstrument->m_uiNetTime-pInstrument->m_uiTimeSystem;
		iTimeDelayH =0;
		int t1=pInstrument->m_usReceiveTime-pInstrumentNext->m_usSendTime;
		if(t1<0)
			t1=t1+TLOffset;
		inDelaylast=t1;
		// 本地时间修正低位
		iTimeDelayL=iTimeDelayL-(inDelaylast-Delayinit0);
		if(iTimeDelayL>=TLOffset)
			iTimeDelayH++;
		//iTimeDelayL=iTimeDelayL%TLOffset;
		pInstrumentNext->m_uiTimeHigh =iTimeDelayH;
		//pInstrumentNext->m_uiTimeLow =iTimeDelayL%0xffff;
		iDelayL = iDelayL + iDelayL/2 +10;
		iDelayL=0;
		pInstrumentNext->m_uiTimeLow =iDelayL %0xffff;

		// 加入队列
		m_olsInstrument.AddTail(pInstrument);
		// 加入索引表
		m_oInstrumentMap.SetAt(pInstrument->m_uiIP, pInstrument);
		//更新本地时间
/*		if(ifrist==1){
			m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
				//m_pSiteData->m_uiSystemTimeSite = (unsigned int)(pInstrumentNext->m_uiTimeSystem/ 4096.0 * 1000.0);
			m_pSiteData->m_uiSystemTimeSite = pInstrumentNext->m_uiTimeSystem;
			m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
		}
*/			if(ifrist==2){
				//TRACE3("m_uiTimeLow=%d ReceiveTime=%d SendTime=%d\r\n", pInstrument->m_uiTimeLow,pInstrument->m_usReceiveTime,pInstrumentNext->m_usSendTime);
				CString strDesc;
				strDesc.Format("Get1 Sn=%x,m_uiSystemTimeSite= %u m_uiTimeSystem= %u m_uiNetTime= %u m_uiTimeDelay= %d m_uiTimeHigh= %d m_uiTimeLow= %d rev= %u send= %u %d",pInstrument->m_uiSN,m_uiSystemTimeSite,pInstrument->m_uiTimeSystem,pInstrument->m_uiNetTime,iTimeDelayHH,pInstrument->m_uiTimeHigh,pInstrument->m_uiTimeLow,pInstrument->m_usReceiveTime,pInstrumentNext->m_usSendTime,inDelaylast-Delayinit0); 
				m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "Calculate1", strDesc);
			}
		pInstrument = pInstrumentNext;
		// 路由尾仪器
		if(pInstrumentNext == m_pRout->m_pTail)
		{
/*			if(ifrist==5){
				// 本地时间修正高位
				//iTimeDelayH =iTimeDelayH+(m_uiSystemTimeSite-pInstrument->m_uiNetTime);
				iTimeDelayH =pInstrument->m_uiNetTime-pInstrument->m_uiTimeSystem;
				m_pSiteData->m_bProcTimeDelayON_stop=true;
				iTimeDelayH =0;
				pInstrument->m_uiTimeHigh =iTimeDelayH;
				iTimeDelayL=0;
				pInstrument->m_uiTimeLow =iTimeDelayL%TLOffset;
			}
*/			break;
		}
	}
		ifrist++;
/*
	if(isum>1)
		inDelay=inDelaylast/isum-Delayinit0;
*/	// 加入队列
	m_olsInstrument.AddTail(pInstrument);
	// 加入索引表
	m_oInstrumentMap.SetAt(pInstrument->m_uiIP, pInstrument);
	CString strDesc;
	strDesc.Format("Get2 Sn=%x,m_uiSystemTimeSite= %u m_uiTimeSystem= %u m_uiNetTime=%u m_uiTimeDelay= %d m_uiTimeHigh= %u m_uiTimeLow= %u rev= %u %u data=%d order=%d",pInstrument->m_uiSN,m_pSiteData->m_uiSystemTimeSite,pInstrument->m_uiTimeSystem,pInstrument->m_uiNetTime,iTimeDelayH,pInstrument->m_uiTimeHigh,(int)pInstrument->m_uiTimeLow,pInstrument->m_usReceiveTime,inDelaylast-Delayinit0,m_pSiteData->NetDataCount,m_pSiteData->NetOrderCount); 
	m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "Calculate1", strDesc);
	return true;
}

/**
* 设置时延
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTimeDelay::ProcTimeDelaySet()
{
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return false;
	}

	CInstrument* pInstrument = NULL;
	while(false == m_olsInstrument.IsEmpty())
	{
		pInstrument = m_olsInstrument.RemoveHead();
		// 第几次设置仪器时间
		pInstrument->m_iTimeSetCount++;
		// 仪器时间设置是否成功
		pInstrument->m_bTimeSetOK = true;	
		m_pSocketTimeSetFrame->MakeFrameData(pInstrument);
		if(m_pSiteData->m_bProcTimeDelayON_stop==true){
			m_pSocketTimeSetFrame->SendTimeSetFrame();
			TRACE3("ProcTimeDelaySet IP=%d TBH=%d  TBL=%d\r\n", pInstrument->m_uiIP,pInstrument->m_uiTimeHigh,pInstrument->m_uiTimeLow);
			//TRACE3("ProcTimeDelaySet IP=%d m_uiTimeSystem=%d  m_uiNetTime=%d\r\n", pInstrument->m_uiIP,pInstrument->m_uiTimeSystem,pInstrument->m_uiNetTime);
		}
		Sleep(1);
	}
	//只做一次时统
	//m_pSiteData->m_bProcTimeDelayON_stop=false;
	return true;
}

/**
* 设置仪器尾包时刻查询结果
* @param CInstrument* pInstrument 仪器指针
* @return CFrameTailTime* pFrameTailTime 尾包指针
* @return void
*/
void CThreadProcTimeDelay::SetInstrumentTimeDelayData(CInstrument* pInstrument, CFrameTailTime* pFrameTailTime)
{
	pInstrument->m_usReceiveTime = pFrameTailTime->m_usReceiveTime;	// 16bits时间，接收时刻低位
	pInstrument->m_usSendTime = pFrameTailTime->m_usSendTime;	// 16bits时间，发送时刻低位

	pInstrument->m_usLineLeftReceiveTime = pFrameTailTime->m_usLineLeftReceiveTime;	// 16bits 测线方向左面尾包接收时刻
	pInstrument->m_usLineRightReceiveTime = pFrameTailTime->m_usLineRightReceiveTime;	// 16bits 测线方向右面尾包接收时刻

	pInstrument->m_usCrossTopReceiveTime = pFrameTailTime->m_usCrossTopReceiveTime;	// 16bits 交叉线方向上面尾包接收时刻
	pInstrument->m_usCrossDownReceiveTime = pFrameTailTime->m_usCrossDownReceiveTime;	// 16bits 交叉线方下面尾包接收时刻

	pInstrument->m_uiTimeSystem = pFrameTailTime->m_uiTimeSystem; //by zl
	pInstrument->m_uiNetTime = pFrameTailTime->m_uiNetTime; //by zl

}

/**
* 设置仪器尾包时刻查询结果
* @param CInstrument* pInstrument 仪器指针
* @return CFrameTailTime* pFrameTailTime 尾包指针
* @return void
*/
bool CThreadProcTimeDelay::SetInstrumentTimeDelayData1(CInstrument* pInstrument, CFrameTailTime* pFrameTailTime)
{
	pInstrument->m_usReceiveTime = pFrameTailTime->m_usReceiveTime%0x4000;	// 16bits时间，接收时刻低位
	pInstrument->m_usSendTime = pFrameTailTime->m_usSendTime%0x4000;	// 16bits时间，发送时刻低位

	pInstrument->m_usLineLeftReceiveTime = pFrameTailTime->m_usLineLeftReceiveTime%0x4000;	// 16bits 测线方向左面尾包接收时刻
	pInstrument->m_usLineRightReceiveTime = pFrameTailTime->m_usLineRightReceiveTime%0x4000;	// 16bits 测线方向右面尾包接收时刻

	pInstrument->m_usCrossTopReceiveTime = pFrameTailTime->m_usCrossTopReceiveTime%0x4000;	// 16bits 交叉线方向上面尾包接收时刻
	pInstrument->m_usCrossDownReceiveTime = pFrameTailTime->m_usCrossDownReceiveTime%0x4000;	// 16bits 交叉线方下面尾包接收时刻

	if(pInstrument->m_uiInstrumentType==1){
		if(pInstrument->m_usLineLeftReceiveTime!=0)
			pInstrument->m_usReceiveTime=pInstrument->m_usLineLeftReceiveTime;
		else
			pInstrument->m_usReceiveTime=pInstrument->m_usLineRightReceiveTime;
	}
	if(pInstrument->m_uiTimeSystem==pFrameTailTime->m_uiTimeSystem){
		pInstrument->m_uiTimeSystem = pFrameTailTime->m_uiTimeSystem; //by zl
		pInstrument->m_uiNetTime = pFrameTailTime->m_uiNetTime; //by zl
		return false;
	}
	pInstrument->m_uiTimeSystem = pFrameTailTime->m_uiTimeSystem; 
	pInstrument->m_uiNetTime = pFrameTailTime->m_uiNetTime; 

	if(m_pSiteData->NetOrderCount==0)
		pInstrument->m_uiNetOrder=pFrameTailTime->m_uiNetOrder;
	int itmp=pFrameTailTime->m_uiNetOrder;
	if(pInstrument->m_uiNetOrder!=pFrameTailTime->m_uiNetOrder)
		m_pSiteData->NetOrderCount+=pFrameTailTime->m_uiNetOrder-pInstrument->m_uiNetOrder;
	pInstrument->m_uiNetOrder = pFrameTailTime->m_uiNetOrder;

	CString strDesc;
/*	if(pInstrument->m_uiNetState!=pFrameTailTime->m_uiNetState){
		strDesc.Format("Sn=%d,lastNetState=%x,NetState=%x,%x NetOrder=%x",pInstrument->m_uiSN,pInstrument->m_uiNetState,pFrameTailTime->m_uiNetState,pInstrument->m_uiNetState-pFrameTailTime->m_uiNetState,pFrameTailTime->m_uiNetOrder); 
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "m_uiNetState", strDesc);
	}
	
	if( pInstrument->m_uiNetState!=pFrameTailTime->m_uiNetState ){
		strDesc.Format("Sn=%d,lastNetState=%x,NetState=%x,%x",pInstrument->m_uiSN,pInstrument->m_uiNetState,pFrameTailTime->m_uiNetState&0xffff,pInstrument->m_uiNetState-pFrameTailTime->m_uiNetState&0xffff); 
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "m_uiNetState", strDesc);
	}
	*/
	if(m_pSiteData->NetDataCount==0)
		pInstrument->m_uiNetState=pFrameTailTime->m_uiNetState;
	if(pInstrument->m_uiNetState!=pFrameTailTime->m_uiNetState)
		m_pSiteData->NetDataCount+=pFrameTailTime->m_uiNetState-pInstrument->m_uiNetState;
	pInstrument->m_uiNetState = pFrameTailTime->m_uiNetState;
/*	CString strDesc;
	strDesc.Format("Sn=%x m_uiSystemTimeSite= %u m_uiTimeSystem= %u m_uiNetTime=%u m_uiTimeHeadFrame= %u m_usRec= %05u m_usSend= %05u LeftRec= %u RightRec= %u CrossTopRec= %u CrossDownRec= %u", pInstrument->m_uiSN,m_pSiteData->m_uiSystemTimeSite,pInstrument->m_uiTimeSystem,pInstrument->m_uiNetTime,pInstrument->m_uiTimeHeadFrame, pInstrument->m_usReceiveTime,pInstrument->m_usSendTime,pInstrument->m_usLineLeftReceiveTime,pInstrument->m_usLineRightReceiveTime,pInstrument->m_usCrossTopReceiveTime,pInstrument->m_usCrossDownReceiveTime);
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTimeDelay", "TimeDelayDat", strDesc);
*/	return true;
}