// DlgSelectShotPoint.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "DlgSelectShotPoint.h"


// CDlgSelectShotPoint 对话框

IMPLEMENT_DYNAMIC(CDlgSelectShotPoint, CDialog)

CDlgSelectShotPoint::CDlgSelectShotPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectShotPoint::IDD, pParent)
{
	m_pSiteData = NULL;
}

CDlgSelectShotPoint::~CDlgSelectShotPoint()
{
}

void CDlgSelectShotPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSelectShotPoint, CDialog)
END_MESSAGE_MAP()


// CDlgSelectShotPoint 消息处理程序
/**
* OnInitDialog()
* @param void
* @return BOOL
*/
BOOL CDlgSelectShotPoint::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect oRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TAB);
	pWnd->GetWindowRect(&oRect);
	ScreenToClient(&oRect);

	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);	// 属性页容器
	m_oTabSelectShotPoint.m_pSiteData = m_pSiteData;
	m_oTabSelectShotPoint.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);	// 属性页SelectShotPoint
	m_oTabWnd.AddTab(&m_oTabSelectShotPoint, "    SelectShotPoint    ");	// 属性页SelectShotPoint

	m_oTabWnd.RecalcLayout();
	m_oTabWnd.SetActiveTab(0);

// 	// 加载绝对排列设置数据
// 	m_pSiteData->OnLoadSpreadSetupDataForAbsolute();
// 	// 加载相对排列设置数据
// 	m_pSiteData->OnLoadSpreadSetupDataForGeneric();
	m_oTabSelectShotPoint.OnReset();
	return TRUE;
}