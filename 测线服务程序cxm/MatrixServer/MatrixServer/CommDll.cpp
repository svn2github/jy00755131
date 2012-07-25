#include "StdAfx.h"
#include "CommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);

CCommDll::CCommDll(void)
{
	m_pMatrixDllCall = NULL;
	m_pMatrixCommDll = NULL;
	m_pCommServer = NULL;
}


CCommDll::~CCommDll(void)
{
}
// 载入MatrixServerDll动态链接库
void CCommDll::LoadMatrixCommDll(CString strPath)
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
void CCommDll::FreeMatrixCommDll(void)
{
	::FreeLibrary(m_hCommDll);
}

// 创建服务端通讯
void CCommDll::OnCreateServerComm()
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
		m_pCommServer->OnInit();
	}
}
// 释放服务端通讯
void CCommDll::OnDeleteServerComm()
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
void CCommDll::OnInit(CString strPath)
{
	LoadMatrixCommDll(strPath);
	OnCreateServerComm();
}
// 关闭
void CCommDll::OnClose(void)
{
	OnDeleteServerComm();
	FreeMatrixCommDll();
}