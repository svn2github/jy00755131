#include "StdAfx.h"
#include "FrameTailTime.h"
#include "Matrixline.h"

CFrameTailTime::CFrameTailTime()
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
	m_iFrameSize = 256;
	Reset();
}

CFrameTailTime::~CFrameTailTime()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameTailTime::Reset()
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

	// 源IP地址
	m_uiIPSource = 0;
	// 目标IP地址
	m_uiIPAim = 0;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 0;

	m_usReceiveTime = 0;	// 16bits时间，接收时刻低位
	m_usSendTime = 0;	// 16bits时间，发送时刻低位

	m_usLineLeftReceiveTime = 0;	// 16bits 测线方向左面尾包接收时刻
	m_usLineRightReceiveTime = 0;	// 16bits 测线方向右面尾包接收时刻

	m_usCrossTopReceiveTime = 0;	// 16bits 交叉线方向上面尾包接收时刻
	m_usCrossDownReceiveTime = 0;	// 16bits 交叉线方下面尾包接收时刻
}

/**
* 解析尾包时刻查询应答帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameTailTime::ParseFrame()
{
	Reset();

	// 源IP地址
	memcpy(&m_uiIPSource, &m_pFrameData[16], 4);
	// 目标IP地址
	memcpy(&m_uiIPAim, &m_pFrameData[20], 4);
	// 目标端口号
	memcpy(&m_usPortAim, &m_pFrameData[24], 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_usCommand, &m_pFrameData[26], 2);

	// 解析命令字
	ParseCommandWord();
//	TRACE1("尾包时刻查询应答帧-仪器IP地址：%d\r\n", m_uiIPSource);
	return true;
}

/**
* 解析命令字
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameTailTime::ParseCommandWord()
{
	byte pData[5];	// 命令字数据
	byte byCommandWord;	// 命令字
	int iMaxCommandWordCount = 9;	// 最多解析多少个命令字
	int iCommandWordCount = 0;	// 命令字计数

	while(true)
	{
		// 得到命令字数据
		memcpy(pData, &m_pFrameData[28 + iCommandWordCount * 5], 5);
		// 命令字
		byCommandWord = pData[0];

		if(0x0 == byCommandWord)	//无命令字
		{
			break;
		}

		if(0x16 == byCommandWord)	// 命令字	0x16接收、发送时刻低位
		{
			// 16bits时间，接收时刻低位
			memcpy(&m_usReceiveTime, &pData[1], 2);
			// 16bits时间，发送时刻低位
			memcpy(&m_usSendTime, &pData[3], 2);
		}
		if(0x1B == byCommandWord)	// 命令字	0x1B接收、发送时刻低位
		{
			// 16bits 测线方向左面尾包接收时刻
			memcpy(&m_usLineLeftReceiveTime, &pData[3], 2);
			// 16bits 测线方向右面尾包接收时刻
			memcpy(&m_usLineRightReceiveTime, &pData[1], 2);
		}
		if(0x1C == byCommandWord)	// 命令字	0x1C接收、发送时刻低位
		{
			// 16bits 交叉线方向上面尾包接收时刻
			memcpy(&m_usCrossTopReceiveTime, &pData[3], 2);
			// 16bits 交叉线方下面尾包接收时刻
			memcpy(&m_usCrossDownReceiveTime, &pData[1], 2);
		}
		if(0x04 == byCommandWord)	// 命令字	0x04接收本地系统时刻 by zl
		{
			
			memcpy(&m_uiTimeSystem, &pData[1], 4);
		}
		if(0x19 == byCommandWord)	// 命令字	0x19接收网络时间
		{
			
			memcpy(&m_uiNetTime, &pData[1], 4);
		}
		if(0x0b == byCommandWord)	// 命令字	0x0b接收网络时间
		{
			m_uiNetOrder=0;
			m_uiNetState=0;
			memcpy(&m_uiNetState, &pData[1], 1);
			memcpy(&m_uiNetOrder, &pData[2], 1);
		}
		if(0x1d == byCommandWord)	// 命令字	0x0b交叉站故障
		{
			
			memcpy(&m_uiNetState, &pData[1], 4);
		}
		if(0x1e == byCommandWord)	// 命令字	0x0b交叉站命令
		{
			;
			//m_uiNetOrder=0;
			//memcpy(&m_uiNetOrder, &pData[1], 2);
		}

		iCommandWordCount++;

		if(iCommandWordCount == iMaxCommandWordCount)	//所有命令字解析完
		{
			break;
		}
	}

	return true;
}

/**
* 生成尾包时刻查询帧
* @param void
* @return void
*/
void CFrameTailTime::MakeFrameData()
{
	// 源IP地址
	memcpy(&m_pFrameData[16], &m_uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &m_uiIPAim, 4);
//	TRACE1("尾包时刻查询帧-仪器IP地址：%d\r\n", m_uiIPAim);
	// 目标端口号
	memcpy(&m_pFrameData[24], &m_usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &m_usCommand, 2);

	int iPos = 32;
	unsigned int i17=PortTimeListen;
	
	if(1 != m_uiInstrumentType){
		i17=PortTimeListen;
		m_pFrameData[iPos] =0x17;
		iPos++;
		memcpy(&m_pFrameData[iPos], &i17, 4);
		iPos = iPos + 4;
	}

	// 命令字	0x16接收、发送时刻低位
	m_pFrameData[iPos] = 0x16;
	iPos++;
	memset(&m_pFrameData[iPos], 0, 4);
	iPos = iPos + 4;
	// 仪器类型 1-交叉站
	if(1 == m_uiInstrumentType)
	{
		// 命令字	0x1B  交叉站大线尾包接收时刻
		m_pFrameData[iPos] = 0x1B;
		iPos++;
		memset(&m_pFrameData[iPos], 0, 4);
		iPos = iPos + 4;
		// 命令字	0x1C  交叉站交叉线尾包接收时刻
		m_pFrameData[iPos] = 0x1C;
		iPos++;
		memset(&m_pFrameData[iPos], 0, 4);
		iPos = iPos + 4;
		// 命令字	0x1D  交叉站故障
		m_pFrameData[iPos] = 0x1D;
		iPos++;
		memset(&m_pFrameData[iPos], 0, 4);
		iPos = iPos + 4;
		// 命令字	0x1E  交叉站命令口故障
		m_pFrameData[iPos] = 0x1E;
		iPos++;
		memset(&m_pFrameData[iPos], 0, 4);
		iPos = iPos + 4;
	}
	// 命令字	0x04接收、发送时刻低位	by zl
	m_pFrameData[iPos] = 0x04;
	iPos++;
	memset(&m_pFrameData[iPos], 0, 4);
	iPos = iPos + 4;
	// 命令字	0x19网络时间	by zl
	m_pFrameData[iPos] = 0x19;
	iPos++;
	memset(&m_pFrameData[iPos], 0, 4);
	iPos = iPos + 4;
	if(1 != m_uiInstrumentType){
		// 命令字	0x0b硬件状态	by zl
		m_pFrameData[iPos] = 0x0b;
		iPos++;
		memset(&m_pFrameData[iPos], 0, 4);
		iPos = iPos + 4;
	}

	// 设置命令字结束
	m_pFrameData[iPos] = 0x00;
}
