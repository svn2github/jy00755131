#include "StdAfx.h"
#include "LineCommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);
static CLineCommDll* m_pCommDll;
void CALLBACK ProcRecCmd(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread)
{
	m_pCommDll->OnProcRecCmd(usCmd, pChar, uiSize, pRecThread);
}
CLineCommDll::CLineCommDll(void)
{
	m_pMatrixCommDll = NULL;
	m_pCommClient = NULL;
	m_hWnd = NULL;
	m_pCommDll = this;
}


CLineCommDll::~CLineCommDll(void)
{
}

// 载入MatrixServerDll动态链接库
void CLineCommDll::LoadMatrixCommDll(CString strPath)
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
void CLineCommDll::FreeMatrixCommDll(void)
{
	::FreeLibrary(m_hCommDll);
}

// 创建客户端通讯
void CLineCommDll::OnCreateClientComm(unsigned int uiPort, CString strIP, HWND hWnd)
{
	CREATEFN pfn = NULL;
	CString str = _T("");
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
		m_pCommClient = m_pMatrixCommDll->CreateCommClient();
		m_pCommClient->m_oProcRecCmdCallBack = ProcRecCmd;
		m_pCommClient->m_bClient = true;
		m_pCommClient->m_iClientType = ClientTypeLine;
		m_pCommClient->m_oRecThread.m_pCommClient = m_pCommClient;
		m_pCommClient->OnInit(uiPort, strIP, hWnd);
	}
}

// 释放客户端通讯
void CLineCommDll::OnDeleteClientComm()
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
		m_pCommClient->OnClose();
		m_pMatrixCommDll->OnClose();
		(*pfn)(m_pMatrixCommDll);
	}
}

