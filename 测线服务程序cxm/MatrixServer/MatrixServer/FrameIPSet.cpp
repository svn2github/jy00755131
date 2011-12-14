#include "StdAfx.h"
#include "FrameIPSet.h"

CFrameIPSet::CFrameIPSet()
{
	Reset();
}

CFrameIPSet::~CFrameIPSet()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameIPSet::Reset()
{
	// 源IP地址
	m_uiIPSource = 0;
	// 目标IP地址
	m_uiIPAim = IPBroadcastAddr;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 0;
	// 仪器SN号
	m_uiSN = 0;
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	m_uiInstrumentType = 0;
	// 仪器本地IP
	m_uiIPInstrument = 0;
	// 路由IP地址，路由方向 1-上
	m_uiRoutIPTop = 0;
	// 路由IP地址，路由方向 2-下
	m_uiRoutIPDown = 0;
	// 路由IP地址，路由方向 3-左
	m_uiRoutIPLeft = 0;
	// 路由IP地址，路由方向 4-右
	m_uiRoutIPRight = 0;
	// 路由IP地址
	m_uiRoutIP = 0;
	// 广播端口
	m_uiBroadCastPort = 0;
	// 路由开关
	m_uiRoutOpen = 0;
}

/**
* 解析IP地址设置应答帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameIPSet::ParseFrame()
{
	unsigned char	byCommandWord = 0;
	int iPos = 28;
	// 重置对象
	Reset();
	while(true)
	{
		memcpy(&byCommandWord, &m_pRcvFrameData[iPos], FrameCmdSize1B);
		iPos += FrameCmdSize1B;
		if (byCommandWord == CmdSn)
		{
			memcpy(&m_uiSN, &m_pRcvFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdLocalIPAddr)
		{
			memcpy(&m_uiIPInstrument, &m_pRcvFrameData[iPos], FramePacketSize4B);
		}
		else if (byCommandWord == CmdEnd)
		{
			break;
		}
		iPos += FramePacketSize4B;
	}
	return true;
}

void CFrameIPSet::MakeFrameData()
{
	memset(m_pSndFrameData, SndFrameBufInit, SndFrameSize);
	m_pSndFrameData[0] = FrameHeadCheck0;
	m_pSndFrameData[1] = FrameHeadCheck1;
	m_pSndFrameData[2] = FrameHeadCheck2;
	m_pSndFrameData[3] = FrameHeadCheck3;
	// 源IP地址
	int iPos = FrameHeadSize;
	memcpy(&m_pSndFrameData[iPos], &m_uiIPSource, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标IP地址
	memcpy(&m_pSndFrameData[iPos], &m_uiIPAim, FramePacketSize4B);
	iPos += FramePacketSize4B;
	// 目标端口号
	memcpy(&m_pSndFrameData[iPos], &m_usPortAim, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pSndFrameData[iPos], &m_usCommand, FramePacketSize2B);
	iPos += FramePacketSize2B;
	// 包时间
	iPos += FramePacketSize4B;
	if (m_usCommand == SendSetCmd)
	{
		// 仪器串号设置命令字
		m_pSndFrameData[iPos] = CmdSn;
		iPos += FrameCmdSize1B;
		// 仪器SN号
		memcpy(&m_pSndFrameData[iPos], &m_uiSN, FramePacketSize4B);
		iPos += FramePacketSize4B;

		// 仪器本地站点IP地址设置命令字
		m_pSndFrameData[iPos] = CmdLocalIPAddr;
		iPos += FrameCmdSize1B;
		// 仪器本地IP
		memcpy(&m_pSndFrameData[iPos], &m_uiIPInstrument, FramePacketSize4B);
		iPos += FramePacketSize4B;
		// 采集站和电源站按照路由地址设置采集站广播端口
		if((m_uiInstrumentType == InstrumentTypeFDU)	
			|| (m_uiInstrumentType == InstrumentTypeLAUL))// 仪器类型 1-交叉站；2-电源站；3-采集站
		{
			// 广播命令
			m_pSndFrameData[iPos] = CmdSetBroadCastPort;
			iPos += FrameCmdSize1B;
			// 设置广播端口    
			memcpy(&m_pSndFrameData[iPos], &m_uiBroadCastPort, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		// LCI和交叉站需要设置路由IP地址
		else
		{
			// 仪器路由地址设置命令字
			m_pSndFrameData[iPos] = CmdLAUXSetRout;
			iPos += FrameCmdSize1B;
			// 路由IP地址，路由方向 4-右
			memcpy(&m_pSndFrameData[iPos], &m_uiRoutIPRight, FramePacketSize4B);
			iPos += FramePacketSize4B;
			// 仪器路由地址设置命令字
			m_pSndFrameData[iPos] = CmdLAUXSetRout;
			iPos += FrameCmdSize1B;
			// 路由IP地址，路由方向 3-左
			memcpy(&m_pSndFrameData[iPos], &m_uiRoutIPLeft, FramePacketSize4B);
			iPos += FramePacketSize4B;
			// 仪器路由地址设置命令字
			m_pSndFrameData[iPos] = CmdLAUXSetRout;
			iPos += FrameCmdSize1B;
			// 路由IP地址，路由方向 1-上
			memcpy(&m_pSndFrameData[iPos], &m_uiRoutIPTop, FramePacketSize4B);
			iPos += FramePacketSize4B;
			// 仪器路由地址设置命令字
			m_pSndFrameData[iPos] = CmdLAUXSetRout;
			iPos += FrameCmdSize1B;
			// 路由IP地址，路由方向 2-下
			memcpy(&m_pSndFrameData[iPos], &m_uiRoutIPDown, FramePacketSize4B);
			iPos += FramePacketSize4B;

			// 如果为交叉站则打开4个方向的路由
			// 仪器路由开关设置命令字
			m_pSndFrameData[iPos] = CmdLAUXRoutOpenSet;
			iPos += FrameCmdSize1B;
			// 路由开关打开
			memcpy(&m_pSndFrameData[iPos], &m_uiRoutOpen, FramePacketSize4B);
			iPos += FramePacketSize4B;
		}
		m_pSndFrameData[iPos] = CmdEnd;
	}
	else if (m_usCommand == SendQueryCmd)
	{
		// 仪器本地站点IP地址设置命令字
		m_pSndFrameData[iPos] = CmdLocalIPAddr;
		iPos += FrameCmdSize1B;
		iPos += FramePacketSize4B;
		m_pSndFrameData[iPos] = CmdEnd;
	}
}