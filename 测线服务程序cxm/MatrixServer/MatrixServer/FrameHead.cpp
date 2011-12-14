#include "StdAfx.h"
#include "FrameHead.h"

CFrameHead::CFrameHead()
{
	Reset();
}

CFrameHead::~CFrameHead()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameHead::Reset()
{
	// 仪器SN号
	m_uiSN = 0;
	// 仪器首包时刻
	m_uiTimeHeadFrame = 0;
	// 路由IP地址
	m_uiRoutIP = 0;
	// 仪器本地时间
	m_uiSysTime = 0;
}

/**
* 解析首包帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameHead::ParseFrame()
{
	unsigned char	byCommandWord = 0;
	// 仪器SN号
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
		else if (byCommandWord == CmdHeadFrameTime)
		{
			memcpy(&m_uiTimeHeadFrame, &m_pFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdLocalSysTime)
		{
			memcpy(&m_uiSysTime, &m_pFrameData[iPos], FramePacketSize4B);
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