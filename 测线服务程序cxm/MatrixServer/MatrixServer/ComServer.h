#pragma once
#include "ComClient.h"

// CComServer 命令目标

class CComServer : public CAsyncSocket
{
public:
	CComServer();
	virtual ~CComServer();
	virtual void OnAccept(int nErrorCode);
public:
	// 客户端连接索引
	hash_map<SOCKET, CComClient*> m_oComClientMap;
public:
	// 初始化
	void OnInit(unsigned int uiSocketPort = 8080, int iSocketType = SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL);
	// 关闭
	void OnClose(void);
};


