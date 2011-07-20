// InstrumentMonitor.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "InstrumentMonitorThread.h"
#include "Parameter.h"

// CInstrumentMonitor

IMPLEMENT_DYNCREATE(CInstrumentMonitorThread, CWinThread)

CInstrumentMonitorThread::CInstrumentMonitorThread()
: m_bclose(false)
, m_pLogFile(NULL)
, m_pwnd(NULL)
, m_pADCSet(NULL)
, m_pInstrumentList(NULL)
, m_pTailFrame(NULL)
, m_uiTailFrameTimerLAUXCount(0)
, m_uiTailFrameStableLAUXCount(0)
, m_uiTailFrameTimerFDUCount(0)
, m_uiTailFrameStableFDUCount(0)
{
}

CInstrumentMonitorThread::~CInstrumentMonitorThread()
{
}

BOOL CInstrumentMonitorThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CInstrumentMonitorThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CInstrumentMonitorThread, CWinThread)
END_MESSAGE_MAP()


// CInstrumentMonitor 消息处理程序

int CInstrumentMonitorThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类
	// 循环，直到关闭标志为真
	while(true)
	{
		if(m_bclose == true)
		{
			break;
		}
 		OnMonitorLAUX();
 		OnMonitorFDU();
		if (m_bclose == true)
		{
			break;
		}
		Sleep(InstrumentMonitorThreadSleepTime);
	}
	::SetEvent(m_hInstrumentMonitorThreadClose);	// 设置事件对象为有信号状态,释放等待线程后将事件置为无信号
	return CWinThread::Run();
}

// 初始化
void CInstrumentMonitorThread::OnInit(void)
{
	m_hInstrumentMonitorThreadClose = ::CreateEvent(false, false, NULL, NULL);	// 创建事件对象
	if ((m_hInstrumentMonitorThreadClose == NULL)||(GetLastError() == ERROR_ALREADY_EXISTS))
	{
		AfxMessageBox(_T("创建设备监视线程关闭事件出错！"));
	}
	else
	{
		::ResetEvent(m_hInstrumentMonitorThreadClose);	// 设置事件对象为无信号状态
	}
}

// 关闭
void CInstrumentMonitorThread::OnClose(void)
{
	m_bclose = true;
}

