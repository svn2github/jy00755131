// ThreadProcIPSet.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcIPSet.h"


// CThreadProcIPSet

IMPLEMENT_DYNCREATE(CThreadProcIPSet, CWinThread)

CThreadProcIPSet::CThreadProcIPSet()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcIPSet::~CThreadProcIPSet()
{
}

BOOL CThreadProcIPSet::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcIPSet::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcIPSet, CWinThread)
END_MESSAGE_MAP()

// CThreadProcIPSet 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcIPSet::Run()
{
	// 循环，直到关闭标志为真	
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		//处理仪器IP地址设置
		ProcIPSet();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 休眠50毫秒
		Wait(1);
	}
	// 仪器IP地址设置线程关闭标志为真
	m_pSiteData->m_bProcIPSetClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcIPSet::OnInit()
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
bool CThreadProcIPSet::OnClose()
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
bool CThreadProcIPSet::OnWork()
{
	// 等待设置IP地址的仪器队列
	m_olsInstrument.RemoveAll();
	// 等待设置IP地址的仪器索引表
	m_oSNInstrumentMap.RemoveAll();
	// 是否工作状态
	m_bWork = true;
	return true;
}

/**
* 线程停止工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcIPSet::OnStop()
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
void CThreadProcIPSet::Wait(int iWaitStopCount)
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
bool CThreadProcIPSet::JudgeProcCondition()
{	
	// 删除仪器或路由处理线程正在运行
	if(true == m_pSiteData->m_bProcDeleteRunning)
	{
		m_pSiteData->m_bProcIPSetPause = true;	// 仪器IP地址设置线程暂停
		return false;
	}
	else
	{
		m_pSiteData->m_bProcIPSetPause = false;	// 仪器IP地址设置线程暂停
	}
	if(false == m_bWork)	// 非工作状态
	{		
		m_pSiteData->m_bProcIPSetStop = true;	// 仪器IP地址设置线程停止
		return false;
	}
	else	// 工作状态
	{
		m_pSiteData->m_bProcIPSetStop = false;	// 仪器IP地址设置线程工作
	}
	return true;
}

/**
* 增加一个需要设置IP地址的仪器，保存在仪器队列中
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CThreadProcIPSet::AddInstrumentForIPSet(CInstrument* pInstrument)
{
	if(pInstrument->m_uiInstrumentType!=1)
		pInstrument->m_bIPSetOK=pInstrument->m_bIPSetOK;
	if(pInstrument->m_bIPSetOK==true && pInstrument->m_uiInstrumentType!=1)
		pInstrument->m_bIPSetOK=pInstrument->m_bIPSetOK;
	// 加入队列
	if(TRUE == m_oSNInstrumentMap.Lookup(pInstrument->m_uiSN,pInstrument)&& pInstrument->m_uiInstrumentType!=1)
		pInstrument->m_uiSN=pInstrument->m_uiSN;
	// 第几次设置IP地址
	pInstrument->m_iIPSetCount = 0;
	// IP地址设置是否成功
	pInstrument->m_bIPSetOK = false;
	m_olsInstrument.AddTail(pInstrument);
	TRACE2("AddTail SN号=%d IP=%d \r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
}

/**
* 处理仪器IP地址设置
* @param void
* @return void
*/
void CThreadProcIPSet::ProcIPSet()
{
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return;
	}
	// 发送仪器IP设置帧
	SendIPSetFrame();
	// 线程等待
	Wait(4);
	// 得到仪器IP设置应答帧
	GetIPSetReturnFrame();
}

