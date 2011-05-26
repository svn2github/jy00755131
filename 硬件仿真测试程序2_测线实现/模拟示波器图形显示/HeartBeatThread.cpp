// HeartBeatThread.cpp : 实现文件
//

#include "stdafx.h"
#include "模拟示波器图形显示.h"
#include "HeartBeatThread.h"
#include "Parameter.h"

// CHeartBeatThread

IMPLEMENT_DYNCREATE(CHeartBeatThread, CWinThread)

CHeartBeatThread::CHeartBeatThread()
: m_bclose(false)
{
}

CHeartBeatThread::~CHeartBeatThread()
{
}

BOOL CHeartBeatThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CHeartBeatThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CHeartBeatThread, CWinThread)
END_MESSAGE_MAP()

// 初始化
void CHeartBeatThread::OnInit(void)
{
	BOOL bReturn = FALSE;
	// 生成网络端口，接收发送命令应答帧，create函数写入第三个参数IP地址则接收固定IP地址发送的帧，不写则全网接收
	bReturn = m_HeartBeatSocket.Create(HeartBeatRec, SOCK_DGRAM);
	if (bReturn == FALSE)
	{
		AfxMessageBox(_T("命令广播端口创建失败！"));
	}

	//设置广播模式
	int	iOptval, iOptlen;
	iOptlen = sizeof(int);
	iOptval = 1;
	m_HeartBeatSocket.SetSockOpt(SO_BROADCAST, &iOptval, iOptlen, SOL_SOCKET);
	MakeHeartBeatFrame();
}
// 线程函数在Run中执行
int CHeartBeatThread::Run()
{
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
		// 发送心跳帧
		SendHeartBeatFrame();
		if (m_bclose == true)
		{
			break;
		}
		Sleep(50);
	}

	// 返回
	return CWinThread::Run();
}

// 生成心跳帧
void CHeartBeatThread::MakeHeartBeatFrame(void)
{
	m_iFrameSize = 128;

	m_pFrameData[0] = 0x11;
	m_pFrameData[1] = 0x22;
	m_pFrameData[2] = 0x33;
	m_pFrameData[3] = 0x44;
	m_pFrameData[4] = 0x00;
	m_pFrameData[5] = 0x00;
	m_pFrameData[6] = 0x00;
	m_pFrameData[7] = 0x00;
	m_pFrameData[8] = 0x00;
	m_pFrameData[9] = 0x00;
	m_pFrameData[10] = 0x00;
	m_pFrameData[11] = 0x00;
	m_pFrameData[12] = 0x00;
	m_pFrameData[13] = 0x00;
	m_pFrameData[14] = 0x00;
	m_pFrameData[15] = 0x00;
	// 源IP地址
	m_uiIPSource = 0x00000000;
	// 目标IP地址
	m_uiIPAim = 0x00000000;
	// 目标端口号
	m_usPortAim = 0;
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	m_usCommand = 1;
	// 源IP地址
	memcpy(&m_pFrameData[16], &m_uiIPSource, 4);
	// 目标IP地址
	memcpy(&m_pFrameData[20], &m_uiIPAim, 4);
	// 目标端口号
	memcpy(&m_pFrameData[24], &m_usPortAim, 2);
	// 命令号 1-设置命令应答；2-查询命令应答；3-AD采样数据重发
	memcpy(&m_pFrameData[26], &m_usCommand, 2);

	int iPos = 32;
	// 设置命令字
	m_pFrameData[iPos] = 0x00;
}

// 发送心跳帧
void CHeartBeatThread::SendHeartBeatFrame(void)
{
	// 发送广播命令帧
	int iCount = m_HeartBeatSocket.SendTo(m_pFrameData, m_iFrameSize, SendPort, IPBroadcastAddr);
}

// 关闭并结束线程
void CHeartBeatThread::OnClose(void)
{
// 	m_HeartBeatSocket.ShutDown(2);
 	m_HeartBeatSocket.Close();
	m_bclose = true;
}
