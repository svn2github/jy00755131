// DlgDelaySetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgDelaySetup.h"


// CDlgDelaySetup 对话框

IMPLEMENT_DYNAMIC(CDlgDelaySetup, CBCGPDialog)

CDlgDelaySetup::CDlgDelaySetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgDelaySetup::IDD, pParent)	
	, m_fEndOfAcq(0)
	, m_fEndOfVP(0)
{
	EnableVisualManagerStyle();
}

CDlgDelaySetup::~CDlgDelaySetup()
{
}

void CDlgDelaySetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);			
	DDX_Text(pDX, IDC_EDITENDOFACQ, m_fEndOfAcq);
	DDX_Text(pDX, IDC_EDITENDOFVP, m_fEndOfVP);
	DDX_Control(pDX, IDC_SLIDERENDOFACQ, m_SliderEndOfAcq);
	DDX_Control(pDX, IDC_SLIDERENDOFVP, m_SliderEndOfVP);
}


BEGIN_MESSAGE_MAP(CDlgDelaySetup, CBCGPDialog)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDERENDOFACQ, &CDlgDelaySetup::OnTRBNThumbPosChangingSliderendofacq)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SLIDERENDOFACQ, &CDlgDelaySetup::OnNMThemeChangedSliderendofacq)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERENDOFACQ, &CDlgDelaySetup::OnNMReleasedcaptureSliderendofacq)
END_MESSAGE_MAP()


// CDlgDelaySetup 消息处理程序

void CDlgDelaySetup::OnTRBNThumbPosChangingSliderendofacq(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_fEndOfAcq = m_SliderEndOfAcq.GetPos()/10.0f;	
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgDelaySetup::OnNMThemeChangedSliderendofacq(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_fEndOfAcq = m_SliderEndOfAcq.GetPos()/10.0f;	
	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgDelaySetup::OnNMReleasedcaptureSliderendofacq(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_fEndOfAcq = m_SliderEndOfAcq.GetPos()/10.0f;	
	UpdateData(FALSE);
	*pResult = 0;
}

BOOL CDlgDelaySetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SliderEndOfAcq.SetRange(0,990);
	m_SliderEndOfVP.SetRange(0,990);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
