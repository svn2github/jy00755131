#include "StdAfx.h"
#include "CommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);
static CCommDll* m_pCommDll;
// 处理查询接收区域命令
void CALLBACK ProcRecCmd(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread)
{
	m_pCommDll->OnProcRecCmd(usCmd, pChar, uiSize, pRecThread);
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
/** 命令字解析*/
void CCommDll::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread)
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
			OnProcQueryRevSection(pRecThread);
			break;
			// 客户端心跳命令（帧内容为空）
		case CmdSetHeartBeat:
			OnProcInstrumentTableUpdate(pRecThread);
			break;
		case CmdQueryWholeTable:
			pRecThread->m_oInstrumentWholeTableMap.clear();
			OnProcInstrumentTableUpdate(pRecThread);
			break;
			// 上电（命令字后帧内容为空，返回值为执行FieldOn剩余时间，为0表示无需等待）
		case CmdSetFieldOn:
			OnProcSetFieldOn(pRecThread);
			break;
			// 断电（命令字后帧内容为空）
		case CmdSetFieldOff:
			OnProcSetFieldOff(pRecThread);
			break;
			// 查询所选仪器全部信息（帧内容为行号+区域号）
		case CmdQueryInstrumentInfo:
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QueryInstrumentInfoByArea);
			break;
			// 查询全部仪器的全部信息（应答帧内容为仪器结构体）
		case CmdQueryInstrumentInfoAll:
			OnProcQueryInfoAll(pRecThread, &CCommDll::QueryInstrumentInfoByArea);
			break;

			// 查询 SurveyXML 文件信息（帧内容为空）
		case CmdQuerySurveyXMLInfo:
			OnProcQuerySurveyXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SurveyXML 文件信息（帧内容为信息结构体）
		case CmdSetSurveyXMLInfo:
			m_pMatrixDllCall->Dll_SetSurverySetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 PointCode XML文件信息（帧内容为空）
		case CmdQueryPointCodeXMLInfo:
			OnProcQueryPointCodeXMLInfo(pRecThread, usCmd);
			break;
			// 设置 PointCode XML文件信息（帧内容为信息结构体）
		case CmdSetPointCodeXMLInfo:
			m_pMatrixDllCall->Dll_SetPointCodeSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Sensor XML文件信息（帧内容为空）
		case CmdQuerySensorXMLInfo:
			OnProcQuerySensorXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Sensor XML文件信息（帧内容为信息结构体）
		case CmdSetSensorXMLInfo:
			m_pMatrixDllCall->Dll_SetSensorSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Marker XML文件信息（帧内容为空）
		case CmdQueryMarkerXMLInfo:
			OnProcQueryMarkerXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Marker XML文件信息（帧内容为信息结构体）
		case CmdSetMarkerXMLInfo:
			m_pMatrixDllCall->Dll_SetMarkerSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Aux XML文件信息（帧内容为空）
		case CmdQueryAuxXMLInfo:
			OnProcQueryAuxXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Aux XML文件信息（帧内容为信息结构体）
		case CmdSetAuxXMLInfo:
			m_pMatrixDllCall->Dll_SetAuxSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Detour XML文件信息（帧内容为空）
		case CmdQueryDetourXMLInfo:
			OnProcQueryDetourXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Detour XML文件信息（帧内容为信息结构体）
		case CmdSetDetourXMLInfo:
			m_pMatrixDllCall->Dll_SetDetourSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Mute XML文件信息（帧内容为空）
		case CmdQueryMuteXMLInfo:
			OnProcQueryMuteXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Mute XML文件信息（帧内容为信息结构体）
		case CmdSetMuteXMLInfo:
			m_pMatrixDllCall->Dll_SetMuteSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 BlastMachine XML文件信息（帧内容为空）
		case CmdQueryBlastMachineXMLInfo:
			OnProcQueryBlastMachineXMLInfo(pRecThread, usCmd);
			break;
			// 设置 BlastMachine XML文件信息（帧内容为信息结构体）
		case CmdSetBlastMachineXMLInfo:
			m_pMatrixDllCall->Dll_SetBlastMachineSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Absolute XML文件信息（帧内容为空）
		case CmdQueryAbsoluteXMLInfo:
			OnProcQueryAbsoluteXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Absolute XML文件信息（帧内容为信息结构体）
		case CmdSetAbsoluteXMLInfo:
			m_pMatrixDllCall->Dll_SetAbsoluteSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break; 
			// 查询 Generic XML文件信息（帧内容为空）
		case CmdQueryGenericXMLInfo:
			OnProcQueryGenericXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Generic XML文件信息（帧内容为信息结构体）
		case CmdSetGenericXMLInfo:
			m_pMatrixDllCall->Dll_SetGenericSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Look XML文件信息（帧内容为空）
		case CmdQueryLookXMLInfo:
			OnProcQueryLookXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Look XML文件信息（帧内容为信息结构体）
		case CmdSetLookXMLInfo:
			m_pMatrixDllCall->Dll_SetLookSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTestBase XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestBaseXMLInfo:
			OnProcQueryInstrumentTestBaseXMLInfo(pRecThread, usCmd);
			break;
			// 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestBaseXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, true);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTestBase XML文件信息（帧内容为空）
		case CmdQuerySensorTestBaseXMLInfo:
			OnProcQuerySensorTestBaseXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SensorTestBase XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestBaseXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, false);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTestLimit XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestLimitXMLInfo:
			OnProcQueryInstrumentTestLimitXMLInfo(pRecThread, usCmd);
			break;
			// 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestLimitXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, true);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTestLimit XML文件信息（帧内容为空）
		case CmdQuerySensorTestLimitXMLInfo:
			OnProcQuerySensorTestLimitXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestLimitXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, false);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTest XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestXMLInfo:
			OnProcQueryInstrumentTestXMLInfo(pRecThread, usCmd);
			break;
			// 设置 InstrumentTest XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestXMLInfo:
			m_pMatrixDllCall->Dll_SetInstrumentTestSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTest XML文件信息（帧内容为空）
		case CmdQuerySensorTestXMLInfo:
			OnProcQuerySensorTestXMLInfo(pRecThread, usCmd);
			break;
			// SensorTest XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestXMLInfo:
			m_pMatrixDllCall->Dll_SetSensorSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 MultipleTest XML文件信息（帧内容为空）
		case CmdQueryMultipleTestXMLInfo:
			OnProcQueryMultipleTestXMLInfo(pRecThread, usCmd);
			break;
			// 设置 MultipleTest XML文件信息（帧内容为信息结构体）
		case CmdSetMultipleTestXMLInfo:
			m_pMatrixDllCall->Dll_SetMultipleTestSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SeisMonitorTest XML文件信息（帧内容为空）
		case CmdQuerySeisMonitorTestXMLInfo:
			OnProcQuerySeisMonitorTestXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）
		case CmdSetSeisMonitorTestXMLInfo:
			m_pMatrixDllCall->Dll_SetSeisMonitorSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 LAULeakage XML文件信息（帧内容为空）
		case CmdQueryLAULeakageXMLInfo:
			OnProcQueryLAULeakageXMLInfo(pRecThread, usCmd);
			break;
			// 设置 LAULeakage XML文件信息（帧内容为信息结构体）
		case CmdSetLAULeakageXMLInfo:
			m_pMatrixDllCall->Dll_SetLAULeakageSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 FormLine XML文件信息（帧内容为空）
		case CmdQueryFormLineXMLInfo:
			OnProcQueryFormLineXMLInfo(pRecThread, usCmd);
			break;
			// 设置 FormLine XML文件信息（帧内容为信息结构体）
		case CmdSetFormLineXMLInfo:
			m_pMatrixDllCall->Dll_SetFormLineSetupData(pChar, uiSize);
			pRecThread->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;

			// 查询所选区域仪器噪声测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrNoiseTestArea:
			m_fInstrumentNoiseLimit = GetTestDataLimitFromXML(true, "Noise");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QueryInstrNoiseTestByArea);
			break;
			// 查询全部仪器的仪器噪声测试数据和测试结果（帧内容为空）
		case CmdQueryInstrNoiseTestAll:
			m_fInstrumentNoiseLimit = GetTestDataLimitFromXML(true, "Noise");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QueryInstrNoiseTestByArea);
			break;
			// 查询所选区域仪器失真测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrDistortionTestArea:
			m_fInstrumentDistortionLimit = GetTestDataLimitFromXML(true, "Distortion");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QueryInstrDistortionTestByArea);
			break;
			// 查询全部仪器失真测试数据和测试结果（帧内容为空）
		case CmdQueryInstrDistortionTestAll:
			m_fInstrumentDistortionLimit = GetTestDataLimitFromXML(true, "Distortion");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QueryInstrDistortionTestByArea);
			break;
			// 查询所选区域仪器串扰测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrCrosstalkTestArea:
			m_fInstrumentCrosstalkLimit = GetTestDataLimitFromXML(true, "Crosstalk");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QueryInstrCrosstalkTestByArea);
			break;
			// 查询全部仪器串扰测试数据和测试结果（帧内容为空）
		case CmdQueryInstrCrosstalkTestAll:
			m_fInstrumentCrosstalkLimit = GetTestDataLimitFromXML(true, "Crosstalk");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QueryInstrCrosstalkTestByArea);
			break;
			// 查询所选区域仪器共模抑制比测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrCMRRTestArea:
			m_fInstrumentCMRRLimit = GetTestDataLimitFromXML(true, "CMRR");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QueryInstrCMRRTestByArea);
			break;
			// 查询全部仪器共模抑制比测试数据和测试结果（帧内容为空）
		case CmdQueryInstrCMRRTestAll:
			m_fInstrumentCMRRLimit = GetTestDataLimitFromXML(true, "CMRR");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QueryInstrCMRRTestByArea);
			break;
			// 查询所选区域仪器增益相位测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrGainPhaseTestArea:
			m_fInstrumentGainPhaseLimit = GetTestDataLimitFromXML(true, "GainPhase");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QueryInstrGainPhaseTestByArea);
			break;
			// 查询全部仪器增益相位测试数据和测试结果（帧内容为空）
		case CmdQueryInstrGainPhaseTestAll:
			m_fInstrumentGainPhaseLimit = GetTestDataLimitFromXML(true, "GainPhase");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QueryInstrGainPhaseTestByArea);
			break;
			// 查询所选区域检波器阻抗测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorResistanceTestArea:
			m_fSensorResistanceLimitMin = GetTestDataLimitFromXML(false, "ResistanceMin");
			m_fSensorResistanceLimitMax = GetTestDataLimitFromXML(false, "ResistanceMax");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QuerySensorResistanceTestByArea);
			break;
			// 查询全部检波器阻抗测试数据和测试结果（帧内容为空）
		case CmdQuerySensorResistanceTestAll:
			m_fSensorResistanceLimitMin = GetTestDataLimitFromXML(false, "ResistanceMin");
			m_fSensorResistanceLimitMax = GetTestDataLimitFromXML(false, "ResistanceMax");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QuerySensorResistanceTestByArea);
			break;
			// 查询所选区域检波器漏电测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorLeakageTestArea:
			m_fSensorLeakageLimit = GetTestDataLimitFromXML(false, "Leakage");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QuerySensorLeakageTestByArea);
			break;
			// 查询全部检波器漏电测试数据和测试结果（帧内容为空）
		case CmdQuerySensorLeakageTestAll:
			m_fSensorLeakageLimit = GetTestDataLimitFromXML(false, "Leakage");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QuerySensorLeakageTestByArea);
			break;
			// 查询所选区域检波器噪声测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorNoiseTestArea:
			m_fSensorNoiseLimit = GetTestDataLimitFromXML(false, "Noise");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QuerySensorNoiseTestByArea);
			break;
			// 查询全部检波器噪声测试数据和测试结果（帧内容为空）
		case CmdQuerySensorNoiseTestAll:
			m_fSensorNoiseLimit = GetTestDataLimitFromXML(false, "Noise");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QuerySensorNoiseTestByArea);
			break;
			// 查询所选区域检波器倾斜度测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorTiltTestArea:
			m_fSensorTiltLimit = GetTestDataLimitFromXML(false, "Tilt");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QuerySensorTiltTestByArea);
			break;
			// 查询全部检波器倾斜度测试数据和测试结果（帧内容为空）
		case CmdQuerySensorTiltTestAll:
			m_fSensorTiltLimit = GetTestDataLimitFromXML(false, "Tilt");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QuerySensorTiltTestByArea);
			break;
			// 查询所选区域检波器倾斜度模式测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorTiltModelTestArea:
			m_fSensorTiltModelLimit = GetTestDataLimitFromXML(false, "Tilt Model");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QuerySensorTiltModelTestByArea);
			break;
			// 查询全部检波器倾斜度模式测试数据和测试结果（帧内容为空）
		case CmdQuerySensorTiltModelTestAll:
			m_fSensorTiltModelLimit = GetTestDataLimitFromXML(false, "Tilt Model");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QuerySensorTiltModelTestByArea);
			break;
			// 查询所选区域地震监测测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySeisMonitorTestArea:
			m_fSeisMonitorLimit = GetTestDataLimitFromXML(false, "Seismonitor");
			OnProcQueryByArea(pRecThread, pChar, uiSize, &CCommDll::QuerySeisMonitorTestByArea);
			break;
			// 查询全部地震监测测试数据和测试结果（帧内容为空）
		case CmdQuerySeisMonitorTestAll:
			m_fSeisMonitorLimit = GetTestDataLimitFromXML(false, "Seismonitor");
			OnProcQueryInfoAll(pRecThread, &CCommDll::QuerySeisMonitorTestByArea);
			break;
		default:
			break;
		}
	}
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
  	map<m_oLocationStruct, unsigned int>::iterator iterLocation;
  	map<m_oAreaStruct, m_oAreaStruct>::iterator iterArea;
	/** 客户端设备位置索引表*/
	map<m_oLocationStruct, unsigned int> InstrumentWholeTableMap;
	/** 客户端设备更新区域索引表*/
	map<m_oAreaStruct, m_oAreaStruct> InstrumentUpdateArea;
	int iLineIndex = 0;
	int iPointIndex = 0;
	unsigned int uiSN = 0;
	unsigned int* uipSNClient = NULL;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	int iPos = 0;
 	int iSize = 0;
	m_oAreaStruct oAreaStruct;
	m_oInstrumentStruct* pInstrument = NULL;
	InstrumentWholeTableMap.clear();
	InstrumentUpdateArea.clear();
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
			AddAreaToMap(&InstrumentUpdateArea, oAreaStruct);
		}
		else
		{
			uipSNClient = pRecThread->GetSnPtrFromLocationMap(iLineIndex, iPointIndex);
			if (uiSN != *uipSNClient)
			{
				*uipSNClient = uiSN;
				GetAreaFromPoint(iLineIndex, iPointIndex, &oAreaStruct);
				// 增加对象到索引表
				AddAreaToMap(&InstrumentUpdateArea, oAreaStruct);
			}
		}
	}
	InstrumentWholeTableMap.swap(pRecThread->m_oInstrumentWholeTableMap);
 	EnterCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
 	// 将客户端的仪器位置索引表与SN索引表相对照
	for (iterLocation = InstrumentWholeTableMap.begin(); iterLocation != InstrumentWholeTableMap.end();)
	{
		// 仪器位置索引表找不到该设备
		if (FALSE == m_pMatrixDllCall->Dll_IfLocationExistInMap(iterLocation->first.m_iLineIndex, 
			iterLocation->first.m_iPointIndex, &m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap))
		{
			// 增加对象到索引表
			GetAreaFromPoint(iterLocation->first.m_iLineIndex, iterLocation->first.m_iPointIndex, &oAreaStruct);
			AddAreaToMap(&InstrumentUpdateArea, oAreaStruct);
			InstrumentWholeTableMap.erase(iterLocation++);
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
	pRecThread->m_oInstrumentWholeTableMap.swap(InstrumentWholeTableMap);

	// 对照后客户端仪器索引表与服务端仪器SN索引表一致，将变化区域发送到客户端
	for (iterArea = InstrumentUpdateArea.begin(); iterArea != InstrumentUpdateArea.end(); iterArea++)
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
}

// 判断仪器更新区域是否已加入索引表
BOOL CCommDll::IfAreaExistInMap(map<m_oAreaStruct, m_oAreaStruct>* pmap, m_oAreaStruct oAreaStruct)
{
	BOOL bResult = FALSE;
	map<m_oAreaStruct, m_oAreaStruct>::iterator iter;
	iter = pmap->find(oAreaStruct);
	if (iter != pmap->end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}
// 增加对象到索引表
void CCommDll::AddAreaToMap(map<m_oAreaStruct, m_oAreaStruct>* pmap, m_oAreaStruct oAreaStruct)
{
	if (FALSE == IfAreaExistInMap(pmap, oAreaStruct))
	{
		pmap->insert(map<m_oAreaStruct, m_oAreaStruct>::value_type (oAreaStruct, oAreaStruct));
	}
}

// 按区域查询仪器信息
unsigned int CCommDll::QueryByArea(CCommRecThread* pRecThread, m_oAreaStruct* pArea, 
	unsigned int uiStartPos, unsigned int(CCommDll::*ptrFun)(m_oInstrumentStruct* pInstrument, 
	unsigned int uiStartPos, CCommRecThread* pRecThread))
{
	unsigned int uiPos = uiStartPos;
	m_oInstrumentStruct* pInstrument = NULL;
	int iLineIndex = 0;
	int iPointMinIndex = 0;
	int iPointMaxIndex = 0;
	// 由区域号得到线号及点号范围
	GetPointRangeFromArea(&iLineIndex, &iPointMinIndex, &iPointMaxIndex, pArea);
	EnterCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	for (int i = iPointMinIndex; i <= iPointMaxIndex; i++)
	{
		pInstrument = m_pMatrixDllCall->Dll_GetInstrumentFromLocationMap(iLineIndex, i, 
			&m_pMatrixDllCall->m_pEnv->m_pLineList->m_pInstrumentList->m_oInstrumentLocationMap);
		uiPos = (this->*ptrFun)(pInstrument, uiPos, pRecThread);
	}
	LeaveCriticalSection(&m_pMatrixDllCall->m_pEnv->m_pLineList->m_oSecLineList);
	return uiPos;
}

// 处理仪器信息查询
void CCommDll::OnProcQueryByArea(CCommRecThread* pRecThread, char* pChar, unsigned int uiSize, 
	unsigned int(CCommDll::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread))
{
	unsigned int uiPos = 0;
	m_oAreaStruct oAreaStruct;
	while(uiPos < uiSize)
	{
		memcpy(&oAreaStruct.m_uiLineNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oAreaStruct.m_uiAreaNb, &pChar[uiPos], 4);
		uiPos += 4;
		uiPos = QueryByArea(pRecThread, &oAreaStruct, uiPos, ptrFun);
	}
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryInstrumentInfo, 
		pRecThread->m_pCommSndFrame->m_cProcBuf, uiPos);
}

// 处理全部信息查询
void CCommDll::OnProcQueryInfoAll(CCommRecThread* pRecThread, 
	unsigned int(CCommDll::*ptrFun)(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread))
{
	int iLineNum = 0;
	int iColumn = 0;
	int iAreaNum = 0;
	int iPos = 0;
	iLineNum = m_uiLineNum * 2 - 1;
	iColumn = m_uiColumnNum * 2 - 1;
	iAreaNum = iColumn / InstrumentTableWindowSize;
	if ((iColumn % InstrumentTableWindowSize) != 0)
	{
		iAreaNum += 1;
	}
	if ((iAreaNum < 1) || (iLineNum < 1))
	{
		return;
	}
	char* pChar = new char[iLineNum * iAreaNum * 8];
	for (int i=1; i<= iLineNum; i++)
	{
		for (int j=1; j<=iAreaNum; j++)
		{
			memcpy(&pChar[iPos], &i, 4);
			iPos += 4;
			memcpy(&pChar[iPos], &j, 4);
			iPos += 4;
		}
	}
	OnProcQueryByArea(pRecThread, pChar, iPos, ptrFun);
	delete[] pChar;
	pChar = NULL;
}

// 按区域查询仪器信息
unsigned int CCommDll::QueryInstrumentInfoByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	if (pInstrument != NULL)
	{
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &pInstrument->m_uiSN, 4);
		uiPos += 4;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &pInstrument->m_uiIP, 4);
		uiPos += 4;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 8);
		uiPos += 8;
	}
	return uiPos;
}

