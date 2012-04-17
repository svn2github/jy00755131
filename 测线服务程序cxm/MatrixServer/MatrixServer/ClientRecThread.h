#pragma once
#include "ClientRecFrame.h"
#include "ClientSndFrame.h"
#include "ClientCommThread.h"
// 接收来自客户端的帧线程
class CClientRecThread : public CClientCommThread
{
public:
	CClientRecThread(void);
	~CClientRecThread(void);
public:
	// 接收客户端类指针
	CClientRecFrame* m_pClientRecFrame;
	// 发送客户端类指针
	CClientSndFrame* m_pClientSndFrame;
 	// 处理函数
 	void OnProc(void);
};

