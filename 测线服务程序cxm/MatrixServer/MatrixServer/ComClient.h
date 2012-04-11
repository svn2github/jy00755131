#pragma once
#include "ClientRecFrame.h"
#include "ClientRecThread.h"
#include "ClientSocket.h"

// CComClient 命令目标

class CComClient
{
public:
	CComClient();
	virtual ~CComClient();
public:
	// 接收客户端帧成员类
	CClientRecFrame m_oClientRecFrame;
	// 接收客户端帧的处理线程成员类
	CClientRecThread m_oClientRecThread;
	// 接收客户端通讯成员类
	CClientSocket m_oClientSocket;
public:
	// 创建一个客户端连接信息
	void OnInit();
	// 释放一个客户端连接信息
	void OnClose();
};


