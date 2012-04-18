#pragma once
#include "ClientSndFrame.h"
#include "ClientCommThread.h"
// 向客户端发送帧的发送线程
class CClientSndThread : public CClientCommThread
{
public:
	CClientSndThread(void);
	~CClientSndThread(void);
public:
	// 发送客户端类指针
	CClientSndFrame* m_pClientSndFrame;
	// 处理函数
	void OnProc(void);
};

