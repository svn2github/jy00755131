// DlgSpreadSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "DlgSpreadSetup.h"


// CDlgSpreadSetup 对话框

IMPLEMENT_DYNAMIC(CDlgSpreadSetup, CDialog)

CDlgSpreadSetup::CDlgSpreadSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpreadSetup::IDD, pParent)
{

}

CDlgSpreadSetup::~CDlgSpreadSetup()
{
}

void CDlgSpreadSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSpreadSetup, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgSpreadSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgSpreadSetup::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSpreadSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgSpreadSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgSpreadSetup::OnBnClickedButtonDelete)
	//2012-03-01
//	ON_BN_CLICKED(IDC_BUTTON_SELECTSHOTPOINT, &CDlgSpreadSetup::OnBnClickedButtonSelectShotPoint)
END_MESSAGE_MAP()

// CDlgSpreadSetup 消息处理程序

/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgSpreadSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect oRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB);
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);	// 属性页容器

	m_oTabSpreadAbsolute.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabSpreadAbsolute.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);	// 属性页SpreadAbsolute
	m_oTabSpreadGeneric.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabSpreadGeneric.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 2);	// 属性页SpreadGeneric
	//2012-03-01
//	m_oDlgSelectShotPoint.m_pSiteData = m_pSiteData;
//	m_oDlgSelectShotPoint.m_pTabSpreadAbsolute = &m_oTabSpreadAbsolute;

	m_oTabWnd.AddTab(&m_oTabSpreadAbsolute, "    Absolute    ");	// 属性页SpreadAbsolute
	m_oTabWnd.AddTab(&m_oTabSpreadGeneric, "    Generic    ");	// 属性页SpreadGeneric

	m_oTabWnd.RecalcLayout();
	m_oTabWnd.SetActiveTab(0);

	// 加载绝对排列设置数据
	m_pSiteData->OnLoadSpreadSetupDataForAbsolute();
	// 加载相对排列设置数据
	m_pSiteData->OnLoadSpreadSetupDataForGeneric();

 	m_oTabSpreadAbsolute.OnBnClickedButtonReset();
 	m_oTabSpreadGeneric.OnBnClickedButtonReset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
* OnClose()
* @param void
* @return void
*/
void CDlgSpreadSetup::OnClose()
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
void CDlgSpreadSetup::OnBnClickedButtonApply()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		try
		{
			m_oTabSpreadAbsolute.OnBnClickedButtonApply();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}

		// Apply绝对排列设置数据
		m_pSiteData->OnApplySpreadSetupDataForAbsolute();
		// 发送测试绝对排列设置数据变化通知命令帧
		m_pSiteData->OnSendCMDFrameForApplySpreadSetupDataForAbsolute();
		break;
	case 1:
		try
		{
			m_oTabSpreadGeneric.OnBnClickedButtonApply();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}

		// Apply相对排列设置数据
		m_pSiteData->OnApplySpreadSetupDataForGeneric();
		// 发送测试相对排列设置数据变化通知命令帧
		m_pSiteData->OnSendCMDFrameForApplySpreadSetupDataForGeneric();
		break;
	}
}

/**
* 按钮点击相应；Reset
* @param void
* @return void
*/
void CDlgSpreadSetup::OnBnClickedButtonReset()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		// 加载绝对排列设置数据
		m_pSiteData->OnLoadSpreadSetupDataForAbsolute();
		m_oTabSpreadAbsolute.OnBnClickedButtonReset();
		break;
	case 1:
		// 加载相对排列设置数据
		m_pSiteData->OnLoadSpreadSetupDataForGeneric();
		m_oTabSpreadGeneric.OnBnClickedButtonReset();
		break;
	}
}

/**
* 按钮点击相应；Add
* @param void
* @return void
*/
void CDlgSpreadSetup::OnBnClickedButtonAdd()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSpreadAbsolute.OnBnClickedButtonAdd();
		break;
	case 1:
		m_oTabSpreadGeneric.OnBnClickedButtonAdd();
		break;
	}	
}

/**
* 按钮点击相应；Change
* @param void
* @return void
*/
void CDlgSpreadSetup::OnBnClickedButtonChange()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSpreadAbsolute.OnBnClickedButtonChange();
		break;
	case 1:
		m_oTabSpreadGeneric.OnBnClickedButtonChange();
		break;
	}
}

/**
* 按钮点击相应；Delete
* @param void
* @return void
*/
void CDlgSpreadSetup::OnBnClickedButtonDelete()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSpreadAbsolute.OnBnClickedButtonDelete();
		break;
	case 1:
		m_oTabSpreadGeneric.OnBnClickedButtonDelete();
		break;
	}
}
//2012-03-01
/*void CDlgSpreadSetup::OnBnClickedButtonSelectShotPoint()
{
	m_oDlgSelectShotPoint.DoModal();
}*/