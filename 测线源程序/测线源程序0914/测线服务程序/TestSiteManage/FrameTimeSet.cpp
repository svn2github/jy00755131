#include "StdAfx.h"
#include "FrameTimeSet.h"

CFrameTimeSet::CFrameTimeSet()
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

CFrameTimeSet::~CFrameTimeSet()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameTimeSet::Reset()
{
	// 源IP地址
	m_uiIPSource = 0;
	// 目标IP地址
	m_uiIPAim = 0;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 0;
	// 本地时间修正高位
	m_uiTimeHigh = 0;
	// 本地时间修正低位
	m_uiTimeLow = 0;
}

/**
* 解析时间设置应答帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameTimeSet::ParseFrame()
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

	return true;
}

/**
* 解析命令字
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameTimeSet::ParseCommandWord()
{
	byte pData[5];	// 命令字数据
	byte byCommandWord;	// 命令字
	int iMaxCommandWordCount = 4;	// 最多解析多少个命令字
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
		if(0x05 == byCommandWord)	// 本地时间修正高位
		{
			memcpy(&m_uiTimeHigh, &pData[1], 4);
		}
		if(0x06 == byCommandWord)	// 本地时间修正低位
		{
			memcpy(&m_uiTimeLow, &pData[1], 4);
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
* 生成仪器时间地址设置帧
* @param void
* @return void
*/
void CFrameTimeSet::MakeFrameData()
{
	// 源IP地址
	memcpy(&m_pFrameData[16], &m_uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &m_uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pFrameData[24], &m_usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &m_usCommand, 2);

//	TRACE1("时间延迟设置帧-仪器IP地址：%d\r\n", m_uiIPAim);

	int iPos = 32;
	// 本地时间修正高位
	m_pFrameData[iPos] = 0x05;
	iPos++;
	memcpy(&m_pFrameData[iPos], &m_uiTimeHigh, 4);
	iPos = iPos + 4;

	// 本地时间修正高位
	m_pFrameData[iPos] = 0x06;
	iPos++;
	memcpy(&m_pFrameData[iPos], &m_uiTimeLow, 4);
	iPos = iPos + 4;

	// 设置命令字结束
	m_pFrameData[iPos] = 0x00;
}