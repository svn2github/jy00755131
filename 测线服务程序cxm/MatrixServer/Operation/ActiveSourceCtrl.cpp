// ActiveSourceCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "ActiveSourceCtrl.h"



// CActiveSourceCtrl 对话框

IMPLEMENT_DYNCREATE(CActiveSourceCtrl, CBCGPFormView)
CActiveSourceCtrl::CActiveSourceCtrl()
	: CBCGPFormView(CActiveSourceCtrl::IDD)
{
	m_iBlasterStatus = 0;
}

CActiveSourceCtrl::~CActiveSourceCtrl()
{
}

void CActiveSourceCtrl::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTONGO, m_btnGo);
	DDX_Control(pDX, IDC_BUTTONSTOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTONABORT, m_btnAbort);
	DDX_Control(pDX, IDC_STATIC_ACTIVESOURCEGROUP, m_staticActiveSourceGroup);
	DDX_Control(pDX, IDC_STATIC_BLASTER, m_staticBlaster);
	DDX_Control(pDX, IDC_STATIC_UPHOLE, m_staticUphole);
	DDX_Control(pDX, IDC_STATIC_TB, m_staticTB);
	DDX_Control(pDX, IDC_EDIT_UPHOLE, m_editUphole);
	DDX_Control(pDX, IDC_EDIT_TB, m_editTB);
}


BEGIN_MESSAGE_MAP(CActiveSourceCtrl, CBCGPFormView)
	ON_BN_CLICKED(IDC_BUTTONGO, &CActiveSourceCtrl::OnBnClickedButtongo)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CActiveSourceCtrl::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONABORT, &CActiveSourceCtrl::OnBnClickedButtonabort)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CActiveSourceCtrl 消息处理程序

BOOL CActiveSourceCtrl::CreateView(CWnd* pParent, CRect rect, CCreateContext* pContext)
{
	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CActiveSourceCtrl!\n");
		return FALSE;
	}
	GetDlgItem(IDC_EDIT_UPHOLE)->SetWindowText(_T("0.00"));
	GetDlgItem(IDC_EDIT_TB)->SetWindowText(_T("0.00"));
	GetDlgItem(IDC_BUTTONGO)->EnableWindow(TRUE);
	m_oBlasterIconNoFire = AfxGetApp()->LoadIcon(IDI_ICON_RED);
	m_oBlasterIconNormal = AfxGetApp()->LoadIcon(IDI_ICON_GREEN);
	m_oBlasterIconWarning = AfxGetApp()->LoadIcon(IDI_ICON_ORANGE);
	return TRUE;
}

void CActiveSourceCtrl::OnBnClickedButtongo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CActiveSourceCtrl::OnBnClickedButtonstop()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CActiveSourceCtrl::OnBnClickedButtonabort()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CActiveSourceCtrl::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	UINT nFlags = SWP_NOZORDER |SWP_DRAWFRAME;
	if(m_btnGo.GetSafeHwnd())
	{
		m_btnGo.SetWindowPos (NULL, cx/20, cy/5, cx/10, cy*3/5, nFlags);
	}
	if(m_btnStop.GetSafeHwnd())
	{
		m_btnStop.SetWindowPos (NULL, cx/5, cy/5, cx/10, cy*3/5, nFlags);
	}
	if(m_btnAbort.GetSafeHwnd())
	{
		m_btnAbort.SetWindowPos (NULL, cx*7/20, cy/5, cx/10, cy*3/5, nFlags);
	}
	if (m_staticActiveSourceGroup.GetSafeHwnd())
	{
		m_staticActiveSourceGroup.SetWindowPos(NULL, cx/2, cy/10, cx*9/20, cy*4/5, nFlags);
	}
	if (m_staticBlaster.GetSafeHwnd())
	{
		m_staticBlaster.SetWindowPos(NULL, cx*109/200, cy*1/5, cx*9/100, cy*1/5, nFlags);
	}
	if (m_staticUphole.GetSafeHwnd())
	{
		m_staticUphole.SetWindowPos(NULL, cx*17/25, cy*1/5, cx*9/100, cy*1/5, nFlags);
	}
	if (m_staticTB.GetSafeHwnd())
	{
		m_staticTB.SetWindowPos(NULL, cx*163/200, cy*1/5, cx*9/100, cy*1/5, nFlags);
	}
	if (m_editUphole.GetSafeHwnd())
	{
		m_editUphole.SetWindowPos(NULL, cx*17/25, cy/2, cx*9/100, cy*3/10, nFlags);
	}
	if (m_editTB.GetSafeHwnd())
	{
		m_editTB.SetWindowPos(NULL, cx*163/200, cy/2, cx*9/100, cy*3/10, nFlags);
	}
}


void CActiveSourceCtrl::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRect rc;
	GetClientRect(&rc);
	SendMessage(WM_SIZE,SIZE_RESTORED,MAKELPARAM(rc.Width(),rc.Height()));
}


void CActiveSourceCtrl::OnDraw(CDC* pDC)
{
	// TODO: 在此添加专用代码和/或调用基类
	CRect rectClient, rectIcon;
	GetClientRect(rectClient);
	int cx = rectClient.right;
	int cy = rectClient.bottom;
	HICON hIcon;
	rectIcon.left = cx*59/100 - cy*3/20;
	rectIcon.top = cy/2;
	rectIcon.right = rectIcon.left + cy*3/10;
	rectIcon.bottom = rectIcon.top + cy*3/10;
	switch(m_iBlasterStatus)
	{
	case 1:
		hIcon = m_oBlasterIconNoFire;
		::DrawIconEx(pDC->m_hDC, rectIcon.left, rectIcon.top, hIcon, rectIcon.Width(), rectIcon.Height(), 0, NULL, DI_NORMAL);
		break;
	case 2:
		hIcon = m_oBlasterIconNormal;
		::DrawIconEx(pDC->m_hDC, rectIcon.left, rectIcon.top, hIcon, rectIcon.Width(), rectIcon.Height(), 0, NULL, DI_NORMAL);
		break;
	default:
		hIcon = m_oBlasterIconWarning;
		::DrawIconEx(pDC->m_hDC, rectIcon.left, rectIcon.top, hIcon, rectIcon.Width(), rectIcon.Height(), 0, NULL, DI_NORMAL);
		break;
	}
}
