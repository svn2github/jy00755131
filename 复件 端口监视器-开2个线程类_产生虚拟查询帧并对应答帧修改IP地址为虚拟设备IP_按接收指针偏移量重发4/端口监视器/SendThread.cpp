// SendThread.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "SendThread.h"
#include "Parameter.h"

// CSendThread

IMPLEMENT_DYNCREATE(CSendThread, CWinThread)

CSendThread::CSendThread()
: m_uiADCFrameCount(0)
, m_uiSendADCRetransmissionFramNum(0)
, m_uiUdpCount(0)
, m_uiRcvByte(0)
{
	
}

CSendThread::~CSendThread()
{

}

BOOL CSendThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CSendThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendThread, CWinThread)
END_MESSAGE_MAP()


// 初始化

void CSendThread::Init()
{
	udp_count = 0;
	m_close = false;
	m_oADCLostMap.RemoveAll();
	m_oADCSendMap.RemoveAll();
// 	int flag =  m_hSocket2.Create(RcvPort2,SOCK_DGRAM);
// 	m_hSocket2.SetSockOpt(SO_RCVBUF,&m_hSocket2,RcvBufSize,SOL_SOCKET);
// 	m_hSocket2.SetSockOpt(SO_SNDBUF,&m_hSocket2,SndBufSize,SOL_SOCKET);
	int            nValidSocket;
	struct sockaddr_in HostIP;
	HostIP.sin_family = AF_INET;
	HostIP.sin_addr.S_un.S_addr = inet_addr("192.168.0.11");
	HostIP.sin_port = htons(RcvPort2);
	m_hSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(SOCKET_ERROR == bind(m_hSocket,(struct sockaddr*)&HostIP,sizeof(HostIP)))
	{
		AfxMessageBox("端口创建失败1！");
	}
	// 接收缓冲区大小
	nValidSocket = RcvBufSize;	
	if (setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nValidSocket, sizeof(nValidSocket)) == SOCKET_ERROR) 
	{
		closesocket(m_hSocket);
		AfxMessageBox("端口创建失败2！");
	}
	// 作为临时变量，发送缓冲区大小
	nValidSocket = SndBufSize;	
	if (setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nValidSocket, sizeof(nValidSocket)) == SOCKET_ERROR) 
	{
		closesocket(m_hSocket);
		AfxMessageBox("端口创建失败3！");
	}
}
// 线程函数在Run中执行
int CSendThread::Run()
{
	unsigned short	usPort = 0;
	unsigned int	uiIPAim = 0;			// 目标IP地址
	unsigned short	usDataPointNow = 0;		// 指针偏移量
	unsigned short	usCommand = 0;			// 命令字
	DWORD dwFrameCount = 0;
	DWORD dwTmp = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if (m_close == true)
		{
			break;
		}

		DWORD dwFrameCount = 0;

		// 得到网络接收缓冲区数据字节数
//		m_hSocket2.IOCtl(FIONREAD, &dwFrameCount);
		icount = recvfrom(m_hSocket,(char *)m_ucUdpBuf,163840,0,NULL,NULL);
//		if(dwFrameCount > 0) 
		if (icount > 0)
		{
			m_uiRcvByte += icount;
//			icount = m_hSocket2.Receive(m_ucUdpBuf,16384);
		
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
							if (pDlg->m_bStartSample == TRUE)
							{
								// 对ADC接收数据设置数据重发
								OnADCDataRetransmission(uiIPAim, usDataPointNow);
							}
						}

						sockaddr_in MainCrsAddr;
						MainCrsAddr.sin_family = AF_INET;
						MainCrsAddr.sin_port = htons(usPort);
						MainCrsAddr.sin_addr.s_addr = inet_addr(pDlg->m_strSendIP);
						int icount = sendto(m_hSocket,(char*)udp_buf[udp_count],256,0,(sockaddr*)&MainCrsAddr,sizeof(sockaddr_in));	
			/*			m_hSocket2.SendTo(udp_buf[udp_count],256,usPort,pDlg->m_strSendIP);*/
						pDlg->OnSaveSendData(udp_buf[udp_count],256);
						udp_count +=1;
						udp_count = udp_count%8;
						if (pDlg->m_bStartSample == TRUE)
						{
							SendADCDataRetransmissionFrame();
						}
					}
					break;
				}
			}
		}
	/*	SendADCDataRetransmissionFrame();*/
		if (m_close == true)
		{
			break;
		}
		Sleep(1);
	}
	
	// 返回
	return CWinThread::Run();
}
void CSendThread::OnClose()
{
// 	m_hSocket2.ShutDown(2);
// 	m_hSocket2.Close();
	closesocket(m_hSocket);
	m_oADCLostMap.RemoveAll();
	m_oADCSendMap.RemoveAll();
	m_close = true;
}
// 设置ADC数据重发
void CSendThread::OnADCDataRetransmission(unsigned int uiIPAim, unsigned short usDataPointNb)
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
void CSendThread::MakeADCDataRetransmissionFrame(unsigned int uiIPAim, unsigned short usDataPointNb)
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


	sockaddr_in MainCrsAddr;
	MainCrsAddr.sin_family = AF_INET;
	MainCrsAddr.sin_port = htons(SendPort1);
	MainCrsAddr.sin_addr.s_addr = inet_addr(pDlg->m_strReceiveIP);
	int icount = sendto(m_hSocket,(char*)m_ucADCRetransmission,128,0,(sockaddr*)&MainCrsAddr,sizeof(sockaddr_in));	
//	int icount = m_hSocket2.SendTo(m_ucADCRetransmission, 128, SendPort1, pDlg->m_strReceiveIP);
}
// 队列中的每个结构体的帧计数加一
void CSendThread::OnListCountAdd(void)
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
void CSendThread::SendADCDataRetransmissionFrame(void)
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
