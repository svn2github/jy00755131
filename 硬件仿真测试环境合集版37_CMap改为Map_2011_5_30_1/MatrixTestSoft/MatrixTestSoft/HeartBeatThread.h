#pragma once
#include "Parameter.h"


// CHeartBeatThread

class CHeartBeatThread : public CWinThread
{
	DECLARE_DYNCREATE(CHeartBeatThread)

public:
	CHeartBeatThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CHeartBeatThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
protected:
	// 帧数据
	byte m_pFrameData[SndFrameSize];
public:
	// 源IP地址
	CString m_csIPSource;
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hHeartBeatThreadClose;
	// 发送端口
	unsigned int m_uiSendPort;
	// 心跳Socket套接字
	sockaddr_in addr, addr2;
	SOCKET m_HeartBeatSocket;
protected:
	// 生成心跳帧
	void MakeHeartBeatFrame(void);
	// 发送心跳帧
	void SendHeartBeatFrame(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
};


