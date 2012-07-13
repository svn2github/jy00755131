// DlgDelaySetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgDelaySetup.h"
#include "afxdialogex.h"
#include "Parameter.h"

// CDlgDelaySetup 对话框

IMPLEMENT_DYNAMIC(CDlgDelaySetup, CDialog)

CDlgDelaySetup::CDlgDelaySetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDelaySetup::IDD, pParent)
	, m_feditAcq(0)
	, m_feditVP(0)
{

}

CDlgDelaySetup::~CDlgDelaySetup()
{
}

void CDlgDelaySetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACQTIME, m_feditAcq);
	DDX_Text(pDX, IDC_EDIT_VPTIME, m_feditVP);
	DDX_Control(pDX, IDC_SLIDER_ACQ, m_ctrlsliderAcq);
	DDX_Control(pDX, IDC_SLIDER_VP, m_ctrlsliderVP);
}


BEGIN_MESSAGE_MAP(CDlgDelaySetup, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgDelaySetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgDelaySetup::OnBnClickedButtonReset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACQ, &CDlgDelaySetup::OnNMCustomdrawSliderAcq)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VP, &CDlgDelaySetup::OnNMCustomdrawSliderVp)
END_MESSAGE_MAP()


// CDlgDelaySetup 消息处理程序


void CDlgDelaySetup::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgDelaySetup::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CDlgDelaySetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctrlsliderAcq.SetRange(0, 1000);
	m_ctrlsliderVP.SetRange(0, 1000);
	m_ctrlsliderAcq.SetPageSize(1);
	m_ctrlsliderVP.SetPageSize(1);
	MoveTitleToCenter((CWnd*)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDelaySetup::OnNMCustomdrawSliderAcq(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_feditAcq = m_ctrlsliderAcq.GetPos() / 10.0f;
	UpdateData(FALSE); 
	*pResult = 0;
}


void CDlgDelaySetup::OnNMCustomdrawSliderVp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_feditVP = m_ctrlsliderVP.GetPos() / 10.0f;
	UpdateData(FALSE); 
	*pResult = 0;
}
