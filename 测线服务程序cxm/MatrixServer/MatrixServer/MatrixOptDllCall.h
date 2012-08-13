#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
class CMatrixOptDllCall
{
public:
	CMatrixOptDllCall(void);
	~CMatrixOptDllCall(void);
public:
	/** Dll调用句柄*/
	HMODULE m_hDllMod;
public:
	// 查询Delay设置数据
	void Dll_QueryDelaySetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询SourceShot设置数据
	void Dll_QuerySourceShotSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询Explo设置数据
	void Dll_QueryExploSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询Vibro设置数据
	void Dll_QueryVibroSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询ProcessRecord设置数据
	void Dll_QueryProcessRecordSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询ProcessAux设置数据
	void Dll_QueryProcessAuxSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询ProcessAcq设置数据
	void Dll_QueryProcessAcqSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询ProcessType设置数据
	void Dll_QueryProcessTypeSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 查询ProcessComments设置数据
	void Dll_ProcessCommentsSetupData(char* cProcBuf, int& iPos, m_oOptSetupDataStruct* pOptSetupData);
	// 设置Delay设置数据
	void Dll_SetDelaySetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置SourceShot设置数据
	void Dll_SetSourceShotSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置Explo设置数据
	void Dll_SetExploSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置Vibro设置数据
	void Dll_SetVibroSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置ProcessRecord设置数据
	void Dll_SetProcessRecordSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置ProcessAux设置数据
	void Dll_SetProcessAuxSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置ProcessAcq设置数据
	void Dll_SetProcessAcqSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置ProcessType设置数据
	void Dll_SetProcessTypeSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
	// 设置ProcessComments设置数据
	void Dll_SetProcessCommentsSetupData(char* pChar, unsigned int uiSize, m_oOptSetupDataStruct* pOptSetupData);
};

