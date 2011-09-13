// DlgSPSSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "Config.h"
#include "DlgSPSSetup.h"


// DlgSPSSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSPSSetup, CBCGPDialog)

CDlgSPSSetup::CDlgSPSSetup(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CDlgSPSSetup::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgSPSSetup::~CDlgSPSSetup()
{
}

void CDlgSPSSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSPSSetup, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTNLOADSPS, &CDlgSPSSetup::OnBnClickedBtnloadsps)
	ON_BN_CLICKED(IDC_BTNGENOPTXML, &CDlgSPSSetup::OnBnClickedBtngenoptxml)
	ON_BN_CLICKED(IDC_BTNGENLINEXML, &CDlgSPSSetup::OnBnClickedBtngenlinexml)
END_MESSAGE_MAP()

// DlgSPSSetup 消息处理程序

void CDlgSPSSetup::OnBnClickedBtnloadsps()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgSPSSetup::OnBnClickedBtngenoptxml()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgSPSSetup::OnBnClickedBtngenlinexml()
{
	// TODO: 在此添加控件通知处理程序代码
}
