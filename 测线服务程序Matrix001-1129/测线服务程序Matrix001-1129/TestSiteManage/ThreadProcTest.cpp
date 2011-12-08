// ThreadProcTest.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "ThreadProcTest.h"


// CThreadProcTest

IMPLEMENT_DYNCREATE(CThreadProcTest, CWinThread)

CThreadProcTest::CThreadProcTest()
{

}

CThreadProcTest::~CThreadProcTest()
{
}

BOOL CThreadProcTest::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadProcTest::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadProcTest, CWinThread)
END_MESSAGE_MAP()

// CThreadProcTest 消息处理程序

/**
* 线程主函数
* @param void
* @return int
*/
int CThreadProcTest::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		// 判断：关闭标志为真
		if(true == m_bClose)
		{
			break;
		}
		// 测试处理
		ProcTest();
		// 判断：关闭标志为真
		if(true == m_bClose)
		{
			break;
		}
		// 设置线程空闲
		m_bFree = true;
		// 休眠线程
		this->SuspendThread();
		// 设置线程忙
		m_bFree = false;
	}
	// 判断：处理方式 1-基本测试
	if(1 == m_iProcTestMethod)
	{
		// 设置测试线程线程关闭标志为真，基本测试
		m_pSiteData->m_bProcTestBaseClose = true;
	}
	// 判断：处理方式 2-噪声监测
	else if(2 == m_iProcTestMethod)
	{
		// 设置测试线程线程关闭标志为真，噪声监测
		m_pSiteData->m_bProcTestNoiseClose = true;
	}
	// 返回
	return CWinThread::Run();
}

/**
* 初始化线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::OnInit()
{
	// 非工作状态
	m_bWork = false;
	// 非关闭线程
	m_bClose = false;
	// 空闲状态
	m_bFree = true;
	// 非启动状态
	m_bSwitchON = false;
	// 处理新加检波器的初始测试
	m_bProcSensorTestOnField = false;
	
	// 初始化噪声监测数据网络发送计数
	m_uiTestNoiseNetSendCount = 0;
	// 初始化噪声监测数据网络发送步长
	m_uiTestNoiseNetSendStep = 1;
	
	//为绘图转发初始化帧计数
	FDUNum=0;
	for(int i=0;i<MaxFDUCount;i++)
		FrameNum[i]=0;

	irecevice=0;
	CString strDesc;
	m_pSiteData->m_oProcTestThreadID=GetCurrentThreadId();
	strDesc.Format("CThreadProcTest_id=%d",m_pSiteData->m_oProcTestThreadID);
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcTest", strDesc);

	char * tmp=strDesc.GetBuffer(strDesc.GetLength() + 1);
	OutputDebugString(tmp );

	return true;
}

/**
* 关闭线程
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::OnClose()
{
	// 关闭线程
	m_bClose = true;
	// 判断：线程空闲状态
	if(true == m_bFree)
	{
		// 启动处理
		ResumeProcTest();
	}
	return true;
}

/**
* 线程开始工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::OnWork()
{
	// 工作状态
	m_bWork = true;
	return true;
}

/**
* 线程停止工作
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::OnStop()
{
	// 非工作状态
	m_bWork = false;
	// 非启动状态
	m_bSwitchON = false;
	return true;
}

/**
* 判断线程是否可以进行处理工作的条件
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::JudgeProcCondition()
{	
	// 判断：非工作状态，或非启动状态
	if((false == m_bWork) || (false == m_bSwitchON))
	{		
		// 判断：处理方式 1-基本测试
		if(1 == m_iProcTestMethod)
		{
			// 设置测试线程线程停止标志为真，基本测试
			m_pSiteData->m_bProcTestBaseStop = true;
		}
		// 判断：处理方式 2-噪声监测
		else if(2 == m_iProcTestMethod)
		{
			// 设置测试线程线程停止标志为真，噪声监测
			m_pSiteData->m_bProcTestNoiseStop = true;
		}
		return false;
	}
	else
	{
		// 判断：处理方式 1-基本测试
		if(1 == m_iProcTestMethod)
		{
			// 设置测试线程线程停止标志为真，基本测试
			m_pSiteData->m_bProcTestBaseStop = false;
		}
		// 判断：处理方式 2-噪声监测
		else if(2 == m_iProcTestMethod)
		{
			// 设置测试线程线程停止标志为真，噪声监测
			m_pSiteData->m_bProcTestNoiseStop = false;
		}
	}
	return true;
}

/**
* 线程等待
* @param int iWaitStopCount 最多等待次数；每次50毫秒
* @return void
*/
void CThreadProcTest::Wait(int iWaitStopCount)
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
* 判断线程是否空闲
* @param void
* @return bool true：是；false：否
*/
bool CThreadProcTest::IsThreadFree()
{
	return m_bFree;
}

/**
* 锁定线程，不允许其他线程使用
* @param void
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::Lock()
{
	bool bLock = false;	
	m_pSiteData->m_oCriticalSectionProcTest.Lock();	// 为测试线程准备的线程同步对象锁定
		if(true == m_bFree)	// 删除线程空闲	
		{		
			m_bFree = false;	// 设置删除线程忙
			bLock = true;
		}		
	m_pSiteData->m_oCriticalSectionProcTest.Unlock();	// 为测试线程准备的线程同步对象解锁
	return bLock;
}

/**
* 解锁线程，允许其他线程使用
* @param void
* @return bool true：成功；false：失败
*/
void CThreadProcTest::Unlock()
{
	m_pSiteData->m_oCriticalSectionProcTest.Lock();	// 为测试线程准备的线程同步对象锁定		
		m_bFree = true;	// 设置删除线程空闲
	m_pSiteData->m_oCriticalSectionProcTest.Unlock();	// 为测试线程准备的线程同步对象解锁
}

