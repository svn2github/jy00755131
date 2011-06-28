// PortMonitoringRec.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "PortMonitoringRec.h"


// CPortMonitoringRec 对话框

IMPLEMENT_DYNAMIC(CPortMonitoringRec, CDialog)

CPortMonitoringRec::CPortMonitoringRec(CWnd* pParent /*=NULL*/)
	: CDialog(CPortMonitoringRec::IDD, pParent)
	, m_pSendThread(NULL)
{

}

CPortMonitoringRec::~CPortMonitoringRec()
{
	if (m_pSendThread != NULL)
	{
		m_pSendThread = NULL;
		delete m_pSendThread;
	}
}

void CPortMonitoringRec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPortMonitoringRec, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CPortMonitoringRec 消息处理程序
void CPortMonitoringRec::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == PortMonitoringRecTimerNb)
	{

		CString strTemp = _T("");
		CString str = _T("");
		unsigned int uiTemp = 0;
		unsigned int uiHeadFrameNum = 0;
		unsigned int uiIPSetReturnNum = 0;
		unsigned int uiTailFrameNum = 0;
		unsigned int uiTailTimeReturnNum = 0;
		unsigned int uiDelayTimeReturnNum = 0;
		unsigned int uiADCSetReturnNum = 0;
		unsigned int uiErrorCodeReturnNum = 0;
		unsigned int uiCollectSysTimeReturnNum = 0;
		unsigned int uiADCRecNum = 0;
		CString csHeadFrameShow = _T("");
		CString csIPSetReturnShow = _T("");
		CString csTailFrameShow = _T("");
		CString csTailTimeReturnShow = _T("");
		CString csDelayTimeReturnShow = _T("");
//		CString csErrorCodeReturnShow = _T("");
		m_Sec_PortMonitorRec.Lock();
		uiHeadFrameNum = m_pSendThread->m_uiHeadFrameNum;
		uiIPSetReturnNum = m_pSendThread->m_uiIPSetReturnNum;
		uiTailFrameNum = m_pSendThread->m_uiTailFrameNum;
		uiTailTimeReturnNum = m_pSendThread->m_uiTailTimeReturnNum;
		uiDelayTimeReturnNum = m_pSendThread->m_uiDelayTimeReturnNum;
		uiErrorCodeReturnNum = m_pSendThread->m_uiErrorCodeReturnNum;
		uiADCSetReturnNum = m_pSendThread->m_uiADCSetReturnNum;
		uiCollectSysTimeReturnNum = m_pSendThread->m_uiCollectSysTimeReturnNum;
		uiADCRecNum = m_pSendThread->m_uiADCRecNum;
		csHeadFrameShow = m_pSendThread->m_csHeadFrameShow;
		csIPSetReturnShow = m_pSendThread->m_csIPSetReturnShow;
		csTailFrameShow = m_pSendThread->m_csTailFrameShow;
		csTailTimeReturnShow = m_pSendThread->m_csTailTimeReturnShow;
		csDelayTimeReturnShow = m_pSendThread->m_csDelayTimeReturnShow;
//		csErrorCodeReturnShow = m_pSendThread->m_csErrorCodeReturnShow;
		m_Sec_PortMonitorRec.Unlock();

		// 首包计数
		GetDlgItem(IDC_STATIC_HEADFRAMENUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiHeadFrameNum)
		{
			strTemp.Format(_T("%d"), uiHeadFrameNum);
			GetDlgItem(IDC_STATIC_HEADFRAMENUM)->SetWindowText(strTemp);
		}
		// IP地址设置计数
		GetDlgItem(IDC_STATIC_IPSETRETURNNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiIPSetReturnNum)
		{
			strTemp.Format(_T("%d"), uiIPSetReturnNum);
			GetDlgItem(IDC_STATIC_IPSETRETURNNUM)->SetWindowText(strTemp);
		}
		// 尾包计数
		GetDlgItem(IDC_STATIC_TAILFRAMENUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiTailFrameNum)
		{
			strTemp.Format(_T("%d"), uiTailFrameNum);
			GetDlgItem(IDC_STATIC_TAILFRAMENUM)->SetWindowText(strTemp);
		}
		// 尾包时刻查询应答计数
		GetDlgItem(IDC_STATIC_TAILTIMERETURNNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiTailTimeReturnNum)
		{
			strTemp.Format(_T("%d"), uiTailTimeReturnNum);
			GetDlgItem(IDC_STATIC_TAILTIMERETURNNUM)->SetWindowText(strTemp);
		}
		// 时延设置应答计数
		GetDlgItem(IDC_STATIC_DELAYTIMERETURNNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiDelayTimeReturnNum)
		{
			strTemp.Format(_T("%d"), uiDelayTimeReturnNum);
			GetDlgItem(IDC_STATIC_DELAYTIMERETURNNUM)->SetWindowText(strTemp);
		}
		// ADC设置应答计数
		GetDlgItem(IDC_STATIC_ADCSETRETURNNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiADCSetReturnNum)
		{
			strTemp.Format(_T("%d"), uiADCSetReturnNum);
			GetDlgItem(IDC_STATIC_ADCSETRETURNNUM)->SetWindowText(strTemp);
		}
		// 硬件设备错误查询应答帧个数
		GetDlgItem(IDC_STATIC_ERRORCODERETURNNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiErrorCodeReturnNum)
		{
			strTemp.Format(_T("%d"), uiErrorCodeReturnNum);
			GetDlgItem(IDC_STATIC_ERRORCODERETURNNUM)->SetWindowText(strTemp);
		}
		// 查询得到的本地时间帧数
		GetDlgItem(IDC_STATIC_COLLECTSYSTIMERETURNNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiCollectSysTimeReturnNum)
		{
			strTemp.Format(_T("%d"), uiCollectSysTimeReturnNum);
			GetDlgItem(IDC_STATIC_COLLECTSYSTIMERETURNNUM)->SetWindowText(strTemp);
		}
		// 接收得到的ADC数据帧数
		GetDlgItem(IDC_STATIC_ADCRECNUM)->GetWindowText(str);
		uiTemp = _tstoi(str);
		if (uiTemp != uiADCRecNum)
		{
			strTemp.Format(_T("%d"), uiADCRecNum);
			GetDlgItem(IDC_STATIC_ADCRECNUM)->SetWindowText(strTemp);
		}

		// 显示首包帧，包含SN和首包时刻
		GetDlgItem(IDC_EDIT_HEADFRAMESHOW)->GetWindowText(strTemp);
		if (strTemp != csHeadFrameShow)
		{
			GetDlgItem(IDC_EDIT_HEADFRAMESHOW)->SetWindowText(csHeadFrameShow);
		}
		// 显示IP地址设置应答帧，包含SN和设置的IP地址
		GetDlgItem(IDC_EDIT_IPSETRETURNSHOW)->GetWindowText(strTemp);
		if (strTemp != csIPSetReturnShow)
		{
			GetDlgItem(IDC_EDIT_IPSETRETURNSHOW)->SetWindowText(csIPSetReturnShow);
		}
		// 显示尾包帧，包含SN
		GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->GetWindowText(strTemp);
		if (strTemp != csTailFrameShow)
		{
			GetDlgItem(IDC_EDIT_TAILFRAMESHOW)->SetWindowText(csTailFrameShow);
		}
		// 显示尾包时刻查询应答帧，包含IP地址和查询结果
		GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->GetWindowText(strTemp);
		if (strTemp != csTailTimeReturnShow)
		{
			GetDlgItem(IDC_EDIT_TAILTIMERETURNSHOW)->SetWindowText(csTailTimeReturnShow);
		}
		// 显示时延设置应答帧，包含IP地址和设置内容
		GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->GetWindowText(strTemp);
		if (strTemp != csDelayTimeReturnShow)
		{
			GetDlgItem(IDC_EDIT_DELAYTIMERETURNSHOW)->SetWindowText(csDelayTimeReturnShow);
		}
		// 显示硬件错误码和码差
// 		GetDlgItem(IDC_EDIT_ERRORCODERETURNSHOW)->GetWindowText(strTemp);
// 		if (strTemp != csErrorCodeReturnShow)
// 		{
// 			GetDlgItem(IDC_EDIT_ERRORCODERETURNSHOW)->SetWindowText(csErrorCodeReturnShow);
// 		}
	}
	CDialog::OnTimer(nIDEvent);
}
