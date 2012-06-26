// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Operation.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_COMMAND_RANGE(ID_VPSHOT_FROM, ID_VPSHOT_FROM + ActiveSourceNumLimit * 3 - 1, &CMainFrame::OnSelectActiveSource)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CONNECT, &CMainFrame::OnConnectServer)
	ON_UPDATE_COMMAND_UI(IDS_CONNECTSERVER_ICON, &CMainFrame::OnConnectServer)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, &CMainFrame::OnToolbarReset)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CONNECT,
	IDS_CONNECTSERVER_ICON,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{

	// TODO: add member initialization code here
	m_bServerConnected = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Menu will not take the focus on activation:
	CBCGPPopupMenu::SetForceMenuFocus (FALSE);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBarSetup.SetLockedSizes(CSize(16,16),CSize(16,16));
	uiToolbarHotID = bIsHighColor ? IDB_BITMAP_SETUPBAR256 : 0;
	if (!m_wndToolBarSetup.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		ID_TOOLBAR_SETUP) ||
		!m_wndToolBarSetup.LoadToolBar (IDR_SETUPBAR, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//	m_wndToolBarView.SetLockedSizes(CSize(25,16),CSize(25,16));
	uiToolbarHotID = bIsHighColor ? IDB_BITMAP_VIEWBAR256 : 0;
	if (!m_wndToolBarView.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		ID_TOOLBAR_VIEW) ||
		!m_wndToolBarView.LoadToolBar (IDR_VIEWBAR, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CBCGPToolBar::AddToolBarForImageCollection (IDR_VPVIEWBAR, 
		bIsHighColor ? IDB_BITMAP_VPVIEW256 : 0);


	if (!CreateStatusBar ())
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CRect rectClient;
	GetClientRect(rectClient);
	if (!m_wndActiveSource.Create (_T("Active Source"), this, CRect (0, 0, rectClient.right-1, rectClient.bottom/5),
		TRUE, ID_VIEW_ACTIVESOURCEBAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Active Source bar\n");
		return -1;      // fail to create
	}

	m_wndActiveSource.LoadActiveSources();
	if (!m_wndAllVP.Create (_T("All VP"), this, CRect (0, rectClient.bottom/6, rectClient.right-1, rectClient.bottom/3),
		TRUE, ID_VIEW_ALLVPBAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create All VP bar\n");
		return -1;      // fail to create
	}
	m_wndAllVP.LoadShotPoints();
	if (!m_wndVPDone.Create (_T("VP Done"), this, CRect (0, rectClient.bottom/3, rectClient.right-1, rectClient.bottom/2),
		TRUE, ID_VIEW_VPDONEBAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create VP Done bar\n");
		return -1;      // fail to create
	}
	m_wndVPDone.LoadShotPoints();
	if (!m_wndVPToDo.Create (_T("VP To Do"), this, CRect (0, rectClient.bottom/2, rectClient.right-1, rectClient.bottom*2/3),
		TRUE, ID_VIEW_VPTODOBAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create VP To Do bar\n");
		return -1;      // fail to create
	}
	m_wndVPToDo.LoadShotPoints();
	if (!m_wndOutput.Create (_T("Status Mail"), this, CSize (rectClient.right-1, rectClient.bottom/8),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create Status Mail bar\n");
		return -1;      // fail to create
	}

	CString strToolbarTitle;
	strToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strToolbarTitle);

	strToolbarTitle.LoadString (IDS_SETUP_TOOLBAR);
	m_wndToolBarSetup.SetWindowText (strToolbarTitle);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarSetup.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndActiveSource.EnableDocking(CBRS_ALIGN_ANY);
	m_wndAllVP.EnableDocking(CBRS_ALIGN_ANY);
	m_wndVPDone.EnableDocking(CBRS_ALIGN_ANY);
	m_wndVPToDo.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBarSetup);
	DockControlBarLeftOf(&m_wndToolBarView,&m_wndToolBarSetup);
	DockControlBarLeftOf(&m_wndToolBar,&m_wndToolBarView);
	DockControlBar(&m_wndActiveSource);
	DockControlBar(&m_wndAllVP);
	DockControlBar(&m_wndVPDone);
	DockControlBar(&m_wndVPToDo);
	DockControlBar(&m_wndOutput);
	// 添加自定义工具栏
	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
	m_wndToolBarSetup.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);

	// Enable control bar context menu (list of bars + customize command):
	EnableControlBarMenu (	
		TRUE,				// Enable
		ID_VIEW_CUSTOMIZE, 	// Customize command ID
		_T("Customize..."),	// Customize command text
		ID_VIEW_TOOLBARS);	// Menu items with this ID will be replaced by
							// toolbars menu

	// enable VS 2010 look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2010));
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
	// VISUAL_MANAGER
	RecalcLayout();
 

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */);
	
	// 在自定义对话框命令中添加User
// 	pDlgCust->AddButton (_T("User"), CBCGPToolbarButton (ID_VIEW_USER_TOOLBAR1, 1, _T("User Tool 1"), TRUE));
// 	pDlgCust->AddButton (_T("User"), CBCGPToolbarButton (ID_VIEW_USER_TOOLBAR2, 2, _T("User Tool 2"), TRUE));
// 	pDlgCust->AddButton (_T("User"), CBCGPToolbarButton (ID_VIEW_USER_TOOLBAR3, 3, _T("User Tool 3"), TRUE));
// 
// 	pDlgCust->SetUserCategory (_T("User"));

	pDlgCust->EnableUserDefinedToolbars ();
	pDlgCust->Create ();
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:

	UINT uiToolBarId = (UINT) wp;
	if (uiToolBarId != IDR_VIEWBAR)
	{
		return 0;
	}
	CMenu menuViews;
	menuViews.LoadMenu (IDR_POPUP_VPVIEW);
	m_wndToolBarView.ReplaceButton (ID_VIEW_VPSOURCEBAR,
		CBCGPToolbarMenuButton (ID_VIEW_VPSOURCEBAR, menuViews, 
		CImageHash::GetImageOfCommand (ID_VIEW_VPSOURCEBAR), NULL));
	return 0;
}
 // RIBBON_APP

BOOL CMainFrame::CreateStatusBar ()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}
	if (m_bmpIconConnected.GetSafeHandle () == NULL)
	{
		HBITMAP hbmp = (HBITMAP) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE(IDB_SERVER_CONNECTED),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
		m_bmpIconConnected.Attach (hbmp);
	}
	if (m_bmpIconDisConnected.GetSafeHandle () == NULL)
	{
		HBITMAP hbmp = (HBITMAP) ::LoadImage (
			AfxGetResourceHandle (),
			MAKEINTRESOURCE(IDB_SERVER_DISCONNECTED),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
		m_bmpIconDisConnected.Attach (hbmp);
	}
	return TRUE;
}

void CMainFrame::OnSelectActiveSource(UINT nMenuItemID)
{	
	int	nIndex;
	// 从VPAll的窗口右键菜单选择
	if(nMenuItemID >= ID_VPSHOT_FROM && nMenuItemID < (ID_VPSHOT_FROM + ActiveSourceNumLimit))
	{
		// 计算所选择的震源在网格中位置
		nIndex = nMenuItemID - ID_VPSHOT_FROM;
		return;
	}
	// 从VPToDo的窗口右键菜单选择
	if(nMenuItemID >= (ID_VPSHOT_FROM + ActiveSourceNumLimit) && nMenuItemID< (ID_VPSHOT_FROM + ActiveSourceNumLimit * 2))
	{
		// 计算所选择的震源在网格中位置
		nIndex = nMenuItemID - (ID_VPSHOT_FROM + ActiveSourceNumLimit);		
		return;
	}
	// 从VPDone的窗口右键菜单选择
	if(nMenuItemID >= (ID_VPSHOT_FROM + ActiveSourceNumLimit * 2) && nMenuItemID < (ID_VPSHOT_FROM + ActiveSourceNumLimit * 3))
	{
		// 计算所选择的震源在网格中位置
		nIndex = nMenuItemID - (ID_VPSHOT_FROM + ActiveSourceNumLimit * 2);		
		return;
	}
}
void CMainFrame::OnConnectServer(CCmdUI* pCmdUI)
{
	CClientDC dc (this);
	CString str = _T("");
	CSize sz;
	int index = m_wndStatusBar.CommandToIndex(pCmdUI->m_nID);
	pCmdUI->Enable(TRUE);
	switch (pCmdUI->m_nID)
	{
	case ID_INDICATOR_CONNECT:
		str = _T("Connected to server 192.168.100.22:8080");
		//设置一个CClientDC对象来获取str的长度
		sz = dc.GetTextExtent(str, 33);
		//设置状态栏宽度
		m_wndStatusBar.SetPaneInfo(index, ID_INDICATOR_CONNECT, SBPS_NORMAL, sz.cx);
		//设置ID_INDICATOR_CONNECT字符串
		m_wndStatusBar.SetPaneText(index, str);
		break;
	case IDS_CONNECTSERVER_ICON:
		m_wndStatusBar.SetPaneInfo(index, ID_INDICATOR_CONNECT, SBPS_POPOUT, 0);
		if (m_bServerConnected)
		{
			m_wndStatusBar.SetPaneIcon (index, m_bmpIconConnected);
		}
		else
		{
			m_wndStatusBar.SetPaneIcon (index, m_bmpIconDisConnected);
		}
		break;
	default:
		break;
	}
}
