#pragma once
#define DATA_BUFSIZE 8192
class CServerSocketEvent
{
public:
	CServerSocketEvent(void);
	~CServerSocketEvent(void);
public:
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	DWORD BytesSEND;
	DWORD BytesRECV;
	SOCKADDR_IN m_ClientAddr;
};

