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
void CThreadManage::OnInit(void)
{
	m_oInstrumentList.OnInit();
	m_HeadFrameSocket.m_pInstrumentList = &m_oInstrumentList;
	m_IPSetSocket.m_pInstrumentList = &m_oInstrumentList;
	m_TailFrameSocket.m_pInstrumentList = &m_oInstrumentList;
	m_TailTimeFrameSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oSysTimeSocket.m_pInstrumentList = &m_oInstrumentList;
	m_oADCRecSocket.m_pADCDataProcessThread = &m_oADCDataProcessThread;
	m_oADCDataProcessThread.m_pADCDataSaveToFileThread = &m_oADCDataSaveToFileThread;

	m_oHeartBeatThread.OnInit();
	m_oHeartBeatThread.CreateThread();
	m_oHeartBeatThread.SuspendThread();

// 	m_oADCDataRecThread.OnInit();
// 	m_oADCDataRecThread.CreateThread();
// 	m_oADCDataRecThread.SuspendThread();

	m_oADCDataProcessThread.OnInit();
	m_oADCDataProcessThread.CreateThread();
	m_oADCDataProcessThread.SuspendThread();

	m_oADCDataSaveToFileThread.OnInit();
	m_oADCDataSaveToFileThread.CreateThread();
	m_oADCDataSaveToFileThread.SuspendThread();

	for (int i=0; i<GraphViewNum; i++)
	{
		ProcessMessages();
		m_oADCRecSocket.m_dADCDataShow[i] = 0;
	}

	OnCreateHeadSocket();
	OnCreateIPSetSocket();
	OnCreateTailSocket();
	OnCreateTailTimeSocket();
	OnCreateADCRecSocket();
	OnCreateCollectSysTimeBroadcastSocket();
	// 发送尾包时刻查询帧计数
	m_uiSendTailTimeFrameCount = 0;
	// 接收到的尾包时刻查询帧计数
	m_uiRecTailTimeFrameCount = 0;
}

// 关闭并结束线程
void CThreadManage::OnClose(void)
{
	int iResult = 0;
	m_oHeartBeatThread.OnClose();
	m_oADCDataProcessThread.OnClose();
	m_oADCDataSaveToFileThread.OnClose();
	iResult = ::WaitForSingleObject(m_oHeartBeatThread.m_hHeartBeatThreadClose,550);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("心跳线程在550ms内未能正常结束！"));
	}
	iResult = ::WaitForSingleObject(m_oADCDataProcessThread.m_hADCDataProcessThreadClose,550);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("ADC数据处理线程在550ms内未能正常结束！"));
	}
	iResult = ::WaitForSingleObject(m_oADCDataSaveToFileThread.m_hADCDataSaveToFileThreadClose,550);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("ADC数据存储线程在550ms内未能正常结束！"));
	}

	m_HeadFrameSocket.Close();
	m_IPSetSocket.Close();
	m_TailFrameSocket.Close();
	m_TailTimeFrameSocket.Close();
	m_oSysTimeSocket.Close();
	m_oADCRecSocket.Close();
	m_oInstrumentList.OnClose();
}

// 打开
void CThreadManage::OnOpen(void)
{
	m_oInstrumentList.OnInit();
	m_oInstrumentList.OnOpen();

	m_oHeartBeatThread.ResumeThread();
	m_oHeartBeatThread.OnOpen();

// 	m_oADCDataRecThread.ResumeThread();
// 	m_oADCDataRecThread.OnOpen();

	m_oADCDataProcessThread.ResumeThread();
	m_oADCDataProcessThread.OnOpen();

	m_oADCDataSaveToFileThread.ResumeThread();
	m_oADCDataSaveToFileThread.OnOpen();

	OnCreateHeadSocket();
	OnCreateIPSetSocket();
	OnCreateTailSocket();
	OnCreateTailTimeSocket();
	OnCreateADCRecSocket();
	OnCreateCollectSysTimeBroadcastSocket();

	// 发送尾包时刻查询帧计数
	m_uiSendTailTimeFrameCount = 0;
	// 接收到的尾包时刻查询帧计数
	m_uiRecTailTimeFrameCount = 0;
}