// 监视函数
void CInstrumentMonitorThread::OnMonitorLAUX(void)
{
	BOOL bTailRecLAUX = FALSE;
	unsigned int uiTailRecLAUXCount = 0;
	m_Sec_InstrumentMonitor.Lock();
	bTailRecLAUX = m_pTailFrame->m_bTailRecLAUX;
	uiTailRecLAUXCount = m_pTailFrame->m_uiTailRecLAUXCount;
	m_Sec_InstrumentMonitor.Unlock();
	// 监视是否有交叉站尾包
 	if (bTailRecLAUX == TRUE)
 	{
		CString str = _T("");
		m_uiTailFrameTimerLAUXCount++;
 		if (m_uiTailFrameTimerLAUXCount > uiTailRecLAUXCount)
 		{
 			if ((m_uiTailFrameTimerLAUXCount - uiTailRecLAUXCount) > TailFrameLAUXCount)
 			{
 				str.Format(_T("无交叉站尾包，此时定时器计数%d，尾包计数%d，删除所有仪器，停止采集！")
 					, m_uiTailFrameTimerLAUXCount, uiTailRecLAUXCount);
 				m_pLogFile->OnWriteLogFile(_T("CInstrumentMonitorThread::OnMonitor"), str, WarningStatus);
				m_Sec_InstrumentMonitor.Lock();
 				m_pTailFrame->m_bTailRecLAUX = FALSE;
				m_pTailFrame->m_bTailRecFDU = FALSE;
				m_Sec_InstrumentMonitor.Unlock();
				OnResetLAUXCounter();
				// 连续5秒未收到交叉站尾包则删除所有仪器
				m_pInstrumentList->DeleteAllInstrument();
				OnStopOperation();
			}
			else
			{
				m_uiTailFrameStableLAUXCount++;
				if (m_uiTailFrameStableLAUXCount == TailFrameStableLAUXCount)
				{
					OnResetLAUXCounter();
				}
			}
 		}
		else
		{
			OnResetLAUXCounter();
		}
 	}
	else
	{
		OnResetLAUXCounter();
	}
}
// 监视函数
void CInstrumentMonitorThread::OnMonitorFDU(void)
{
	BOOL bTailRecFDU = FALSE;
	unsigned int uiTailRecFDUCount = 0;
	m_Sec_InstrumentMonitor.Lock();
	bTailRecFDU = m_pTailFrame->m_bTailRecFDU;
	uiTailRecFDUCount = m_pTailFrame->m_uiTailRecFDUCount;
	m_Sec_InstrumentMonitor.Unlock();
	// 监视是否有采集站尾包
	if (bTailRecFDU == TRUE)
	{
		CString str = _T("");
		m_uiTailFrameTimerFDUCount++;
		if (m_uiTailFrameTimerFDUCount > uiTailRecFDUCount)
		{
			if ((m_uiTailFrameTimerFDUCount - uiTailRecFDUCount) > TailFrameFDUCount)
			{
				CInstrument* pInstrument = NULL;
				str.Format(_T("无采集站尾包，此时定时器计数%d，尾包计数%d，删除所有仪器，停止采集！")
					, m_uiTailFrameTimerFDUCount, uiTailRecFDUCount);
				m_pLogFile->OnWriteLogFile(_T("CInstrumentMonitorThread::OnMonitor"), str, WarningStatus);
				m_Sec_InstrumentMonitor.Lock();
				m_pTailFrame->m_bTailRecFDU = FALSE;
				m_Sec_InstrumentMonitor.Unlock();
				// 连续5秒未收到采集站尾包则删除交叉站后所有仪器
				if(TRUE == m_pInstrumentList->GetInstrumentFromSNMap(m_pTailFrame->m_uiSN, pInstrument))
				{
					m_pInstrumentList->TailFrameDeleteInstrument(pInstrument);
				}
				OnResetFDUCounter();
				OnStopOperation();
			}
			else
			{
				m_uiTailFrameStableFDUCount++;
				if (m_uiTailFrameStableFDUCount == TailFrameStableFDUCount)
				{
					OnResetFDUCounter();
				}
			}
		}
		else
		{
			OnResetFDUCounter();
		}
	}
	else
	{
		OnResetFDUCounter();
	}
}
void CInstrumentMonitorThread::OnStopOperation(void)
{
	// 发送采样结束操作命令帧
	m_pADCSet->OnADCSampleStop();
	m_pwnd->SetTimer(TabSampleStopSampleTimerNb, TabSampleStopSampleTimerSet, NULL);
	m_pwnd->KillTimer(TabSampleQueryErrorCountTimerNb);
	m_pwnd->GetDlgItem(IDC_BUTTON_SETBYHAND)->EnableWindow(FALSE);
	m_pwnd->GetDlgItem(IDC_BUTTON_TIMECAL)->EnableWindow(FALSE);
	m_pwnd->GetDlgItem(IDC_BUTTON_SETADC)->EnableWindow(FALSE);
}
// 重置交叉站尾包计数器
void CInstrumentMonitorThread::OnResetLAUXCounter(void)
{
	m_Sec_InstrumentMonitor.Lock();
	m_pTailFrame->m_uiTailRecLAUXCount = 0;
	m_Sec_InstrumentMonitor.Unlock();
	m_uiTailFrameTimerLAUXCount = 0;
	m_uiTailFrameStableLAUXCount = 0;
}

// 重置采集站尾包计数器
void CInstrumentMonitorThread::OnResetFDUCounter(void)
{
	m_Sec_InstrumentMonitor.Lock();
	m_pTailFrame->m_uiTailRecFDUCount = 0;
	m_Sec_InstrumentMonitor.Unlock();
	m_uiTailFrameTimerFDUCount = 0;
	m_uiTailFrameStableFDUCount = 0;
}
