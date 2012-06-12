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
* 解析帧头
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameInterface::ParseFrameHead()
{
	unsigned short iPos = 0;
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
	case 101:	// 命令 101-Server Field ON/OFF
		m_byFieldOperation = m_pData[iPos];	// Field ON/OFF
		break;
	}
	return true;
}

/**
* 生成应答帧
* @param CFrameInterface* m_pFrameInterface 收到帧
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
* @param unsigned int uiChangeAim 输出目标
* @return void
*/
void CFrameInterface::MakeSiteDataOutputCmdFrame(unsigned int uiChangeAim)
{
	m_uiChangeAim = uiChangeAim;

	m_usFrameSize = 20;
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
	m_usCommand = 20;
	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 1;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	memcpy(&m_pFrameData[iPos], &m_uiChangeAim, 4);
	iPos = iPos + 4;
	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}

/**
* 生成Field On/Off帧
* @param void
* @return void
*/
void CFrameInterface::MakeFieldFrame()
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
	// 命令	1-Field ON/OFF
	m_usCommand = 1;
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
* 生成FormLine帧
* @param unsigned int uiSN 仪器SN号
* @return void
*/
void CFrameInterface::MakeFormLineFrame(unsigned int uiSN)
{
	m_usFrameSize = 20;
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
	m_usCommand = 2;
	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 1;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	memcpy(&m_pFrameData[iPos], &uiSN, 4);
	iPos = iPos + 4;

	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}

/**
* 生成测试命令帧，测试设置对话框GO按钮
* @param unsigned short usCommand 命令 4-Test From InstrumentTestSetup；5-Test From SensorTestSetup；6-Test From MultipleTestSetup；7-Test From SeismonitorTestSetup
* @param UINT uiTestNb 测试索引号
* @return void
*/
void CFrameInterface::MakeTestCmdFrameForTestSetup(unsigned short usCommand, UINT uiTestNb)
{
	m_usFrameSize = 20;
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
	m_usCommand = usCommand;
	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 1;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	memcpy(&m_pFrameData[iPos], &uiTestNb, 4);
	iPos = iPos + 4;
	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}

/**
* 生成测试命令帧，响应仪器、检波器图形视图测试GO按钮
* @param unsigned unsigned int uiTestAim 测试对象	1-仪器；2-检波器
* @param unsigned unsigned int uiTestType 测试类型
* @param unsigned CString strTestAbsoluteSpread 绝对排列定义
* @return void
*/
void CFrameInterface::MakeTestCmdFrameForGraphViewGOButton(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread)
{	
	int iSizeAbsoluteSpread = strTestAbsoluteSpread.GetLength();

	m_usFrameSize = 26 + (unsigned short)iSizeAbsoluteSpread;
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
	if(1 == uiTestAim)
	{
		// 命令
		m_usCommand = 8;	// 命令	8-Test From InstrumentView 来自仪器视图的“GO”按钮
	}
	else if(2 == uiTestAim)
	{
		// 命令
		m_usCommand = 9;	// 命令	9-Test From SensorTestView	来自检波器视图的“GO”按钮
	}

	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 1;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	memcpy(&m_pFrameData[iPos], &uiTestAim, 4);
	iPos = iPos + 4;
	memcpy(&m_pFrameData[iPos], &uiTestType, 4);
	iPos = iPos + 4;
	memcpy(&m_pFrameData[iPos], strTestAbsoluteSpread.GetBuffer(), iSizeAbsoluteSpread);
	iPos = iPos + iSizeAbsoluteSpread;
	m_pFrameData[iPos] = 0;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0;
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
* 生成测试命令帧，响应噪声监测图形视图Seismonitor按钮
* @param byte byTestOperation 操作 1-启动测试；0-停止测试
* @param unsigned CString strTestAbsoluteSpread 绝对排列定义
* @return void
*/
void CFrameInterface::MakeTestCmdFrameForGraphViewSeismonitorButton(byte byTestOperation, CString strTestAbsoluteSpread)
{	
	int iSizeAbsoluteSpread = strTestAbsoluteSpread.GetLength();

	m_usFrameSize = 19 + (unsigned short)iSizeAbsoluteSpread;
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
	m_usCommand = 10;	// 命令	10-Test From SeismonitorView	来自噪声监测器视图的“Seismonitor”按钮

	memcpy(&m_pFrameData[9], &m_usCommand, 2);
	// 命令字个数
	m_usCommandCount = 1;
	memcpy(&m_pFrameData[11], &m_usCommandCount, 2);

	int iPos = 13;
	m_pFrameData[iPos] = byTestOperation;
	iPos = iPos + 1;

	memcpy(&m_pFrameData[iPos], strTestAbsoluteSpread.GetBuffer(), iSizeAbsoluteSpread);
	iPos = iPos + iSizeAbsoluteSpread;
	m_pFrameData[iPos] = 0;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0;
	iPos = iPos + 1;
	// 校验位
	m_pFrameData[iPos] = 0x00;
	iPos = iPos + 1;
	// 帧尾{0x146F}
	m_pFrameData[iPos] = 0x14;
	iPos = iPos + 1;
	m_pFrameData[iPos] = 0x6F;
}