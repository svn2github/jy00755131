// DlgLoginAdminServer.cpp : 实现文件
//

#include "stdafx.h"
#include "Start.h"
#include "DlgLoginAdminServer.h"


// CDlgLoginAdminServer 对话框

IMPLEMENT_DYNAMIC(CDlgLoginAdminServer, CBCGPDialog)

CDlgLoginAdminServer::CDlgLoginAdminServer(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgLoginAdminServer::IDD, pParent)
	, m_strLabel(_T("428GUI"))
	, m_strURL(_T("192.168.0.12"))
	, m_strUserName(_T("User"))
	, m_strPassword(_T(""))
{
	EnableVisualManagerStyle();
}

CDlgLoginAdminServer::~CDlgLoginAdminServer()
{
	m_hWnd = NULL;
}

void CDlgLoginAdminServer::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITLABEL, m_strLabel);
	DDX_Text(pDX, IDC_EDITURL, m_strURL);
	DDX_Text(pDX, IDC_EDITUSERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDITPASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgLoginAdminServer, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTNCONNECT, &CDlgLoginAdminServer::OnBnClickedBtnconnect)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgLoginAdminServer 消息处理程序

BOOL CDlgLoginAdminServer::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLoginAdminServer::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	DestroyWindow();
	//CBCGPDialog::OnOK();
}

void CDlgLoginAdminServer::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();
	//CBCGPDialog::OnCancel();
}
void CDlgLoginAdminServer::OnDestroy()
{
	CBCGPDialog::OnDestroy();
	// 退出前向主窗口
	GetParent()->SendMessage(WM_DLGLOGINDESTROY,0,0);
	
}

void CDlgLoginAdminServer::PostNcDestroy()
{
	// 作为无模式对话框，自动删除自身
	delete this;
	CBCGPDialog::PostNcDestroy();
}
/**
 * @brief 响应Connect按钮
 * @note  响应Connect按钮，向服务器发送用户登录数据帧
 * @param void
 * @return void
 */
void CDlgLoginAdminServer::OnBnClickedBtnconnect()
{
	UpdateData(TRUE);
	theApp.m_sockAdminServer.SendFrm_UserLogin(m_strUserName,m_strPassword,m_strURL);
}

