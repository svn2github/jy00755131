// ComServer.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "ComServer.h"


// CComServer

CComServer::CComServer()
{
}

CComServer::~CComServer()
{
}


// CComServer 成员函数


void CComServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CComClient* pComClient = NULL;
	pComClient = new CComClient;
	if (CAsyncSocket::Accept(*pComClient))
	{
		pComClient->m_pComClientMap = &m_oComClientMap;
		pComClient->CreateSocketInformation();
	}
	else
	{
		delete pComClient;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


// 初始化
void CComServer::OnInit(unsigned int uiSocketPort, int iSocketType, LPCTSTR lpszSocketAddress)
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
	CAsyncSocket::Listen(ConnectClientMaxNum);
	m_oComClientMap.clear();
}


// 关闭
void CComServer::OnClose(void)
{
	hash_map<SOCKET, CComClient*>::iterator iter;
	for (iter = m_oComClientMap.begin(); iter != m_oComClientMap.end();)
	{
		delete iter->second;
		m_oComClientMap.erase(iter++);
	}
	CAsyncSocket::Close();
}