#include "StdAfx.h"
#include "CommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);
static CCommDll* m_pCommDll;
void CALLBACK ProcRecCmd(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread)
{
	m_pCommDll->OnProcRecCmd(usCmd, pChar, uiSize, pRecThread);
}
CCommDll::CCommDll(void)
{
	m_pMatrixCommDll = NULL;
	m_pCommClient = NULL;
	m_pCommDll = this;
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

// 创建客户端通讯
void CCommDll::OnCreateClientComm(unsigned int uiPort, CString strIP, HWND hWnd)
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
		m_pCommClient->OnInit(uiPort, strIP, hWnd);
	}
}

// 释放客户端通讯
void CCommDll::OnDeleteClientComm()
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
void CCommDll::OnInit(CString strPath, unsigned int uiPort, CString strIP, HWND hWnd)
{
	LoadMatrixCommDll(strPath);
	OnCreateClientComm(uiPort, strIP, hWnd);
}
// 关闭
void CCommDll::OnClose(void)
{
	OnDeleteClientComm();
	FreeMatrixCommDll();
}
/** 接收帧命令字处理*/
void CCommDll::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread)
{

}