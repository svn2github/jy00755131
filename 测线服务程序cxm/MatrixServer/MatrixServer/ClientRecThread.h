#pragma once
#include "ClientRecFrame.h"

class CClientRecThread
{
public:
	CClientRecThread(void);
	~CClientRecThread(void);
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 线程句柄
	HANDLE m_hThread;
	// 线程ID
	DWORD m_dwThreadID;
	// 线程函数
	DWORD ThreadRunFunc(void);
	// 线程关闭事件
	HANDLE m_hThreadClose;
	// 线程执行事件
	HANDLE m_hThreadRun;
	// 接收客户端类指针
	CClientRecFrame* m_pClientRecFrame;
	// 处理接收帧
	void OnProcRecFrame(void);
};

