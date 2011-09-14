// ThreadProcIPDistribute.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcIPDistribute.h"


// CThreadProcIPDistribute

IMPLEMENT_DYNCREATE(CThreadProcIPDistribute, CWinThread)

CThreadProcIPDistribute::CThreadProcIPDistribute()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcIPDistribute::~CThreadProcIPDistribute()
{
}

BOOL CThreadProcIPDistribute::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcIPDistribute::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcIPDistribute, CWinThread)
END_MESSAGE_MAP()

// CThreadProcIPDistribute 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcIPDistribute::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 处理IP地址分配
		ProcIPDistribute();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			break;
		}
		// 休眠1000毫秒
		Wait(20);
	}
	// 仪器IP地址分配线程关闭标志为真
	m_pSiteData->m_bProcIPDistributeClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcIPDistribute::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	// 仪器IP地址分配时间
	m_iIPDistributeTime = 0;
	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcIPDistribute::OnClose()
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
bool CThreadProcIPDistribute::OnWork()
{
	// 辅助道仪器SN索引表
	m_oInstrumentAuxSNMap.RemoveAll(); 
	// 爆炸机控制器仪器SN索引表
	m_oInstrumentBlastMachineSNMap.RemoveAll(); 
	// 迂回道标记仪器SN索引表
	m_oInstrumentDetourLogoSNMap.RemoveAll(); 
	// 迂回道仪器SN索引表
	m_oInstrumentDetourSNMap.RemoveAll(); 
	// 采集道标记仪器SN索引表
	m_oInstrumentMarkerLogoSNMap.RemoveAll(); 
	// 采集道仪器SN索引表
	m_oInstrumentMarkerSNMap.RemoveAll(); 
	// 是否工作状态
	m_bWork = true;
	return true;
}

/**
* 线程停止工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcIPDistribute::OnStop()
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
void CThreadProcIPDistribute::Wait(int iWaitStopCount)
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
* @note 线程在下列条件下不处理：现场停止；删除仪器或路由处理线程正在运行；现场数据输出线程正在运行；要求仪器IP地址分配线程暂停。
* @param void
* @return bool true：继续处理；false：停止处理
*/
bool CThreadProcIPDistribute::JudgeProcCondition()
{	
	// 删除仪器或路由处理线程正在运行，或现场数据输出线程正在运行，或暂停要求：要求仪器IP地址分配线程暂停
	if((true == m_pSiteData->m_bProcDeleteRunning) || (true == m_pSiteData->m_bProcSiteDataOutputRunning) || (true == m_pSiteData->m_bPauseRequestForProcIPDistribute))
	{		
		m_pSiteData->m_bProcIPDistributePause = true;	// 仪器IP地址分配线程暂停
		return false;
	}
	else
	{		
		m_pSiteData->m_bProcIPDistributePause = false;	// 仪器IP地址分配线程运行
	}
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcIPDistributeStop = true;	// 仪器IP地址分配线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcIPDistributeStop = false;	// 仪器IP地址分配线程工作
	}
	return true;
}

/**
* 处理IP地址分配
* @param void
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistribute()
{	
	// 判断是否可以处理的条件
	if(false == JudgeProcCondition())
	{
		return;
	}
	// 设置仪器IP地址分配时间
	m_iIPDistributeTime = GetTickCount();
	// 为辅助道分配IP地址
	if(false == ProcIPDistributeAux())
	{
		return;
	}
	// 为爆炸机控制器分配IP地址
	if(false == ProcIPDistributeBlastMachine())
	{
		return;
	}
	// 为迂回道分配IP地址
	if(false == ProcIPDistributeDetour())
	{
		return;
	}
	// 按标记点分配IP地址
	ProcIPDistributeMarker();
}

/**
* 处理辅助道IP地址分配
* @param void
* @return bool true：可以继续分配；false：终止分配
*/
bool CThreadProcIPDistribute::ProcIPDistributeAux()
{
	// 得到辅助道队列
	CAuxList* pAuxList = &m_pLogicData->m_oLayoutSetupData.m_oAuxList;
	// 辅助道指针为空
	CAuxData* pAuxData = NULL;
	// 循环；得到所有辅助道
	for(unsigned int i = 0; i < pAuxList->m_uiCountAll; i++)
	{
		// 得到辅助道
		pAuxData = pAuxList->GetAux(i);
		// 处理一个辅助道的IP地址分配
		ProcIPDistributeAuxOne(pAuxData);
	}
	// 清理过期辅助道标记
	return ProcClearTimeOutAux();
}

