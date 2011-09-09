#include "stdafx.h"
#include "Operation.h"
#include "GridWorkspace.h"
#include "MainFrm.h"

CGridWorkspace::CGridWorkspace(void)
{
	
}

CGridWorkspace::~CGridWorkspace(void)
{
}
BEGIN_MESSAGE_MAP(CGridWorkspace, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()
/**
 * @brief 重载OnCreate，创建可浮动窗口
 * @note  创建可浮动窗口，窗口内嵌入网格，用于显示各种状态的炮点
 * @param LPCREATESTRUCT lpCreateStruct
 * @return int
 */
int CGridWorkspace::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
  
//    CBCGPTabbedControlBar:m_bTabsAlwaysTop = TRUE;
	// TODO:  在此添加您专用的创建代码	
// 	CRuntimeClass* pFactory = RUNTIME_CLASS(CBCGPTabView);
// 	m_pTabView = (CBCGPTabView *)(pFactory->CreateObject());

	// m_pTabView->m_bTabsAlwaysTop = TRUE; 
	// SetDockMode(BCGP_DT_IMMEDIATE);
	
	CRect rect(0,0,500,200);
	if (!m_GridView.Create(WS_CHILD|WS_VISIBLE,rect,this,1))
	{
		TRACE0("Warning: couldn't create Grid View!\n");
		return FALSE;
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
	// SetGridHead();

//	pGridCtrl->LoadState (_T("VPGrid"));
//	LoadState (_T("GridWorkSpace"));
	return 0;
}

void CGridWorkspace::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_GridView.GetSafeHwnd ())
	{
		m_GridView.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CGridWorkspace::OnDestroy()
{
//	SaveState (_T("GridWorkSpace"));
	CBCGPDockingControlBar::OnDestroy();

	m_GridView.RemoveAll();
//	m_GridView.OnDestroy();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////// CVPGridWorkspace /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CVPGridWorkspace, CGridWorkspace)
	ON_WM_CREATE()
END_MESSAGE_MAP()

CVPGridWorkspace :: CVPGridWorkspace()
{

}
CVPGridWorkspace ::~CVPGridWorkspace()
{

}

/**
 * @brief 重载OnCreate，创建可浮动窗口
 * @note  创建可浮动窗口，窗口内嵌入网格，用于显示各种状态的炮点
 * @param LPCREATESTRUCT lpCreateStruct
 * @return int
 */
int CVPGridWorkspace::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGridWorkspace::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 设置网格属性
	m_ImageList.Create (IDB_BITMAPVPSTATE,16,0,RGB (255, 0, 255));
	m_GridView.SetImageList (&m_ImageList);
	SetGridHead();
	return 0;
}
bool CVPGridWorkspace::SetGridHead(void)
{		
	return true;
}
void CVPGridWorkspace::LoadShotPoints(CShotPoints* pAllVP)
{
	return;
}
void CVPGridWorkspace::LoadShotPoint(CShotPoint* pVP)
{
	return;
}

/**
 * @brief 设置放炮点的状态
 * @note  依据炮点的NB编号值，找到对应的炮点对象，并设置该炮点的状态
 * @param  CShotPoint* pShotPoint, 炮点对象
 * @param  unsigned char byState，炮点状态
 * @return 如果找到相应的炮点，则返回炮点在网格中行号；反之则返回-1
 */
int CVPGridWorkspace::RefreshShotPointState(CShotPoint* pShotPoint)
{
	if(!m_GridView || !pShotPoint)
		return -1;
	DWORD dwShotNb = pShotPoint->m_dwShotNb;
	CBCGPGridRow *pRow=NULL;
	int nRowCount = m_GridView.GetRowCount();
	_variant_t  vt;
	for(int i=0;i<nRowCount;i++)
	{
		pRow = m_GridView.GetRow(i);
		vt = pRow->GetItem(1)->GetValue();
		if(vt.ulVal== dwShotNb)
		{
			pRow->GetItem(1)->SetImage(pShotPoint->m_VPState);
			return i;
		}
	}
	return -1;
}
/**
 * @brief 得到网格中当前选中的炮点编号
 * @note  该函数得到当前网格中选中的炮点编号
 * @param  DWORD* pdwNb，保存选中的炮点编号
 * @return 如果存在选中炮点，则返回true；反之则返回false
 */
bool CVPGridWorkspace::GetActiveShotPointNb(DWORD* pdwNb)
{
	if(pdwNb==NULL)
		return false;

	CBCGPGridRow* pRow= m_GridView.GetCurSel();
	if(NULL==pRow)
		return false;
	CBCGPGridItem* pItem=pRow->GetItem(1);
	_variant_t vt = pItem->GetValue();
	try
	{
		vt.ChangeType(VT_UI4);
		*pdwNb= vt.ulVal;
		return true;
	}
	catch (CException* )
	{
		return false;
	}	

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////// CVPAllGridWnd /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CVPAllGridWnd, CGridWorkspace)
	ON_WM_CONTEXTMENU()	
END_MESSAGE_MAP()

//ON_COMMAND(ID_VPSHOT_ONE, OnVpshotOne)
CVPAllGridWnd::CVPAllGridWnd(void)
{
}

CVPAllGridWnd::~CVPAllGridWnd(void)
{
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 创建 All VP 表格标题

    修改历史：

*******************************************************************************/
bool CVPAllGridWnd::SetGridHead(void)
{	
	if(!m_GridView.GetSafeHwnd())
		return false;
	
	m_GridView.InsertColumn (0, _T(" 口 "), 50);
	m_GridView.InsertColumn (1, _T("Shot Nb"), 80);
	m_GridView.InsertColumn (2, _T("SourcePoint\n\rLine"), 150);
	m_GridView.InsertColumn (3, _T("SourcePointNumber"), 150);
	m_GridView.InsertColumn (4, _T("SourcePointIndex"), 150);
	m_GridView.InsertColumn (5, _T("Sfl"), 100);
	m_GridView.InsertColumn (6, _T("Sfn"), 100);
	m_GridView.InsertColumn (7, _T("Spread#"), 100);
	m_GridView.InsertColumn (8, _T("SuperSpread"), 100);
	m_GridView.InsertColumn (9, _T("ProcessType"), 100);
	m_GridView.InsertColumn (10, _T("Comment"), 100);
	m_GridView.InsertColumn (11, _T("Swath"), 100);	
	return true;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 装载所有状态的放炮表，VP ALL

    修改历史：

*******************************************************************************/
void CVPAllGridWnd::LoadShotPoints(CShotPoints* pAllVP)
{
	if(NULL==pAllVP)
		return;

	int nCount = pAllVP->GetCount();
	m_GridView.RemoveAll();
	for (int nRow = 0; nRow < nCount; nRow++)
	{
		LoadShotPoint(pAllVP->GetShotPoint(nRow));
	}
	m_GridView.AdjustLayout ();
	return;
}
////////////////////////////////////////////////////////////////////////////
// 增加一条放炮记录
void CVPAllGridWnd::LoadShotPoint(CShotPoint* pVP)
{
	if(NULL==pVP)
		return ;
	CBCGPGridRow* pRow = m_GridView.CreateRow (12);
	// pRow->GetItem (0)->SetValue (pVP->m_VPState);
	pRow->GetItem (0)->SetImage(pVP->m_VPState);
	pRow->GetItem (1)->SetValue (pVP->m_dwShotNb);
	pRow->GetItem (2)->SetValue (pVP->m_fSourceLine);
	pRow->GetItem (3)->SetValue (pVP->m_fSourceNb);
	pRow->GetItem (4)->SetValue (pVP->m_dwSourcePointIndex);

	pRow->GetItem (5)->SetValue (pVP->m_dwSpreadSFL);
	pRow->GetItem (6)->SetValue (pVP->m_dwSpreadSFN);
	pRow->GetItem (7)->SetValue (pVP->m_dwSpreadNb);

	pRow->GetItem (8)->SetValue (pVP->m_dwSuperSpread);
	pRow->GetItem (9)->SetValue (pVP->m_dwProcessNb);
	pRow->GetItem (10)->SetValue (LPCTSTR(pVP->m_szComments));
	pRow->GetItem (11)->SetValue (pVP->m_dwSwathNb);
	m_GridView.AddRow (pRow, FALSE );
	return;
}

void CVPAllGridWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码

	CBCGPGridRow* pRow= m_GridView.GetCurSel();
	if(NULL==pRow)
		return;
	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_VPSHOT));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	// 动态添加菜单
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	int i;
	int nCount = pFrm->m_AllShotSources.GetCount();
	CString  strTemp;
	CShotSource* pShotSource=NULL;
	
	for(i=0;i<nCount;i++)
	{	
		// 得到震源
		pShotSource = pFrm->m_AllShotSources.GetShotSource(i);
		strTemp = _T("Start Seismonitor with ")+pShotSource->m_strLabel;		
		// 动态添加菜单
		pPopup->AppendMenu(MF_STRING ,ID_VPSHOT_ONE+i, strTemp);
		pPopup->EnableMenuItem(ID_VPSHOT_ONE+i,MF_BYCOMMAND | MF_ENABLED);		 
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	// 显示菜单，由主框架响应菜单
	pPopupMenu->Create (pFrm, point.x, point.y,pPopup->Detach(),FALSE,TRUE); //pPopup->m_hMenu 
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 在界面上右击菜单，选择一个炮号准备放炮

    修改历史：

******************************************************************************
void CVPAllGridWnd::OnVpshotOne()
{	
	CBCGPGridRow* pRow= m_GridView.GetCurSel();
	if(NULL==pRow)
		return;
	CBCGPGridItem* pItem=pRow->GetItem(1);
	_variant_t vt = pItem->GetValue();

	vt.ChangeType(VT_UI4);
	DWORD  dwShotNb = vt.ulVal;
	
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->SendMessage(WM_MSG_SHOT,dwShotNb,1);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////   CVPToDoGridWnd   //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// CVPToDoGridWnd

IMPLEMENT_DYNCREATE(CVPToDoGridWnd, CGridWorkspace)

CVPToDoGridWnd::CVPToDoGridWnd()
{

}

CVPToDoGridWnd::~CVPToDoGridWnd()
{
}

BEGIN_MESSAGE_MAP(CVPToDoGridWnd, CGridWorkspace)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// CVPToDoGridWnd 消息处理程序
bool CVPToDoGridWnd::SetGridHead(void)
{	
	if(!m_GridView.GetSafeHwnd())		
		return false;

	m_GridView.InsertColumn (0, _T(" 口 "), 50);
	m_GridView.InsertColumn (1, _T("Shot Nb"), 80);
	m_GridView.InsertColumn (2, _T("Break Point"), 150);
	m_GridView.InsertColumn (3, _T("SourcePointLine"), 150);
	m_GridView.InsertColumn (4, _T("SourcePointNumber"), 150);
	m_GridView.InsertColumn (5, _T("SourcePointIndex"), 150);
	m_GridView.InsertColumn (6, _T("Sfl"), 100);
	m_GridView.InsertColumn (7, _T("Sfn"), 100);
	m_GridView.InsertColumn (8, _T("Spread#"), 100);
	m_GridView.InsertColumn (9, _T("SuperSpread"), 100);
	m_GridView.InsertColumn (10, _T("ProcessType"), 100);
	m_GridView.InsertColumn (11, _T("Comment"), 100);
	m_GridView.InsertColumn (12, _T("Swath"), 100);
	// pGridCtrl->AddRow();
	
	return true;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 装载所有状态的放炮表，VP ALL

    修改历史：

*******************************************************************************/
void CVPToDoGridWnd::LoadShotPoints(CShotPoints* pAllVP)
{
	if(NULL==pAllVP)
		return;

	int nCount = pAllVP->GetCount();
	CShotPoint* pVP = NULL;
	m_GridView.RemoveAll();
	for (int nRow = 0; nRow < nCount; nRow++)
	{
		pVP=pAllVP->GetShotPoint(nRow);
		if(!pVP || pVP->m_VPState!=VP_TODO)
			continue;
		LoadShotPoint(pVP);
	}
	m_GridView.AdjustLayout ();
	return;
}
////////////////////////////////////////////////////////////////////////////
// 增加一条放炮记录
void CVPToDoGridWnd::LoadShotPoint(CShotPoint* pVP )
{
	if(NULL==pVP)
		return ;
	
	CBCGPGridRow* pRow = m_GridView.CreateRow (13);
	//pRow->GetItem (0)->SetValue (pVP->m_VPState);
	pRow->GetItem (0)->SetImage(pVP->m_VPState);
	pRow->GetItem (1)->SetValue (pVP->m_dwShotNb);
	if(pVP->m_byBreakPoint)
		pRow->GetItem (2)->SetValue (_T("Y"));
	else
		pRow->GetItem (2)->SetValue (_T("N"));
	pRow->GetItem (3)->SetValue (pVP->m_fSourceLine);
	pRow->GetItem (4)->SetValue (pVP->m_fSourceNb);
	pRow->GetItem (5)->SetValue (pVP->m_dwSourcePointIndex);

	pRow->GetItem (6)->SetValue (pVP->m_dwSpreadSFL);
	pRow->GetItem (7)->SetValue (pVP->m_dwSpreadSFN);
	pRow->GetItem (8)->SetValue (pVP->m_dwSpreadNb);

	pRow->GetItem (9)->SetValue (pVP->m_dwSuperSpread);
	pRow->GetItem (10)->SetValue (pVP->m_dwProcessNb);
	pRow->GetItem (11)->SetValue (LPCTSTR(pVP->m_szComments));
	pRow->GetItem (12)->SetValue (pVP->m_dwSwathNb);
	m_GridView.AddRow (pRow, FALSE );
	return;
}
//////////////////////////////////////////////////////////////////////////
// 处理右击菜单
void CVPToDoGridWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////CVPDoneGridWnd///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// CVPDoneGridWnd

IMPLEMENT_DYNAMIC(CVPDoneGridWnd, CGridWorkspace)

CVPDoneGridWnd::CVPDoneGridWnd()
{
	
}

CVPDoneGridWnd::~CVPDoneGridWnd()
{
}


BEGIN_MESSAGE_MAP(CVPDoneGridWnd, CGridWorkspace)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CVPDoneGridWnd 消息处理程序


// CVPToDoGridWnd 消息处理程序
bool CVPDoneGridWnd::SetGridHead(void)
{	
	if(!m_GridView.GetSafeHwnd())		
		return false;		
	m_GridView.InsertColumn (0, _T(" 口 "), 50);
	m_GridView.InsertColumn (1, _T("Shot Nb"), 80);
	m_GridView.InsertColumn (2, _T("Break Point"), 150);
	m_GridView.InsertColumn (3, _T("SourcePointLine"), 150);
	m_GridView.InsertColumn (4, _T("SourcePointNumber"), 150);
	m_GridView.InsertColumn (5, _T("SourcePointIndex"), 150);
	m_GridView.InsertColumn (6, _T("Sfl"), 100);
	m_GridView.InsertColumn (7, _T("Sfn"), 100);
	m_GridView.InsertColumn (8, _T("Spread#"), 100);
	m_GridView.InsertColumn (9, _T("SuperSpread"), 100);
	m_GridView.InsertColumn (10, _T("ProcessType"), 100);
	m_GridView.InsertColumn (11, _T("Comment"), 100);
	m_GridView.InsertColumn (12, _T("Swath"), 100);
	// pGridCtrl->AddRow();
	
	return true;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 装载所有状态的放炮表，VP ALL

    修改历史：

*******************************************************************************/
void CVPDoneGridWnd::LoadShotPoints(CShotPoints* pAllVP)
{
	if(NULL==pAllVP)
		return;

	int nCount = pAllVP->GetCount();
	CShotPoint* pVP = NULL;
	m_GridView.RemoveAll();
	for (int nRow = 0; nRow < nCount; nRow++)
	{
		pVP=pAllVP->GetShotPoint(nRow);
		if(!pVP || pVP->m_VPState!=VP_DONE)
			continue;
		LoadShotPoint(pVP);
	}
	m_GridView.AdjustLayout ();
	return;
}
////////////////////////////////////////////////////////////////////////////
// 增加一条放炮记录
void CVPDoneGridWnd::LoadShotPoint(CShotPoint* pVP )
{
	if(NULL==pVP)
		return ;
	
	CBCGPGridRow* pRow = m_GridView.CreateRow (13);
	//pRow->GetItem (0)->SetValue (pVP->m_VPState);
	pRow->GetItem (0)->SetImage(pVP->m_VPState);
	pRow->GetItem (1)->SetValue (pVP->m_dwShotNb);
	if(pVP->m_byBreakPoint)
		pRow->GetItem (2)->SetValue (_T("Y"));
	else
		pRow->GetItem (2)->SetValue (_T("N"));
	pRow->GetItem (3)->SetValue (pVP->m_fSourceLine);
	pRow->GetItem (4)->SetValue (pVP->m_fSourceNb);
	pRow->GetItem (5)->SetValue (pVP->m_dwSourcePointIndex);

	pRow->GetItem (6)->SetValue (pVP->m_dwSpreadSFL);
	pRow->GetItem (7)->SetValue (pVP->m_dwSpreadSFN);
	pRow->GetItem (8)->SetValue (pVP->m_dwSpreadNb);

	pRow->GetItem (9)->SetValue (pVP->m_dwSuperSpread);
	pRow->GetItem (10)->SetValue (pVP->m_dwProcessNb);
	pRow->GetItem (11)->SetValue (LPCTSTR(pVP->m_szComments));
	pRow->GetItem (12)->SetValue (pVP->m_dwSwathNb);
	m_GridView.AddRow (pRow, FALSE );
	return;
}
//////////////////////////////////////////////////////////////////////////
// 右击菜单
void CVPDoneGridWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}
//////////////////////////////////////////////////////////////////////////
// 重载父类函数，将已经放炮的炮点对象添加到一放炮对象中
int CVPDoneGridWnd::RefreshShotPointState(CShotPoint* pShotPoint)
{
	LoadShotPoint(pShotPoint);
	return -1;
}