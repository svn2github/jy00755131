// ThreadProcHeadFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcHeadFrame.h"


// CThreadProcHeadFrame

IMPLEMENT_DYNCREATE(CThreadProcHeadFrame, CWinThread)

CThreadProcHeadFrame::CThreadProcHeadFrame()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
}

CThreadProcHeadFrame::~CThreadProcHeadFrame()
{
}

BOOL CThreadProcHeadFrame::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcHeadFrame::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcHeadFrame, CWinThread)
END_MESSAGE_MAP()

// CThreadProcHeadFrame 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcHeadFrame::Run()
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
		// 处理首包帧
		ProcHeadFrame();
		// 判断关闭标志是否为真
		if(true == m_bClose)
		{
			// 跳出循环
			break;
		}
		// 休眠50毫秒
		Wait(1);
	}
	// 首包处理线程关闭标志为真
	m_pSiteData->m_bProcHeadFrameClose = true;
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcHeadFrame::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	// 是否发生了增加仪器
	m_bInstrumentAdd = false;
	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcHeadFrame::OnClose()
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
bool CThreadProcHeadFrame::OnWork()
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
bool CThreadProcHeadFrame::OnStop()
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
void CThreadProcHeadFrame::Wait(int iWaitStopCount)
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
bool CThreadProcHeadFrame::JudgeProcCondition()
{	
	// 删除仪器或路由处理线程正在运行，或现场数据输出线程正在运行
	if((true == m_pSiteData->m_bProcDeleteRunning) || (true == m_pSiteData->m_bProcSiteDataOutputRunning))
	{		
		m_pSiteData->m_bProcHeadFramePause = true;	// 首包处理线程暂停
		return false;
	}
	else
	{		
		m_pSiteData->m_bProcHeadFramePause = false;	// 首包处理线程工作
	}
	// 非工作状态
	if(false == m_bWork)
	{		
		m_pSiteData->m_bProcHeadFrameStop = true;	// 首包处理线程停止
		return false;
	}
	else
	{
		m_pSiteData->m_bProcHeadFrameStop = false;	// 首包处理线程工作
	}
	return true;
}

/**
* 处理首包帧
* @note 由网络端口得到野外仪器发来的首包帧，逐帧处理。
* @note 非处理状态时，读取首包帧，然后丢弃不处理。
* @note 非处理状态发生在：现场处理停止时；删除仪器或路由处理线程正在运行时；现场数据输出线程正在运行时。
* @param void
* @return void
*/
void CThreadProcHeadFrame::ProcHeadFrame()
{
	// 是否发生了增加仪器标志设置为假
	m_bInstrumentAdd = false;
	// 帧数量设置为0
	int iFrameCount = 0;
	// 得到首包接收网络端口帧数量
	iFrameCount = m_pSocketHeadFrame->GetFrameCount();
	// 判断帧数量是否大于0
	if(iFrameCount > 0)
	{
		// 循环处理每1帧
		for(int i = 0; i < iFrameCount; i++)
		{
			if(iFrameCount>3)
				int k=0;
			// 得到帧数据
			m_pSocketHeadFrame->GetFrameData();
			// 判断是否可以处理的条件
			if(true ==JudgeProcCondition())
			{
				// 处理单个首包帧
				ProcHeadFrameOne(&m_pSocketHeadFrame->m_oFrameHead);			
			}
		}		
	}
	// 判断增加仪器标志是否为真
	if(true == m_bInstrumentAdd)
	{
		// 更新上次现场数据变化时刻
		m_pSiteData->UpdateSiteDataChangedTime();
	}
}

/**
* 重发时间靠后的首包
* @note 处理流程：。
* @param CFrameHead* pFrameHead 首包帧指针
* @return void
*/
void CThreadProcHeadFrame::SendHeadFrame()
{
	m_pSocketHeadFrame->SendFrameData();
}

