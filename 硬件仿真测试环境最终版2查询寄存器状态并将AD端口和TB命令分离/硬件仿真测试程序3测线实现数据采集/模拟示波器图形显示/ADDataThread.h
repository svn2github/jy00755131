#pragma once
#include "afxsock.h"



// ADDataThread

class CADDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CADDataThread)

public:
	CADDataThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CADDataThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:

	// 初始化
	void OnInit(void);

	// 关闭并结束线程
	void OnClose(void);
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hADDDataThreadClose;
	// AD数据接收Socket
	CSocket m_ADDataSocket;
	
	// AD数据接收缓冲区
	byte ADData[256];
};


