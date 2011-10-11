#include "StdAfx.h"
#include "ThreadManage.h"
CThreadManage::CThreadManage(void)
: m_pLogFile(NULL)
{
}

CThreadManage::~CThreadManage(void)
{
}

// 初始化
//************************************
// Method:    OnInit
// FullName:  CThreadManage::OnInit
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnInit(void)
{
	m_oInstrumentList.OnInit();
	m_oHeadFrame.m_pInstrumentList = &m_oInstrumentList;
	m_oIPSet.m_pInstrumentList = &m_oInstrumentList;
	m_oTailFrame.m_pInstrumentList = &m_oInstrumentList;
	m_oTailTimeFrame.m_pInstrumentList = &m_oInstrumentList;
	m_oADCSet.m_pInstrumentList = &m_oInstrumentList;
	m_oSysTime.m_pInstrumentList = &m_oInstrumentList;
	m_oADCDataRecThread.m_pInstrumentList = &m_oInstrumentList;
	m_oADCDataSaveToFile.m_pInstrumentList = &m_oInstrumentList;
	m_oInstrumentMonitorThread.m_pInstrumentList = &m_oInstrumentList;
	m_oInstrumentMonitorThread.m_pTailFrame = &m_oTailFrame;
	m_oSysTime.m_pADCSet = &m_oADCSet;
	m_oInstrumentMonitorThread.m_pADCSet = &m_oADCSet;
	m_oADCDataRecThread.m_pADCDataSaveToFile = & m_oADCDataSaveToFile;
	m_oADCDataRecThread.m_pADCFrameInfo = &m_oADCFrameInfo;
	m_oADCSet.m_pADCDataRecThread = &m_oADCDataRecThread;
	m_oSysTime.m_pADCFrameInfo = &m_oADCFrameInfo;

	m_oADCSet.m_pLogFile = m_pLogFile;
	m_oHeadFrame.m_pLogFile = m_pLogFile;
	m_oTailTimeFrame.m_pLogFile = m_pLogFile;
	m_oTailFrame.m_pLogFile = m_pLogFile;
	m_oInstrumentList.m_pLogFile = m_pLogFile;
	m_oSysTime.m_pLogFile = m_pLogFile;
	m_oADCDataRecThread.m_pLogFile = m_pLogFile;
	m_oInstrumentMonitorThread.m_pLogFile = m_pLogFile;
	m_oIPSet.m_pLogFile = m_pLogFile;

	m_oHeartBeatThread.OnInit();
	// 创建线程并挂起
	m_oHeartBeatThread.CreateThread(CREATE_SUSPENDED, 0, 0);
	m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnInit"), _T("心跳发送线程创建成功！"), SuccessStatus);

	m_oADCDataRecThread.OnInit();
	// 创建线程并挂起
	m_oADCDataRecThread.CreateThread(CREATE_SUSPENDED, 0, 0);
	m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnInit"), _T("ADC数据接收线程创建成功！"), SuccessStatus);

	m_oInstrumentMonitorThread.OnInit();
	// 创建线程并挂起
	m_oInstrumentMonitorThread.CreateThread(CREATE_SUSPENDED, 0, 0);
	m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnInit"), _T("设备监视线程创建成功！"), SuccessStatus);
}

// 关闭并结束线程
//************************************
// Method:    OnClose
// FullName:  CThreadManage::OnClose
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnClose(void)
{
	int iResult = 0;
	CString str = _T("");

	m_oHeartBeatThread.ResumeThread();
	m_oADCDataRecThread.ResumeThread();
	m_oInstrumentMonitorThread.ResumeThread();
	m_oHeartBeatThread.OnClose();
	m_oADCDataRecThread.OnClose();
	m_oInstrumentMonitorThread.OnClose();
	iResult = WaitForSingleObject(m_oHeartBeatThread.m_hHeartBeatThreadClose, WaitForThreadCloseTime);
	if (iResult != WAIT_OBJECT_0)
	{
		str.Format(_T("心跳线程在%dms内未能正常结束！"), WaitForThreadCloseTime);
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnClose"), str, ErrorStatus);
		AfxMessageBox(str);
	}
	else
	{
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnClose"), _T("心跳线程正常结束！"), SuccessStatus);
	}
	// 关闭事件句柄
	CloseHandle(m_oHeartBeatThread.m_hHeartBeatThreadClose);

	iResult = WaitForSingleObject(m_oADCDataRecThread.m_hADCDataThreadClose, WaitForThreadCloseTime);
	if (iResult != WAIT_OBJECT_0)
	{
		str.Format(_T("ADC数据接收线程在%dms内未能正常结束！"), WaitForThreadCloseTime);
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnClose"), str, ErrorStatus);
		AfxMessageBox(str);
	}
	else
	{
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnClose"), _T("ADC数据接收线程正常结束！"), SuccessStatus);
	}
	// 关闭事件句柄
	CloseHandle(m_oADCDataRecThread.m_hADCDataThreadClose);

	iResult = WaitForSingleObject(m_oInstrumentMonitorThread.m_hInstrumentMonitorThreadClose, WaitForThreadCloseTime);
	if (iResult != WAIT_OBJECT_0)
	{
		str.Format(_T("设备监视线程在%dms内未能正常结束！"), WaitForThreadCloseTime);
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnClose"), str, ErrorStatus);
		AfxMessageBox(str);
	}
	else
	{
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnClose"), _T("设备监视线程正常结束！"), SuccessStatus);
	}
	// 关闭事件句柄
	CloseHandle(m_oInstrumentMonitorThread.m_hInstrumentMonitorThreadClose);

	OnCloseUDPSocket();
	m_oInstrumentList.OnClose();
}

