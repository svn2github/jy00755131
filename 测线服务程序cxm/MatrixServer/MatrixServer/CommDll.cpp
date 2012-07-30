#include "StdAfx.h"
#include "CommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);
static CCommDll* m_pCommDll;
// 处理查询接收区域命令
void CALLBACK ProcRecCmd(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread)
{
	// 客户端登陆验证（帧内容为验证码）
	if (usCmd == CmdClientConnect)
	{
		// 与连接校验码相匹配
		if (0 == strcmp(pChar, CommCheck))
		{
			pRecThread->m_bCheckConnected = true;
		}
	}
	else
	{
		if (pRecThread->m_bCheckConnected == false)
		{
			return;
		}
		switch (usCmd)
		{
		case CmdQueryRevSection:
			m_pCommDll->OnProcQueryRevSection(pRecThread);
			break;
			// 客户端心跳命令（帧内容为空）
		case CmdSetHeartBeat:
			m_pCommDll->OnProcInstrumentTableUpdate(pRecThread);
			break;
		case CmdQueryWholeTable:
			pRecThread->m_oInstrumentWholeTableMap.clear();
			m_pCommDll->OnProcInstrumentTableUpdate(pRecThread);
			break;
			// 上电（命令字后帧内容为空，返回值为执行FieldOn剩余时间，为0表示无需等待）
		case CmdSetFieldOn:
			m_pCommDll->OnProcSetFieldOn(pRecThread);
			break;
			// 断电（命令字后帧内容为空）
		case CmdSetFieldOff:
			m_pCommDll->OnProcSetFieldOff(pRecThread);
			break;
// 			// 查询所选仪器全部信息（帧内容为行号+区域号）
// 		case CmdQueryInstrumentInfo:
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrumentInfoByArea);
// 			break;
// 			// 查询全部仪器的全部信息（应答帧内容为仪器结构体）
// 		case CmdQueryInstrumentInfoAll:
// 			OnProcQueryInfoAll(&CClientRecThread::QueryInstrumentInfoByArea);
// 			break;
// 
// 			// 查询 SurveyXML 文件信息（帧内容为空）
// 		case CmdQuerySurveyXMLInfo:
// 			OnProcQuerySurveyXMLInfo(usCmd);
// 			break;
// 			// 设置 SurveyXML 文件信息（帧内容为信息结构体）
// 		case CmdSetSurveyXMLInfo:
// 			m_pMatrixDllCall->Dll_SetSurverySetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 PointCode XML文件信息（帧内容为空）
// 		case CmdQueryPointCodeXMLInfo:
// 			OnProcQueryPointCodeXMLInfo(usCmd);
// 			break;
// 			// 设置 PointCode XML文件信息（帧内容为信息结构体）
// 		case CmdSetPointCodeXMLInfo:
// 			m_pMatrixDllCall->Dll_SetPointCodeSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Sensor XML文件信息（帧内容为空）
// 		case CmdQuerySensorXMLInfo:
// 			OnProcQuerySensorXMLInfo(usCmd);
// 			break;
// 			// 设置 Sensor XML文件信息（帧内容为信息结构体）
// 		case CmdSetSensorXMLInfo:
// 			m_pMatrixDllCall->Dll_SetSensorSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Marker XML文件信息（帧内容为空）
// 		case CmdQueryMarkerXMLInfo:
// 			OnProcQueryMarkerXMLInfo(usCmd);
// 			break;
// 			// 设置 Marker XML文件信息（帧内容为信息结构体）
// 		case CmdSetMarkerXMLInfo:
// 			m_pMatrixDllCall->Dll_SetMarkerSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Aux XML文件信息（帧内容为空）
// 		case CmdQueryAuxXMLInfo:
// 			OnProcQueryAuxXMLInfo(usCmd);
// 			break;
// 			// 设置 Aux XML文件信息（帧内容为信息结构体）
// 		case CmdSetAuxXMLInfo:
// 			m_pMatrixDllCall->Dll_SetAuxSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Detour XML文件信息（帧内容为空）
// 		case CmdQueryDetourXMLInfo:
// 			OnProcQueryDetourXMLInfo(usCmd);
// 			break;
// 			// 设置 Detour XML文件信息（帧内容为信息结构体）
// 		case CmdSetDetourXMLInfo:
// 			m_pMatrixDllCall->Dll_SetDetourSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Mute XML文件信息（帧内容为空）
// 		case CmdQueryMuteXMLInfo:
// 			OnProcQueryMuteXMLInfo(usCmd);
// 			break;
// 			// 设置 Mute XML文件信息（帧内容为信息结构体）
// 		case CmdSetMuteXMLInfo:
// 			m_pMatrixDllCall->Dll_SetMuteSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 BlastMachine XML文件信息（帧内容为空）
// 		case CmdQueryBlastMachineXMLInfo:
// 			OnProcQueryBlastMachineXMLInfo(usCmd);
// 			break;
// 			// 设置 BlastMachine XML文件信息（帧内容为信息结构体）
// 		case CmdSetBlastMachineXMLInfo:
// 			m_pMatrixDllCall->Dll_SetBlastMachineSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Absolute XML文件信息（帧内容为空）
// 		case CmdQueryAbsoluteXMLInfo:
// 			OnProcQueryAbsoluteXMLInfo(usCmd);
// 			break;
// 			// 设置 Absolute XML文件信息（帧内容为信息结构体）
// 		case CmdSetAbsoluteXMLInfo:
// 			m_pMatrixDllCall->Dll_SetAbsoluteSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break; 
// 			// 查询 Generic XML文件信息（帧内容为空）
// 		case CmdQueryGenericXMLInfo:
// 			OnProcQueryGenericXMLInfo(usCmd);
// 			break;
// 			// 设置 Generic XML文件信息（帧内容为信息结构体）
// 		case CmdSetGenericXMLInfo:
// 			m_pMatrixDllCall->Dll_SetGenericSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 Look XML文件信息（帧内容为空）
// 		case CmdQueryLookXMLInfo:
// 			OnProcQueryLookXMLInfo(usCmd);
// 			break;
// 			// 设置 Look XML文件信息（帧内容为信息结构体）
// 		case CmdSetLookXMLInfo:
// 			m_pMatrixDllCall->Dll_SetLookSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 InstrumentTestBase XML文件信息（帧内容为空）
// 		case CmdQueryInstrumentTestBaseXMLInfo:
// 			OnProcQueryInstrumentTestBaseXMLInfo(usCmd);
// 			break;
// 			// 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）
// 		case CmdSetInstrumentTestBaseXMLInfo:
// 			m_pMatrixDllCall->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, true);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 SensorTestBase XML文件信息（帧内容为空）
// 		case CmdQuerySensorTestBaseXMLInfo:
// 			OnProcQuerySensorTestBaseXMLInfo(usCmd);
// 			break;
// 			// 设置 SensorTestBase XML文件信息（帧内容为信息结构体）
// 		case CmdSetSensorTestBaseXMLInfo:
// 			m_pMatrixDllCall->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, false);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 InstrumentTestLimit XML文件信息（帧内容为空）
// 		case CmdQueryInstrumentTestLimitXMLInfo:
// 			OnProcQueryInstrumentTestLimitXMLInfo(usCmd);
// 			break;
// 			// 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）
// 		case CmdSetInstrumentTestLimitXMLInfo:
// 			m_pMatrixDllCall->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, true);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 SensorTestLimit XML文件信息（帧内容为空）
// 		case CmdQuerySensorTestLimitXMLInfo:
// 			OnProcQuerySensorTestLimitXMLInfo(usCmd);
// 			break;
// 			// 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）
// 		case CmdSetSensorTestLimitXMLInfo:
// 			m_pMatrixDllCall->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, false);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 InstrumentTest XML文件信息（帧内容为空）
// 		case CmdQueryInstrumentTestXMLInfo:
// 			OnProcQueryInstrumentTestXMLInfo(usCmd);
// 			break;
// 			// 设置 InstrumentTest XML文件信息（帧内容为信息结构体）
// 		case CmdSetInstrumentTestXMLInfo:
// 			m_pMatrixDllCall->Dll_SetInstrumentTestSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 SensorTest XML文件信息（帧内容为空）
// 		case CmdQuerySensorTestXMLInfo:
// 			OnProcQuerySensorTestXMLInfo(usCmd);
// 			break;
// 			// SensorTest XML文件信息（帧内容为信息结构体）
// 		case CmdSetSensorTestXMLInfo:
// 			m_pMatrixDllCall->Dll_SetSensorSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 MultipleTest XML文件信息（帧内容为空）
// 		case CmdQueryMultipleTestXMLInfo:
// 			OnProcQueryMultipleTestXMLInfo(usCmd);
// 			break;
// 			// 设置 MultipleTest XML文件信息（帧内容为信息结构体）
// 		case CmdSetMultipleTestXMLInfo:
// 			m_pMatrixDllCall->Dll_SetMultipleTestSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 SeisMonitorTest XML文件信息（帧内容为空）
// 		case CmdQuerySeisMonitorTestXMLInfo:
// 			OnProcQuerySeisMonitorTestXMLInfo(usCmd);
// 			break;
// 			// 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）
// 		case CmdSetSeisMonitorTestXMLInfo:
// 			m_pMatrixDllCall->Dll_SetSeisMonitorSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 LAULeakage XML文件信息（帧内容为空）
// 		case CmdQueryLAULeakageXMLInfo:
// 			OnProcQueryLAULeakageXMLInfo(usCmd);
// 			break;
// 			// 设置 LAULeakage XML文件信息（帧内容为信息结构体）
// 		case CmdSetLAULeakageXMLInfo:
// 			m_pMatrixDllCall->Dll_SetLAULeakageSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 			// 查询 FormLine XML文件信息（帧内容为空）
// 		case CmdQueryFormLineXMLInfo:
// 			OnProcQueryFormLineXMLInfo(usCmd);
// 			break;
// 			// 设置 FormLine XML文件信息（帧内容为信息结构体）
// 		case CmdSetFormLineXMLInfo:
// 			m_pMatrixDllCall->Dll_SetFormLineSetupData(pChar, uiSize);
// 			BroadCastXMLChange(usCmd, pChar, uiSize);
// 			break;
// 
// 			// 查询所选区域仪器噪声测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQueryInstrNoiseTestArea:
// 			m_fInstrumentNoiseLimit = GetTestDataLimitFromXML(true, "Noise");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrNoiseTestByArea);
// 			break;
// 			// 查询全部仪器的仪器噪声测试数据和测试结果（帧内容为空）
// 		case CmdQueryInstrNoiseTestAll:
// 			m_fInstrumentNoiseLimit = GetTestDataLimitFromXML(true, "Noise");
// 			OnProcQueryInfoAll(&CClientRecThread::QueryInstrNoiseTestByArea);
// 			break;
// 			// 查询所选区域仪器失真测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQueryInstrDistortionTestArea:
// 			m_fInstrumentDistortionLimit = GetTestDataLimitFromXML(true, "Distortion");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrDistortionTestByArea);
// 			break;
// 			// 查询全部仪器失真测试数据和测试结果（帧内容为空）
// 		case CmdQueryInstrDistortionTestAll:
// 			m_fInstrumentDistortionLimit = GetTestDataLimitFromXML(true, "Distortion");
// 			OnProcQueryInfoAll(&CClientRecThread::QueryInstrDistortionTestByArea);
// 			break;
// 			// 查询所选区域仪器串扰测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQueryInstrCrosstalkTestArea:
// 			m_fInstrumentCrosstalkLimit = GetTestDataLimitFromXML(true, "Crosstalk");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrCrosstalkTestByArea);
// 			break;
// 			// 查询全部仪器串扰测试数据和测试结果（帧内容为空）
// 		case CmdQueryInstrCrosstalkTestAll:
// 			m_fInstrumentCrosstalkLimit = GetTestDataLimitFromXML(true, "Crosstalk");
// 			OnProcQueryInfoAll(&CClientRecThread::QueryInstrCrosstalkTestByArea);
// 			break;
// 			// 查询所选区域仪器共模抑制比测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQueryInstrCMRRTestArea:
// 			m_fInstrumentCMRRLimit = GetTestDataLimitFromXML(true, "CMRR");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrCMRRTestByArea);
// 			break;
// 			// 查询全部仪器共模抑制比测试数据和测试结果（帧内容为空）
// 		case CmdQueryInstrCMRRTestAll:
// 			m_fInstrumentCMRRLimit = GetTestDataLimitFromXML(true, "CMRR");
// 			OnProcQueryInfoAll(&CClientRecThread::QueryInstrCMRRTestByArea);
// 			break;
// 			// 查询所选区域仪器增益相位测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQueryInstrGainPhaseTestArea:
// 			m_fInstrumentGainPhaseLimit = GetTestDataLimitFromXML(true, "GainPhase");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QueryInstrGainPhaseTestByArea);
// 			break;
// 			// 查询全部仪器增益相位测试数据和测试结果（帧内容为空）
// 		case CmdQueryInstrGainPhaseTestAll:
// 			m_fInstrumentGainPhaseLimit = GetTestDataLimitFromXML(true, "GainPhase");
// 			OnProcQueryInfoAll(&CClientRecThread::QueryInstrGainPhaseTestByArea);
// 			break;
// 			// 查询所选区域检波器阻抗测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQuerySensorResistanceTestArea:
// 			m_fSensorResistanceLimitMin = GetTestDataLimitFromXML(false, "ResistanceMin");
// 			m_fSensorResistanceLimitMax = GetTestDataLimitFromXML(false, "ResistanceMax");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorResistanceTestByArea);
// 			break;
// 			// 查询全部检波器阻抗测试数据和测试结果（帧内容为空）
// 		case CmdQuerySensorResistanceTestAll:
// 			m_fSensorResistanceLimitMin = GetTestDataLimitFromXML(false, "ResistanceMin");
// 			m_fSensorResistanceLimitMax = GetTestDataLimitFromXML(false, "ResistanceMax");
// 			OnProcQueryInfoAll(&CClientRecThread::QuerySensorResistanceTestByArea);
// 			break;
// 			// 查询所选区域检波器漏电测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQuerySensorLeakageTestArea:
// 			m_fSensorLeakageLimit = GetTestDataLimitFromXML(false, "Leakage");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorLeakageTestByArea);
// 			break;
// 			// 查询全部检波器漏电测试数据和测试结果（帧内容为空）
// 		case CmdQuerySensorLeakageTestAll:
// 			m_fSensorLeakageLimit = GetTestDataLimitFromXML(false, "Leakage");
// 			OnProcQueryInfoAll(&CClientRecThread::QuerySensorLeakageTestByArea);
// 			break;
// 			// 查询所选区域检波器噪声测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQuerySensorNoiseTestArea:
// 			m_fSensorNoiseLimit = GetTestDataLimitFromXML(false, "Noise");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorNoiseTestByArea);
// 			break;
// 			// 查询全部检波器噪声测试数据和测试结果（帧内容为空）
// 		case CmdQuerySensorNoiseTestAll:
// 			m_fSensorNoiseLimit = GetTestDataLimitFromXML(false, "Noise");
// 			OnProcQueryInfoAll(&CClientRecThread::QuerySensorNoiseTestByArea);
// 			break;
// 			// 查询所选区域检波器倾斜度测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQuerySensorTiltTestArea:
// 			m_fSensorTiltLimit = GetTestDataLimitFromXML(false, "Tilt");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorTiltTestByArea);
// 			break;
// 			// 查询全部检波器倾斜度测试数据和测试结果（帧内容为空）
// 		case CmdQuerySensorTiltTestAll:
// 			m_fSensorTiltLimit = GetTestDataLimitFromXML(false, "Tilt");
// 			OnProcQueryInfoAll(&CClientRecThread::QuerySensorTiltTestByArea);
// 			break;
// 			// 查询所选区域检波器倾斜度模式测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQuerySensorTiltModelTestArea:
// 			m_fSensorTiltModelLimit = GetTestDataLimitFromXML(false, "Tilt Model");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySensorTiltModelTestByArea);
// 			break;
// 			// 查询全部检波器倾斜度模式测试数据和测试结果（帧内容为空）
// 		case CmdQuerySensorTiltModelTestAll:
// 			m_fSensorTiltModelLimit = GetTestDataLimitFromXML(false, "Tilt Model");
// 			OnProcQueryInfoAll(&CClientRecThread::QuerySensorTiltModelTestByArea);
// 			break;
// 			// 查询所选区域地震监测测试数据和测试结果（帧内容为行号+区域号）
// 		case CmdQuerySeisMonitorTestArea:
// 			m_fSeisMonitorLimit = GetTestDataLimitFromXML(false, "Seismonitor");
// 			OnProcQueryByArea(pChar, uiSize, &CClientRecThread::QuerySeisMonitorTestByArea);
// 			break;
// 			// 查询全部地震监测测试数据和测试结果（帧内容为空）
// 		case CmdQuerySeisMonitorTestAll:
// 			m_fSeisMonitorLimit = GetTestDataLimitFromXML(false, "Seismonitor");
// 			OnProcQueryInfoAll(&CClientRecThread::QuerySeisMonitorTestByArea);
// 			break;
		default:
			break;
		}
 	}
}

