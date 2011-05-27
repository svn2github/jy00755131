// PortMonitoringSend.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixTestSoft.h"
#include "PortMonitoringSend.h"


// CPortMonitoringSend 对话框

IMPLEMENT_DYNAMIC(CPortMonitoringSend, CDialog)

CPortMonitoringSend::CPortMonitoringSend(CWnd* pParent /*=NULL*/)
	: CDialog(CPortMonitoringSend::IDD, pParent)
	, m_pRecThread(NULL)
{

}

CPortMonitoringSend::~CPortMonitoringSend()
{
}

void CPortMonitoringSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPortMonitoringSend, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CPortMonitoringSend 消息处理程序
//************************************
// Method:    OnInitDialog
// FullName:  CPortMonitoringSend::OnInitDialog
// Access:    virtual protected 
// Returns:   BOOL
// Qualifier:
//************************************
BOOL CPortMonitoringSend::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetTimer(PortMonitoringSendTimerNb, PortMonitoringSendTimerSet, NULL);
	return TRUE;
}
void CPortMonitoringSend::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == PortMonitoringSendTimerNb)
	{
		CString strTemp = _T("");
		CString str = _T("");
		unsigned int uiTemp = 0;
		unsigned int uiHeartBeatNum = 0;
		unsigned int uiIPSetNum = 0;
		unsigned int uiTailTimeNum = 0;
		unsigned int uiDelayTimeSetNum = 0;
		unsigned int uiADCSetNum = 0;
		
		m_Sec.Lock();
		uiHeartBeatNum = m_pRecThread->m_uiHeartBeatNum;
		uiIPSetNum = m_pRecThread->m_uiIPSetNum;
		uiTailTimeNum = m_pRecThread->m_uiTailTimeNum;
		uiDelayTimeSetNum = m_pRecThread->m_uiDelayTimeSetNum;
		uiADCSetNum = m_pRecThread->m_uiADCSetNum;
		m_Sec.Unlock();

// 		// 心跳
// 		GetDlgItem(IDC_STATIC_HEARTBEATFRAMENUM)->GetWindowText(str);
// 		uiTemp = atoi(str);
// 		if (uiTemp != uiHeartBeatNum)
// 		{
// 			strTemp.Format(_T("%d"), uiHeartBeatNum);
// 			GetDlgItem(IDC_STATIC_HEARTBEATFRAMENUM)->SetWindowText(strTemp);
// 		}
// 		// IP地址设置
// 		GetDlgItem(IDC_STATIC_IPSETNUM)->GetWindowText(str);
// 		uiTemp = atoi(str);
// 		if (uiTemp != uiIPSetNum)
// 		{
// 			strTemp.Format(_T("%d"), uiIPSetNum);
// 			GetDlgItem(IDC_STATIC_IPSETNUM)->SetWindowText(strTemp);
// 		}
// 		// 尾包时刻查询
// 		GetDlgItem(IDC_STATIC_TAILTIMESETNUM)->GetWindowText(str);
// 		uiTemp = atoi(str);
// 		if (uiTemp != uiTailTimeNum)
// 		{
// 			strTemp.Format(_T("%d"), uiTailTimeNum);
// 			GetDlgItem(IDC_STATIC_TAILTIMESETNUM)->SetWindowText(strTemp);
// 		}
// 		// 时统设置
// 		GetDlgItem(IDC_STATIC_DELAYTIMESETNUM)->GetWindowText(str);
// 		uiTemp = atoi(str);
// 		if (uiTemp != uiDelayTimeSetNum)
// 		{
// 			strTemp.Format(_T("%d"), uiDelayTimeSetNum);
// 			GetDlgItem(IDC_STATIC_DELAYTIMESETNUM)->SetWindowText(strTemp);
// 		}
// 		// ADC参数设置
// 		GetDlgItem(IDC_STATIC_ADCSETNUM)->GetWindowText(str);
// 		uiTemp = atoi(str);
// 		if (uiTemp != uiADCSetNum)
// 		{
// 			strTemp.Format(_T("%d"), uiADCSetNum);
// 			GetDlgItem(IDC_STATIC_ADCSETNUM)->SetWindowText(strTemp);
// 		}
	}
	CDialog::OnTimer(nIDEvent);
}
