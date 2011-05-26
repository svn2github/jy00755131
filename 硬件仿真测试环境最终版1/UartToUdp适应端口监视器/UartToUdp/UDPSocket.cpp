// UDPSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "UartToUdp.h"
#include "UDPSocket.h"
#include "UartToUdpDlg.h"


// CUDPSocket

CUDPSocket::CUDPSocket(CUartToUdpDlg* pTDoc)
{
	m_pTDoc = pTDoc;
	udp_count = 0;
}

CUDPSocket::~CUDPSocket()
{
}
// CUDPSocket 成员函数
void CUDPSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int ret=0;
	ret = Receive(udp_buf[udp_count],1024);

	if(ret==ERROR)
	{
		TRACE("ERROR!");
	}
	else if(ret != 0xffffffff) 
	{
		m_pTDoc->OnSendFromSerialPort(udp_buf[udp_count],ret);
		udp_count +=1;
		udp_count = udp_count%8;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
