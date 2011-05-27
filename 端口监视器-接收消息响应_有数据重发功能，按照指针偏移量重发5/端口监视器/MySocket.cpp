// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "MySocket.h"
#include "PortMonitoringDlg.h"

// CMySocket

CMySocket::CMySocket(CPortMonitoringDlg* pTDoc)
{
	m_pTDoc=pTDoc;
	udp_count = 0;
	m_uiUdpCount = 0;
}

CMySocket::~CMySocket()
{
}


// CMySocket 成员函数
void CMySocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int icount=0;

	icount=Receive(m_ucUdpBuf,8192);

	if(icount==ERROR)
	{
		TRACE("ERROR!");
	}
	else if(icount != 0xffffffff) 
	{
		for (int i=0; i<icount; i++)
		{
			switch(m_uiUdpCount)
			{
			case 0:
				if (m_ucUdpBuf[i] == 0x11)
				{
					udp_buf[udp_count][0] = m_ucUdpBuf[i];
					m_uiUdpCount++;
					break;
				}
				else
				{
					m_uiUdpCount = 0;
					break;
				}

			case 1:
				if (m_ucUdpBuf[i] == 0x22)
				{
					udp_buf[udp_count][1] = m_ucUdpBuf[i];
					m_uiUdpCount++;
					break;
				}
				else
				{
					m_uiUdpCount = 0;
					break;
				}
			case 2:
				if (m_ucUdpBuf[i] == 0x33)
				{
					udp_buf[udp_count][2] = m_ucUdpBuf[i];
					m_uiUdpCount++;
					break;
				}
				else
				{
					m_uiUdpCount = 0;
					break;
				}
			case 3:
				if (m_ucUdpBuf[i] == 0x44)
				{
					udp_buf[udp_count][3] = m_ucUdpBuf[i];
					m_uiUdpCount++;
					break;
				}
				else
				{
					m_uiUdpCount = 0;
					break;
				}
			default:
				udp_buf[udp_count][m_uiUdpCount] = m_ucUdpBuf[i];
				m_uiUdpCount++;
				if (m_uiUdpCount == 128)
				{
					m_uiUdpCount = 0;
					SendTo(udp_buf[udp_count],128,SendPort1,m_pTDoc->m_strReceiveIP);
					m_pTDoc->OnSaveReceiveData(udp_buf[udp_count],128);
					udp_count +=1;
					udp_count = udp_count%8;
				}
				break;
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