/**
* 处理一个辅助道的IP地址分配
* @param CAuxData* pAuxData 辅助道指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeAuxOne(CAuxData* pAuxData)
{	
	// 仪器指针为空
	CInstrument* pInstrument = NULL;
	// IP地址
	unsigned int uiIP;

	// 判断辅助道对应仪器是否已链接到现场
	if(TRUE == m_pSiteData->GetInstrumentFromSNMapBySN(pAuxData->m_uiSN, pInstrument))
	{
		// 判断：仪器不是采集站
		if(3 != pInstrument->m_uiInstrumentType)
		{
			return;
		}
		// 得到辅助道IP地址
		uiIP = pAuxData->m_uiIP;

		// 判断：辅助道对应仪器未设置IP地址
		if(0 == pInstrument->m_uiIP)
		{
			// 设置辅助道属性为真
			pInstrument->m_bAux = true;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 设置辅助道标记时间
			pInstrument->m_uiAuxTime = m_iIPDistributeTime;
			// 设置仪器IP地址
			pInstrument->m_uiIP = uiIP;
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);
			// 加入辅助道仪器索引表
			m_oInstrumentAuxSNMap.SetAt(pInstrument->m_uiSN, pInstrument);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
		}
		// 判断：辅助道需要重新设置IP地址
		else if(uiIP != pInstrument->m_uiIP)
		{
			// 判断：仪器为采集站，非本次分配
			if((3 == pInstrument->m_uiInstrumentType) && (m_iIPDistributeTime != pInstrument->m_uiIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
			}
			// 设置辅助道为真
			pInstrument->m_bAux = true;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 设置辅助道标记时间
			pInstrument->m_uiAuxTime = m_iIPDistributeTime;
			// 设置仪器IP地址
			pInstrument->m_uiIP = uiIP;
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);
			// 加入辅助道仪器索引表
			m_oInstrumentAuxSNMap.SetAt(pInstrument->m_uiSN, pInstrument);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
		}
		// 上次分配正确
		else
		{
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 辅助道标记时间
			pInstrument->m_uiAuxTime = m_iIPDistributeTime;
		}
	}
}

/**
* 清理过期辅助道标记
* @param void
* @return bool true：没有要清理的过期标记；false：有要清理的过期标记
*/
bool CThreadProcIPDistribute::ProcClearTimeOutAux()
{
	bool bReturn = true;

	POSITION pos = NULL;	// 位置	
	unsigned int uiSN;	// 仪器设备号
	CInstrument* pInstrument = NULL;	// 仪器
	pos = m_oInstrumentAuxSNMap.GetStartPosition();
	while(NULL != pos)
	{
		m_oInstrumentAuxSNMap.GetNextAssoc(pos, uiSN, pInstrument);
		// 判断：非本次分配
		if(pInstrument->m_uiAuxTime != m_iIPDistributeTime)
		{
			// 判断：仪器IP地址非0；仪器为采集站；非本次分配
			if((0 != pInstrument->m_uiIP) && (3 == pInstrument->m_uiInstrumentType) && (pInstrument->m_uiIPDistributeTime != m_iIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
				// 仪器IP地址
				pInstrument->m_uiIP = 0;
				// 加入仪器IP地址设置队列
				m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
				bReturn = false;
			}
			// 是否辅助道
			pInstrument->m_bAux = false;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = 0;
			// 辅助道标记时间
			pInstrument->m_uiAuxTime = 0;
			m_oInstrumentAuxSNMap.RemoveKey(uiSN);
		}
	}
	return bReturn;
}

/**
* 处理爆炸机控制器IP地址分配
* @param void
* @return bool true：可以继续分配；false：终止分配
*/
bool CThreadProcIPDistribute::ProcIPDistributeBlastMachine()
{
	// 爆炸机控制器队列
	CBlastMachineList* pBlastMachineList = &m_pLogicData->m_oLayoutSetupData.m_oBlastMachineList;
	// 爆炸机控制器
	CBlastMachineData* pBlastMachineData;
	for(unsigned int i = 0; i < pBlastMachineList->m_uiCountAll; i++)
	{
		pBlastMachineData = pBlastMachineList->GetBlastMachine(i);
		ProcIPDistributeBlastMachineOne(pBlastMachineData);
	}
	// 清理过期爆炸机控制器标记
	return ProcClearTimeOutBlastMachine();
}

/**
* 处理一个爆炸机控制器的IP地址分配
* @param CBlastMachineData* pBlastMachineData 爆炸机控制器指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeBlastMachineOne(CBlastMachineData* pBlastMachineData)
{	
	CInstrument* pInstrument = NULL;	// 仪器	
	unsigned int uiIP;	// IP地址

	// 爆炸机控制器已链接到现场
	if(TRUE == m_pSiteData->GetInstrumentFromSNMapBySN(pBlastMachineData->m_uiSN, pInstrument))
	{
		// 判断：仪器不是采集站
		if(3 != pInstrument->m_uiInstrumentType)
		{
			return;
		}
		// 得到爆炸机控制器IP地址
		uiIP = pBlastMachineData->m_uiIP;;

		// 判断：爆炸机控制器未设置IP地址
		if(0 == pInstrument->m_uiIP)
		{
			// 是否爆炸机控制器
			pInstrument->m_bBlastMachine = true;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;	
			// 爆炸机控制器标记时间
			pInstrument->m_uiBlastMachineTime = m_iIPDistributeTime;
			// 仪器IP地址
			pInstrument->m_uiIP = uiIP;
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);
			// 加入爆炸机控制器仪器队列
			m_oInstrumentBlastMachineSNMap.SetAt(pInstrument->m_uiSN, pInstrument);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
		}
		// 判断：爆炸机控制器需要重新设置IP地址
		else if(uiIP != pInstrument->m_uiIP)
		{
			// 判断：仪器为采集站，非本次分配
			if((3 == pInstrument->m_uiInstrumentType) && (m_iIPDistributeTime != pInstrument->m_uiIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
			}
			// 是否爆炸机控制器
			pInstrument->m_bBlastMachine = true;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;	
			// 爆炸机控制器标记时间
			pInstrument->m_uiBlastMachineTime = m_iIPDistributeTime;
			// 仪器IP地址
			pInstrument->m_uiIP = uiIP;
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);
			// 加入爆炸机控制器仪器索引表
			m_oInstrumentBlastMachineSNMap.SetAt(pInstrument->m_uiSN, pInstrument);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
		}
		else	// 上次分配正确
		{
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;	
			// 爆炸机控制器标记时间
			pInstrument->m_uiBlastMachineTime = m_iIPDistributeTime;
		}
	}
}

/**
* 清理过期爆炸机控制器标记
* @param void
* @return bool true：没有要清理的过期标记；false：有要清理的过期标记
*/
bool CThreadProcIPDistribute::ProcClearTimeOutBlastMachine()
{
	bool bReturn = true;

	POSITION pos = NULL;	// 位置	
	unsigned int uiSN;	// 仪器设备号
	CInstrument* pInstrument = NULL;	// 仪器
	pos = m_oInstrumentBlastMachineSNMap.GetStartPosition();
	while(NULL != pos)
	{
		m_oInstrumentBlastMachineSNMap.GetNextAssoc(pos, uiSN, pInstrument);
		if(pInstrument->m_uiBlastMachineTime != m_iIPDistributeTime)
		{
			// 判断：仪器IP地址非0；仪器为采集站；非本次分配
			if((0 != pInstrument->m_uiIP) && (3 == pInstrument->m_uiInstrumentType) && (pInstrument->m_uiIPDistributeTime != m_iIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
				// 仪器IP地址
				pInstrument->m_uiIP = 0;
				// 加入仪器IP地址设置队列
				m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
				bReturn = false;
			}
			// 是否爆炸机控制器
			pInstrument->m_bBlastMachine = false;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = 0;
			// 爆炸机控制器标记时间
			pInstrument->m_uiBlastMachineTime = 0;
			m_oInstrumentBlastMachineSNMap.RemoveKey(uiSN);
		}
	}
	return bReturn;
}

/**
* 处理迂回道IP地址分配
* @param void
* @return bool true：可以继续分配；false：终止分配
*/
bool CThreadProcIPDistribute::ProcIPDistributeDetour()
{
	// 得到迂回道队列
	CDetourList* pDetourList = &m_pLogicData->m_oLayoutSetupData.m_oDetourList;
	// 迂回道指针为空
	CDetourData* pDetourData = NULL;
	// 循环；依次处理所有迂回道
	for(unsigned int i = 0; i < pDetourList->m_uiCountAll; i++)
	{
		// 得到迂回道指针
		pDetourData = pDetourList->GetDetour(i);
		// 处理一个迂回道IP地址分配
		ProcIPDistributeDetourOne(pDetourData);
	}
	// 清理过期迂回道标记
	return ProcClearTimeOutDetour();
}

/**
* 处理一个迂回道IP地址分配
* @param CDetourData* pDetourData 迂回道指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeDetourOne(CDetourData* pDetourData)
{	
	// 设置迂回道低端仪器指针为空
	CInstrument* pInstrumentLow = NULL;
	// 设置迂回道高端仪器指针为空
	CInstrument* pInstrumentHigh= NULL;
	// 设置仪器指针为空
	CInstrument* pInstrument = NULL;

	// 判断：迂回道低端仪器已链接到现场
	if(TRUE == m_pSiteData->GetInstrumentFromSNMapBySN(pDetourData->m_uiSNLow, pInstrumentLow))
	{
		// 判断：上次设置正确
		if((true == pInstrumentLow->m_bDetourMarkerLow) && (pInstrumentLow->m_uiStopMarking == pDetourData->m_uiStopMarking))
		{
			// 迂回道标记时间
			pInstrumentLow->m_uiDetourLogoTime = m_iIPDistributeTime;
		}
		// 设置改变
		else
		{
			// 设置是否迂回道停止标记
			pInstrumentLow->m_uiStopMarking = pDetourData->m_uiStopMarking;
			// 设置是否迂回道低端标记点
			pInstrumentLow->m_bDetourMarkerLow = true;
			// 设置迂回道标记时间
			pInstrumentLow->m_uiDetourLogoTime = m_iIPDistributeTime;
			// 加入迂回道标记仪器索引表
			m_oInstrumentDetourLogoSNMap.SetAt(pInstrumentLow->m_uiSN, pInstrumentLow);
		}
	}
	// 判断迂回道高端仪器已链接到现场
	if(TRUE == m_pSiteData->GetInstrumentFromSNMapBySN(pDetourData->m_uiSNHigh, pInstrumentHigh))
	{
		// 判断上次设置正确
		if((true == pInstrumentHigh->m_bDetourMarkerHigh) && (pInstrumentHigh->m_uiStopMarking == pDetourData->m_uiStopMarking))
		{
			// 设置迂回道标记时间
			pInstrumentHigh->m_uiDetourLogoTime = m_iIPDistributeTime;
		}
		// 设置改变
		else
		{
			// 设置是否迂回道停止标记
			pInstrumentHigh->m_uiStopMarking = pDetourData->m_uiStopMarking;
			// 设置是否迂回道高端标记点
			pInstrumentHigh->m_bDetourMarkerHigh= true;
			// 设置迂回道标记时间
			pInstrumentHigh->m_uiDetourLogoTime = m_iIPDistributeTime;
			// 加入迂回道标记仪器索引表
			m_oInstrumentDetourLogoSNMap.SetAt(pInstrumentHigh->m_uiSN, pInstrumentHigh);
		}
	}
	// 判断迂回道低端仪器已链接到现场,迂回道高端仪器未链接到现场
	if((NULL != pInstrumentLow) && (NULL == pInstrumentHigh))
	{
		// 设置仪器指针指向低端仪器的右链接仪器
		pInstrument = pInstrumentLow->m_pInstrumentRight;
		// 高端方向链接的所有仪器（右方）为迂回道
		// 循环；直到仪器指针为空
		while(true)
		{
			// 判断仪器指针为空
			if(NULL == pInstrument)
			{
				break;
			}
			// 处理一个仪器的迂回道IP地址分配
			ProcIPDistributeDetourOneInstrument(pInstrument);
			// 得到右边链接的仪器
			pInstrument = pInstrument->m_pInstrumentRight;
		}
	}

	// 迂回道低端仪器未链接到现场,迂回道高端仪器已链接到现场
	if((NULL == pInstrumentLow) && (NULL != pInstrumentHigh))
	{
		// 设置仪器指针指向高端仪器的左链接仪器
		pInstrument = pInstrumentHigh->m_pInstrumentLeft;
		// 低端方向链接的所有仪器（左方）为迂回道
		// 循环；直到仪器指针为空
		while(true)
		{
			// 判断仪器指针为空
			if(NULL == pInstrument)
			{
				break;
			}
			// 处理一个仪器的迂回道IP地址分配
			ProcIPDistributeDetourOneInstrument(pInstrument);
			// 得到左边链接的仪器
			pInstrument = pInstrument->m_pInstrumentLeft;
		}
	}

	// 迂回道低端仪器已链接到现场,迂回道高端仪器已链接到现场
	if((NULL != pInstrumentLow) && (NULL != pInstrumentHigh))
	{
		// 设置仪器指针指向低端仪器的右链接仪器
		pInstrument = pInstrumentLow->m_pInstrumentRight;
		// 低端和高端质检链接的所有仪器为迂回道
		// 循环；直到仪器指针为空；或仪器指针指向高端仪器
		while(true)
		{
			// 判断仪器指针为空
			if(NULL == pInstrument)
			{
				break;
			}
			// 判断仪器指针指向高端仪器
			if(pInstrumentHigh == pInstrument)
			{
				break;
			}
			// 处理一个仪器的迂回道IP地址分配
			ProcIPDistributeDetourOneInstrument(pInstrument);
			// 得到右边链接的仪器
			pInstrument = pInstrument->m_pInstrumentRight;
		}
	}
}

/**
* 处理一个仪器的迂回道IP地址分配
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeDetourOneInstrument(CInstrument* pInstrument)
{
	// 判断：仪器未设置IP地址
	if(0 == pInstrument->m_uiIP)
	{
		// 是否迂回道
		pInstrument->m_bDetour = true;
		// 设置仪器IP地址分配时间
		pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;
		// 迂回道标记时间
		pInstrument->m_uiDetourTime = m_iIPDistributeTime;
		// 得到迂回道仪器IP地址
		pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetDetourIP();
		// 加入仪器IP地址索引表
		m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);
		// 加入迂回道仪器索引表
		m_oInstrumentDetourSNMap.SetAt(pInstrument->m_uiSN, pInstrument);
		// 加入仪器IP地址设置队列
		m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
	}
	// 已设置IP地址
	else
	{
		// 判断：上次已设置为迂回道
		if(true == pInstrument->m_bDetour)
		{
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 迂回道标记时间
			pInstrument->m_uiDetourTime = m_iIPDistributeTime;
		}
		// 上次不是设置为迂回道
		else
		{
			// 判断：仪器为采集站；非本次分配
			if((3 == pInstrument->m_uiInstrumentType) && (pInstrument->m_uiIPDistributeTime != m_iIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
			}
			// 是否迂回道
			pInstrument->m_bDetour = true;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 迂回道标记时间
			pInstrument->m_uiDetourTime = m_iIPDistributeTime;
			// 得到迂回道仪器IP地址
			pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetDetourIP();
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);
			// 加入迂回道仪器索引表
			m_oInstrumentDetourSNMap.SetAt(pInstrument->m_uiSN, pInstrument);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
		}
	}
}

/**
* 清理过期迂回道标记
* @param void
* @return bool true：没有要清理的过期标记；false：有要清理的过期标记
*/
bool CThreadProcIPDistribute::ProcClearTimeOutDetour()
{
	bool bReturn = true;

	POSITION pos = NULL;	// 位置	
	unsigned int uiSN;	// 仪器设备号
	CInstrument* pInstrument = NULL;	// 仪器

	pos = m_oInstrumentDetourLogoSNMap.GetStartPosition();
	while(NULL != pos)
	{
		m_oInstrumentDetourLogoSNMap.GetNextAssoc(pos, uiSN, pInstrument);
		if(pInstrument->m_uiDetourLogoTime != m_iIPDistributeTime)
		{
			// 是否迂回道低端标记点
			pInstrument->m_bDetourMarkerLow = false;
			// 是否迂回道高端标记点
			pInstrument->m_bDetourMarkerHigh= false;
			// 是否迂回道停止标记
			pInstrument->m_uiStopMarking = 0;
			// 迂回道标记时间
			pInstrument->m_uiDetourLogoTime = 0;
			m_oInstrumentDetourLogoSNMap.RemoveKey(uiSN);
		}
	}

	pos = m_oInstrumentDetourSNMap.GetStartPosition();
	while(NULL != pos)
	{
		m_oInstrumentDetourSNMap.GetNextAssoc(pos, uiSN, pInstrument);
		if(pInstrument->m_uiDetourTime != m_iIPDistributeTime)
		{
			// 判断：仪器IP地址非0；仪器为采集站；非本次分配
			if((0 != pInstrument->m_uiIP) && (3 == pInstrument->m_uiInstrumentType) && (pInstrument->m_uiIPDistributeTime != m_iIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
				// 仪器IP地址
				pInstrument->m_uiIP = 0;
				// 加入仪器IP地址设置队列
				m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
				bReturn = false;
			}
			// 是否迂回道
			pInstrument->m_bDetour = false;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = 0;
			// 迂回道标记时间
			pInstrument->m_uiDetourTime = 0;
			m_oInstrumentDetourSNMap.RemoveKey(uiSN);
		}
	}
	return bReturn;
}

/**
* 处理标记点IP地址分配
* @param void
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarker()
{
	// 得到标记点队列
	CMarkerList* pMarkerList = &m_pLogicData->m_oLayoutSetupData.m_oMarkerList;
	// 设置标记点为空
	CMarkerData* pMarkerData = NULL;
	// 循环；依次处理每个标记点
	for(unsigned int i = 0; i < pMarkerList->m_uiCountAll; i++)
	{
		// 得到标记点
		pMarkerData = pMarkerList->GetMarker(i);
		// 处理单个标记点IP地址分配
		ProcIPDistributeMarkerOne(pMarkerData);
	}
	// 清理过期采集道标记
	ProcClearTimeOutMarker();
}

/**
* 处理单个标记点IP地址分配
* @param CMarkerData* pMarkerData 标记点指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerOne(CMarkerData* pMarkerData)
{
	// 判断标记点是否是正常标记顺序
	if(0 == pMarkerData->m_uiReversed)
	{
		// 处理单个标记点IP地址分配，正常顺序
		ProcIPDistributeMarkerOneNormal(pMarkerData);
	}
	// 判断标记点是否是翻转标记顺序
	else if(1 == pMarkerData->m_uiReversed)
	{
		// 处理单个标记点IP地址分配，翻转顺序
		ProcIPDistributeMarkerOneReversed(pMarkerData);
	}
}

/**
* 处理单个标记点IP地址分配，正常顺序
* @param CMarkerData* pMarkerData 标记点指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerOneNormal(CMarkerData* pMarkerData)
{
	// 设置标记点仪器指针为空
	CInstrument* pInstrumentMarker = NULL;
	// 设置标记点测道指针为空
	CChannelSetupData* pChannelMarker = NULL;
	// 判断标记点未链接到现场
	if(TRUE != m_pSiteData->GetInstrumentFromSNMapBySN(pMarkerData->m_uiSN, pInstrumentMarker))
	{
		return;
	}
	// 判断：上次已设置为标记点
	if(true == pInstrumentMarker->m_bMarker)
	{
		// 标记点标记时间
		pInstrumentMarker->m_uiMarkerLogoTime = m_iIPDistributeTime;
	}
	// 判断：上次不是设置为标记点
	else
	{
		// 是否标记点
		pInstrumentMarker->m_bMarker = true;
		// 标记点标记时间
		pInstrumentMarker->m_uiMarkerLogoTime = m_iIPDistributeTime;
		// 加入采集道标记仪器索引表
		m_oInstrumentMarkerLogoSNMap.SetAt(pInstrumentMarker->m_uiSN, pInstrumentMarker);
	}

	// 设置采集站指针为空
	CInstrument* pInstrumentCollect = NULL;
	// 设置测道指针为空
	CChannelSetupData* pChannel = NULL;
	// 向左得到采集站
	pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentMarker, 3);
	// 判断采集站指针不为空
	if(NULL != pInstrumentCollect)
	{
		// 得到测道
		pChannelMarker = m_pLogicData->m_oSurveySetupData.GetChannel(pMarkerData->m_uiLineName, pMarkerData->m_uiPointNb, pMarkerData->m_uiChannelNb);
		// 判断测道指针不为空
		if(NULL != pChannelMarker)
		{
			// 处理单个采集站IP地址分配
			ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannelMarker);
			// 判断：非迂回道高端标记点，或非迂回道停止标记
			if((false == pInstrumentCollect->m_bDetourMarkerHigh) || (0 == pInstrumentCollect->m_uiStopMarking))
			{
				// 得到测点
				CPointSetupData* pPoint = m_pLogicData->m_oSurveySetupData.GetPoint(pChannelMarker->m_uiIndexPoint);
				// 判断本测点本测道向左有测道
				if(pPoint->m_pChannelHead != pChannelMarker)
				{
					// 向左分配本测点其它测道
					 CInstrument* pInstrumentLeft = ProcIPDistributeMarkerOnePointLeft(pPoint, pInstrumentCollect->m_pInstrumentLeft, pChannelMarker->m_pChannelLeft);
					 // 判断左仪器指针不为空
					if(NULL != pInstrumentLeft)
					 {
						 // 根据增量和方向，得到测点的相邻点
						 pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 3);
						 // 向左依次分配测点IP地址
						 ProcIPDistributeMarkerPointLeft(pPoint, pInstrumentLeft->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);
					 }			
				}
				// 本测点本测道向左无测道
				else
				{
					// 根据增量和方向，得到测点的相邻点
					pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 3);
					// 向左依次分配测点IP地址
					ProcIPDistributeMarkerPointLeft(pPoint, pInstrumentCollect->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);			
				}
			}

			// 判断：非迂回道低端标记点，或非迂回道停止标记
			if((false == pInstrumentCollect->m_bDetourMarkerLow) || (0 == pInstrumentCollect->m_uiStopMarking))
			{
				// 得到测点
				CPointSetupData* pPoint = m_pLogicData->m_oSurveySetupData.GetPoint(pChannelMarker->m_uiIndexPoint);
				// 判断本测点本测道向右有测道
				if(pPoint->m_pChannelTail != pChannelMarker)
				{
					// 向右分配本测点其它测道
					CInstrument* pInstrumentRight = ProcIPDistributeMarkerOnePointRight(pPoint, pInstrumentCollect->m_pInstrumentRight, pChannelMarker->m_pChannelRight);
					// 判断右仪器指针不为空
					if(NULL != pInstrumentRight)
					{
						// 根据增量和方向，得到测点的相邻点
						pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
						// 向右依次分配测点IP地址
						ProcIPDistributeMarkerPointRight(pPoint, pInstrumentRight->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);
					}				
				}
				// 本测点本测道向右无测道
				else
				{
					// 根据增量和方向，得到测点的相邻点
					pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
					// 向右依次分配测点IP地址
					ProcIPDistributeMarkerPointRight(pPoint, pInstrumentCollect->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);		
				}
			}
		}
		// 测道为空
		else
		{
			// 得到测线
			CLineSetupData* pLine = m_pLogicData->m_oSurveySetupData.GetLine(pMarkerData->m_uiLineName);
			// 判断测线不为空
			if(NULL != pLine)
			{
				// 得到测点
				CPointSetupData* pPoint = pLine->m_pPointHead;
				// 向右依次分配测点IP地址
				ProcIPDistributeMarkerPointRight(pPoint, pInstrumentCollect->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);	
			}			
		}
	}
	// 没有找到采集站
	else
	{
		// 向右得到采集站
		pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentMarker, 4);
		// 判断采集站为空
		if(NULL == pInstrumentCollect)
		{
			return;
		}
		// 得到测道
		pChannelMarker = m_pLogicData->m_oSurveySetupData.GetChannel(pMarkerData->m_uiLineName, pMarkerData->m_uiPointNb, pMarkerData->m_uiChannelNb);		
		// 判断标记测道不为空
		if(NULL != pChannelMarker)
		{
			// 向右得到测道
			pChannel = pChannelMarker->m_pChannelRight;
		}
		// 判断测道不为空
		if(NULL != pChannel)
		{
			// 处理单个采集站IP地址分配
			ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannel);
			// 判断：非迂回道低端标记点，或非迂回道停止标记
			if((false == pInstrumentCollect->m_bDetourMarkerLow) || (0 == pInstrumentCollect->m_uiStopMarking))
			{
				// 得到测点
				CPointSetupData* pPoint = m_pLogicData->m_oSurveySetupData.GetPoint(pChannel->m_uiIndexPoint);
				// 判断本测点本测道向右有测道
				if(pPoint->m_pChannelTail != pChannel)
				{
					// 向右分配本测点其它测道
					CInstrument* pInstrumentRight = ProcIPDistributeMarkerOnePointRight(pPoint, pInstrumentCollect->m_pInstrumentRight, pChannel->m_pChannelRight);
					// 判断右仪器不为空
					if(NULL != pInstrumentRight)
					{
						// 根据增量和方向，得到测点的相邻点
						pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
						// 向右依次分配测点IP地址
						ProcIPDistributeMarkerPointRight(pPoint, pInstrumentRight->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);
					}				
				}
				// 本测点本测道向右无测道
				else
				{
					// 根据增量和方向，得到测点的相邻点
					pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
					// 向右依次分配测点IP地址
					ProcIPDistributeMarkerPointRight(pPoint, pInstrumentCollect->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);		
				}
			}
		}
	}
}

/**
* 处理单个标记点IP地址分配，翻转顺序
* @param CMarkerData* pMarkerData 标记点指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerOneReversed(CMarkerData* pMarkerData)
{
	CInstrument* pInstrumentMarker = NULL;	// 标记点仪器	
	CChannelSetupData* pChannelMarker = NULL;	// 标记点测道
	// 标记点未链接到现场
	if(TRUE != m_pSiteData->GetInstrumentFromSNMapBySN(pMarkerData->m_uiSN, pInstrumentMarker))
	{
		return;
	}
	// 判断：上次已设置为标记点
	if(true == pInstrumentMarker->m_bMarker)
	{
		// 标记点标记时间
		pInstrumentMarker->m_uiMarkerLogoTime = m_iIPDistributeTime;
	}
	// 判断：上次不是设置为标记点
	else
	{
		// 是否标记点
		pInstrumentMarker->m_bMarker = true;
		// 标记点标记时间
		pInstrumentMarker->m_uiMarkerLogoTime = m_iIPDistributeTime;
		// 加入采集道标记仪器索引表
		m_oInstrumentMarkerLogoSNMap.SetAt(pInstrumentMarker->m_uiSN, pInstrumentMarker);
	}

	CInstrument* pInstrumentCollect = NULL;	// 采集站
	CChannelSetupData* pChannel = NULL;	// 测道
	// 向右得到采集站
	pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentMarker, 4);	
	if(NULL != pInstrumentCollect)	// 找到采集站
	{
		// 得到测道
		pChannelMarker = m_pLogicData->m_oSurveySetupData.GetChannel(pMarkerData->m_uiLineName, pMarkerData->m_uiPointNb, pMarkerData->m_uiChannelNb);
		// 测道不为空
		if(NULL != pChannelMarker)
		{
			// 处理单个采集站IP地址分配
			ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannelMarker);
			// 非迂回道低端标记点，或非迂回道停止标记
			if((false == pInstrumentCollect->m_bDetourMarkerLow) || (0 == pInstrumentCollect->m_uiStopMarking))
			{
				// 测点
				CPointSetupData* pPoint = m_pLogicData->m_oSurveySetupData.GetPoint(pChannelMarker->m_uiIndexPoint);
				if(pPoint->m_pChannelHead != pChannelMarker)	// 本测点本测道向左有测道
				{
					// 向左分配本测点其它测道
					CInstrument* pInstrumentRight = ProcIPDistributeMarkerOnePointLeftReversed(pPoint, pInstrumentCollect->m_pInstrumentRight, pChannelMarker->m_pChannelLeft);
					if(NULL != pInstrumentRight)	// 成功
					{
						// 根据增量和方向，得到测点的相邻点
						pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 3);
						// 向左依次分配测点IP地址
						ProcIPDistributeMarkerPointLeftReversed(pPoint, pInstrumentRight->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);
					}			
				}
				else	// 本测点本测道向左无测道
				{
					// 根据增量和方向，得到测点的相邻点
					pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 3);
					// 向左依次分配测点IP地址
					ProcIPDistributeMarkerPointLeftReversed(pPoint, pInstrumentCollect->m_pInstrumentRight, pMarkerData->m_uiMarkerIncr);			
				}
			}

			// 非迂回道高端标记点，或非迂回道停止标记
			if((false == pInstrumentCollect->m_bDetourMarkerHigh) || (0 == pInstrumentCollect->m_uiStopMarking))
			{
				// 测点
				CPointSetupData* pPoint = m_pLogicData->m_oSurveySetupData.GetPoint(pChannelMarker->m_uiIndexPoint);
				if(pPoint->m_pChannelTail != pChannelMarker)	// 本测点本测道向右有测道
				{
					// 向右分配本测点其它测道
					CInstrument* pInstrumentLeft = ProcIPDistributeMarkerOnePointRightReversed(pPoint, pInstrumentCollect->m_pInstrumentLeft, pChannelMarker->m_pChannelRight);
					if(NULL != pInstrumentLeft)	// 成功
					{
						// 根据增量和方向，得到测点的相邻点
						pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
						// 向右依次分配测点IP地址
						ProcIPDistributeMarkerPointRightReversed(pPoint, pInstrumentLeft->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);
					}				
				}
				else	// 本测点本测道向右无测道
				{
					// 根据增量和方向，得到测点的相邻点
					pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
					// 向右依次分配测点IP地址
					ProcIPDistributeMarkerPointRightReversed(pPoint, pInstrumentCollect->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);		
				}
			}
		}
		else	// 测道为空
		{
			// 得到测线
			CLineSetupData* pLine = m_pLogicData->m_oSurveySetupData.GetLine(pChannelMarker->m_uiNbLine);
			if(NULL != pLine)
			{
				// 得到测点
				CPointSetupData* pPoint = pLine->m_pPointHead;
				// 向右依次分配测点IP地址
				ProcIPDistributeMarkerPointRightReversed(pPoint, pInstrumentCollect->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);	
			}			
		}
	}
	else	// 没有找到采集站
	{
		// 向左得到采集站
		pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentMarker, 3);	
		if(NULL == pInstrumentCollect)	// 采集站为空
		{
			return;
		}

		// 得到测道
		pChannelMarker = m_pLogicData->m_oSurveySetupData.GetChannel(pMarkerData->m_uiLineName, pMarkerData->m_uiPointNb, pMarkerData->m_uiChannelNb);		
		if(NULL != pChannelMarker)	// 测道不为空
		{
			// 向右得到测道
			pChannel = pChannelMarker->m_pChannelRight;	// 测道
		}

		if(NULL != pChannel)	// 测道不为空
		{
			// 处理单个采集站IP地址分配
			ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannel);
			// 非迂回道高端标记点，或非迂回道停止标记
			if((false == pInstrumentCollect->m_bDetourMarkerHigh) || (0 == pInstrumentCollect->m_uiStopMarking))
			{
				// 测点
				CPointSetupData* pPoint = m_pLogicData->m_oSurveySetupData.GetPoint(pChannel->m_uiIndexPoint);
				if(pPoint->m_pChannelTail != pChannel)	// 本测点本测道向右有测道
				{
					// 向右分配本测点其它测道
					CInstrument* pInstrumentLeft = ProcIPDistributeMarkerOnePointRightReversed(pPoint, pInstrumentCollect->m_pInstrumentLeft, pChannel->m_pChannelRight);
					if(NULL != pInstrumentLeft)	// 成功
					{
						// 根据增量和方向，得到测点的相邻点
						pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
						// 向右依次分配测点IP地址
						ProcIPDistributeMarkerPointRightReversed(pPoint, pInstrumentLeft->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);
					}				
				}
				else	// 本测点本测道向右无测道
				{
					// 根据增量和方向，得到测点的相邻点
					pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, pMarkerData->m_uiMarkerIncr, 4);
					// 向右依次分配测点IP地址
					ProcIPDistributeMarkerPointRightReversed(pPoint, pInstrumentCollect->m_pInstrumentLeft, pMarkerData->m_uiMarkerIncr);		
				}
			}
		}
	}
}

/**
* 处理单个采集站IP地址分配
* @param CInstrument* pInstrumentCollect 采集站指针
* @param CChannelSetupData* pChannel 测道指针
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerOneInstrument(CInstrument* pInstrumentCollect, CChannelSetupData* pChannel)
{
	if((NULL != pInstrumentCollect) && (NULL != pChannel))
	{
		// 判断：仪器未分配过IP地址
		if(0 == pInstrumentCollect->m_uiIP)
		{
			// 仪器IP地址
			pInstrumentCollect->m_uiIP = pChannel->m_uiIP;
			// 仪器测线号
			pInstrumentCollect->m_uiLineNb = pChannel->m_uiNbLine;
			// 仪器测点号
			pInstrumentCollect->m_uiPointNb = pChannel->m_uiNbPoint;
			// 仪器测道号
			pInstrumentCollect->m_uiChannelNb = pChannel->m_uiNbChannel;
			// 是否连接检波器
			pInstrumentCollect->m_bSensor = true;
			// 是否跳过道
			pInstrumentCollect->m_bJumpedChannel = pChannel->m_bJumped;
			// 设置仪器IP地址分配时间
			pInstrumentCollect->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 标记点标记时间
			pInstrumentCollect->m_uiMarkerTime = m_iIPDistributeTime;
			// 仪器设备号
			pChannel->m_uiSN = pInstrumentCollect->m_uiSN;
			// 加入采集道仪器SN索引表
			m_oInstrumentMarkerSNMap.SetAt(pInstrumentCollect->m_uiSN, pInstrumentCollect);
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrumentCollect->m_uiIP, pInstrumentCollect);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrumentCollect);
			TRACE2("Marker IPSet SN号=%d IP=%d \r\n", pInstrumentCollect->m_uiSN,pInstrumentCollect->m_uiIP);
		}
		// 判断：仪器需要重新设置IP地址
		else if(pChannel->m_uiIP != pInstrumentCollect->m_uiIP)
		{
			// 判断：仪器为采集站；非本次分配
			if((3 == pInstrumentCollect->m_uiInstrumentType) && (pInstrumentCollect->m_uiIPDistributeTime != m_iIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrumentCollect->m_uiIP, m_iIPDistributeTime);
			}
			// 仪器IP地址
			pInstrumentCollect->m_uiIP = pChannel->m_uiIP;
			// 仪器测线号
			pInstrumentCollect->m_uiLineNb = pChannel->m_uiNbLine;
			// 仪器测点号
			pInstrumentCollect->m_uiPointNb = pChannel->m_uiNbPoint;
			// 仪器测道号
			pInstrumentCollect->m_uiChannelNb = pChannel->m_uiNbChannel;
			// 是否连接检波器
			pInstrumentCollect->m_bSensor = true;
			// 是否跳过道
			pInstrumentCollect->m_bJumpedChannel = pChannel->m_bJumped;
			// 设置仪器IP地址分配时间
			pInstrumentCollect->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 标记点标记时间
			pInstrumentCollect->m_uiMarkerTime = m_iIPDistributeTime;
			// 仪器设备号
			pChannel->m_uiSN = pInstrumentCollect->m_uiSN;
			// 加入采集道仪器SN索引表
			m_oInstrumentMarkerSNMap.SetAt(pInstrumentCollect->m_uiSN, pInstrumentCollect);
			// 加入仪器IP地址索引表
			m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrumentCollect->m_uiIP, pInstrumentCollect);
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrumentCollect);
			TRACE2("pChannel IPSet SN号=%d IP=%d \r\n", pInstrumentCollect->m_uiSN,pInstrumentCollect->m_uiIP);
		}
		else	// 上次分配正确
		{
			// 设置仪器IP地址分配时间
			pInstrumentCollect->m_uiIPDistributeTime = m_iIPDistributeTime;
			// 标记点标记时间
			pInstrumentCollect->m_uiMarkerTime = m_iIPDistributeTime;
		}
	}
}

/**
* 处理单个测点IP地址分配，向左分配
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param CChannelSetupData* pChannel 测道指针
* @return void
*/
CInstrument* CThreadProcIPDistribute::ProcIPDistributeMarkerOnePointLeft(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel)
{
	CInstrument* pInstrumentCollect = pInstrument;
	CChannelSetupData* pChannelMarker = pChannel;

	while(true)
	{
		if(NULL == pInstrument)
		{
			return NULL;
		}
		if(NULL == pChannel)
		{
			return NULL;
		}
		pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentCollect, 3);
		if(NULL == pInstrumentCollect)
		{
			return NULL;
		}
		// 处理单个采集站IP地址分配
		ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannelMarker);
		// 迂回道高端标记点，且迂回道停止标记
		if((true == pInstrumentCollect->m_bDetourMarkerHigh) && (1 == pInstrumentCollect->m_uiStopMarking))
		{
			return NULL;
		}
		if(pPoint->m_pChannelHead == pChannelMarker)
		{
			return pInstrumentCollect;
		}
		pInstrumentCollect = pInstrumentCollect->m_pInstrumentLeft;
		pChannelMarker = pChannelMarker->m_pChannelLeft;
	}
	return NULL;
}

/**
* 处理单个测点IP地址分配，向右分配
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param CChannelSetupData* pChannel 测道指针
* @return void
*/
CInstrument* CThreadProcIPDistribute::ProcIPDistributeMarkerOnePointRight(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel)
{

	CInstrument* pInstrumentCollect = pInstrument;
	CChannelSetupData* pChannelMarker = pChannel;

	while(true)
	{
		if(NULL == pInstrument)
		{
			return NULL;
		}
		if(NULL == pChannel)
		{
			return NULL;
		}
		pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentCollect, 4);
		if(NULL == pInstrumentCollect)
		{
			return NULL;
		}
		// 处理单个采集站IP地址分配
		ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannelMarker);
		// 迂回道低端标记点，且迂回道停止标记
		if((true == pInstrumentCollect->m_bDetourMarkerLow) && (1 == pInstrumentCollect->m_uiStopMarking))
		{
			return NULL;
		}
		if(pPoint->m_pChannelTail== pChannelMarker)
		{
			return pInstrumentCollect;
		}
		pInstrumentCollect = pInstrumentCollect->m_pInstrumentRight;
		pChannelMarker = pChannelMarker->m_pChannelRight;
	}
	return false;
}

