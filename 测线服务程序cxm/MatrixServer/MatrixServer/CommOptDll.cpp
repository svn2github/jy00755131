#include "StdAfx.h"
#include "CommOptDll.h"


CCommOptDll::CCommOptDll(void)
{
	m_pMatrixOpt = NULL;
	m_pOptSetupData = NULL;
}


CCommOptDll::~CCommOptDll(void)
{
}
// 初始化
void CCommOptDll::OnInit(CString strPath)
{
	m_pMatrixOpt = new CMatrixOptDllCall;
	m_pMatrixOpt->LoadMatrixServerDll(strPath);
	m_pOptSetupData = new m_oOptSetupDataStruct;
	m_pMatrixOpt->Dll_Init_Instance(m_pOptSetupData);
}
// 关闭
void CCommOptDll::OnClose(void)
{
	m_pMatrixOpt->Dll_Free_Instance(m_pOptSetupData);
	m_pMatrixOpt->FreeMatrixServerDll();
	delete m_pMatrixOpt;
}
// 设置 OperationDelay XML文件信息
void CCommOptDll::OnProcSetDelayOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetDelaySetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 炮表 XML文件信息
void CCommOptDll::OnProcSetSourceShotOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetSourceShotSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 Explo震源类型 XML文件信息
void CCommOptDll::OnProcSetExploOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetExploSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 Vibro震源类型 XML文件信息
void CCommOptDll::OnProcSetVibroOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetVibroSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 ProcessRecord XML文件信息
void CCommOptDll::OnProcSetProcessRecordOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetProcessRecordSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 ProcessAux XML文件信息
void CCommOptDll::OnProcSetProcessAuxOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetProcessAuxSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 ProcessAcq XML文件信息
void CCommOptDll::OnProcSetProcessAcqOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetProcessAcqSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 ProcessType XML文件信息
void CCommOptDll::OnProcSetProcessTypeOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetProcessTypeSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 设置 注释 XML文件信息
void CCommOptDll::OnProcSetCommentsOptXMLInfo(char* pChar, unsigned int uiSize, bool bSave)
{
	m_pMatrixOpt->Dll_SetCommentsSetupData(pChar, uiSize, m_pOptSetupData, bSave);
}
// 查询 OperationDelay XML文件信息
void CCommOptDll::OnProcQueryDelayOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryDelaySetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 炮表 XML文件信息
void CCommOptDll::OnProcQuerySourceShotOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QuerySourceShotSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 Explo震源类型 XML文件信息
void CCommOptDll::OnProcQueryExploOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryExploSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 Vibro震源类型 XML文件信息
void CCommOptDll::OnProcQueryVibroOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryVibroSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessRecord XML文件信息
void CCommOptDll::OnProcQueryProcessRecordOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryProcessRecordSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessAux XML文件信息
void CCommOptDll::OnProcQueryProcessAuxOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryProcessAuxSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessAcq XML文件信息
void CCommOptDll::OnProcQueryProcessAcqOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryProcessAcqSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 ProcessType XML文件信息
void CCommOptDll::OnProcQueryProcessTypeOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryProcessTypeSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}
// 查询 注释 XML文件信息
void CCommOptDll::OnProcQueryCommentsOptXMLInfo(CCommRecThread* pRecThread, unsigned short usCmd)
{
	int iPos = 0;
	m_pMatrixOpt->Dll_QueryCommentsSetupData(pRecThread->m_pCommSndFrame->m_cProcBuf, iPos, m_pOptSetupData);
	pRecThread->m_pCommSndFrame->MakeSetFrame(usCmd, pRecThread->m_pCommSndFrame->m_cProcBuf, iPos);
}