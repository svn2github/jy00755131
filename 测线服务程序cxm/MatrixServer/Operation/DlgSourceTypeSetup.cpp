// DlgSourceTypeSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgSourceTypeSetup.h"
#include "afxdialogex.h"
#include "OptParam.h"

// CDlgSourceTypeSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSourceTypeSetup, CBCGPDialog)

CDlgSourceTypeSetup::CDlgSourceTypeSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSourceTypeSetup::IDD, pParent)
{
//	EnableVisualManagerStyle();
}

CDlgSourceTypeSetup::~CDlgSourceTypeSetup()
{
}

void CDlgSourceTypeSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSourceTypeSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgSourceTypeSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgSourceTypeSetup::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlgSourceTypeSetup 消息处理程序
BOOL CDlgSourceTypeSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
 	CRect rectTab;
	GetDlgItem(IDC_STATIC_TAB)->GetWindowRect (&rectTab);
	ScreenToClient (&rectTab);

	m_wndTab.Create (CBCGPTabWnd::STYLE_3D_ONENOTE, rectTab, this, 1, CBCGPTabWnd::LOCATION_TOP);

	m_DlgExplo.Create(IDD_DIALOG_SOURCEEXPLO,&m_wndTab);
	m_DlgExplo.SetWindowText (_T("Explo"));
	m_DlgVibro.Create(IDD_DIALOG_SOURCEVIBRO,&m_wndTab);
	m_DlgVibro.SetWindowText (_T("Vibro"));

	m_wndTab.AddTab (&m_DlgExplo, _T(" Explo "), 0, FALSE);
	m_wndTab.AddTab (&m_DlgVibro, _T(" Vibro "), 1, FALSE);

	m_wndTab.EnableTabSwap(FALSE);
	m_wndTab.SetDrawFrame(TRUE);
	m_wndTab.SetButtonsVisible(FALSE);
	m_wndTab.SetFlatFrame(FALSE,TRUE);
	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	MoveTitleToCenter((CWnd*)this);
	return TRUE;
}

void CDlgSourceTypeSetup::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceTypeSetup::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
}


