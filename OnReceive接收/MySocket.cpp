// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "OnReceive接收.h"
#include "MySocket.h"


// CMySocket

CMySocket::CMySocket()
: m_uiRcvByteNum(0)
{
}

CMySocket::~CMySocket()
{
}


// CMySocket 成员函数

void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSocket::OnReceive(nErrorCode);
// 	DWORD dwReceived;
 	unsigned char ucUdpBuf[51200];
 	int iCount = 0;
// 	if (IOCtl(FIONREAD, &dwReceived))
// 	{
// 		if (dwReceived > 0)   // Process only if you have enough data
// 		{
			iCount = Receive(ucUdpBuf,51200);
			m_uiRcvByteNum += 256;
		//	SendTo(ucUdpBuf, 256, 0x9800, _T("192.168.0.19"));
// 		}
// 	}
// 	else
// 	{
// 		// Error handling here
// 	}

}
