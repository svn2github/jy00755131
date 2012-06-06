#pragma once
#include "CommProtocol.h"
/**
* @brief 与客户端通讯线程基类
*/
class CClientCommThread
{
public:
	CClientCommThread(void);
	virtual ~CClientCommThread(void);
public:
	/** 初始化*/
	void OnInit(void);
	/** 关闭*/
	void OnClose(void);
	/** 线程句柄*/
	HANDLE m_hThread;
	/** 线程ID*/
	DWORD m_dwThreadID;
	/** 线程函数*/
	DWORD ThreadRunFunc(void);
	/** 线程等待函数*/
	void WaitForThread(void);
	/** 线程关闭事件*/
	HANDLE m_hThreadClose;
	/** 线程关闭标志位*/
	bool m_bClose;
public:
	/** 处理函数*/
	virtual void OnProc(void);
};

