#pragma once
#include "MatrixServerDllCall.h"
#include "MatrixLineDllCall.h"
#include "MatrixOptDllCall.h"

/**
* @class CMatrixDllCall
* @brief 调用Dll函数类
*/
class CMatrixDllCall
{
public:
	CMatrixDllCall(void);
	~CMatrixDllCall(void);
public:
	/** Dll调用句柄*/
	HMODULE m_hDllMod;
	/** Dll资源创建指针*/
	m_oEnvironmentStruct* m_pEnv;
	/** CMatrixServerDllCall类成员*/
	CMatrixServerDllCall m_oMatrixServer;
	/** CMatrixLineDllCall类成员*/
	CMatrixLineDllCall m_oMatrixLine;
	/** CMatrixOptDllCall类成员*/
	CMatrixOptDllCall m_oMatrixOpt;
public:
	// DLL创建实例
	void Dll_Create_Instance(void);
	// DLL初始化
	void Dll_Init_Instance(void);
	// DLL关闭实例
	void Dll_Close_Instance(void);
	// DLL释放实例
	void Dll_Free_Instance(void);
	// 载入MatrixServerDll动态链接库
	void LoadMatrixServerDll(CString strPath);
	// 释放MatrixServerDll动态链接库
	void FreeMatrixServerDll(void);
	// 初始化
	void OnInit(CString strPath);
	// 关闭
	void OnClose(void);
};

