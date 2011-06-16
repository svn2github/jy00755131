#include "StdAfx.h"
#include "ThreadManage.h"

CThreadManage::CThreadManage(void)
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
	m_oHeadFrameSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oIPSetSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oTailFrameSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oTailTimeFrameSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oADCSetSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oSysTimeSocket.m_pADCSet = &m_oADCSetSocket;
	m_oHeartBeatThread.m_pHeartBeatSocket = &m_oHeartBeatSocket;
	m_oADCDataRecThread.m_pADCDataSocket = & m_oADCDataSocket;
	m_oADCDataRecThread.m_pADCDataSaveToFile = & m_oADCDataSaveToFile;
	m_oADCDataRecThread.m_pADCFrameInfo = &m_oADCFrameInfo;
	m_oSysTimeSocket.m_pADCFrameInfo = &m_oADCFrameInfo;
	m_oHeartBeatThread.OnInit();
	m_oHeartBeatThread.CreateThread();
	m_oHeartBeatThread.SuspendThread();
	TRACE(_T("心跳发送线程创建"));
	m_oADCDataRecThread.OnInit();
	m_oADCDataRecThread.CreateThread();
	m_oADCDataRecThread.SuspendThread();
	TRACE(_T("ADC数据接收线程创建"));
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

	m_oHeartBeatThread.OnClose();
	m_oADCDataRecThread.OnClose();
	iResult = WaitForSingleObject(m_oHeartBeatThread.m_hHeartBeatThreadClose, WaitForThreadCloseTime);
	if (iResult != WAIT_OBJECT_0)
	{
		str.Format(_T("心跳线程在%dms内未能正常结束！"), WaitForThreadCloseTime);
		AfxMessageBox(str);
	}
	::CloseHandle(m_oHeartBeatThread.m_hHeartBeatThreadClose);

	iResult = WaitForSingleObject(m_oADCDataRecThread.m_hADCDataThreadClose, WaitForThreadCloseTime);
	if (iResult != WAIT_OBJECT_0)
	{
		str.Format(_T("ADC数据接收线程在%dms内未能正常结束！"), WaitForThreadCloseTime);
		AfxMessageBox(str);
	}
	::CloseHandle(m_oADCDataRecThread.m_hADCDataThreadClose);
	m_oHeadFrameSocket.ShutDown(2);
	m_oHeadFrameSocket.Close();
	m_oIPSetSocket.ShutDown(2);
	m_oIPSetSocket.Close();
	m_oTailFrameSocket.ShutDown(2);
	m_oTailFrameSocket.Close();
	m_oTailTimeFrameSocket.ShutDown(2);
	m_oTailTimeFrameSocket.Close();
	m_oSysTimeSocket.ShutDown(2);
	m_oSysTimeSocket.Close();
	m_oADCSetSocket.ShutDown(2);
	m_oADCSetSocket.Close();
	m_oHeartBeatSocket.ShutDown(2);
	m_oHeartBeatSocket.Close();
	m_oADCDataSocket.ShutDown(2);
	m_oADCDataSocket.Close();
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
	m_oHeartBeatThread.ResumeThread();
	m_oADCDataRecThread.OnReset();
	m_oADCDataRecThread.ResumeThread();

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
	m_oHeartBeatThread.SuspendThread();
	m_oADCDataRecThread.SuspendThread();

	m_oHeadFrameSocket.ShutDown(2);
	m_oHeadFrameSocket.Close();
	m_oIPSetSocket.ShutDown(2);
	m_oIPSetSocket.Close();
	m_oTailFrameSocket.ShutDown(2);
	m_oTailFrameSocket.Close();
	m_oTailTimeFrameSocket.ShutDown(2);
	m_oTailTimeFrameSocket.Close();
	m_oSysTimeSocket.ShutDown(2);
	m_oSysTimeSocket.Close();
	m_oADCSetSocket.ShutDown(2);
	m_oADCSetSocket.Close();
	m_oHeartBeatSocket.ShutDown(2);
	m_oHeartBeatSocket.Close();
	m_oADCDataSocket.ShutDown(2);
	m_oADCDataSocket.Close();
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
void CThreadManage::OnCreateAndSetSocket(CSocket* socket, bool bBroadCast, 
										 int iSocketPort, CString str, int iRecBuf, int iSendBuf)
{
	CString strTemp = _T("");
// 	socket->ShutDown(2);
// 	socket->Close();
	BOOL bReturn = FALSE;
	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	// @@@@@@@@@需要改进
	bReturn = socket->Create(iSocketPort, SOCK_DGRAM);
	if (bReturn == FALSE)
	{
		strTemp = str + _T("创建失败！");
		AfxMessageBox(strTemp);
	}
	else
	{
		if (bBroadCast == true)
		{
			//设置广播模式
			int iOptlen = sizeof(int);
			int iOptval = 1;
			bReturn = socket->SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
			if (bReturn == FALSE)
			{
				strTemp = str + _T("设置为广播端口失败！");
				AfxMessageBox(strTemp);
			}
		}
		int iOptionValue = iSendBuf;
		int iOptionLen = sizeof(int);
		bReturn = socket->SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
		if (bReturn == FALSE)
		{
			strTemp = str + _T("发送缓冲区设置失败！");
			AfxMessageBox(strTemp);
		}

		iOptionValue = iRecBuf;
		bReturn = socket->SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
		if (bReturn == FALSE)
		{
			strTemp = str + _T("接收缓冲区设置失败！");
			AfxMessageBox(strTemp);
		}
	}
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
	OnCreateAndSetSocket(&m_oSysTimeSocket, true, CollectSysTimePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oSysTimeSocket);
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
	OnCreateAndSetSocket(&m_oHeadFrameSocket, true, HeadFramePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oHeadFrameSocket);
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
	OnCreateAndSetSocket(&m_oIPSetSocket, true, IPSetPort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oIPSetSocket);
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
	OnCreateAndSetSocket(&m_oTailFrameSocket, true, TailFramePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oTailFrameSocket);
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
	OnCreateAndSetSocket(&m_oTailTimeFrameSocket, true, TailTimeFramePort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oTailTimeFrameSocket);
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
	OnCreateAndSetSocket(&m_oADCSetSocket, true, ADSetReturnPort, str, 
		InstrumentMaxCount * RcvFrameSize, InstrumentMaxCount * SndFrameSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oADCSetSocket);
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
	OnCreateAndSetSocket(&m_oHeartBeatSocket, true, HeartBeatRec, str, 
		HeartBeatSndBufSize, HeartBeatSndBufSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oHeartBeatSocket);
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
	OnCreateAndSetSocket(&m_oADCDataSocket, true, ADRecPort, str, 
		ADCBufSize, ADCBufSize);
	// 避免端口阻塞
	OnAvoidIOBlock(m_oADCDataSocket);
}
// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CThreadManage::ProcessMessages
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CThreadManage::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}