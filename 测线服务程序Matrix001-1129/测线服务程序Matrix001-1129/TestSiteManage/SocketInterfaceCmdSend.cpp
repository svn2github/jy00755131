// SocketInterfaceCmdSend.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
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

/**
* 响应数据到达消息，接收网络数据
* @param int nErrorCode
* @return void
*/
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

/**
* 初始化
* @param CString strIPForInterface 信源IP地址
* @param UINT uiPortForCommandSend 信源端口号
* @param UINT uiPortAimToLine 信宿端口号，测线
* @param UINT uiPortAimToOperation 信宿端口号，施工
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketInterfaceCmdSend::OnInit(CString strIPForInterface, UINT uiPortForCommandSend, UINT uiPortAimToLine, UINT uiPortAimToOperation)
{
	BOOL bReturn = false;

	// 和内部通讯的本机IP地址
	m_strIPForInterface = strIPForInterface;
	// 用于命令帧的端口号
	m_uiPortForCommandSend = uiPortForCommandSend;
	// 客户端目标IP端口号，测线
	m_uiPortAimToLine = uiPortAimToLine;
	// 客户端目标IP端口号，施工
	m_uiPortAimToOperation = uiPortAimToOperation;
	// 生成网络端口
	bReturn = Create(m_uiPortForCommandSend, SOCK_DGRAM, m_strIPForInterface);
	Bind(m_uiPortForCommandSend,m_strIPForInterface);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	return bReturn;	
}

/**
* 设置发送缓冲区大小
* @param int iBufferSize 缓冲区大小
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketInterfaceCmdSend::SetBufferSize(int iBufferSize)
{
	BOOL bReturn = false;

	int iOptionValue = iBufferSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 发送命令帧
* @param void
* @return void
*/
void CSocketInterfaceCmdSend::SendCommandFrame()
{
	int iCount = 0;
	iCount = SendTo(m_oFrameInterface.m_pFrameData, m_oFrameInterface.m_usFrameSize, m_uiPortAimToLine, m_strIPAim);
	iCount = SendTo(m_oFrameInterface.m_pFrameData, m_oFrameInterface.m_usFrameSize, m_uiPortAimToOperation, m_strIPAim);
	int tmp8=0,tmp9=0;
	memcpy(&tmp8, &m_oFrameInterface.m_pFrameData[8], 1);
	memcpy(&tmp9, &m_oFrameInterface.m_pFrameData[9], 2);
	TRACE3("命令帧字节数%d ,%d,%d\r\n", iCount,tmp8,tmp9);
}

/**
* 保存收到的应答帧
* @param void
* @return void
*/
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
