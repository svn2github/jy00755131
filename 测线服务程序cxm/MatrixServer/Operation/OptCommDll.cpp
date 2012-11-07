#include "StdAfx.h"
#include "OptCommDll.h"

typedef CMatrixCommDll* (*CREATEFN)(void);
typedef void (*DELETEFN)(CMatrixCommDll*);
static COptCommDll* m_pCommDll;
void CALLBACK ProcRecCmd(unsigned short usCmd, char* pChar, 
	unsigned int uiSize, CCommRecThread* pRecThread)
{
	m_pCommDll->OnProcRecCmd(usCmd, pChar, uiSize, pRecThread);
}
COptCommDll::COptCommDll(void)
{
	m_pMatrixCommDll = NULL;
	m_pCommClient = NULL;
	m_hWnd = NULL;
	m_pCommDll = this;
}


COptCommDll::~COptCommDll(void)
{
}

// 载入MatrixServerDll动态链接库
void COptCommDll::LoadMatrixCommDll(CString strPath)
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
void COptCommDll::FreeMatrixCommDll(void)
{
	::FreeLibrary(m_hCommDll);
}

// 创建客户端通讯
void COptCommDll::OnCreateClientComm(unsigned int uiPort, CString strIP, HWND hWnd)
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
		m_pCommClient->m_oRecThread.m_pCommClient = m_pCommClient;
		m_pCommClient->m_iClientType = ClientTypeOpt;
		m_pCommClient->OnInit(uiPort, strIP, hWnd);
	}
}

// 释放客户端通讯
void COptCommDll::OnDeleteClientComm()
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
void COptCommDll::OnInit(CString strPath, unsigned int uiPort, CString strIP, HWND hWnd)
{
	LoadMatrixCommDll(strPath);
	OnCreateClientComm(uiPort, strIP, hWnd);
}
// 关闭
void COptCommDll::OnClose(void)
{
	OnDeleteClientComm();
	FreeMatrixCommDll();
}
/** 接收帧命令字处理*/
void COptCommDll::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize, CCommRecThread* pRecThread)
{
	switch (usCmd)
	{
		// 查询 OperationDelay XML文件信息
	case CmdQueryDelayOptXMLInfo:
		// 设置 OperationDelay XML文件信息
	case CmdSetDelayOptXMLInfo:
		m_oXMLDllOpt.OnProcSetDelayOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetDelayOptXMLInfo, 0);
		break;
		// 查询 炮表 XML文件信息
	case CmdQuerySourceShotOptXMLInfo:
		// 设置 炮表 XML文件信息
	case CmdSetSourceShotOptXMLInfo:
		m_oXMLDllOpt.OnProcSetSourceShotOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetSourceShotOptXMLInfo, 0);
		break;
		// 查询 Explo震源类型 XML文件信息
	case CmdQueryExploOptXMLInfo:
		// 设置 Explo震源类型 XML文件信息
	case CmdSetExploOptXMLInfo:
		m_oXMLDllOpt.OnProcSetExploOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetExploOptXMLInfo, 0);
		break;
		// 查询 Vibro震源类型 XML文件信息
	case CmdQueryVibroOptXMLInfo:
		// 设置 Vibro震源类型 XML文件信息
	case CmdSetVibroOptXMLInfo:
		m_oXMLDllOpt.OnProcSetVibroOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetVibroOptXMLInfo, 0);
		break;
		// 查询 ProcessRecord XML文件信息
	case CmdQueryProcessRecordOptXMLInfo:
		// 设置 ProcessRecord XML文件信息
	case CmdSetProcessRecordOptXMLInfo:
		m_oXMLDllOpt.OnProcSetProcessRecordOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetProcessRecordOptXMLInfo, 0);
		break;
		// 查询 ProcessAux XML文件信息
	case CmdQueryProcessAuxOptXMLInfo:
		// 设置 ProcessAux XML文件信息
	case CmdSetProcessAuxOptXMLInfo:
		m_oXMLDllOpt.OnProcSetProcessAuxOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetProcessAuxOptXMLInfo, 0);
		break;
		// 查询 ProcessAcq XML文件信息
	case CmdQueryProcessAcqOptXMLInfo:
		// 设置 ProcessAcq XML文件信息
	case CmdSetProcessAcqOptXMLInfo:
		m_oXMLDllOpt.OnProcSetProcessAcqOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetProcessAcqOptXMLInfo, 0);
		break;
		// 查询 ProcessType XML文件信息
	case CmdQueryProcessTypeOptXMLInfo:
		// 设置 ProcessType XML文件信息
	case CmdSetProcessTypeOptXMLInfo:
		m_oXMLDllOpt.OnProcSetProcessTypeOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetProcessTypeOptXMLInfo, 0);
		break;
		// 查询 注释 XML文件信息
	case CmdQueryCommentsOptXMLInfo:
		// 设置 注释 XML文件信息
	case CmdSetCommentsOptXMLInfo:
		m_oXMLDllOpt.OnProcSetCommentsOptXMLInfo(pChar, uiSize, false);
		::PostMessage(m_pCommDll->m_hWnd, WM_MSG_CLIENT, CmdSetCommentsOptXMLInfo, 0);
		break;
	default:
		break;
	}
}