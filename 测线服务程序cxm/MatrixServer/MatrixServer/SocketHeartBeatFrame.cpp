// SocketHeartBeatFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "SocketHeartBeatFrame.h"
#include "Parameter.h"

// CSocketHeartBeatFrame

CSocketHeartBeatFrame::CSocketHeartBeatFrame()
{
}

CSocketHeartBeatFrame::~CSocketHeartBeatFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信源IP地址
* @param UINT uiPortForHeartBeat	信源端口号
* @param CString strIPLCI 信宿IP地址
* @param UINT uiPortLCI	信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketHeartBeatFrame::OnInit(CString strIPForInstrument, UINT uiPortForHeartBeat, CString strIPLCI, UINT uiPortLCI)
{
	BOOL bReturn = false;

	// LCI设备IP地址
	m_strIPLCI = strIPLCI;
	//  LCI设备IP地址端口号
	m_uiPortLCI = uiPortLCI;

	// 和现场仪器通讯的本机IP地址
	m_strIPForInstrument = strIPForInstrument;
	// 用于心跳帧发送的端口号
	m_uiPortForHeartBeat = uiPortForHeartBeat;
	// 生成网络端口
	bReturn = Create(m_uiPortForHeartBeat, SOCK_DGRAM, m_strIPForInstrument);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
	Bind(m_uiPortForHeartBeat,m_strIPForInstrument);
	// 生成心跳发送帧
	m_oFrameHeartBeat.MakeHeartBeatFrame();
	return bReturn;
}

/**
* 发送仪器心跳帧
* @param void
* @return void
*/
void CSocketHeartBeatFrame::SendIHeartBeatFrame()
{
	// 发送心跳帧
	SendTo(m_oFrameHeartBeat.m_pFrameData, SndFrameSize, m_uiPortLCI, m_strIPLCI);
}