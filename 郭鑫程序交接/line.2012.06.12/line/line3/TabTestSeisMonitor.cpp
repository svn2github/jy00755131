// TabTestSeisMonitor.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "TabTestSeisMonitor.h"


// CTabTestSeisMonitor

IMPLEMENT_DYNAMIC(CTabTestSeisMonitor, CTabPage)

CTabTestSeisMonitor::CTabTestSeisMonitor()
{
}

CTabTestSeisMonitor::~CTabTestSeisMonitor()
{
}


BEGIN_MESSAGE_MAP(CTabTestSeisMonitor, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTabTestSeisMonitor 消息处理程序

int CTabTestSeisMonitor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabTestSeisMonitor::OnDestroy()
{
	CWnd::OnDestroy();
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabTestSeisMonitor::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 45;	
	m_oStaticAbsoluteSpread.Create("Absolute Spread:", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX | WS_DISABLED,
		oRect, this, 0);	// 绝对排列组框
	oRect.left = oRect.left + 10;
	oRect.top = oRect.top + 25;
	oRect.right = oRect.right - 10;
	oRect.bottom = oRect.bottom - 10;
	// cxm 2011-09-20 加入ES_WANTRETURN属性，可回车换行
	m_oEditAbsoluteSpread.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL|ES_WANTRETURN,
		oRect, this, IDC_EDIT_ABSOLUTESPREAD);	// 绝对排列编辑框
}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return bool true：成功；false：失败
*/
bool CTabTestSeisMonitor::OnBnClickedButtonApply()
{
	CString strAbsoluteSpread;
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->GetWindowText(strAbsoluteSpread);
	if("" == strAbsoluteSpread)
	{
		AfxMessageBox("Please Input Absolute Spread.", MB_OK | MB_ICONSTOP);
		return false;
	}
	if(-1 != strAbsoluteSpread.Find(';'))
	{
		AfxMessageBox("Please Check Input Absolute Spread.", MB_OK | MB_ICONSTOP);
		return false;
	}
	strAbsoluteSpread.Replace("\r\n", ";");
	// 清除多余";"
	while(";" == strAbsoluteSpread.Right(1))
	{
		strAbsoluteSpread = strAbsoluteSpread.Left(strAbsoluteSpread.GetLength() - 1);
	}
	CTestProject* pTestProject = m_pSiteData->m_oTestProjectListSeisMonitor.m_olsTestProject.GetHead();
	pTestProject->m_strAbsoluteSpread = strAbsoluteSpread;
	return true;
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabTestSeisMonitor::OnBnClickedButtonReset()
{
	CTestProject* pTestProject = m_pSiteData->m_oTestProjectListSeisMonitor.m_olsTestProject.GetHead();
	CString strAbsoluteSpread = pTestProject->m_strAbsoluteSpread;
	strAbsoluteSpread.Replace(";", "\r\n");
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->SetWindowText(strAbsoluteSpread);
}

/**
* 响应鼠标单击按钮；GO
* @param void
* @return bool true：成功；false：失败
*/
bool CTabTestSeisMonitor::OnBnClickedButtonGO()
{
	CString strAbsoluteSpread;
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->GetWindowText(strAbsoluteSpread);
	if("" == strAbsoluteSpread)
	{
		AfxMessageBox("Please Input Absolute Spread.", MB_OK | MB_ICONSTOP);
		return false;
	}
	return true;
}

/**
* 响应鼠标单击按钮；Clear
* @param void
* @return void
*/
void CTabTestSeisMonitor::OnBnClickedButtonClear()
{
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->SetWindowText("");
}
