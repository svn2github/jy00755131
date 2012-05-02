#pragma once
#include "CommClient.h"
#include "MatrixDllCall.h"
// CComServer 命令目标

class CCommServer : public CAsyncSocket
{
public:
	CCommServer();
	virtual ~CCommServer();
	virtual void OnAccept(int nErrorCode);
public:
	// 客户端连接索引
	hash_map<SOCKET, CCommClient*> m_oComClientMap;
	// DLL函数调用类成员
	CMatrixDllCall* m_pMatrixDllCall;
public:
	// 初始化
	void OnInit(unsigned int uiSocketPort = 8080, int iSocketType = SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL);
	// 关闭
	void OnClose(void);
};


