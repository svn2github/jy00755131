#pragma once



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
public:
	CSocket m_HeartBeatSocket;
	// 帧大小
	int m_iFrameSize;
	// 帧数据
	byte m_pFrameData[256];
	// 源IP地址
	unsigned int m_uiIPSource;
	// 目标IP地址
	unsigned int m_uiIPAim;
	// 目标端口号
	unsigned short m_usPortAim;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	unsigned short m_usCommand;
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hHeartBeatThreadClose;
	// 生成心跳帧
	void MakeHeartBeatFrame(void);
	// 发送心跳帧
	void SendHeartBeatFrame(void);
};


