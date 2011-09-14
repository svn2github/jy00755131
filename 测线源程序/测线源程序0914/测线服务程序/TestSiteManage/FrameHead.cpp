#include "StdAfx.h"
#include "FrameHead.h"

CFrameHead::CFrameHead()
{
	// 帧大小
	m_iFrameSize = 256;
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
	// 源IP地址
	m_uiIPSource = 0;
	// 目标IP地址
	m_uiIPAim = 0;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 0;
	// 仪器SN号
	m_uiSN = 0;
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	m_uiInstrumentType = 0;
	// 仪器首包时刻
	m_uiTimeHeadFrame = 0;
	// 路由IP地址
	m_uiRoutIP = 0;
}

/**
* 解析首包帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameHead::ParseFrame()
{
	// 重置对象
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
bool CFrameHead::ParseCommandWord()
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

		//判断：无命令字
		if(0x0 == byCommandWord)
		{
			break;
		}

		//判断：串号命令字
		if(0x01 == byCommandWord)
		{
			// 仪器SN号
			memcpy(&m_uiSN, &pData[1], 4);
//			TRACE1("首包帧-仪器SN号：%d\r\n", m_uiSN);
			// 仪器类型 1-交叉站；2-电源站；3-采集站
			m_uiInstrumentType = m_uiSN % 4;
//			if(m_uiSN!=268570881)				// temp add by zl 2011.3.15
//				m_uiInstrumentType=3;
		}

		//判断：首包时间命令字
		if(0x02 == byCommandWord)
		{
			// 仪器首包时刻
			memcpy(&m_uiTimeHeadFrame, &pData[1], 4);
//			TRACE1("首包帧-首包时刻：%d\r\n", m_uiTimeHeadFrame);
		}

		//判断：仪器路由IP地址命令字
		if(0x3F == byCommandWord)	//
		{
			// 路由IP地址
			memcpy(&m_uiRoutIP, &pData[1], 4);
		}

		iCommandWordCount++;

		if(iCommandWordCount == iMaxCommandWordCount)	//所有命令字解析完
		{
			break;
		}
	}
	return true;
}

