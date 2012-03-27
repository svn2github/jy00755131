#pragma once
#define DATA_BUFSIZE 8192
class CClientInfo
{
public:
	CClientInfo(void);
	~CClientInfo(void);
public:
	CHAR RevBuffer[DATA_BUFSIZE];
	CHAR SendBuffer[DATA_BUFSIZE];
	WSABUF DataRevBuf;
	WSABUF DataSendBuf;
	SOCKET Socket;
	DWORD BytesSEND;
	DWORD BytesRECV;
};

