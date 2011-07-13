// TailFrameSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "TailFrameSocket.h"
#include "Parameter.h"

// CTailFrameSocket

CTailFrame::CTailFrame()
: m_uiSN(0)
, m_uiIPSource(0)
, m_bTailRec(FALSE)
, m_uiTailRecCount(0)
, m_pInstrumentList(NULL)
, m_pLogFile(NULL)
, m_TailFrameSocket(INVALID_SOCKET)
{
}

CTailFrame::~CTailFrame()
{
}


// CTailFrameSocket 成员函数

//************************************
// Method:    OnReceive
// FullName:  CTailFrameSocket::OnReceive
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int nErrorCode
//************************************
void CTailFrame::OnReceive(void)
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	int ret=0;
	unsigned short uiPort = 0;
	sockaddr_in SenderAddr;
	int n = sizeof(SenderAddr);
	ret = recvfrom(m_TailFrameSocket, (char*)&m_pTailFrameData, sizeof(m_pTailFrameData), 0, (sockaddr*)&SenderAddr, &n);
	if(ret == RcvFrameSize) 
	{
		// 单个尾包处理
		ProcTailFrameOne();
	}
	else if (ret == SOCKET_ERROR)
	{
		int iError = 0;
		CString str = _T("");
		iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			str.Format(_T("尾包接收帧错误，错误号为%d！"), iError);
			m_pLogFile->OnWriteLogFile(_T("CTailFrame::OnReceive"), str, ErrorStatus);
		}
	}
	else
	{
		CString str = _T("");
		str.Format(_T("尾包接收帧帧长错误，帧长为%d！"), ret);
		m_pLogFile->OnWriteLogFile(_T("CTailFrame::OnReceive"), str, ErrorStatus);
	}
}
// 处理单个尾包
//************************************
// Method:    ProcTailFrameOne
// FullName:  CTailFrameSocket::ProcTailFrameOne
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CTailFrame::ProcTailFrameOne(void)
{
	if (ParseTailFrame() == FALSE)
	{
		m_pLogFile->OnWriteLogFile(_T("CTailFrameSocket::ProcTailFrameOne"), _T("解析尾包帧出错！"), ErrorStatus);
		return;
	}
	CInstrument* pInstrument = NULL;
	// 在索引表中则找到该仪器,得到该仪器指针
	if (TRUE == m_pInstrumentList->GetInstrumentFromMap(m_uiSN, pInstrument))
	{
		// 如果是交叉站尾包则不处理，如果是采集站尾包
		if (pInstrument->m_uiInstrumentType == InstrumentTypeFDU)
		{
			// 尾包计数器加一
			pInstrument->m_iTailFrameCount++;
			// 如果尾包时刻查询在下一次尾包来临之际还未收全，则放弃上次查询结果并对未收到尾包时刻查询的仪器设为过期标志
			if (m_uiSendTailTimeFrameCount != m_uiRecTailTimeFrameCount)
			{
				m_uiRecTailTimeFrameCount = 0;
				m_uiSendTailTimeFrameCount = 0;
				m_pInstrumentList->ClearExperiedTailTimeResult();
			}
		}
	}
	else
	{
		return;
	}
	if (pInstrument->m_uiInstrumentType == InstrumentTypeLAUX)
	{
		m_bTailRec = TRUE;
		m_uiTailRecCount++;
	}
	// 判断尾包计数器达到设定值
	if (pInstrument->m_iTailFrameCount == TailFrameCount)
	{
		pInstrument->m_iTailFrameCount = 0;
		m_pInstrumentList->TailFrameDeleteInstrument(pInstrument);
	}
}

// 解析尾包
//************************************
// Method:    ParseTailFrame
// FullName:  CTailFrameSocket::ParseTailFrame
// Access:    private 
// Returns:   BOOL
// Qualifier:
// Parameter: void
//************************************
BOOL CTailFrame::ParseTailFrame(void)
{
	byte	usCommand = 0;
	unsigned short usCRC16 = 0;
	CString str = _T("");
	memcpy(&usCRC16, &m_pTailFrameData[RcvFrameSize - CRCSize], CRCSize);
	if (usCRC16 != get_crc_16(&m_pTailFrameData[FrameHeadSize], RcvFrameSize - FrameHeadSize - CRCCheckSize))
	{
		//	return FALSE;
	}
	int iPos = 33;
	// 仪器SN号
	memcpy(&usCommand, &m_pTailFrameData[iPos], FrameCmdSize1B);
	if (usCommand != CmdSn)
	{
		return FALSE;
	}
	iPos += FrameCmdSize1B;
	memcpy(&m_uiSN, &m_pTailFrameData[iPos], FramePacketSize4B);
	str.Format(_T("接收到SN为%x的仪器的尾包！"), m_uiSN);
	m_pLogFile->OnWriteLogFile(_T("CTailFrameSocket::ParseTailFrame"), str, SuccessStatus);
	return TRUE;
}

