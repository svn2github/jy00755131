#include "StdAfx.h"
#include "Comm.h"


CComm::CComm(void)
{
}


CComm::~CComm(void)
{
}


// 初始化
void CComm::OnInit(void)
{
	// 初始化套接字库
	OnInitSocketLib();
	// 初始化服务端AsyncSocket
	m_oServerAsyncSocket.OnInit();

}


// 关闭
void CComm::OnClose(void)
{
	// 关闭服务端AsyncSocket
	m_oServerAsyncSocket.OnClose();
	// 释放套接字库
	OnCloseSocketLib();
}

// 初始化套接字库
void CComm::OnInitSocketLib(void)
{
	WSADATA wsaData;
	CString str = _T("");
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		str.Format(_T("WSAStartup() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
}
// 释放套接字库
void CComm::OnCloseSocketLib(void)
{
	CString str = _T("");
	// 释放套接字库
	if (WSACleanup() != 0)
	{
		str.Format(_T("WSACleanup() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		PostQuitMessage(0);
	}
}