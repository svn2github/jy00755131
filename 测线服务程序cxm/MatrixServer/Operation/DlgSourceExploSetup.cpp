// DlgSourceExploSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgSourceExploSetup.h"
#include "afxdialogex.h"


// CDlgSourceExploSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSourceExploSetup, CBCGPDialog)

CDlgSourceExploSetup::CDlgSourceExploSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSourceExploSetup::IDD, pParent)
{
//	EnableVisualManagerStyle();
}

CDlgSourceExploSetup::~CDlgSourceExploSetup()
{
}

void CDlgSourceExploSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSourceExploSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSourceExploSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgSourceExploSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgSourceExploSetup::OnBnClickedButtonDelete)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgSourceExploSetup 消息处理程序
BOOL CDlgSourceExploSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	CRect rectGrid;
	int iNbWidth, iLabelWidth, iShooterNbWidth, iIncrNbWidth, iCommentWidth;
	GetDlgItem(IDC_STATIC_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_NB)->GetWindowRect (&rectGrid);
	iNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_LABEL)->GetWindowRect (&rectGrid);
	iLabelWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_SHOOTERNB)->GetWindowRect (&rectGrid);
	iShooterNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_INCRNB)->GetWindowRect (&rectGrid);
	iIncrNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_COMMENT)->GetWindowRect (&rectGrid);
	iCommentWidth = rectGrid.Width();
	m_wndEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndEditGrid.InsertColumn (1, _T("Label"), iLabelWidth);
	m_wndEditGrid.InsertColumn (2, _T("Shooter Nb"), iShooterNbWidth);
	m_wndEditGrid.InsertColumn (3, _T("Incr Nb"), iIncrNbWidth);
	m_wndEditGrid.InsertColumn (4, _T("Comment"), iCommentWidth);

	GetDlgItem(IDC_STATIC_GRID_LIST)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndListGrid.InsertColumn (1, _T("Label"), iLabelWidth);
	m_wndListGrid.InsertColumn (2, _T("Shooter Nb"), iShooterNbWidth);
	m_wndListGrid.InsertColumn (3, _T("Incr Nb"), iIncrNbWidth);
	m_wndListGrid.InsertColumn (4, _T("Comment"), iCommentWidth);
	for(int i=0;i<m_wndEditGrid.GetColumnCount();i++)
	{
		m_wndEditGrid.SetColumnLocked(i,TRUE);
		m_wndListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();
	return TRUE;
}

void CDlgSourceExploSetup::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceExploSetup::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceExploSetup::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceExploSetup::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_wndEditGrid.RemoveAll();
	m_wndListGrid.RemoveAll();
}
