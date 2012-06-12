// CSocketInterfaceDataRev.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketInterfaceDataRev.h"
#include "MainFrm.h"


// CSocketInterfaceDataRev

CSocketInterfaceDataRev::CSocketInterfaceDataRev()
{
}

CSocketInterfaceDataRev::~CSocketInterfaceDataRev()
{
}

// CSocketInterfaceDataRev 成员函数

void CSocketInterfaceDataRev::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int iCount = ReceiveFrom(m_oFrameInterface.m_pFrameData, 65536, m_strIPSource, m_uiPortSource);
	if(iCount > 0)
	{
		// 帧字节数
		m_iFrameSize = iCount;
		// 解析帧头
		m_oFrameInterface.ParseFrameHead();
		// 处理内部网络命令帧
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		pMainFrame->OnProcInterFaceCommand(&m_oFrameInterface);
	}

	CSocket::OnReceive(nErrorCode);
}

// 初始化
BOOL CSocketInterfaceDataRev::OnInit(CString strIPForInterface, UINT uiPortForDataReceive)
{
	BOOL bReturn = false;

	// 和内部通讯的本机IP地址
	m_strIPForInterface = strIPForInterface;
	// 用于命令帧的端口号
	m_uiPortForDataReceive = uiPortForDataReceive;
	// 生成网络端口
	bReturn = Create(m_uiPortForDataReceive, SOCK_DGRAM, m_strIPForInterface);

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	return bReturn;	
}

// 设置接收缓冲区大小
BOOL CSocketInterfaceDataRev::SetBufferSize(int iBufferSize)
{
	BOOL bReturn = false;

	int iOptionValue = iBufferSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}
