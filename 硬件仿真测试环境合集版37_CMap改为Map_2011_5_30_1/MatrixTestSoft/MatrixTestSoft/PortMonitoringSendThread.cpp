// PortMonitoringSendThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "PortMonitoringSendThread.h"

// CPortMonitoringSendThread

IMPLEMENT_DYNCREATE(CPortMonitoringSendThread, CWinThread)

CPortMonitoringSendThread::CPortMonitoringSendThread()
: m_usudp_count(0)
, m_iRecPort(0)
, m_iSendPort(0)
, m_csIP(_T(""))
, m_uiHeadFrameNum(0)
, m_uiIPSetReturnNum(0)
, m_uiTailFrameNum(0)
, m_uiTailTimeReturnNum(0)
, m_uiDelayTimeReturnNum(0)
, m_uiADCSetReturnNum(0)
, m_uiErrorCodeReturnNum(0)
, m_uiCollectSysTimeReturnNum(0)
, m_uiADCRecNum(0)
, m_csHeadFrameShow(_T(""))
, m_csIPSetReturnShow(_T(""))
, m_csTailFrameShow(_T(""))
, m_csTailTimeReturnShow(_T(""))
, m_csDelayTimeReturnShow(_T(""))
, m_csErrorCodeReturnShow(_T(""))
, m_uiRecFrameNum(0)
, m_pSaveFile(NULL)
, m_bPortDistribution(FALSE)
, m_bclose(false)
, m_uiUdpCount(0)
, m_pLogFile(NULL)
{
}

CPortMonitoringSendThread::~CPortMonitoringSendThread()
{
}

BOOL CPortMonitoringSendThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPortMonitoringSendThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPortMonitoringSendThread, CWinThread)
END_MESSAGE_MAP()


// CPortMonitoringSendThread 消息处理程序

int CPortMonitoringSendThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类
	DWORD dwFrameCount = 0;
	int icount = 0;
	int n = sizeof(addr);
	// 循环，直到关闭标志为真
	while(true)
	{
		if (m_bclose == true)
		{
			break;
		}
		// 得到网络接收缓冲区数据字节数
		// 可以解决粘包现象
		while(SOCKET_ERROR != ioctlsocket(m_SendSocket, FIONREAD, &dwFrameCount))
		{
			if(dwFrameCount > 0) 
			{
				icount = recvfrom(m_SendSocket, (char*)&m_ucUdpBuf, sizeof(m_ucUdpBuf), 0, (sockaddr*)&addr, &n);
				if (icount != SOCKET_ERROR)
				{
					OnProcess(icount);
				}
				else
				{
					int iError = 0;
					CString str = _T("");
					iError = WSAGetLastError();
					str.Format(_T("端口监视发送的接收帧错误，错误号为%d！"), iError);
					m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::Run"), str, ErrorStatus);
				}
			}
			else
			{
				break;
			}
		}

		if (m_bclose == true)
		{
			break;
		}
		Sleep(PortMonitoringSendThreadSleepTime);
	}
	::SetEvent(m_hPortMonitoringSendThreadClose);	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return CWinThread::Run();
}
// 数据处理
//************************************
// Method:    OnProcess
// FullName:  CPortMonitoringSendThread::OnProcess
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int iCount
//************************************
void CPortMonitoringSendThread::OnProcess(int iCount)
{
	unsigned short usCRC16 = 0;
	for (int i=0; i<iCount; i++)
	{
//		ProcessMessages();
		switch(m_uiUdpCount)
		{
		case 0:
			if (m_ucUdpBuf[i] == FrameHeadCheck0)
			{
				m_ucudp_buf[m_usudp_count][0] = m_ucUdpBuf[i];
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
			if (m_ucUdpBuf[i] == FrameHeadCheck1)
			{
				m_ucudp_buf[m_usudp_count][1] = m_ucUdpBuf[i];
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
			if (m_ucUdpBuf[i] == FrameHeadCheck2)
			{
				m_ucudp_buf[m_usudp_count][2] = m_ucUdpBuf[i];
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
			if (m_ucUdpBuf[i] == FrameHeadCheck3)
			{
				m_ucudp_buf[m_usudp_count][3] = m_ucUdpBuf[i];
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
			if (m_uiUdpCount >=  RcvFrameSize)
			{
				m_uiUdpCount = 0;
				m_usudp_count += 1;
				m_usudp_count = m_usudp_count % RcvBufNum;
				break;
			}
			m_ucudp_buf[m_usudp_count][m_uiUdpCount] = m_ucUdpBuf[i];
			m_uiUdpCount++;
			if (m_uiUdpCount == RcvFrameSize)
			{
				m_uiUdpCount = 0;

				memcpy(&usCRC16, &m_ucudp_buf[m_usudp_count][RcvFrameSize - CRCSize], CRCSize);
				if (usCRC16 != get_crc_16(&m_ucudp_buf[m_usudp_count][FrameHeadSize], RcvFrameSize - FrameHeadSize - CRCCheckSize))
				{
					//	continue;
				}
				OnPortMonitoringProc();
				m_usudp_count += 1;
				m_usudp_count = m_usudp_count % RcvBufNum;
			}
			break;
		}
	}
}
// 初始化
//************************************
// Method:    OnInit
// FullName:  CPortMonitoringSendThread::OnInit
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::OnInit(void)
{
	m_hPortMonitoringSendThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if ((m_hPortMonitoringSendThreadClose == NULL)||(GetLastError() == ERROR_ALREADY_EXISTS))
	{
		AfxMessageBox(_T("创建端口监视发送线程关闭事件出错！"));
	}
	else
	{
		::ResetEvent(m_hPortMonitoringSendThreadClose);	// 设置事件对象为无信号状态
	}
	for (int i=0; i<InstrumentNum; i++)
	{
		m_uiErrorCodeReturnCount[i] = 0;
		memset(&m_ucErrorCodeReturn[i], SndFrameBufInit, QueryErrorCodeNum);
	}
}

// 打开
//************************************
// Method:    OnOpen
// FullName:  CPortMonitoringSendThread::OnOpen
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::OnOpen(void)
{
	CString str = _T("");
	m_SendSocket = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;											// 填充套接字地址结构
	addr.sin_port = htons(m_iRecPort);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	int iReturn = bind(m_SendSocket, (sockaddr*)&addr, sizeof(addr));	// 绑定本地地址
	listen(m_SendSocket, 2);
	if (iReturn == SOCKET_ERROR)
	{
		str = _T("端口监视程序的发送端口创建失败！");
		AfxMessageBox(str);
		m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnOpen"), str, ErrorStatus);
	}
	else
	{
		int nSendBuf = PortMonitoringBufSize;
		iReturn = setsockopt(m_SendSocket, SOL_SOCKET, SO_SNDBUF,  ( const char* )&nSendBuf, sizeof(int));
		if (iReturn == SOCKET_ERROR)
		{
			str = _T("端口监视程序的接收端口发送缓冲区设置失败！");
			AfxMessageBox(str);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnOpen"), str, ErrorStatus);
		}
		int nRecvBuf = PortMonitoringBufSize;
		iReturn = setsockopt(m_SendSocket, SOL_SOCKET, SO_RCVBUF,  ( const char* )&nRecvBuf, sizeof(int));
		if (iReturn == SOCKET_ERROR)
		{
			str = _T("端口监视程序的发送端口接收缓冲区设置失败！");
			AfxMessageBox(str);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnOpen"), str, ErrorStatus);
		}
		// 避免端口阻塞
		OnAvoidIOBlock(m_SendSocket);
	}
}
// 避免端口阻塞
//************************************
// Method:    OnAvoidIOBlock
// FullName:  CPortMonitoringSendThread::OnAvoidIOBlock
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: SOCKET socket
//************************************
void CPortMonitoringSendThread::OnAvoidIOBlock(SOCKET socket)
{
	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	DWORD status = 0;
	status = WSAIoctl(socket, SIO_UDP_CONNRESET ,
		&bNewBehavior, sizeof(bNewBehavior),
		NULL, 0, &dwBytesReturned,
		NULL, NULL);
}
// 停止
//************************************
// Method:    OnStop
// FullName:  CPortMonitoringSendThread::OnStop
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::OnStop(void)
{
	OnCloseUDP();
	// 硬件设备错误查询应答帧个数
	m_uiErrorCodeReturnNum = 0;
	for (int i=0; i<InstrumentNum; i++)
	{
		m_uiErrorCodeReturnCount[i] = 0;
		memset(&m_ucErrorCodeReturn[i], SndFrameBufInit, QueryErrorCodeNum);
	}
}

// 关闭
//************************************
// Method:    OnClose
// FullName:  CPortMonitoringSendThread::OnClose
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::OnClose(void)
{
	OnCloseUDP();
	m_bclose = true;
}
// 重置界面
//************************************
// Method:    OnReset
// FullName:  CPortMonitoringSendThread::OnReset
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::OnReset(void)
{
	// 首包计数
	m_uiHeadFrameNum = 0;
	// IP地址设置计数
	m_uiIPSetReturnNum = 0;
	// 尾包计数
	m_uiTailFrameNum = 0;
	// 尾包时刻查询应答计数
	m_uiTailTimeReturnNum = 0;
	// 时延设置应答计数
	m_uiDelayTimeReturnNum = 0;
	// ADC设置应答计数
	m_uiADCSetReturnNum = 0;
	// 硬件设备错误查询应答帧个数
	m_uiErrorCodeReturnNum = 0;
	// 查询得到的本地时间帧数
	m_uiCollectSysTimeReturnNum = 0;
	// 接收得到的ADC数据帧数
	m_uiADCRecNum = 0;
	// 显示首包帧，包含SN和首包时刻
	m_csHeadFrameShow.Empty();
	// 显示IP地址设置应答帧，包含SN和设置的IP地址
	m_csIPSetReturnShow.Empty();
	// 显示尾包帧，包含SN
	m_csTailFrameShow.Empty();
	// 显示尾包时刻查询应答帧，包含IP地址和查询结果
	m_csTailTimeReturnShow.Empty();
	// 显示时延设置应答帧，包含IP地址和设置内容
	m_csDelayTimeReturnShow.Empty();
	// 显示硬件错误码和码差
	m_csErrorCodeReturnShow.Empty();
	// 接收帧数
	m_uiRecFrameNum = 0;
}
// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CPortMonitoringSendThread::ProcessMessages
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}

// 端口监视处理函数
//************************************
// Method:    OnPortMonitoringProc
// FullName:  CPortMonitoringSendThread::OnPortMonitoringProc
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringSendThread::OnPortMonitoringProc(void)
{
	unsigned short uiPort = 0;
	CString strTemp = _T("");
	unsigned int uiTemp = 0;
	unsigned int uiCommand = 0;
	int iPos = 0;
	// 加入端口分发功能
	iPos = 24;
	memcpy(&uiPort, &m_ucudp_buf[m_usudp_count][iPos], FramePacketSize2B);
	if (uiPort == HeadFramePort)
	{
		iPos = 28;
		memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][iPos], FrameCmdSize1B);
		if (uiCommand == CmdSn)
		{
			iPos += FrameCmdSize1B;
			// 显示SN
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csHeadFrameShow += strTemp;
			}
			m_csHeadFrameShow += _T("\t");
			// 显示首包时刻
			iPos = 34;
			for (int k=0; k<FramePacketSize4B; k++)
			{
				strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csHeadFrameShow += strTemp;
			}
			m_csHeadFrameShow += _T("\r\n");
			m_uiHeadFrameNum++;
		}
		else
		{
			strTemp.Format(_T("首包端口正确，命令字错误，错误的命令字为%d"), uiCommand);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
		}
	}
	else if (uiPort == IPSetPort)
	{
		iPos = 28;
		memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][iPos], FrameCmdSize1B);
		if (uiCommand == CmdSn)
		{
			iPos += FrameCmdSize1B;
			// 显示SN
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csIPSetReturnShow += strTemp;
			}
			m_csIPSetReturnShow += _T("\t");
			// 显示IP
			iPos = 34;
			for (int k=0; k<FramePacketSize4B; k++)
			{
				strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csIPSetReturnShow += strTemp;
			}
			m_csIPSetReturnShow += _T("\r\n");
			m_uiIPSetReturnNum++;
		}
		else
		{
			strTemp.Format(_T("IP地址设置应答端口正确，命令字错误，错误的命令字为%d"), uiCommand);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
		}
	}
	else if (uiPort == TailFramePort)
	{
		iPos = 33;
		memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][iPos], FrameCmdSize1B);
		if (uiCommand == CmdSn)
		{
			iPos += FrameCmdSize1B;
			// 显示SN
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csTailFrameShow += strTemp;
			}
			m_csTailFrameShow += _T("\t");

			// 显示IP
			iPos = 29;
			for (int k=0; k<FramePacketSize4B; k++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csTailFrameShow += strTemp;
			}
			m_csTailFrameShow += _T("\r\n");
			m_uiTailFrameNum++;
		}
		else
		{
			strTemp.Format(_T("尾包端口正确，命令字错误，错误的命令字为%d"), uiCommand);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
		}
	}
	else if (uiPort == TailTimeFramePort)
	{
		iPos = 28;
		memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][iPos], FrameCmdSize1B);
		if (uiCommand == CmdSn)
		{
			iPos += FrameCmdSize1B;
			// 显示SN
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csTailTimeReturnShow += strTemp;
			}
			m_csTailTimeReturnShow += _T(" ");
			// 显示网络时间
			iPos = 34;
			for (int k=0; k<FramePacketSize4B; k++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csTailTimeReturnShow += strTemp;
			}
			m_csTailTimeReturnShow += _T(" ");
			// 显示本地时间
			iPos = 39;
			for (int p=0; p<FramePacketSize4B; p++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csTailTimeReturnShow += strTemp;
			}
			m_csTailTimeReturnShow += _T("\r\n");
			m_uiTailTimeReturnNum++;
		}
		else
		{
			strTemp.Format(_T("尾包时刻查询应答端口正确，命令字错误，错误的命令字为%d"), uiCommand);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
		}
	}
	else if (uiPort == TimeSetPort)
	{
		iPos = 28;
		memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][iPos], FrameCmdSize1B);
		if (uiCommand == CmdLocalFixedTimeHigh)
		{
			// 显示IP
			iPos = 16;
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csDelayTimeReturnShow += strTemp;
			}
			m_csDelayTimeReturnShow += _T(" ");

			// 显示延时修正高位
			iPos = 29;
			for (int k=0; k<FramePacketSize4B; k++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csDelayTimeReturnShow += strTemp;
			}
			m_csDelayTimeReturnShow += _T(" ");

			// 显示延时修正低位
			iPos = 34;
			for (int p=0; p<FramePacketSize4B; p++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csDelayTimeReturnShow += strTemp;
			}
			m_csDelayTimeReturnShow += _T("\r\n");
			m_uiDelayTimeReturnNum++;
		}
		else
		{
			strTemp.Format(_T("时统设置应答端口正确，命令字错误，错误的命令字为%d"), uiCommand);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
		}
	}
	else if (uiPort == ADSetReturnPort)
	{
		// 每次采集只有一个ADC设置应答帧
		m_uiADCSetReturnNum++;
	}
	else if (uiPort == QueryErrorCodePort)
	{
		unsigned int uiIP = 0;
		unsigned int uiInstrumentNb = 0;
		// 显示IP
		iPos = 16;
		for (int j=0; j<FramePacketSize4B; j++)
		{
			strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
			iPos++;
			m_csErrorCodeReturnShow += strTemp;
		}
		iPos = 16;
		memcpy(&uiIP, &m_ucudp_buf[m_usudp_count][iPos], FramePacketSize4B);
		for (int k=0; k<InstrumentMaxCount; k++)
		{
			if (uiIP == (IPSetAddrStart + k * IPSetAddrInterval))
			{
				uiInstrumentNb = k;
				m_uiErrorCodeReturnCount[uiInstrumentNb]++;
				break;
			}
		}
		iPos = 33;
		memcpy(&uiCommand, &m_ucudp_buf[m_usudp_count][iPos], FrameCmdSize1B);
		if (uiCommand == CmdFDUErrorCode)
		{
			iPos += FrameCmdSize1B;
			m_csErrorCodeReturnShow += _T("\t");
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				m_csErrorCodeReturnShow += strTemp;
			}
			m_csErrorCodeReturnShow += _T("\t");
			if (m_uiErrorCodeReturnCount[uiInstrumentNb] == 1)
			{
				iPos = 34;
				for (int j=0; j<FramePacketSize4B; j++)
				{
					m_ucErrorCodeReturn[uiInstrumentNb][j] = m_ucudp_buf[m_usudp_count][iPos];
					strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
					iPos++;
					m_csErrorCodeReturnShow += strTemp;
				}
			}
			else
			{
				iPos = 34;
				for (int j=0; j<FramePacketSize4B; j++)
				{
					if (m_ucudp_buf[m_usudp_count][iPos] < m_ucErrorCodeReturn[uiInstrumentNb][j])
					{
						uiTemp = 256 + m_ucudp_buf[m_usudp_count][iPos] - m_ucErrorCodeReturn[uiInstrumentNb][j];
					}
					else
					{
						uiTemp = m_ucudp_buf[m_usudp_count][iPos] - m_ucErrorCodeReturn[uiInstrumentNb][j] ;
					}
					m_ucErrorCodeReturn[uiInstrumentNb][j] = m_ucudp_buf[m_usudp_count][iPos];
					strTemp.Format(_T("%02x "), uiTemp);
					iPos++;
					m_csErrorCodeReturnShow += strTemp;
				}
			}
			m_csErrorCodeReturnShow += _T("\r\n");
		}
		else if (uiCommand == CmdLAUXErrorCode1)
		{
			m_csErrorCodeReturnShow += _T("\t");
			iPos = 34;
			for (int j=0; j<FramePacketSize4B; j++)
			{
				strTemp.Format(_T("%02x "),m_ucudp_buf[m_usudp_count][iPos]);
				iPos++;
				if (iPos == 36)
				{
					iPos = 39;
				}
				m_csErrorCodeReturnShow += strTemp;
			}
			m_csErrorCodeReturnShow += _T("\t");
			if (m_uiErrorCodeReturnCount[uiInstrumentNb] == 1)
			{
				iPos = 34;
				for (int j=0; j<FramePacketSize4B; j++)
				{
					m_ucErrorCodeReturn[uiInstrumentNb][j] = m_ucudp_buf[m_usudp_count][iPos];
					strTemp.Format(_T("%02x "), m_ucudp_buf[m_usudp_count][iPos]);
					iPos++;
					if (iPos == 36)
					{
						iPos = 39;
					}
					m_csErrorCodeReturnShow += strTemp;
				}
			}
			else
			{
				iPos = 34;
				for (int j=0; j<FramePacketSize4B; j++)
				{
					if (m_ucudp_buf[m_usudp_count][iPos] < m_ucErrorCodeReturn[uiInstrumentNb][j])
					{
						uiTemp = 256 + m_ucudp_buf[m_usudp_count][iPos] - m_ucErrorCodeReturn[uiInstrumentNb][j];
					}
					else
					{
						uiTemp = m_ucudp_buf[m_usudp_count][iPos] - m_ucErrorCodeReturn[uiInstrumentNb][j] ;
					}
					m_ucErrorCodeReturn[uiInstrumentNb][j] = m_ucudp_buf[m_usudp_count][iPos];
					strTemp.Format(_T("%02x "), uiTemp);
					iPos++;
					if (iPos == 36)
					{
						iPos = 39;
					}
					m_csErrorCodeReturnShow += strTemp;
				}
			}
			m_csErrorCodeReturnShow += _T("\r\n");
		}
		else
		{
			strTemp.Format(_T("误码查询应答端口正确，命令字错误，错误的命令字为%d"), uiCommand);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
		}
		m_uiErrorCodeReturnNum++;
	}
	else if (uiPort == CollectSysTimePort)
	{
		m_uiCollectSysTimeReturnNum++;
	}
	else if (uiPort == ADRecPort)
	{
		m_uiADCRecNum++;
	}
	else
	{
		strTemp.Format(_T("错误的端口号%d"), uiPort);
		m_pLogFile->OnWriteLogFile(_T("CPortMonitoringSendThread::OnPortMonitoringProc"), strTemp, ErrorStatus);
	}
	m_uiRecFrameNum++;
	if (m_bPortDistribution == TRUE)
	{
		addr2.sin_family = AF_INET;											// 填充套接字地址结构
		addr2.sin_port = htons(uiPort);
		addr2.sin_addr.S_un.S_addr = inet_addr(ConvertCStringToConstCharPointer(m_csIP));

		// 开启端口分发功能
		sendto(m_SendSocket, (const char*)&m_ucudp_buf[m_usudp_count], RcvFrameSize, 0, (sockaddr*)&addr2, sizeof(addr2));
	}
	else
	{
		addr2.sin_family = AF_INET;											// 填充套接字地址结构
		addr2.sin_port = htons(m_iSendPort);
		addr2.sin_addr.S_un.S_addr = inet_addr(ConvertCStringToConstCharPointer(m_csIP));

		sendto(m_SendSocket, (const char*)&m_ucudp_buf[m_usudp_count], RcvFrameSize, 0, (sockaddr*)&addr2, sizeof(addr2));
	}
	m_pSaveFile->OnSaveSendData(m_ucudp_buf[m_usudp_count],RcvFrameSize);
}
// 关闭UDP套接字
void CPortMonitoringSendThread::OnCloseUDP(void)
{
	shutdown(m_SendSocket, SD_BOTH);
	closesocket(m_SendSocket);
	m_SendSocket = INVALID_SOCKET;
}