// 向仪器发送尾包时刻查询帧
//************************************
// Method:    SendTailTimeFrame
// FullName:  CTailFrameSocket::SendTailTimeFrame
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CTailFrame::SendTailTimeFrame(void)
{
	CString str = _T("");
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_pInstrumentList->m_oInstrumentMap.begin(); iter!=m_pInstrumentList->m_oInstrumentMap.end(); iter++)
	{
//		ProcessMessages();
		if (NULL != iter->second)
		{
			if (iter->second->m_bIPSetOK == true)
			{
				// 生成仪器尾包时刻查询帧
				MakeTailTimeFrameData(iter->second);
				// 发送仪器尾包时刻查询帧
				SendTailTimeFrameToSocket();
				m_uiSendTailTimeFrameCount++;
				iter->second->m_bSendTailTimeFrame = true;
				str.Format(_T("向IP地址为%d的仪器发送尾包时刻查询帧！"), iter->second->m_uiIPAddress);
				m_pLogFile->OnWriteLogFile(_T("CTailFrameSocket::SendTailTimeFrame"), str, SuccessStatus);
			}
		}
	}
}

// 生成尾包时刻查询帧
//************************************
// Method:    MakeTailTimeFrameData
// FullName:  CTailFrameSocket::MakeTailTimeFrameData
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: CInstrument * pInstrument
//************************************
void CTailFrame::MakeTailTimeFrameData(CInstrument* pInstrument)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned short usPortAim	=	0;
	unsigned short usCommand	=	0;
	memset(m_pTailTimeSendData, SndFrameBufInit, SndFrameSize);
	m_pTailTimeSendData[0] = FrameHeadCheck0;
	m_pTailTimeSendData[1] = FrameHeadCheck1;
	m_pTailTimeSendData[2] = FrameHeadCheck2;
	m_pTailTimeSendData[3] = FrameHeadCheck3;
	memset(&m_pTailTimeSendData[FrameHeadCheckSize], SndFrameBufInit, (FrameHeadSize - FrameHeadCheckSize));

	uiIPSource	=	m_uiIPSource;
	uiIPAim		=	pInstrument->m_uiIPAddress;
	usPortAim	=	TailTimeFramePort;
	usCommand	=	SendQueryCmd;
	int iPos = 16;
	// 源IP地址
	memcpy(&m_pTailTimeSendData[iPos], &uiIPSource, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&m_pTailTimeSendData[iPos], &uiIPAim, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&m_pTailTimeSendData[iPos], &usPortAim, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pTailTimeSendData[iPos], &usCommand, FramePacketSize2B);
	iPos += FramePacketSize2B;

	iPos = 32;
	// 命令字0x01读取设备SN
	m_pTailTimeSendData[iPos] = CmdSn;
	iPos += FrameCmdSize1B;
	memset(&m_pTailTimeSendData[iPos], SndFrameBufInit, FramePacketSize4B);
	iPos += FramePacketSize4B;

	// 命令字0x19读取网络时刻
	m_pTailTimeSendData[iPos] = CmdNetTime;
	iPos += FrameCmdSize1B;
	memset(&m_pTailTimeSendData[iPos], SndFrameBufInit, FramePacketSize4B);
	iPos += FramePacketSize4B;

	// 命令字0x04读取本地系统时间
	m_pTailTimeSendData[iPos] = CmdLocalSysTime;
	iPos += FrameCmdSize1B;
	memset(&m_pTailTimeSendData[iPos], SndFrameBufInit, FramePacketSize4B);
	iPos += FramePacketSize4B;

	// 仪器类型	1-交叉站
	if(InstrumentTypeLAUX == pInstrument->m_uiInstrumentType)
	{
		// 命令字0x1B  交叉站大线尾包接收时刻
		m_pTailTimeSendData[iPos] = CmdTailRecTimeLAUX;
		iPos += FrameCmdSize1B;
		memset(&m_pTailTimeSendData[iPos], SndFrameBufInit, FramePacketSize4B);
		iPos += FramePacketSize4B;
	}
	else
	{
		// 命令字0x16接收、发送时刻低位
		m_pTailTimeSendData[iPos] = CmdTailRecSndTimeLow;
		iPos += FrameCmdSize1B;
		memset(&m_pTailTimeSendData[iPos], SndFrameBufInit, FramePacketSize4B);
		iPos += FramePacketSize4B;
	}

	// 设置命令字结束
	m_pTailTimeSendData[iPos] = SndFrameBufInit;

	unsigned short usCRC16 = 0;
	usCRC16 = get_crc_16(&m_pTailTimeSendData[FrameHeadSize], SndFrameSize - FrameHeadSize - CRCCheckSize);
	memcpy(&m_pTailTimeSendData[SndFrameSize - CRCSize], &usCRC16, CRCSize);
}

// 发送尾包时刻查询帧
//************************************
// Method:    SendTailTimeFrameToSocket
// FullName:  CTailFrameSocket::SendTailTimeFrameToSocket
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CTailFrame::SendTailTimeFrameToSocket(void)
{
	// 发送帧
	int iCount = sendto(m_TailFrameSocket, (const char*)&m_pTailTimeSendData, SndFrameSize, 0, (sockaddr*)&m_SendToAddr, sizeof(m_SendToAddr));
}
// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CTailFrameSocket::ProcessMessages
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CTailFrame::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}

// 关闭UDP套接字
void CTailFrame::OnCloseUDP(void)
{
	shutdown(m_TailFrameSocket, SD_BOTH);
	closesocket(m_TailFrameSocket);
	m_TailFrameSocket = INVALID_SOCKET;
}
