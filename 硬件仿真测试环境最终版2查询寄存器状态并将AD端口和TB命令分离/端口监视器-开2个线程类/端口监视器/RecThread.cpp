// RecThread.cpp : 实现文件
//

#include "stdafx.h"
#include "端口监视器.h"
#include "RecThread.h"
#include "Parameter.h"
// CRecThread

IMPLEMENT_DYNCREATE(CRecThread, CWinThread)

CRecThread::CRecThread()
{
}

CRecThread::~CRecThread()
{	
}

BOOL CRecThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CRecThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRecThread, CWinThread)
END_MESSAGE_MAP()


// 初始化

void CRecThread::Init()
{
	udp_count = 0;
	m_close = false;
	m_hRecThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hRecThreadClose);	// 设置事件对象为无信号状态
 	int flag =  m_hSocket1.Create(RcvPort1,SOCK_DGRAM);
 	m_hSocket1.SetSockOpt(NULL,&m_hSocket1,65536,SOL_SOCKET);
}
// 线程函数在Run中执行
int CRecThread::Run()
{
	DWORD dwFrameCount = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_close == true)
		{
			break;
		}

		// 得到网络接收缓冲区数据字节数
		m_hSocket1.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/128;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				icount = m_hSocket1.Receive(udp_buf[udp_count],128);
				if (icount == 128)
				{
					m_hSocket1.SendTo(udp_buf[udp_count],128,SendPort1,pDlg->m_strReceiveIP);
					pDlg->OnSaveReceiveData(udp_buf[udp_count],128);
					udp_count +=1;
					udp_count = udp_count%8;
				}
				else
				{
					break;
				}
			}
		}

		if (m_close == true)
		{
			break;
		}
		Sleep(50);
	}
	::SetEvent(m_hRecThreadClose);	// 设置事件对象为有信号状态
	// 返回
	return CWinThread::Run();
}
void CRecThread::OnClose()
{
	m_hSocket1.ShutDown(2);
	m_hSocket1.Close();
	m_close = true;
}
