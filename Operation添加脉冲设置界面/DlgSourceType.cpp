// DlgSourceType.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "DlgSourceType.h"
#include "MainFrm.h"

// CDlgSourceType 对话框

IMPLEMENT_DYNAMIC(CDlgSourceType, CBCGPDialog)

CDlgSourceType::CDlgSourceType(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgSourceType::IDD, pParent)
{
	EnableVisualManagerStyle();
}

CDlgSourceType::~CDlgSourceType()
{
}

void CDlgSourceType::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICTAB, m_ctrlTabLocation);
	DDX_Control(pDX, IDC_BTNAPPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTNRESET, m_btnReset);
}


BEGIN_MESSAGE_MAP(CDlgSourceType, CBCGPDialog)
	ON_BN_CLICKED(IDC_BTNAPPLY, &CDlgSourceType::OnBnClickedBtnapply)
	ON_BN_CLICKED(IDC_BTNRESET, &CDlgSourceType::OnBnClickedBtnreset)
END_MESSAGE_MAP()


// CDlgSourceType 消息处理程序

BOOL CDlgSourceType::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectTab;

	m_ctrlTabLocation.GetWindowRect (&rectTab);
	ScreenToClient (&rectTab);

	m_wndTab.Create (CBCGPTabWnd::STYLE_3D_ONENOTE, rectTab, this, 1,CBCGPTabWnd::LOCATION_TOP);

	m_DlgExplo.Create(IDD_SOURCEEXPLODLG,&m_wndTab);
	m_DlgExplo.SetWindowText (_T("Explo"));
	m_DlgVibro.Create(IDD_SOURCEVIBRODLG,&m_wndTab);
	m_DlgVibro.SetWindowText (_T("Vibro"));

	m_wndTab.AddTab (&m_DlgExplo, _T(" Explo "), 0, FALSE);
	m_wndTab.AddTab (&m_DlgVibro, _T(" Vibro "), 1, FALSE);

	m_wndTab.EnableTabSwap(FALSE);
	m_wndTab.SetDrawFrame(TRUE);
	m_wndTab.SetButtonsVisible(FALSE);
	m_wndTab.SetFlatFrame(FALSE,TRUE);
	// TabWnd.SetActiveTabBoldFont(TRUE);
	//CBCGPTabWnd::Style style1 = CBCGPTabWnd ::STYLE_3D ;
	//m_wndTab.ModifyTabStyle(style1);
	//m_wndTab.SetLocation (CBCGPTabWnd ::LOCATION_TOP);
	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
/**
 * @brief 点击“Apply”按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgSourceType::OnBnClickedBtnapply()
{
	CMainFrame*	pFrm = (CMainFrame*)AfxGetMainWnd();
	if(!pFrm)
		return;
	
	CShotSources OldShotSources;
	// 先临时保存内存中对象
	OldShotSources.Copy(&pFrm->m_AllShotSrcs);
	// 删除数组，但不释放放炮对象
	pFrm->m_AllShotSrcs.m_AllSrc.RemoveAll();
	// 读取网格中对象，保存到pFrm->m_AllShotSrcs
	m_DlgExplo.Apply(&OldShotSources,&pFrm->m_AllShotSrcs);
	m_DlgVibro.Apply(&OldShotSources,&pFrm->m_AllShotSrcs);
	pFrm->m_wndActiveSource.LoadShotSources(&pFrm->m_AllShotSrcs);
	// 释放掉内存中旧的放炮表
	OldShotSources.RemoveAll();
}
/**
 * @brief 点击“Reset”按钮
 * @note  
 * @param  
 * @return 
 */
void CDlgSourceType::OnBnClickedBtnreset()
{
	m_DlgExplo.Load();
	m_DlgVibro.Load();
}
