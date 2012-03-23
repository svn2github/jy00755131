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
	SOCKET SocketAccept;
	CClientInfo* pSocketInfo;
	DWORD Event;
	WSANETWORKEVENTS NetworkEvents;
	DWORD Flags;
	CString str = _T("");
	DWORD RecvBytes;
	DWORD SendBytes;

	// Wait for one of the sockets to receive I/O notification and 
	if ((Event = WSAWaitForMultipleEvents(m_dwEventTotal, m_WSAEventArray, FALSE, 100, FALSE)) == WSA_WAIT_FAILED)
	{
		str.Format(_T("WSAWaitForMultipleEvents() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	if (WSAEnumNetworkEvents(m_ClientInfoArray[Event - WSA_WAIT_EVENT_0]->Socket,
		m_WSAEventArray[Event - WSA_WAIT_EVENT_0], &NetworkEvents) == SOCKET_ERROR)
	{
		str.Format(_T("WSAEnumNetworkEvents() failed with error %d"), WSAGetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
	{
		if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			str.Format(_T("FD_ACCEPT failed with error %d"), NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
			AfxMessageBox(str);
			return TRUE;
		}
		if ((SocketAccept = accept(m_ClientInfoArray[Event - WSA_WAIT_EVENT_0]->Socket, NULL, NULL)) == INVALID_SOCKET)
		{
			printf("accept() failed with error %d\n", WSAGetLastError());
			return TRUE;
		}
		if (m_dwEventTotal > WSA_MAXIMUM_WAIT_EVENTS)
		{
			printf("Too many connections - closing socket...\n");
			closesocket(SocketAccept);
			return TRUE;
		}
		CreateSocketInformation(SocketAccept);
		if (FALSE == OnSocketEventSelect(SocketAccept, m_WSAEventArray[m_dwEventTotal - 1], FD_READ|FD_WRITE|FD_CLOSE))
		{
			return FALSE;
		}
		printf("Socket %d got connected...\n", SocketAccept);
	}
	// Try to read and write data to and from the data buffer if read and write events occur
	if (NetworkEvents.lNetworkEvents & FD_READ || NetworkEvents.lNetworkEvents & FD_WRITE)
	{
		if (NetworkEvents.lNetworkEvents & FD_READ && NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			printf("FD_READ failed with error %d\n", NetworkEvents.iErrorCode[FD_READ_BIT]);
			return TRUE;
		}
		else
			printf("FD_READ is OK!\n");
		if (NetworkEvents.lNetworkEvents & FD_WRITE && NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
		{
			printf("FD_WRITE failed with error %d\n", NetworkEvents.iErrorCode[FD_WRITE_BIT]);
			return TRUE;
		}
		else
			printf("FD_WRITE is OK!\n");
		pSocketInfo = m_ClientInfoArray[Event - WSA_WAIT_EVENT_0];
		// Read data only if the receive buffer is empty
		if (pSocketInfo->BytesRECV == 0)
		{
			pSocketInfo->DataBuf.buf = pSocketInfo->Buffer;
			pSocketInfo->DataBuf.len = DATA_BUFSIZE;
			Flags = 0;
			if (WSARecv(pSocketInfo->Socket, &(pSocketInfo->DataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSARecv() failed with error %d\n", WSAGetLastError());
					FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
					return FALSE;
				}
			}
			else
			{
				printf("WSARecv() is working!\n");
				pSocketInfo->BytesRECV = RecvBytes;
			}
		}
		// Write buffer data if it is available
		if (pSocketInfo->BytesRECV > pSocketInfo->BytesSEND)
		{
			pSocketInfo->DataBuf.buf = pSocketInfo->Buffer + pSocketInfo->BytesSEND;
			pSocketInfo->DataBuf.len = pSocketInfo->BytesRECV - pSocketInfo->BytesSEND;
			if (WSASend(pSocketInfo->Socket, &(pSocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
					return FALSE;
				}
				// A WSAEWOULDBLOCK error has occurred. An FD_WRITE event will be posted
				// when more buffer space becomes available
			}
			else
			{
				printf("WSASend() is fine! Thank you...\n");
				pSocketInfo->BytesSEND += SendBytes;
				if (pSocketInfo->BytesSEND == pSocketInfo->BytesRECV)
				{
					pSocketInfo->BytesSEND = 0;
					pSocketInfo->BytesRECV = 0;
				}
			}
		}
	}
	if (NetworkEvents.lNetworkEvents & FD_CLOSE)
	{
		if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			printf("FD_CLOSE failed with error %d\n", NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
			return TRUE;
		}
		else
			printf("FD_CLOSE is OK!\n");
		printf("Closing socket information %d\n", m_ClientInfoArray[Event - WSA_WAIT_EVENT_0]->Socket);
		FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
	}
	return TRUE;
}

