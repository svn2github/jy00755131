// EepromSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "EepromSocket.h"


// CEepromSocket

CEepromSocket::CEepromSocket()
{
	m_uiReadEepromNum = 0;
}

CEepromSocket::~CEepromSocket()
{
}


// CEepromSocket 成员函数

void CEepromSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int ret=0;
	unsigned short uiPort = 0;
	ret = Receive(m_ucRecBuf, RcvFrameSize);
	if(ret == RcvFrameSize) 
	{
		// 单个帧处理
		ProcFrameOne();
	}
	CSocket::OnReceive(nErrorCode);
}
// 单个帧处理
void CEepromSocket::ProcFrameOne(void)
{
	unsigned short usCommand = 0;
	unsigned char ucCommand = 0;
	unsigned int uiInstrumentIP = 0;
	int iPos = FrameHeadSize;
	// [16]到[19]为源地址
	memcpy(&uiInstrumentIP, &m_ucRecBuf[iPos], FramePacketSize4B);
	iPos += FramePacketSize4B;
	iPos += FramePacketSize4B;
	iPos += FramePacketSize2B;
	// 设m_oADCRecFrameBuf[26]到m_oADCRecFrameBuf[27]为命令字
	memcpy(&usCommand, &m_ucRecBuf[iPos], FramePacketSize2B);
	iPos += FramePacketSize2B;
	if (usCommand == SendQueryCmd)
	{
		do 
		{
			memcpy(&ucCommand, &m_ucRecBuf[iPos], FrameCmdSize1B);
			iPos += FrameCmdSize1B;
			if (ucCommand == CmdADCSet)
			{
				memcpy(&m_ucReadEepromBuf[m_uiReadEepromNum], &m_ucRecBuf[iPos], FramePacketSize4B);
				iPos += FramePacketSize4B;
				m_uiReadEepromNum += FramePacketSize4B;
			}
		} while (ucCommand != SndFrameBufInit);
	}
}