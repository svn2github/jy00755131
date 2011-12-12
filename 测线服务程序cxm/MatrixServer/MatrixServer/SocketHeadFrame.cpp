// SocketHeadFrame.cpp : 实现文件
//

#include "stdafx.h"

#include "MatrixServer.h"
#include "SocketHeadFrame.h"


// CSocketHeadFrame

CSocketHeadFrame::CSocketHeadFrame()
{
	// 帧字节数
	m_iFrameSize = 256;
}

CSocketHeadFrame::~CSocketHeadFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信宿IP地址
* @param UINT uiPortForHeadFrame	信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketHeadFrame::OnInit(CString strIPForInstrument, UINT uiPortForHeadFrame)
{
	BOOL bReturn = false;

	// 和现场仪器通讯的本机IP地址
	m_strIPForInstrument = strIPForInstrument;
	// 用于首包接收的端口号
	m_uiPortForHeadFrame = uiPortForHeadFrame;
	// 生成网络端口
	bReturn = Create(m_uiPortForHeadFrame, SOCK_DGRAM, m_strIPForInstrument);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
	Bind(uiPortForHeadFrame,m_strIPForInstrument);
	return bReturn;
}

/**
* 得到网络接收缓冲区收到的帧数量
* @param void
* @return DWORD 帧数量
*/
DWORD CSocketHeadFrame::GetFrameCount()
{
	DWORD dwFrameCount = 0;

	// 得到网络接收缓冲区数据字节数
	IOCtl(FIONREAD, &dwFrameCount);
	// 得到帧数量
	dwFrameCount = dwFrameCount / m_iFrameSize;
/*	
	m_oFrameCountHead++;
	if(m_oFrameCountHead<1)
		return dwFrameCount;
	if(m_oFrameCountHead<10)
		return 0;
	else
*/		
	return dwFrameCount;
}

/**
* 设置接收缓冲区可以接收的帧数量
* @param int iFrameCount IP帧数量
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketHeadFrame::SetBufferSize(int iFrameCount)
{
	BOOL bReturn = false;

	int iOptionValue = iFrameCount * m_iFrameSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 得到帧数据
* @param void
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketHeadFrame::GetFrameData()
{
	BOOL bReturn = false;

	// 得到帧数据
	int iCount = Receive(m_oFrameHead.m_pFrameData, m_iFrameSize);
	if(iCount == m_iFrameSize)	//帧大小正确
	{
		// 解析帧数据
		bReturn = m_oFrameHead.ParseFrame();
	}
	return bReturn;
}

/**
* 重发时间靠后的首包
* @param void
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketHeadFrame::SendFrameData()
{
	BOOL bReturn = false;

	int iCount = SendTo(m_oFrameHead.m_pFrameData, 256, m_uiPortForHeadFrame, m_strIPForInstrument);
	return bReturn;
}