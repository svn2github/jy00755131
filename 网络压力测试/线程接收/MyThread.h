#pragma once
#include "afxsock.h"



// CMyThread

class CMyThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyThread)

public:
	CMyThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// Socket对象
	CSocket m_Socket;
	virtual int Run();
	// 接收到的字节数
	unsigned int m_uiRcvByteNum;
};


