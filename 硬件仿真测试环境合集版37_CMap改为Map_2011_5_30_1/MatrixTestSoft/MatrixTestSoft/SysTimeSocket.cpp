// SysTimeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "SysTimeSocket.h"
#include "Parameter.h"

// CSysTimeSocket

CSysTime::CSysTime()
: m_uiSysTime(0)
, m_uiIPSource(0)
, m_pADCSet(NULL)
, m_pwnd(NULL)
, m_uiSysTimeCount(0)
, m_pADCFrameInfo(NULL)
, m_pLogFile(NULL)
, m_SysTimeSocket(INVALID_SOCKET)
{
}

CSysTime::~CSysTime()
{
}


// CSysTimeSocket 成员函数
void CSysTime::OnReceive(void) 
{
	// TODO: Add your specialized code here and/or call the base class
	int ret=0;
	sockaddr_in SenderAddr;
	int n = sizeof(SenderAddr);
	ret = recvfrom(m_SysTimeSocket, (char*)&udp_buf, sizeof(udp_buf), 0, (sockaddr*)&SenderAddr, &n);
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
	else if (ret == SOCKET_ERROR)
	{
		int iError = 0;
		CString str = _T("");
		iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			str.Format(_T("本地时间查询接收帧错误，错误号为%d！"), iError);
			m_pLogFile->OnWriteLogFile(_T("CSysTime::OnReceive"), str, ErrorStatus);
		}
	}
	else
	{
		CString str = _T("");
		str.Format(_T("本地时间查询接收帧帧长错误，帧长为%d！"), ret);
		m_pLogFile->OnWriteLogFile(_T("CSysTime::OnReceive"), str, ErrorStatus);
	}
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
void CSysTime::OnADCStartSample(unsigned int tnow)
{
	m_pADCSet->OnADCStartSample(tnow);
//	SetTimer(m_pwnd->m_hWnd, TabSampleStartSampleTimerNb, TabSampleStartSampleTimerSet, NULL);
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
void CSysTime::MakeCollectSysTimeFrameData(int* pSelectObject)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	CString str = _T("");
	memset(m_cCollectSysTimeSendData, SndFrameBufInit, SndFrameSize);
	m_cCollectSysTimeSendData[0] = FrameHeadCheck0;
	m_cCollectSysTimeSendData[1] = FrameHeadCheck1;
	m_cCollectSysTimeSendData[2] = FrameHeadCheck2;
	m_cCollectSysTimeSendData[3] = FrameHeadCheck3;
	memset(&m_cCollectSysTimeSendData[FrameHeadCheckSize], SndFrameBufInit, (FrameHeadSize - FrameHeadCheckSize));

	uiIPSource	=	m_uiIPSource;
	for (int i=0; i<InstrumentNum; i++)
	{
//		ProcessMessages();
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
	str.Format(_T("查询IP地址为%d的采集站本地时间！"), uiIPAim);
	m_pLogFile->OnWriteLogFile(_T("CSysTimeSocket::MakeCollectSysTimeFrameData"), str, SuccessStatus);
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
void CSysTime::SendCollectSysTimeFrameToSocket(void)
{
	// 发送帧
	int iCount = sendto(m_SysTimeSocket, (const char*)&m_cCollectSysTimeSendData, SndFrameSize, 0, (sockaddr*)&m_SendToAddr, sizeof(m_SendToAddr));
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
void CSysTime::ProcessMessages(void)
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
void CSysTime::OnProcSysTimeReturn(int iPos)
{
	unsigned int uiSysTime = 0;
	memcpy(&uiSysTime, &udp_buf[iPos], FramePacketSize4B);
	m_uiSysTimeCount++;
	if (m_uiSysTimeCount == 1)
	{
		CString strOutput = _T("");
		m_uiSysTime = uiSysTime;
		// 广播命令开始采样
		OnADCStartSample(m_uiSysTime);
		strOutput.Format(_T("设置ADC数据采样TB开始时间为%d\r\n"), m_uiSysTime + TBSleepTimeHigh);
		//因为需要保存的内容包含中文，所以需要如下的转换过程
//		WriteCHToCFile(m_pADCFrameInfo->m_arFileSave, strOutput);
		if (m_pADCFrameInfo->m_FileSave.m_hFile != NULL)
		{
			m_pADCFrameInfo->m_arFileSave->WriteString(strOutput);
		}
	}
}
// 关闭UDP套接字
void CSysTime::OnCloseUDP(void)
{
	shutdown(m_SysTimeSocket, SD_BOTH);
	closesocket(m_SysTimeSocket);
	m_SysTimeSocket = INVALID_SOCKET;
}
