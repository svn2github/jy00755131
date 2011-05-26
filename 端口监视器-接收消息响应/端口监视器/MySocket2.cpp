// MySocket2.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "MySocket2.h"
#include "PortMonitoringDlg.h"

// CMySocket2

CMySocket2::CMySocket2(CPortMonitoringDlg* pTDoc)
{
	m_pTDoc=pTDoc;
	udp_count = 0;
}

CMySocket2::~CMySocket2()
{
}


// CMySocket2 成员函数
void CMySocket2::OnReceive(int nErrorCode) 
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
		unsigned short uiPort;
		bool bError = false;
		memcpy(&uiPort, &udp_buf[udp_count][24], 2);
		SendTo(udp_buf[udp_count],ret,uiPort,m_pTDoc->m_strSendIP);
		for (int j=0; j<UDPPortNb; j++)
		{
			if (uiPort == m_pTDoc->m_uiPortNb[j])
			{
				bError = true;
			}
		}
		if (bError == true)
		{
			m_pTDoc->OnSaveSendData(udp_buf[udp_count],ret, true);
		}
		else
		{
			m_pTDoc->OnSaveSendData(udp_buf[udp_count],ret, false);
		}
		udp_count +=1;
		udp_count = udp_count%8;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
