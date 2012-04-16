#pragma once
#include "ClientRecFrame.h"
#include "ClientRecThread.h"
#include "ClientSndFrame.h"
#include "ClientSndThread.h"
#include "ClientSocket.h"

// CComClient 命令目标

class CCommClient
{
public:
	CCommClient();
	virtual ~CCommClient();
public:
	// 接收客户端帧成员类
	CClientRecFrame m_oClientRecFrame;
	// 接收客户端帧的处理线程成员类
	CClientRecThread m_oClientRecThread;
	// 发送客户端帧成员类
	CClientSndFrame m_oClientSndFrame;
	// 发送客户端帧的处理线程成员类
	CClientSndThread m_oClientSndThread;
	// 接收客户端通讯成员类
	CClientSocket m_oClientSocket;
public:
	// 创建一个客户端连接信息
	void OnInit();
	// 释放一个客户端连接信息
	void OnClose();
};


