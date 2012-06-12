// DlgSurveySetup.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "DlgSurveySetup.h"


// CDlgSurveySetup 对话框

IMPLEMENT_DYNAMIC(CDlgSurveySetup, CDialog)

CDlgSurveySetup::CDlgSurveySetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSurveySetup::IDD, pParent)
{

}

CDlgSurveySetup::~CDlgSurveySetup()
{
}

void CDlgSurveySetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSurveySetup, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLYALL, &CDlgSurveySetup::OnBnClickedButtonApplyAll)
	ON_BN_CLICKED(IDC_BUTTON_APPLYSENSOR, &CDlgSurveySetup::OnBnClickedButtonApplySensor)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgSurveySetup::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSurveySetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgSurveySetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgSurveySetup::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CDlgSurveySetup::OnBnClickedButtonReverse)
END_MESSAGE_MAP()

// CDlgSurveySetup 消息处理程序

/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgSurveySetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect oRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB);
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);	// 属性页容器

	m_oTabSurvey.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabSurvey.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);	// 属性页地形
	m_oTabPointCode.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabPointCode.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 2);	// 属性页点代码
	m_oTabSensor.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabSensor.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 3);	// 属性页检波器

	m_oTabWnd.AddTab(&m_oTabSurvey, "    Survey    ");	// 属性页地形
	m_oTabWnd.AddTab(&m_oTabPointCode, "    Point Code    ");	// 属性页地形
	m_oTabWnd.AddTab(&m_oTabSensor, "    Sensor    ");	// 属性页地形

	m_oTabWnd.RecalcLayout();
	m_oTabWnd.SetActiveTab(0);

	OnBnClickedButtonReset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
* OnClose()
* @param void
* @return void
*/
void CDlgSurveySetup::OnClose()
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
void CDlgSurveySetup::OnBnClickedButtonApplyAll()
{
	try
	{
		m_oTabSurvey.OnBnClickedButtonApplyAll();
		m_oTabPointCode.OnBnClickedButtonApplyAll();
		m_oTabSensor.OnBnClickedButtonApplyAll();
	}
	catch (CException* e)
	{
		AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
		return;
	}

	// 保存地形设置数据；ApplyAll
	m_pSiteData->OnApplyAllSurveySetupData();
	// 发送地形设置ApplyAll通知命令帧
	m_pSiteData->OnSendCMDFrameForApplyAllSurveySetupData();
}

/**
* 按钮点击相应；ApplySensor
* @param void
* @return void
*/
void CDlgSurveySetup::OnBnClickedButtonApplySensor()
{
	try
	{
		m_oTabSensor.OnBnClickedButtonApplySensor();
	}
	catch (CException* e)
	{
		AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
		return;
	}	
	// 保存地形设置数据；ApplySensor
	m_pSiteData->OnApplySensorSurveySetupData();
	// 发送地形设置ApplyAll通知命令帧
	m_pSiteData->OnSendCMDFrameForApplySensorSurveySetupData();
}

/**
* 按钮点击相应；Reset
* @param void
* @return void
*/
void CDlgSurveySetup::OnBnClickedButtonReset()
{
	// 加载地形设置数据
	m_pSiteData->OnLoadSurveySetupData();
	m_oTabSurvey.OnBnClickedButtonReset();
	m_oTabPointCode.OnBnClickedButtonReset();
	m_oTabSensor.OnBnClickedButtonReset();
}

/**
* 按钮点击相应；Add
* @param void
* @return void
*/
void CDlgSurveySetup::OnBnClickedButtonAdd()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSurvey.OnBnClickedButtonAdd();
		break;
	case 1:
		m_oTabPointCode.OnBnClickedButtonAdd();
		break;
	case 2:
		m_oTabSensor.OnBnClickedButtonAdd();
		break;
	}
}

/**
* 按钮点击相应；Change
* @param void
* @return void
*/
void CDlgSurveySetup::OnBnClickedButtonChange()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSurvey.OnBnClickedButtonChange();
		break;
	case 1:
		m_oTabPointCode.OnBnClickedButtonChange();
		break;
	case 2:
		m_oTabSensor.OnBnClickedButtonChange();
		break;
	}
}

/**
* 按钮点击相应；Delete
* @param void
* @return void
*/
void CDlgSurveySetup::OnBnClickedButtonDelete()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSurvey.OnBnClickedButtonDelete();
		break;
	case 1:
		m_oTabPointCode.OnBnClickedButtonDelete();
		break;
	case 2:
		m_oTabSensor.OnBnClickedButtonDelete();
		break;
	}
}

/**
* 按钮点击相应；Reverse
* @param void
* @return void
*/
void CDlgSurveySetup::OnBnClickedButtonReverse()
{
	int iActiveTab = m_oTabWnd.GetActiveTab();
	switch(iActiveTab)
	{
	case 0:
		m_oTabSurvey.OnBnClickedButtonReverse();
		break;
	case 1:
		m_oTabPointCode.OnBnClickedButtonReverse();
		break;
	case 2:
		m_oTabSensor.OnBnClickedButtonReverse();
		break;
	}
}
