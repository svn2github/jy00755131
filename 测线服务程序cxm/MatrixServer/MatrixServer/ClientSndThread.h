#pragma once
#include "ClientSndFrame.h"
#include "ClientCommThread.h"
/**
* @class CClientSndThread
* @brief 与客户端通讯发送线程类
*/
class CClientSndThread : public CClientCommThread
{
public:
	CClientSndThread(void);
	~CClientSndThread(void);
public:
	/** 发送客户端类指针*/
	CClientSndFrame* m_pClientSndFrame;
public:
	// 处理函数
	void OnProc(void);
};

