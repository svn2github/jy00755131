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
	m_uiADCFrameCount = 0;
	m_uiSendADCRetransmissionFramNum = 0;
	m_uiUdpCount = 0;

	m_uiPortNb[0] = 0x9002;
	m_uiPortNb[1] = 0x7000;
	m_uiPortNb[2] = 0x8000;
	m_uiPortNb[3] = 0x9000;
	m_uiPortNb[4] = 0x9100;
	m_uiPortNb[5] = 0x9200;
	m_uiPortNb[6] = 0x9300;
	m_uiPortNb[7] = 0x8204;
	m_uiPortNb[8] = 0x8300;
	m_uiPortNb[9] = 0x8205;
	m_uiPortNb[10] = 0x8206;
	m_uiPortNb[11] = 0x6666;
	m_uiPortNb[12] = 0x6200;
	m_uiPortNb[13] = 0x6100;
	m_uiPortNb[14] = 0x6400;
	m_uiPortNb[15] = 0x6600;
	m_uiPortNb[16] = 0x6300;
	m_uiPortNb[17] = 0x6700;
}

CMySocket2::~CMySocket2()
{
	m_oADCLostMap.RemoveAll();
	m_oADCSendMap.RemoveAll();
}


// CMySocket2 成员函数
void CMySocket2::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int icount = 0;
	unsigned short usPort;
	unsigned int	uiIPAim = 0;			// 目标IP地址
	unsigned short	usDataPointNow = 0;		// 指针偏移量
	unsigned short	usCommand = 0;			// 命令字
	bool bError = false;

	icount = Receive(m_ucUdpBuf, 16384);

	if(icount == ERROR)
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
					//	m_uiUdpCount = 0;
					m_uiUdpCount++;
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
					//	m_uiUdpCount = 0;
					m_uiUdpCount++;
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
					//	m_uiUdpCount = 0;
					m_uiUdpCount++;
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
					//	m_uiUdpCount = 0;
					m_uiUdpCount++;
					break;
				}
			default:
				udp_buf[udp_count][m_uiUdpCount] = m_ucUdpBuf[i];
				m_uiUdpCount++;
				if (m_uiUdpCount == 256)
				{
					m_uiUdpCount = 0;

					// 加入端口分发功能
					usPort = 0;
					memcpy(&usPort, &udp_buf[udp_count][24], 2);
					usCommand = 0;
					memcpy(&usCommand, &udp_buf[udp_count][26], 2);
					// 通过命令字判断是ADC接收数据
					if (usCommand == ADCRetransmissionCmd)
					{
						uiIPAim = 0;			// 目标IP地址
						memcpy(&uiIPAim, &udp_buf[udp_count][16], 4);
						usDataPointNow = 0;		// 指针偏移量
						memcpy(&usDataPointNow, &udp_buf[udp_count][28], 2);
						m_uiADCFrameCount++;
						if (m_pTDoc->m_bStartSample == TRUE)
						{
							// 对ADC接收数据设置数据重发
							OnADCDataRetransmission(uiIPAim, usDataPointNow);
						}
					}

					SendTo(udp_buf[udp_count],256,usPort,m_pTDoc->m_strSendIP);

					for (int j=0; j<UDPPortNb; j++)
					{
						if (usPort == m_uiPortNb[j])
						{
							bError = true;
						}
					}
					if (bError == true)
					{
						m_pTDoc->OnSaveSendData(udp_buf[udp_count],icount, true);
					}
					else
					{
						m_pTDoc->OnSaveSendData(udp_buf[udp_count],icount, false);
					}
					udp_count +=1;
					udp_count = udp_count%8;
					if (m_pTDoc->m_bStartSample == TRUE)
					{
						SendADCDataRetransmissionFrame();
					}
				}
				break;
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
// 设置ADC数据重发
void CMySocket2::OnADCDataRetransmission(unsigned int uiIPAim, unsigned short usDataPointNb)
{
	m_structADC ADCFindFromStruct;
	unsigned int uiIPAimForTest = 0;
	// 接收到的ADC数据不是重发数据
	if (m_oADCLostMap.Lookup(usDataPointNb, ADCFindFromStruct) == NULL)
	{
		OnListCountAdd();
		m_structADC ADCDataStruct;
		ADCDataStruct.uiRetransmissionNum = 0;
		ADCDataStruct.uiFrameCount = 0;
		ADCDataStruct.uiIPAim = uiIPAim;
		m_oADCLostMap.SetAt(usDataPointNb, ADCDataStruct);
		m_oADCSendMap.SetAt(usDataPointNb, ADCDataStruct);
	}
	else
	{
		// 如果为重发数据则修改IP地址为虚拟仪器IP地址
		ADCFindFromStruct.uiRetransmissionNum++;
		uiIPAimForTest = uiIPAim + ADCFindFromStruct.uiRetransmissionNum * 10;
		memcpy(&udp_buf[udp_count][16], &uiIPAimForTest, 4);
		if (ADCFindFromStruct.uiRetransmissionNum == InstrumentForTest)
		{
			m_oADCLostMap.RemoveKey(usDataPointNb);
		}
		else
		{
			m_oADCLostMap.SetAt(usDataPointNb, ADCFindFromStruct);
		}
	}
}

