// HeartBeatSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "HeartBeatSocket.h"
#include "Parameter.h"

// CHeartBeatSocket

CHeartBeatSocket::CHeartBeatSocket()
: m_uiSendPort(0)
{
}

CHeartBeatSocket::~CHeartBeatSocket()
{
}


// CHeartBeatSocket 成员函数
// 生成心跳帧
void CHeartBeatSocket::MakeHeartBeatFrame(void)
{
	unsigned int uiIPSource = 0;
	unsigned int uiIPAim = 0;
	unsigned short usPortAim = 0;
	unsigned short usCommand = 0;
	m_pFrameData[0] = 0x11;
	m_pFrameData[1] = 0x22;
	m_pFrameData[2] = 0x33;
	m_pFrameData[3] = 0x44;
	m_pFrameData[4] = 0x00;
	m_pFrameData[5] = 0x00;
	m_pFrameData[6] = 0x00;
	m_pFrameData[7] = 0x00;
	m_pFrameData[8] = 0x00;
	m_pFrameData[9] = 0x00;
	m_pFrameData[10] = 0x00;
	m_pFrameData[11] = 0x00;
	m_pFrameData[12] = 0x00;
	m_pFrameData[13] = 0x00;
	m_pFrameData[14] = 0x00;
	m_pFrameData[15] = 0x00;
	// 源IP地址
	uiIPSource = 0x00000000;
	// 目标IP地址
	uiIPAim = 0x00000000;
	// 目标端口号
	usPortAim = HeartBeatRec;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	usCommand = 1;
	// 源IP地址
	memcpy(&m_pFrameData[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pFrameData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &usCommand, 2);

	int iPos = 32;
	// 设置命令字
	m_pFrameData[iPos] = 0x00;
}

// 发送心跳帧
void CHeartBeatSocket::SendHeartBeatFrame(void)
{
	// 发送广播命令帧
	int iCount = SendTo(m_pFrameData, 128, m_uiSendPort, IPBroadcastAddr);
}