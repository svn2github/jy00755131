#pragma once
#define DATA_BUFSIZE 8192
class CClientInfo
{
public:
	CClientInfo(void);
	~CClientInfo(void);
public:
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	DWORD BytesSEND;
	DWORD BytesRECV;
	SOCKADDR_IN m_ClientAddr;
};

