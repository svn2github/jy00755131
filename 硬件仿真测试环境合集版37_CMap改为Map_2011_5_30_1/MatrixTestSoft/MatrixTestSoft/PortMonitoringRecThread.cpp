// PortMonitoringRecThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "PortMonitoringRecThread.h"


// CPortMonitoringRecThread

IMPLEMENT_DYNCREATE(CPortMonitoringRecThread, CWinThread)

CPortMonitoringRecThread::CPortMonitoringRecThread()
: m_usudp_count(0)
, m_iRecPort(0)
, m_iSendPort(0)
, m_csIP(_T(""))
, m_pSaveFile(NULL)
, m_uiHeartBeatNum(0)
, m_uiIPSetNum(0)
, m_uiTailTimeNum(0)
, m_uiDelayTimeSetNum(0)
, m_uiADCSetNum(0)
, m_uiSendFrameNum(0)
, m_bclose(false)
, m_uiUdpCount(0)
, m_pLogFile(NULL)
{
}

CPortMonitoringRecThread::~CPortMonitoringRecThread()
{
	if (m_pSaveFile != NULL)
	{
		m_pSaveFile = NULL;
		delete m_pSaveFile;
	}
	if (m_pLogFile != NULL)
	{
		m_pLogFile = NULL;
		delete m_pLogFile;
	}
}

BOOL CPortMonitoringRecThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPortMonitoringRecThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPortMonitoringRecThread, CWinThread)
END_MESSAGE_MAP()


// CPortMonitoringRecThread 消息处理程序

int CPortMonitoringRecThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类

	DWORD dwFrameCount = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
		// 得到网络接收缓冲区数据字节数
		// 可以解决粘包现象
		while(m_RecSocket.IOCtl(FIONREAD, &dwFrameCount))
		{
			if(dwFrameCount > 0) 
			{
				icount = m_RecSocket.Receive(m_ucUdpBuf, PortMonitoringBufSize);
				OnProcess(icount);
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
		Sleep(PortMonitoringRecThreadSleepTime);
	}

	::SetEvent(m_hPortMonitoringRecThreadClose);	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return CWinThread::Run();
}
// 处理数据
//************************************
// Method:    OnProcess
// FullName:  CPortMonitoringRecThread::OnProcess
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int iCount
//************************************
void CPortMonitoringRecThread::OnProcess(int iCount)
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
			if (m_uiUdpCount >=  SndFrameSize)
			{
				m_uiUdpCount = 0;
				m_usudp_count += 1;
				m_usudp_count = m_usudp_count % RcvBufNum;
				break;
			}
			m_ucudp_buf[m_usudp_count][m_uiUdpCount] = m_ucUdpBuf[i];
			m_uiUdpCount++;
			if (m_uiUdpCount == SndFrameSize)
			{
				m_uiUdpCount = 0;

				memcpy(&usCRC16, &m_ucudp_buf[m_usudp_count][SndFrameSize - CRCSize], CRCSize);
				if (usCRC16 != get_crc_16(&m_ucudp_buf[m_usudp_count][FrameHeadSize], SndFrameSize - FrameHeadSize - CRCCheckSize))
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
// FullName:  CPortMonitoringRecThread::OnInit
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringRecThread::OnInit(void)
{
	m_hPortMonitoringRecThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if ((m_hPortMonitoringRecThreadClose == NULL)||(GetLastError() == ERROR_ALREADY_EXISTS))
	{
		AfxMessageBox(_T("创建端口监视发送线程关闭事件出错！"));
	}
	else
	{
		::ResetEvent(m_hPortMonitoringRecThreadClose);	// 设置事件对象为无信号状态
	}
}

// 打开
//************************************
// Method:    OnOpen
// FullName:  CPortMonitoringRecThread::OnOpen
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringRecThread::OnOpen(void)
{
	CString str = _T("");
	m_RecSocket.ShutDown(2);
	m_RecSocket.Close();
	BOOL bReturn =  m_RecSocket.Create(m_iRecPort,SOCK_DGRAM);
	if (bReturn == FALSE)
	{
		str = _T("端口监视程序的接收端口创建失败！");
		AfxMessageBox(str);
		m_pLogFile->OnWriteLogFile(_T("CPortMonitoringRecThread::OnOpen"), str, ErrorStatus);
	}
	else
	{
		bReturn = m_RecSocket.SetSockOpt(SO_SNDBUF,&m_RecSocket,PortMonitoringBufSize,SOL_SOCKET);
		if (bReturn == FALSE)
		{
			str = _T("端口监视程序的接收端口发送缓冲区设置失败！");
			AfxMessageBox(str);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringRecThread::OnOpen"), str, ErrorStatus);
		}
		bReturn = m_RecSocket.SetSockOpt(SO_RCVBUF,&m_RecSocket,PortMonitoringBufSize,SOL_SOCKET);
		if (bReturn == FALSE)
		{
			str = _T("端口监视程序的接收端口接收缓冲区设置失败！");
			AfxMessageBox(str);
			m_pLogFile->OnWriteLogFile(_T("CPortMonitoringRecThread::OnOpen"), str, ErrorStatus);
		}
		// 避免端口阻塞
		OnAvoidIOBlock(m_RecSocket);
	}
}
// 避免端口阻塞
//************************************
// Method:    OnAvoidIOBlock
// FullName:  CPortMonitoringRecThread::OnAvoidIOBlock
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: SOCKET socket
//************************************
void CPortMonitoringRecThread::OnAvoidIOBlock(SOCKET socket)
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
// FullName:  CPortMonitoringRecThread::OnStop
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringRecThread::OnStop(void)
{
	m_RecSocket.ShutDown(2);
	m_RecSocket.Close();
}

// 关闭
//************************************
// Method:    OnClose
// FullName:  CPortMonitoringRecThread::OnClose
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringRecThread::OnClose(void)
{
	m_RecSocket.ShutDown(2);
	m_RecSocket.Close();
	m_bclose = true;
}
// 重置界面
//************************************
// Method:    OnReset
// FullName:  CPortMonitoringRecThread::OnReset
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringRecThread::OnReset(void)
{
	m_uiHeartBeatNum = 0;
	m_uiIPSetNum = 0;
	m_uiTailTimeNum = 0;
	m_uiDelayTimeSetNum = 0;
	m_uiADCSetNum = 0;
	m_uiSendFrameNum = 0;
}
// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CPortMonitoringRecThread::ProcessMessages
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CPortMonitoringRecThread::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}

// 端口监视处理函数
void CPortMonitoringRecThread::OnPortMonitoringProc(void)
{
	CString strTemp = _T("");
	unsigned int uiCommand = 0;
	int iPos = 0;
	unsigned short uiPort = 0;
	// 注释掉下面的输出信息后在ADC参数设置和零漂过程中心跳就不会出现停顿现象
	// 通过端口识别功能
	iPos = 24;
	memcpy(&uiPort, &m_ucudp_buf[m_usudp_count][iPos], FramePacketSize2B);
	if (uiPort == HeartBeatRec)
	{
		m_uiHeartBeatNum ++;
	}
	else if (uiPort == IPSetPort)
	{
		m_uiIPSetNum ++;
	}
	else if (uiPort == TailTimeFramePort)
	{
		m_uiTailTimeNum ++;
	}
	else if (uiPort == TimeSetPort)
	{
		m_uiDelayTimeSetNum ++;
	}
	else if (uiPort == ADSetReturnPort)
	{
		m_uiADCSetNum ++;
	}
	m_uiSendFrameNum ++;

	m_RecSocket.SendTo(m_ucudp_buf[m_usudp_count],SndFrameSize,m_iSendPort,m_csIP);
	m_pSaveFile->OnSaveReceiveData(m_ucudp_buf[m_usudp_count],SndFrameSize);
}