// 生成ADC数据重发帧
void CMySocket2::MakeADCDataRetransmissionFrame(unsigned int uiIPAim, unsigned short usDataPointNb)
{
	unsigned int uiIPSource =	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	m_ucADCRetransmission[0] = 0x11;
	m_ucADCRetransmission[1] = 0x22;
	m_ucADCRetransmission[2] = 0x33;
	m_ucADCRetransmission[3] = 0x44;
	m_ucADCRetransmission[4] = 0x00;
	m_ucADCRetransmission[5] = 0x00;
	m_ucADCRetransmission[6] = 0x00;
	m_ucADCRetransmission[7] = 0x00;
	m_ucADCRetransmission[8] = 0x00;
	m_ucADCRetransmission[9] = 0x00;
	m_ucADCRetransmission[10] = 0x00;
	m_ucADCRetransmission[11] = 0x00;
	m_ucADCRetransmission[12] = 0x00;
	m_ucADCRetransmission[13] = 0x00;
	m_ucADCRetransmission[14] = 0x00;
	m_ucADCRetransmission[15] = 0x00;


	uiIPSource	=	inet_addr("127.0.0.1");
	usPortAim	=	ADRecPort;
	usCommand	=	ADCRetransmissionCmd;
	// 源IP地址
	memcpy(&m_ucADCRetransmission[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_ucADCRetransmission[20], &uiIPAim, 4);
	// 目标端口号
	memcpy(&m_ucADCRetransmission[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_ucADCRetransmission[26], &usCommand, 2);

	int iPos = 32;

	memmove(m_ucADCRetransmission + iPos, &usDataPointNb, 2);
	iPos = iPos + 2;
	m_ucADCRetransmission[iPos] = 0x00;

	int icount = SendTo(m_ucADCRetransmission, 128, SendPort1,m_pTDoc->m_strReceiveIP);
}
// 队列中的每个结构体的帧计数加一
void CMySocket2::OnListCountAdd(void)
{
	POSITION pos = m_oADCLostMap.GetStartPosition();	// 得到索引表起始位置
	unsigned int uiKey;									// 索引键	

	while(NULL != pos)
	{
		m_structADC ADCStructTemp;	
		m_oADCLostMap.GetNextAssoc(pos, uiKey, ADCStructTemp);	// 得到仪器对象
		ADCStructTemp.uiFrameCount++;
		if (ADCStructTemp.uiFrameCount == ClearListFrameCount)
		{
			m_oADCLostMap.RemoveKey(uiKey);
		} 
		else
		{
			m_oADCLostMap.SetAt(uiKey, ADCStructTemp);
		}
	}
}

// 发送ADC数据重发帧
void CMySocket2::SendADCDataRetransmissionFrame(void)
{
	// 设置重发帧

	POSITION pos = m_oADCSendMap.GetStartPosition();	// 得到索引表起始位置
	unsigned int uiKey;					// 索引键	

	if (NULL != pos)
	{
		m_structADC ADCStructTemp;	
		m_oADCSendMap.GetNextAssoc(pos, uiKey, ADCStructTemp);	// 得到仪器对象

		for (int i=0; i<SendADCFrameCountMax; i++)
		{
			MakeADCDataRetransmissionFrame(ADCStructTemp.uiIPAim, uiKey);
			ADCStructTemp.uiRetransmissionNum++;
			m_uiSendADCRetransmissionFramNum++;
			if (ADCStructTemp.uiRetransmissionNum == InstrumentForTest)
			{
				m_oADCSendMap.RemoveKey(uiKey);
				return;
			}
		}
		m_oADCSendMap.SetAt(uiKey, ADCStructTemp);	
	}
}