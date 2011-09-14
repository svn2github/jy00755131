#include "StdAfx.h"
#include "FrameTestData.h"

CFrameTestData::CFrameTestData()
{
	// 帧大小
	m_iFrameSize = 256;
	// 测试数据
	m_pData = &m_pFrameData[30];
	Reset();
}

CFrameTestData::~CFrameTestData()
{
}

/**
* 重置对象
* @param void
* @return void
*/
void CFrameTestData::Reset()
{
	// 源IP地址
	m_uiIPSource = 0;
	// 目标IP地址
	m_uiIPAim = 0;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 0;

	// 数据索引
	m_usDataIndex = 0;
}

/**
* 解析测试数据帧
* @param void
* @return bool true：成功；false：失败
*/
bool CFrameTestData::ParseFrame()
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
	// 数据索引
	memcpy(&m_usDataIndex, &m_pFrameData[28], 2);
//	TRACE1("测试数据帧-仪器IP地址：%d\r\n", m_uiIPSource);
//	TRACE1("测试数据帧-数据索引：%d\r\n", m_usDataIndex);
	//if(m_usCommand==0x3 && m_usPortAim==0x5000)
	if(m_usCommand==0x3 )
		return true;
	else
		return false;
}