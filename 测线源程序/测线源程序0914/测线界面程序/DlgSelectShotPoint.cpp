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
	m_pTabSpreadAbsolute = NULL;
}

CDlgSelectShotPoint::~CDlgSelectShotPoint()
{
}

void CDlgSelectShotPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSelectShotPoint, CDialog)
	ON_BN_CLICKED(IDC_OK, &CDlgSelectShotPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CDlgSelectShotPoint::OnBnClickedCancel)
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
	m_oTabSelectShotPoint.m_pTabSpreadAbsolute = m_pTabSpreadAbsolute;
	m_oTabSelectShotPoint.Create("", WS_CHILD | WS_VISIBLE, oRect, &m_oTabWnd, 1);	// 属性页SelectShotPoint
	m_oTabWnd.AddTab(&m_oTabSelectShotPoint, "    SelectShotPoint    ");	// 属性页SelectShotPoint

	m_oTabWnd.RecalcLayout();
	m_oTabWnd.SetActiveTab(0);
	// 加载炮点设置数据
	m_pSiteData->OnLoadShotPointSetupData();
	m_oTabSelectShotPoint.OnReset();
	return TRUE;
}
void CDlgSelectShotPoint::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CBCGPGridRow* pRow = NULL;
	COleVariant oVariantBool;
	COleVariant oVariantString;
	map<unsigned int, unsigned int>::iterator  iter;
	CString str = _T("");
	pRow = m_oTabSelectShotPoint.m_oGridCtrlEdit.GetRow(0);
	oVariantString = pRow->GetItem(1)->GetValue();
	str = oVariantString;
	iter = m_pSiteData->m_oShotPointMap.m_oShotPointMap.find(_ttoi(str));
	if (iter != m_pSiteData->m_oShotPointMap.m_oShotPointMap.end())
	{
		m_pSiteData->m_oSpreadAbsoluteList.m_uiShotPointSelect = iter->second + 1;
		m_pSiteData->OnLoadSpreadSetupData();
		m_pTabSpreadAbsolute->OnBnClickedButtonReset();
	}
	OnOK();
}

void CDlgSelectShotPoint::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
