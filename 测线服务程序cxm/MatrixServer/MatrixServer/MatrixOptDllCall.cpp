#include "StdAfx.h"
#include "MatrixOptDllCall.h"

// 创建施工客户端通讯信息结构体
typedef m_oOptSetupDataStruct* (*Create_OptAppSetupData)(void);
// 释放施工客户端参数设置信息结构体缓冲区
typedef void (*Free_OptXMLSetupData)(m_oOptSetupDataStruct* pOptSetupData);
// 查询Delay设置数据
typedef void (*Query_DelaySetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询SourceShot设置数据
typedef void (*Query_SourceShotSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询Explo设置数据
typedef void (*Query_ExploSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询Vibro设置数据
typedef void (*Query_VibroSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询ProcessRecord设置数据
typedef void (*Query_ProcessRecordSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询ProcessAux设置数据
typedef void (*Query_ProcessAuxSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询ProcessAcq设置数据
typedef void (*Query_ProcessAcqSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询ProcessType设置数据
typedef void (*Query_ProcessTypeSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 查询ProcessComments设置数据
typedef void (*Query_ProcessCommentsSetupData)(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
// 设置Delay设置数据
typedef void (*Set_DelaySetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置SourceShot设置数据
typedef void (*Set_SourceShotSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置Explo设置数据
typedef void (*Set_ExploSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置Vibro设置数据
typedef void (*Set_VibroSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessRecord设置数据
typedef void (*Set_ProcessRecordSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessAux设置数据
typedef void (*Set_ProcessAuxSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessAcq设置数据
typedef void (*Set_ProcessAcqSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessType设置数据
typedef void (*Set_ProcessTypeSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
// 设置ProcessComments设置数据
typedef void (*Set_ProcessCommentsSetupData)(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);

CMatrixOptDllCall::CMatrixOptDllCall(void)
{
}


CMatrixOptDllCall::~CMatrixOptDllCall(void)
{
}

// DLL创建实例
m_oOptSetupDataStruct* CMatrixOptDllCall::Dll_Create_Instance(void)
{
	m_oOptSetupDataStruct* pOptSetupData = NULL;
	Create_OptAppSetupData Dll_On_Create = NULL;
	Dll_On_Create = (Create_OptAppSetupData)GetProcAddress(m_hDllMod, "OnCreateOptAppSetupData");
	if (!Dll_On_Create)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		pOptSetupData = (*Dll_On_Create)();
	}
	return pOptSetupData;
}
// DLL释放实例
void CMatrixOptDllCall::Dll_Free_Instance(m_oOptSetupDataStruct* pOptSetupData)
{
	Free_OptXMLSetupData Dll_On_Free = NULL;
	Dll_On_Free = (Free_OptXMLSetupData)GetProcAddress(m_hDllMod, "OnFreeOptXMLSetupData");
	if (!Dll_On_Free)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Free)(pOptSetupData);
	}
}
// 载入MatrixServerDll动态链接库
void CMatrixOptDllCall::LoadMatrixServerDll(CString strPath)
{
	CString str = _T("");
	m_hDllMod = ::LoadLibrary(strPath);
	if (m_hDllMod == NULL)
	{
		str.Format(_T("载入失败！错误码为%d。"), GetLastError());
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
}
// 释放MatrixServerDll动态链接库
void CMatrixOptDllCall::FreeMatrixServerDll(void)
{
	::FreeLibrary(m_hDllMod);
}
// 查询Delay设置数据
void CMatrixOptDllCall::Dll_QueryDelaySetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_DelaySetupData Dll_Query_DelaySetupData = NULL;
	Dll_Query_DelaySetupData = (Query_DelaySetupData)GetProcAddress(m_hDllMod, "QueryDelaySetupData");
	if (!Dll_Query_DelaySetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_DelaySetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询SourceShot设置数据
void CMatrixOptDllCall::Dll_QuerySourceShotSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_SourceShotSetupData Dll_Query_SourceShotSetupData = NULL;
	Dll_Query_SourceShotSetupData = (Query_SourceShotSetupData)GetProcAddress(m_hDllMod, "QuerySourceShotSetupData");
	if (!Dll_Query_SourceShotSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_SourceShotSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询Explo设置数据
void CMatrixOptDllCall::Dll_QueryExploSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_ExploSetupData Dll_Query_ExploSetupData = NULL;
	Dll_Query_ExploSetupData = (Query_ExploSetupData)GetProcAddress(m_hDllMod, "QueryExploSetupData");
	if (!Dll_Query_ExploSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_ExploSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询Vibro设置数据
void CMatrixOptDllCall::Dll_QueryVibroSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_VibroSetupData Dll_Query_VibroSetupData = NULL;
	Dll_Query_VibroSetupData = (Query_VibroSetupData)GetProcAddress(m_hDllMod, "QueryVibroSetupData");
	if (!Dll_Query_VibroSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_VibroSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询ProcessRecord设置数据
void CMatrixOptDllCall::Dll_QueryProcessRecordSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_ProcessRecordSetupData Dll_Query_ProcessRecordSetupData = NULL;
	Dll_Query_ProcessRecordSetupData = (Query_ProcessRecordSetupData)GetProcAddress(m_hDllMod, "QueryProcessRecordSetupData");
	if (!Dll_Query_ProcessRecordSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_ProcessRecordSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询ProcessAux设置数据
void CMatrixOptDllCall::Dll_QueryProcessAuxSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_ProcessAuxSetupData Dll_Query_ProcessAuxSetupData = NULL;
	Dll_Query_ProcessAuxSetupData = (Query_ProcessAuxSetupData)GetProcAddress(m_hDllMod, "QueryProcessAuxSetupData");
	if (!Dll_Query_ProcessAuxSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_ProcessAuxSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询ProcessAcq设置数据
void CMatrixOptDllCall::Dll_QueryProcessAcqSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_ProcessAcqSetupData Dll_Query_ProcessAcqSetupData = NULL;
	Dll_Query_ProcessAcqSetupData = (Query_ProcessAcqSetupData)GetProcAddress(m_hDllMod, "QueryProcessAcqSetupData");
	if (!Dll_Query_ProcessAcqSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_ProcessAcqSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询ProcessType设置数据
void CMatrixOptDllCall::Dll_QueryProcessTypeSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_ProcessTypeSetupData Dll_Query_ProcessTypeSetupData = NULL;
	Dll_Query_ProcessTypeSetupData = (Query_ProcessTypeSetupData)GetProcAddress(m_hDllMod, "QueryProcessTypeSetupData");
	if (!Dll_Query_ProcessTypeSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_ProcessTypeSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 查询Comments设置数据
void CMatrixOptDllCall::Dll_QueryCommentsSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData)
{
	Query_ProcessCommentsSetupData Dll_Query_ProcessCommentsSetupData = NULL;
	Dll_Query_ProcessCommentsSetupData = (Query_ProcessCommentsSetupData)GetProcAddress(m_hDllMod, "QueryProcessCommentsSetupData");
	if (!Dll_Query_ProcessCommentsSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Query_ProcessCommentsSetupData)(cProcBuf, iPos, pOptSetupData);
	}
}
// 设置Delay设置数据
void CMatrixOptDllCall::Dll_SetDelaySetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_DelaySetupData Dll_Set_DelaySetupData= NULL;
	Dll_Set_DelaySetupData = (Set_DelaySetupData)GetProcAddress(m_hDllMod, "SetDelaySetupData");
	if (!Dll_Set_DelaySetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_DelaySetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置SourceShot设置数据
void CMatrixOptDllCall::Dll_SetSourceShotSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_SourceShotSetupData Dll_Set_SourceShotSetupData = NULL;
	Dll_Set_SourceShotSetupData = (Set_SourceShotSetupData)GetProcAddress(m_hDllMod, "SetSourceShotSetupData");
	if (!Dll_Set_SourceShotSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_SourceShotSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置Explo设置数据
void CMatrixOptDllCall::Dll_SetExploSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_ExploSetupData Dll_Set_ExploSetupData = NULL;
	Dll_Set_ExploSetupData = (Set_ExploSetupData)GetProcAddress(m_hDllMod, "SetExploSetupData");
	if (!Dll_Set_ExploSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_ExploSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置Vibro设置数据
void CMatrixOptDllCall::Dll_SetVibroSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_VibroSetupData Dll_Set_VibroSetupData = NULL;
	Dll_Set_VibroSetupData = (Set_VibroSetupData)GetProcAddress(m_hDllMod, "SetVibroSetupData");
	if (!Dll_Set_VibroSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_VibroSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置ProcessRecord设置数据
void CMatrixOptDllCall::Dll_SetProcessRecordSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_ProcessRecordSetupData Dll_Set_ProcessRecordSetupData = NULL;
	Dll_Set_ProcessRecordSetupData = (Set_ProcessRecordSetupData)GetProcAddress(m_hDllMod, "SetProcessRecordSetupData");
	if (!Dll_Set_ProcessRecordSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_ProcessRecordSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置ProcessAux设置数据
void CMatrixOptDllCall::Dll_SetProcessAuxSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_ProcessAuxSetupData Dll_Set_ProcessAuxSetupData = NULL;
	Dll_Set_ProcessAuxSetupData = (Set_ProcessAuxSetupData)GetProcAddress(m_hDllMod, "SetProcessAuxSetupData");
	if (!Dll_Set_ProcessAuxSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_ProcessAuxSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置ProcessAcq设置数据
void CMatrixOptDllCall::Dll_SetProcessAcqSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_ProcessAcqSetupData Dll_Set_ProcessAcqSetupData = NULL;
	Dll_Set_ProcessAcqSetupData = (Set_ProcessAcqSetupData)GetProcAddress(m_hDllMod, "SetProcessAcqSetupData");
	if (!Dll_Set_ProcessAcqSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_ProcessAcqSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置ProcessType设置数据
void CMatrixOptDllCall::Dll_SetProcessTypeSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_ProcessTypeSetupData Dll_Set_ProcessTypeSetupData = NULL;
	Dll_Set_ProcessTypeSetupData = (Set_ProcessTypeSetupData)GetProcAddress(m_hDllMod, "SetProcessTypeSetupData");
	if (!Dll_Set_ProcessTypeSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_ProcessTypeSetupData)(pChar, uiSize, pOptSetupData);
	}
}
// 设置Comments设置数据
void CMatrixOptDllCall::Dll_SetCommentsSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData)
{
	Set_ProcessCommentsSetupData Dll_Set_ProcessCommentsSetupData = NULL;
	Dll_Set_ProcessCommentsSetupData = (Set_ProcessCommentsSetupData)GetProcAddress(m_hDllMod, "SetProcessCommentsSetupData");
	if (!Dll_Set_ProcessCommentsSetupData)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_Set_ProcessCommentsSetupData)(pChar, uiSize, pOptSetupData);
	}
}