/**
* 处理单个首包帧
* @note 处理流程：仪器SN是否在SN索引表中处理；仪器是否有路由地址处理。
* @param CFrameHead* pFrameHead 首包帧指针
* @return void
*/
void CThreadProcHeadFrame::ProcHeadFrameOne(CFrameHead* pFrameHead)
{
	// 判断仪器SN是否在SN索引表中
	if(TRUE == m_pSiteData->IfSNExistInSNMap(pFrameHead->m_uiSN))
	{
		// 更新路由对象的路由时间
		m_pSiteData->UpdateRoutTime(pFrameHead->m_uiRoutIP);
		// 增加一个需要设置IP地址的仪器
		//AddInstrumentForIPSet(pFrameHead);		//bug,不需要再增加IP设置的仪器
		return;
	}

	CString strDesc;
	strDesc.Format("ProcHeadFrameOne1 SN=%d IP=%d TimeHead=%x RoutIP=%x", pFrameHead->m_uiSN, pFrameHead->m_uiRoutIP,pFrameHead->m_uiTimeHeadFrame,pFrameHead->m_uiRoutIP);
	m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcHeadFrame", "ProcHeadFrameOne", strDesc);
	TRACE3("首包 SN=%d IP=%d TimeHead=%x\r\n",pFrameHead->m_uiSN, pFrameHead->m_uiRoutIP,pFrameHead->m_uiTimeHeadFrame); 

		// 判断仪器是否有路由地址；没有路由地址的仪器为主交叉站
	if(0 == pFrameHead->m_uiRoutIP)
	{
		// 新仪器指针为空
		CInstrument* pInstrument = NULL;
		// 得到新仪器
		GetFreeInstrument(pInstrument, pFrameHead);
		// 增加仪器标志设置为真
		m_bInstrumentAdd = true;
		// 判断首个主交叉站指针是否为空；首个主交叉站指针为空，增加首个主交叉站
		if(NULL == m_pSiteData->m_pFirstMainCross)
		{
			// 首个主交叉站指针指向新仪器
			m_pSiteData->m_pFirstMainCross = pInstrument;
			// 增加主交叉站
			AddMainCross(pInstrument);
			
			m_pSiteData->HeadminTime=0xffffffff;
			m_pSiteData->HeadCount=0;
			m_pSiteData->HeadEtime=0;
			//初始化网络命令错和数据错
			m_pSiteData->NetDataCount=0;
			m_pSiteData->NetOrderCount=0;

			// 运行状态数据
			CString strDesc;
			// 格式化运行状态数据
			strDesc.Format("Add First Main Cross SN=%d；IP=%d", pInstrument->m_uiSN, pInstrument->m_uiIP);
			// 设置运行状态数据
			m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcHeadFrame", "ProcHeadFrameOne", strDesc);
		}
		// 首个主交叉站指针不为空；增加主交叉站，增加在主交叉站链的尾部
		else
		{
			// 主交叉站指针指向首个主交叉站
			CInstrument* pMainCross = m_pSiteData->m_pFirstMainCross;
			//循环，直到找到主交叉站链的尾部
			while(NULL != pMainCross->m_pMainCrossNext)
			{
				// 得到最后一个主交叉站
				pMainCross = pMainCross->m_pMainCrossNext;
			}
			// 新主交叉站加在主交叉站链尾部
			pMainCross->m_pMainCrossNext = pInstrument;
			pMainCross->m_uiSNMainCrossNext = pInstrument->m_uiSN;
			pMainCross->m_iIndexCrossDown = pInstrument->m_uiIndex;
			pInstrument->m_pMainCrossPreview = pMainCross;
			pInstrument->m_uiSNMainCrossPreview = pMainCross->m_uiSN;
			pInstrument->m_iIndexMainCrossPreview = pMainCross->m_uiIndex;

			// 增加主交叉站
			AddMainCross(pInstrument);
		}
		// 增加一个需要设置IP地址的仪器
		AddInstrumentForIPSet(pInstrument);
	}
	//仪器有路由地址
	else
	{
		if(m_pSiteData->HeadCount<5){
			m_pSiteData->HeadCount++;
			return;
		}

		pFrameHead->m_uiRoutIP=0x0f & pFrameHead->m_uiRoutIP;		// add test by zl 04.12
		// 路由指针为空
		CRout* pRout = NULL;
		// 判断是否得到路由对象
		if(false == m_pSiteData->m_oRoutIPRoutMap.GetRout(pFrameHead->m_uiRoutIP, pRout))
		{
			// 找不到路由对象；记录错误到日志文件
			m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcHeadFrame", "ProcHeadFrameOne", "Can not find Rout");
			return;
		}
		// 新仪器指针为空
		CInstrument* pInstrument = NULL;
		// 得到新仪器
		GetFreeInstrument(pInstrument, pFrameHead);
		// 路由方向 1-上；2-下；3-左；4右
		pInstrument->m_uiRoutDirection = pRout->m_uiRoutDirection;
		// 增加仪器标志设置为真
		m_bInstrumentAdd = true;

		// 判断有路由地址，来自交叉站上方
		if(1 == pRout->m_uiRoutDirection)
		{
			//增加一个交叉站
			AddCrossTop(pInstrument);
		}
		// 判断有路由地址，来自交叉站下方
		if(2 == pRout->m_uiRoutDirection)
		{
			//增加一个交叉站
			AddCrossDown(pInstrument);
		}
		// 判断有路由地址，来自交叉站左方
		if(3 == pRout->m_uiRoutDirection)
		{
			// 判断仪器为交叉站，仪器类型 1-交叉站；2-电源站；3-采集站
			if(1 == pInstrument->m_uiInstrumentType)
			{
				// 增加交叉站测线方向，来自交叉站左方
				AddCrossLeft(pInstrument);
			}
			// 判断仪器为电源站，仪器类型 1-交叉站；2-电源站；3-采集站
			if(2 == pInstrument->m_uiInstrumentType)
			{
				// 增加电源站，来自交叉站左方
				AddPowerLeft(pInstrument);
			}
			// 判断仪器为采集站，仪器类型 1-交叉站；2-电源站；3-采集站
			if(3 == pInstrument->m_uiInstrumentType)
			{
				// 增加采集站，来自交叉站左方
				AddCollectLeft(pInstrument);
			}
		}
		// 判断有路由地址，来自交叉站右方
		if(4 == pRout->m_uiRoutDirection)
		{
			// 判断仪器为交叉站，仪器类型 1-交叉站；2-电源站；3-采集站
			if(1 == pInstrument->m_uiInstrumentType)
			{
				// 增加交叉站测线方向，来自交叉站右方
				AddCrossRight(pInstrument);
			}
			// 判断仪器为电源站，仪器类型 1-交叉站；2-电源站；3-采集站
			if(2 == pInstrument->m_uiInstrumentType)
			{
				// 增加电源站，来自交叉站右方
				AddPowerRight(pInstrument);
			}
			// 判断仪器为采集站，仪器类型 1-交叉站；2-电源站；3-采集站
			if(3 == pInstrument->m_uiInstrumentType)
			{
				// 增加采集站，来自交叉站右方
				AddCollectRight(pInstrument);
			}
		}
		// 增加一个需要设置IP地址的仪器		重复的IP分配，取消 2011.9.13
		//AddInstrumentForIPSet(pInstrument);

		// 运行状态数据
		CString strDesc;
		// 格式化运行状态数据
		strDesc.Format("Add Instrument SN=%d IP=%d TimeHead=%x", pInstrument->m_uiSN, pInstrument->m_uiIP,pInstrument->m_uiTimeHeadFrame);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcHeadFrame", "ProcHeadFrameOne", strDesc);
		TRACE3("IP设置 Instrument SN=%d IP=%d TimeHead=%x\r\n",pInstrument->m_uiSN, pInstrument->m_uiIP,pInstrument->m_uiTimeHeadFrame); 
	}
}