/**
* 唤醒线程，启动处理
* @param void
* @return void
*/
void CThreadProcTest::ResumeProcTest()
{
	m_bFree = false;	// 设置删除线程忙
	m_bSwitchON = true;	// 启动状态
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
* 测试处理
* @param void
* @return void
*/
void CThreadProcTest::ProcTest()
{
	CString strDesc;
	strDesc.Format("Test Project Start.Method:%d;Type:%d;Nb:%d", m_iProcTestMethod, m_pTestProject->m_uiProjectType, m_pTestProject->m_uiTestNb);
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcTest", strDesc);

	// 循环；直到测试请求队列为空
	while(false == m_pTestProject->m_olsTestRequest.IsEmpty())
	{
		m_pSocketTestDataFrame->m_irevFrameCount=0;
		m_pSocketTestDataFrame->m_iFrameCount=0;

		m_pSocketTestSetFrame->strrecv.Empty(); 
		m_pSocketTestSetFrame->adgetcount=0;
		m_pSocketTestSetFrame->adsetcount=0;
		m_pSocketTestSetFrame->gettestcount=0;
		m_pSocketTestSetFrame->getTBcount=0;
		m_pSocketTestSetFrame->resetcheckrecv();

		FDUNum=0;
		for(int i=0;i<MaxFDUCount;i++)
			FrameNum[i]=0;
		ViewBuf=(byte *)malloc(MaxFDUCount*FrameLength);

		// 停止正在进行数据采集的仪器
		StopInstrumentWhichIsCollecting();

		// 得到测试请求
		m_pTestRequest = m_pTestProject->m_olsTestRequest.RemoveHead();
		// 根据测试类型，得到测试基本设置指针
		m_pTestBaseData = m_pTestProject->GetTestBaseDataByTestType(m_pTestRequest->m_uiType);
		// 设置噪声监测数据网络发送步长
		SetTestNoiseNetSendStep();

		strDesc.Format("Test Request Start.Nb=%d msLength=%d,Aim:%d;Type:%d,m_uiADTimeBegin=%d m_uiTimeCollectEnd=%d",m_pTestProject->m_uiTestNb,m_pTestProject->m_uiRecordLength, m_pTestRequest->m_uiAim, m_pTestRequest->m_uiType,m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin,m_pTestRequest->m_uiTimeCollectEnd);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcTest", strDesc);
		//m_pTestProject->m_uiRecordResult=1;
		// 测试处理，一次测试请求，开始
		ProcTestRequestStart();
		strDesc.Format("Test Request Start1.Aim:%d;Type:%d,m_uiADTimeBegin=%d m_uiTimeCollectEnd=%d", m_pTestRequest->m_uiAim, m_pTestRequest->m_uiType,m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin,m_pTestRequest->m_uiTimeCollectEnd);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcTest", strDesc);
		// 测试处理，一次测试请求，处理
		ProcTestRequest();
		// 测试处理，一次测试请求，结束
		ProcTestRequestEnd();

		strDesc.Format("Test Request End");
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcTest", strDesc);
		
		free(ViewBuf);
		// 判断；不能继续处理
		if(false == JudgeProcCondition())
		{
			// 停止正在进行数据采集的仪器
			StopInstrumentWhichIsCollecting();
			Sleep(100);
			break;
		}
		// 得到测试请求之间的等待时间，每次测试之间的间隔（毫秒）
		int iWaitStopCount = m_pTestProject->m_uiDelayBetweenTest / 50;
		// 等待
		Wait(iWaitStopCount);
		// 判断；不能继续处理
		if(false == JudgeProcCondition())
		{
			// 停止正在进行数据采集的仪器
			StopInstrumentWhichIsCollecting();
			Sleep(100);
			break;
		}
		// 停止正在进行数据采集的仪器
		StopInstrumentWhichIsCollecting();
		Sleep(100);
		CString strDesc;
		strDesc.Format("totalcount=%d recevicecount=%d saveecount=%d,lossframecount=%d",totalcount,recevicecount,saveecount,lossframecount);
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "total", strDesc);
		TRACE3("recevicecount=%d saveecount=%d,lossframecount=%d\r\n",recevicecount,saveecount,lossframecount);
		m_pSiteData->m_oSocketTestDataFrame.Lock();
		TRACE3("AD adsetcount=%d getTBcount=%d gettestcout=%d\r\n",m_pSocketTestSetFrame->adsetcount,m_pSocketTestSetFrame->getTBcount,m_pSocketTestSetFrame->gettestcount);
		strDesc.Format("AD adsetcount=%d getTBcount=%d gettestcout=%d",m_pSocketTestSetFrame->adsetcount,m_pSocketTestSetFrame->getTBcount,m_pSocketTestSetFrame->gettestcount);
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "totalrecv", strDesc);
		m_pSiteData->m_oSocketTestDataFrame.Unlock();
		TRACE("%s",m_pSocketTestSetFrame->strrecv);

		TRACE1("一次测试请求完成。测试类型：%d\r\n", m_pTestRequest->m_uiType);
	}

	// 发送测试结果，基本测试-新加检波器的初始测试
	SendTestResultBaseForOnFieldTest();
	// 更新上次现场数据变化时刻，基本测试
	UpdateSiteDataChangedTimeForTestData();

	// 处理新加检波器的初始测试
	m_bProcSensorTestOnField = false;
	strDesc.Format("Test Project End");
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcTest", strDesc);
}

/**
* 测试处理，一次测试请求，开始
* @param void
* @return void
*/
void CThreadProcTest::ProcTestRequestStart()
{
	// 重置，为一次新的测试请求
	m_pSocketTestSetFrame->m_pTestElementList->ResetForNewTestRequest();
	// 判断：基本测试
	if(1 == m_iProcTestMethod)
	{
		m_pLogicData->SetTestElementListLimit(m_pTestRequest->m_uiType);
	}
	// 重置，为一次新的测试请求
	m_pTestDataBuffer->ResetForNewTestRequest(); 

	// 打开测试数据文件
	FileTestDataOpen();
	lossframecount=0;
	recevicecount=0;
	saveecount=0;
	totalcount=0;
}

/**
* 测试处理，一次测试请求，处理
* @param void
* @return void
*/
void CThreadProcTest::ProcTestRequest()
{
	m_pADCDataSaveToFile.OnOpenADCSaveFile(m_pSocketTestSetFrame->m_uiIPCount);
	//设置AD参数,代替ProcSendTestSetFrame
	ADSet();
	// 处理发送测试设置帧
	//ProcSendTestSetFrame();
	// 等待
//	Wait(2);		//等待AD命令发送完毕
	// 处理发送测试开始帧
	//ProcSendTestStartFrame();
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
		m_pSiteData->m_bProcTimeDelayON_stop = false;	// 仪器时延处理线程停止
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();

	ProcSendTestADStart();

	Wait(1);		//等待AD命令发送完毕
	CString strDesc;
	m_pSiteData->m_oSocketTestDataFrame.Lock();
	TRACE3("AD1 adsetcount1=%d getTBcount=%d gettestcout=%d\r\n",m_pSocketTestSetFrame->adsetcount,m_pSocketTestSetFrame->getTBcount,m_pSocketTestSetFrame->gettestcount);
	strDesc.Format("AD1 adsetcount1=%d getTBcount=%d gettestcout=%d",m_pSocketTestSetFrame->adsetcount,m_pSocketTestSetFrame->getTBcount,m_pSocketTestSetFrame->gettestcount);
	m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "totalrecv", strDesc);
	m_pSiteData->m_oSocketTestDataFrame.Unlock();
	
	// 得到并处理测试数据
	GetAndProcTestData();
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
		m_pSiteData->m_bProcTimeDelayON_stop = true;	// 仪器时延处理线程工作
		m_pSiteData->iDelayfrist=0;
	//m_pSiteData->m_bProcTimeDelayON_stop = false;	// 暂时只对一次时。仪器时延处理线程停止
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
	m_pADCDataSaveToFile.OnCloseADCSaveFile();
}

/**
* 测试处理，一次测试请求，结束
* @param void
* @return void
*/
void CThreadProcTest::ProcTestRequestEnd()
{
	// 关闭测试数据文件
	FileTestDataClose();
}

/**
* 发送测试结果，基本测试-新加检波器的初始测试
* @param void
* @return void
*/
void CThreadProcTest::SendTestResultBaseForOnFieldTest()
{
	// 判断：非处理新加检波器的初始测试	
	if(false == m_bProcSensorTestOnField)
	{
		return;
	}
	// 判断：处理方式 2-噪声监测
	if(2 == m_iProcTestMethod)
	{
		return;
	}
	// 判断：不继续处理
	if(false == JudgeProcCondition())
	{
		return;
	}

	m_pTestRequest->m_uiType = 45;	// 新加检波器的初始测试
	// 发送测试结果，基本测试
	SendTestResultBase();
}

/**
* 更新上次现场数据变化时刻，基本测试
* @param void
* @return void
*/
void CThreadProcTest::UpdateSiteDataChangedTimeForTestData()
{
	// 判断：处理方式 2-噪声监测
	if(2 == m_iProcTestMethod)
	{
		return;
	}
	// 判断：不继续处理
	if(false == JudgeProcCondition())
	{
		return;
	}

	// 更新上次现场数据变化时刻
	m_pSiteData->UpdateSiteDataChangedTime();
	m_pSiteData->m_bTestDataChanged = true;
}

/**
* 打开测试数据文件
* @param void
* @return void
*/
void CThreadProcTest::FileTestDataOpen()
{
	// 判断：不记录测试数据
	if(0 == m_pTestProject->m_uiRecordResult)
	{
		return;
	}

	CString strFileName;
	CString strDate;
	CString strTime;
	SYSTEMTIME oSystemTime;

	// 得到本地时间
	GetLocalTime(&oSystemTime);
	// 日期
	strDate.Format("%d-%d-%d", oSystemTime.wYear, oSystemTime.wMonth, oSystemTime.wDay);
	// 时间
	strTime.Format("%d %d %d", oSystemTime.wHour, oSystemTime.wMinute, oSystemTime.wSecond);

	// 设置测试数据文件名
	strFileName.Format("TestData%d-%d %s %s.dat", m_pTestProject->m_uiTestNb, m_pTestRequest->m_uiType, strDate, strTime);
	BOOL bData = m_oFileTestData.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
}

