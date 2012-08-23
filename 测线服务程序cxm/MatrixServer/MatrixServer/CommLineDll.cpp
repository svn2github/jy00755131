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
	m_pMatrixLine->Dll_SetSensorSetupData(pChar, uiSize, m_pLineSetupData, bSave);
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