/**
* 由空闲仪器队列得到新仪器；把首包帧带来的仪器属性赋值给新仪器；新仪器加入SN索引表
* @param CInstrument* &pInstrument 新仪器指针，引用
* @param CFrameHead* pFrameHead 首包帧指针
* @return void
*/
void CThreadProcHeadFrame::GetFreeInstrument(CInstrument* &pInstrument, CFrameHead* pFrameHead)
{
	// 得到一个新仪器
	pInstrument = m_pSiteData->GetFreeInstrument();
	//设置新仪器的SN
	pInstrument->m_uiSN = pFrameHead->m_uiSN;
	//设置新仪器的仪器类型
	pInstrument->m_uiInstrumentType = pFrameHead->m_uiInstrumentType;
	//设置新仪器的路由IP地址
	pInstrument->m_uiRoutIP = pFrameHead->m_uiRoutIP;
	//设置新仪器的首包时刻
	pInstrument->m_uiTimeHeadFrame = pFrameHead->m_uiTimeHeadFrame;
	// 新仪器加入SN索引表
	m_pSiteData->m_oSNInstrumentMap.AddInstrument(pInstrument->m_uiSN, pInstrument);
}

/**
* 增加主交叉站
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddMainCross(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreeCrossIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	if(1 == m_pLogicData->m_oLookSetupData.m_uiLookAuto)	// 自动识别
	{
		// 设置交叉站路由方向上
		SetCrossRout(pInstrument, 1);
		// 设置交叉站路由方向下
		SetCrossRout(pInstrument, 2);
		// 设置交叉站路由方向左
		SetCrossRout(pInstrument, 3);
		// 设置交叉站路由方向右
		SetCrossRout(pInstrument, 4);
	}
}

/**
* 增加交叉站；交叉线方向，上方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddCrossTop(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreeCrossIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	// 设置下行方向路由地址
	pInstrument->m_uiRoutIPDown = 0xFFFF;
	if(1 == m_pLogicData->m_oLookSetupData.m_uiLookAuto)	// 自动识别
	{
		// 设置交叉站路由方向上
//		SetCrossRout(pInstrument, 1);
		// 设置交叉站路由方向左
		SetCrossRout(pInstrument, 3);
		// 设置交叉站路由方向右
		SetCrossRout(pInstrument, 4);
	}

	CRout* pRout = NULL;
	// 得到路由对象
	m_pSiteData->m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout);
	// 更新路由对象的路由时间
	pRout->UpdateRoutTime();
	// 判断：交叉站路由方向没有链接仪器
	if(NULL == pRout->m_pTail)
	{
		// 链接仪器，新仪器下指针指向路由头指针
		pInstrument->m_pCrossDown = pRout->m_pHead;
		pInstrument->m_uiSNCrossDown = pRout->m_pHead->m_uiSN;
		pInstrument->m_iIndexCrossDown = pRout->m_pHead->m_uiIndex;

		// 链接仪器，路由头仪器上指针指向新仪器
		pRout->m_pHead->m_pCrossTop = pInstrument;
		pRout->m_pHead->m_uiSNCrossTop = pInstrument->m_uiSN;
		pRout->m_pHead->m_iIndexCrossTop = pInstrument->m_uiIndex;

		// 设置路由对象尾指针
		pRout->m_pTail = pInstrument;
		pRout->m_uiSNTail = pInstrument->m_uiSN;
		pRout->m_iIndexTail = pInstrument->m_uiIndex;
	}
	// 判断：交叉站路由方向链接仪器
	else
	{
		// 链接仪器，新仪器右指针指向路由尾仪器
		pInstrument->m_pCrossDown = pRout->m_pTail;
		pInstrument->m_uiSNCrossDown = pRout->m_pTail->m_uiSN;
		pInstrument->m_iIndexCrossDown = pRout->m_pTail->m_uiIndex;

		// 链接仪器，路由尾仪器左指针指向新仪器
		pRout->m_pTail->m_pCrossTop = pInstrument;
		pRout->m_pTail->m_uiSNCrossTop = pInstrument->m_uiSN;
		pRout->m_pTail->m_iIndexCrossTop = pInstrument->m_uiIndex;

		// 设置路由对象尾指针
		pRout->m_pTail = pInstrument;
		pRout->m_uiSNTail = pInstrument->m_uiSN;
		pRout->m_iIndexTail = pInstrument->m_uiIndex;
	}
}

/**
* 增加交叉站；交叉线方向，下方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddCrossDown(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreeCrossIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	// 设置下行方向路由地址
	pInstrument->m_uiRoutIPTop = 0xFFFF;
	if(1 == m_pLogicData->m_oLookSetupData.m_uiLookAuto)	// 自动识别
	{
		// 设置交叉站路由方向下
//		SetCrossRout(pInstrument, 2);
		// 设置交叉站路由方向左
		SetCrossRout(pInstrument, 3);
		// 设置交叉站路由方向右
		SetCrossRout(pInstrument, 4);
	}

	CRout* pRout = NULL;
	// 得到路由对象
	m_pSiteData->m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout);
	// 更新路由对象的路由时间
	pRout->UpdateRoutTime();
	// 判断：交叉站路由方向没有链接仪器
	if(NULL == pRout->m_pTail)
	{
		// 链接仪器，新仪器上指针指向路由头指针
		pInstrument->m_pCrossTop = pRout->m_pHead;
		pInstrument->m_uiSNCrossTop = pRout->m_pHead->m_uiSN;
		pInstrument->m_iIndexCrossTop = pRout->m_pHead->m_uiIndex;

		// 链接仪器，路由头仪器下指针指向新仪器
		pRout->m_pHead->m_pCrossDown = pInstrument;
		pRout->m_pHead->m_uiSNCrossDown = pInstrument->m_uiSN;
		pRout->m_pHead->m_iIndexCrossDown = pInstrument->m_uiIndex;

		// 设置路由对象尾指针
		pRout->m_pTail = pInstrument;
		pRout->m_uiSNTail = pInstrument->m_uiSN;
		pRout->m_iIndexTail = pInstrument->m_uiIndex;
	}
	// 判断：交叉站路由方向链接仪器
	else
	{
		// 链接仪器，新仪器右指针指向路由尾仪器
		pInstrument->m_pCrossTop = pRout->m_pTail;
		pInstrument->m_uiSNCrossTop = pRout->m_pTail->m_uiSN;
		pInstrument->m_iIndexCrossTop = pRout->m_pTail->m_uiIndex;

		// 链接仪器，路由尾仪器左指针指向新仪器
		pRout->m_pTail->m_pCrossDown = pInstrument;
		pRout->m_pTail->m_uiSNCrossDown = pInstrument->m_uiSN;
		pRout->m_pTail->m_iIndexCrossDown = pInstrument->m_uiIndex;

		// 设置路由对象尾指针
		pRout->m_pTail = pInstrument;
		pRout->m_uiSNTail = pInstrument->m_uiSN;
		pRout->m_iIndexTail = pInstrument->m_uiIndex;
	}
}

/**
* 增加交叉站；测线方向，左方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddCrossLeft(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreeCrossIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	// 设置右行方向路由地址
	pInstrument->m_uiRoutIPRight = 0xFFFF;

	// 增加交叉站测线方向，左方
	AddCollectLeft(pInstrument);
}

/**
* 增加交叉站；测线方向，右方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddCrossRight(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreeCrossIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	// 设置左行方向路由地址
	pInstrument->m_uiRoutIPLeft = 0xFFFF;

	// 增加电源站测线方向，右方
	AddCollectRight(pInstrument);	
}

/**
* 增加电源站；测线方向，左方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddPowerLeft(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreePowerIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	// 增加电源站测线方向，左方
	AddCollectLeft(pInstrument);
}

/**
* 增加电源站；测线方向，右方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddPowerRight(CInstrument* pInstrument)
{
	// 为新仪器分配IP地址
	pInstrument->m_uiIP = m_pSiteData->m_oIPList.GetFreePowerIP();
	// 新仪器加入IP索引表
	m_pSiteData->m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);

	// 增加电源站测线方向，右方
	AddCollectRight(pInstrument);	
}

/**
* 增加采集站；测线方向，左方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddCollectLeft(CInstrument* pInstrument)
{
	CRout* pRout = NULL;
	// 得到路由对象
	m_pSiteData->m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout);
	// 更新路由对象的路由时间
	pRout->UpdateRoutTime();

	if(NULL == pRout->m_pTail)	// 交叉站路由方向没有链接仪器
	{
		// 链接仪器，新仪器右指针指向路由头仪器
		pInstrument->m_pInstrumentRight = pRout->m_pHead;
		pInstrument->m_uiSNInstrumentRight = pRout->m_pHead->m_uiSN;
		pInstrument->m_iIndexInstrumentRight = pRout->m_pHead->m_uiIndex;

		// 链接仪器，路由头仪器左指针指向新仪器
		pRout->m_pHead->m_pInstrumentLeft = pInstrument;
		pRout->m_pHead->m_uiSNInstrumentLeft = pInstrument->m_uiSN;
		pRout->m_pHead->m_iIndexInstrumentLeft = pInstrument->m_uiIndex;

		// 设置路由对象尾指针
		pRout->m_pTail = pInstrument;
		pRout->m_uiSNTail = pInstrument->m_uiSN;
		pRout->m_iIndexTail = pInstrument->m_uiIndex;
	}
	else	// 交叉站路由方向了链接仪器
	{
		// 判断：仪器首包时刻大于路由链尾仪器首包时刻		
		if(pInstrument->m_uiTimeHeadFrame > pRout->m_pTail->m_uiTimeHeadFrame)	// 新仪器加在路由链的链尾
		{
			// 链接仪器，新仪器右指针指向路由尾仪器
			pInstrument->m_pInstrumentRight = pRout->m_pTail;
			pInstrument->m_uiSNInstrumentRight = pRout->m_pTail->m_uiSN;
			pInstrument->m_iIndexInstrumentRight = pRout->m_pTail->m_uiIndex;
			TRACE3("左增加仪器尾 SN=%d IP=%d TimeHead=%x\r\n",pInstrument->m_uiSN, pInstrument->m_uiIP,pInstrument->m_uiTimeHeadFrame); 
			
			// 链接仪器，路由尾仪器左指针指向新仪器
			pRout->m_pTail->m_pInstrumentLeft = pInstrument;
			pRout->m_pTail->m_uiSNInstrumentLeft = pInstrument->m_uiSN;
			pRout->m_pTail->m_iIndexInstrumentLeft = pInstrument->m_uiIndex;

			// 设置路由对象尾指针
			pRout->m_pTail = pInstrument;
			pRout->m_uiSNTail = pInstrument->m_uiSN;
			pRout->m_iIndexTail = pInstrument->m_uiIndex;
		}
		// 判断：仪器首包时刻小于路由链尾仪器首包时刻
		else
		{
			// 从路由链尾仪器开始，找到首包时刻小于仪器首包时刻的仪器
			// 起点为尾仪器右边第一个仪器
			CInstrument* pInstrumentTemp = pRout->m_pTail->m_pInstrumentRight;
			// 从尾部向头部找
			// 新仪器首包时刻小于被比较仪器
			while(pInstrument->m_uiTimeHeadFrame < pInstrumentTemp->m_uiTimeHeadFrame)
			{
				// 向头部方向得到下一个仪器
				pInstrumentTemp = pInstrumentTemp->m_pInstrumentRight;
			}
			// 找到仪器的首包时刻小于新仪器，新仪器插入在找到仪器的左边
			// 链接仪器，新仪器右指针指向找到仪器
			pInstrument->m_pInstrumentRight = pInstrumentTemp;
			pInstrument->m_uiSNInstrumentRight = pInstrumentTemp->m_uiSN;
			pInstrument->m_iIndexInstrumentRight = pInstrumentTemp->m_uiIndex;

			// 链接仪器，新仪器左指针指向找到仪器的左指针
			pInstrument->m_pInstrumentLeft = pInstrumentTemp->m_pInstrumentLeft;
			pInstrument->m_uiSNInstrumentLeft = pInstrumentTemp->m_pInstrumentLeft->m_uiSN;
			pInstrument->m_iIndexInstrumentLeft = pInstrumentTemp->m_pInstrumentLeft->m_uiIndex;

			// 链接仪器，新仪器左边仪器的右指针指向新仪器
			pInstrumentTemp->m_pInstrumentLeft->m_pInstrumentRight = pInstrument;
			pInstrumentTemp->m_pInstrumentLeft->m_uiSNInstrumentRight = pInstrument->m_uiSN;
			pInstrumentTemp->m_pInstrumentLeft->m_iIndexInstrumentRight = pInstrument->m_uiIndex;

			TRACE3("左插入仪器中 SN=%d IP=%d TimeHead=%x\r\n",pInstrument->m_uiSN, pInstrument->m_uiIP,pInstrument->m_uiTimeHeadFrame); 
			pInstrumentTemp->m_pInstrumentLeft = pInstrument;
			pInstrumentTemp->m_uiSNInstrumentLeft = pInstrument->m_uiSN;
			pInstrumentTemp->m_iIndexInstrumentLeft = pInstrument->m_uiIndex;
		}
	}	
}

/**
* 增加采集站；测线方向，右方
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddCollectRight(CInstrument* pInstrument)
{
	CRout* pRout = NULL;
	// 得到路由对象
	m_pSiteData->m_oRoutIPRoutMap.GetRout(pInstrument->m_uiRoutIP, pRout);
	// 更新路由对象的路由时间
	pRout->UpdateRoutTime();

	if(NULL == pRout->m_pTail)	// 交叉站路由方向没有链接仪器
	{
		// 链接仪器，新仪器左指针指向路由头仪器
		pInstrument->m_pInstrumentLeft = pRout->m_pHead;
		pInstrument->m_uiSNInstrumentLeft = pRout->m_pHead->m_uiSN;
		pInstrument->m_iIndexInstrumentLeft = pRout->m_pHead->m_uiIndex;

		// 链接仪器，路由头仪器右指针指向新仪器
		pRout->m_pHead->m_pInstrumentRight = pInstrument;
		pRout->m_pHead->m_uiSNInstrumentRight = pInstrument->m_uiSN;
		pRout->m_pHead->m_iIndexInstrumentRight = pInstrument->m_uiIndex;

		// 设置路由对象尾指针
		pRout->m_pTail = pInstrument;
		pRout->m_uiSNTail = pInstrument->m_uiSN;
		pRout->m_iIndexTail = pInstrument->m_uiIndex;
	}
	else	// 交叉站路由方向了链接仪器
	{
		if(pInstrument->m_uiTimeHeadFrame > pRout->m_pTail->m_uiTimeHeadFrame)	// 仪器首包时刻大于路由链尾仪器首包时刻
		{
			// 链接仪器，新仪器左指针指向路由尾仪器
			TRACE3("右增加仪器尾 SN=%d IP=%d TimeHead=%x\r\n",pInstrument->m_uiSN, pInstrument->m_uiIP,pInstrument->m_uiTimeHeadFrame); 
			pInstrument->m_pInstrumentLeft = pRout->m_pTail;
			pInstrument->m_uiSNInstrumentLeft = pRout->m_pTail->m_uiSN;
			pInstrument->m_iIndexInstrumentLeft = pRout->m_pTail->m_uiIndex;

			// 链接仪器，路由尾仪器右指针指向新仪器
			pRout->m_pTail->m_pInstrumentRight = pInstrument;
			pRout->m_pTail->m_uiSNInstrumentRight = pInstrument->m_uiSN;
			pRout->m_pTail->m_iIndexInstrumentRight = pInstrument->m_uiIndex;

			// 设置路由对象尾指针
			pRout->m_pTail = pInstrument;
			pRout->m_uiSNTail = pInstrument->m_uiSN;
			pRout->m_iIndexTail = pInstrument->m_uiIndex;
		}
		else	// 仪器首包时刻小于路由链尾仪器首包时刻
		{
			// 从路由链尾仪器开始，找到首包时刻小于仪器首包时刻的仪器
			// 起点为尾仪器左边第一个仪器
			CInstrument* pInstrumentTemp = pRout->m_pTail->m_pInstrumentLeft;
			// 从尾部向头部找
			// 新仪器首包时刻小于被比较仪器
			while(pInstrument->m_uiTimeHeadFrame < pInstrumentTemp->m_uiTimeHeadFrame)
			{
				// 向头部方向得到下一个仪器
				pInstrumentTemp = pInstrumentTemp->m_pInstrumentLeft;
			}
			// 找到仪器的首包时刻小于新仪器，新仪器插入在找到仪器的右边
			// 链接仪器，新仪器左指针指向找到仪器
			pInstrument->m_pInstrumentLeft = pInstrumentTemp;
			pInstrument->m_uiSNInstrumentLeft = pInstrumentTemp->m_uiSN;
			pInstrument->m_iIndexInstrumentLeft = pInstrumentTemp->m_uiIndex;

			// 链接仪器，新仪器右指针指向找到仪器的右指针
			pInstrument->m_pInstrumentRight = pInstrumentTemp->m_pInstrumentRight;
			pInstrument->m_uiSNInstrumentRight = pInstrumentTemp->m_pInstrumentRight->m_uiSN;
			pInstrument->m_iIndexInstrumentRight = pInstrumentTemp->m_pInstrumentRight->m_uiIndex;

			// 链接仪器，新仪器右边仪器的左指针指向新仪器
			pInstrumentTemp->m_pInstrumentRight->m_pInstrumentLeft = pInstrument;
			pInstrumentTemp->m_pInstrumentRight->m_uiSNInstrumentLeft = pInstrument->m_uiSN;
			pInstrumentTemp->m_pInstrumentRight->m_iIndexInstrumentLeft = pInstrument->m_uiIndex;

			// 链接仪器，找到仪器的右指针指向新仪器
			TRACE3("右插入仪器中 SN=%d IP=%d TimeHead=%x\r\n",pInstrument->m_uiSN, pInstrument->m_uiIP,pInstrument->m_uiTimeHeadFrame); 
			pInstrumentTemp->m_pInstrumentRight = pInstrument;
			pInstrumentTemp->m_uiSNInstrumentRight = pInstrument->m_uiSN;
			pInstrumentTemp->m_iIndexInstrumentRight = pInstrument->m_uiIndex;
		}
	}	
}

/**
* 设置交叉站某个方向的路由
* @param CInstrument* &pInstrument 仪器指针
* @param unsigned int uiRoutDirection 路由方向
* @return void
*/
void CThreadProcHeadFrame::SetCrossRout(CInstrument* pInstrument, unsigned int uiRoutDirection)
{
	CRout* pRout = NULL;
	// 得到空闲路由对象
	pRout = m_pSiteData->m_oRoutList.GetFreeRout();

	// 判断方向，设置仪器路由地址
	switch(uiRoutDirection)
	{
	case 1:	// 上
		{
			pInstrument->m_uiRoutIPTop = pRout->m_uiRoutIP;
			break;
		}
	case 2:	// 下
		{
			pInstrument->m_uiRoutIPDown = pRout->m_uiRoutIP;
			break;
		}
	case 3:	// 左
		{
			pInstrument->m_uiRoutIPLeft = pRout->m_uiRoutIP;
			break;
		}
	case 4:	// 右
		{
			pInstrument->m_uiRoutIPRight = pRout->m_uiRoutIP;
			break;
		}
	}

	// 设置交叉站路由方向
	pRout->m_uiRoutDirection = uiRoutDirection;
	// 设置路由头
	pRout->m_pHead = pInstrument;
	pRout->m_uiSNHead = pInstrument->m_uiSN;
	pRout->m_iIndexHead = pInstrument->m_uiIndex;
	// 设置路由尾
	pRout->m_pTail = NULL;
	// 更新路由对象的路由时间
	pRout->UpdateRoutTime();
	// 路由对象加入路由索引表
	m_pSiteData->m_oRoutIPRoutMap.AddRout(pRout->m_uiRoutIP, pRout);
}

/**
* 增加一个需要设置IP地址的仪器
* @note 加入IP地址设置线程的待设置IP地址仪器队列
* @param CInstrument* &pInstrument 仪器指针
* @return void
*/
void CThreadProcHeadFrame::AddInstrumentForIPSet(CInstrument* pInstrument)
{
	if(pInstrument->m_uiIP > 0)	//仪器被分配了IP地址
	{
		// 加入待设置IP地址仪器队列
		m_pThreadProcIPSet->AddInstrumentForIPSet(pInstrument);
	}
}

/**
* 增加一个需要设置IP地址的仪器
* @note 加入IP地址设置线程的待设置IP地址仪器队列
* @param CFrameHead* pFrameHead 首包帧指针
* @return void
*/
void CThreadProcHeadFrame::AddInstrumentForIPSet(CFrameHead* pFrameHead)
{
	// 仪器指针
	CInstrument* pInstrument = NULL;
	// 由仪器SN号得到仪器
	if(TRUE == m_pSiteData->m_oSNInstrumentMap.GetInstrument(pFrameHead->m_uiSN, pInstrument))
	{
		// 增加一个需要设置IP地址的仪器
		AddInstrumentForIPSet(pInstrument);
	}
}