// 查询 SurveyXML 文件信息
void CCommDll::OnProcQuerySurveyXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySurverySetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 PointCode XML文件信息
void CCommDll::OnProcQueryPointCodeXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryPointCodeSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Sensor XML文件信息
void CCommDll::OnProcQuerySensorXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySensorSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Marker XML文件信息
void CCommDll::OnProcQueryMarkerXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryMarkerSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Aux XML文件信息
void CCommDll::OnProcQueryAuxXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryAuxSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Detour XML文件信息
void CCommDll::OnProcQueryDetourXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryDetourSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Mute XML文件信息
void CCommDll::OnProcQueryMuteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryMuteSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 BlastMachine XML文件信息
void CCommDll::OnProcQueryBlastMachineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryBlastMachineSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Absolute XML文件信息
// 炮点+排列个数+排列
void CCommDll::OnProcQueryAbsoluteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryAbsoluteSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Generic XML文件信息
void CCommDll::OnProcQueryGenericXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryGenericSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 Look XML文件信息
void CCommDll::OnProcQueryLookXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryLookSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 InstrumentTestBase XML文件信息
void CCommDll::OnProcQueryInstrumentTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestBaseSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, true);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 SensorTestBase XML文件信息
void CCommDll::OnProcQuerySensorTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestBaseSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, false);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 InstrumentTestLimit XML文件信息
void CCommDll::OnProcQueryInstrumentTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestLimitSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, true);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 SensorTestLimit XML文件信息
void CCommDll::OnProcQuerySensorTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrument_SensorTestLimitSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, false);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 InstrumentTest XML文件信息
void CCommDll::OnProcQueryInstrumentTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryInstrumentTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 SensorTest XML文件信息
void CCommDll::OnProcQuerySensorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySensorTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 MultipleTest XML文件信息
// 关键字结构体+排列个数+排列
void CCommDll::OnProcQueryMultipleTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryMultipleTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 SeisMonitorTest XML文件信息
void CCommDll::OnProcQuerySeisMonitorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QuerySeisMonitorSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 LAULeakage XML文件信息
void CCommDll::OnProcQueryLAULeakageXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryLAULeakageSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}


