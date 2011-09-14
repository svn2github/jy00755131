#include "StdAfx.h"
#include "FrameInterface.h"

CFrameInterface::CFrameInterface()
{
	// 数据区
	m_pData = &m_pFrameData[13];
}

CFrameInterface::~CFrameInterface()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameInterface::Reset()
{

}

/**
* 解析帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameInterface::ParseFrame()
{
	Reset();
	// 解析帧头
	ParseFrameHead();
	// 解析命令字
	ParseCommandWord();
	return true;
}

/**
* 解析帧头
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameInterface::ParseFrameHead()
{
	int iPos = 0;
	iPos = iPos + 2;

	// 帧长度
	memcpy(&m_usFrameSize, &m_pFrameData[iPos], 2);
	iPos = iPos + 2;

	// 帧计数
	memcpy(&m_uiFrameCount, &m_pFrameData[iPos], 4);
	iPos = iPos + 4;

	// 帧类型
	m_byFrameType = m_pFrameData[iPos];
	iPos = iPos + 1;

	// 命令
	memcpy(&m_usCommand, &m_pFrameData[iPos], 2);
	iPos = iPos + 2;

	// 命令字个数
	memcpy(&m_usCommandCount, &m_pFrameData[iPos], 2);
	iPos = iPos + 2;

	// 数据区长度
	m_usDataSize = m_usFrameSize - iPos - 3;

	return true;
}

/**
* 解析命令字
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameInterface::ParseCommandWord()
{
	if(0 == m_usCommandCount)
	{
		return true;
	}

	int iPos = 0;

	switch(m_usCommand)
	{
	case 1:	// 命令	1-Field ON/OFF		
		m_byFieldOperation = m_pData[iPos];	// Field ON/OFF
		break;

	case 2:	// 命令	2-FormLine				
		memcpy(&m_uiSN, &m_pData[iPos], 4);	// 仪器设备号
		break;

	case 3:	// 命令	3-Look		
		memcpy(&m_uiLookAuto, &m_pData[iPos], 4);	// 是否自动查找
		iPos = iPos + 4;		
		memcpy(&m_uiTestResistance, &m_pData[iPos], 4);	// 新仪器是否测试Resistance
		iPos = iPos + 4;		
		memcpy(&m_uiTestTilt, &m_pData[iPos], 4);	// 新仪器是否测试Tilt
		iPos = iPos + 4;		
		memcpy(&m_uiTestLeakage, &m_pData[iPos], 4);	// 新仪器是否测试Leakage
		break;

	case 4:	// 命令	4-Test From InstrumentTestSetup
		memcpy(&m_uiTestNb, &m_pData[iPos], 4);	// 测试索引号
		m_uiTestNb = m_uiTestNb + 1000;
		break;

	case 5:	// 命令	5-Test From SensorTestSetup
		memcpy(&m_uiTestNb, &m_pData[iPos], 4);	// 测试索引号
		m_uiTestNb = m_uiTestNb + 2000;
		break;

	case 6:	// 命令	6-Test From MultipleTestSetup
		memcpy(&m_uiTestNb, &m_pData[iPos], 4);	// 测试索引号
		m_uiTestNb = m_uiTestNb + 3000;
		break;

	case 7:	// 命令	7-Test From SeismonitorTestSetup
		iPos = iPos + 1;
		memcpy(&m_uiTestNb, &m_pData[iPos], 4);	// 测试索引号
		break;

	case 8:	// 命令	8-Test From InstrumentView 来自仪器视图的“GO”按钮
	case 9:	// 命令	9-Test From SensorTestView	来自检波器视图的“GO”按钮
		memcpy(&m_uiTestAim, &m_pData[iPos], 4);	// 测试对象	1-仪器；2-检波器
		iPos = iPos + 4;
		memcpy(&m_uiTestType, &m_pData[iPos], 4);	// 测试类型
		iPos = iPos + 4;
		m_strTestAbsoluteSpread = &m_pData[iPos];	// 绝对排列定义
		break;

	case 10:	// 命令	10-Test From SeismonitoView
		m_byTestOperation = m_pData[iPos];	// 1-启动测试；0-停止测试
		iPos = iPos + 1;
		m_strTestAbsoluteSpread = &m_pData[iPos];	// 绝对排列定义
		break;

	case 20:	// 命令	20-测线配置文件更改命令
		memcpy(&m_uiChangeAim, &m_pData[iPos], 4);	// 更改的目标
		break;
	}
	return true;
}

/**
* 生成应答帧
* @param CFrameInterface* m_pFrameInterface 命令帧指针
* @return void
*/
void CFrameInterface::MakeReplyFrame(CFrameInterface* m_pFrameInterface)
{
	m_usFrameSize = 16;
	// 帧头{0xEB90}
	m_pFrameData[0] = 0xEB;
	m_pFrameData[1] = 0x90;
	// 帧长度
	memcpy(&m_pFrameData[2], &m_usFrameSize, 2);
	// 帧计数
	memcpy(&m_pFrameData[4], &m_pFrameInterface->m_uiFrameCount, 4);
	// 帧类型 0xFF-回令帧
	m_pFrameData[8] = 0xFF;
	// 命令
	memcpy(&m_pFrameData[9], &m_pFrameInterface->m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 0;
	memcpy(&m_pFrameData[11], &m_pFrameInterface->m_usCommandCount, 2);
	// 校验位
	m_pFrameData[13] = 0x00;
	// 帧尾{0x146F}
	m_pFrameData[14] = 0x14;
	m_pFrameData[15] = 0x6F;
}

/**
* 生成现场数据输出通知命令帧
* @param void
* @return void
*/
void CFrameInterface::MakeSiteDataOutputCmdFrame()
{
	m_usFrameSize = 16;
	// 帧头{0xEB90}
	m_pFrameData[0] = 0xEB;
	m_pFrameData[1] = 0x90;
	// 帧长度
	memcpy(&m_pFrameData[2], &m_usFrameSize, 2);
	// 帧计数
	UINT iNow = GetTickCount();
	memcpy(&m_pFrameData[4], &iNow, 4);
	// 帧类型 0x01-命令帧，不要求回令帧
	m_pFrameData[8] = 0x01;
	// 命令
	m_usCommand = 100;
	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 0;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}

/**
* 生成ServerField On/Off帧
* @param void
* @return void
*/
void CFrameInterface::MakeServerFieldFrame()
{
	m_usFrameSize = 17;
	// 帧头{0xEB90}
	m_pFrameData[0] = 0xEB;
	m_pFrameData[1] = 0x90;
	// 帧长度
	memcpy(&m_pFrameData[2], &m_usFrameSize, 2);
	// 帧计数
	UINT iNow = GetTickCount();
	memcpy(&m_pFrameData[4], &iNow, 4);
	// 帧类型 0x01-命令帧，不要求回令帧
	m_pFrameData[8] = 0x01;
	// 命令 101-Server Field ON/OFF
	m_usCommand = 101;
	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 1;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	m_pFrameData[iPos] = m_byFieldOperation;
	iPos = iPos + 1;
	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}

/**
* 生成测试数据帧
* @param unsigned int uiTestType 测试类型
* @param unsigned unsigned int uiDataSize 每个测试数据占用字节数
* @paramunsigned short usDataCount 测试数据个数
* @paramunsigned byte pData 测试数据
* @return void
*/
void CFrameInterface::CreateOutputDataFrameTest(unsigned int uiTestType, unsigned int uiDataSize, unsigned short usDataCount, byte* pData)
{
	unsigned short usDataLength = uiDataSize * usDataCount;
	m_usFrameSize = 16 + usDataLength;
	// 帧头{0xEB90}
	m_pFrameData[0] = 0xEB;
	m_pFrameData[1] = 0x90;
	// 帧长度
	memcpy(&m_pFrameData[2], &m_usFrameSize, 2);
	// 帧计数
	UINT iNow = GetTickCount();
	memcpy(&m_pFrameData[4], &iNow, 4);
	// 帧类型 0x03-数据帧，不要求回令帧
	m_pFrameData[8] = 0x03;
	// 命令
	m_usCommand = 200 + (unsigned short)uiTestType;
	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = usDataCount;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	// 测试数据
	memcpy(&m_pFrameData[iPos], pData, usDataLength);
	iPos = iPos + usDataLength;
	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}