/**
* 写测试数据文件
* @param byte* pBlockData 数据指针
* @param unsigned int uiSize 数据长度
* @return void
*/
void CThreadProcTest::FileTestDataWrite(byte* pBlockData, unsigned int uiSize)
{
	// 判断：不记录测试数据
	if(0 == m_pTestProject->m_uiRecordResult)
	{
		return;
	}

	m_oFileTestData.Write(pBlockData, uiSize);

}

/**
* 关闭测试数据文件
* @param void
* @return void
*/
void CThreadProcTest::FileTestDataClose()
{
	// 判断：不记录测试数据
	if(0 == m_pTestProject->m_uiRecordResult)
	{
		return;
	}

	m_oFileTestData.Close();
}

/**
* 处理来自测试设置的基本测试任务
* @param unsigned int uiTestNb 测试号
* @return void
*/
bool CThreadProcTest::OnTestTaskBaseFromTestSetup(unsigned int uiTestNb)
{	
	// 判断：线程工作中
	if(false == Lock())
	{
		return false;
	}
	// 设置基本测试项目，来自测试设置的测试任务
	if(false == m_pLogicData->OnSetupTestProjectForTestBaseFromTestSetup(uiTestNb))
	{
		Unlock();
		return false;
	}
	m_pTestProject = &m_pLogicData->m_oTestSetupData.m_oTestProjectBase;	// 测试项目队列对象
	m_pSocketTestSetFrame->m_pTestElementList = &m_pLogicData->m_oTestSetupData.m_oTestElementListBase;	// 测试单元队列
	m_pSocketTestSetFrame->m_uiIPCount = m_pSocketTestSetFrame->m_pTestElementList->m_uiCountUsed;	// 设置参加测试IP地址数量
	m_pTestDataBuffer = &m_pLogicData->m_oTestSetupData.m_oTestDataBufferBase;	// 测试数据缓冲区
	m_bProcSensorTestOnField = false;	// 处理新加检波器的初始测试

	CString strDesc;
	strDesc.Format("Test Project Start.From TestSetup Go Button.Method:%d;Type:%d;Nb:%d", m_iProcTestMethod, m_pTestProject->m_uiProjectType, m_pTestProject->m_uiTestNb);
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "OnTestTaskBaseFromTestSetup", strDesc);

	// 启动处理
	ResumeProcTest();
	return true;
}

/**
* 处理来自测试设置的噪声监测任务
* @param byte byTestOperation 操作 1-启动测试；0-停止测试
* @return void
*/
bool CThreadProcTest::OnTestTaskNoise(byte byTestOperation)
{	
	// 判断：0-停止测试
	if(0 == byTestOperation)
	{
		// 判断：线程空闲
		if(true == IsThreadFree())
		{
			return true;
		}		

		CString strDesc;
		strDesc.Format("Stop SeisMonitor Test.From SeisMonitorView SeisMonitor Button");
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "OnTestTaskNoise", strDesc);

		m_bSwitchON = false;	// 停止状态
		return true;
	}
	// 判断：1-启动测试
	else if(1 == byTestOperation)
	{	
		// 判断：线程工作中
		if(false == IsThreadFree())
		{
			return true;
		}
		// 设置噪声测试项目，来自测试设置的测试任务
		if(false == m_pLogicData->OnSetupTestProjectForTestNoiseFromTestSetup())
		{
			return false;
		}
		m_pTestProject = &m_pLogicData->m_oTestSetupData.m_oTestProjectNoise;	// 测试项目队列对象
		m_pSocketTestSetFrame->m_pTestElementList = &m_pLogicData->m_oTestSetupData.m_oTestElementListNoise;	// 测试单元队列
		m_pSocketTestSetFrame->m_uiIPCount = m_pSocketTestSetFrame->m_pTestElementList->m_uiCountUsed;	// 设置参加测试IP地址数量
		m_pTestDataBuffer = &m_pLogicData->m_oTestSetupData.m_oTestDataBufferNoise;	// 测试数据缓冲区

		CString strDesc;
		strDesc.Format("Start SeisMonitor Test.From SeisMonitorTestSetup Go Button");
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "OnTestTaskNoise", strDesc);

		// 启动处理
		ResumeProcTest();
		return true;
	}
	return true;
}

/**
* 处理来自测试设置的噪声监测任务
* @param unsigned CString strTestAbsoluteSpread 绝对排列
* @return void
*/
bool CThreadProcTest::OnTestTaskNoise(CString strTestAbsoluteSpread)
{
	// 判断：线程工作中
	if(false == IsThreadFree())
	{
		return true;
	}
	// 设置噪声测试项目，来自噪声监测视图的测试任务
	if(false == m_pLogicData->OnSetupTestProjectForTestNoiseFromTestView(strTestAbsoluteSpread))
	{
		return false;
	}
	m_pTestProject = &m_pLogicData->m_oTestSetupData.m_oTestProjectNoise;	// 测试项目队列对象
	m_pSocketTestSetFrame->m_pTestElementList = &m_pLogicData->m_oTestSetupData.m_oTestElementListNoise;	// 测试单元队列
	m_pSocketTestSetFrame->m_uiIPCount = m_pSocketTestSetFrame->m_pTestElementList->m_uiCountUsed;	// 设置参加测试IP地址数量
	m_pTestDataBuffer = &m_pLogicData->m_oTestSetupData.m_oTestDataBufferNoise;	// 测试数据缓冲区

	CString strDesc;
	strDesc.Format("Start SeisMonitor Test.From SeisMonitorView SeisMonitor Button");
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "OnTestTaskNoise", strDesc);

	// 启动处理
	ResumeProcTest();
	return true;
}

/**
* 处理来自测试视图的基本测试任务
* @param unsigned int uiTestAim 测试目标
* @param unsigned unsigned int uiTestType 测试类型 1-仪器测试；2-检波器测试
* @param unsigned CString strTestAbsoluteSpread 绝对排列
* @return bool true：成功；false：失败
*/
bool CThreadProcTest::OnTestTaskBaseFromTestView(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread)
{	
	// 判断：线程工作中
	if(false == Lock())
	{
		return false;
	}
	// 设置基本测试项目，来自测试图形的测试任务
	if(false == m_pLogicData->OnSetupTestProjectForTestBaseFromTestView(uiTestAim, uiTestType, strTestAbsoluteSpread))
	{
		Unlock();
		return false;
	}
	m_pTestProject = &m_pLogicData->m_oTestSetupData.m_oTestProjectBase;	// 测试项目队列对象
	m_pSocketTestSetFrame->m_pTestElementList = &m_pLogicData->m_oTestSetupData.m_oTestElementListBase;	// 测试单元队列
	m_pSocketTestSetFrame->m_uiIPCount = m_pSocketTestSetFrame->m_pTestElementList->m_uiCountUsed;	// 设置参加测试IP地址数量
	m_pTestDataBuffer = &m_pLogicData->m_oTestSetupData.m_oTestDataBufferBase;	// 测试数据缓冲区
	m_bProcSensorTestOnField = false;	// 处理新加检波器的初始测试

	CString strDesc;
	strDesc.Format("Start Instrument/Sensor Test.From Instrument/SensorView Go Button");
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "OnTestTaskBaseFromTestView", strDesc);

	// 启动处理
	ResumeProcTest();
	return true;
}

/**
* 处理新加检波器的初始测试
* @param void
* @return void
*/
void CThreadProcTest::ProcSensorTestOnField()
{	
	// 判断：线程工作中
	if(false == Lock())
	{
		return;
	}
	// 设置基本测试项目，处理新加检波器的初始测试
	if(false == m_pLogicData->OnSetupTestProjectForSensorTestOnField())
	{
		Unlock();
		return;
	}
	m_pTestProject = &m_pLogicData->m_oTestSetupData.m_oTestProjectBase;	// 测试项目队列对象
	m_pSocketTestSetFrame->m_pTestElementList = &m_pLogicData->m_oTestSetupData.m_oTestElementListBase;	// 测试单元队列
	m_pSocketTestSetFrame->m_uiIPCount = m_pSocketTestSetFrame->m_pTestElementList->m_uiCountUsed;	// 设置参加测试IP地址数量
	m_pTestDataBuffer = &m_pLogicData->m_oTestSetupData.m_oTestDataBufferBase;	// 测试数据缓冲区
	m_bProcSensorTestOnField = true;	// 处理新加检波器的初始测试	

	CString strDesc;
	strDesc.Format("Start Sensor OnField Test");
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcSensorTestOnField", strDesc);

	// 启动处理
	ResumeProcTest();
}

