#pragma once
#include "ThreadComm.h"
#include "ThreadSend.h"

class CCom
{
public:
	CCom(void);
	~CCom(void);
public:
	// 网络连接和接收线程
	CThreadComm m_oThreadComm;
	// 网络发送线程
	CThreadSend m_oThreadSend;
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

