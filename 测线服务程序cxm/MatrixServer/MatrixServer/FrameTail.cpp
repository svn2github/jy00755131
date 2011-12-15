#include "StdAfx.h"
#include "FrameTail.h"

CFrameTail::CFrameTail()
{
}

CFrameTail::~CFrameTail()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameTail::Reset()
{
	// 仪器本地IP地址
	m_uiIPInstrument = 0;
	// 仪器SN号
	m_uiSN = 0;
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	m_uiInstrumentType = 0;
	// 仪器系统时间
	m_uiTimeSystem = 0;
	// 路由IP地址，路由方向 1-上；2-下；3-左；4-右
	m_uiRoutIP = 0;
}

/**
* 解析尾包帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameTail::ParseFrame()
{
	unsigned char	byCommandWord = 0;
	int iPos = 28;
	// 重置对象
	Reset();
	while(true)
	{
		memcpy(&byCommandWord, &m_pFrameData[iPos], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (byCommandWord == CmdSn)
		{
			memcpy(&m_uiSN, &m_pFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdLocalIPAddr)
		{
			memcpy(&m_uiIPInstrument, &m_pFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdLocalSysTime)
		{
			memcpy(&m_uiTimeSystem, &m_pFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdReturnRout)
		{
			memcpy(&m_uiRoutIP, &m_pFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdEnd)
		{
			break;
		}
		iPos += FramePacketSize4B;
	}
	return true;
}