// 打开
//************************************
// Method:    OnOpen
// FullName:  CThreadManage::OnOpen
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnOpen(void)
{
	m_oInstrumentList.OnOpen();
	OnCreateHeadSocket();
	OnCreateIPSetSocket();
	OnCreateTailSocket();
	OnCreateTailTimeSocket();
	OnCreateGetSysTimeSocket();	
	OnCreateADCSetSocket();
	OnCreateHeartBeatSocket();
	OnCreateADCDataSocket();
	// 发送尾包时刻查询帧计数
	m_uiSendTailTimeFrameCount = 0;
	// 接收到的尾包时刻查询帧计数
	m_uiRecTailTimeFrameCount = 0;	 

	m_oHeartBeatThread.ResumeThread();
	m_oADCDataRecThread.OnReset();
	m_oADCDataRecThread.ResumeThread();
	m_oInstrumentMonitorThread.ResumeThread();
}

// 停止
//************************************
// Method:    OnStop
// FullName:  CThreadManage::OnStop
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnStop(void)
{
	m_oHeartBeatThread.OnCloseUDP();
	m_oHeartBeatThread.SuspendThread();
	m_oADCDataRecThread.OnCloseUDP();
	m_oADCDataRecThread.SuspendThread();
	m_oInstrumentMonitorThread.SuspendThread();
	OnCloseUDPSocket();
	m_oInstrumentList.OnStop();
}
// 避免端口阻塞
//************************************
// Method:    OnAvoidIOBlock
// FullName:  CThreadManage::OnAvoidIOBlock
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: SOCKET socket
//************************************
void CThreadManage::OnAvoidIOBlock(SOCKET socket)
{
	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	DWORD status = 0;
	status = WSAIoctl(socket, SIO_UDP_CONNRESET ,
		&bNewBehavior, sizeof(bNewBehavior),
		NULL, 0, &dwBytesReturned,
		NULL, NULL);
}
// 创建和设置Socket
//************************************
// Method:    OnCreateAndSetSocket
// FullName:  CThreadManage::OnCreateAndSetSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: CSocket * socket
// Parameter: bool bBroadCast
// Parameter: int iSocketPort
// Parameter: CString str
// Parameter: int iRecBuf
// Parameter: int iSendBuf
//************************************
SOCKET CThreadManage::OnCreateAndSetSocket(sockaddr_in addrName, bool bBroadCast, 
										 int iSocketPort, CString str, int iRecBuf, int iSendBuf)
{
	CString strTemp = _T("");
	SOCKET socketName = INVALID_SOCKET;
	socketName = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	addrName.sin_family = AF_INET;											// 填充套接字地址结构
	addrName.sin_port = htons(iSocketPort);
	addrName.sin_addr.S_un.S_addr = INADDR_ANY;
	int iReturn = bind(socketName, reinterpret_cast<sockaddr*>(&addrName), sizeof(addrName));	// 绑定本地地址
	listen(socketName, 2);
	if (iReturn == SOCKET_ERROR)
	{
		strTemp = str + _T("创建失败！");
		AfxMessageBox(strTemp);
		m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnCreateAndSetSocket"), strTemp, ErrorStatus);
	}
	else
	{
		//设置非堵塞通讯
// 		DWORD ul = 1;
// 		ioctlsocket(socketName, FIONBIO, &ul);
		if (bBroadCast == true)
		{
			//设置广播模式
			int iOptlen = sizeof(int);
			int iOptval = 1;
			iReturn = setsockopt(socketName, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char *>(&iOptval), iOptlen);
			if (iReturn == SOCKET_ERROR)
			{
				strTemp = str + _T("设置为广播端口失败！");
				AfxMessageBox(strTemp);
				m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnCreateAndSetSocket"), strTemp, ErrorStatus);
			}
		}

		iReturn = setsockopt(socketName, SOL_SOCKET, SO_SNDBUF,  reinterpret_cast<const char *>(&iSendBuf), sizeof(int));
		if (iReturn == SOCKET_ERROR)
		{
			strTemp = str + _T("发送缓冲区设置失败！");
			AfxMessageBox(strTemp);
			m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnCreateAndSetSocket"), strTemp, ErrorStatus);
		}
		iReturn = setsockopt(socketName, SOL_SOCKET, SO_RCVBUF,  reinterpret_cast<const char *>(&iRecBuf), sizeof(int));
		if (iReturn == SOCKET_ERROR)
		{
			strTemp = str + _T("接收缓冲区设置失败！");
			AfxMessageBox(strTemp);
			m_pLogFile->OnWriteLogFile(_T("CThreadManage::OnCreateAndSetSocket"), strTemp, ErrorStatus);
		}
		// 避免端口阻塞
		OnAvoidIOBlock(socketName);
	}
	return socketName;
}
// 创建查询采集站本地时间的广播端口
//************************************
// Method:    OnCreateCollectSysTimeBroadcastSocket
// FullName:  CThreadManage::OnCreateCollectSysTimeBroadcastSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateGetSysTimeSocket(void)
{
	CString str = _T("");
	str = _T("查询采集站本地时间的端口");
	m_oSysTime.m_SysTimeSocket = OnCreateAndSetSocket(m_oSysTime.m_RecvAddr, false, CollectSysTimePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
}

// 创建首包Socket
//************************************
// Method:    OnCreateHeadSocket
// FullName:  CThreadManage::OnCreateHeadSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateHeadSocket(void)
{
	CString str = _T("");
	str = _T("首包接收端口");
	m_oHeadFrame.m_HeadFrameSocket = OnCreateAndSetSocket(m_oHeadFrame.m_RecvAddr, false, HeadFramePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
}

// 创建IP地址设置Socket
//************************************
// Method:    OnCreateIPSetSocket
// FullName:  CThreadManage::OnCreateIPSetSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateIPSetSocket(void)
{
	CString str = _T("");
	str = _T("IP地址设置端口");
	m_oIPSet.m_IPSetSocket = OnCreateAndSetSocket(m_oIPSet.m_RecvAddr, false, IPSetPort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
}

// 创建尾包Socket
//************************************
// Method:    OnCreateTailSocket
// FullName:  CThreadManage::OnCreateTailSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateTailSocket(void)
{
	CString str = _T("");
	str = _T("尾包接收端口");
	m_oTailFrame.m_TailFrameSocket = OnCreateAndSetSocket(m_oTailFrame.m_RecvAddr, false, TailFramePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
}

// 创建尾包时刻查询和时统Socket
//************************************
// Method:    OnCreateTailTimeSocket
// FullName:  CThreadManage::OnCreateTailTimeSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateTailTimeSocket(void)
{
	CString str = _T("");
	str = _T("尾包时刻查询和时统端口");
	m_oTailTimeFrame.m_TailTimeSocket = OnCreateAndSetSocket(m_oTailTimeFrame.m_RecvAddr, false, TailTimeFramePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
}

// 创建ADC设置Socket
//************************************
// Method:    OnCreateADCSetSocket
// FullName:  CThreadManage::OnCreateADCSetSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateADCSetSocket(void)
{
	CString str = _T("");
	str = _T("ADC设置端口");
	m_oADCSet.m_ADCSetSocket = OnCreateAndSetSocket(m_oADCSet.m_RecvAddr, false, 
		ADSetReturnPort, str, InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
}

// 创建心跳Socket
//************************************
// Method:    OnCreateHeartBeatSocket
// FullName:  CThreadManage::OnCreateHeartBeatSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateHeartBeatSocket(void)
{
	CString str = _T("");
	str = _T("心跳端口");
	m_oHeartBeatThread.m_HeartBeatSocket = OnCreateAndSetSocket(m_oHeartBeatThread.m_RecvAddr, true, 
		HeartBeatRec, str, HeartBeatSndBufSize, HeartBeatSndBufSize);
}

// 创建ADC数据接收Socket
//************************************
// Method:    OnCreateADCDataSocket
// FullName:  CThreadManage::OnCreateADCDataSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::OnCreateADCDataSocket(void)
{
	CString str = _T("");
	str = _T("ADC数据接收端口");
	m_oADCDataRecThread.m_ADCDataSocket = OnCreateAndSetSocket(m_oADCDataRecThread.m_RecvAddr, false, 
		ADRecPort, str, ADCDataBufSize, ADCDataBufSize);
	str = _T("ADC数据图形化显示接收设置端口");
	m_oADCDataRecThread.m_ADCGraphShowSocket = OnCreateAndSetSocket(m_oADCDataRecThread.m_RecvAddr, true, 
		ADCGraphSetRecPort, str, ADCDataBufSize, ADCDataBufSize);
}
// 关闭UDP套接字
void CThreadManage::OnCloseUDPSocket(void)
{
	m_oHeadFrame.OnCloseUDP();
	m_oIPSet.OnCloseUDP();
	m_oTailFrame.OnCloseUDP();
	m_oTailTimeFrame.OnCloseUDP();
	m_oSysTime.OnCloseUDP();
	m_oADCSet.OnCloseUDP();
}