/**
* 处理测点IP地址分配，向左分配
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param unsigned int uiIncrement 测点增量
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerPointLeft(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement)
{
	CInstrument* pInstrumentCollect = pInstrument;

	if(NULL == pInstrument)
	{
		return;
	}
	if(NULL == pPoint)
	{
		return;
	}

	pInstrumentCollect = ProcIPDistributeMarkerOnePointLeft(pPoint, pInstrumentCollect, pPoint->m_pChannelTail);
	if(NULL == pInstrumentCollect)
	{
		return;
	}
	// 根据增量和方向，得到测点的相邻点
	pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, uiIncrement, 3);
	// 处理单个采集站IP地址分配
	ProcIPDistributeMarkerPointLeft(pPoint, pInstrumentCollect->m_pInstrumentLeft, uiIncrement);
}

/**
* 处理测点IP地址分配，向右分配
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param unsigned int uiIncrement 测点增量
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerPointRight(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement)
{
	CInstrument* pInstrumentCollect = pInstrument;

	if(NULL == pInstrument)
	{
		return;
	}
	if(NULL == pPoint)
	{
		return;
	}

	pInstrumentCollect = ProcIPDistributeMarkerOnePointRight(pPoint, pInstrumentCollect, pPoint->m_pChannelHead);
	if(NULL == pInstrumentCollect)
	{
		return;
	}
	// 根据增量和方向，得到测点的相邻点
	pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, uiIncrement, 4);
	// 处理单个采集站IP地址分配
	ProcIPDistributeMarkerPointRight(pPoint, pInstrumentCollect->m_pInstrumentRight, uiIncrement);
}

/**
* 处理单个测点IP地址分配，向左分配，翻转
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param CChannelSetupData* pChannel 测道指针
* @return void
*/
CInstrument* CThreadProcIPDistribute::ProcIPDistributeMarkerOnePointLeftReversed(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel)
{
	CInstrument* pInstrumentCollect = pInstrument;
	CChannelSetupData* pChannelMarker = pChannel;

	while(true)
	{
		if(NULL == pInstrument)
		{
			return NULL;
		}
		if(NULL == pChannel)
		{
			return NULL;
		}
		pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentCollect, 4);
		if(NULL == pInstrumentCollect)
		{
			return NULL;
		}
		// 处理单个采集站IP地址分配
		ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannelMarker);
		// 迂回道低端标记点，且迂回道停止标记
		if((true == pInstrumentCollect->m_bDetourMarkerLow) && (1 == pInstrumentCollect->m_uiStopMarking))
		{
			return NULL;
		}
		if(pPoint->m_pChannelHead == pChannelMarker)
		{
			return pInstrumentCollect;
		}
		pInstrumentCollect = pInstrumentCollect->m_pInstrumentRight;
		pChannelMarker = pChannelMarker->m_pChannelLeft;
	}
	return NULL;
}

