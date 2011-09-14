// SocketTailTimeFrame.cpp : 实现文件
//

#include "stdafx.h"
// CSocketTailTimeFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "SocketTailTimeFrame.h"


// CSocketTailTimeFrame

CSocketTailTimeFrame::CSocketTailTimeFrame()
{
	// 帧字节数
	m_iFrameSize = 256;
}

CSocketTailTimeFrame::~CSocketTailTimeFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信源IP地址
* @param UINT uiIPForInstrument 信源IP地址
* @param UINT uiPortForTailTime	信源端口号
* @param CString strIPLCI 信宿IP地址
* @param UINT uiPortLCI	信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTailTimeFrame::OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForTailTime, CString strIPLCI, UINT uiPortLCI)
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
	// 用于仪器尾包时刻查询的端口号
	m_uiPortForTailTime = uiPortForTailTime;
	// 生成网络端口
	bReturn = Create(m_uiPortForTailTime, SOCK_DGRAM, m_strIPForInstrument);

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
DWORD CSocketTailTimeFrame::GetFrameCount()
{
	DWORD dwFrameCount = 0;

	// 得到网络接收缓冲区数据字节数
	IOCtl(FIONREAD, &dwFrameCount);
	// 得到帧数量
	dwFrameCount = dwFrameCount / m_iFrameSize;

	return dwFrameCount;
}

/**
* 设置缓冲区可以收发的帧数量
* @param int iFrameCount IP帧数量
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTailTimeFrame::SetBufferSize(int iFrameCount)
{
	BOOL bReturn = false;

	int iOptionValue = iFrameCount * m_iFrameSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	iOptionValue = iFrameCount * m_iFrameSize;
	iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);
	return bReturn;
}

/**
* 得到帧数据
* @param void
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTailTimeFrame::GetFrameData()
{
	BOOL bReturn = false;

	// 得到帧数据
	m_oFrameTailTime.Reset();
	int iCount = Receive(m_oFrameTailTime.m_pFrameData, m_iFrameSize);
	if(iCount == m_iFrameSize)	//帧大小正确
	{
		// 解析帧数据
		bReturn = m_oFrameTailTime.ParseFrame();
	}
	return bReturn;
}

/**
* 生成仪器尾包时刻查询帧
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CSocketTailTimeFrame::MakeFrameData(CInstrument* pInstrument)
{
	m_oFrameTailTime.Reset();
	// 源IP地址
	m_oFrameTailTime.m_uiIPSource = m_uiIPForInstrument;
	// 目标IP地址
	//m_oFrameTailTime.m_uiIPAim = pInstrument->m_uiIP;
	if(pInstrument->m_uiInstrumentType==1)
		m_oFrameTailTime.m_uiIPAim = pInstrument->m_uiIP;
	else
		m_oFrameTailTime.m_uiIPAim = 0xFFFFFFFF;

	// 仪器类型 1-交叉站；2-电源站；3-采集站
	m_oFrameTailTime.m_uiInstrumentType = pInstrument->m_uiInstrumentType;
	// 目标端口号
	m_oFrameTailTime.m_usPortAim = m_uiPortForTailTime;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_oFrameTailTime.m_usCommand = 2;
	// 生成帧
	m_oFrameTailTime.MakeFrameData();
}

/**
* 发送仪器尾包时刻查询帧
* @param void
* @return void
*/
void CSocketTailTimeFrame::SendTailTimeFrame()
{
	// 发送帧
	int iCount = SendTo(m_oFrameTailTime.m_pFrameData, 128, m_uiPortLCI, m_strIPLCI);
}

/**
* 读空网络接收缓冲区
* @param void
* @return void
*/
void CSocketTailTimeFrame::RemoveFrameDataAll()
{
	int iFrameCount = 0;
	iFrameCount = GetFrameCount();
	if(iFrameCount > 0)
	{
		for(int i = 0; i < iFrameCount; i++)
		{
			GetFrameData();
		}		
	}
}

