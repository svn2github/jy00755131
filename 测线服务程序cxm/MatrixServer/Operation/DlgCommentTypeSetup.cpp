// DlgCommentTypeSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgCommentTypeSetup.h"
#include "afxdialogex.h"
#include "Parameter.h"

// CDlgCommentTypeSetup 对话框

IMPLEMENT_DYNAMIC(CDlgCommentTypeSetup, CBCGPDialog)

CDlgCommentTypeSetup::CDlgCommentTypeSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgCommentTypeSetup::IDD, pParent)
{

}

CDlgCommentTypeSetup::~CDlgCommentTypeSetup()
{
}

void CDlgCommentTypeSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCommentTypeSetup, CBCGPDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgCommentTypeSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgCommentTypeSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgCommentTypeSetup::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgCommentTypeSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgCommentTypeSetup::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlgCommentTypeSetup 消息处理程序


BOOL CDlgCommentTypeSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectGrid;
	int iNbWidth, iLabelWidth;
	GetDlgItem(IDC_STATIC_GRID_EDIT)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndEditGrid.CreateGrid(rectGrid,this);
	GetDlgItem(IDC_STATIC_NB)->GetWindowRect(&rectGrid);
	iNbWidth = rectGrid.Width();
	GetDlgItem(IDC_STATIC_LABEL)->GetWindowRect(&rectGrid);
	iLabelWidth = rectGrid.Width();
	m_wndEditGrid.InsertColumn (0, _T("Nb"), iNbWidth);
	m_wndEditGrid.InsertColumn (1, _T("Label"), iLabelWidth);

	GetDlgItem(IDC_STATIC_GRID_LIST)->GetWindowRect (&rectGrid);
	ScreenToClient (&rectGrid);
	m_wndListGrid.CreateGrid(rectGrid,this);
	m_wndListGrid.InsertColumn (0, _T("Shot Id"), iNbWidth);
	m_wndListGrid.InsertColumn (1, _T("BreakPoint"), iLabelWidth);

	for(int i=0;i<m_wndEditGrid.GetColumnCount();i++)
	{
		m_wndEditGrid.SetColumnLocked(i,TRUE);
		m_wndListGrid.SetColumnLocked(i,TRUE);
	}
	m_wndEditGrid.AddRow();
	m_wndEditGrid.AdjustLayout();
	MoveTitleToCenter((CWnd*)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCommentTypeSetup::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_wndEditGrid.RemoveAll();
	m_wndListGrid.RemoveAll();
}


void CDlgCommentTypeSetup::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgCommentTypeSetup::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgCommentTypeSetup::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgCommentTypeSetup::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgCommentTypeSetup::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
}
