// SysTimeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "SysTimeSocket.h"
#include "Parameter.h"

// CSysTimeSocket

CSysTimeSocket::CSysTimeSocket()
: m_uiSysTime(0)
, m_csIPSource(_T(""))
, m_uiSendPort(0)
, m_pADCSet(NULL)
, m_pwnd(NULL)
, m_uiSysTimeCount(0)
, m_pADCFrameInfo(NULL)
{
}

CSysTimeSocket::~CSysTimeSocket()
{
	if (m_pADCSet != NULL)
	{
		m_pADCSet = NULL;
		delete m_pADCSet;
	}
	if (m_pwnd != NULL)
	{
		m_pwnd = NULL;
		delete m_pwnd;
	}
	if (m_pADCFrameInfo != NULL)
	{
		m_pADCFrameInfo = NULL;
		delete m_pADCFrameInfo;
	}
}


// CSysTimeSocket 成员函数
void CSysTimeSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int ret=0;
	ret = Receive(udp_buf, RcvFrameSize);
	if(ret == RcvFrameSize) 
	{
		unsigned short usCommand = 0;
		byte	ucCommand = 0;
		int iPos = 26;
		unsigned short usCRC16 = 0;
		memcpy(&usCRC16, &udp_buf[RcvFrameSize - CRCSize], CRCSize);
		if (usCRC16 != get_crc_16(&udp_buf[FrameHeadSize], RcvFrameSize - FrameHeadSize - CRCCheckSize))
		{
		//	return FALSE;
		}
		memcpy(&usCommand, &udp_buf[iPos], FramePacketSize2B);
		iPos += FramePacketSize2B;
		if (usCommand == SendQueryCmd)
		{
			memcpy(&ucCommand, &udp_buf[iPos], FrameCmdSize1B);
			iPos += FrameCmdSize1B;
			if (ucCommand == CmdLocalSysTime)
			{
				// 查询本地时间，0x04命令
				OnProcSysTimeReturn(iPos);
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

// ADC设置TB时刻开始采集
//************************************
// Method:    OnADCStartSample
// FullName:  CSysTimeSocket::OnADCStartSample
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: unsigned int uiIPAim
// Parameter: unsigned int tnow
//************************************
void CSysTimeSocket::OnADCStartSample(unsigned int tnow)
{
	m_pADCSet->OnADCStartSample(tnow);
	SetTimer(m_pwnd->m_hWnd, TabSampleStartSampleTimerNb, TabSampleStartSampleTimerSet, NULL);
}
// 生成采集站本地时间查询帧
//************************************
// Method:    MakeCollectSysTimeFrameData
// FullName:  CThreadManage::MakeCollectSysTimeFrameData
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int * pSelectObject
//************************************
void CSysTimeSocket::MakeCollectSysTimeFrameData(int* pSelectObject)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	memset(m_cCollectSysTimeSendData, SndFrameBufInit, SndFrameSize);
	m_cCollectSysTimeSendData[0] = FrameHeadCheck0;
	m_cCollectSysTimeSendData[1] = FrameHeadCheck1;
	m_cCollectSysTimeSendData[2] = FrameHeadCheck2;
	m_cCollectSysTimeSendData[3] = FrameHeadCheck3;
	memset(&m_cCollectSysTimeSendData[FrameHeadCheckSize], SndFrameBufInit, (FrameHeadSize - FrameHeadCheckSize));

	// CString转换为const char*
	char pach[100];
	CStringW strw;
	wstring wstr;
	strw = m_csIPSource;
	wstr = strw;
	string mstring = WideCharToMultiChar(wstr );
	strcpy_s( pach, sizeof(pach), mstring.c_str() );

	uiIPSource	=	inet_addr(pach);
	for (int i=0; i<InstrumentNum; i++)
	{
		ProcessMessages();
		if (pSelectObject[i] == 1)
		{
			uiIPAim	= IPSetAddrStart + IPSetAddrInterval * (i+1);
			break;
		}
	}
	usPortAim	=	CollectSysTimePort;
	usCommand	=	SendQueryCmd;
	int iPos = 16;
	// 源IP地址
	memcpy(&m_cCollectSysTimeSendData[iPos], &uiIPSource, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&m_cCollectSysTimeSendData[iPos], &uiIPAim, FramePacketSize4B);
	iPos += FramePacketSize4B;
	TRACE1("采集站本地时间查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&m_cCollectSysTimeSendData[iPos], &usPortAim, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_cCollectSysTimeSendData[iPos], &usCommand, FramePacketSize2B);

	iPos = 32;
	// 命令字0x04读取本地系统时间
	m_cCollectSysTimeSendData[iPos] = CmdLocalSysTime;
	iPos += FrameCmdSize1B;
	memset(&m_cCollectSysTimeSendData[iPos], SndFrameBufInit, FramePacketSize4B);
	iPos += FramePacketSize4B;

	// 设置命令字结束
	m_cCollectSysTimeSendData[iPos] = SndFrameBufInit;

	unsigned short usCRC16 = 0;
	usCRC16 = get_crc_16(&m_cCollectSysTimeSendData[FrameHeadSize], SndFrameSize - FrameHeadSize - CRCCheckSize);
	memcpy(&m_cCollectSysTimeSendData[SndFrameSize - CRCSize], &usCRC16, CRCSize);
}

// 发送采集站本地时间查询帧
//************************************
// Method:    SendCollectSysTimeFrameToSocket
// FullName:  CThreadManage::SendCollectSysTimeFrameToSocket
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSysTimeSocket::SendCollectSysTimeFrameToSocket(void)
{
	// 发送帧
	int iCount = SendTo(m_cCollectSysTimeSendData, SndFrameSize, m_uiSendPort, IPBroadcastAddr);
}

// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CSysTimeSocket::ProcessMessages
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CSysTimeSocket::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}

// 处理本地时间查询应答
//************************************
// Method:    OnProcSysTimeReturn
// FullName:  CSysTimeSocket::OnProcSysTimeReturn
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int iPos
//************************************
void CSysTimeSocket::OnProcSysTimeReturn(int iPos)
{
	unsigned int uiSysTime = 0;
	memcpy(&uiSysTime, &udp_buf[iPos], FramePacketSize4B);
	m_uiSysTimeCount++;
	if (m_uiSysTimeCount == 1)
	{
		CString strOutput = _T("");
		m_uiSysTime = uiSysTime;
		// 广播命令开始采样
		TRACE(_T("查询到的本地时间%d"), m_uiSysTime);
		OnADCStartSample(m_uiSysTime);
		// 在ADC数据帧信息文件中写入TB时间
// 		if (m_pADCFrameInfo->m_pFileSave != NULL)
// 		{
// 			strOutput.Format(_T("设置ADC数据采样TB开始时间为%d\n"), m_uiSysTime + TBSleepTimeHigh);
// 			fprintf(m_pADCFrameInfo->m_pFileSave, _T("%s"), strOutput); 			
// 		}
		strOutput.Format(_T("设置ADC数据采样TB开始时间为%d\n"), m_uiSysTime + TBSleepTimeHigh);
		//因为需要保存的内容包含中文，所以需要如下的转换过程
		int ansiCount=WideCharToMultiByte(CP_ACP,0,strOutput,-1,NULL,0,NULL,NULL);
		char * pTempChar=(char*)malloc(ansiCount*sizeof(char));
		memset(pTempChar,0,ansiCount);
		WideCharToMultiByte(CP_ACP,0,strOutput,-1,pTempChar,ansiCount,NULL,NULL);
		m_pADCFrameInfo->m_FileSave.Write(pTempChar, ansiCount);
		free(pTempChar);
	}
}