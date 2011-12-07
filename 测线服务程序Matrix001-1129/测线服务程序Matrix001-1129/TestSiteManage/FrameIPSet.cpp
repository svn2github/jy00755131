#include "StdAfx.h"
#include "FrameIPSet.h"
#include "Matrixline.h"

CFrameIPSet::CFrameIPSet()
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
	m_uiIPAim = 0xFFFFFFFF;
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
}

/**
* 解析IP地址设置应答帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameIPSet::ParseFrame()
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

//	TRACE1("IP地址设置应答帧-仪器SN号：%d\r\n", m_uiSN);
//	TRACE1("IP地址设置应答帧-仪器IP地址：%d\r\n", m_uiIPInstrument);

	return true;
}

/**
* 解析命令字
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameIPSet::ParseCommandWord()
{
	byte pData[5];	// 命令字数据
	byte byCommandWord;	// 命令字
	int iMaxCommandWordCount = 8;	// 最多解析多少个命令字
	int iCommandWordCount = 0;	// 命令字计数
	int i0x1FCount = 0;	// 0x1F命令字计数

	while(true)
	{
		// 得到命令字数据
		memcpy(pData, &m_pFrameData[28 + iCommandWordCount * 5], 5);
		// 命令字
		byCommandWord = pData[0];

		// 判断：无命令字
		if(0x0 == byCommandWord)
		{
			break;
		}

		// 判断：串号命令字
		if(0x01 == byCommandWord)
		{
			// 仪器SN号
			memcpy(&m_uiSN, &pData[1], 4);
			// 仪器类型 1-交叉站；2-电源站；3-采集站
			m_uiInstrumentType = m_uiSN % 4;
		}

		// 判断：仪器本地IP命令字
		if(0x03 == byCommandWord)
		{
			// 仪器本地IP
			memcpy(&m_uiIPInstrument, &pData[1], 4);
		}

		// 判断：仪器路由IP地址设置命令字
		if(0x1F == byCommandWord)
		{
			i0x1FCount++;
			switch(i0x1FCount)
			{
			case 1:
				// 路由IP地址，路由方向 1上
				memcpy(&m_uiRoutIPTop, &pData[1], 4);
				break;
			case 2:
				// 路由IP地址，路由方向 2下
				memcpy(&m_uiRoutIPDown, &pData[1], 4);
				break;
			case 3:
				// 路由IP地址，路由方向 3左
				memcpy(&m_uiRoutIPLeft, &pData[1], 4);
				break;
			case 4:
				// 路由IP地址，路由方向 4右
				memcpy(&m_uiRoutIPRight, &pData[1], 4);
				break;
			}
		}

		// 判断：仪器路由IP地址命令字
		if(0x3F == byCommandWord)
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

/**
* 生成仪器IP地址设置帧
* @param void
* @return void
*/
/*
void CFrameIPSet::MakeFrameData()
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
	memcpy(&m_pFrameData[16], &m_uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &m_uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pFrameData[24], &m_usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &m_usCommand, 2);

//	TRACE1("IP地址设置帧-仪器SN号：%d\r\n", m_uiSN);
//	TRACE1("IP地址设置帧-仪器IP地址：%d\r\n", m_uiIPInstrument);

	int iPos = 32;
	// 仪器串号设置命令字
	m_pFrameData[iPos] = 0x01;
	iPos++;
	// 仪器SN号
	memcpy(&m_pFrameData[iPos], &m_uiSN, 4);
	iPos = iPos + 4;

	// 仪器本地站点IP地址设置命令字
	m_pFrameData[iPos] = 0x03;
	iPos++;
	// 仪器本地IP
	memcpy(&m_pFrameData[iPos], &m_uiIPInstrument, 4);
	iPos = iPos + 4;

	// 判断：仪器类型 1-交叉站 需要设置路由IP地址
	if(1 == m_uiInstrumentType)
	{
		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 3-左
		//memcpy(&m_pFrameData[iPos], &m_uiRoutIPLeft, 4);
		int itmp=0x33333333;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 4-右
		//memcpy(&m_pFrameData[iPos], &m_uiRoutIPRight, 4);
		itmp=0x44444444;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 1-上
		//memcpy(&m_pFrameData[iPos], &m_uiRoutIPTop, 4);
		itmp=0x11111111;					//by zl 04.12
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 2-下
		//memcpy(&m_pFrameData[iPos], &m_uiRoutIPDown, 4);
		itmp=0x22222222;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

	}else
	{
		// 设置广播端口
		m_pFrameData[iPos] = 0x0a;
		iPos++;
		// 广播命令，端口0x5000
		int intPortADListen=PortADListen;
		memcpy(&m_pFrameData[iPos], &intPortADListen, 4);
		iPos = iPos + 4;
	}

	// 设置命令字结束
	m_pFrameData[iPos] = 0x00;
}
*/

