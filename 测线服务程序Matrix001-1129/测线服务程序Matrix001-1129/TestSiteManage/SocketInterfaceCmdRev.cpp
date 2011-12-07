// SocketInterface.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "SocketInterfaceCmdRev.h"

// CSocketInterface

CSocketInterfaceCmdRev::CSocketInterfaceCmdRev()
{
	// 收到命令帧的帧计数队列最大长度
	m_iFrameCountListLengthMaxReceive = 100;
}

CSocketInterfaceCmdRev::~CSocketInterfaceCmdRev()
{
}

// CSocketInterface 成员函数

/**
* 响应数据到达消息，接收网络数据
* @param int nErrorCode
* @return void
*/
void CSocketInterfaceCmdRev::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int iCount = ReceiveFrom(m_oFrameInterface.m_pFrameData, 65536, m_strIPSource, m_uiPortSource);
	if(iCount > 0)
	{
		// 帧字节数
		m_iFrameSize = iCount;
		// 解析帧头
		m_oFrameInterface.ParseFrameHead();
		// 发送回令帧
		SendReplyFrame();
		// 检查是否是重复接收到的命令帧
		if(true == CheckIfRepeatFrameCommand())
		{
			return;
		}
		// 解析命令字
		m_oFrameInterface.ParseCommandWord();
		// 处理内部网络命令帧
		m_pNetProcInterface->OnProcInterFaceCommand(&m_oFrameInterface);
	}

	CSocket::OnReceive(nErrorCode);
}

/**
* 初始化
* @param CString strIPForInterface 接收IP地址
* @param UINT uiPortForCommandReceive 接收端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketInterfaceCmdRev::OnInit(CString strIPForInterface, UINT uiPortForCommandReceive)
{
	BOOL bReturn = false;

	// 和内部通讯的本机IP地址
	m_strIPForInterface = strIPForInterface;
	// 用于命令帧的端口号
	m_uiPortForCommandReceive = uiPortForCommandReceive;
	// 生成网络端口
	bReturn = Create(m_uiPortForCommandReceive, SOCK_DGRAM, m_strIPForInterface);
	Bind(uiPortForCommandReceive,m_strIPForInterface);
	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);

	return bReturn;	
}

/**
* 设置接收缓冲区大小
* @param int iBufferSize 缓冲区大小
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketInterfaceCmdRev::SetBufferSize(int iBufferSize)
{
	BOOL bReturn = false;

	int iOptionValue = iBufferSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_RCVBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 发送回令帧
* @param void
* @return void
*/
void CSocketInterfaceCmdRev::SendReplyFrame()
{
	// 0x02-命令帧，要求回令帧
	if(2 == m_oFrameInterface.m_byFrameType)
	{
		m_oFrameReply.MakeReplyFrame(&m_oFrameInterface);
		SendTo(m_oFrameReply.m_pFrameData, m_oFrameReply.m_usFrameSize, m_uiPortSource, m_strIPSource);
	}
}

/**
* 检查是否是重复接收到的帧
* @param int iBufferSize 缓冲区大小
* @return bool true：是；false：否
*/
bool CSocketInterfaceCmdRev::CheckIfRepeatFrameCommand()
{
	bool bReturn = false;
	UINT uiFrameCount;
	// 以前收到过
	if(TRUE == m_oFrameCountMapReceive.Lookup(m_oFrameInterface.m_uiFrameCount, uiFrameCount))
	{
		return true;
	}
	else	// 第一次收到
	{
		// 加入索引表
		m_oFrameCountMapReceive.SetAt(m_oFrameInterface.m_uiFrameCount, m_oFrameInterface.m_uiFrameCount);
		// 加入队列
		m_oFrameCountListReceive.AddTail(m_oFrameInterface.m_uiFrameCount);
		// 队列过长
		if(m_iFrameCountListLengthMaxReceive < m_oFrameCountListReceive.GetCount())
		{
			// 移出队列
			uiFrameCount = m_oFrameCountListReceive.RemoveHead();
			// 移出索引表
			m_oFrameCountMapReceive.RemoveKey(uiFrameCount);
		}
	}
	return bReturn;
}