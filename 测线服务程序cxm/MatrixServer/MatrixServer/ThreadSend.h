#pragma once
// 线程函数的等待时间
#define WaitForSendThreadSleepTimes	100
// 线程关闭的等待时间
#define WaitForCloseSendThreadTimes	500
class CThreadSend
{
public:
	CThreadSend(void);
	~CThreadSend(void);
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
	// 关闭标志位
	bool m_bClose;
};

