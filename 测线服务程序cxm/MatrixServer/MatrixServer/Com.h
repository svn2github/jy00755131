#pragma once
#include "ComServer.h"

class CCom
{
public:
	CCom(void);
	~CCom(void);
public:
	// 服务端CAsyncSocket
	CComServer m_oServerAsyncSocket;
public:
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 初始化套接字库
	void OnInitSocketLib(void);
	// 释放套接字库
	void OnCloseSocketLib(void);
};

