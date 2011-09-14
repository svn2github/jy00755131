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
	// 源IP地址
	m_uiIPSource = 0;
	// 目标IP地址
	m_uiIPAim = 0;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 0;
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
bool CFrameTail::ParseCommandWord()
{
	byte pData[5];	// 命令字数据
	byte byCommandWord;	// 命令字
	int iMaxCommandWordCount = 5;	// 最多解析多少个命令字
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
		//判断：仪器本地IP地址命令字
		if(0x03 == byCommandWord)
		{
			// 仪器本地IP地址
			memcpy(&m_uiIPInstrument, &pData[1], 4);
		}

		//判断：串号命令字
		if(0x01 == byCommandWord)
		{
			// 仪器SN号
			memcpy(&m_uiSN, &pData[1], 4);
//			TRACE1("尾包帧-仪器SN号：%d\r\n", m_uiSN);
//			TRACE1("尾包帧-仪器IP地址：%d\r\n", m_uiIPInstrument);
			// 仪器类型 1-交叉站；2-电源站；3-采集站
			m_uiInstrumentType = m_uiSN % 4;
//			m_uiInstrumentType=3;			// temp add by zl 2011.3.15
		}

		//判断：仪器系统时间命令字
		if(0x04 == byCommandWord)
		{
			// 仪器系统时间
			memcpy(&m_uiTimeSystem, &pData[1], 4);
		}

		//判断：仪器路由IP地址命令字
		if(0x3F == byCommandWord)
		{
			// 路由IP地址
			memcpy(&m_uiRoutIP, &pData[1], 4);
//			if(m_uiRoutIP!=0x4)
//				m_uiInstrumentType=1;			// temp add by zl 2011.3.15
		}

		iCommandWordCount++;

		if(iCommandWordCount == iMaxCommandWordCount)	//所有命令字解析完
		{
			break;
		}
	}

	return true;
}