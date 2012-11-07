#include "StdAfx.h"
#include "CommLineDll.h"


CCommLineDll::CCommLineDll(void)
	: m_uiLineNum(0)
	, m_uiColumnNum(0)
{
	m_pMatrixLine = NULL;
	m_pLineSetupData = NULL;
}


CCommLineDll::~CCommLineDll(void)
{
}
// 初始化
void CCommLineDll::OnInit(CString strPath)
{
	m_pMatrixLine = new CMatrixLineDllCall;
	m_pMatrixLine->LoadMatrixServerDll(strPath);
	m_pLineSetupData = new m_oLineSetupDataStruct;
	m_pMatrixLine->Dll_Init_Instance(m_pLineSetupData);
}
// 关闭
void CCommLineDll::OnClose(void)
{
	m_pMatrixLine->Dll_Free_Instance(m_pLineSetupData);
	m_pMatrixLine->FreeMatrixServerDll();
	delete m_pMatrixLine;
}

// 设置 SurveyXML 文件信息
void CCommLineDll::OnProcSetSurveyXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetSurverySetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 PointCode XML文件信息
void CCommLineDll::OnProcSetPointCodeXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetPointCodeSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Sensor XML文件信息
void CCommLineDll::OnProcSetSensorXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetSensorSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Marker XML文件信息
void CCommLineDll::OnProcSetMarkerXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetMarkerSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Aux XML文件信息
void CCommLineDll::OnProcSetAuxXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetAuxSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Detour XML文件信息
void CCommLineDll::OnProcSetDetourXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetDetourSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Mute XML文件信息
void CCommLineDll::OnProcSetMuteXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetMuteSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 BlastMachine XML文件信息
void CCommLineDll::OnProcSetBlastMachineXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetBlastMachineSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Absolute XML文件信息
void CCommLineDll::OnProcSetAbsoluteXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetAbsoluteSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Generic XML文件信息
void CCommLineDll::OnProcSetGenericXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetGenericSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 Look XML文件信息
void CCommLineDll::OnProcSetLookXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetLookSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 InstrumentTestBase XML文件信息
void CCommLineDll::OnProcSetInstrumentTestBaseXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, true, m_pLineSetupData, bSave);
}	
// 设置 SensorTestBase XML文件信息
void CCommLineDll::OnProcSetSensorTestBaseXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetInstrument_SensorTestBaseSetupData(pChar, uiSize, false, m_pLineSetupData, bSave);
}	
// 设置 InstrumentTestLimit XML文件信息
void CCommLineDll::OnProcSetInstrumentTestLimitXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, true, m_pLineSetupData, bSave);
}
// 设置 SensorTestLimit XML文件信息
void CCommLineDll::OnProcSetSensorTestLimitXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetInstrument_SensorTestLimitSetupData(pChar, uiSize, false, m_pLineSetupData, bSave);
}
// 设置 InstrumentTest XML文件信息
void CCommLineDll::OnProcSetInstrumentTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetInstrumentTestSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 SensorTest XML文件信息
void CCommLineDll::OnProcSetSensorTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetSensorTestSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 MultipleTest XML文件信息
void CCommLineDll::OnProcSetMultipleTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetMultipleTestSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 SeisMonitorTest XML文件信息
void CCommLineDll::OnProcSetSeisMonitorTestXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetSeisMonitorSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 LAULeakage XML文件信息
void CCommLineDll::OnProcSetLAULeakageXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetLAULeakageSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 设置 FormLine XML文件信息
void CCommLineDll::OnProcSetFormLineXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixLine->Dll_SetFormLineSetupData(pChar, uiSize, m_pLineSetupData, bSave);
}
// 处理查询接收区域命令
void CCommLineDll::OnProcQueryRevSection(CCommRecThread* pRecThread)
{
	int iPos = 0;
	m_pMatrixLine->Dll_GetLineRevSection(m_uiLineNum, m_uiColumnNum, m_pLineSetupData);
	memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &m_uiLineNum, 4);
	iPos += 4;
	memcpy(&pRecThread->m_pCommSndFrame->m_cProcBuf[iPos], &m_uiColumnNum, 4);
	iPos += 4;
	pRecThread->m_pCommSndFrame->MakeSetFrame(CmdQueryRevSection, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 从XML配置文件得到测试数据限制值
float CCommLineDll::QueryTestDataLimitFromXML(bool bInstrument, string str)
{
	return m_pMatrixLine->Dll_QueryTestDataLimit(bInstrument, str, m_pLineSetupData);
}

// 查询 SurveyXML 文件信息
void CCommLineDll::OnProcQuerySurveyXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QuerySurverySetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 PointCode XML文件信息
void CCommLineDll::OnProcQueryPointCodeXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryPointCodeSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Sensor XML文件信息
void CCommLineDll::OnProcQuerySensorXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QuerySensorSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Marker XML文件信息
void CCommLineDll::OnProcQueryMarkerXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryMarkerSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Aux XML文件信息
void CCommLineDll::OnProcQueryAuxXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryAuxSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Detour XML文件信息
void CCommLineDll::OnProcQueryDetourXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryDetourSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Mute XML文件信息
void CCommLineDll::OnProcQueryMuteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryMuteSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 BlastMachine XML文件信息
void CCommLineDll::OnProcQueryBlastMachineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryBlastMachineSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Absolute XML文件信息
// 炮点+排列个数+排列
void CCommLineDll::OnProcQueryAbsoluteXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryAbsoluteSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Generic XML文件信息
void CCommLineDll::OnProcQueryGenericXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryGenericSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 Look XML文件信息
void CCommLineDll::OnProcQueryLookXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryLookSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 InstrumentTestBase XML文件信息
void CCommLineDll::OnProcQueryInstrumentTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryInstrument_SensorTestBaseSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, true, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SensorTestBase XML文件信息
void CCommLineDll::OnProcQuerySensorTestBaseXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryInstrument_SensorTestBaseSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, false, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 InstrumentTestLimit XML文件信息
void CCommLineDll::OnProcQueryInstrumentTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryInstrument_SensorTestLimitSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, true, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SensorTestLimit XML文件信息
void CCommLineDll::OnProcQuerySensorTestLimitXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryInstrument_SensorTestLimitSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, false, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 InstrumentTest XML文件信息
void CCommLineDll::OnProcQueryInstrumentTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryInstrumentTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SensorTest XML文件信息
void CCommLineDll::OnProcQuerySensorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QuerySensorTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 MultipleTest XML文件信息
// 关键字结构体+排列个数+排列
void CCommLineDll::OnProcQueryMultipleTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryMultipleTestSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 SeisMonitorTest XML文件信息
void CCommLineDll::OnProcQuerySeisMonitorTestXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QuerySeisMonitorSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 LAULeakage XML文件信息
void CCommLineDll::OnProcQueryLAULeakageXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryLAULeakageSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}

// 查询 FormLine XML文件信息
void CCommLineDll::OnProcQueryFormLineXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixLine->Dll_QueryFormLineSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pLineSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}