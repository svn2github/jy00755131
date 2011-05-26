#pragma once


// CRecThread

class CRecThread : public CWinThread
{
	DECLARE_DYNCREATE(CRecThread)

public:
	CRecThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CRecThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSocket m_hSocket1;
	// 线程结束事件
	HANDLE m_hRecThreadClose;
	// 初始化
	void Init();
	bool m_close;
	void OnClose();
	// 数据接收缓冲
	unsigned char udp_buf[8][128];
	unsigned char udp_count;
	
};


