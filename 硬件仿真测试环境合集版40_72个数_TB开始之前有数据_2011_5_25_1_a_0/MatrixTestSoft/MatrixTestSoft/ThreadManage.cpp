#include "StdAfx.h"
#include "ThreadManage.h"

// 发送尾包时刻查询帧计数
unsigned int m_uiSendTailTimeFrameCount;
// 接收到的尾包时刻查询帧计数
unsigned int m_uiRecTailTimeFrameCount;

static unsigned short CRC_TABLE[256]=
{
	0x0000,0x8005,0x800F,0x000A,0x801B,0x001E,0x0014,0x8011,
	0x8033,0x0036,0x003C,0x8039,0x0028,0x802D,0x8027,0x0022,
	0x8063,0x0066,0x006C,0x8069,0x0078,0x807D,0x8077,0x0072,
	0x0050,0x8055,0x805F,0x005A,0x804B,0x004E,0x0044,0x8041,
	0x80C3,0x00C6,0x00CC,0x80C9,0x00D8,0x80DD,0x80D7,0x00D2,
	0x00F0,0x80F5,0x80FF,0x00FA,0x80EB,0x00EE,0x00E4,0x80E1,
	0x00A0,0x80A5,0x80AF,0x00AA,0x80BB,0x00BE,0x00B4,0x80B1,
	0x8093,0x0096,0x009C,0x8099,0x0088,0x808D,0x8087,0x0082,
	0x8183,0x0186,0x018C,0x8189,0x0198,0x819D,0x8197,0x0192,
	0x01B0,0x81B5,0x81BF,0x01BA,0x81AB,0x01AE,0x01A4,0x81A1,
	0x01E0,0x81E5,0x81EF,0x01EA,0x81FB,0x01FE,0x01F4,0x81F1,
	0x81D3,0x01D6,0x01DC,0x81D9,0x01C8,0x81CD,0x81C7,0x01C2,
	0x0140,0x8145,0x814F,0x014A,0x815B,0x015E,0x0154,0x8151,
	0x8173,0x0176,0x017C,0x8179,0x0168,0x816D,0x8167,0x0162,
	0x8123,0x0126,0x012C,0x8129,0x0138,0x813D,0x8137,0x0132,
	0x0110,0x8115,0x811F,0x011A,0x810B,0x010E,0x0104,0x8101,
	0x8303,0x0306,0x030C,0x8309,0x0318,0x831D,0x8317,0x0312,
	0x0330,0x8335,0x833F,0x033A,0x832B,0x032E,0x0324,0x8321,
	0x0360,0x8365,0x836F,0x036A,0x837B,0x037E,0x0374,0x8371,
	0x8353,0x0356,0x035C,0x8359,0x0348,0x834D,0x8347,0x0342,
	0x03C0,0x83C5,0x83CF,0x03CA,0x83DB,0x03DE,0x03D4,0x83D1,
	0x83F3,0x03F6,0x03FC,0x83F9,0x03E8,0x83ED,0x83E7,0x03E2,
	0x83A3,0x03A6,0x03AC,0x83A9,0x03B8,0x83BD,0x83B7,0x03B2,
	0x0390,0x8395,0x839F,0x039A,0x838B,0x038E,0x0384,0x8381,
	0x0280,0x8285,0x828F,0x028A,0x829B,0x029E,0x0294,0x8291,
	0x82B3,0x02B6,0x02BC,0x82B9,0x02A8,0x82AD,0x82A7,0x02A2,
	0x82E3,0x02E6,0x02EC,0x82E9,0x02F8,0x82FD,0x82F7,0x02F2,
	0x02D0,0x82D5,0x82DF,0x02DA,0x82CB,0x02CE,0x02C4,0x82C1,
	0x8243,0x0246,0x024C,0x8249,0x0258,0x825D,0x8257,0x0252,
	0x0270,0x8275,0x827F,0x027A,0x826B,0x026E,0x0264,0x8261,
	0x0220,0x8225,0x822F,0x022A,0x823B,0x023E,0x0234,0x8231,
	0x8213,0x0216,0x021C,0x8219,0x0208,0x820D,0x8207,0x0202
};

/*
函数名称：unsigned short get_crc_16_private(const char* buf, int buf_size)
功能描述：通过输入指定长度的字符串生成CRC校验码
返回值：CRC16编码
参数：
buf		cnost char*			传入字符串指针
buf_size	int				字符串长度
注：
*/
unsigned short get_crc_16(unsigned char* buf, int buf_size)
{
	unsigned short crc_16 = 0;
	unsigned char tmp = 0;
	unsigned char* ptr = (unsigned char*)buf;

	while(buf_size--){

		tmp = crc_16 >> 8;
		crc_16 <<= 8;
		crc_16 ^= CRC_TABLE[tmp ^ *ptr++];
	}
	return crc_16;
}

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
	m_oADCDataRecThread.m_pADCDataSocket = &m_oADCDataSocket;
	m_oADCDataRecThread.m_pADCDataSaveToFile = &m_oADCDataSaveToFile;
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

	iResult = WaitForSingleObject(m_oADCDataRecThread.m_hADCDataThreadClose, WaitForThreadCloseTime);
	if (iResult != WAIT_OBJECT_0)
	{
		str.Format(_T("ADC数据接收线程在%dms内未能正常结束！"), WaitForThreadCloseTime);
		AfxMessageBox(str);
	}
	
	m_oHeadFrameSocket.Close();
	m_oIPSetSocket.Close();
	m_oTailFrameSocket.Close();
	m_oTailTimeFrameSocket.Close();
	m_oSysTimeSocket.Close();
	m_oADCSetSocket.Close();
	m_oHeartBeatSocket.Close();
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

	m_oHeadFrameSocket.Close();
	m_oIPSetSocket.Close();
	m_oTailFrameSocket.Close();
	m_oTailTimeFrameSocket.Close();
	m_oSysTimeSocket.Close();
	m_oADCSetSocket.Close();
	m_oHeartBeatSocket.Close();
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
//	socket->ShutDown(2);
	socket->Close();
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