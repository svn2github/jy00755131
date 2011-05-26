#pragma once


// CSendThread

class CSendThread : public CWinThread
{

	DECLARE_DYNCREATE(CSendThread)

public:
	CSendThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSendThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSocket m_hSocket2;
	//CPortMonitoringDlg* m_pTDoc;
	// 初始化
	void Init();
	bool m_close;
	void OnClose();
	unsigned char udp_buf[8][256];
	unsigned char udp_count;
};


