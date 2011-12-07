#include "StdAfx.h"
#include "FrameHeartBeat.h"

CFrameHeartBeat::CFrameHeartBeat()
{
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
	// 帧大小
	m_iFrameSize = 128;
	Reset();
}

CFrameHeartBeat::~CFrameHeartBeat()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameHeartBeat::Reset()
{
	// 源IP地址
	m_uiIPSource = 0x00000000;
	// 目标IP地址
	m_uiIPAim = 0x00000000;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 1;
	// 源IP地址
	memcpy(&m_pFrameData[16], &m_uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &m_uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pFrameData[24], &m_usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &m_usCommand, 2);

	int iPos = 32;
	// 设置命令字
	m_pFrameData[iPos] = 0x00;
}