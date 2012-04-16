#pragma once
#include "CommProtocol.h"
class CClientCommThread
{
public:
	CClientCommThread(void);
	~CClientCommThread(void);
public:
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
public:
	// 处理函数
	virtual void OnProc(void);
};

