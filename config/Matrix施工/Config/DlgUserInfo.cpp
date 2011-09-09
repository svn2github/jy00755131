// DlgUserInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "Config.h"
#include "DlgUserInfo.h"


// CDlgUserInfo 对话框

IMPLEMENT_DYNAMIC(CDlgUserInfo, CBCGPDialog)

CDlgUserInfo::CDlgUserInfo(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgUserInfo::IDD, pParent)
	, m_strUserInfo(_T(""))
{
	EnableVisualManagerStyle();
}

CDlgUserInfo::~CDlgUserInfo()
{
}

void CDlgUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITUSERINFO, m_strUserInfo);
}


BEGIN_MESSAGE_MAP(CDlgUserInfo, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTNOK, &CDlgUserInfo::OnBnClickedBtnok)
	ON_BN_CLICKED(IDC_BTNAPPLY, &CDlgUserInfo::OnBnClickedBtnapply)
	ON_BN_CLICKED(IDC_BTNRESET, &CDlgUserInfo::OnBnClickedBtnreset)
END_MESSAGE_MAP()


// CDlgUserInfo 消息处理程序
BOOL CDlgUserInfo::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OnBnClickedBtnreset();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgUserInfo::OnBnClickedBtnok()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedBtnapply();
	OnOK();
}

void CDlgUserInfo::OnBnClickedBtnapply()
{
	CConfigParseXML  ParseXML;
	UpdateData(TRUE);
	ParseXML.WriteUserInfo(theApp.m_strLocalXMLFile,m_strUserInfo);
	theApp.WriteLog(_T("CDlgUserInfo::OnBnClickedBtnapply"),_T("User info setup has been modified!"),LOGTYPE_NORMAL,TRUE);
	theApp.SaveXMLToFTPServer();
}

void CDlgUserInfo::OnBnClickedBtnreset()
{
	CConfigParseXML  ParseXML;
	ParseXML.ParseUserInfo(theApp.m_strLocalXMLFile,m_strUserInfo);
	UpdateData(FALSE);
}

