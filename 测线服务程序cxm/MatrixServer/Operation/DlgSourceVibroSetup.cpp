// DlgSourceVibroSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgSourceVibroSetup.h"
#include "afxdialogex.h"


// CDlgSourceVibroSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSourceVibroSetup, CBCGPDialog)

CDlgSourceVibroSetup::CDlgSourceVibroSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSourceVibroSetup::IDD, pParent)
{
//	EnableVisualManagerStyle();
}

CDlgSourceVibroSetup::~CDlgSourceVibroSetup()
{
}

void CDlgSourceVibroSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSourceVibroSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSourceVibroSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgSourceVibroSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgSourceVibroSetup::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CDlgSourceVibroSetup 消息处理程序
BOOL CDlgSourceVibroSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	CRect rectGrid;
	int iNbWidth, iLabelWidth, iFleetNbWidth, iTypeOfMovingWidth, iStepWidth, iWorkByAcqWidth, iClusterNbWidth;
	GetDlgItem(IDC_STATIC_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
 	m_wndEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_NB)->GetWindowRect (&rectGrid);
	iNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_LABEL)->GetWindowRect (&rectGrid);
	iLabelWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_FLEETNB)->GetWindowRect (&rectGrid);
	iFleetNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_TYPEOFMOVING)->GetWindowRect (&rectGrid);
	iTypeOfMovingWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_STEP)->GetWindowRect (&rectGrid);
	iStepWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_WORKBYACQ)->GetWindowRect (&rectGrid);
	iWorkByAcqWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_CLUSTERNB)->GetWindowRect (&rectGrid);
	iClusterNbWidth = rectGrid.Width();
	m_wndEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndEditGrid.InsertColumn (1, _T("Label"), iLabelWidth);
	m_wndEditGrid.InsertColumn (2, _T("Fleet Nb"), iFleetNbWidth);
	m_wndEditGrid.InsertColumn (3, _T("Type Of Moving"), iTypeOfMovingWidth);
	m_wndEditGrid.InsertColumn (4, _T("Step"), iStepWidth);
	m_wndEditGrid.InsertColumn (5, _T("Work By Acq"), iWorkByAcqWidth);
	m_wndEditGrid.InsertColumn (6, _T("Cluster Nb"), iClusterNbWidth);

	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndListGrid.InsertColumn (1, _T("Label"), iLabelWidth);
	m_wndListGrid.InsertColumn (2, _T("Fleet Nb"), iFleetNbWidth);
	m_wndListGrid.InsertColumn (3, _T("Type Of Moving"), iTypeOfMovingWidth);
	m_wndListGrid.InsertColumn (4, _T("Step"), iStepWidth);
	m_wndListGrid.InsertColumn (5, _T("Work By Acq"), iWorkByAcqWidth);
	m_wndListGrid.InsertColumn (6, _T("Cluster Nb"), iClusterNbWidth);
	for(int i=0;i<7;i++)
	{
		m_wndEditGrid.SetColumnLocked(i,TRUE);
		m_wndListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();
	return TRUE;
}

void CDlgSourceVibroSetup::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceVibroSetup::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceVibroSetup::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}