/**
* 发送仪器IP设置帧
* @param void
* @return void
*/
void CThreadProcIPSet::SendIPSetFrame()
{
	CInstrument* pInstrument;	// 仪器指针
	unsigned int uiSN;	// 仪器号
	POSITION pos;	// 位置

	if(false == m_olsInstrument.IsEmpty())	// 不为空
	{
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcIPSet", "SendIPSetFrame", "Set Instrument IP");
		// 更新上次现场数据变化时刻
		m_pSiteData->UpdateSiteDataChangedTime();	
	}

	// 循环；得到队列所有仪器
	while(false == m_olsInstrument.IsEmpty())
	{
		// 从队列中移走仪器
		pInstrument = m_olsInstrument.RemoveHead();
		// 仪器加入索引表
		m_oSNInstrumentMap.SetAt(pInstrument->m_uiSN, pInstrument);
	}
	// 为索引表中的每个仪器发送IP设置帧
	pos = m_oSNInstrumentMap.GetStartPosition();
	// 循环
	while(NULL != pos)
	{
		// 依次得到每个需要设置IP地址的仪器
		m_oSNInstrumentMap.GetNextAssoc(pos, uiSN, pInstrument);
		if(false == pInstrument->m_bInUsed)	//清除未链接的仪器
		{
			m_oSNInstrumentMap.RemoveKey(uiSN);
		}
		if(pInstrument->m_bIPSetOK ==true){		//因收到IP应答还会发送IP设置 add by zl 04.20
			pInstrument->m_iIPSetCount=0;
			m_oSNInstrumentMap.RemoveKey(uiSN);
			CString strDesc;
			strDesc.Format("m_bIPSetOK Sn=%d IP=%d", pInstrument->m_uiSN, pInstrument->m_uiIP);
			m_pSiteData->m_oRunTimeDataList.Set(1,"CThreadProcIPSet", "SendIPSetFrame3", strDesc);
			TRACE2("重复IP分配-仪器SN号=%x IP=%d \r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
			continue;
		}
		else if(3 == pInstrument->m_iIPSetCount && pInstrument->m_bIPSetOK ==false)	//清除设置了3次，仍无法设置IP地址的仪器
		{
			CString strDesc;
			strDesc.Format("Instrument IP Set Fail. SN=%d；IP=%d", pInstrument->m_uiSN, pInstrument->m_uiIP);
			// 设置运行状态数据
			m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcIPSet", "SendIPSetFrame1", strDesc);
			m_oSNInstrumentMap.RemoveKey(uiSN);
			TRACE2("IP错误 Instrument IP Set Fail. SN=%d；IP=%d\r\n", pInstrument->m_uiSN, pInstrument->m_uiIP);
		}
		else
		{
			CString strDesc;
			strDesc.Format("Sn=%d IP=%d", pInstrument->m_uiSN, pInstrument->m_uiIP);
			m_pSiteData->m_oRunTimeDataList.Set(1,"CThreadProcIPSet", "SendIPSetFrame2", strDesc);
			TRACE2("IP设置-仪器SN号=%d IP=%d \r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
			if(pInstrument->m_uiSN==0){
				TRACE2("IP设置-仪器SN号=%x IP=%d \r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
				continue;
			}
			if(pInstrument->m_bIPSetOK ==true)
				break;
			// 生成仪器IP地址设置帧数据
			m_pSocketIPSetFrame->MakeFrameData(pInstrument);
			// 发送仪器IP地址设置帧
			m_pSocketIPSetFrame->SendIPSetFrame();
			// 第几次设置IP地址
			pInstrument->m_iIPSetCount++;
			Sleep(1);
		}
	}
}

/**
* 得到仪器IP设置应答帧
* @param void
* @return void
*/
void CThreadProcIPSet::GetIPSetReturnFrame()
{
	if(false == m_bWork)	// 是否工作状态
	{
		return;
	}

	CInstrument* pInstrument;	// 仪器指针
	unsigned int uiSN;	// 仪器号
	int iFrameCount = 0;

	while(true)	// 读空接收缓冲区
	{
		// 得到仪器IP设置应答帧数
		iFrameCount = m_pSocketIPSetFrame->GetFrameCount();
		if(0 == iFrameCount)	//无仪器IP设置应答帧
		{
			break;
		}
		for(int i = 0; i < iFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
		{
			// 得到仪器IP设置应答帧
			m_pSocketIPSetFrame->GetFrameData();
			// 得到仪器SN
			uiSN = m_pSocketIPSetFrame->m_oFrameIPSet.m_uiSN;
			// 仪器在索引表中
			if(TRUE == m_oSNInstrumentMap.Lookup(uiSN, pInstrument))
			{
				// 从索引表中删除仪器
				m_oSNInstrumentMap.RemoveKey(uiSN);
				// 仪器IP地址设置成功
				pInstrument->m_bIPSetOK = true;
				CString strDesc;
				if( pInstrument!=NULL){
					strDesc.Format("GetIPSetReturnFrame. SN=%x IP=%d", pInstrument->m_uiSN,pInstrument->m_uiIP);
					TRACE2("IP应答-仪器SN号=%x IP=%d \r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
					//strDesc.Format("GetIPSetReturnFrame. SN=%x ", uiSN);
					// 设置运行状态数据
					m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcIPSet", "GetIPSetReturnFrame", strDesc);
				}else{
					strDesc.Format("GetIPSetReturnFrame. Fail");
					// 设置运行状态数据
					m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcIPSet", "GetIPSetReturnFrame", strDesc);
				}
			}
		}
	}
}