/**
* 处理单个测点IP地址分配，向右分配，翻转
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param CChannelSetupData* pChannel 测道指针
* @return void
*/
CInstrument* CThreadProcIPDistribute::ProcIPDistributeMarkerOnePointRightReversed(CPointSetupData* pPoint, CInstrument* pInstrument, CChannelSetupData* pChannel)
{

	CInstrument* pInstrumentCollect = pInstrument;
	CChannelSetupData* pChannelMarker = pChannel;

	while(true)
	{
		if(NULL == pInstrument)
		{
			return NULL;
		}
		if(NULL == pChannel)
		{
			return NULL;
		}
		pInstrumentCollect = m_pSiteData->GetInstrumentCollect(pInstrumentCollect, 3);
		if(NULL == pInstrumentCollect)
		{
			return NULL;
		}
		// 处理单个采集站IP地址分配
		ProcIPDistributeMarkerOneInstrument(pInstrumentCollect, pChannelMarker);
		// 迂回道高端标记点，且迂回道停止标记
		if((true == pInstrumentCollect->m_bDetourMarkerHigh) && (1 == pInstrumentCollect->m_uiStopMarking))
		{
			return NULL;
		}
		if(pPoint->m_pChannelTail== pChannelMarker)
		{
			return pInstrumentCollect;
		}
		pInstrumentCollect = pInstrumentCollect->m_pInstrumentLeft;
		pChannelMarker = pChannelMarker->m_pChannelRight;
	}
	return false;
}