CCommDll::CCommDll(void)
	: m_uiLineNum(0)
	, m_uiColumnNum(0)
	, m_fInstrumentNoiseLimit(0)
	, m_fInstrumentDistortionLimit(0)
	, m_fInstrumentCrosstalkLimit(0)
	, m_fInstrumentGainPhaseLimit(0)
	, m_fInstrumentCMRRLimit(0)
	, m_fSensorResistanceLimitMin(0)
	, m_fSensorResistanceLimitMax(0)
	, m_fSensorLeakageLimit(0)
	, m_fSensorNoiseLimit(0)
	, m_fSensorTiltLimit(0)
	, m_fSensorTiltModelLimit(0)
	, m_fSeisMonitorLimit(0)
{
	m_pMatrixDllCall = NULL;
	m_pMatrixCommDll = NULL;
	m_pCommServer = NULL;
	m_pCommDll = this;
}


CCommDll::~CCommDll(void)
{
}
// 载入MatrixServerDll动态链接库
void CCommDll::LoadMatrixCommDll(CString strPath)
{
	CString str = _T("");
	m_hCommDll = ::LoadLibrary(strPath);
	if (m_hCommDll == NULL)
	{
		str.Format(_T("载入失败！错误码为%d。"), GetLastError());
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
}
// 释放MatrixServerDll动态链接库
void CCommDll::FreeMatrixCommDll(void)
{
	::FreeLibrary(m_hCommDll);
}

// 创建服务端通讯
void CCommDll::OnCreateServerComm()
{
	CREATEFN pfn = NULL;
	pfn = (CREATEFN)GetProcAddress(m_hCommDll, "CreateMatrixCommDll");
	if (!pfn)
	{
		FreeLibrary(m_hCommDll);
		PostQuitMessage(0);
	}
	else
	{
		m_pMatrixCommDll = (*pfn)();
		m_pMatrixCommDll->OnInit();
		m_pCommServer = m_pMatrixCommDll->CreateCommServer();
		m_pCommServer->m_oProcRecCmdCallBack = ProcRecCmd;
		m_pCommServer->OnInit();
	}
}
// 释放服务端通讯
void CCommDll::OnDeleteServerComm()
{
	DELETEFN pfn = NULL;
	pfn = (DELETEFN)GetProcAddress(m_hCommDll, "DeleteMatrixCommDll");
	if (!pfn)
	{
		FreeLibrary(m_hCommDll);
		PostQuitMessage(0);
	}
	else
	{
		m_pCommServer->OnClose();
		m_pMatrixCommDll->OnClose();
		m_pMatrixCommDll->DeleteCommServer(m_pCommServer);
		(*pfn)(m_pMatrixCommDll);
	}
}
// 初始化
void CCommDll::OnInit(CString strPath)
{
	LoadMatrixCommDll(strPath);
	OnCreateServerComm();
}
// 关闭
void CCommDll::OnClose(void)
{
	OnDeleteServerComm();
	FreeMatrixCommDll();
}

// 处理查询接收区域命令
void CCommDll::OnProcQueryRevSection(CCommRecThread* pRecThread)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_GetLineRevSection(m_uiLineNum, m_uiColumnNum);
	memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &m_uiLineNum, 4);
	iPos += 4;
	memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &m_uiColumnNum, 4);
	iPos += 4;
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryRevSection, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 由线号和点号得到区域位置
void CCommDll::GetAreaFromPoint(int iLineIndex, int iPointIndex, m_oAreaStruct* pAreaStruct)
{
	pAreaStruct->m_uiLineNb = m_uiLineNum + iLineIndex;
	pAreaStruct->m_uiAreaNb = m_uiColumnNum + iPointIndex;
	if ((pAreaStruct->m_uiAreaNb % InstrumentTableWindowSize) == 0)
	{
		pAreaStruct->m_uiAreaNb /= InstrumentTableWindowSize;
	}
	else
	{
		pAreaStruct->m_uiAreaNb /= InstrumentTableWindowSize;
		pAreaStruct->m_uiAreaNb += 1;
	}
}
// 由区域号得到线号及点号范围
void CCommDll::GetPointRangeFromArea(int* iLineIndex, int* iPointMinIndex, 
	int* iPointMaxIndex, m_oAreaStruct* pAreaStruct)
{
	*iLineIndex = (int)pAreaStruct->m_uiLineNb - m_uiLineNum;
	*iPointMinIndex = ((int)pAreaStruct->m_uiAreaNb - 1) * InstrumentTableWindowSize + 1 - m_uiColumnNum;
	*iPointMaxIndex = ((int)pAreaStruct->m_uiAreaNb) * InstrumentTableWindowSize - m_uiColumnNum;
}

