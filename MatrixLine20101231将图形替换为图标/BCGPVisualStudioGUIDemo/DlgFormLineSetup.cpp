// DlgFormLineSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "DlgFormLineSetup.h"


// CDlgFormLineSetup 对话框

IMPLEMENT_DYNAMIC(CDlgFormLineSetup, CDialog)

CDlgFormLineSetup::CDlgFormLineSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormLineSetup::IDD, pParent)
{

}

CDlgFormLineSetup::~CDlgFormLineSetup()
{
}

void CDlgFormLineSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFormLineSetup, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgFormLineSetup::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgFormLineSetup::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_GO, &CDlgFormLineSetup::OnBnClickedButtonGO)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgFormLineSetup::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDlgFormLineSetup::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgFormLineSetup::OnBnClickedButtonDelete)
END_MESSAGE_MAP()

// CDlgFormLineSetup 消息处理程序

/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgFormLineSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect oRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB);
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);	// 属性页容器

	m_oTabFormLine.m_pSiteData = m_pSiteData;	// 现场数据
	m_oTabFormLine.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);	// 属性页FormLine

	m_oTabWnd.AddTab(&m_oTabFormLine, "    FormLine    ");	// 属性页FormLine

	m_oTabWnd.RecalcLayout();
	m_oTabWnd.HideSingleTab(TRUE);

	OnBnClickedButtonReset();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/**
* OnClose()
* @param void
* @return void
*/
void CDlgFormLineSetup::OnClose()
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
void CDlgFormLineSetup::OnBnClickedButtonApply()
{
	try
	{
		m_oTabFormLine.OnBnClickedButtonApply();
	}
	catch (CException* e)
	{
		AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
		return;
	}

	// 保存FormLine设置数据；Apply
	m_pSiteData->OnApplyFormLineSetupData();
}

/**
* 按钮点击相应；Reset
* @param void
* @return void
*/
void CDlgFormLineSetup::OnBnClickedButtonReset()
{
	// 加载FormLine设置数据
	m_pSiteData->OnLoadFormLineSetupData();
	m_oTabFormLine.OnBnClickedButtonReset();
}

/**
* 按钮点击相应；GO
* @param void
* @return void
*/
void CDlgFormLineSetup::OnBnClickedButtonGO()
{
	try
	{
		if(false == m_oTabFormLine.OnBnClickedButtonGO())
		{
			return;
		}
	}
	catch (CException* e)
	{
		AfxMessageBox("Illegal Data.\r\nPlease Check Data.", MB_OK | MB_ICONSTOP);
		return;
	}	
	// 发送FormLine操作GO通知命令帧
	m_pSiteData->OnSendFormLineCMDFrameGO(m_oTabFormLine.m_oFormLineData.m_uiSN);
}

/**
* 按钮点击相应；Add
* @param void
* @return void
*/
void CDlgFormLineSetup::OnBnClickedButtonAdd()
{

	m_oTabFormLine.OnBnClickedButtonAdd();
}

/**
* 按钮点击相应；Change
* @param void
* @return void
*/
void CDlgFormLineSetup::OnBnClickedButtonChange()
{
	m_oTabFormLine.OnBnClickedButtonChange();
}

/**
* 按钮点击相应；Delete
* @param void
* @return void
*/
void CDlgFormLineSetup::OnBnClickedButtonDelete()
{
	m_oTabFormLine.OnBnClickedButtonDelete();
}

/**
*  响应仪器图形视图弹出菜单FormLine设置
* @param unsigned int uiInstrumentType 仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 仪器设备号
* @return void
*/
void CDlgFormLineSetup::OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN)
{
	m_oTabFormLine.OnGraphViewInstrumentFormLine(uiInstrumentType, uiSN);
}