/**
* 处理发送测试设置帧
* @param void
* @return void
*/
void CThreadProcTest::ProcSendTestSetFrame()
{
	// 设置采集命令监听端口
	//m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen = m_pSocketTestDataFrame->m_uiPortForTestDataFrame;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen =(unsigned short)m_pSocketTestSetFrame->m_uiPortForTestSet;

	CTestElementData* pTestElementData = NULL;	// 测试单元
	for(unsigned int i = 0; i < m_pSocketTestSetFrame->m_uiIPCount; i++)
	{
		// 得到一个单元
		pTestElementData = m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataByIndex(i);
		// 设置目标IP地址
		m_pSocketTestSetFrame->m_oFrameTestSet.m_uiIPAim = pTestElementData->m_uiIP;
		// 生成测试设置帧
		m_pSocketTestSetFrame->MakeFrameDataForTestSet();
		// 发送测试设置帧
		m_pSocketTestSetFrame->SendTestSetFrame();
	}
}

/**
* 测试AD参数设置
* @param void
* @return void
*/
void CThreadProcTest::ADSet()
{
	// 设置采集命令监听端口
	//m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen = m_pSocketTestDataFrame->m_uiPortForTestDataFrame;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen =(unsigned short)m_pSocketTestSetFrame->m_uiPortForTestSet;
	// 设置数据采集输出的目的端口
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortDataOut = (unsigned short)m_pSocketTestDataFrame->m_uiPortForTestDataFrame;

		//设置采样速率
	m_pSocketTestSetFrame->m_uiSamplingRate=m_pTestBaseData->m_uiSamplingRate;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiSamplingRate = m_pTestBaseData->m_uiSamplingRate;
//	CTestElementData* pTestElementData = NULL;	// 测试单元
	//for(unsigned int i = 0; i < m_pSocketTestSetFrame->m_uiIPCount; i++)
	//广播
// 	for(unsigned int i = 0; i < 1; i++)
// 	{
// 		// 得到一个单元
// 		pTestElementData = m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataByIndex(i);
// 		// 设置目标IP地址
// 		m_pSocketTestSetFrame->m_oFrameTestSet.m_uiIPAim = pTestElementData->m_uiIP;
// 		m_pSocketTestSetFrame->ofccheck=pTestElementData->ofccheck;
// 		// 生成测试设置帧
// 		m_pSocketTestSetFrame->MakeFrameADSet();
// 		TRACE1("AD SET IP=%x\r\n", pTestElementData->m_uiIP);
// 		//m_pSocketTestSetFrame->MakeFrameDataForTestSet();
// 		// 发送测试设置帧
// 		//m_pSocketTestSetFrame->SendTestSetFrame();
// 	}
	// 生成测试设置帧
//	m_pSocketTestSetFrame->MakeFrameADSet();
	// ADC参数设置
	m_pSocketTestSetFrame->OnADCSet();
	// ADC零漂校正
	m_pSocketTestSetFrame->OnADCZeroDrift();
}

/**
* 处理发送测试开始帧
* @param void
* @return void
*/
void CThreadProcTest::ProcSendTestStartFrame()
{
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
		// 得到野外设备系统时间
		unsigned int uiTimeNow = m_pSiteData->m_uiSystemTimeSite + 1000;
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
	
	m_pTestRequest->m_uiTimeADBegin = uiTimeNow + 1000;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin = m_pTestRequest->m_uiTimeADBegin;	// AD采集开始时刻
	m_pTestRequest->m_uiTimeCollectBegin = m_pTestRequest->m_uiTimeADBegin + 500;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeBegin = m_pTestRequest->m_uiTimeCollectBegin;	// 采集开始时刻	

	// 得到采样时间
	unsigned int uiCollectTime = m_pTestBaseData->m_uiSamplingLength;
	// 判断：记录测试数据，且记录时间大于采样时间
	if((1 == m_pTestProject->m_uiRecordResult) && (m_pTestProject->m_uiRecordLength > uiCollectTime))
	{
		// 采样时间设为记录时间
		uiCollectTime = m_pTestProject->m_uiRecordLength;
	}
	// 得到附加测试时间
	unsigned int uiCollectTimeTail = FrameADCount * 1000 / m_pTestBaseData->m_uiSamplingRate;
	// 判断：有小数位
	if(0 < ((FrameADCount * 1000) % m_pTestBaseData->m_uiSamplingRate))
	{
		//附加测试时间增加1毫秒
		uiCollectTimeTail++;
	}
	// 得到采样时间
	uiCollectTime = uiCollectTime + uiCollectTimeTail;
	// 判断：测试项目为野外噪声监测
	if(4000 == m_pTestProject->m_uiTestNb)	// 野外噪声监测
	{
		// 最长240小时连续监测
		uiCollectTime = 864000000;	// 240小时
	}
	// 得到采样结束时间
	m_pTestRequest->m_uiTimeCollectEnd = m_pTestRequest->m_uiTimeCollectBegin + uiCollectTime;
	// 设置采样结束时间
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeEnd = m_pTestRequest->m_uiTimeCollectEnd;
	// 设置采集命令监听端口
	//m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen = m_pSocketTestDataFrame->m_uiPortForTestDataFrame;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen =(unsigned short)m_pSocketTestSetFrame->m_uiPortForTestSet;
	// 设置数据采集输出的目的端口
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortDataOut = (unsigned short)m_pSocketTestDataFrame->m_uiPortForTestDataFrame;
	// 生成测试开始帧
	m_pSocketTestSetFrame->MakeFrameDataForTestStart();
	// 连发3次
	for(int i = 0; i < 3; i++)
	{
		// 发送测试开始帧
		m_pSocketTestSetFrame->SendTestSetFrame();
	}
}