// 查询 FormLine XML文件信息
void CCommDll::OnProcQueryFormLineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixDllCall->Dll_QueryFormLineSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询所选区域仪器噪声测试数据和测试结果
unsigned int CCommDll::QueryInstrNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentNoiseLimit) || (fData < -m_fInstrumentNoiseLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器失真测试数据和测试结果
unsigned int CCommDll::QueryInstrDistortionTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentDistortionLimit) || (fData < -m_fInstrumentDistortionLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器串扰测试数据和测试结果
unsigned int CCommDll::QueryInstrCrosstalkTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentCrosstalkLimit) || (fData < -m_fInstrumentCrosstalkLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器共模抑制比测试数据和测试结果
unsigned int CCommDll::QueryInstrCMRRTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentCMRRLimit) || (fData < -m_fInstrumentCMRRLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域仪器增益相位测试数据和测试结果
unsigned int CCommDll::QueryInstrGainPhaseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fInstrumentGainPhaseLimit) || (fData < -m_fInstrumentGainPhaseLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器阻抗测试数据和测试结果
unsigned int CCommDll::QuerySensorResistanceTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorResistanceLimitMax) || (fData < m_fSensorResistanceLimitMin))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器漏电测试数据和测试结果
unsigned int CCommDll::QuerySensorLeakageTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorLeakageLimit) || (fData < -m_fSensorLeakageLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器噪声测试数据和测试结果
unsigned int CCommDll::QuerySensorNoiseTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorNoiseLimit) || (fData < -m_fSensorNoiseLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器倾斜度测试数据和测试结果
unsigned int CCommDll::QuerySensorTiltTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorTiltLimit) || (fData < -m_fSensorTiltLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域检波器倾斜度模式测试数据和测试结果
unsigned int CCommDll::QuerySensorTiltModelTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSensorTiltModelLimit) || (fData < -m_fSensorTiltModelLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 查询所选区域地震监测测试数据和测试结果
unsigned int CCommDll::QuerySeisMonitorTestByArea(m_oInstrumentStruct* pInstrument, unsigned int uiStartPos, CCommRecThread* pRecThread)
{
	unsigned int uiPos = uiStartPos;
	float fData = 0.0;
	char cResult = 0;
	if (pInstrument != NULL)
	{
		// 数据处理
		fData = CalTestDataMeanSquare(pInstrument);
		fData = (float)(2.5 * 1e6 * fData / 0x800000);

		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &fData, 4);
		uiPos += 4;
		// 处理结果
		if ((fData > m_fSeisMonitorLimit) || (fData < -m_fSeisMonitorLimit))
		{
			cResult = TestResultFailure;
		}
		else
		{
			cResult = TestResultQualified;
		}
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	else
	{
		memset(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], 0, 4);
		uiPos += 4;
		cResult = TestResultNull;
		memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[uiPos], &cResult, 1);
		uiPos += 1;
	}
	return uiPos;
}


// 从XML配置文件得到测试数据限制值
float CCommDll::GetTestDataLimitFromXML(bool bInstrument, string str)
{
	return m_pMatrixDllCall->Dll_QueryTestDataLimit(bInstrument, str);
}


// 计算测试数据的算术均方根
float CCommDll::CalTestDataMeanSquare(m_oInstrumentStruct* pInstrument)
{
	return (float)m_pMatrixDllCall->Dll_CalMeanSquare(pInstrument);
}
