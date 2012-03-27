#include "StdAfx.h"
#include "ConnectClients.h"


CConnectClients::CConnectClients(void)
{
	m_dwEventTotal = 0;
	m_SocketListen = INVALID_SOCKET;
}


CConnectClients::~CConnectClients(void)
{
}
// 得到服务器连接Socket
BOOL CConnectClients::OnGetListenSocket(void)
{
	CString str = _T("");
	if ((m_SocketListen = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		str.Format(_T("socket() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
// 选择触发事件的Socket消息
BOOL CConnectClients::OnSocketEventSelect(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents)
{
	CString str = _T("");
	if (WSAEventSelect(s, hEventObject, lNetworkEvents) == SOCKET_ERROR)
	{
		str.Format(_T("WSAEventSelect() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
// 将Socket同接收客户端IP地址和端口绑定
BOOL CConnectClients::OnSocketBind(u_long hostlong, u_short hostshort)
{
	CString str = _T("");
	m_ServerInternetAddr.sin_family = AF_INET;
	m_ServerInternetAddr.sin_addr.s_addr = htonl(hostlong);
	m_ServerInternetAddr.sin_port = htons(hostshort);
	if (bind(m_SocketListen, (PSOCKADDR) &m_ServerInternetAddr, sizeof(m_ServerInternetAddr)) == SOCKET_ERROR)
	{
		str.Format(_T("bind() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
// 设定监听客户端个数
BOOL CConnectClients::OnSocketListen(int backlog)
{
	CString str = _T("");
	if (listen(m_SocketListen, backlog))
	{
		str.Format(_T("listen() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
// 创建Server端口监听
BOOL CConnectClients::OnCreateSocketListen(u_long hostlong, u_short hostshort, int backlog)
{
	if (FALSE == OnGetListenSocket())
	{
		return FALSE;
	}
	if (FALSE == CreateSocketInformation(m_SocketListen))
	{
		return FALSE;
	}
	if (FALSE == OnSocketEventSelect(m_SocketListen, m_WSAEventArray[m_dwEventTotal - 1], FD_ACCEPT|FD_CLOSE))
	{
		return FALSE;
	}
	if (FALSE == OnSocketBind(hostlong, hostshort))
	{
		return FALSE;
	}
	if (FALSE == OnSocketListen(backlog))
	{
		return FALSE;
	}
	return TRUE;
}
// 创建一个客户端连接信息
BOOL CConnectClients::CreateSocketInformation(SOCKET s)
{
	CClientInfo* SI;
	CString str = _T("");
	if ((m_WSAEventArray[m_dwEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		str.Format(_T("WSACreateEvent() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	SI = new CClientInfo;
	// Prepare SocketInfo structure for use
	SI->Socket = s;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;
	m_ClientInfoArray[m_dwEventTotal] = SI;
	m_dwEventTotal++;
	return TRUE;
}
// 释放一个客户端连接信息
void CConnectClients::FreeSocketInformation(DWORD Event)
{
	CClientInfo* SI = m_ClientInfoArray[Event];
	CString str = _T("");
	closesocket(SI->Socket);
	delete SI;
	if(FALSE == WSACloseEvent(m_WSAEventArray[Event]))
	{
		str.Format(_T("WSACloseEvent() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
	// Squash the socket and event arrays
	for (DWORD i = Event; i < m_dwEventTotal; i++)
	{
		m_WSAEventArray[i] = m_WSAEventArray[i + 1];
		m_ClientInfoArray[i] = m_ClientInfoArray[i + 1];
	}
	m_dwEventTotal--;
}

// 处理函数
BOOL CConnectClients::OnProcComm(void)
{
	DWORD Event;
	WSANETWORKEVENTS NetworkEvents;
	CString str = _T("");
	int iResult = 0;

	// Wait for one of the sockets to receive I/O notification and 
	if ((Event = WSAWaitForMultipleEvents(m_dwEventTotal, m_WSAEventArray, FALSE, 
		WaitForCommThreadSleepTimes, FALSE)) == WSA_WAIT_FAILED)
	{
		str.Format(_T("WSAWaitForMultipleEvents() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	else if (Event == WSA_WAIT_TIMEOUT)
	{
		return TRUE;
	}
	if (WSAEnumNetworkEvents(m_ClientInfoArray[Event - WSA_WAIT_EVENT_0]->Socket,
		m_WSAEventArray[Event - WSA_WAIT_EVENT_0], &NetworkEvents) == SOCKET_ERROR)
	{
		str.Format(_T("WSAEnumNetworkEvents() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	// 处理TCP/IP连接事件
	iResult = OnProcAcceptEvent(&NetworkEvents, Event);
	if (iResult == 1)
	{
		return TRUE;
	}
	else if (iResult == -1)
	{
		return FALSE;
	}
	// 处理TCP/IP接收事件
	iResult = OnProcRevEvent(&NetworkEvents, Event);
	if (iResult == 1)
	{
		return TRUE;
	}
	else if (iResult == -1)
	{
		return FALSE;
	}
	// 处理TCP/IP发送事件
	iResult = OnProcSendEvent(&NetworkEvents, Event);
	if (iResult == 1)
	{
		return TRUE;
	}
	else if (iResult == -1)
	{
		return FALSE;
	}
	// 处理TCP/IP发送事件
	iResult = OnProcCloseEvent(&NetworkEvents, Event);
	if (iResult == 1)
	{
		return TRUE;
	}
	else if (iResult == -1)
	{
		return FALSE;
	}
	return TRUE;
}



// 处理TCP/IP连接事件
int CConnectClients::OnProcAcceptEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event)
{
	CString str = _T("");
	SOCKET SocketAccept;
	if (pNetworkEvents->lNetworkEvents & FD_ACCEPT)
	{
		if (pNetworkEvents->iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			str.Format(_T("FD_ACCEPT failed with error %d"), pNetworkEvents->iErrorCode[FD_ACCEPT_BIT]);
			AfxMessageBox(str);
			return 1;
		}
		if ((SocketAccept = accept(m_ClientInfoArray[Event - WSA_WAIT_EVENT_0]->Socket, NULL, NULL)) == INVALID_SOCKET)
		{
			str.Format(_T("accept() failed with error %d"), WSAGetLastError());
			AfxMessageBox(str);
			return 1;
		}
		if (m_dwEventTotal > WSA_MAXIMUM_WAIT_EVENTS)
		{
			AfxMessageBox(_T("Too many connections - closing socket..."));
			closesocket(SocketAccept);
			return 1;
		}
		CreateSocketInformation(SocketAccept);
		if (FALSE == OnSocketEventSelect(SocketAccept, m_WSAEventArray[m_dwEventTotal - 1], FD_READ|FD_WRITE|FD_CLOSE))
		{
			return -1;
		}
	}
	return 0;
}


// 处理TCP/IP接收事件
int CConnectClients::OnProcRevEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event)
{
	CString str = _T("");
	CClientInfo* pSocketInfo = NULL;
	DWORD Flags = 0;
	DWORD RecvBytes = 0;
	if (pNetworkEvents->lNetworkEvents & FD_READ)
	{
		if (pNetworkEvents->lNetworkEvents & FD_READ && pNetworkEvents->iErrorCode[FD_READ_BIT] != 0)
		{
			str.Format(_T("FD_READ failed with error %d"), pNetworkEvents->iErrorCode[FD_READ_BIT]);
			AfxMessageBox(str);
			return 1;
		}
		pSocketInfo = m_ClientInfoArray[Event - WSA_WAIT_EVENT_0];
		pSocketInfo->DataRevBuf.buf = pSocketInfo->RevBuffer + pSocketInfo->BytesRECV;
		pSocketInfo->DataRevBuf.len = DATA_BUFSIZE - pSocketInfo->BytesRECV;
		if (WSARecv(pSocketInfo->Socket, &(pSocketInfo->DataRevBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				str.Format(_T("WSARecv() failed with error %d"), WSAGetLastError());
				AfxMessageBox(str);
				FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
				return -1;
			}
		}
		else
		{
			pSocketInfo->BytesRECV += RecvBytes;
			// 接收数据处理

		}
	}
	return 0;
}


// 处理TCP/IP发送事件
int CConnectClients::OnProcSendEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event)
{
	CString str = _T("");
	CClientInfo* pSocketInfo = NULL;
	DWORD SendBytes = 0;
	if (pNetworkEvents->lNetworkEvents & FD_WRITE)
	{
		if (pNetworkEvents->lNetworkEvents & FD_WRITE && pNetworkEvents->iErrorCode[FD_WRITE_BIT] != 0)
		{
			str.Format(_T("FD_WRITE failed with error %d"), pNetworkEvents->iErrorCode[FD_WRITE_BIT]);
			AfxMessageBox(str);
			return 1;
		}
		pSocketInfo = m_ClientInfoArray[Event - WSA_WAIT_EVENT_0];
		pSocketInfo->DataSendBuf.buf = pSocketInfo->SendBuffer;
		pSocketInfo->DataSendBuf.len = pSocketInfo->BytesSEND;
		if (WSASend(pSocketInfo->Socket, &(pSocketInfo->DataSendBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				str.Format(_T("WSASend() failed with error %d"), WSAGetLastError());
				AfxMessageBox(str);
				FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
				return -1;
			}
		}
		else
		{
			pSocketInfo->BytesSEND -= SendBytes;
		}
	}
	return 0;
}


// 处理TCP/IP关闭事件
int CConnectClients::OnProcCloseEvent(WSANETWORKEVENTS* pNetworkEvents, DWORD Event)
{
	CString str = _T("");
	if (pNetworkEvents->lNetworkEvents & FD_CLOSE)
	{
		if (pNetworkEvents->iErrorCode[FD_CLOSE_BIT] != 0)
		{
			str.Format(_T("FD_CLOSE failed with error %d"), pNetworkEvents->iErrorCode[FD_CLOSE_BIT]);
			AfxMessageBox(str);
			return TRUE;
		}
		FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
	}
	return 0;
}
