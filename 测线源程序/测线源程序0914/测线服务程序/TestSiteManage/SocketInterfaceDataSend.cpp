// SocketInterfaceDataSend.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSiteManage.h"
#include "SocketInterfaceDataSend.h"


// CSocketInterfaceDataSend

CSocketInterfaceDataSend::CSocketInterfaceDataSend()
{
	// 客户端目标IP地址
	m_strIPAim = "255.255.255.255";
}

CSocketInterfaceDataSend::~CSocketInterfaceDataSend()
{
}

/**
* 初始化
* @param CString strIPForInterface 信源IP地址
* @param UINT uiPortForCommandSend 信源端口号
* @param UINT uiPortAim 信宿端口号
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CSocketInterfaceDataSend::OnInit(CString strIPForInterface, UINT uiPortForCommandSend, UINT uiPortAim)
{
	BOOL bReturn = false;

	// 和内部通讯的本机IP地址
	m_strIPForInterface = strIPForInterface;
	// 用于命令帧的端口号
	m_uiPortForCommandSend = uiPortForCommandSend;
	// 客户端目标IP端口号
	m_uiPortAim = uiPortAim;
	// 生成网络端口
	bReturn = Create(m_uiPortForCommandSend, SOCK_DGRAM, m_strIPForInterface);

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
BOOL CSocketInterfaceDataSend::SetBufferSize(int iBufferSize)
{
	BOOL bReturn = false;

	int iOptionValue = iBufferSize;
	int iOptionLen = sizeof(int);
	bReturn = SetSockOpt(SO_SNDBUF, (void*)&iOptionValue, iOptionLen, SOL_SOCKET);

	return bReturn;
}

/**
* 发送数据帧
* @param void
* @return void
*/
int CSocketInterfaceDataSend::SendDataFrame()
{
	int iCount = SendTo(m_oFrameInterface.m_pFrameData, m_oFrameInterface.m_usFrameSize, m_uiPortAim, m_strIPAim);
	//TRACE1("测试结果帧发送：帧大小%d\r\n", iCount);
	return iCount;
}

/**
* 生成测试数据帧
* @param unsigned int uiTestType 测试类型
* @param unsigned unsigned int uiDataSize 每个测试数据占用字节数
* @paramunsigned short usDataCount 测试数据个数
* @paramunsigned byte pData 测试数据
* @return void
*/
void CSocketInterfaceDataSend::CreateOutputDataFrameTest(unsigned int uiTestType, unsigned int uiDataSize, unsigned short usDataCount, byte* pData)
{
	m_oFrameInterface.CreateOutputDataFrameTest(uiTestType, uiDataSize, usDataCount, pData);
}