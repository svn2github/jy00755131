// DlgTestBaseSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "DlgTestBaseSetup.h"


// CDlgTestBaseSetup 对话框

IMPLEMENT_DYNAMIC(CDlgTestBaseSetup, CDialog)

CDlgTestBaseSetup::CDlgTestBaseSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestBaseSetup::IDD, pParent)
{

}

CDlgTestBaseSetup::~CDlgTestBaseSetup()
{
}

void CDlgTestBaseSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTestBaseSetup, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgTestBaseSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgTestBaseSetup::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgTestBaseSetup::OnBnClickedButtonChange)
END_MESSAGE_MAP()

// CDlgTestBaseSetup 消息处理程序

/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgTestBaseSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect oRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB);
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);	// 属性页容器

	m_oTabTestBaseInstrument.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabTestBaseInstrument.m_uiAim = 1;	// 测试对象 1-仪器；2-检波器
	m_oTabTestBaseInstrument.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);	// 属性页测试基本设置，仪器

	m_oTabTestBaseSensor.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabTestBaseSensor.m_uiAim = 2;	// 测试对象 1-仪器；2-检波器
	m_oTabTestBaseSensor.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 2);	// 属性页测试基本设置，检波器

	m_oTabTestLimitInstrument.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabTestLimitInstrument.m_uiAim = 1;	// 测试对象 1-仪器；2-检波器
	m_oTabTestLimitInstrument.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 3);	// 属性页测试判据设置，仪器

	m_oTabTestLimitSensor.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabTestLimitSensor.m_uiAim = 2;	// 测试对象 1-仪器；2-检波器
	m_oTabTestLimitSensor.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 4);	// 属性页测试判据设置，检波器

	m_oTabWnd.AddTab(&m_oTabTestBaseInstrument, "    Instrument Base    ");	// 属性页测试基本设置，仪器
	m_oTabWnd.AddTab(&m_oTabTestBaseSensor, "    Sensor Base    ");	// 属性页测试基本设置，检波器
	m_oTabWnd.AddTab(&m_oTabTestLimitInstrument, "    Instrument Limit    ");	// 属性页测试判据设置，仪器
	m_oTabWnd.AddTab(&m_oTabTestLimitSensor, "    Sensor Limit    ");	// 属性页测试判据设置，检波器

	m_oTabWnd.RecalcLayout();
	m_oTabWnd.SetActiveTab(0);

	m_oTabTestBaseInstrument.OnBnClickedButtonReset();
	m_oTabTestBaseSensor.OnBnClickedButtonReset();
	m_oTabTestLimitInstrument.OnBnClickedButtonReset();
	m_oTabTestLimitSensor.OnBnClickedButtonReset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
* OnClose()
* @param void
* @return void
*/
void CDlgTestBaseSetup::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
//	CDialog::OnClose();
}

/**
* 按钮点击相应；ApplyAll
* @param void
* @return void
*/
void CDlgTestBaseSetup::OnBnClickedButtonApply()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		try
		{
			m_oTabTestBaseInstrument.OnBnClickedButtonApply();
			// Apply测试基本设置数据，仪器
			m_pSiteData->OnApplyTestBaseSetupDataForInstrument();
			// 发送测试基本设置数据变化通知命令帧，仪器
			m_pSiteData->OnSendCMDFrameForApplyTestBaseSetupDataOfInstrument();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 1:
		try
		{
			m_oTabTestBaseSensor.OnBnClickedButtonApply();
			// Apply测试基本设置数据，检波器
			m_pSiteData->OnApplyTestBaseSetupDataForSensor();
			// 发送测试基本设置数据变化通知命令帧，检波器
			m_pSiteData->OnSendCMDFrameForApplyTestBaseSetupDataOfSensor();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 2:
		try
		{
			m_oTabTestLimitInstrument.OnBnClickedButtonApply();
			// Apply测试判据设置数据，仪器
			m_pSiteData->OnApplyTestLimitSetupDataForInstrument();
			// 发送测试判据设置数据变化通知命令帧，仪器
			m_pSiteData->OnSendCMDFrameForApplyTestLimitSetupDataOfInstrument();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	case 3:
		try
		{
			m_oTabTestLimitSensor.OnBnClickedButtonApply();
			// Apply测试判据设置数据，检波器
			m_pSiteData->OnApplyTestLimitSetupDataForSensor();
			// 发送测试判据设置数据变化通知命令帧，检波器
			m_pSiteData->OnSendCMDFrameForApplyTestLimitSetupDataOfSensor();
		}
		catch (CException* e)
		{
			AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
			return;
		}
		break;
	}
}

/**
* 按钮点击相应；Reset
* @param void
* @return void
*/
void CDlgTestBaseSetup::OnBnClickedButtonReset()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		// 加载测试基本设置数据，仪器
		m_pSiteData->OnLoadTestBaseSetupDataForInstrument();
		m_oTabTestBaseInstrument.OnBnClickedButtonReset();
		break;
	case 1:
		// 加载测试基本设置数据，检波器
		m_pSiteData->OnLoadTestBaseSetupDataForSensor();
		m_oTabTestBaseSensor.OnBnClickedButtonReset();
		break;
	case 3:
		// 加载测试判据设置数据，仪器
		m_pSiteData->OnLoadTestLimitSetupDataForInstrument();
		m_oTabTestLimitInstrument.OnBnClickedButtonReset();
		break;
	case 4:
		// 加载测试判据设置数据，检波器
		m_pSiteData->OnLoadTestLimitSetupDataForSensor();
		m_oTabTestLimitSensor.OnBnClickedButtonReset();
		break;
	}
}

/**
* 按钮点击相应；Change
* @param void
* @return void
*/
void CDlgTestBaseSetup::OnBnClickedButtonChange()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabTestBaseInstrument.OnBnClickedButtonChange();
		break;
	case 1:
		m_oTabTestBaseSensor.OnBnClickedButtonChange();
		break;
	case 2:
		m_oTabTestLimitInstrument.OnBnClickedButtonChange();
		break;
	case 3:
		m_oTabTestLimitSensor.OnBnClickedButtonChange();
		break;
	}
}
