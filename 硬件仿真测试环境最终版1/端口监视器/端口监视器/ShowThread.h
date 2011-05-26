#pragma once



// CShowThread

class CShowThread : public CWinThread
{
	DECLARE_DYNCREATE(CShowThread)

public:
	CShowThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CShowThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	// 初始化
	void Init();
	bool m_close;
	void OnClose();
	int m_icsEditShowLength;	// 上一次的字符串长度 
protected:
	DECLARE_MESSAGE_MAP()
};