/**
* 处理测点IP地址分配，向左分配，翻转
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param unsigned int uiIncrement 测点增量
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerPointLeftReversed(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement)
{
	CInstrument* pInstrumentCollect = pInstrument;

	if(NULL == pInstrument)
	{
		return;
	}
	if(NULL == pPoint)
	{
		return;
	}

	pInstrumentCollect = ProcIPDistributeMarkerOnePointLeftReversed(pPoint, pInstrumentCollect, pPoint->m_pChannelTail);
	if(NULL == pInstrumentCollect)
	{
		return;
	}
	// 根据增量和方向，得到测点的相邻点
	pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, uiIncrement, 3);
	// 处理单个采集站IP地址分配
	ProcIPDistributeMarkerPointLeftReversed(pPoint, pInstrumentCollect->m_pInstrumentRight, uiIncrement);
}

/**
* 处理测点IP地址分配，向右分配，翻转
* @param CPointSetupData* pPoint 测点指针
* @param CInstrument* pInstrumentCollect 采集站指针
* @param unsigned int uiIncrement 测点增量
* @return void
*/
void CThreadProcIPDistribute::ProcIPDistributeMarkerPointRightReversed(CPointSetupData* pPoint, CInstrument* pInstrument, unsigned int uiIncrement)
{
	CInstrument* pInstrumentCollect = pInstrument;

	if(NULL == pInstrument)
	{
		return;
	}
	if(NULL == pPoint)
	{
		return;
	}

	pInstrumentCollect = ProcIPDistributeMarkerOnePointRightReversed(pPoint, pInstrumentCollect, pPoint->m_pChannelHead);
	if(NULL == pInstrumentCollect)
	{
		return;
	}
	// 根据增量和方向，得到测点的相邻点
	pPoint = m_pLogicData->m_oSurveySetupData.GetPointByIncrementAndDirection(pPoint, uiIncrement, 4);
	// 处理单个采集站IP地址分配
	ProcIPDistributeMarkerPointRightReversed(pPoint, pInstrumentCollect->m_pInstrumentLeft, uiIncrement);
}