/**
* 处理发送测试开始帧
* @param void
* @return void
*/
void CThreadProcTest::ProcSendTestADStart()
{
	irecevice=0;
	unsigned int TBtime=TBSleepTimeHigh;
	CString strDesc;

	// 生成测试开始帧
	//m_pSocketTestSetFrame->MakeFrameDataForTestStart();
	//m_pSocketTestSetFrame->MakeFrameADTestStart();
	// 连发3次

// 	// 发送测试开始帧
// 	//m_pSocketTestSetFrame->SendTestStartFrame();
// 	TRACE1("ADTimeBegin=%d\r\n", m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin);
// 	//m_pSocketTestSetFrame->MakeFrameADTestStart();
// 	m_pSocketTestSetFrame->MakeFrameADClickTestStart1();
// 	Sleep(200);		//由于发送速度慢，所以等待一下
// 
// 	CTestElementData* pTestElementData = NULL;	// 测试单元
// 	for(unsigned int i = 0; i < m_pSocketTestSetFrame->m_uiIPCount; i++)
// 	{
// 		// 得到一个单元
// 		pTestElementData = m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataByIndex(i);
// 		// 设置目标IP地址
// 		m_pSocketTestSetFrame->m_oFrameTestSet.m_uiIPAim = pTestElementData->m_uiIP;
// 		m_pSocketTestSetFrame->ofccheck=pTestElementData->ofccheck;
// 		//strDesc.Format("Test AD REG Write IP=%d,ofccheck=%06x",pTestElementData->m_uiIP,pTestElementData->ofccheck);
// 		//m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcSendTestADStart", strDesc);
// 		// 生成测试设置帧
// 		m_pSocketTestSetFrame->MakeFrameADIPTestStart();
// 		Sleep(70);		//由于发送速度慢，所以等待一下,现在下行速率最快为50ms帧
// 	}
// 	Sleep(500);		//由于发送速度慢，所以等待一下
	// 开始ADC采样准备
	m_pSocketTestSetFrame->OnADCPrepareToSample();

	m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
	// 得到野外设备系统时间
	unsigned int uiTimeNow = m_pSiteData->m_uiSystemTimeSite;
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
	TRACE1("Start begintime=%d\r\n", uiTimeNow);
	//TB开始间隔时间
	uiTimeNow=uiTimeNow+TBtime;

	m_pTestRequest->m_uiTimeADBegin = uiTimeNow + 0;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin = m_pTestRequest->m_uiTimeADBegin;	// AD采集开始时刻
	m_pTestRequest->m_uiTimeCollectBegin = m_pTestRequest->m_uiTimeADBegin + 500;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeBegin = m_pTestRequest->m_uiTimeCollectBegin;	// 采集开始时刻	

	// 得到采样时间
	unsigned int uiCollectTime = m_pTestBaseData->m_uiSamplingLength;
	// 判断：记录测试数据，且记录时间大于采样时间
	if((1 == m_pTestProject->m_uiRecordResult) && (m_pTestProject->m_uiRecordLength > uiCollectTime))
	{
		// 采样时间设为记录时间
		uiCollectTime = m_pTestProject->m_uiRecordLength;
	}
	// 得到附加测试时间
	unsigned int uiCollectTimeTail = 1000*(m_pTestBaseData->m_uiSamplingLength/m_pTestBaseData->m_uiSamplingRate) ;
	// 判断：有小数位
	if(0 < ((FrameADCount * 1000) % m_pTestBaseData->m_uiSamplingRate))
	{
		//附加测试时间增加1毫秒
		uiCollectTimeTail++;
	}
	// 得到采样时间
	//uiCollectTime = (uiCollectTime + uiCollectTimeTail)*4;
	uiCollectTime = (uiCollectTime + uiCollectTimeTail)*2;
	// 判断：测试项目为野外噪声监测
	if(4000 == m_pTestProject->m_uiTestNb)	// 野外噪声监测
	{
		// 最长240小时连续监测
		uiCollectTime = 864000000;	// 240小时
	}
	// 得到采样结束时间
	m_pTestRequest->m_uiTimeCollectEnd = m_pTestRequest->m_uiTimeCollectBegin + uiCollectTime;
	// 设置采样结束时间
	m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeEnd = m_pTestRequest->m_uiTimeCollectEnd;
	// 设置采集命令监听端口
	//m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen = m_pSocketTestDataFrame->m_uiPortForTestDataFrame;
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortListen =(unsigned short)m_pSocketTestSetFrame->m_uiPortForTestSet;
	// 设置数据采集输出的目的端口
	m_pSocketTestSetFrame->m_oFrameTestSet.m_usPortDataOut =(unsigned short) m_pSocketTestDataFrame->m_uiPortForTestDataFrame;

	strDesc.Format("Test AD Start tnow=%d,m_uiADTimeBegin=%d m_uiTimeCollectEnd=%d",m_pSiteData->m_uiSystemTimeSite,m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin,m_pTestRequest->m_uiTimeCollectEnd);
	// 设置运行状态数据
	m_pSiteData->m_oRunTimeDataList.Set(1, "CThreadProcTest", "ProcSendTestADStart", strDesc);
	TRACE3("AD开始 tnow=%d,m_uiADTimeBegin=%d m_uiTimeCollectEnd=%d\r\n",m_pSiteData->m_uiSystemTimeSite,m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin,m_pTestRequest->m_uiTimeCollectEnd);
	//开始TB
	//m_pSocketTestSetFrame->MakeFrameADClickTestStart2();
	m_pSocketTestDataFrame->okBegin=0;
	m_pSocketTestDataFrame->m_irevFrameCount=0;
	m_pSocketTestDataFrame->m_iFrameCount=0;

	for(int i=0;i<1;i++){
		m_pSocketTestSetFrame->MakeFrameADClickTestStart2();
		if(m_pSocketTestSetFrame->getTBcount>=0x03){
			TRACE2("getTBcount=%d TB Sendtime=%d\r\n", m_pSocketTestSetFrame->getTBcount,m_pSocketTestSetFrame->m_oFrameTestSet.m_uiADTimeBegin);
			break;
		}
		//TRACE2("重新发送TB getTBcount=%d TB Sendtime=%d\r\n",m_pSocketTestSetFrame->getTBcount,iTimeNow); 
	}
	//m_pSocketTestSetFrame->MakeFrameADClickTestStart2();
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
	//uiTimeNow = m_pSiteData->m_uiSystemTimeSite - 500;
	unsigned int iTimeNow = m_pSiteData->m_uiSystemTimeSite;
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
	TRACE1("ADClickTestStart2 tnow=%d\r\n",iTimeNow);
//	if(m_pTestRequest->m_uiType==22)
//		m_pSocketTestSetFrame->ViewSet(0);
}

/**
* 得到并处理测试数据
* @param void
* @return void
*/
void CThreadProcTest::GetAndProcTestData()
{
	unsigned int uiTimeNow;
	unsigned int uiFrameCount,idatacount=0;
	
	TRACE1("CThreadProcTest1 ID=%d\r\n", GetCurrentThreadId());
	MSG msg;
	while(true)
	{
		while(true)
		{
			// 得到网络对象收到的测试数据帧的数量
			uiFrameCount = m_pSocketTestDataFrame->GetFrameCount1();
			// 判断：测试数据帧的数量为0
			if(0 == uiFrameCount)
			{
				Sleep(5);
				break;
			}

		CString strDesc;
		if (uiFrameCount>200)
		{
			strDesc.Format("CThreadProcTest1 uiFrameCount=%d ",uiFrameCount);
			m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "GetAndProcTestData 1", strDesc);
		}
		m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
			uiTimeNow = m_pSiteData->m_uiSystemTimeSite;
		m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();

			// 依次得到测试数据帧
			for(unsigned int i = 0; i < uiFrameCount; i++)
			{
				// 得到测试数据帧
				m_pSiteData->m_oSocketTestDataFrame.Lock();
				if(m_pSocketTestDataFrame->GetFrameData1()==false){	//处理错误连续帧，跳过，初始化等待下一次idx=0
					TRACE2("ERROR m_irevFrameCount=%d m_iFrameCount=%d\r\n", m_pSocketTestDataFrame->m_irevFrameCount,m_pSocketTestDataFrame->m_iFrameCount);
					m_pSocketTestDataFrame->okBegin=0;
					m_pSocketTestDataFrame->m_irevFrameCount=0;
					m_pSocketTestDataFrame->m_iFrameCount=0;
					m_pSiteData->m_oSocketTestDataFrame.Unlock();
					break;
				}
				//GetMessage(&msg,0,0,0);
				m_pSiteData->m_oSocketTestDataFrame.Unlock();
				// 设置测试数据
				SetTestData(m_pSocketTestDataFrame->m_oFrameTestData.m_pData,
					m_pSocketTestDataFrame->m_oFrameTestData.m_uiIPSource,
					m_pSocketTestDataFrame->m_oFrameTestData.m_usDataIndex);
				idatacount++;
				//转发绘图帧
				if(m_pTestRequest->m_uiType==22 && m_pSocketTestSetFrame->Viewtype==1)
					TranferView(m_pSocketTestDataFrame->m_oFrameTestData.m_pFrameData);
				else
					for(int i=0;i<MaxFDUCount;i++)
						FrameNum[i]=0;
			}

			if(false == JudgeProcCondition())	// 判断是否可以处理的条件
			{
				return;
			}
			if((m_pTestRequest->m_uiTimeCollectEnd  < uiTimeNow))
				break;
		}
		m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
			uiTimeNow = m_pSiteData->m_uiSystemTimeSite;
		m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();

		m_pSocketTestSetFrame->OnReceiveADCSetReturn(1);
		// 判断：采集结束时间到
		if((m_pTestRequest->m_uiTimeCollectEnd  < uiTimeNow))
		{
			TRACE1("SetTestData uiFrameCount=%d\r\n", uiFrameCount);
			TRACE3("END time=%d m_uiTimeCollectEnd=%d m_uiIPCount=%d\r\n", uiTimeNow,m_pTestRequest->m_uiTimeCollectEnd,m_pSocketTestSetFrame->m_uiIPCount);
			//TRACE2("END time=%d m_pSocketTestSetFrame->m_uiIPCount=%d\r\n", uiTimeNow,m_pSocketTestSetFrame->m_uiIPCount);
			CString strDesc;
			strDesc.Format("END uiTimeNow=%d m_uiTimeCollectEnd=%d m_uiIPCount=%d",uiTimeNow,m_pTestRequest->m_uiTimeCollectEnd,m_pSocketTestSetFrame->m_uiIPCount);
			m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "END time ", strDesc);
			break;
		}
		//Wait(1);
		Sleep(0);
		if(false == JudgeProcCondition())	// 判断是否可以处理的条件
		{
			return;
		}
	}
	if(false == JudgeProcCondition())	// 判断是否可以处理的条件
	{
		return;
	}

	// 处理测试数据，结束前
	SetTestDataToProcAll();
}

