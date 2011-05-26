#include "StdAfx.h"
#include "ThreadManage.h"

CThreadManage::CThreadManage(void)
{
}

CThreadManage::~CThreadManage(void)
{
}

// 初始化
void CThreadManage::OnInit(void)
{
	m_oInstrumentList.OnInit();
	m_oHeadFrameThread.m_oInstrumentList = &m_oInstrumentList;
	m_oTailFrameThread.m_oInstrumentList = &m_oInstrumentList; 

	m_oHeartBeatThread.OnInit();
	m_oHeartBeatThread.CreateThread();
	m_oHeartBeatThread.SuspendThread();

	m_oADDataThread.OnInit();
	m_oADDataThread.CreateThread();
	m_oADDataThread.SuspendThread();

	m_oHeadFrameThread.OnInit();
	m_oHeadFrameThread.CreateThread();
	m_oHeadFrameThread.SuspendThread();

	m_oTailFrameThread.OnInit();
	m_oTailFrameThread.CreateThread();
	m_oTailFrameThread.SuspendThread();
}

// 关闭并结束线程
void CThreadManage::OnClose(void)
{
	int iResult = 0;
	m_oHeartBeatThread.OnClose();
	iResult = ::WaitForSingleObject(m_oHeartBeatThread.m_hHeartBeatThreadClose,300);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("心跳线程在300ms内未能正常结束！"));
	}
	m_oHeadFrameThread.OnClose();
	iResult = ::WaitForSingleObject(m_oHeadFrameThread.m_hHeadFrameThreadClose,300);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("首包线程在300ms内未能正常结束！"));
	}
	m_oADDataThread.OnClose();
	iResult = ::WaitForSingleObject(m_oADDataThread.m_hADDDataThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("数据采集线程在100ms内未能正常结束！"));
	}
	m_oTailFrameThread.OnClose();
	iResult = ::WaitForSingleObject(m_oTailFrameThread.m_hTailFrameThreadClose,300);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("尾包线程在300ms内未能正常结束！"));
	}
	m_oInstrumentList.OnClose();

}

// 打开
void CThreadManage::OnOpen(void)
{
	m_oInstrumentList.OnInit();
	m_oInstrumentList.OnOpen();
	m_oHeartBeatThread.ResumeThread();
	m_oHeartBeatThread.OnOpen();

	m_oADDataThread.ResumeThread();
	m_oADDataThread.OnOpen();

	m_oHeadFrameThread.ResumeThread();
	m_oHeadFrameThread.OnOpen();

	m_oTailFrameThread.ResumeThread();
	m_oTailFrameThread.OnOpen();
	
	OnCreateCollectSysTimeBroadcastSocket();
}

// 停止
void CThreadManage::OnStop(void)
{
	m_oInstrumentList.OnStop();

	m_oHeartBeatThread.SuspendThread();
	m_oHeartBeatThread.OnStop();

	m_oADDataThread.SuspendThread();
	m_oADDataThread.OnStop();

	m_oHeadFrameThread.SuspendThread();
	m_oHeadFrameThread.OnStop();

	m_oTailFrameThread.SuspendThread();
	m_oTailFrameThread.OnStop();

//	m_oSysTimeSocket.ShutDown(2);
	m_oSysTimeSocket.Close();
}

// 创建查询采集站本地时间的广播端口
void CThreadManage::OnCreateCollectSysTimeBroadcastSocket(void)
{
	m_oSysTimeSocket.Close();
	BOOL bReturn = FALSE;
	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	bReturn = m_oSysTimeSocket.Create(CollectSysTimePort, SOCK_DGRAM);
	if (bReturn == FALSE)
	{
		AfxMessageBox(_T("命令广播端口创建失败！"));
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_oSysTimeSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
}

// 生成采集站本地时间查询帧
void CThreadManage::MakeCollectSysTimeFrameData(unsigned int uiTestNb)
{
	unsigned int uiIPSource =	0;
	unsigned int uiIPAim	=	0;
	unsigned int usPortAim	=	0;
	unsigned int usCommand	=	0;
	m_cCollectSysTimeSendData[0] = 0x11;
	m_cCollectSysTimeSendData[1] = 0x22;
	m_cCollectSysTimeSendData[2] = 0x33;
	m_cCollectSysTimeSendData[3] = 0x44;
	m_cCollectSysTimeSendData[4] = 0x00;
	m_cCollectSysTimeSendData[5] = 0x00;
	m_cCollectSysTimeSendData[6] = 0x00;
	m_cCollectSysTimeSendData[7] = 0x00;
	m_cCollectSysTimeSendData[8] = 0x00;
	m_cCollectSysTimeSendData[9] = 0x00;
	m_cCollectSysTimeSendData[10] = 0x00;
	m_cCollectSysTimeSendData[11] = 0x00;
	m_cCollectSysTimeSendData[12] = 0x00;
	m_cCollectSysTimeSendData[13] = 0x00;
	m_cCollectSysTimeSendData[14] = 0x00;
	m_cCollectSysTimeSendData[15] = 0x00;
	
	CString str;
	str = m_oSysTimeSocket.m_csIPSource;
	uiIPSource	=	inet_addr(str);
	for (int i=0; i<GraphViewNum; i++)
	{
		if ((uiTestNb & (0x01<<i)) != 0)
		{
			uiIPAim	= 111 - 10*i;
			break;
		}
	}
	/*	uiIPAim = 111;*/
	usPortAim	=	CollectSysTimePort;
	usCommand	=	2;
	// 源IP地址
	memcpy(&m_cCollectSysTimeSendData[16], &uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_cCollectSysTimeSendData[20], &uiIPAim, 4);
	TRACE1("尾包时刻查询帧-仪器IP地址：%d\r\n", uiIPAim);
	// 目标端口号
	memcpy(&m_cCollectSysTimeSendData[24], &usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_cCollectSysTimeSendData[26], &usCommand, 2);

	int iPos = 32;

	// 命令字0x04读取本地系统时间
	m_cCollectSysTimeSendData[iPos] = 0x04;
	iPos++;
	memset(&m_cCollectSysTimeSendData[iPos], 0, 4);
	iPos = iPos + 4;

	// 设置命令字结束
	m_cCollectSysTimeSendData[iPos] = 0x00;

}

// 发送采集站本地时间查询帧
void CThreadManage::SendCollectSysTimeFrameToSocket(void)
{
	// 发送帧
	int iCount = m_oSysTimeSocket.SendTo(m_cCollectSysTimeSendData, 128, m_oSysTimeSocket.m_uiSendPort, IPBroadcastAddr);
}
