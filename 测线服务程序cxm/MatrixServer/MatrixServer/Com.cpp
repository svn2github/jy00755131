#include "StdAfx.h"
#include "Com.h"


CCom::CCom(void)
{
}


CCom::~CCom(void)
{
}


// 初始化
void CCom::OnInit(void)
{
	// 初始化套接字库
	OnInitSocketLib();
	// 初始化服务端AsyncSocket
	m_oServerAsyncSocket.OnInit();
// 	// 初始化网络连接和接收线程
// 	m_oThreadComm.OnInit();
// 	// 初始化网络发送线程
// 	m_oThreadSend.OnInit();
}


// 关闭
void CCom::OnClose(void)
{
// 	// 关闭网络连接和接收线程
// 	m_oThreadComm.OnClose();
// 	// 关闭网络发送线程
// 	m_oThreadSend.OnClose();
	// 关闭服务端AsyncSocket
	m_oServerAsyncSocket.OnClose();
	// 释放套接字库
	OnCloseSocketLib();
}

// 初始化套接字库
void CCom::OnInitSocketLib(void)
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
void CCom::OnCloseSocketLib(void)
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