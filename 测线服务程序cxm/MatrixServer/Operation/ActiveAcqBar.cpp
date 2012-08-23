#include "StdAfx.h"
#include "Operation.h"
#include "ActiveAcqBar.h"
#include "OptParam.h"

CActiveAcqBar::CActiveAcqBar(void)
	: m_bReload(true)
	, m_pOptSetupData(NULL)
{
}


CActiveAcqBar::~CActiveAcqBar(void)
{
}
BEGIN_MESSAGE_MAP(CActiveAcqBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
END_MESSAGE_MAP()


int CActiveAcqBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty ();
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
		TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_GridView.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Grid View!\n");
		return -1;
	}
	BCGP_GRID_COLOR_DATA clrData;
	clrData = m_GridView.GetColorTheme();
	clrData.m_clrBackground = ::GetSysColor(COLOR_3DFACE);
	clrData.m_EvenColors.m_clrBackground = RGB(255,255,255);	// 奇数行
	clrData.m_OddColors.m_clrBackground = RGB(250,250,250);		// 偶数行

	m_GridView.SetColorTheme(clrData);	
	m_GridView.SetReadOnly();	
	m_GridView.SetSingleSel(TRUE);
	m_GridView.SetWholeRowSel(TRUE);	
	m_GridView.EnableHeader (TRUE, BCGP_GRID_HEADER_HIDE_ITEMS);
	m_GridView.EnableDragSelection(FALSE);

	// 设置表头
	SetGridHead();
	m_ImageList.Create (IDB_BITMAP_ACQ, 16, 0, RGB (255, 0, 255));
	m_GridView.SetImageList (&m_ImageList);
	SetTimer(AcqReloadTimerID, AcqReloadTime, NULL);
	return 0;
}


void CActiveAcqBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_GridView.GetSafeHwnd ())
	{
		m_GridView.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}


void CActiveAcqBar::OnDestroy()
{
	CBCGPDockingControlBar::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_GridView.RemoveAll();
	KillTimer(AcqReloadTimerID);
}

// 设置表头
bool CActiveAcqBar::SetGridHead(void)
{
	if(!m_GridView.GetSafeHwnd())		
		return false;
	m_GridView.InsertColumn (0, _T("口"), 50);
	m_GridView.InsertColumn (1, _T("Shot#"), 150);
	m_GridView.SetHeaderAlign (1, HDF_CENTER);
	m_GridView.SetColumnAlign (1, HDF_RIGHT);
	m_GridView.InsertColumn (2, _T("Acq Nb"), 150);
	m_GridView.SetHeaderAlign (2, HDF_CENTER);
	m_GridView.SetColumnAlign (2, HDF_RIGHT);
	m_GridView.InsertColumn (3, _T("Acq Type"), 150);
	m_GridView.SetHeaderAlign (3, HDF_CENTER);
	m_GridView.SetColumnAlign (3, HDF_RIGHT);
	m_GridView.InsertColumn (4, _T("Dump"), 150);
	m_GridView.SetHeaderAlign (4, HDF_CENTER);
	m_GridView.SetColumnAlign (4, HDF_LEFT);
	return true;
}


// 载入全部Acq信息
void CActiveAcqBar::LoadAcqInfos(void)
{
	if (m_bReload == false)
	{
		return;
	}
	list<m_oProcessAcqStruct>::iterator iter;
	m_bReload = false;
	m_GridView.RemoveAll();
	if (m_pOptSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&m_pOptSetupData->m_oSecCommInfo);
	for (iter = m_pOptSetupData->m_olsProcessAcqStruct.begin();
		iter != m_pOptSetupData->m_olsProcessAcqStruct.end(); iter++)
	{
		LoadAcqInfo(&(*iter));
	}
	LeaveCriticalSection(&m_pOptSetupData->m_oSecCommInfo);
}


// 载入单条Acq信息
void CActiveAcqBar::LoadAcqInfo(m_oProcessAcqStruct* pStruct)
{
	CBCGPGridRow* pRow = m_GridView.CreateRow (m_GridView.GetColumnCount());
	pRow->GetItem (0)->SetImage(pStruct->m_uiAcqStatus);
	pRow->GetItem (2)->SetValue (pStruct->m_uiAcqNb);
	pRow->GetItem (3)->SetValue (pStruct->m_uiAcqType);
	if (pStruct->m_uiOutPut == 0)
	{
		pRow->GetItem (4)->SetValue (_T("No Op"));
	}
	else if (pStruct->m_uiOutPut == 1)
	{
		pRow->GetItem (4)->SetValue (_T("Dump"));
	}
	else if (pStruct->m_uiOutPut == 2)
	{
		pRow->GetItem (4)->SetValue (_T("Xdump"));
	}
	m_GridView.AddRow (pRow, TRUE);
}


void CActiveAcqBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}


void CActiveAcqBar::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == AcqReloadTimerID)
	{
		LoadAcqInfos();
	}
	CBCGPDockingControlBar::OnTimer(nIDEvent);
}