// 处理仪器设备表更新
void CCommDll::OnProcInstrumentTableUpdate(CCommRecThread* pRecThread)
{
  	map<CCommRecThread::m_oLocationStruct, unsigned int>::iterator iterLocation;
  	map<m_oAreaStruct, m_oAreaStruct>::iterator iterArea;
	int iLineIndex = 0;
	int iPointIndex = 0;
	unsigned int uiSN = 0;
	unsigned int uiSNClient = 0;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	int iPos = 0;
 	int iSize = 0;
	m_oAreaStruct oAreaStruct;
	m_oInstrumentStruct* pInstrument = NULL;
	pRecThread->m_oInstrumentUpdateArea.clear();
	// 将SN索引表与客户端的仪器位置索引表相对照
	m_pMatrixDllCall->Dll_QueryInstrumentLocation(pRecThread->m_pCommSndFrame->m_cProcBuf, iSize);
	for (int i=0; i<iSize;)
	{
		memcpy(&iLineIndex, &pRecThread->m_pCommSndFrame->m_cProcBuf[i], 4);
		i += 4;
		memcpy(&iPointIndex, &pRecThread->m_pCommSndFrame->m_cProcBuf[i], 4);
		i += 4;
		memcpy(&uiSN, &pRecThread->m_pCommSndFrame->m_cProcBuf[i], 4);
		i += 4;
		// 客户端仪器索引表中找不到该设备
		if (FALSE == pRecThread->IfLocationExistInMap(iLineIndex, iPointIndex))
		{
			pRecThread->AddLocationToMap(iLineIndex, iPointIndex, uiSN);
			GetAreaFromPoint(iLineIndex, iPointIndex, &oAreaStruct);
			// 增加对象到索引表
			pRecThread->AddAreaToMap(oAreaStruct);
		}
		else
		{
			uiSNClient = pRecThread->GetSnFromLocationMap(iLineIndex, iPointIndex);
			if (uiSN != uiSNClient)
			{
				uiSNClient = uiSN;
				GetAreaFromPoint(iLineIndex, iPointIndex, &oAreaStruct);
				// 增加对象到索引表
				pRecThread->AddAreaToMap(oAreaStruct);
			}
		}
	}
 	EnterCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
 	// 将客户端的仪器位置索引表与SN索引表相对照
	for (iterLocation = pRecThread->m_oInstrumentWholeTableMap.begin(); iterLocation != pRecThread->m_oInstrumentWholeTableMap.end();)
	{
		// 仪器IP地址索引表找不到该设备
		if (FALSE == m_pMatrixDllCall->Dll_IfLocationExistInMap(iterLocation->first.m_iLineIndex, 
			iterLocation->first.m_iPointIndex, &m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap))
		{
			// 增加对象到索引表
			GetAreaFromPoint(iterLocation->first.m_iLineIndex, iterLocation->first.m_iPointIndex, &oAreaStruct);
			pRecThread->AddAreaToMap(oAreaStruct);
			pRecThread->m_oInstrumentWholeTableMap.erase(iterLocation++);
		}
		else
		{
			uiSN = m_pMatrixDllCall->Dll_GetInstrumentFromLocationMap(iterLocation->first.m_iLineIndex, 
				iterLocation->first.m_iPointIndex, &m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap)->m_uiSN;
			// SN不同则更新
			if (uiSN != iterLocation->second)
			{
				iterLocation->second = uiSN;
			}
			iterLocation++;
		}
	}
 	LeaveCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	// 对照后客户端仪器索引表与服务端仪器SN索引表一致，将变化区域发送到客户端
	for (iterArea = pRecThread->m_oInstrumentUpdateArea.begin(); iterArea != pRecThread->m_oInstrumentUpdateArea.end(); iterArea++)
	{
		// 由区域号得到线号及点号范围
		GetPointRangeFromArea(&iLineIndex, &iPointMinIndex, &iPointMaxIndex, &iterArea->second);
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &iterArea->second.m_uiLineNb, 4);
		iPos += 4;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &iterArea->second.m_uiAreaNb, 4);
		iPos += 4;
		for (int i = iPointMinIndex; i <= iPointMaxIndex; i++)
		{
			pInstrument = m_pMatrixDllCall->Dll_GetInstrumentFromLocationMap(iLineIndex, i, 
				&m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
			if (pInstrument != NULL)
			{
				memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &pInstrument->m_uiSN, 4);
				iPos += 4;
				memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &pInstrument->m_uiIP, 4);
				iPos += 4;
			}
			else
			{
				memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], 0, 8);
				iPos += 8;
			}
		}
	}
	if (iPos > 0)
	{
		pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryUpdateTable, 
			pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	}
}

// 处理上电
void CCommDll::OnProcSetFieldOn(CCommRecThread* pRecThread)
{
	unsigned int uiFieldOnNeedTime = 0;;
	uiFieldOnNeedTime = m_pMatrixDllCall->Dll_Work();
	memcpy(pRecThread->m_pCommSndFrame->m_cProcBuf, &uiFieldOnNeedTime, 4);
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdFieldOnWaitTime, 
		pRecThread->m_pCommSndFrame->m_cProcBuf, 4);
}
// 处理断电
void CCommDll::OnProcSetFieldOff(CCommRecThread* pRecThread)
{
	m_pMatrixDllCall->Dll_Stop();
	pRecThread->m_oInstrumentWholeTableMap.clear();
	pRecThread->m_oInstrumentUpdateArea.clear();
}