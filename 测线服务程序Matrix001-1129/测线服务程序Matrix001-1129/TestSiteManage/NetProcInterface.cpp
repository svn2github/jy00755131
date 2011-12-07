#include "stdafx.h"
#include "NetProcInterface.h"

CNetProcInterface::CNetProcInterface()
{
}

CNetProcInterface::~CNetProcInterface()
{
}

/**
* 初始化
* @param void
* @return void
*/
bool CNetProcInterface::OnInit()
{
	// 是否工作状态
	m_bWork = false;
	// 是否关闭线程
	m_bClose = false;
	return true;
}

/**
* 关闭
* @param void
* @return void
*/
bool CNetProcInterface::OnClose()
{
	// 是否关闭线程
	m_bClose = true;
	return true;
}

/**
* 开始工作
* @param void
* @return bool true：成功；false：失败
*/
bool CNetProcInterface::OnWork()
{
	// 是否工作状态
	m_bWork = true;
	return true;
}

/**
* 停止工作
* @param void
* @return bool true：成功；false：失败
*/
bool CNetProcInterface::OnStop()
{
	// 是否工作状态
	m_bWork = false;
	return true;
}

/**
* 处理内部网络命令帧
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::OnProcInterFaceCommand(CFrameInterface* pFrameInterface)
{
	switch(pFrameInterface->m_usCommand)
	{
	case 1:	// 命令	1-Field ON/OFF
		// 处理内部网络命令帧；命令	1-Field ON/OFF
		ProcInterFaceCommandForField(pFrameInterface);

		break;

	case 2:	// 命令	2-FormLine
		// 处理内部网络命令帧；命令	2-FormLine
		ProcInterFaceCommandForFormLine(pFrameInterface);
		break;

	case 3:	// 命令	3-Look
		// 处理内部网络命令帧；命令	3-Look
		ProcInterFaceCommandForLook(pFrameInterface);
		break;

	case 4:	// 命令	4-Test From InstrumentTestSetup
	case 5:	// 命令	5-Test From SensorTestSetup
	case 6:	// 6-Test From MultipleTestSetup
		// 处理内部网络命令帧；基本测试命令；来自测试设置的“GO”按钮
		ProcInterFaceCommandForTestBaseFromTestSetup(pFrameInterface);
		break;

	case 7:	// 命令 7-Test From SeismonitorTestSetup
		// 处理内部网络命令帧；噪声监测命令；来自测试设置的“GO”按钮
		ProcInterFaceCommandForTestNoiseFromTestSetup(pFrameInterface);
		break;

	case 8:	// 命令	8-Test From InstrumentView 来自仪器视图的“GO”按钮
	case 9:	// 命令 9-Test From SensorTestView	来自检波器视图的“GO”按钮
		// 处理内部网络命令帧；基本测试命令；来自仪器视图或检波器视图的“GO”按钮
		ProcInterFaceCommandForTestBaseFromTestView(pFrameInterface);
		break;

	case 10:	// 命令 7-Test From SeismonitorView
		// 处理内部网络命令帧；噪声监测命令；来自噪声监测视图的“Seismonitor”按钮
		ProcInterFaceCommandForTestNoiseFromTestView(pFrameInterface);
		break;

	case 20:	// 命令	20-测线配置文件更改命令
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令
		ProcInterFaceCommandForSetupDataChange(pFrameInterface);
		break;
	}
}

/**
* 处理内部网络命令帧；命令	1-Field ON/OFF
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForField(CFrameInterface* pFrameInterface)
{
	ProcInterFaceCommandForField(pFrameInterface->m_byFieldOperation);
}

/**
* 处理内部网络命令帧；命令	1-Field ON/OFF
* @param byte byFieldOperation FieldON/OFF 0：OFF；1：ON
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForField(byte byFieldOperation)
{
	if(0 == byFieldOperation)	// Field OFF
	{
		if(false == m_bWork)
		{
			// 发送服务器Field命令
			SendServerFieldCmd(byFieldOperation);
			return;
		}
		m_pThreadProcHeartBeat->OnStop();	// 停止心跳处理对象
		m_pThreadProcHeadFrame->OnStop();	// 停止首包处理对象
		m_pThreadProcTailFrame->OnStop();	// 停止尾包处理对象
		m_pThreadProcMonitorRout->OnStop();	// 停止路由监视对象
		m_pThreadProcIPDistribute->OnStop();	// 停止仪器IP地址分配对象
		m_pThreadProcTimeDelay->OnStop();	// 停止时延处理线程对象
		m_pThreadProcIPSet->OnStop();	// 停止仪器IP地址设置对象		
		m_pThreadProcDelete->OnStop();	// 停止仪器删除对象
		m_pThreadProcSiteDataOutput->OnStop();	// 停止现场数据输出对象		
		m_pThreadProcTestBase->OnStop();	// 停止测试对象，基本测试		
		m_pThreadProcTestNoise->OnStop();	// 停止测试对象，噪声监测

		while(true)	// 等待线程停止
		{
			if((true == m_pSiteData->m_bProcHeartBeatStop)	// 心跳处理线程停止
				&& (true == m_pSiteData->m_bProcHeadFrameStop)	// 首包处理线程停止
				&& (true == m_pSiteData->m_bProcTailFrameStop)	// 尾包处理线程停止
				&& (true == m_pSiteData->m_bProcMonitorRoutStop)	// 路由监视线程停止
				&& (true == m_pSiteData->m_bProcIPDistributeStop)	// 仪器IP地址分配线程停止
				&& (true == m_pSiteData->m_bProcIPSetStop)	// 仪器IP地址设置线程停止
				&& (true == m_pSiteData->m_bProcSiteDataOutputStop)	// 现场数据输出线程线程停止
				&& (true == m_pThreadProcTimeDelay->IsThreadFree())	// 时延处理线程空闲
				&& (true == m_pThreadProcDelete->IsThreadFree())	// 仪器删除线程空闲
				&& (true == m_pThreadProcTestBase->IsThreadFree())	// 测试处理线程空闲，基本测试
				&& (true == m_pThreadProcTestNoise->IsThreadFree()))	// 测试处理线程空闲，噪声监测
			{
				break;
			}
			Sleep(50);	// 休眠，等待线程处理停止
		}		
		m_pLogicData->OnReset();	// 重置测线设置数据对象
		m_pSiteData->OnReset();	// 重置现场数据对象
		m_bWork = false;
		// 发送服务器Field命令
		SendServerFieldCmd(byFieldOperation);
		// 输出重置后的现场数据
		m_pThreadProcSiteDataOutput->OutputToFileForReset();
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CNetProcInterface", "ProcInterFaceCommandForField", "FieldOff");
	}
	else if(1 == byFieldOperation)	// Field ON
	{
		if(true == m_bWork)
		{
			// 发送服务器Field命令
			SendServerFieldCmd(byFieldOperation);
			return;
		}		
		m_pThreadProcHeartBeat->OnWork();	// 启动心跳处理对象
		m_pThreadProcHeadFrame->OnWork();	// 启动首包处理对象
		m_pThreadProcTailFrame->OnWork();	// 启动尾包处理对象
		m_pThreadProcMonitorRout->OnWork();	// 启动路由监视对象
		m_pThreadProcIPDistribute->OnWork();	// 启动仪器IP地址分配对象
		m_pThreadProcIPSet->OnWork();	// 启动仪器IP地址设置对象	
		m_pThreadProcSiteDataOutput->OnWork();	// 启动现场数据输出对象
		m_pThreadProcTimeDelay->OnWork();	// 启动时延处理线程对象
		m_pThreadProcDelete->OnWork();	// 启动仪器删除对象
		m_pThreadProcTestBase->OnWork();	// 停启动测试对象，基本测试		
		m_pThreadProcTestNoise->OnWork();	// 启动测试对象，噪声监测
		m_bWork = true;
		// 发送服务器Field命令
		SendServerFieldCmd(byFieldOperation);
		// 设置运行状态数据
		m_pSiteData->m_oRunTimeDataList.Set(1, "CNetProcInterface", "ProcInterFaceCommandForField", "FieldOn");
	}
}

/**
* 处理内部网络命令帧；命令	2-FormLine
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForFormLine(CFrameInterface* pFrameInterface)
{	
	if(false == m_bWork)	// 现场停止
	{
		return;
	}

	CInstrument* pInstrument = NULL;	// 仪器指针
	// 由仪器SN号得到仪器
	if(false == m_pSiteData->m_oSNInstrumentMap.GetInstrument(pFrameInterface->m_uiSN, pInstrument))
	{
		return;
	}
	if(1 != pInstrument->m_uiInstrumentType)	// 非交叉站
	{
		return;
	}
	// 判断：主交叉站
	if((0 == pInstrument->m_uiRoutIP) && (0 == pInstrument->m_uiRoutDirection))
	{
		// 判断：不需要设置路由地址
		if((0 == pInstrument->m_uiRoutIPLeft) && (0 == pInstrument->m_uiRoutIPRight) && (0 == pInstrument->m_uiRoutIPTop) && (0 == pInstrument->m_uiRoutIPDown))
		{
			// 设置交叉站路由方向上
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 1);
			// 设置交叉站路由方向下
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 2);
			// 设置交叉站路由方向左
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 3);
			// 设置交叉站路由方向右
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 4);
			// 增加一个需要设置IP地址的仪器
			m_pThreadProcHeadFrame->AddInstrumentForIPSet(pInstrument);		
		}
	}
	// 判断：交叉站，来自交叉站上方
	if((0 < pInstrument->m_uiRoutIP) && (1 == pInstrument->m_uiRoutDirection))
	{
		// 判断：不需要设置路由地址
		if((0 == pInstrument->m_uiRoutIPLeft) && (0 == pInstrument->m_uiRoutIPRight) && (0 == pInstrument->m_uiRoutIPTop) && (0xFFFF == pInstrument->m_uiRoutIPDown))
		{
			// 设置交叉站路由方向左
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 3);
			// 设置交叉站路由方向右
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 4);
			// 增加一个需要设置IP地址的仪器
			m_pThreadProcHeadFrame->AddInstrumentForIPSet(pInstrument);	
		}
	}
	// 判断：交叉站，来自交叉站下方
	if((0 < pInstrument->m_uiRoutIP) && (2 == pInstrument->m_uiRoutDirection))
	{
		// 判断：不需要设置路由地址
		if((0 == pInstrument->m_uiRoutIPLeft) && (0 == pInstrument->m_uiRoutIPRight) && (0xFFFF == pInstrument->m_uiRoutIPTop) && (0 == pInstrument->m_uiRoutIPDown))
		{
			// 设置交叉站路由方向左
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 3);
			// 设置交叉站路由方向右
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 4);
			// 增加一个需要设置IP地址的仪器
			m_pThreadProcHeadFrame->AddInstrumentForIPSet(pInstrument);	
		}
	}
	// 判断：交叉站，来自交叉站左方
	if((0 < pInstrument->m_uiRoutIP) && (3 == pInstrument->m_uiRoutDirection))
	{
		// 判断：不需要设置路由地址
		if((0 == pInstrument->m_uiRoutIPLeft) && (0xFFFF == pInstrument->m_uiRoutIPRight) && (0 == pInstrument->m_uiRoutIPTop) && (0 == pInstrument->m_uiRoutIPDown))
		{
			// 设置交叉站路由方向左
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 1);
			// 设置交叉站路由方向右
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 2);
			// 增加一个需要设置IP地址的仪器
			m_pThreadProcHeadFrame->AddInstrumentForIPSet(pInstrument);	
		}
	}
	// 判断：交叉站，来自交叉站右方
	if((0 < pInstrument->m_uiRoutIP) && (4 == pInstrument->m_uiRoutDirection))
	{
		// 判断：不需要设置路由地址
		if((0xFFFF == pInstrument->m_uiRoutIPLeft) && (0 == pInstrument->m_uiRoutIPRight) && (0 == pInstrument->m_uiRoutIPTop) && (0 == pInstrument->m_uiRoutIPDown))
		{
			// 设置交叉站路由方向左
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 1);
			// 设置交叉站路由方向右
			m_pThreadProcHeadFrame->SetCrossRout(pInstrument, 2);
			// 增加一个需要设置IP地址的仪器
			m_pThreadProcHeadFrame->AddInstrumentForIPSet(pInstrument);	
		}
	}	
}

/**
* 处理内部网络命令帧；命令	3-Look
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForLook(CFrameInterface* pFrameInterface)
{
	m_pLogicData->m_oLookSetupData.m_uiLookAuto = pFrameInterface->m_uiLookAuto;	// 是否自动查找
	m_pLogicData->m_oLookSetupData.m_uiTestResistance = pFrameInterface->m_uiTestResistance;	// 新仪器是否测试Resistance
	m_pLogicData->m_oLookSetupData.m_uiTestTilt = pFrameInterface->m_uiTestTilt;	// 新仪器是否测试Tilt
	m_pLogicData->m_oLookSetupData.m_uiTestLeakage = pFrameInterface->m_uiTestLeakage;	// 新仪器是否测试Leakage
}

/**
* 处理内部网络命令帧；命令	4、5、6-基本测试命令；来自测试设置的“GO”按钮
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForTestBaseFromTestSetup(CFrameInterface* pFrameInterface)
{
	if(false == m_bWork)	// 现场停止
	{
		return;
	}
	// 处理来自测试设置的基本测试任务
	m_pThreadProcTestBase->OnTestTaskBaseFromTestSetup(pFrameInterface->m_uiTestNb);
}

/**
* 处理内部网络命令帧；命令	7-噪声监测命令；来自测试设置的“GO”按钮
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForTestNoiseFromTestSetup(CFrameInterface* pFrameInterface)
{
	if(false == m_bWork)	// 现场停止
	{
		return;
	}
	// 处理来自测试设置的噪声监测任务
	m_pThreadProcTestNoise->OnTestTaskNoise(1);
}

/**
* 处理内部网络命令帧；命令	8、9-基本测试命令；来自仪器视图或检波器视图的“GO”按钮
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForTestBaseFromTestView(CFrameInterface* pFrameInterface)
{
	if(false == m_bWork)	// 现场停止
	{
		return;
	}
	// 处理来自测试视图的基本测试任务
	m_pThreadProcTestBase->OnTestTaskBaseFromTestView(pFrameInterface->m_uiTestAim, pFrameInterface->m_uiTestType, pFrameInterface->m_strTestAbsoluteSpread);
}

/**
* 处理内部网络命令帧；命令	10-噪声监测命令；来自噪声监测视图的“Seismonitor”按钮
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForTestNoiseFromTestView(CFrameInterface* pFrameInterface)
{
	if(false == m_bWork)	// 现场停止
	{
		return;
	}
	// 判断：停止噪声监测
	if(0 == pFrameInterface->m_byTestOperation)
	{
		// 处理来自测试设置的噪声监测任务
		m_pThreadProcTestNoise->OnTestTaskNoise(0);
	}
	// 判断：启动噪声监测
	else if(1 == pFrameInterface->m_byTestOperation)
	{
		// 处理来自噪声监测视图的噪声监测任务
		m_pThreadProcTestNoise->OnTestTaskNoise(pFrameInterface->m_strTestAbsoluteSpread);
	}
}

/**
* 处理内部网络命令帧；命令	20-测线配置文件更改命令
* @param CFrameInterface* pFrameInterface 客户端发来命令帧指针
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChange(CFrameInterface* pFrameInterface)
{
	// 更改的目标	1-SurveySetup的ApplyAll；2-SurveySetup的ApplySensor
	// 更改的目标	11-LayoutSetup的ApplyBlast；12-LayoutSetup的ApplyMarkers；13-LayoutSetup的ApplyAux；14-LayoutSetup的ApplyDetour；15-LayoutSetup的ApplyMute
	// 更改的目标	21-LookSetup的Apply
	// 更改的目标	31-TestSetup的ApplyInstrument；32-TestSetup的ApplySensor；33-TestSetup的ApplyMultiple；34-TestSetup的ApplySeisMonitor
	// 更改的目标	41-TestSetup的ApplyInstrumentTestBase；42-TestSetup的ApplySensorTestBase
	// 更改的目标	51-TestSetup的ApplyInstrumentTestLimit；52-TestSetup的ApplySensorTestLimit
	switch (pFrameInterface->m_uiChangeAim)
	{
	case 1:	// 更改的目标	1-SurveySetup的ApplyAll
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	1-SurveySetup的ApplyAll
		ProcInterFaceCommandForSetupDataChangeForSurveySetupApplyAll();
		break;
	case 2:	// 更改的目标	2-SurveySetup的ApplySensor
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	1-SurveySetup的ApplySensor
		ProcInterFaceCommandForSetupDataChangeForSurveySetupApplySensor();
		break;
	case 11:	// 更改的目标	11-LayoutSetup的ApplyBlast
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	11-LayoutSetup的ApplyBlast
		ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyBlast();
		break;
	case 12:	// 更改的目标	12-LayoutSetup的ApplyMarkers
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	12-LayoutSetup的ApplyMarkers
		ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyMarkers();
		break;
	case 13:	// 更改的目标	13-LayoutSetup的ApplyAux
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	13-LayoutSetup的ApplyAux
		ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyAux();
		break;
	case 14:	// 更改的目标	14-LayoutSetup的ApplyDetour
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	14-LayoutSetup的ApplyDetour
		ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyDetour();
		break;		
	case 15:	// 更改的目标	15-LayoutSetup的ApplyMute
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	15-LayoutSetup的ApplyMute
		ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyMute();
		break;
	case 21:	// 更改的目标	21-LookSetup的Apply
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	21-LookSetup的Apply
		ProcInterFaceCommandForSetupDataChangeForLookSetupApply();
		break;
	case 31:	// 更改的目标	31-TestSetup的ApplyInstrument
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	31-TestSetup的ApplyInstrument
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrument();
		break;
	case 32:	// 更改的目标	31-TestSetup的ApplySensor
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	32-TestSetup的ApplySensor
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensor();
		break;
	case 33:	// 更改的目标	31-TestSetup的ApplyMultiple
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	33-TestSetup的ApplyMultiple
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplyMultiple();
		break;
	case 34:	// 更改的目标	34-TestSetup的ApplySeisMonitor
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	34-TestSetup的ApplySeisMonitor
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplySeisMonitor();
		break;
	case 41:	// 更改的目标	41-TestSetup的ApplyInstrumentTestBase
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	41-TestSetup的ApplyInstrumentTestBase
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrumentTestBase();
		break;
	case 42:	// 更改的目标	42-TestSetup的ApplySensorTestBase
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	42-TestSetup的ApplySensorTestBase
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensorTestBase();
		break;
	case 51:	// 更改的目标	41-TestSetup的ApplyInstrumentTestLimit
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	41-TestSetup的ApplyInstrumentTestLimit
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrumentTestLimit();
		break;
	case 52:	// 更改的目标	42-TestSetup的ApplySensorTestLimit
		// 处理内部网络命令帧；命令	20-测线配置文件更改命令	更改的目标	42-TestSetup的ApplySensorTestLimit
		ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensorTestLimit();
		break;
	}
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	1-SurveySetup的ApplyAll
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForSurveySetupApplyAll()
{
	bool bWork = m_bWork;		
	// 停止现场管理
	ProcInterFaceCommandForField((byte)0);
	// 处理配置文件更改	SurveySetup的ApplyAll
	m_pLogicData->OnSetupDataChangeForSurveySetupApplyAll();
	if(true == bWork)
	{
		// 启动现场管理
//SOS	ProcInterFaceCommandForField((byte)1);
	}
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	1-SurveySetup的ApplySensor
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForSurveySetupApplySensor()
{
	// 处理配置文件更改	SurveySetup的ApplySensor
	m_pLogicData->OnSetupDataChangeForSurveySetupApplySensor();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	11-LayoutSetup的ApplyBlast
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyBlast()
{
	if(false == m_bWork)	// 现场管理停止
	{
		// 处理配置文件更改	LayoutSetup的ApplyBlast
		m_pLogicData->OnSetupDataChangeForLayoutSetupApplyBlast();
		return;
	}
	else	// 现场管理工作中
	{	
		m_pSiteData->m_bPauseRequestForProcIPDistribute = true;	// 暂停要求，要求仪器IP地址分配线程暂停	
		if(true == WaitForProcIPDistributePause())	// 等待仪器IP地址分配线程暂停
		{
			// 处理配置文件更改	LayoutSetup的ApplyBlast
			m_pLogicData->OnSetupDataChangeForLayoutSetupApplyBlast();
		}
		m_pSiteData->m_bPauseRequestForProcIPDistribute = false;	// 暂停要求，要求仪器IP地址分配线程暂停
	}	
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	12-LayoutSetup的ApplyMarkers
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyMarkers()
{
	if(false == m_bWork)	// 现场管理停止
	{
		// 处理配置文件更改	LayoutSetup的ApplyMarkers
		m_pLogicData->OnSetupDataChangeForLayoutSetupApplyMarkers();
		return;
	}
	else	// 现场管理工作中
	{	
		m_pSiteData->m_bPauseRequestForProcIPDistribute = true;	// 暂停要求，要求仪器IP地址分配线程暂停	
		if(true == WaitForProcIPDistributePause())	// 等待仪器IP地址分配线程暂停
		{
			// 处理配置文件更改	LayoutSetup的ApplyMarkers
			m_pLogicData->OnSetupDataChangeForLayoutSetupApplyMarkers();
		}
		m_pSiteData->m_bPauseRequestForProcIPDistribute = false;	// 暂停要求，要求仪器IP地址分配线程暂停
	}	
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	13-LayoutSetup的ApplyAux
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyAux()
{
	if(false == m_bWork)	// 现场管理停止
	{
		// 处理配置文件更改	LayoutSetup的ApplyAux
		m_pLogicData->OnSetupDataChangeForLayoutSetupApplyAux();
		return;
	}
	else	// 现场管理工作中
	{	
		m_pSiteData->m_bPauseRequestForProcIPDistribute = true;	// 暂停要求，要求仪器IP地址分配线程暂停	
		if(true == WaitForProcIPDistributePause())	// 等待仪器IP地址分配线程暂停
		{
			// 处理配置文件更改	LayoutSetup的ApplyAux
			m_pLogicData->OnSetupDataChangeForLayoutSetupApplyAux();
		}
		m_pSiteData->m_bPauseRequestForProcIPDistribute = false;	// 暂停要求，要求仪器IP地址分配线程暂停
	}	
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	14-LayoutSetup的ApplyDetour
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyDetour()
{
	if(false == m_bWork)	// 现场管理停止
	{
		// 处理配置文件更改	LayoutSetup的ApplyDetour
		m_pLogicData->OnSetupDataChangeForLayoutSetupApplyDetour();
		return;
	}
	else	// 现场管理工作中
	{
		m_pSiteData->m_bPauseRequestForProcIPDistribute = true;	// 暂停要求，要求仪器IP地址分配线程暂停	
		if(true == WaitForProcIPDistributePause())	// 等待仪器IP地址分配线程暂停
		{
			// 处理配置文件更改	LayoutSetup的ApplyDetour
			m_pLogicData->OnSetupDataChangeForLayoutSetupApplyDetour();
		}
		m_pSiteData->m_bPauseRequestForProcIPDistribute = false;	// 暂停要求，要求仪器IP地址分配线程暂停
	}	
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	15-LayoutSetup的ApplyMute
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForLayoutSetupApplyMute()
{
	// 处理配置文件更改	LayoutSetup的ApplyMute
	m_pLogicData->OnSetupDataChangeForLayoutSetupApplyMute();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	21-LookSetup的Apply
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForLookSetupApply()
{
	// 处理配置文件更改	LookSetup的Apply
	m_pLogicData->OnSetupDataChangeForLookSetupApply();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	31-TestSetup的ApplyInstrument
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrument()
{
	// 处理配置文件更改	TestSetup的ApplyInstrument
	m_pLogicData->OnSetupDataChangeForTestSetupApplyInstrument();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	32-TestSetup的ApplySensor
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensor()
{
	// 处理配置文件更改	TestSetup的ApplySensor
	m_pLogicData->OnSetupDataChangeForTestSetupApplySensor();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	33-TestSetup的ApplyMultiple
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplyMultiple()
{
	// 处理配置文件更改	TestSetup的ApplyMultiple
	m_pLogicData->OnSetupDataChangeForTestSetupApplyMultiple();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	34-TestSetup的ApplySeisMonitor
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplySeisMonitor()
{
	// 处理配置文件更改	TestSetup的ApplySeisMonitor
	m_pLogicData->OnSetupDataChangeForTestSetupApplySeisMonitor();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	41-TestSetup的ApplyInstrumentTestBase
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrumentTestBase()
{
	// 处理配置文件更改	TestSetup的ApplyInstrumentTestBase
	m_pLogicData->OnSetupDataChangeForTestSetupApplyInstrumentTestBase();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	42-TestSetup的ApplySensorTestBase
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensorTestBase()
{
	// 处理配置文件更改	TestSetup的ApplySensorTestBase
	m_pLogicData->OnSetupDataChangeForTestSetupApplySensorTestBase();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	41-TestSetup的ApplyInstrumentTestLimit
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplyInstrumentTestLimit()
{
	// 处理配置文件更改	TestSetup的ApplyInstrumentTestLimit
	m_pLogicData->OnSetupDataChangeForTestSetupApplyInstrumentTestLimit();
}

/**
* 处理内部网络命令	20-测线配置文件更改命令 更改的目标	更改的目标	42-TestSetup的ApplySensorTestLimit
* @param void
* @return void
*/
void CNetProcInterface::ProcInterFaceCommandForSetupDataChangeForTestSetupApplySensorTestLimit()
{
	// 处理配置文件更改	TestSetup的ApplySensorTestLimit
	m_pLogicData->OnSetupDataChangeForTestSetupApplySensorTestLimit();
}