void CFrameIPSet::MakeFrameData()
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
	memcpy(&m_pFrameData[16], &m_uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &m_uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pFrameData[24], &m_usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &m_usCommand, 2);

//	TRACE1("IP地址设置帧-仪器SN号：%d\r\n", m_uiSN);
//	TRACE1("IP地址设置帧-仪器IP地址：%d\r\n", m_uiIPInstrument);

	int iPos = 32;
	// 仪器串号设置命令字
	m_pFrameData[iPos] = 0x01;
	iPos++;
	// 仪器SN号
	memcpy(&m_pFrameData[iPos], &m_uiSN, 4);
	iPos = iPos + 4;

	// 仪器本地站点IP地址设置命令字
	m_pFrameData[iPos] = 0x03;
	iPos++;
	// 仪器本地IP
	memcpy(&m_pFrameData[iPos], &m_uiIPInstrument, 4);
	iPos = iPos + 4;

	// 判断：仪器类型 1-交叉站 需要设置路由IP地址
	if(1 == m_uiInstrumentType)
	{
/*		//新交叉站次序为0x11111111-0x44444444，旧交叉站次序为0x33333333-44,11-22
		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 1-上
		//memcpy(&m_pFrameData[iPos], &m_uiRoutIPTop, 4);
		int itmp=0x11111111;					//by zl 04.12
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 2-下
		//memcpy(&m_pFrameData[iPos], &m_uiRoutIPDown, 4);
		itmp=0x22222222;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;
*/
		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 3-左
		memcpy(&m_pFrameData[iPos], &m_uiRoutIPLeft, 4);
		//int itmp=0x33333333;
		int itmp=0x44444444;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 4-右
		memcpy(&m_pFrameData[iPos], &m_uiRoutIPRight, 4);
		//itmp=0x44444444;
		itmp=0x33333333;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

	// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 1-上
		memcpy(&m_pFrameData[iPos], &m_uiRoutIPTop, 4);
		itmp=0x11111111;					//by zl 04.12
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 仪器本地站点路由IP地址设置命令字
		m_pFrameData[iPos] = 0x1F;
		iPos++;
		// 路由IP地址，路由方向 2-下
		memcpy(&m_pFrameData[iPos], &m_uiRoutIPDown, 4);
		itmp=0x22222222;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;

		// 设置路由开关
		m_pFrameData[iPos] = 0x0F;
		iPos++;
		itmp=0;
		memcpy(&m_pFrameData[iPos], &itmp, 4);
		iPos = iPos + 4;
	}else
	{
		// 设置广播端口
		m_pFrameData[iPos] = 0x0a;
		iPos++;
		// 广播命令，端口0x5000
		int intPortADListen=PortADListen;
		memcpy(&m_pFrameData[iPos], &intPortADListen, 4);
		iPos = iPos + 4;
/*
		// 设置广播端口
		m_pFrameData[iPos] = 0x0a;
		iPos++;
		// 广播命令，端口0x5000
		intPortADListen=PortTimeListen;
		memcpy(&m_pFrameData[iPos], &intPortADListen, 4);
		iPos = iPos + 4;
*/		}

	// 设置命令字结束
	m_pFrameData[iPos] = 0x00;
}