// SocketInterfaceCmdSend.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketInterfaceCmdSend.h"


// CSocketInterfaceCmdSend

CSocketInterfaceCmdSend::CSocketInterfaceCmdSend()
{
	// 收到应答帧的帧计数队列最大长度
	m_iFrameCountListLengthMaxReply = 100;
	// 客户端目标IP地址
	m_strIPAim = "255.255.255.255";
}

CSocketInterfaceCmdSend::~CSocketInterfaceCmdSend()
{
}


// CSocketInterfaceCmdSend 成员函数

void CSocketInterfaceCmdSend::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int iCount = Receive(m_oFrameInterface.m_pFrameData, 65536);
	if(iCount > 0)
	{
		// 帧字节数
		m_iFrameSize = iCount;
		// 解析帧头
		m_oFrameInterface.ParseFrameHead();
		// 保存收到的应答帧
		SaveReplyFrame();
	}
	CSocket::OnReceive(nErrorCode);
}

// 初始化
BOOL CSocketInterfaceCmdSend::OnInit(CString strIPForInterface, UINT uiPortForCommandSend, UINT uiPortAimToServer, UINT uiPortAimToOperation)
{
	BOOL bReturn = false;

	// 和内部通讯的本机IP地址
	m_strIPForInterface = strIPForInterface;
	// 用于命令帧的端口号
	m_uiPortForCommandSend = uiPortForCommandSend;
	// 客户端目标IP端口号，测线服务器
	m_uiPortAimToServer = uiPortAimToServer;
	// 客户端目标IP端口号，施工客户端
	m_uiPortAimToOperation = uiPortAimToOperation;
	// 生成网络端口
	bReturn = Create(m_uiPortForCommandSend, SOCK_DGRAM, m_strIPForInterface);

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	return bReturn;	
}

// 设置缓冲区大小
BOOL CSocketInterfaceCmdSend::SetBufferSize(int iBufferSize)
{
	BOOL bReturn = false;

	int iOptionValue = iBufferSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	iOptionValue = iBufferSize;
	iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

// 发送命令帧
void CSocketInterfaceCmdSend::SendCommandFrame()
{
	int iCount = 0;
	iCount = SendTo(m_oFrameInterface.m_pFrameData, m_oFrameInterface.m_usFrameSize, m_uiPortAimToServer, m_strIPAim);
	iCount = SendTo(m_oFrameInterface.m_pFrameData, m_oFrameInterface.m_usFrameSize, m_uiPortAimToOperation, m_strIPAim);
}

// 保存收到的应答帧
void CSocketInterfaceCmdSend::SaveReplyFrame()
{
	UINT uiFrameCount;
	// 以前收到过
	if(TRUE == m_oFrameCountMapReply.Lookup(m_oFrameInterface.m_uiFrameCount, uiFrameCount))
	{
		return;
	}
	else	// 第一次收到
	{
		// 加入索引表
		m_oFrameCountMapReply.SetAt(m_oFrameInterface.m_uiFrameCount, m_oFrameInterface.m_uiFrameCount);
		// 加入队列
		m_oFrameCountListReply.AddTail(m_oFrameInterface.m_uiFrameCount);
		// 队列过长
		if(m_iFrameCountListLengthMaxReply < m_oFrameCountListReply.GetCount())
		{
			// 移出队列
			uiFrameCount = m_oFrameCountListReply.RemoveHead();
			// 移出索引表
			m_oFrameCountMapReply.RemoveKey(uiFrameCount);
		}
	}
}