/**
* 清理过期采集道标记
* @param void
* @return bool true：没有要清理的过期标记；false：有要清理的过期标记
*/
bool CThreadProcIPDistribute::ProcClearTimeOutMarker()
{
	bool bReturn = true;

	POSITION pos = NULL;	// 位置	
	unsigned int uiSN;	// 仪器设备号
	CInstrument* pInstrument = NULL;	// 仪器

	pos = m_oInstrumentMarkerLogoSNMap.GetStartPosition();
	while(NULL != pos)
	{
		m_oInstrumentMarkerLogoSNMap.GetNextAssoc(pos, uiSN, pInstrument);
		if(pInstrument->m_uiMarkerLogoTime != m_iIPDistributeTime)
		{
			// 是否采集道
			pInstrument->m_bMarker = false;
			// 采集道标记时间
			pInstrument->m_uiMarkerLogoTime = 0;
			m_oInstrumentMarkerLogoSNMap.RemoveKey(uiSN);
		}
	}

	pos = m_oInstrumentMarkerSNMap.GetStartPosition();
	while(NULL != pos)
	{
		m_oInstrumentMarkerSNMap.GetNextAssoc(pos, uiSN, pInstrument);
		if(pInstrument->m_uiMarkerTime != m_iIPDistributeTime)
		{
			// 判断：仪器IP地址非0；仪器为采集站；非本次分配
			if((0 != pInstrument->m_uiIP) && (3 == pInstrument->m_uiInstrumentType) && (pInstrument->m_uiIPDistributeTime != m_iIPDistributeTime))
			{
				// 释放仪器IP地址
				m_pSiteData->ReleaseInstrument(pInstrument->m_uiIP, m_iIPDistributeTime);
				// 仪器IP地址
				pInstrument->m_uiIP = 0;
				// 加入仪器IP地址设置队列
				m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
				bReturn = false;
			}
			// 仪器IP地址
			pInstrument->m_uiIP = 0;
			// 加入仪器IP地址设置队列
			m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
			// 仪器测线号
			pInstrument->m_uiLineNb = 0;
			// 仪器测点号
			pInstrument->m_uiPointNb = 0;
			// 仪器测道号
			pInstrument->m_uiChannelNb = 0;
			// 是否连接检波器
			pInstrument->m_bSensor = false;
			// 是否跳过道
			pInstrument->m_bJumpedChannel = false;
			// 设置仪器IP地址分配时间
			pInstrument->m_uiIPDistributeTime = 0;
			// 采集道标记时间
			pInstrument->m_uiMarkerTime = 0;
			m_oInstrumentMarkerSNMap.RemoveKey(uiSN);
		}
	}
	return bReturn;
}

