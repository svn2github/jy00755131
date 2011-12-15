// SocketIPSetFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "SocketIPSetFrame.h"
#include "..\\parameter\\Parameter.h"

// CSocketIPSetFrame

CSocketIPSetFrame::CSocketIPSetFrame()
{
}

CSocketIPSetFrame::~CSocketIPSetFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信源IP地址
* @param UINT uiIPForInstrument 信源IP地址
* @param UINT uiPortForIPSet	信源端口号
* @param CString strIPLCI 信宿IP地址
* @param UINT uiPortLCI	信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketIPSetFrame::OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForIPSet, CString strIPLCI, UINT uiPortLCI)
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
	// 用于仪器IP地址设置的端口号
	m_uiPortForIPSet = uiPortForIPSet;
	// 生成网络端口
	bReturn = Create(m_uiPortForIPSet, SOCK_DGRAM, m_strIPForInstrument);
	Bind(m_uiPortForIPSet,m_strIPForInstrument);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	return bReturn;
}

/**
* 得到网络接收缓冲区收到的帧数量
* @param void
* @return DWORD 帧数量
*/
DWORD CSocketIPSetFrame::GetFrameCount()
{
	DWORD dwFrameCount = 0;

	// 得到网络接收缓冲区数据字节数
	IOCtl(FIONREAD, &dwFrameCount);
	// 得到帧数量
	dwFrameCount = dwFrameCount / RcvFrameSize;

	return dwFrameCount;
}

/**
* 设置缓冲区可以收发的帧数量
* @param int iFrameCount IP帧数量
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketIPSetFrame::SetBufferSize(int iFrameCount)
{
	BOOL bReturn = false;

	int iOptionValue = iFrameCount * RcvFrameSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	iOptionValue = iFrameCount * SndFrameSize;
	iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 得到帧数据
* @param void
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketIPSetFrame::GetFrameData()
{
	BOOL bReturn = false;

	// 得到帧数据
	int iCount = Receive(m_oFrameIPSet.m_pRcvFrameData, RcvFrameSize);
	if(iCount == RcvFrameSize)	//帧大小正确
	{
		// 解析帧数据
		bReturn = m_oFrameIPSet.ParseFrame();
	}
	return bReturn;
}

/**
* 生成仪器IP地址设置帧
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CSocketIPSetFrame::MakeFrameData(CInstrument* pInstrument, unsigned short usCommand)
{
	// 源IP地址
	m_oFrameIPSet.m_uiIPSource = m_uiIPForInstrument;
	// 目标IP地址
	if (usCommand == SendSetCmd)
	{
		m_oFrameIPSet.m_uiIPAim = IPBroadcastAddr;
	}
	else if (usCommand == SendQueryCmd)
	{
		m_oFrameIPSet.m_uiIPAim = pInstrument->m_uiIP;
	}
	// 目标端口号
	m_oFrameIPSet.m_usPortAim = (unsigned short)m_uiPortForIPSet;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_oFrameIPSet.m_usCommand = usCommand;
	// 设置仪器IP地址信息
	// 仪器类型 1-交叉站；2-电源站；3-采集站
	m_oFrameIPSet.m_uiInstrumentType = pInstrument->m_uiInstrumentType;
	// 仪器SN号
	m_oFrameIPSet.m_uiSN = pInstrument->m_uiSN;
	// 仪器本地IP
	m_oFrameIPSet.m_uiIPInstrument = pInstrument->m_uiIP;
	// 路由IP地址，路由方向 1-上
	m_oFrameIPSet.m_uiRoutIPTop = pInstrument->m_uiRoutIPTop;
	// 路由IP地址，路由方向 2-下
	m_oFrameIPSet.m_uiRoutIPDown = pInstrument->m_uiRoutIPDown;
	// 路由IP地址，路由方向 3-左
	m_oFrameIPSet.m_uiRoutIPLeft = pInstrument->m_uiRoutIPLeft;
	// 路由IP地址，路由方向 4-右
	m_oFrameIPSet.m_uiRoutIPRight = pInstrument->m_uiRoutIPRight;
	// 广播端口
	m_oFrameIPSet.m_uiBroadCastPort = pInstrument->m_uiBroadCastPort;
	// 生成帧
	m_oFrameIPSet.MakeFrameData();
}

/**
* 发送仪器IP地址设置帧
* @param void
* @return void
*/
void CSocketIPSetFrame::SendIPSetFrame()
{
	// 发送帧
	SendTo(m_oFrameIPSet.m_pSndFrameData, SndFrameSize, m_uiPortLCI, m_strIPLCI);
}