// 停止
void CThreadManage::OnStop(void)
{
	m_oInstrumentList.OnStop();

	m_oHeartBeatThread.SuspendThread();
	m_oHeartBeatThread.OnStop();

// 	m_oADCDataRecThread.SuspendThread();
// 	m_oADCDataRecThread.OnStop();

	m_oADCDataProcessThread.SuspendThread();
	m_oADCDataProcessThread.OnStop();

	m_oADCDataSaveToFileThread.SuspendThread();
	m_oADCDataSaveToFileThread.OnStop();
	
	m_HeadFrameSocket.Close();
	m_IPSetSocket.Close();
	m_TailFrameSocket.Close();
	m_TailTimeFrameSocket.Close();
	m_oSysTimeSocket.Close();
	m_oADCRecSocket.Close();
}

// 创建查询采集站本地时间的广播端口
void CThreadManage::OnCreateCollectSysTimeBroadcastSocket(void)
{
	m_oSysTimeSocket.Close();

	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	if (FALSE == m_oSysTimeSocket.Create(CollectSysTimePort, SOCK_DGRAM))
	{
		AfxMessageBox(_T("命令广播端口创建失败！"));
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_oSysTimeSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	int iOptionValue = InstrumentMaxCount * SndFrameSize;
	int iOptionLen = sizeof(int);
	m_oSysTimeSocket.SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	iOptionValue = InstrumentMaxCount * RcvFrameSize;
	m_oSysTimeSocket.SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
}

// 生成采集站本地时间查询帧
void CThreadManage::MakeCollectSysTimeFrameData(int* pSelectObject)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	for(int i=0; i<SndFrameSize; i++)
	{
		m_cCollectSysTimeSendData[i] = 0x00;
	}
	m_cCollectSysTimeSendData[0] = 0x11;
	m_cCollectSysTimeSendData[1] = 0x22;
	m_cCollectSysTimeSendData[2] = 0x33;
	m_cCollectSysTimeSendData[3] = 0x44;
	m_cCollectSysTimeSendData[4] = 0x00;
	m_cCollectSysTimeSendData[5] = 0x00;
	m_cCollectSysTimeSendData[6] = 0x00;
	m_cCollectSysTimeSendData[7] = 0x00;
	m_cCollectSysTimeSendData[8] = 0x00;
	m_cCollectSysTimeSendData[9] = 0x00;
	m_cCollectSysTimeSendData[10] = 0x00;
	m_cCollectSysTimeSendData[11] = 0x00;
	m_cCollectSysTimeSendData[12] = 0x00;
	m_cCollectSysTimeSendData[13] = 0x00;
	m_cCollectSysTimeSendData[14] = 0x00;
	m_cCollectSysTimeSendData[15] = 0x00;
	
	CString str;
	str = m_oSysTimeSocket.m_csIPSource;
	uiIPSource	=	inet_addr(str);
	for (int i=0; i<GraphViewNum; i++)
	{
		ProcessMessages();
		if (pSelectObject[i] == 1)
		{
			uiIPAim	= 81 + 10*i;
			break;
		}
	}
	/*	uiIPAim = 111;*/
	usPortAim	=	CollectSysTimePort;
	usCommand	=	SendQueryCmd;
	// 源IP地址
	memcpy(&m_cCollectSysTimeSendData[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_cCollectSysTimeSendData[20], &uiIPAim, 4);
	TRACE1("采集站本地时间查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&m_cCollectSysTimeSendData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_cCollectSysTimeSendData[26], &usCommand, 2);

	int iPos = 32;

	// 命令字0x04读取本地系统时间
	m_cCollectSysTimeSendData[iPos] = 0x04;
	iPos++;
	memset(&m_cCollectSysTimeSendData[iPos], 0, 4);
	iPos = iPos + 4;

	// 设置命令字结束
	m_cCollectSysTimeSendData[iPos] = 0x00;

	unsigned short usCRC16 = 0;
	usCRC16 = get_crc_16(&m_cCollectSysTimeSendData[FrameHeadSize], SndFrameSize - FrameHeadSize - CRCCheckSize);
	memcpy(&m_cCollectSysTimeSendData[SndFrameSize - CRCSize], &usCRC16, 2);
}

// 发送采集站本地时间查询帧
void CThreadManage::SendCollectSysTimeFrameToSocket(void)
{
	// 发送帧
	int iCount = m_oSysTimeSocket.SendTo(m_cCollectSysTimeSendData, SndFrameSize, m_oSysTimeSocket.m_uiSendPort, IPBroadcastAddr);
}

// 创建首包Socket
void CThreadManage::OnCreateHeadSocket(void)
{
	m_HeadFrameSocket.Close();
	if (FALSE == m_HeadFrameSocket.Create(HeadFramePort,SOCK_DGRAM))
	{
		AfxMessageBox(_T("首包接收端口创建失败！"));
	}
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_HeadFrameSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	int iOptionValue = InstrumentMaxCount * SndFrameSize;
	int iOptionLen = sizeof(int);
	m_HeadFrameSocket.SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	iOptionValue = InstrumentMaxCount * RcvFrameSize;
	m_HeadFrameSocket.SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
}

// 创建IP地址设置Socket
void CThreadManage::OnCreateIPSetSocket(void)
{
	m_IPSetSocket.Close();

	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	if (FALSE ==  m_IPSetSocket.Create(IPSetPort, SOCK_DGRAM))
	{
		AfxMessageBox(_T("IP地址设置端口创建失败！"));
	}
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_IPSetSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	int iOptionValue = InstrumentMaxCount * SndFrameSize;
	int iOptionLen = sizeof(int);
	m_IPSetSocket.SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	iOptionValue = InstrumentMaxCount * RcvFrameSize;
	m_IPSetSocket.SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
}

// 创建尾包Socket
void CThreadManage::OnCreateTailSocket(void)
{
	m_TailFrameSocket.Close();

	if (FALSE == m_TailFrameSocket.Create(TailFramePort,SOCK_DGRAM))
	{
		AfxMessageBox(_T("尾包接收端口创建失败！"));
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_TailFrameSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	int iOptionValue = InstrumentMaxCount * SndFrameSize;
	int iOptionLen = sizeof(int);
	m_TailFrameSocket.SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	iOptionValue = InstrumentMaxCount * RcvFrameSize;
	m_TailFrameSocket.SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

}

// 创建尾包时刻查询和时统Socket
void CThreadManage::OnCreateTailTimeSocket(void)
{
	m_TailTimeFrameSocket.Close();
	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	if (FALSE == m_TailTimeFrameSocket.Create(TailTimeFramePort, SOCK_DGRAM))
	{
		AfxMessageBox(_T("尾包时刻查询和时统端口创建失败！"));
	}
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_TailTimeFrameSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	int iOptionValue = InstrumentMaxCount * SndFrameSize;
	int iOptionLen = sizeof(int);
	m_TailTimeFrameSocket.SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	iOptionValue = InstrumentMaxCount * RcvFrameSize;
	m_TailTimeFrameSocket.SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
}
// 防止程序在循环中运行无法响应消息
void CThreadManage::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}

// 创建ADC数据接收Socket
void CThreadManage::OnCreateADCRecSocket(void)
{
	m_oADCRecSocket.Close();

	if (FALSE == m_oADCRecSocket.Create(ADRecPort,SOCK_DGRAM))
	{
		AfxMessageBox(_T("ADC数据接收端口创建失败！"));
	}
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_oADCRecSocket.SetSockOpt(SO_BROADCAST, (void*)&iOptval, iOptlen, SOL_SOCKET);

	int iOptionValue = ADCBufSize;
	int iOptionLen = sizeof(int);
	m_oADCRecSocket.SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	m_oADCRecSocket.SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

}
