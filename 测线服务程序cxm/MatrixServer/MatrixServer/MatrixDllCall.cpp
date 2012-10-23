#include "StdAfx.h"
#include "MatrixDllCall.h"

// 创建实例，并返回实例指针
typedef m_oEnvironmentStruct* (*Create_Instance)(void);
// 释放实例资源
typedef void (*Free_Instance)(m_oEnvironmentStruct* pEnv);
// 初始化实例
typedef void (*Init_Instance)(m_oEnvironmentStruct* pEnv);
// 关闭实例
typedef void (*Close_Instance)(m_oEnvironmentStruct* pEnv);

CMatrixDllCall::CMatrixDllCall(void)
{
	m_pEnv = NULL;
}


CMatrixDllCall::~CMatrixDllCall(void)
{
}

// DLL创建实例
void CMatrixDllCall::Dll_Create_Instance(void)
{
	Create_Instance Dll_On_Create = NULL;
	Dll_On_Create = (Create_Instance)GetProcAddress(m_hDllMod, "OnCreateInstance");
	if (!Dll_On_Create)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		m_pEnv = (*Dll_On_Create)();
	}
}
// DLL初始化
void CMatrixDllCall::Dll_Init_Instance(void)
{
	Init_Instance Dll_On_Init = NULL;
	Dll_On_Init = (Init_Instance)GetProcAddress(m_hDllMod, "OnInit");
	if (!Dll_On_Init)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Init)(m_pEnv);
	}
}
// DLL关闭实例
void CMatrixDllCall::Dll_Close_Instance(void)
{
	Close_Instance Dll_On_Close = NULL;
	Dll_On_Close = (Close_Instance)GetProcAddress(m_hDllMod, "OnClose");
	if (!Dll_On_Close)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Close)(m_pEnv);
	}
}
// DLL释放实例
void CMatrixDllCall::Dll_Free_Instance(void)
{
	Free_Instance Dll_On_Free = NULL;
	Dll_On_Free = (Free_Instance)GetProcAddress(m_hDllMod, "OnFreeInstance");
	if (!Dll_On_Free)
	{
		// handle the error
		FreeLibrary(m_hDllMod);
		PostQuitMessage(0);
	}
	else
	{
		// call the function
		(*Dll_On_Free)(m_pEnv);
	}
}
// 载入MatrixServerDll动态链接库
void CMatrixDllCall::LoadMatrixServerDll(CString strPath)
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
void CMatrixDllCall::FreeMatrixServerDll(void)
{
	::FreeLibrary(m_hDllMod);
}
// 初始化
void CMatrixDllCall::OnInit(CString strPath)
{
	// 载入动态链接库
	LoadMatrixServerDll(strPath);
	// DLL创建实例
	Dll_Create_Instance();
	m_oMatrixServer.m_pEnv = m_pEnv;
	m_oMatrixServer.m_hDllMod = m_hDllMod;
	m_oMatrixLine.m_hDllMod = m_hDllMod;
	m_oMatrixOpt.m_hDllMod = m_hDllMod;
	// DLL手动设置AD
//	Dll_ADCSet_ByHand();
// 	// DLL得到采样数据处理的回调函数
// 	m_oMatrixServer.Dll_GetProSampleData_CallBack();
	// DLL初始化实例
	Dll_Init_Instance();
}
// 关闭
void CMatrixDllCall::OnClose(void)
{
	// DLL关闭实例
	Dll_Close_Instance();
	// DLL释放实例
	Dll_Free_Instance();
	// 释放MatrixServerDll动态链接库
	FreeMatrixServerDll();
}