/**
* 等待仪器IP地址分配线程暂停
* @param void
* @return bool true：成功；false：失败
*/
bool CNetProcInterface::WaitForProcIPDistributePause()
{	
	int iWaitCount = 0;	// 等待次数
	while(true)
	{	
		if(true == m_pSiteData->m_bProcIPDistributePause)	// 仪器IP地址分配线程暂停
		{
			return true;
		}			
		iWaitCount++;	// 等待次数
		Sleep(50);	// 休眠			
		if(5 <= iWaitCount)	// 等待次数
		{
			return false;
		}
	}
	return false;
}

/**
* 发送服务器Field命令
* @param byte byFieldOperation FieldON/OFF 0：OFF；1：ON
* @return void
*/
void CNetProcInterface::SendServerFieldCmd(byte byFieldOperation)
{
	// 为服务器端命令帧发送网络接口准备的线程同步对象锁定
	m_pSiteData->m_oCriticalSectionInterfaceCmdSend.Lock();
	// 生成现场数据输出通知命令帧
	m_pSocketInterfaceCmdSend->m_oFrameInterface.m_byFieldOperation = byFieldOperation;
	m_pSocketInterfaceCmdSend->m_oFrameInterface.MakeServerFieldFrame();
	// 发送命令帧
	TRACE("SendServerFieldCmd\r\n");
	m_pSocketInterfaceCmdSend->SendCommandFrame();
	// 为服务器端命令帧发送网络接口准备的线程同步对象解锁
	m_pSiteData->m_oCriticalSectionInterfaceCmdSend.Unlock();
}