// 初始化
void CLineCommDll::OnInit(CString strPath, unsigned int uiPort, CString strIP, HWND hWnd)
{
	LoadMatrixCommDll(strPath);
	OnCreateClientComm(uiPort, strIP, hWnd);
}
// 关闭
void CLineCommDll::OnClose(void)
{
	OnDeleteClientComm();
	FreeMatrixCommDll();
}
/** 接收帧命令字处理*/
void CLineCommDll::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread)
{
	unsigned int uiFieldOnLeftTime = 0;
	char test[2000];
	switch (usCmd)
	{
		// 查询接收区域（帧内容为行数（4个字节）+列数（4个字节））
	case CmdQueryRevSection:
		memcpy(&m_oXMLDllLine.m_uiLineNum, &pChar[0], 4);
		memcpy(&m_oXMLDllLine.m_uiColumnNum, &pChar[4], 4);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdQueryRevSection, 0);
		break;
		// 上电
	case CmdSetFieldOn:
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetFieldOn, 0);
		break;
		// 断电
	case CmdSetFieldOff:
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetFieldOff, 0);
		break;
		// Field On需要等待的时间（帧内容为执行FieldOn剩余时间，为0表示无需等待）
	case CmdFieldOnWaitTime:
		memcpy(&uiFieldOnLeftTime, pChar, uiSize);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdFieldOnWaitTime, uiFieldOnLeftTime);
		// @@@@查询所选仪器全部信息（帧内容为行号+区域号）
	case CmdQueryInstrumentInfo:
		break;
		// @@@@查询全部仪器的全部信息（帧内容为仪器结构体）
	case CmdQueryInstrumentInfoAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
		// @@@@查询Update Table（帧内容为行号+区域号+ 仪器SN+仪器IP，SN = 0表明无仪器）
	case CmdQueryUpdateTable:
		// @@@调试用
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrumentInfoAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrNoiseTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrDistortionTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrCrosstalkTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrCMRRTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrGainPhaseTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorResistanceTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorLeakageTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorNoiseTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorTiltTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorTiltModelTestAll, NULL, 0);
		m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySeisMonitorTestAll, NULL, 0);
		break;
		// 查询 SurveyXML 文件信息
	case CmdQuerySurveyXMLInfo:
		// 设置 SurveyXML 文件信息
	case CmdSetSurveyXMLInfo:
		m_oXMLDllLine.OnProcSetSurveyXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSurveyXMLInfo, 0);
		break;
		// 查询 PointCode XML文件信息
	case CmdQueryPointCodeXMLInfo:
		// 设置 PointCode XML文件信息
	case CmdSetPointCodeXMLInfo:
		m_oXMLDllLine.OnProcSetPointCodeXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetPointCodeXMLInfo, 0);
		break;
		// 查询 Sensor XML文件信息
	case CmdQuerySensorXMLInfo:
		// 设置 Sensor XML文件信息
	case CmdSetSensorXMLInfo:
		m_oXMLDllLine.OnProcSetSensorXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSensorXMLInfo, 0);
		break;
		// 查询 Marker XML文件信息
	case CmdQueryMarkerXMLInfo:
		// 设置 Marker XML文件信息
	case CmdSetMarkerXMLInfo:
		m_oXMLDllLine.OnProcSetMarkerXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetMarkerXMLInfo, 0);
		break;
		// 查询 Aux XML文件信息
	case CmdQueryAuxXMLInfo:
		// 设置 Aux XML文件信息
	case CmdSetAuxXMLInfo:
		m_oXMLDllLine.OnProcSetAuxXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetAuxXMLInfo, 0);
		break;
		// 查询 Detour XML文件信息
	case CmdQueryDetourXMLInfo:
		// 设置 Detour XML文件信息
	case CmdSetDetourXMLInfo:
		m_oXMLDllLine.OnProcSetDetourXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetDetourXMLInfo, 0);
		break;
		// 查询 Mute XML文件信息
	case CmdQueryMuteXMLInfo:
		// 设置 Mute XML文件信息
	case CmdSetMuteXMLInfo:
		m_oXMLDllLine.OnProcSetMuteXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetMuteXMLInfo, 0);
		break;
		// 查询 BlastMachine XML文件信息
	case CmdQueryBlastMachineXMLInfo:
		// 设置 BlastMachine XML文件信息
	case CmdSetBlastMachineXMLInfo:
		m_oXMLDllLine.OnProcSetBlastMachineXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetBlastMachineXMLInfo, 0);
		break;
		// 查询 Absolute XML文件信息
	case CmdQueryAbsoluteXMLInfo:
		// 设置 Absolute XML文件信息
	case CmdSetAbsoluteXMLInfo:
		m_oXMLDllLine.OnProcSetAbsoluteXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetAbsoluteXMLInfo, 0);
		break;
		// 查询 Generic XML文件信息
	case CmdQueryGenericXMLInfo:
		// 设置 Generic XML文件信息
	case CmdSetGenericXMLInfo:
		m_oXMLDllLine.OnProcSetGenericXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetGenericXMLInfo, 0);
		break;
		// 查询 Look XML文件信息
	case CmdQueryLookXMLInfo:
		// 设置 Look XML文件信息
	case CmdSetLookXMLInfo:
		m_oXMLDllLine.OnProcSetLookXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetLookXMLInfo, 0);
		break;
		// 查询 InstrumentTestBase XML文件信息
	case CmdQueryInstrumentTestBaseXMLInfo:
		// 设置 InstrumentTestBase XML文件信息
	case CmdSetInstrumentTestBaseXMLInfo:
		m_oXMLDllLine.OnProcSetInstrumentTestBaseXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetInstrumentTestBaseXMLInfo, 0);
		break;
		// 查询 SensorTestBase XML文件信息
	case CmdQuerySensorTestBaseXMLInfo:
		// 设置 SensorTestBase XML文件信息
	case CmdSetSensorTestBaseXMLInfo:
		m_oXMLDllLine.OnProcSetSensorTestBaseXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSensorTestBaseXMLInfo, 0);
		break;
		// 查询 InstrumentTestLimit XML文件信息
	case CmdQueryInstrumentTestLimitXMLInfo:
		// 设置 InstrumentTestLimit XML文件信息
	case CmdSetInstrumentTestLimitXMLInfo:
		m_oXMLDllLine.OnProcSetInstrumentTestLimitXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetInstrumentTestLimitXMLInfo, 0);
		break;
		// 查询 SensorTestLimit XML文件信息
	case CmdQuerySensorTestLimitXMLInfo:
		// 设置 SensorTestLimit XML文件信息
	case CmdSetSensorTestLimitXMLInfo:
		m_oXMLDllLine.OnProcSetSensorTestLimitXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSensorTestLimitXMLInfo, 0);
		break;
		// 查询 InstrumentTest XML文件信息
	case CmdQueryInstrumentTestXMLInfo:
		// 设置 InstrumentTest XML文件信息
	case CmdSetInstrumentTestXMLInfo:
		m_oXMLDllLine.OnProcSetInstrumentTestXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetInstrumentTestXMLInfo, 0);
		break;
		// 查询 SensorTest XML文件信息
	case CmdQuerySensorTestXMLInfo:
		// 设置 SensorTest XML文件信息
	case CmdSetSensorTestXMLInfo:
		m_oXMLDllLine.OnProcSetSensorTestXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSensorTestXMLInfo, 0);
		break;
		// 查询 MultipleTest XML文件信息
	case CmdQueryMultipleTestXMLInfo:
		// 设置 MultipleTest XML文件信息
	case CmdSetMultipleTestXMLInfo:
		m_oXMLDllLine.OnProcSetMultipleTestXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetMultipleTestXMLInfo, 0);
		break;
		// 查询 SeisMonitorTest XML文件信息
	case CmdQuerySeisMonitorTestXMLInfo:
		// 设置 SeisMonitorTest XML文件信息
	case CmdSetSeisMonitorTestXMLInfo:
		m_oXMLDllLine.OnProcSetSeisMonitorTestXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSeisMonitorTestXMLInfo, 0);
		break;
		// 查询 LAULeakage XML文件信息
	case CmdQueryLAULeakageXMLInfo:
		// 设置 LAULeakage XML文件信息
	case CmdSetLAULeakageXMLInfo:
		m_oXMLDllLine.OnProcSetLAULeakageXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetLAULeakageXMLInfo, 0);
		break;
		// 查询 FormLine XML文件信息
	case CmdQueryFormLineXMLInfo:
		// 设置 FormLine XML文件信息
	case CmdSetFormLineXMLInfo:
		m_oXMLDllLine.OnProcSetFormLineXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetFormLineXMLInfo, 0);
		break;
	case CmdQueryInstrNoiseTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQueryInstrDistortionTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQueryInstrCrosstalkTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQueryInstrCMRRTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQueryInstrGainPhaseTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQuerySensorResistanceTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQuerySensorLeakageTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQuerySensorNoiseTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQuerySensorTiltTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQuerySensorTiltModelTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	case CmdQuerySeisMonitorTestAll:
		// @@@@测试用
		memcpy(test, pChar, uiSize);
		break;
	default:
		break;
	}
}