/**
* 转发采集数据到绘图
* @param byte* lpData 数据指针
* @param 16~19为采样设备绘图序号，从0开始
* @param 30~33为设备接收帧数（针对单个设备，从0开始）
* @return void
*/
void CThreadProcTest::TranferView(byte* lpData)
{
	 CString m_strIPLCI=ViewIPStr;
	UINT m_uiPortLCI=ViewPort;
	CTestElementData* pTestElementData = NULL;	
	byte* pData = NULL;	
	int iViewGain=m_pSocketTestSetFrame->m_uiSamplingView;
	int itail=0,j=0;
	for(unsigned int i = 0; i < m_pSocketTestSetFrame->m_uiIPCount; i++)
	{
		// 得到一个单元
		pTestElementData = m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataByIndex(i);
		if(m_pSocketTestDataFrame->m_oFrameTestData.m_uiIPSource==pTestElementData->m_uiIP){
			FDUNum=i;
			break;
		}
	}
	if(FrameNum[FDUNum]==0){
		ViewOffset[FDUNum]=0;
		ViewOffsetFrame[FDUNum]=0;
		memcpy(ViewBuf+FDUNum*FrameLength, lpData, FrameLength);
		memset(ViewBuf+FDUNum*FrameLength+36,0x55,FrameADCount72*3);
	}
	//取抽样点拼帧
	for(int i=0;ViewOffsetFrame[FDUNum]+i<(FrameADCount72);i+=iViewGain){
		memcpy(ViewBuf+FDUNum*FrameLength+36+ViewOffset[FDUNum]*3+j*3, lpData+36+ViewOffsetFrame[FDUNum]*3+i*3, 3);
		FrameNum[FDUNum]++;
		j++;
		//接收帧满
		if(ViewOffsetFrame[FDUNum]+i>FrameADCount72){
			break;
		}
		//发送帧满
		if(FrameNum[FDUNum]%FrameADCount72==0){
			//itail=i;
			//ViewOffsetFrame[FDUNum]+=(i+1)*iViewGain;
			break;
		}
	}
	itail=FrameADCount72-((ViewOffsetFrame[FDUNum]+j*iViewGain)%FrameADCount72);
	ViewOffset[FDUNum]+=j;

	//发送帧
	if(FrameNum[FDUNum]%FrameADCount72==0){
		memcpy(ViewBuf+FDUNum*FrameLength+16,&FDUNum,sizeof(int));
		int itmp=FrameNum[FDUNum]/FrameADCount72-1;
		memcpy(ViewBuf+FDUNum*FrameLength+30,&itmp,sizeof(int));
		// 发送帧
		int iCount = m_pSocketTestSetFrame->SendTo(ViewBuf+FDUNum*FrameLength, FrameLength, m_uiPortLCI, m_strIPLCI);
		ViewOffset[FDUNum]=0;
		memset(ViewBuf+FDUNum*FrameLength+36,0x11,FrameADCount72*3);
		//处理接收帧剩余数据
		j=0;
		if(itail!=0){
			for(int i=itail;ViewOffsetFrame[FDUNum]+i<(FrameADCount72);i+=iViewGain){
				memcpy(ViewBuf+FDUNum*FrameLength+36+j*3, lpData+36+ViewOffsetFrame[FDUNum]*3+i*3,3);
				FrameNum[FDUNum]++;
				j++;
			}
			itail=(FrameADCount72-(ViewOffsetFrame[FDUNum]+itail+j*iViewGain));
			if(itail!=0)
				ViewOffsetFrame[FDUNum]=(iViewGain-itail%iViewGain)%iViewGain;
			else
				ViewOffsetFrame[FDUNum]=itail;
		}
		ViewOffset[FDUNum]=j;
	}else
		if(itail!=0)
			ViewOffsetFrame[FDUNum]=(iViewGain-itail%iViewGain)%iViewGain;
		else
			ViewOffsetFrame[FDUNum]=itail;
}

