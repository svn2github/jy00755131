// DlgLookSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "DlgLookSetup.h"


// CDlgLookSetup 对话框

IMPLEMENT_DYNAMIC(CDlgLookSetup, CDialog)

CDlgLookSetup::CDlgLookSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLookSetup::IDD, pParent)
{

}

CDlgLookSetup::~CDlgLookSetup()
{
}

void CDlgLookSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLookSetup, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgLookSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgLookSetup::OnBnClickedButtonReset)
END_MESSAGE_MAP()

// CDlgLookSetup 消息处理程序

/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgLookSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	OnBnClickedButtonReset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
* OnClose()
* @param void
* @return void
*/
void CDlgLookSetup::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
//	CDialog::OnClose();
}

/**
* 按钮点击相应；Apply
* @param void
* @return void
*/
void CDlgLookSetup::OnBnClickedButtonApply()
{
	m_pSiteData->m_oLookSetupData.m_uiLookAuto = IsDlgButtonChecked(IDC_CHECK_LOOKAUTO);
	m_pSiteData->m_oLookSetupData.m_uiTestResistance = IsDlgButtonChecked(IDC_CHECK_RESISTANCE);
	m_pSiteData->m_oLookSetupData.m_uiTestTilt = IsDlgButtonChecked(IDC_CHECK_TILT);
	m_pSiteData->m_oLookSetupData.m_uiTestLeakage = IsDlgButtonChecked(IDC_CHECK_LEAKAGE);
	// 保存Look设置数据；Apply
	m_pSiteData->OnApplyLookSetupData();
	// 发送Look设置Apply通知命令帧
	m_pSiteData->OnSendCMDFrameForApplyLookSetupData();
}

/**
* 按钮点击相应；Reset
* @param void
* @return void
*/
void CDlgLookSetup::OnBnClickedButtonReset()
{
	// 加载Look设置数据
	m_pSiteData->OnLoadLookSetupData();
	CheckDlgButton(IDC_CHECK_LOOKAUTO, m_pSiteData->m_oLookSetupData.m_uiLookAuto);
	CheckDlgButton(IDC_CHECK_RESISTANCE, m_pSiteData->m_oLookSetupData.m_uiTestResistance);
	CheckDlgButton(IDC_CHECK_TILT, m_pSiteData->m_oLookSetupData.m_uiTestTilt);
	CheckDlgButton(IDC_CHECK_LEAKAGE, m_pSiteData->m_oLookSetupData.m_uiTestLeakage);
}
