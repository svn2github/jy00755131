// SendThread.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "SendThread.h"
#include "Parameter.h"

// CSendThread

IMPLEMENT_DYNCREATE(CSendThread, CWinThread)

CSendThread::CSendThread()
{
	
}

CSendThread::~CSendThread()
{

}

BOOL CSendThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CSendThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendThread, CWinThread)
END_MESSAGE_MAP()


// 初始化

void CSendThread::Init()
{
	udp_count = 0;
	m_close = false;
	int flag =  m_hSocket2.Create(RcvPort2,SOCK_DGRAM);
	m_hSocket2.SetSockOpt(SO_RCVBUF,&m_hSocket2,RcvBufSize,SOL_SOCKET);
	m_hSocket2.SetSockOpt(SO_SNDBUF,&m_hSocket2,SndBufSize,SOL_SOCKET);
}
// 线程函数在Run中执行
int CSendThread::Run()
{
	unsigned short uiPort = 0;
	DWORD dwFrameCount = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if (m_close == true)
		{
			break;
		}

		// 得到网络接收缓冲区数据字节数
		while(m_hSocket2.IOCtl(FIONREAD, &dwFrameCount))
		{
			dwFrameCount = dwFrameCount/256;
			if(0 != dwFrameCount)	
			{
				for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
				{

					icount = m_hSocket2.Receive(udp_buf[udp_count],256);
					if (icount == 256)
					{
						// 加入端口分发功能
						memcpy(&uiPort, &udp_buf[udp_count][24], 2);
						m_hSocket2.SendTo(udp_buf[udp_count],256,uiPort,pDlg->m_strSendIP);
						//m_hSocket2.SendTo(udp_buf[udp_count],256,SendPort2,pDlg->m_strSendIP);
						pDlg->OnSaveSendData(udp_buf[udp_count],256);
						udp_count +=1;
						udp_count = udp_count%8;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
		if (m_close == true)
		{
			break;
		}
		Sleep(1);
	}
	
	// 返回
	return CWinThread::Run();
}
void CSendThread::OnClose()
{
	m_hSocket2.ShutDown(2);
	m_hSocket2.Close();
	m_close = true;
}