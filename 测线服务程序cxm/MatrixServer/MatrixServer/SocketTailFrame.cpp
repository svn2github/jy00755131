// SocketTailFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "SocketTailFrame.h"
#include "Parameter.h"

// CSocketTailFrame

CSocketTailFrame::CSocketTailFrame()
{
}

CSocketTailFrame::~CSocketTailFrame()
{
}

/**
* 初始化
* @param CString strIPForInstrument 信宿IP地址
* @param UINT uiPortForTailFrame	信宿端口号
* @param UINT uiPortForTailFrameSend	用于尾包转发的端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTailFrame::OnInit(CString strIPForInstrument, UINT uiPortForTailFrame, UINT uiPortForTailFrameSend)
{
	BOOL bReturn = false;

	// 和现场仪器通讯的本机IP地址
	m_strIPForInstrument = strIPForInstrument;
	// 用于尾包接收的端口号
	m_uiPortForTailFrame = uiPortForTailFrame;
	// 用于尾包转发的端口号
	m_uiPortForTailFrameSend = uiPortForTailFrameSend;
	// 生成网络端口
	bReturn = Create(m_uiPortForTailFrame, SOCK_DGRAM, m_strIPForInstrument);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
	Bind(m_uiPortForTailFrame,m_strIPForInstrument);

	return bReturn;
}

/**
* 得到网络接收缓冲区收到的帧数量
* @param void
* @return DWORD 帧数量
*/
DWORD CSocketTailFrame::GetFrameCount()
{
	DWORD dwFrameCount = 0;

	// 得到网络接收缓冲区数据字节数
	IOCtl(FIONREAD, &dwFrameCount);
	// 得到帧数量
	dwFrameCount = dwFrameCount / RcvFrameSize;

	return dwFrameCount;
}

/**
* 设置接收缓冲区可以接收的帧数量
* @param int iFrameCount IP帧数量
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketTailFrame::SetBufferSize(int iFrameCount)
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
BOOL CSocketTailFrame::GetFrameData()
{
	BOOL bReturn = false;

	// 得到帧数据
	int iCount = Receive(m_oFrameTail.m_pFrameData, RcvFrameSize);
	if(iCount == RcvFrameSize)	//帧大小正确
	{
		// 解析帧数据
		bReturn = m_oFrameTail.ParseFrame();

// 		//if(0 == m_oFrameTail.m_uiRoutIP)
// 		if(0 != m_oFrameTail.m_uiRoutIP)	//主交叉站尾包时间错误，暂将采集站尾包发给施工	by zl 04.14
// 		{
// 			// 转发尾包帧给施工
// 			SendTo(m_oFrameTail.m_pFrameData, m_iFrameSize, m_uiPortForTailFrameSend, _T("255.255.255.255"));
// 		}
	}
	return bReturn;
}
