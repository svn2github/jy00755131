// LandedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IC_TEST.h"
#include "LandedDlg.h"
#include "afxdialogex.h"
#include "Parameter.h"

// CLandedDlg 对话框

IMPLEMENT_DYNAMIC(CLandedDlg, CDialogEx)

CLandedDlg::CLandedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLandedDlg::IDD, pParent)
{

}

CLandedDlg::~CLandedDlg()
{
}

void CLandedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANDED, m_ctrlUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ctrlPassword);
}


BEGIN_MESSAGE_MAP(CLandedDlg, CDialogEx)
END_MESSAGE_MAP()


// CLandedDlg 消息处理程序
BOOL CLandedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CComboBox*)GetDlgItem(IDC_COMBO_LANDED))->SetCurSel(0);
	return TRUE;
}

void CLandedDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnLand();
	CDialogEx::OnOK();
}


// 登陆
void CLandedDlg::OnLand(void)
{
	CString strUser = _T("");
	CString strPassword = _T("");
	m_ctrlUserName.GetWindowText(strUser);
	m_ctrlPassword.GetWindowText(strPassword);
	if (strUser == _T("Admin"))
	{
		if (strPassword == _T("gxb"))
		{
			m_bAdmin = true;
		}
	}
	else
	{
		m_bAdmin = false;
	}
}
