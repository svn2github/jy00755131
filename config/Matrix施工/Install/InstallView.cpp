// InstallView.cpp : CInstallView 类的实现
//

#include "stdafx.h"
#include "Install.h"

#include "InstallDoc.h"
#include "InstallView.h"
#include "SetupScalabilityView.h"
#include "SetupMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInstallView

IMPLEMENT_DYNCREATE(CInstallView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CInstallView, CBCGPFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNAPPLY, &CInstallView::OnBnClickedBtnapply)
END_MESSAGE_MAP()

// CInstallView 构造/析构

CInstallView::CInstallView()
: CBCGPFormView(CInstallView::IDD)
, m_InstallParam()
{
	// TODO: 在此处添加构造代码
	EnableVisualManagerStyle ();
}

CInstallView::~CInstallView()
{
}

void CInstallView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_BTNAPPLY, m_BtnApply);
}

BOOL CInstallView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CBCGPFormView::PreCreateWindow(cs);
}


// CInstallView 诊断

#ifdef _DEBUG
void CInstallView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CInstallView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CInstallDoc* CInstallView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInstallDoc)));
	return (CInstallDoc*)m_pDocument;
}
#endif //_DEBUG



int CInstallView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}
/**
 * @brief 创建TAB窗口
 * @note  创建TAB窗口
 * @param void
 * @return void
 */
void CInstallView::CreateTabWnd(void)
{
	CRect rectTab;
//	m_ctrlTabLocation.GetWindowRect (&rectTab);
//	ScreenToClient (&rectTab);

	GetClientRect(&rectTab);
	rectTab.bottom-=60;
	m_wndTab.Create (CBCGPTabWnd::STYLE_3D_ONENOTE, rectTab, this, (UINT)-1,CBCGPTabWnd::LOCATION_TOP);

	m_MainView.Create(IDD_SETUPMAINVIEW,&m_wndTab);
//	m_MainView.Create(_T("CSetupMainView"),_T("Main"),WS_CHILD|WS_VISIBLE|WS_VSCROLL,rectTab,this,2,NULL);	
	m_MainView.SetWindowText (_T("Main"));
	m_ScalabilityView.Create(IDD_SETUPSCALABILITYVIEW,&m_wndTab);
	m_ScalabilityView.SetWindowText (_T("Scalability"));

	m_wndTab.AddTab (&m_MainView, _T(" Main "), 0, FALSE);
	m_wndTab.AddTab (&m_ScalabilityView, _T(" Scalability "), 1, FALSE);

	m_wndTab.EnableTabSwap(FALSE);
	m_wndTab.SetDrawFrame(TRUE);
	m_wndTab.SetButtonsVisible(FALSE);
	m_wndTab.SetFlatFrame(FALSE,TRUE);
	
	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	m_wndTab.SetFocus();
	
	// 读XML文件并初始化参数
	m_ParseXML.ParseInstallParam(theApp.m_strLocalXMLFile,&m_InstallParam);
	m_MainView.m_nVibratorType = m_InstallParam.m_nVibratorType;
	m_MainView.m_nBlasterType = m_InstallParam.m_nBlasterType;
	m_MainView.SetBaudRate(m_InstallParam.m_nBlasterBaudRate);
	m_MainView.m_nNbOfPlotters = m_InstallParam.m_nNbOfPlotters-1;
	m_MainView.m_nPlotterType1 = m_InstallParam.m_nPlotterType1;
	m_MainView.m_strPlotterName1 = m_InstallParam.m_strPlotterName1;
	m_MainView.m_strPlotterMacAddress1 = m_InstallParam.m_strPlotterMacAddress1;
	m_MainView.m_nPlotterType2 = m_InstallParam.m_nPlotterType2;
	m_MainView.m_strPlotterName2 = m_InstallParam.m_strPlotterName2;
	m_MainView.m_strPlotterMacAddress2 = m_InstallParam.m_strPlotterMacAddress2;
	m_MainView.UpdateData(FALSE);
}

void CInstallView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	
	CreateTabWnd();
}

void CInstallView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	
	if (m_wndTab.GetSafeHwnd())
	{
		m_wndTab.SetWindowPos (NULL, 10, 10, cx, cy-60, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if(m_BtnApply.GetSafeHwnd())
	{
		m_BtnApply.SetWindowPos (NULL, cx/2-30, cy-50, 100, 30, SWP_NOZORDER  |SWP_DRAWFRAME);
	}
}

/**
 * @brief 保存参数设置
 * @note  用户点击Apply，保存参数
 * @param void
 * @return void
 */
void CInstallView::OnBnClickedBtnapply()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MainView.UpdateData();
	m_ScalabilityView.UpdateData();

	m_InstallParam.m_nVibratorType=m_MainView.m_nVibratorType;
	m_InstallParam.m_nBlasterType = m_MainView.m_nBlasterType ;
	m_InstallParam.m_nBlasterBaudRate=m_MainView.GetBaudRate();
	m_InstallParam.m_nNbOfPlotters = m_MainView.m_nNbOfPlotters+1;
	m_InstallParam.m_nPlotterType1= m_MainView.m_nPlotterType1;
	m_InstallParam.m_strPlotterName1 = m_MainView.m_strPlotterName1;
	m_InstallParam.m_strPlotterMacAddress1 = m_MainView.m_strPlotterMacAddress1;
	m_InstallParam.m_nPlotterType2 = m_MainView.m_nPlotterType2;
	m_InstallParam.m_strPlotterName2 = m_MainView.m_strPlotterName2;
	m_InstallParam.m_strPlotterMacAddress2 =m_MainView.m_strPlotterMacAddress2 ;

	// 将配置项写入配置文件中，传送到FTP服务器，并通知其他程序
	if(m_ParseXML.WriteInstallParam(theApp.m_strLocalXMLFile,&m_InstallParam))
	{
		theApp.WriteLog(_T("CInstallView::OnBnClickedBtnapply"),_T("Installation setup file was modified."),LOGTYPE_NORMAL,TRUE);
	}
	else
	{
		theApp.WriteLog(_T("CInstallView::OnBnClickedBtnapply"),_T("Fail to save Installation setup file."),LOGTYPE_ERROR,TRUE);
	}
	theApp.SaveXMLToFTPServer();
}
