// ADDataThread.cpp : 实现文件
//

#include "stdafx.h"
#include "模拟示波器图形显示.h"
#include "ADDataThread.h"
#include "Parameter.h"

// ADDataThread

IMPLEMENT_DYNCREATE(CADDataThread, CWinThread)

CADDataThread::CADDataThread()
: m_bclose(false)
{
}

CADDataThread::~CADDataThread()
{
}

BOOL CADDataThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CADDataThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CADDataThread, CWinThread)
END_MESSAGE_MAP()


// ADDataThread 消息处理程序
// 初始化
void CADDataThread::OnInit(void)
{
	m_hADDDataThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	::ResetEvent(m_hADDDataThreadClose);	// 设置事件对象为无信号状态
	BOOL flag = FALSE; 
	flag =  m_ADDataSocket.Create(ADRecPort,SOCK_DGRAM);
	if (flag == FALSE)
	{
		AfxMessageBox(_T("接收端口创建失败！"));
	}

}
// 线程函数在Run中执行
int CADDataThread::Run()
{
	DWORD dwFrameCount = 0;
	int icount = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}

		// 得到网络接收缓冲区数据字节数
		m_ADDataSocket.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/256;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				icount = m_ADDataSocket.Receive(ADData,256);
				if (icount == 256)
				{
					byte* pSampleData = &(ADData[16]);
					pHardwareTestDlg->ReceiveSampleData(pSampleData);
				}
				else
				{
					break;
				}
			}
		}

		if (m_bclose == true)
		{
			break;
		}
		Sleep(50);
	}
	::SetEvent(m_hADDDataThreadClose);	// 设置事件对象为有信号状态
	// 返回
	return CWinThread::Run();
}
// 关闭并结束线程
void CADDataThread::OnClose(void)
{
 //	m_ADDataSocket.ShutDown(2);
 	m_ADDataSocket.Close();
	m_bclose = true;
}
