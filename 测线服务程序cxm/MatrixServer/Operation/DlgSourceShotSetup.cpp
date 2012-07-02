// DlgSourceShotSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgSourceShotSetup.h"
#include "afxdialogex.h"


// CDlgSourceShotSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSourceShotSetup, CBCGPDialog)

CDlgSourceShotSetup::CDlgSourceShotSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSourceShotSetup::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgSourceShotSetup::~CDlgSourceShotSetup()
{
}

void CDlgSourceShotSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSourceShotSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSourceShotSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgSourceShotSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgSourceShotSetup::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CDlgSourceShotSetup::OnBnClickedButtonReverse)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgSourceShotSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgSourceShotSetup::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlgSourceShotSetup 消息处理程序
BOOL CDlgSourceShotSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;
	int iShotIdWidth, iBreakPointWidth, iSwathNbWidth, iSourceLineWidth, 
		iSourceRcvWidth, iSpreadSflWidth, iSpreadSfnWidth, iSpreadTypeWidth,
		iSuperSpreadWidth, iProcTypeWidth, iCommentWidth;
	GetDlgItem(IDC_STATIC_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_SHOTID)->GetWindowRect(&rectGrid);
	iShotIdWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_BREAKPOINT)->GetWindowRect(&rectGrid);
	iBreakPointWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_SWATHNB)->GetWindowRect(&rectGrid);
	iSwathNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_LINE)->GetWindowRect(&rectGrid);
	iSourceLineWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_RCV)->GetWindowRect(&rectGrid);
	iSourceRcvWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_SFL)->GetWindowRect(&rectGrid);
	iSpreadSflWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_SFN)->GetWindowRect(&rectGrid);
	iSpreadSfnWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_TYPE)->GetWindowRect(&rectGrid);
	iSpreadTypeWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_SUPERSPREAD)->GetWindowRect(&rectGrid);
	iSuperSpreadWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_PROCTYPE)->GetWindowRect(&rectGrid);
	iProcTypeWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_COMMENT)->GetWindowRect(&rectGrid);
	iCommentWidth = rectGrid.Width();
	m_wndEditGrid.InsertColumn (0, _T("Shot Id"), iShotIdWidth);
	m_wndEditGrid.InsertColumn (1, _T("BreakPoint"), iBreakPointWidth);
	m_wndEditGrid.InsertColumn (2, _T("Swath Nb"), iSwathNbWidth);
	m_wndEditGrid.InsertColumn (3, _T("Source Line"), iSourceLineWidth);
	m_wndEditGrid.InsertColumn (4, _T("Source Rcv"), iSourceRcvWidth);

	m_wndEditGrid.InsertColumn (5, _T("Spread Sfl"), iSpreadSflWidth);
	m_wndEditGrid.InsertColumn (6, _T("Spread Sfn"), iSpreadSfnWidth);
	m_wndEditGrid.InsertColumn (7, _T("Spread Type"), iSpreadTypeWidth);
	m_wndEditGrid.InsertColumn (8, _T("Super Spread"), iSuperSpreadWidth);
	m_wndEditGrid.InsertColumn (9, _T("Proc Type"), iProcTypeWidth);
	m_wndEditGrid.InsertColumn (10, _T("Comment"), iCommentWidth);

	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Shot Id"), iShotIdWidth);
	m_wndListGrid.InsertColumn (1, _T("BreakPoint"), iBreakPointWidth);
	m_wndListGrid.InsertColumn (2, _T("Swath Nb"), iSwathNbWidth);
	m_wndListGrid.InsertColumn (3, _T("Source Line"), iSourceLineWidth);
	m_wndListGrid.InsertColumn (4, _T("Source Rcv"), iSourceRcvWidth);

	m_wndListGrid.InsertColumn (5, _T("Spread Sfl"), iSpreadSflWidth);
	m_wndListGrid.InsertColumn (6, _T("Spread Sfn"), iSpreadSfnWidth);
	m_wndListGrid.InsertColumn (7, _T("Spread Type"), iSpreadTypeWidth);
	m_wndListGrid.InsertColumn (8, _T("Super Spread"), iSuperSpreadWidth);
	m_wndListGrid.InsertColumn (9, _T("Proc Type"), iProcTypeWidth);
	m_wndListGrid.InsertColumn (10, _T("Comment"), iCommentWidth);

	for(int i=0;i<11;i++)
	{
		m_wndEditGrid.SetColumnLocked(i,TRUE);
		m_wndListGrid.SetColumnLocked(i,TRUE);
	}	

	CBCGPGridRow* pRow = m_wndEditGrid.CreateRow (m_wndEditGrid.GetColumnCount ());
	CBCGPGridItem* pItem = new CBCGPGridItem (_T("No"));
	pItem->AddOption (_T("No"), TRUE);
	pItem->AddOption (_T("Yes"), TRUE);
	pRow->ReplaceItem (1, pItem);

	m_wndEditGrid.AddRow(pRow);
	m_wndEditGrid.AdjustLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSourceShotSetup::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceShotSetup::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceShotSetup::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceShotSetup::OnBnClickedButtonReverse()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceShotSetup::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSourceShotSetup::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
}



