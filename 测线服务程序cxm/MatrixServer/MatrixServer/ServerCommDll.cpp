#include "StdAfx.h"
#include "ServerCommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);
static CServerCommDll* m_pCommDll;
// 处理查询接收区域命令
void CALLBACK ProcRecCmd(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread)
{
	m_pCommDll->OnProcRecCmd(usCmd, pChar, uiSize, pRecThread);
}
void CALLBACK ProcCloseClientXML(CCommClient* pCommClient)
{
	m_pCommDll->OnProcCloseClientXML(pCommClient);
}
CServerCommDll::CServerCommDll(void)
{
	m_pMatrixCommDll = NULL;
	m_pCommServer = NULL;
	m_pCommDll = this;
}


CServerCommDll::~CServerCommDll(void)
{
}
// 载入MatrixServerDll动态链接库
void CServerCommDll::LoadMatrixCommDll(CString strPath)
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
void CServerCommDll::FreeMatrixCommDll(void)
{
	::FreeLibrary(m_hCommDll);
}

// 创建服务端通讯
void CServerCommDll::OnCreateServerComm()
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
		m_pCommServer->m_oProcCloseClientXML = ProcCloseClientXML;
		m_pCommServer->OnInit();
	}
}
// 释放服务端通讯
void CServerCommDll::OnDeleteServerComm()
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
void CServerCommDll::OnInit(CString strPath)
{
	LoadMatrixCommDll(strPath);
	OnCreateServerComm();
}
// 关闭
void CServerCommDll::OnClose(void)
{
	OnDeleteServerComm();
	FreeMatrixCommDll();
}
/** 命令字解析*/
void CServerCommDll::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread)
{
	CCommClient* pCommClient = pRecThread->m_pCommClient;
	// 客户端登陆验证（帧内容为验证码）
	if (usCmd == CmdClientConnect)
	{
		CString str = _T("");
		string strCheckLine = "";
		string strCheckOpt = "";
		str.Format(_T("ClientType = %d"), ClientTypeLine);
		str = CommCheck + str;
		strCheckLine = (CStringA)str;
		str.Format(_T("ClientType = %d"), ClientTypeOpt);
		str = CommCheck + str;
		strCheckOpt = (CStringA)str;
		// 与连接校验码相匹配
		if (0 == strcmp(pChar, strCheckLine.c_str()))
		{
			pCommClient->m_bCheckConnected = true;
			pCommClient->m_iClientType = ClientTypeLine;
			pCommClient->m_ptr = (void*)malloc(sizeof(CCommLineDll));
			((CCommLineDll*)pCommClient->m_ptr)->m_pMatrixLine = m_pMatrixLine;
			((CCommLineDll*)pCommClient->m_ptr)->OnInit(_T("MatrixServerDll.dll"));
			((CCommLineDll*)pCommClient->m_ptr)->m_pLineSetupData->m_strLineXMLFilePath = (CStringA)pCommClient->m_strLinePath;
			((CCommLineDll*)pCommClient->m_ptr)->LoadLineAppSetupData();
		}
		else if (0 == strcmp(pChar, strCheckOpt.c_str()))
		{
			pCommClient->m_bCheckConnected = true;
			pCommClient->m_iClientType = ClientTypeOpt;
			pCommClient->m_ptr = (void*)malloc(sizeof(CCommOptDll));
			((CCommOptDll*)pCommClient->m_ptr)->m_pMatrixOpt = m_pMatrixOpt;
			((CCommOptDll*)pCommClient->m_ptr)->OnInit(_T("MatrixServerDll.dll"));
			((CCommOptDll*)pCommClient->m_ptr)->m_pOptSetupData->m_strOptXMLFilePath = (CStringA)pCommClient->m_strOptPath;
			((CCommOptDll*)pCommClient->m_ptr)->LoadOptAppSetupData();
		}
	}
	else
	{
		if (pCommClient->m_bCheckConnected == false)
		{
			return;
		}
		switch (usCmd)
		{
		case CmdQueryRevSection:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryRevSection(pRecThread);
			break;
			// 客户端心跳命令（帧内容为空）
		case CmdSetHeartBeat:
			m_oCommServerDll.OnProcInstrumentTableUpdate(pRecThread);
			break;
		case CmdQueryWholeTable:
			pRecThread->m_oInstrumentWholeTableMap.clear();
			m_oCommServerDll.OnProcInstrumentTableUpdate(pRecThread);
			break;
			// 上电（命令字后帧内容为空，返回值为执行FieldOn剩余时间，为0表示无需等待）
		case CmdSetFieldOn:
			m_oCommServerDll.OnProcSetFieldOn(pRecThread);
			break;
			// 断电（命令字后帧内容为空）
		case CmdSetFieldOff:
			m_oCommServerDll.OnProcSetFieldOff(pRecThread);
			break;
			// 查询所选仪器全部信息（帧内容为行号+区域号）
		case CmdQueryInstrumentInfo:
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QueryInstrumentInfoBySN);
			break;
			// 查询全部仪器的全部信息（应答帧内容为仪器结构体）
		case CmdQueryInstrumentInfoAll:
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QueryInstrumentInfoBySN);
			break;

			// 查询 SurveyXML 文件信息（帧内容为空）
		case CmdQuerySurveyXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQuerySurveyXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SurveyXML 文件信息（帧内容为信息结构体）
		case CmdSetSurveyXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetSurveyXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 PointCode XML文件信息（帧内容为空）
		case CmdQueryPointCodeXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryPointCodeXMLInfo(pRecThread, usCmd);
			break;
			// 设置 PointCode XML文件信息（帧内容为信息结构体）
		case CmdSetPointCodeXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetPointCodeXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Sensor XML文件信息（帧内容为空）
		case CmdQuerySensorXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQuerySensorXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Sensor XML文件信息（帧内容为信息结构体）
		case CmdSetSensorXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetSensorXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Marker XML文件信息（帧内容为空）
		case CmdQueryMarkerXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryMarkerXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Marker XML文件信息（帧内容为信息结构体）
		case CmdSetMarkerXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetMarkerXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Aux XML文件信息（帧内容为空）
		case CmdQueryAuxXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryAuxXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Aux XML文件信息（帧内容为信息结构体）
		case CmdSetAuxXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetAuxXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Detour XML文件信息（帧内容为空）
		case CmdQueryDetourXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryDetourXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Detour XML文件信息（帧内容为信息结构体）
		case CmdSetDetourXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetDetourXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Mute XML文件信息（帧内容为空）
		case CmdQueryMuteXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryMuteXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Mute XML文件信息（帧内容为信息结构体）
		case CmdSetMuteXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetMuteXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 BlastMachine XML文件信息（帧内容为空）
		case CmdQueryBlastMachineXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryBlastMachineXMLInfo(pRecThread, usCmd);
			break;
			// 设置 BlastMachine XML文件信息（帧内容为信息结构体）
		case CmdSetBlastMachineXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetBlastMachineXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Absolute XML文件信息（帧内容为空）
		case CmdQueryAbsoluteXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryAbsoluteXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Absolute XML文件信息（帧内容为信息结构体）
		case CmdSetAbsoluteXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetAbsoluteXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break; 
			// 查询 Generic XML文件信息（帧内容为空）
		case CmdQueryGenericXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryGenericXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Generic XML文件信息（帧内容为信息结构体）
		case CmdSetGenericXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetGenericXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Look XML文件信息（帧内容为空）
		case CmdQueryLookXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryLookXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Look XML文件信息（帧内容为信息结构体）
		case CmdSetLookXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetLookXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTestBase XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestBaseXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryInstrumentTestBaseXMLInfo(pRecThread, usCmd);
			break;
			// 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestBaseXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetInstrumentTestBaseXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTestBase XML文件信息（帧内容为空）
		case CmdQuerySensorTestBaseXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQuerySensorTestBaseXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SensorTestBase XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestBaseXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetSensorTestBaseXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTestLimit XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestLimitXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryInstrumentTestLimitXMLInfo(pRecThread, usCmd);
			break;
			// 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestLimitXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetInstrumentTestLimitXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTestLimit XML文件信息（帧内容为空）
		case CmdQuerySensorTestLimitXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQuerySensorTestLimitXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestLimitXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetSensorTestLimitXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 InstrumentTest XML文件信息（帧内容为空）
		case CmdQueryInstrumentTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryInstrumentTestXMLInfo(pRecThread, usCmd);
			break;
			// 设置 InstrumentTest XML文件信息（帧内容为信息结构体）
		case CmdSetInstrumentTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetInstrumentTestXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SensorTest XML文件信息（帧内容为空）
		case CmdQuerySensorTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQuerySensorTestXMLInfo(pRecThread, usCmd);
			break;
			// SensorTest XML文件信息（帧内容为信息结构体）
		case CmdSetSensorTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetSensorTestXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 MultipleTest XML文件信息（帧内容为空）
		case CmdQueryMultipleTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryMultipleTestXMLInfo(pRecThread, usCmd);
			break;
			// 设置 MultipleTest XML文件信息（帧内容为信息结构体）
		case CmdSetMultipleTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetMultipleTestXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 SeisMonitorTest XML文件信息（帧内容为空）
		case CmdQuerySeisMonitorTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQuerySeisMonitorTestXMLInfo(pRecThread, usCmd);
			break;
			// 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）
		case CmdSetSeisMonitorTestXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetSeisMonitorTestXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 LAULeakage XML文件信息（帧内容为空）
		case CmdQueryLAULeakageXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryLAULeakageXMLInfo(pRecThread, usCmd);
			break;
			// 设置 LAULeakage XML文件信息（帧内容为信息结构体）
		case CmdSetLAULeakageXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetLAULeakageXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 FormLine XML文件信息（帧内容为空）
		case CmdQueryFormLineXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcQueryFormLineXMLInfo(pRecThread, usCmd);
			break;
			// 设置 FormLine XML文件信息（帧内容为信息结构体）
		case CmdSetFormLineXMLInfo:
			((CCommLineDll*)pCommClient->m_ptr)->OnProcSetFormLineXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;


			// 查询所选区域仪器噪声测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrNoiseTestArea:
			m_oCommServerDll.m_fInstrumentNoiseLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "Noise");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QueryInstrNoiseTestBySN);
			break;
			// 查询全部仪器的仪器噪声测试数据和测试结果（帧内容为空）
		case CmdQueryInstrNoiseTestAll:
			m_oCommServerDll.m_fInstrumentNoiseLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "Noise");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QueryInstrNoiseTestBySN);
			break;
			// 查询所选区域仪器失真测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrDistortionTestArea:
			m_oCommServerDll.m_fInstrumentDistortionLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "Distortion");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QueryInstrDistortionTestBySN);
			break;
			// 查询全部仪器失真测试数据和测试结果（帧内容为空）
		case CmdQueryInstrDistortionTestAll:
			m_oCommServerDll.m_fInstrumentDistortionLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "Distortion");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QueryInstrDistortionTestBySN);
			break;
			// 查询所选区域仪器串扰测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrCrosstalkTestArea:
			m_oCommServerDll.m_fInstrumentCrosstalkLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "Crosstalk");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QueryInstrCrosstalkTestBySN);
			break;
			// 查询全部仪器串扰测试数据和测试结果（帧内容为空）
		case CmdQueryInstrCrosstalkTestAll:
			m_oCommServerDll.m_fInstrumentCrosstalkLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "Crosstalk");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QueryInstrCrosstalkTestBySN);
			break;
			// 查询所选区域仪器共模抑制比测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrCMRRTestArea:
			m_oCommServerDll.m_fInstrumentCMRRLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "CMRR");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QueryInstrCMRRTestBySN);
			break;
			// 查询全部仪器共模抑制比测试数据和测试结果（帧内容为空）
		case CmdQueryInstrCMRRTestAll:
			m_oCommServerDll.m_fInstrumentCMRRLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "CMRR");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QueryInstrCMRRTestBySN);
			break;
			// 查询所选区域仪器增益相位测试数据和测试结果（帧内容为行号+区域号）
		case CmdQueryInstrGainPhaseTestArea:
			m_oCommServerDll.m_fInstrumentGainPhaseLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "GainPhase");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QueryInstrGainPhaseTestBySN);
			break;
			// 查询全部仪器增益相位测试数据和测试结果（帧内容为空）
		case CmdQueryInstrGainPhaseTestAll:
			m_oCommServerDll.m_fInstrumentGainPhaseLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(true, "GainPhase");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QueryInstrGainPhaseTestBySN);
			break;
			// 查询所选区域检波器阻抗测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorResistanceTestArea:
			m_oCommServerDll.m_fSensorResistanceLimitMin = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "ResistanceMin");
			m_oCommServerDll.m_fSensorResistanceLimitMax = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "ResistanceMax");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QuerySensorResistanceTestBySN);
			break;
			// 查询全部检波器阻抗测试数据和测试结果（帧内容为空）
		case CmdQuerySensorResistanceTestAll:
			m_oCommServerDll.m_fSensorResistanceLimitMin = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "ResistanceMin");
			m_oCommServerDll.m_fSensorResistanceLimitMax = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "ResistanceMax");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QuerySensorResistanceTestBySN);
			break;
			// 查询所选区域检波器漏电测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorLeakageTestArea:
			m_oCommServerDll.m_fSensorLeakageLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Leakage");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QuerySensorLeakageTestBySN);
			break;
			// 查询全部检波器漏电测试数据和测试结果（帧内容为空）
		case CmdQuerySensorLeakageTestAll:
			m_oCommServerDll.m_fSensorLeakageLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Leakage");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QuerySensorLeakageTestBySN);
			break;
			// 查询所选区域检波器噪声测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorNoiseTestArea:
			m_oCommServerDll.m_fSensorNoiseLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Noise");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QuerySensorNoiseTestBySN);
			break;
			// 查询全部检波器噪声测试数据和测试结果（帧内容为空）
		case CmdQuerySensorNoiseTestAll:
			m_oCommServerDll.m_fSensorNoiseLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Noise");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QuerySensorNoiseTestBySN);
			break;
			// 查询所选区域检波器倾斜度测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorTiltTestArea:
			m_oCommServerDll.m_fSensorTiltLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Tilt");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QuerySensorTiltTestBySN);
			break;
			// 查询全部检波器倾斜度测试数据和测试结果（帧内容为空）
		case CmdQuerySensorTiltTestAll:
			m_oCommServerDll.m_fSensorTiltLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Tilt");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QuerySensorTiltTestBySN);
			break;
			// 查询所选区域检波器倾斜度模式测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySensorTiltModelTestArea:
			m_oCommServerDll.m_fSensorTiltModelLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Tilt Model");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QuerySensorTiltModelTestBySN);
			break;
			// 查询全部检波器倾斜度模式测试数据和测试结果（帧内容为空）
		case CmdQuerySensorTiltModelTestAll:
			m_oCommServerDll.m_fSensorTiltModelLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Tilt Model");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QuerySensorTiltModelTestBySN);
			break;
			// 查询所选区域地震监测测试数据和测试结果（帧内容为行号+区域号）
		case CmdQuerySeisMonitorTestArea:
			m_oCommServerDll.m_fSeisMonitorLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Seismonitor");
			m_oCommServerDll.OnProcQueryBySN(pRecThread, usCmd, pChar, uiSize, &CCommServerDll::QuerySeisMonitorTestBySN);
			break;
			// 查询全部地震监测测试数据和测试结果（帧内容为空）
		case CmdQuerySeisMonitorTestAll:
			m_oCommServerDll.m_fSeisMonitorLimit = ((CCommLineDll*)pCommClient->m_ptr)->QueryTestDataLimitFromXML(false, "Seismonitor");
			m_oCommServerDll.OnProcQueryInfoAll(pRecThread, usCmd, &CCommServerDll::QuerySeisMonitorTestBySN);
			break;
			// 查询 OperationDelay XML文件信息（帧内容为空）
		case CmdQueryDelayOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryDelayOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 OperationDelay XML文件信息（帧内容为信息结构体）
		case CmdSetDelayOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetDelayOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 炮表 XML文件信息（帧内容为空）
		case CmdQuerySourceShotOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQuerySourceShotOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 炮表 XML文件信息（帧内容为信息结构体）
		case CmdSetSourceShotOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetSourceShotOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Explo震源类型 XML文件信息（帧内容为空）
		case CmdQueryExploOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryExploOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Explo震源类型 XML文件信息（帧内容为信息结构体）
		case CmdSetExploOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetExploOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 Vibro震源类型 XML文件信息（帧内容为空）
		case CmdQueryVibroOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryVibroOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 Vibro震源类型 XML文件信息（帧内容为信息结构体）
		case CmdSetVibroOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetVibroOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 ProcessRecord XML文件信息（帧内容为空）
		case CmdQueryProcessRecordOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryProcessRecordOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 ProcessRecord XML文件信息（帧内容为信息结构体）
		case CmdSetProcessRecordOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetProcessRecordOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 ProcessAux XML文件信息（帧内容为空）
		case CmdQueryProcessAuxOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryProcessAuxOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 ProcessAux XML文件信息（帧内容为信息结构体）
		case CmdSetProcessAuxOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetProcessAuxOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 ProcessAcq XML文件信息（帧内容为空）
		case CmdQueryProcessAcqOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryProcessAcqOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 ProcessAcq XML文件信息（帧内容为信息结构体）
		case CmdSetProcessAcqOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetProcessAcqOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 ProcessType XML文件信息（帧内容为空）
		case CmdQueryProcessTypeOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryProcessTypeOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 ProcessType XML文件信息（帧内容为信息结构体）
		case CmdSetProcessTypeOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetProcessTypeOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
			// 查询 注释 XML文件信息（帧内容为空）
		case CmdQueryCommentsOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcQueryCommentsOptXMLInfo(pRecThread, usCmd);
			break;
			// 设置 注释 XML文件信息（帧内容为信息结构体）
		case CmdSetCommentsOptXMLInfo:
			((CCommOptDll*)pCommClient->m_ptr)->OnProcSetCommentsOptXMLInfo(pChar, uiSize);
			pCommClient->BroadCastXMLChange(usCmd, pChar, uiSize);
			break;
		default:
			break;
		}
	}
}

/** 关闭并保存客户端XML文件*/
void CServerCommDll::OnProcCloseClientXML(CCommClient* pCommClient)
{
	if (pCommClient->m_ptr != NULL)
	{
		if (pCommClient->m_iClientType == ClientTypeLine)
		{
			((CCommLineDll*)pCommClient->m_ptr)->SaveLineAppSetupData();
			((CCommLineDll*)pCommClient->m_ptr)->OnClose();
		}
		else if (pCommClient->m_iClientType == ClientTypeOpt)
		{
			((CCommOptDll*)pCommClient->m_ptr)->SaveOptAppSetupData();
			((CCommOptDll*)pCommClient->m_ptr)->OnClose();
		}
	}
}