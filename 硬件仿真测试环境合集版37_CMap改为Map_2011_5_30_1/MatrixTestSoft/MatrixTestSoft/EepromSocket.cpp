// EepromSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "EepromSocket.h"


// CEepromSocket

CEepromSocket::CEepromSocket()
: m_uiNeedToReadNum(0)
, m_uiOneFrameReadMax(0)
, m_uiReadEepromNum(0)
{
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
	unsigned int uiMaxNum = 0;
	unsigned int ui18Count = 0;
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
		iPos += FrameCmdSize1B;
		iPos += FramePacketSize4B;
		if (m_uiNeedToReadNum < m_uiReadEepromNum + m_uiOneFrameReadMax)
		{
			uiMaxNum = m_uiNeedToReadNum - m_uiReadEepromNum;
		} 
		else
		{
			uiMaxNum = m_uiOneFrameReadMax;
		}
		do 
		{
			memcpy(&ucCommand, &m_ucRecBuf[iPos], FrameCmdSize1B);
			iPos += FrameCmdSize1B;
			if (ucCommand == CmdADCSet)
			{
				if (uiMaxNum <= (ui18Count + FramePacketSize4B))
				{
					memcpy(&m_ucReadEepromBuf[m_uiReadEepromNum], &m_ucRecBuf[iPos], uiMaxNum - ui18Count);
					iPos += uiMaxNum - ui18Count;
					m_uiReadEepromNum += uiMaxNum - ui18Count;
					break;
				}
				memcpy(&m_ucReadEepromBuf[m_uiReadEepromNum], &m_ucRecBuf[iPos], FramePacketSize4B);
				iPos += FramePacketSize4B;
				m_uiReadEepromNum += FramePacketSize4B;
				ui18Count += FramePacketSize4B;
			}
		} while (ucCommand != SndFrameBufInit);
	}
}
// 初始化
void CEepromSocket::OnInit(void)
{
	m_uiNeedToReadNum = 0;
	m_uiOneFrameReadMax = 0;
	m_uiReadEepromNum = 0;
}