/**
* 设置测试数据
* @param byte* lpData 数据指针
* @param unsigned int uiIP 数据指针
* @param unsigned short usDataIndex 数据位置索引
* @return void
*/
void CThreadProcTest::SetTestData(byte* lpData, unsigned int uiIP, unsigned short usDataIndex)
{
	CString strDesc;
	CTestElementData* pTestElementData = NULL;
	// 判断：仪器IP地址不在本次测试中
	if(FALSE == m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataFromMap(uiIP, pTestElementData))
	{
		CString strDesc;
		strDesc.Format("dIP=%d m_uiFrameIndex=%d ERROR",uiIP,usDataIndex);
		m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcTest", "SetTestData 5", strDesc);
		TRACE2("仪器IP地址不在本次测试中 dIP=%d m_uiFrameIndex=%d\r\n",uiIP, usDataIndex); 
		return;
	}
	totalcount++;
	irecevice++;

	//设置开始偏移量
	if(pTestElementData->m_uiDataBegin==-1)
		pTestElementData->m_uiDataBegin = usDataIndex-FrameADCount72;
	// 判断：数据索引进位
	if(usDataIndex < pTestElementData->m_uiDataIndexPreview && usDataIndex<FrameADCount72)	// 进位
	{
		if(usDataIndex!=pTestElementData->IndexPreview)
		// 数据索引进位基数增加一次进位
		pTestElementData->m_uiDataStep = pTestElementData->m_uiDataStep + ADoffset;
	}
	int itmp=pTestElementData->m_uiFrameIndex;
	//pTestElementData->m_uiFrameIndex = (pTestElementData->m_uiDataStep + usDataIndex) / m_pTestDataBuffer->m_uiGroupDataCount;	// 数据索引
	if(usDataIndex!= pTestElementData->IndexPreview)
		// 得到帧计数
		pTestElementData->m_uiFrameIndex = (pTestElementData->m_uiDataStep + usDataIndex-pTestElementData->m_uiDataBegin) / m_pTestDataBuffer->m_uiGroupDataCount;	// 数据索引
	else
		if(pTestElementData->IndexPreview>ADoffset-2*FrameADCount72)
			pTestElementData->m_uiFrameIndex = (pTestElementData->m_uiDataStep + usDataIndex-pTestElementData->m_uiDataBegin-ADoffset) / m_pTestDataBuffer->m_uiGroupDataCount;	// 数据索引
		else
			pTestElementData->m_uiFrameIndex = (pTestElementData->m_uiDataStep + usDataIndex-pTestElementData->m_uiDataBegin) / m_pTestDataBuffer->m_uiGroupDataCount;	// 数据索引
	
	//防止溢出
	if(pTestElementData->m_uiFrameIndex-itmp>12 && itmp-pTestElementData->m_uiFrameIndex>12){
		pTestElementData->m_uiFrameIndex=itmp;
		recevicecount++;
		strDesc.Format("dIP=%d m_uiFrameIndex=%d ERROR",uiIP,usDataIndex);
		m_pSiteData->m_oRunTimeDataList.Set(2, "CThreadProcTest", "SetTestData 4", strDesc);
		TRACE2("偏移量溢出 dIP=%d m_uiFrameIndex=%d\r\n",uiIP, usDataIndex); 
		return ;
	}

	int x=usDataIndex;
	if(usDataIndex<pTestElementData->m_uiDataIndexPreview && pTestElementData->IndexPreview==-1)
		x=usDataIndex+ADoffset;
	int	iSampleData1 = 0;
	int	iSampleData2 = 0;
	if(FrameADCount72==FrameADCount){
		memcpy(&iSampleData1, lpData, 3);
		memcpy(&iSampleData2, lpData+9, 3);
	}else{
		memcpy(&iSampleData1, lpData, 4);
		memcpy(&iSampleData2, lpData+9, 3);
	}
	if(recevicecount%(MaxFDUCount*5)==0 || recevicecount<MaxFDUCount*4){
	//if(recevicecount%MaxFDUCount==0 || recevicecount<MaxFDUCount){
		strDesc.Format("dIP=%d m_uiFrameIndex=%d %d Index=%d d1=%x d2=%x %d %d %d",uiIP,usDataIndex,x-pTestElementData->m_uiDataIndexPreview,m_pTestDataBuffer->m_uiFrameIndex,iSampleData1,iSampleData2,irecevice,pTestElementData->m_uiDataStep,pTestElementData->IndexPreview);
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "SetTestData 1", strDesc);
		//TRACE3("接收数据帧2=%d %d IP=%d\r\n", usDataIndex,x-pTestElementData->m_uiDataIndexPreview,uiIP );
	}
	if(m_pTestRequest->m_uiType==22 && (recevicecount%(m_pSocketTestSetFrame->m_uiIPCount*600))==0){
		strDesc.Format("dIP=%d m_uiFrameIndex=%d %d Index=%d d1=%x d2=%x %d %d %d",uiIP,usDataIndex,x-pTestElementData->m_uiDataIndexPreview,m_pTestDataBuffer->m_uiFrameIndex,iSampleData1,iSampleData2,irecevice,pTestElementData->m_uiDataStep,pTestElementData->IndexPreview);
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "SetTestData 8", strDesc);
	}
	//判断：是否有丢帧情况
	//if(pTestElementData->m_uiFrameIndex - m_pTestDataBuffer->m_uiFrameIndex!=1){
	if(x-pTestElementData->m_uiDataIndexPreview!=FrameADCount72 && x-pTestElementData->m_uiDataIndexPreview!=0 && (x-pTestElementData->m_uiDataIndexPreview)<FrameADCount72*3 && pTestElementData->m_uiFrameIndex>1 && pTestElementData->IndexPreview==-1){
		//暂时只补一桢
		CString strDesc;
		//补上一包数据
		int y=usDataIndex-FrameADCount72;
		if(y<0)
			y=y+ADoffset;
		if(y!=pTestElementData->IndexPreview){
			strDesc.Format("dIP=%d m_uiFrameIndex=%d",uiIP, y);
			m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "SetTestData 3", strDesc);
			TRACE2("补测试数据帧数 dIP=%d m_uiFrameIndex=%d\r\n",uiIP, y); 
			pTestElementData->IndexPreview=y;
			lossframecount++;
			m_pSocketTestSetFrame->ReloadTestDataFrame(uiIP,y);
		}
	}
	//如果是补包帧成功
	if( (pTestElementData->IndexPreview==usDataIndex) ){
		pTestElementData->m_uiDataIndexPreview=usDataIndex+FrameADCount72;
		if(pTestElementData->m_uiDataIndexPreview>ADoffset)
			pTestElementData->m_uiDataIndexPreview=pTestElementData->m_uiDataIndexPreview-ADoffset;
		pTestElementData->IndexPreview=-1;
	}else{
		// 记录上一次数据索引
		pTestElementData->m_uiDataIndexPreview = usDataIndex;
		//补包在2帧之后还没有补上,恢复原始状态
		if((usDataIndex-pTestElementData->IndexPreview)>FrameADCount72*2)
			pTestElementData->IndexPreview=-1;
	}
	// 设置测试数据
	m_pTestDataBuffer->SetElementTestDataOfOneFrame(lpData, pTestElementData->m_uiIndex, pTestElementData->m_uiFrameIndex);
	recevicecount++;

	// 判断：野外设备发来新一组采集
	if(pTestElementData->m_uiFrameIndex > m_pTestDataBuffer->m_uiFrameIndex)
	{
		// 设置测试数据存储对象帧计数
		m_pTestDataBuffer->m_uiFrameIndex = pTestElementData->m_uiFrameIndex;
		// 输出前一组前的所有数据
		if((m_pTestDataBuffer->m_uiFrameIndex - 1) > m_pTestDataBuffer->m_uiFrameIndexWrite)
		{
			saveecount++;
			// 处理测试数据
			SetTestDataToProc();
			// 设置测试数据存储对象已处理帧计数
			m_pTestDataBuffer->m_uiFrameIndexWrite = m_pTestDataBuffer->m_uiFrameIndex - 1;
		}
	}

}

/**
* 处理测试数据
* @param void
* @return void
*/
void CThreadProcTest::SetTestDataToProc()
{
	// 处理测试数据，噪声监测
	SetTestDataToProcNoise();
	// 处理测试数据，保存测试数据
	SetTestDataToProcFileTestDataWrite();
}

/**
* 处理测试数据，噪声监测
* @param void
* @return void
*/
void CThreadProcTest::SetTestDataToProcNoise()
{
	// 判断：仪器测试
	if(1 == m_pTestRequest->m_uiAim)
	{
		return;
	}
	// 判断：第一帧
	if(0 == m_pTestDataBuffer->m_uiFrameIndex)
	{
		return;
	}
	// 判断：测试数据存储对象已处理帧计数大于等于噪声监测数据网络发送计数
	if(m_pTestDataBuffer->m_uiFrameIndexWrite >= m_uiTestNoiseNetSendCount)
	{
		// 设置噪声监测数据网络发送计数
		m_uiTestNoiseNetSendCount = m_uiTestNoiseNetSendCount + m_uiTestNoiseNetSendStep;
		CTestElementData* pTestElementData = NULL;	
		byte* pData = NULL;	
//		TRACE1("噪声监测处理帧计数：%d\r\n", m_pTestDataBuffer->m_uiFrameIndexWrite);
		for(unsigned int i = 0; i < m_pSocketTestSetFrame->m_uiIPCount; i++)
		{
			// 得到一个单元
			pTestElementData = m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataByIndex(i);
			// 得到测试单元一次测试数据，噪声监测
			pData = m_pTestDataBuffer->GetElementTestDataOfOneTestForNoiseMonitor(m_pTestDataBuffer->m_uiFrameIndexWrite, i);
			// 计算
			pTestElementData->CalculateTestResult(m_pTestRequest->m_uiType, pData, FrameADCount,(float)m_pTestDataBuffer->m_AVGNoiseValue);

		}
		// 发送噪声监测测试结果
		SendTestResultNoise();
	}
}

/**
* 处理测试数据，保存测试数据
* @param void
* @return void
*/
void CThreadProcTest::SetTestDataToProcFileTestDataWrite()
{
	// 判断：第一帧
	if(0 == m_pTestDataBuffer->m_uiFrameIndex)
	{
		return;
	}
	// 判断：处理方式 2-噪声监测
	if(2 == m_iProcTestMethod)
	{
		return;
	}
	// 判断：不记录测试数据
	if(0 == m_pTestProject->m_uiRecordResult)
	{
		return;
	}

	CTestElementData* pTestElementData = NULL;	
	byte* pBlockData = NULL;	
	for(unsigned int i = m_pTestDataBuffer->m_uiFrameIndexWrite; i < (m_pTestDataBuffer->m_uiFrameIndex - 1); i++)
	{
		// 得到一帧测试数据，单位块
		pBlockData = m_pTestDataBuffer->GetBlockTestDataOneFrame(i);
		// 写测试数据文件
		FileTestDataWrite(pBlockData, m_pTestDataBuffer->m_uiBlockByteSizeUsed);
	}
}

/**
* 处理测试数据，结束测试请求前
* @param void
* @return void
*/
void CThreadProcTest::SetTestDataToProcAll()
{
	// 判断：处理方式 2-噪声监测
	if(2 == m_iProcTestMethod)
	{
		return;
	}
	// 判断：第一帧
	if(0 == m_pTestDataBuffer->m_uiFrameIndex)
	{
		return;
	}
	
	//处理测试数据，结束测试请求前，保存测试数据
	SetTestDataToProcAllFileTestDataWrite();
	// 得到测试结果，基本测试
	GetTestResultBase();
	// 设置测试结果，基本测试
	SetTestResultBase();
	// 发送测试结果，基本测试
	SendTestResultBase();
}

