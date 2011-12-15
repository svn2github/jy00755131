// ThreadProcIPSet.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "ThreadProcIPSet.h"
#include "..\\parameter\\Parameter.h"
#include <hash_map>
using stdext::hash_map;
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
		// 休眠100ms
		Wait(IPSetFrameSleepTimes);
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
		Sleep(OneSleepTime);	// 休眠
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
// 	// 删除仪器或路由处理线程正在运行
// 	if(true == m_pSiteData->m_bProcDeleteRunning)
// 	{
// 		m_pSiteData->m_bProcIPSetPause = true;	// 仪器IP地址设置线程暂停
// 		return false;
// 	}
// 	else
// 	{
// 		m_pSiteData->m_bProcIPSetPause = false;	// 仪器IP地址设置线程暂停
// 	}
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

// /**
// * 增加一个需要设置IP地址的仪器，保存在仪器队列中
// * @param CInstrument* pInstrument 仪器指针
// * @return void
// */
// void CThreadProcIPSet::AddInstrumentForIPSet(CInstrument* pInstrument)
// {
// 	// 加入队列
// 	if(TRUE == m_oSNInstrumentMap.Lookup(pInstrument->m_uiSN,pInstrument)&& pInstrument->m_uiInstrumentType!=1)
// 		pInstrument->m_uiSN=pInstrument->m_uiSN;
// 	// 第几次设置IP地址
// 	pInstrument->m_iIPSetCount = 0;
// 	// IP地址设置是否成功
// 	pInstrument->m_bIPSetOK = false;
// 	m_olsInstrument.AddTail(pInstrument);
// 	//TRACE2("AddTail SN号=%d IP=%d \r\n", pInstrument->m_uiSN,pInstrument->m_uiIP);
// }

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
	// 得到仪器IP设置应答帧
	GetIPSetReturnFrame();
	// 发送仪器IP设置帧
	SendIPSetFrame();
// 	// 线程等待
// 	Wait(4);
}

/**
* 发送仪器IP设置帧
* @param void
* @return void
*/
void CThreadProcIPSet::SendIPSetFrame()
{
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	// IP地址设置索引不为空
	if (false == m_pSiteData->m_oInstrumentList.m_oIPSetMap.empty())
	{
		// 发送索引表内设备的IP地址设置帧
		for(iter = m_pSiteData->m_oInstrumentList.m_oIPSetMap.begin(); iter != m_pSiteData->m_oInstrumentList.m_oIPSetMap.end();)
		{
			// IP地址设置次数为0
			if (iter->second->m_iIPSetCount == 0)
			{
				// 生成仪器IP地址设置帧数据
				m_pSocketIPSetFrame->MakeFrameData(iter->second, SendSetCmd);
				// 发送仪器IP地址设置帧
				m_pSocketIPSetFrame->SendIPSetFrame();
				// 第几次设置IP地址
				iter->second->m_iIPSetCount++;
				iter++;
			}
			// IP地址设置次数不超过指定次数则重新设置IP地址和查询IP地址
			else if (iter->second->m_iIPSetCount <= IPAddrResetTimes)
			{
				// 生成仪器IP地址查询帧数据
				m_pSocketIPSetFrame->MakeFrameData(iter->second, SendQueryCmd);
				// 发送仪器IP地址设置帧
				m_pSocketIPSetFrame->SendIPSetFrame();
				// 生成仪器IP地址设置帧数据
				m_pSocketIPSetFrame->MakeFrameData(iter->second, SendSetCmd);
				// 发送仪器IP地址设置帧
				m_pSocketIPSetFrame->SendIPSetFrame();
				// 第几次设置IP地址
				iter->second->m_iIPSetCount++;
				iter++;
			}
			// IP地址设置次数超过指定次数则从索引表中删除该仪器指针
			else
			{
				iter->second->m_iIPSetCount = 0;
				m_pSiteData->m_oInstrumentList.m_oIPSetMap.erase(iter++);
			}
		}
		// 更新上次现场数据变化时刻
		m_pSiteData->UpdateSiteDataChangedTime();
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
	unsigned int uiIPInstrument;// 仪器IP地址
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
			// 得到仪器IP
			uiIPInstrument = m_pSocketIPSetFrame->m_oFrameIPSet.m_uiIPInstrument;
			// 仪器在索引表中
			if (TRUE == m_pSiteData->m_oInstrumentList.IfIndexExistInIPSetMap(uiIPInstrument))
			{
				m_pSiteData->m_oInstrumentList.GetInstrumentFromIPSetMap(uiIPInstrument, pInstrument);
				// 从IP地址设置索引表中删除仪器
				m_pSiteData->m_oInstrumentList.DeleteInstrumentFromIPSetMap(uiIPInstrument);
				// 将仪器加入IP地址索引表
				pInstrument->m_bIPSetOK = true;
				m_pSiteData->m_oInstrumentList.AddInstrumentToIPSetMap(uiIPInstrument, pInstrument);
				//更新现场数据变化
 				m_pSiteData->UpdateSiteDataChangedTime();
			}
		}
	}
}
