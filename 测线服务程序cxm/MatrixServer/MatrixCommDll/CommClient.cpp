#include "stdafx.h"
#include "MatrixCommDll.h"

CCommClient::CCommClient()
{
	m_pComClientMap = NULL;
	m_bClient = false;
	m_iClientType = ClientTypeLine;
	m_bCheckConnected = false;
	m_strClientIP = _T("");
	m_ptr = NULL;
	m_strLinePath = _T("");
	m_strOptPath = _T("");
}

CCommClient::~CCommClient()
{
}

// 创建一个客户端连接信息
void CCommClient::OnInit(unsigned int uiPort, CString strIP, HWND hWnd)
{
	if (m_bClient == true)
	{
		if ( FALSE == m_oClientSocket.Create(0, SOCK_STREAM, 
			FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE))
		{
			AfxMessageBox(_T("Client Socket Create Error!"));
			return;
		}
	}
	m_oClientSocket.OnInit(this, CommSndBufferSize, CommRecBufferSize);
	m_oClientSocket.m_hWnd = hWnd;
	m_oClientSocket.m_bClient = m_bClient;
	m_oRecFrame.OnInit();
	m_oRecThread.m_pCommRecFrame = &m_oRecFrame;
	m_oRecThread.m_pCommSndFrame = &m_oSndFrame;
	m_oRecThread.m_oProcRecCmdCallBack = m_oProcRecCmdCallBack;
	m_oRecThread.OnInit();
	m_oSndFrame.OnInit();
	m_oSndFrame.m_pClientSocket = &m_oClientSocket;
	m_oSndThread.m_pCommSndFrame = &m_oSndFrame;
	m_oSndThread.OnInit();
	if (m_bClient == true)
	{
		m_oHeartBeatThread.m_pCommSndFrame = &m_oSndFrame;
		m_oHeartBeatThread.OnInit();
		m_oClientSocket.m_uiServerPort = uiPort;
		m_oClientSocket.m_strServerIP = strIP;
		// 连接服务器
		m_oClientSocket.ConnectServer();
	}
}

// 释放一个客户端连接信息
void CCommClient::OnClose(void)
{
	m_oSndThread.OnClose();
	m_oRecThread.OnClose();
	if (m_bClient == true)
	{
		m_oHeartBeatThread.OnClose();
	}
	else
	{
		(*m_oCloseClientXml)(this);
	}
	m_oRecFrame.OnClose();
	m_oSndFrame.OnClose();
	m_oClientSocket.OnClose();
	if (m_ptr != NULL)
	{
		free(m_ptr);
	}
	delete this;
}
/** 向所有在线客户端广播配置文件变更*/
void CCommClient::BroadCastXMLChange(unsigned short usCmd, char* pChar, unsigned int uiSize)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	if (m_pComClientMap == NULL)
	{
		return;
	}
	for (iter = m_pComClientMap->begin(); iter != m_pComClientMap->end(); iter++)
	{
		if (iter->second->m_bCheckConnected == true)
		{
			iter->second->m_oSndFrame.MakeSetFrame(usCmd, pChar, uiSize);
		}
	}
}