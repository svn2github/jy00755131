#include "stdafx.h"
#include "MatrixCommDll.h"

CCommServer::CCommServer()
{
	m_pComClientMap = NULL;
}

CCommServer::~CCommServer()
{
}


// CComServer 成员函数


void CCommServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CCommClient* pComClient = NULL;
	pComClient = new CCommClient;
	if (CAsyncSocket::Accept(pComClient->m_oClientSocket))
	{
		pComClient->m_pComClientMap = m_pComClientMap;
		pComClient->OnInit();
	}
	else
	{
		delete pComClient;
		pComClient = NULL;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


// 初始化
void CCommServer::OnInit(unsigned int uiSocketPort, int iSocketType, LPCTSTR lpszSocketAddress)
{
	BOOL bFlag = FALSE;
	bFlag = CAsyncSocket::Create(uiSocketPort, iSocketType, 
		FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE, lpszSocketAddress);
	if (!bFlag)
	{
		AfxMessageBox(_T("Server Socket Create Error!"));
		PostQuitMessage(0);
		return;
	}
	CAsyncSocket::Listen(ListenClientMaxNum);
}


// 关闭
void CCommServer::OnClose(void)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	int iSize = m_pComClientMap->size();
	for (int i=0; i<iSize; i++)
	{
		iter = m_pComClientMap->begin();
		iter->second->OnClose();
	}
	Close();
}