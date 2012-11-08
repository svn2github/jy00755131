#pragma once
#include "..\\MatrixServerDll\\ConfigOperationXml.h"
class CMatrixOptDllCall
{
public:
	CMatrixOptDllCall(void);
	~CMatrixOptDllCall(void);
public:
	/** Dll调用句柄*/
	HMODULE m_hDllMod;
public:
	// 初始化施工客户程序设置信息DLL实例
	void Dll_Init_Instance(m_oOptSetupDataStruct* pOptSetupData);
	// 载入施工客户程序设置信息
	void Dll_Load_SetupData(m_oOptSetupDataStruct* pOptSetupData);
	// 保存施工客户程序设置信息
	void Dll_Save_SetupData(m_oOptSetupDataStruct* pOptSetupData);
	// DLL释放实例
	void Dll_Free_Instance(m_oOptSetupDataStruct* pOptSetupData);
	// 载入MatrixServerDll动态链接库
	void LoadMatrixServerDll(CString strPath);
	// 释放MatrixServerDll动态链接库
	void FreeMatrixServerDll(void);
public:
	/** 设置Delay设置数据*/
	void Dll_SetDelaySetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置SourceShot设置数据*/
	void Dll_SetSourceShotSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置Explo设置数据*/
	void Dll_SetExploSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置Vibro设置数据*/
	void Dll_SetVibroSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置ProcessRecord设置数据*/
	void Dll_SetProcessRecordSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置ProcessAux设置数据*/
	void Dll_SetProcessAuxSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置ProcessAcq设置数据*/
	void Dll_SetProcessAcqSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置ProcessType设置数据*/
	void Dll_SetProcessTypeSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 设置Comments设置数据*/
	void Dll_SetCommentsSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData, bool bSave = true);
	/** 查询Delay设置数据*/
	void Dll_QueryDelaySetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询SourceShot设置数据*/
	void Dll_QuerySourceShotSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询Explo设置数据*/
	void Dll_QueryExploSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询Vibro设置数据*/
	void Dll_QueryVibroSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询ProcessRecord设置数据*/
	void Dll_QueryProcessRecordSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询ProcessAux设置数据*/
	void Dll_QueryProcessAuxSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询ProcessAcq设置数据*/
	void Dll_QueryProcessAcqSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询ProcessType设置数据*/
	void Dll_QueryProcessTypeSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	/** 查询Comments设置数据*/
	void Dll_QueryCommentsSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
};

