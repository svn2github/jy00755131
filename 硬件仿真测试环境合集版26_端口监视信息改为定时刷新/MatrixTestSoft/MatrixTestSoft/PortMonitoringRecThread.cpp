// PortMonitoringRecThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "PortMonitoringRecThread.h"


// CPortMonitoringRecThread

IMPLEMENT_DYNCREATE(CPortMonitoringRecThread, CWinThread)

CPortMonitoringRecThread::CPortMonitoringRecThread()
: m_usudp_count(0)
, m_iRecPort(0)
, m_iSendPort(0)
, m_csIP(_T(""))
, m_pSaveFile(NULL)
, m_uiHeartBeatNum(0)
, m_uiIPSetNum(0)
, m_uiTailTimeNum(0)
, m_uiDelayTimeSetNum(0)
, m_uiADCSetNum(0)
, m_uiSendFrameNum(0)
, m_bclose(false)
{
}

CPortMonitoringRecThread::~CPortMonitoringRecThread()
{
}

BOOL CPortMonitoringRecThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPortMonitoringRecThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPortMonitoringRecThread, CWinThread)
END_MESSAGE_MAP()


// CPortMonitoringRecThread 消息处理程序

int CPortMonitoringRecThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类

	DWORD dwFrameCount = 0;
	int icount = 0;
	unsigned short uiPort = 0;
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
	
		// 得到网络接收缓冲区数据字节数
		m_RecSocket.IOCtl(FIONREAD, &dwFrameCount);
		dwFrameCount = dwFrameCount/SndFrameSize;
		if(0 != dwFrameCount)	
		{
			for(unsigned int i = 0; i < dwFrameCount; i++)	// 依次检查每个仪器IP设置应答帧
			{
				ProcessMessages();
				icount = m_RecSocket.Receive(m_ucudp_buf[m_usudp_count],SndFrameSize);
				if (icount == SndFrameSize)
				{ 
					CString strTemp = _T("");
					unsigned int uiCommand = 0;
					// 注释掉下面的输出信息后在ADC参数设置和零漂过程中心跳就不会出现停顿现象
					// 通过端口识别功能
					memcpy(&uiPort, &m_ucudp_buf[m_usudp_count][24], 2);
					if (uiPort == HeartBeatRec)
					{
						m_uiHeartBeatNum ++;
					}
					else if (uiPort == IPSetPort)
					{
						m_uiIPSetNum ++;
					}
					else if (uiPort == TailTimeFramePort)
					{
						m_uiTailTimeNum ++;
					}
					else if (uiPort == TimeSetPort)
					{
						m_uiDelayTimeSetNum ++;
					}
					else if (uiPort == ADSetReturnPort)
					{
						m_uiADCSetNum ++;
					}
					m_uiSendFrameNum ++;

					m_RecSocket.SendTo(m_ucudp_buf[m_usudp_count],SndFrameSize,m_iSendPort,m_csIP);
					m_pSaveFile->OnSaveReceiveData(m_ucudp_buf[m_usudp_count],SndFrameSize);

					m_usudp_count +=1;
					m_usudp_count = m_usudp_count%8;
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
		Sleep(1);
	}

	::SetEvent(m_hPortMonitoringRecThreadClose);	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return CWinThread::Run();
}

// 初始化
void CPortMonitoringRecThread::OnInit(void)
{
	m_hPortMonitoringRecThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if ((m_hPortMonitoringRecThreadClose == NULL)||(GetLastError() == ERROR_ALREADY_EXISTS))
	{
		AfxMessageBox(_T("创建端口监视发送线程关闭事件出错！"));
	}
	else
	{
		::ResetEvent(m_hPortMonitoringRecThreadClose);	// 设置事件对象为无信号状态
	}
}

// 打开
void CPortMonitoringRecThread::OnOpen(void)
{
	m_RecSocket.Close();
	int flag =  m_RecSocket.Create(m_iRecPort,SOCK_DGRAM);
	m_RecSocket.SetSockOpt(SO_RCVBUF,&m_RecSocket,PortMonitoringBufSize,SOL_SOCKET);
	m_RecSocket.SetSockOpt(SO_SNDBUF,&m_RecSocket,PortMonitoringBufSize,SOL_SOCKET);
}

// 停止
void CPortMonitoringRecThread::OnStop(void)
{
	m_RecSocket.Close();
}

// 关闭
void CPortMonitoringRecThread::OnClose(void)
{
	m_RecSocket.Close();
	m_bclose = true;
}
// 重置界面
void CPortMonitoringRecThread::OnReset(void)
{
	CString strTemp;
	m_uiHeartBeatNum = 0;
	m_uiIPSetNum = 0;
	m_uiTailTimeNum = 0;
	m_uiDelayTimeSetNum = 0;
	m_uiADCSetNum = 0;
	m_uiSendFrameNum = 0;
}
// 防止程序在循环中运行无法响应消息
void CPortMonitoringRecThread::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}
