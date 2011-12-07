// SocketTimeSetFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "SocketTimeSetFrame.h"


// CSocketTimeSetFrame

CSocketTimeSetFrame::CSocketTimeSetFrame()
{
	// 帧字节数
	m_iFrameSize = 256;
}

CSocketTimeSetFrame::~CSocketTimeSetFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信源IP地址
* @param UINT uiIPForInstrument 信源IP地址
* @param UINT uiPortForTimeSet	信源端口号
* @param CString strIPLCI 信宿IP地址
* @param UINT uiPortLCI	信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTimeSetFrame::OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForTimeSet, CString strIPLCI, UINT uiPortLCI)
{
	BOOL bReturn = false;

	// LCI设备IP地址
	m_strIPLCI = strIPLCI;
	//  LCI设备IP地址端口号
	m_uiPortLCI = uiPortLCI;

	// 和现场仪器通讯的本机IP地址
	m_strIPForInstrument = strIPForInstrument;
	// 和现场仪器通讯的本机IP地址
	m_uiIPForInstrument = uiIPForInstrument;
	// 用于仪器时间设置的端口号
	m_uiPortForTimeSet = uiPortForTimeSet;
	// 生成网络端口
	bReturn = Create(m_uiPortForTimeSet, SOCK_DGRAM, m_strIPForInstrument);
	Bind(m_uiPortForTimeSet,m_strIPForInstrument);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	return bReturn;
}

/**
* 设置缓冲区可以发送的帧数量
* @param void
* @return DWORD 帧数量
*/
BOOL CSocketTimeSetFrame::SetBufferSize(int iFrameCount)
{
	BOOL bReturn = false;

	int iOptionValue = iFrameCount * m_iFrameSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 生成仪器时间设置帧
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CSocketTimeSetFrame::MakeFrameData(CInstrument* pInstrument)
{
	// 源IP地址
	m_oFrameTimeSet.m_uiIPSource = m_uiIPForInstrument;
	// 目标IP地址
	m_oFrameTimeSet.m_uiIPAim = pInstrument->m_uiIP;
	// 目标端口号
	m_oFrameTimeSet.m_usPortAim = (unsigned short)m_uiPortForTimeSet;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_oFrameTimeSet.m_usCommand = 1;

	// 设置仪器时间信息
	// 本地时间修正高位
	m_oFrameTimeSet.m_uiTimeHigh = pInstrument->m_uiTimeHigh;
	// 本地时间修正低位IP
	m_oFrameTimeSet.m_uiTimeLow = pInstrument->m_uiTimeLow;
	// 生成帧
	m_oFrameTimeSet.MakeFrameData();
}

/**
* 发送仪器时间设置帧
* @param void
* @return void
*/
void CSocketTimeSetFrame::SendTimeSetFrame()
{
	// 发送帧
	int iCount = SendTo(m_oFrameTimeSet.m_pFrameData, 128, m_uiPortLCI, m_strIPLCI);
}
