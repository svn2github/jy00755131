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
	// 连接验证
	bool m_bCheckConnected;
 	// 处理函数
 	void OnProc(void);
	// 将帧内容加入待处理任务中
	void SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame);
	// 处理接收命令函数
	void OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize);
};

