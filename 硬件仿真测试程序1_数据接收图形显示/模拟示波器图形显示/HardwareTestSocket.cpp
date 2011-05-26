// HardwareTestSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "HardwareTestSocket.h"
#include "HardwareTestDlg.h"

// CHardwareTestSocket

CHardwareTestSocket::CHardwareTestSocket(CHardwareTestDlg* pDlg)
{
	m_pDlg = pDlg;
	udp_count = 0;
}

CHardwareTestSocket::~CHardwareTestSocket()
{
}


// CHardwareTestSocket 成员函数
void CHardwareTestSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int ret=0;

	ret=Receive(udp_buf[udp_count],1024);

	if(ret==ERROR)
	{
		TRACE("ERROR!");
	}
	else if(ret != 0xffffffff) 
	{
		
		byte* pSampleData = &(udp_buf[udp_count][16]);
		m_pDlg->ReceiveSampleData(pSampleData);
		udp_count +=1;
		udp_count = udp_count%8;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
