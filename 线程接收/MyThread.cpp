// MyThread.cpp : 实现文件
//

#include "stdafx.h"
#include "线程接收.h"
#include "MyThread.h"


// CMyThread

IMPLEMENT_DYNCREATE(CMyThread, CWinThread)

CMyThread::CMyThread()
: m_uiRcvByteNum(0)
{
}

CMyThread::~CMyThread()
{
}

BOOL CMyThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CMyThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyThread, CWinThread)
END_MESSAGE_MAP()


// CMyThread 消息处理程序

int CMyThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类
	while(1)
	{
		DWORD dwReceived;
		unsigned char ucUdpBuf[2048];
		int iCount = 0;
		while(m_Socket.IOCtl(FIONREAD, &dwReceived))
		{
			if (dwReceived > 0)   // Process only if you have enough data
			{
			//	iCount = m_Socket.Receive(ucUdpBuf,2048);
				CString str = _T("192.168.1.56");
				unsigned int uiPort = 0x9001;
				// ReceiveFrom可以绑定数据来源（来源地的IP和端口）
				iCount = m_Socket.ReceiveFrom(ucUdpBuf, 2048, str, uiPort);
				m_uiRcvByteNum += iCount;
			}
		}
		Sleep(1);
	}
	return CWinThread::Run();
}