/**
* 处理测试数据，结束测试请求前，保存测试数据
* @param void
* @return void
*/
void CThreadProcTest::SetTestDataToProcAllFileTestDataWrite()
{
	if(0 == m_pTestProject->m_uiRecordResult)
	{
		return;
	}
	// 测试数据写入数据文件
	byte* pBlockData = NULL;
	TRACE1("收到测试数据帧的总数：%d\r\n", m_pTestDataBuffer->m_uiFrameIndex);
	for(unsigned int i = m_pTestDataBuffer->m_uiFrameIndexWrite; i <= m_pTestDataBuffer->m_uiFrameIndex; i++)
	{
		// 得到一帧测试数据，单位块
		pBlockData = m_pTestDataBuffer->GetBlockTestDataOneFrame(i);
		// 写测试数据文件
		FileTestDataWrite(pBlockData, m_pTestDataBuffer->m_uiBlockByteSizeUsed);
//		TRACE1("写测试数据文件 桢计数：%d\r\n", i);
	}
}

/**
* 得到测试结果，基本测试
* @param void
* @return void
*/
void CThreadProcTest::GetTestResultBase()
{
	CTestElementData* pTestElementData = NULL;	
	byte* pData = NULL;	
	bool bfinish=false;
	m_Revidx=0;
	int upADCDataNum=0;
	lineCount=ReceiveDataPoint;
	for(unsigned int i = 0; i < m_pSocketTestSetFrame->m_uiIPCount; i++)
	{
		// 得到一个单元
		pTestElementData = m_pSocketTestSetFrame->m_pTestElementList->GetTestElementDataByIndex(i);
		// 得到测试单元一次测试数据
		pData = m_pTestDataBuffer->GetElementTestDataOfOneTest(i, m_pTestBaseData->m_uiSamplingPointCount);
		// 计算
		pTestElementData->CalculateTestResult(m_pTestRequest->m_uiType, pData, m_pTestBaseData->m_uiSamplingPointCount,(float)m_pTestDataBuffer->m_AVGNoiseValue);
		//TRACE2("m_uiIP=%d m_AVGNoiseValue=%f\r\n",pTestElementData->m_uiIP, pTestElementData->m_fInstrumentNoiseValue);
		rev_count[i]=ReceiveDataPoint;
		m_pADCDataSaveToFile.OnSaveADCToFile(pData+(FrameADCount)*4,&upADCDataNum,rev_count, m_pSocketTestSetFrame->m_uiIPCount, bfinish,0, m_pSocketTestSetFrame->m_uiIPCount);
		// 判读
		pTestElementData->JudgeTestResult(m_pTestRequest->m_uiType);
	}
}

/**
* 设置测试结果，基本测试
* @param void
* @return void
*/
void CThreadProcTest::SetTestResultBase()
{
	// 判断：仪器测试
	if(1 == m_pTestRequest->m_uiAim)
	{
		m_pSiteData->SetTestResultBase(m_pTestRequest->m_uiType, m_pSocketTestSetFrame->m_pTestElementList);
	}
	// 判断：检波器测试
	else if(2 == m_pTestRequest->m_uiAim)
	{
		m_pLogicData->SetTestResultBase(m_pTestRequest->m_uiType, m_pSocketTestSetFrame->m_pTestElementList, m_bProcSensorTestOnField);
	}
}

/**
* 发送测试结果
* @param void
* @return void
*/
void CThreadProcTest::SendTestResult()
{
	unsigned int uiDataSize = 0;
	unsigned short usDataCount = 0;
	// 按批次计算，测试数据批次数
	unsigned int uiDataBatchCount = m_pSocketTestSetFrame->m_pTestElementList->GetTestDataBatchCount();
	for(unsigned int i = 0; i < uiDataBatchCount; i++)
	{
		// 按批次计算，测试数据批次数
		m_pSocketTestSetFrame->m_pTestElementList->GetTestDataByBatchIndex(m_pTestRequest->m_uiType, i, uiDataSize, usDataCount);
		// 生成测试数据帧
		m_pSocketInterfaceDataSend->CreateOutputDataFrameTest(m_pTestRequest->m_uiType, uiDataSize, usDataCount, m_pSocketTestSetFrame->m_pTestElementList->m_pTestData);
		// 发送数据帧
		int iCount=m_pSocketInterfaceDataSend->SendDataFrame();
		//TRACE2("测试结果帧发送：帧大小%d %d\r\n", iCount,i);
		// 判断；不能继续处理
		if(false == JudgeProcCondition())
		{
			break;
		}
	}
}

/**
* 发送测试结果，基本测试
* @param void
* @return void
*/
void CThreadProcTest::SendTestResultBase()
{
	// 判断：处理方式 2-噪声监测
	if(2 == m_iProcTestMethod)
	{
		return;
	}
	// 发送测试结果
	SendTestResult();
}

/**
* 发送测试结果，噪声监测
* @param void
* @return void
*/
void CThreadProcTest::SendTestResultNoise()
{
	// 判断：处理方式 1-基本测试
	if(1 == m_iProcTestMethod)
	{
		return;
	}
	// 发送测试结果
	SendTestResult();
}

/**
* 停止正在进行数据采集的仪器
* @param void
* @return void
*/
void CThreadProcTest::StopInstrumentWhichIsCollecting()
{
	unsigned int uiFrameCount;
	// 向仪器发送采集结束帧
	ProcSendTestEndFrame(0xFFFFFFFF);
	while(true)
	{
		// 得到网络端口收到数据帧数量
		uiFrameCount = m_pSocketTestDataFrame->GetFrameCount1();
		TRACE1("收到测试帧数量end：%d\r\n", uiFrameCount);
		if(0 == uiFrameCount)
		{
			break;
		}
		CString strDesc;
		strDesc.Format("End uiFrameCount=%d",uiFrameCount);
		m_pSiteData->m_oRunTimeDataList.Set(3, "CThreadProcTest", "SetTestData STOP", strDesc);

		// 读空数据
		for(unsigned int i = 0; i < uiFrameCount; i++)
		{
			// 得到帧数据，不解析
			m_pSocketTestDataFrame->GetFrameDataNotParse1();

		}
		// 等待
		Wait(2);
	}
}

/**
* 设置噪声监测数据网络发送步长
* @param void
* @return void
*/
void CThreadProcTest::SetTestNoiseNetSendStep()
{
	// 判断：仪器测试
	if(1 == m_pTestRequest->m_uiAim)
	{
		return;
	}
	// 设置噪声监测数据网络发送步长
	m_uiTestNoiseNetSendStep = NetSendStep / (m_pTestBaseData->m_uiSamplingRate * FrameADCount);
	if(m_uiTestNoiseNetSendStep == 0)
	{
		m_uiTestNoiseNetSendStep = 1;
	}
	// 噪声监测数据网络发送计数
	m_uiTestNoiseNetSendCount = 0;
}

/**
* 处理发送测试停止帧
* @param void
* @return void
*/
void CThreadProcTest::ProcSendTestEndFrame(unsigned int uiInstrumentIP)
{
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Lock();
		m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeEnd = m_pSiteData->m_uiSystemTimeSite;	// 采集结束时刻
	m_pSiteData->m_oCriticalSectionSystemTimeSite.Unlock();
	// 连发21次
	for(int i = 0; i <= 1; i++)
	{
		// 测试结束时间递增50毫秒
		m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeEnd = m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeEnd + 50;
		// 生成测试结束帧
		m_pSocketTestSetFrame->MakeFrameADTestSTOP(uiInstrumentIP);
		//m_pSocketTestSetFrame->MakeFrameDataForTestEnd(uiInstrumentIP);
		//TRACE1("FrameADTestSTOP：m_uiCollectTimeEnd=%d\r\n", m_pSocketTestSetFrame->m_oFrameTestSet.m_uiCollectTimeEnd);
		// 发送测试结束帧
		//m_pSocketTestSetFrame->SendTestEndFrame();
	}
}