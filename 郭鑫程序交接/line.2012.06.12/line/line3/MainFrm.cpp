// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "line.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT NEAR WM_NEWXMLFILE = RegisterWindowMessage(MSG_XMLFILE_NEW);

IMPLEMENT_DYNAMIC(CMyBCGPDockingControlBar, CBCGPDockingControlBar)

	CMyBCGPDockingControlBar::CMyBCGPDockingControlBar()
{

}

CMyBCGPDockingControlBar::~CMyBCGPDockingControlBar()
{
}

void CMyBCGPDockingControlBar::OnShowProperty(matrix_data::data_base_helper::device_info& di)
{

}

// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)
	ON_COMMAND(ID_MATRIX_FORMLINESETUP, &CMainFrame::OnMatrixFormLineSetup)
	ON_COMMAND(ID_MATRIX_INSTRUMENTGRAPH, &CMainFrame::OnMatrixInstrumentGraph)
	ON_COMMAND(ID_MATRIX_SENSORGRAPH, &CMainFrame::OnMatrixSensorGraph)
	ON_COMMAND(ID_MATRIX_SURVEYSETUP, &CMainFrame::OnMatrixSurveySetup)
	ON_COMMAND(ID_MATRIX_LAYOUTSETUP, &CMainFrame::OnMatrixLayoutSetup)
	ON_COMMAND(ID_MATRIX_SPREADSETUP, &CMainFrame::OnMatrixSpreadSetup)
	ON_COMMAND(ID_MATRIX_LOOKSETUP, &CMainFrame::OnMatrixLookSetup)
	ON_COMMAND(ID_MATRIX_TESTBASESETUP, &CMainFrame::OnMatrixTestBaseSetup)
	ON_COMMAND(ID_MATRIX_TESTSETUP, &CMainFrame::OnMatrixTestSetup)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND(ID_BUTTON_MATRIX_FIELD_ON, OnMatrixFieldOn)
	ON_COMMAND(ID_BUTTON_MATRIX_FIELD_OFF, OnMatrixFieldOff)
	ON_COMMAND(ID_MATRIX_SEISMONITORGRAPH, &CMainFrame::OnMatrixSeismonitorGraph)
	ON_COMMAND(ID_MATRIX_INSTRUMENTLIST, &CMainFrame::OnMatrixInstrumentlist)
	ON_COMMAND(ID_MATRIX_SENSORLIST, &CMainFrame::OnMatrixSensorlist)
	ON_MESSAGE(WM_LOGIN_SERVER, &CMainFrame::OnLoginServer)
	ON_MESSAGE(WM_QUERY_RECV_SECTION, &CMainFrame::OnQueryRecvSection)
	ON_MESSAGE(WM_QUERY_RECV_SECTION_RESULT, &CMainFrame::OnQueryRecvSectionResult)
	ON_MESSAGE(WM_FIELD_ON_COMPLETE, &CMainFrame::OnFieldOnCompletely)
	ON_MESSAGE(WM_FIELD_OFF_COMPLETE, &CMainFrame::OnFieldOffCompletely)
	ON_MESSAGE(WM_UPDATE_DEVICE_TABLE, &CMainFrame::OnUpdateDeviceTable)
	ON_MESSAGE(WM_QUERY_INSTRUMENT_TEST_BASE_LIMIT, &CMainFrame::OnQueryInstrumentTestBaseLimit)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction
//2012-02-28
CMainFrame::CMainFrame()
:m_pDlgFormLineSetup(NULL),
m_pDlgSurveySetup(NULL),
m_pDlgLayoutSetup(NULL),
m_pDlgSpreadSetup(NULL),
m_pDlgLookSetup(NULL),
m_pDlgTestBaseSetup(NULL),
m_pDlgTestSetup(NULL),// FormLine设置
m_byFieldOperation(0)	// Field ON
{
	// TODO: add member initialization code here
	ZeroMemory(&paper_size_, sizeof(paper_size_));
	ZeroMemory(&center_grid_, sizeof(center_grid_));
	grid_x_ = 8; //初始化时,图标为8*8 为5个设备
	grid_y_ = 32;//初始化时,,图标为8*8 为4个设备高度
	current_bitmap_array_index_ = 0x0;

	unit_width_min_ = 8;	//!< 图形单元宽度，最小尺寸模式
	unit_hight_min_ = 8;	//!< 图形单元高度，最小尺寸模式
	unit_width_max_ = 40;	//!< 图形单元宽度，最大尺寸模式	
	unit_hight_max_ = 40;	//!< 图形单元高度，最大尺寸模式

	grid_x_min_ = 8;	//!< 单元格X方向最小尺寸
	grid_y_min_ =   8;	//!< 单元格Y方向最小尺寸
	grid_x_max_ =  40;	//!< 单元格X方向最大尺寸
	grid_y_max_ =  40;//!< 单元格Y方向最大尺寸
	grid_y_prev_ = grid_y_;
	grid_x_prev_ = grid_x_;
	be_draw_select_area_ = true;
}

CMainFrame::~CMainFrame()
{
	if(NULL != m_pDlgFormLineSetup)	// FormLine设置
	{
		m_pDlgFormLineSetup->DestroyWindow();
		delete m_pDlgFormLineSetup;
		m_pDlgFormLineSetup = NULL;
	}
	//2012-02-28
	if(NULL != m_pDlgSurveySetup)	// 地形设置
	{
		m_pDlgSurveySetup->DestroyWindow();
		delete m_pDlgSurveySetup;
		m_pDlgSurveySetup = NULL;
	}
	if(NULL != m_pDlgLayoutSetup)	// 布局设置
	{
		m_pDlgLayoutSetup->DestroyWindow();
		delete m_pDlgLayoutSetup;
		m_pDlgLayoutSetup = NULL;
	}
	if(NULL != m_pDlgSpreadSetup)	// 排列设置
	{
		m_pDlgSpreadSetup->DestroyWindow();
		delete m_pDlgSpreadSetup;
		m_pDlgSpreadSetup = NULL;
	}
	if(NULL != m_pDlgLookSetup)	// Look设置
	{
		m_pDlgLookSetup->DestroyWindow();
		delete m_pDlgLookSetup;
		m_pDlgLookSetup = NULL;
	}
	if(NULL != m_pDlgTestBaseSetup)	// 测试基本设置
	{
		m_pDlgTestBaseSetup->DestroyWindow();
		delete m_pDlgTestBaseSetup;
		m_pDlgTestBaseSetup = NULL;
	}
	if(NULL != m_pDlgTestSetup)	// 测试设置
	{
		m_pDlgTestSetup->DestroyWindow();
		delete m_pDlgTestSetup;
		m_pDlgTestSetup = NULL;
	}

	// 现场数据对象销毁
	OnSiteDataClose();
}
// 现场数据对象初始化
void CMainFrame::OnSiteDataInit()
{
	m_oSiteData.OnInit();	// 现场数据对象
}

// 现场数据对象销毁
void CMainFrame::OnSiteDataClose()
{
	m_oSiteData.OnClose();	// 现场数据对象
}

// 加载现场数据
void CMainFrame::LoadSiteData()
{
	m_oSiteData.LoadSiteData();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_bTabIcons = TRUE;
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
	mdiTabParams.m_bDocumentMenu = TRUE;
	EnableMDITabbedGroups (TRUE, mdiTabParams);

	CBCGPToolBar::EnableQuickCustomization ();

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// Menu will not take the focus on activation:
	CBCGPPopupMenu::SetForceMenuFocus (FALSE);

	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;

	if (!m_wndToolBarMatrixEdit.Create(this, 
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, 
		ID_MATRIX_EDIT_TOOLBAR) ||
		!m_wndToolBarMatrixEdit.LoadToolBar(IDR_EDIT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBarMatrixEdit.SetWindowText ("Matrix Edit");

	//---------------------
	// Create main toolbar:
	//---------------------
	if (!m_wndToolBarMatrixFile.Create(this,
		//TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, 
		ID_MATRIX_FILE_TOOLBAR) ||
		!m_wndToolBarMatrixFile.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBarMatrixFile.SetWindowText("Matrix File");

	//---------------------
	// Create Matrix View toolbar:
	//---------------------
	if (!m_wndToolBarMatrixView.Create(this,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		ID_VIEW_VIEW_TOOLBAR) ||
		!m_wndToolBarMatrixView.LoadToolBar (IDR_TOOLBAR_MATRIX_VIEW))
	{
		TRACE0("Failed to create Matrix View toolbar\n");
		return FALSE;      // fail to create
	}
	m_wndToolBarMatrixView.SetWindowText ("Matrix View");

	//---------------------
	// Create Matrix Setup toolbar:
	//---------------------
	if (!m_wndToolBarMatrixSetup.Create(this,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		ID_VIEW_SETUP_TOOLBAR) ||
		!m_wndToolBarMatrixSetup.LoadToolBar (IDR_TOOLBAR_MATRIX_SETUP))
	{
		TRACE0("Failed to create Matrix FieldON/OFF toolbar\n");
		return FALSE;      // fail to create
	}
	m_wndToolBarMatrixSetup.SetWindowText ("Matrix Setup");

	//---------------------
	// Create Matrix FieldON/OFF toolbar:
	//---------------------
	if (!m_wndToolBarMatrixField.Create(this,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		ID_VIEW_FIELD_TOOLBAR) ||
		!m_wndToolBarMatrixField.LoadToolBar (IDR_TOOLBAR_MATRIX_FIELD))
	{
		TRACE0("Failed to create Matrix FieldON/OFF toolbar\n");
		return FALSE;      // fail to create
	}
	m_wndToolBarMatrixField.SetWindowText ("Matrix Field");

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}

	CBCGPToolBar::AddToolBarForImageCollection (IDR_MENU_IMAGES);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarMatrixEdit.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarMatrixFile.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarMatrixView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarMatrixSetup.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarMatrixField.EnableDocking(CBRS_ALIGN_ANY);	

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndOutput);

	DockControlBar(&m_wndToolBarMatrixField);
	DockControlBarLeftOf(&m_wndToolBarMatrixSetup, &m_wndToolBarMatrixField);
	DockControlBarLeftOf(&m_wndToolBarMatrixView, &m_wndToolBarMatrixSetup);
	DockControlBarLeftOf(&m_wndToolBarMatrixFile, &m_wndToolBarMatrixView);
	DockControlBarLeftOf(&m_wndToolBarMatrixEdit, &m_wndToolBarMatrixFile);

	// enable VS 2005 look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
	CBCGPVisualManager::GetInstance ();
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
	// VISUAL_MANAGER

	EnableControlBarMenu (	TRUE, ID_VIEW_CUSTOMIZE, 
		_T("Customize..."), ID_VIEW_TOOLBARS,
		FALSE, TRUE);

	//加入显示元素 
	load_bitmaps();

return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
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

	pDlgCust->Create ();
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM lp)
{
	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_TOOLBAR_MATRIX_VIEW:
		{
			CMenu menuViewGraph;
			menuViewGraph.LoadMenu (IDR_MENU_MATRIX_VIEW_GRAPH);
			CBCGPToolbarMenuButton bnViewGraph(ID_MATRIX_VIEWGRAPH, menuViewGraph.GetSubMenu (0)->GetSafeHmenu(), -1);
			m_wndToolBarMatrixView.ReplaceButton(ID_MATRIX_VIEWGRAPH, bnViewGraph);

			CMenu menuViewList;
			menuViewList.LoadMenu (IDR_MENU_MATRIX_VIEW_LIST);
			CBCGPToolbarMenuButton bnViewList(ID_MATRIX_VIEWLIST, menuViewList.GetSubMenu (0)->GetSafeHmenu(), -1);
			m_wndToolBarMatrixView.ReplaceButton(ID_MATRIX_VIEWLIST, bnViewList);
		}
		break;
	case IDR_TOOLBAR_MATRIX_FIELD:
		{		
			m_wndToolBarMatrixField.SetToolBarBtnText (0, NULL, TRUE, TRUE);
			m_wndToolBarMatrixField.SetToolBarBtnText (1, NULL, TRUE, TRUE);
		}
		break;
	}

	return 0;
}

BOOL CMainFrame::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu (bDrop ? IDR_POPUP_DROP_MDITABS : IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_VERT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->Create (this, point.x, point.y, pPopup->GetSafeHmenu ());

	return TRUE;
}

void CMainFrame::OnMdiMoveToNextGroup() 
{
	MDITabMoveToNextGroup ();
}

void CMainFrame::OnMdiMoveToPrevGroup() 
{
	MDITabMoveToNextGroup (FALSE);
}

void CMainFrame::OnMdiNewHorzTabGroup() 
{
	MDITabNewGroup (FALSE);
}

void CMainFrame::OnMdiNewVertGroup() 
{
	MDITabNewGroup ();
}

void CMainFrame::OnMdiCancel() 
{
	// TODO: Add your command handler code here
	
}

CBCGPMDIChildWnd* CMainFrame::CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/)
{
	if (lpcszDocName != NULL && lpcszDocName [0] != '\0'){
		CDocument* pDoc = AfxGetApp()->OpenDocumentFile (lpcszDocName);
		if (pDoc != NULL){
			POSITION pos = pDoc->GetFirstViewPosition();
			if (pos != NULL){
				CView* pView = pDoc->GetNextView (pos);
				if (pView == NULL)	return NULL;
				return DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pView->GetParent ());
			}   
		}
	}

	return NULL;
}

void CMainFrame::OnClose() 
{
	SaveMDIState (theApp.GetRegSectionPath ());
	CBCGPMDIFrameWnd::OnClose();
}

// 处理内部网络命令帧
void CMainFrame::OnProcInterFaceCommand(CFrameInterface* pFrameInterface)
{
}

void CMainFrame::OnMatrixFormLineSetup()
{
	if(NULL == m_pDlgFormLineSetup)	// FormLine设置
	{
		m_pDlgFormLineSetup = new CDlgFormLineSetup;
		m_pDlgFormLineSetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgFormLineSetup->Create(IDD_DIALOG_FORMLINESETUP, this);
	}
	m_pDlgFormLineSetup->ShowWindow(SW_SHOWNORMAL);
}
//2012-02-28
// 地形设置
void CMainFrame::OnMatrixSurveySetup()
{
	if(NULL == m_pDlgSurveySetup)	// 地形设置
	{
		m_pDlgSurveySetup = new CDlgSurveySetup;
		m_pDlgSurveySetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgSurveySetup->Create(IDD_DIALOG_SURVEYSETUP, this);
	}
	m_pDlgSurveySetup->ShowWindow(SW_SHOWNORMAL);
}

// 布局设置
void CMainFrame::OnMatrixLayoutSetup()
{
	if(NULL == m_pDlgLayoutSetup)	// 布局设置
	{
		m_pDlgLayoutSetup = new CDlgLayoutSetup;
		m_pDlgLayoutSetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgLayoutSetup->Create(IDD_DIALOG_LAYOUTSETUP, this);
	}
	m_pDlgLayoutSetup->ShowWindow(SW_SHOWNORMAL);
}

// 排列设置
void CMainFrame::OnMatrixSpreadSetup()
{
	if(NULL == m_pDlgSpreadSetup)	// 排列设置
	{
		m_pDlgSpreadSetup = new CDlgSpreadSetup;
		m_pDlgSpreadSetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgSpreadSetup->Create(IDD_DIALOG_SPREADSETUP, this);
	}
	m_pDlgSpreadSetup->ShowWindow(SW_SHOWNORMAL);
}
// Look设置
void CMainFrame::OnMatrixLookSetup()
{
	if(NULL == m_pDlgLookSetup)	// Look设置
	{
		m_pDlgLookSetup = new CDlgLookSetup;
		m_pDlgLookSetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgLookSetup->Create(IDD_DIALOG_LOOKSETUP, this);
	}
	m_pDlgLookSetup->ShowWindow(SW_SHOWNORMAL);
}

// 测试基本设置
void CMainFrame::OnMatrixTestBaseSetup()
{
	if(NULL == m_pDlgTestBaseSetup)	// 测试基本设置
	{
		m_pDlgTestBaseSetup = new CDlgTestBaseSetup;
		m_pDlgTestBaseSetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgTestBaseSetup->Create(IDD_DIALOG_TESTBASESETUP, this);
	}
	m_pDlgTestBaseSetup->ShowWindow(SW_SHOWNORMAL);
}

// 测试设置
void CMainFrame::OnMatrixTestSetup()
{
	if(NULL == m_pDlgTestSetup)	// 测试设置
	{
		m_pDlgTestSetup = new CDlgTestSetup;
		m_pDlgTestSetup->m_pSiteData = &m_oSiteData;	// 现场数据
		m_pDlgTestSetup->Create(IDD_DIALOG_TESTSETUP, this);
	}
	m_pDlgTestSetup->ShowWindow(SW_SHOWNORMAL);
}

void CMainFrame::show_matrix_instrument_graph_view()
{
	CBCGPToolbarMenuButton* pButton =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton,
		m_wndToolBarMatrixView.GetButton (0));
	if (pButton != NULL)
	{
		pButton->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_INSTRUMENTGRAPH));
		m_wndToolBarMatrixView.Invalidate ();
		m_wndToolBarMatrixView.UpdateWindow ();
	}

	CLineApp* pApp = (CLineApp*)AfxGetApp();
	POSITION posDoc = NULL;
	// 打开仪器图形视图
	posDoc = pApp->m_pDocTemplateInstrumentGraph->GetFirstDocPosition();
	if(NULL == posDoc)
	{
		((CLineApp*)AfxGetApp())->m_pDocTemplateInstrumentGraph->OpenDocumentFile(NULL);
	}
}

void CMainFrame::OnMatrixInstrumentGraph()
{
	((CLineApp*)AfxGetApp())->query_instrument_test_base_limit();
}

void CMainFrame::ProcInterFaceCommandForSiteDataOutput()
{

}

// 响应仪器图形视图弹出菜单设置标记点
void CMainFrame::OnGraphViewInstrumentSetMarker(unsigned int uiInstrumentType, unsigned int uiSN)
{

}

// 响应仪器图形视图弹出菜单设置辅助道
void CMainFrame::OnGraphViewInstrumentSetAuxiliary(unsigned int uiInstrumentType, unsigned int uiSN)
{

}

// 响应仪器图形视图弹出菜单设置迂回道
void CMainFrame::OnGraphViewInstrumentSetDetour(unsigned int uiInstrumentTypeLow, unsigned int uiSNLow, unsigned int uiInstrumentTypeHigh, unsigned int uiSNHight)
{

}

// 响应仪器图形视图弹出菜单FormLine设置
void CMainFrame::OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN)
{

}

void CMainFrame::OnGraphViewSensorSetMute(unsigned int uiNbLine, unsigned int uiNbPoint)
{

}
void CMainFrame::OnMatrixSensorGraph()
{
	CBCGPToolbarMenuButton* pButton =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton,
		m_wndToolBarMatrixView.GetButton (0));
	if (pButton != NULL)
	{
		pButton->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_SENSORGRAPH));
		m_wndToolBarMatrixView.Invalidate ();
		m_wndToolBarMatrixView.UpdateWindow ();
	}

	CLineApp* pApp = (CLineApp*)AfxGetApp();
	POSITION posDoc = NULL;
	// 打开检波器图形视图
	//测试使用的数据,并没有直接从服务器返回
	matrix_data::data_base_helper::sensor_limit sl[7];
	ZeroMemory(&sl, sizeof(sl));

	sl[0].m_uiNb = 1500;
	strcpy(sl[0].m_pcDescr, "ResistanceMin");
	strcpy(sl[0].m_pcUnit, "ohms");
	sl[0].m_uiTestAim =2 ;
	sl[0].m_uiTestType = 15;
	sl[0].m_fLimit = 90;

	sl[1].m_uiNb = 1501;
	strcpy(sl[1].m_pcDescr, "ResistanceMax");
	strcpy(sl[1].m_pcUnit, "ohms");
	sl[1].m_uiTestAim =2 ;
	sl[1].m_uiTestType = 15;
	sl[1].m_fLimit = 110;


	sl[2].m_uiNb =1400 ;
	strcpy(sl[2].m_pcDescr, "Leakage");
	strcpy(sl[2].m_pcUnit, "Mohms");
	sl[2].m_uiTestAim = 2;
	sl[2].m_uiTestType = 14;
	sl[2].m_fLimit =30 ;

	sl[3].m_uiNb = 100;
	strcpy(sl[3].m_pcDescr, "Noise");
	strcpy(sl[3].m_pcUnit, "uv");
	sl[3].m_uiTestAim =2 ;
	sl[3].m_uiTestType =1 ;
	sl[3].m_fLimit = 50;

	sl[4].m_uiNb = 2000;
	strcpy(sl[4].m_pcDescr, "Tilt");
	strcpy(sl[4].m_pcUnit, "%");
	sl[4].m_uiTestAim = 2;
	sl[4].m_uiTestType = 20;
	sl[4].m_fLimit = 14;

	sl[5].m_uiNb =2100 ;
	strcpy(sl[5].m_pcDescr, "Tilt Model");
	strcpy(sl[5].m_pcUnit, "%");
	sl[5].m_uiTestAim =2 ;
	sl[5].m_uiTestType =21 ;
	sl[5].m_fLimit =15 ;

	sl[6].m_uiNb =2200 ;
	strcpy(sl[6].m_pcDescr, "Seismonitor");
	strcpy(sl[6].m_pcUnit,"dB" );
	sl[6].m_uiTestAim = 2;
	sl[6].m_uiTestType =22 ;
	sl[6].m_fLimit =-66 ;

	int i = 0;
	matrix_data::data_base_helper* helper = ((CLineApp*)AfxGetApp())->get_data_base_helper();
	for(int i = 0; i < 6; ++i)	
		helper->add_sensor_limit(sl[i]);

	matrix_data::data_base_helper::sensor_type st;
	for(i = 0; i < 10; ++i){
		st.nb_ = i + 1;
		st.segd_code_ = i + 1;
		sprintf(st.label_, "Sensor%d", i+1);
		st.continuity_max = 8;
		st.continuity_min_ = 2;
		st.leakage_ = 30;
		st.tilt_ = 6;
		st.noise_ = 3;
		helper->add_sensor_type(st);
	}
	posDoc = pApp->m_pDocTemplateSensorGraph->GetFirstDocPosition();
	if(NULL == posDoc)
		((CLineApp*)AfxGetApp())->m_pDocTemplateSensorGraph->OpenDocumentFile(NULL);
}

/**
* 重置工具条
* @param void
* @return void
*/
void CMainFrame::OnResetToolBarMatrix()
{
	CBCGPToolbarMenuButton* pButtonGraph =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, m_wndToolBarMatrixView.GetButton (0));
	if (pButtonGraph != NULL)
	{
		pButtonGraph->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_VIEWGRAPH));
	}

	CBCGPToolbarMenuButton* pButtonList =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, m_wndToolBarMatrixView.GetButton (1));
	if (pButtonList != NULL)
	{
		pButtonList->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_VIEWLIST));
	}
	m_wndToolBarMatrixView.Invalidate ();
	m_wndToolBarMatrixView.UpdateWindow ();

	CBCGPToolbarButton* pBN = NULL;
	pBN = m_wndToolBarMatrixField.GetButton(0);	
	pBN->SetImage(CImageHash::GetImageOfCommand (ID_BUTTON_MATRIX_FIELD_ON, FALSE));

	pBN = m_wndToolBarMatrixField.GetButton(1);	
	pBN->SetImage(CImageHash::GetImageOfCommand (ID_BUTTON_MATRIX_FIELD_OFF, FALSE));

	m_wndToolBarMatrixField.Invalidate ();
	m_wndToolBarMatrixField.RedrawWindow();
}

void CMainFrame::OnViewOutput() 
{
	m_wndOutput.ShowControlBar (!m_wndOutput.IsVisible (), FALSE, TRUE);
}

/**
* 响应仪器图形视图弹出菜单FieldOn/Off操作
* @param void
* @return void
*/
// 
void CMainFrame::OnToolBarFieldOperation()
{
	m_oSiteData.m_oSocketInterfaceCmdSend.m_oFrameInterface.m_byFieldOperation = m_byFieldOperation;
	m_oSiteData.m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeFieldFrame();
	m_oSiteData.m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 响应仪器图形视图弹出菜单FieldOn/Off操作
* @param void
* @return void
*/
void CMainFrame::OnMatrixFieldOn()
{
	((CLineApp*)AfxGetApp())->field_on();

	//m_byFieldOperation = 1;	// Field ON
	//	OnToolBarFieldOperation();
}

// 
void CMainFrame::OnMatrixFieldOff()
{
	((CLineApp*)AfxGetApp())->field_off();
	//m_byFieldOperation = 0;	// Field ON
	//OnToolBarFieldOperation();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
} 

void CMainFrame::OnMatrixSeismonitorGraph()
{
	CBCGPToolbarMenuButton* pButton =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton,
		m_wndToolBarMatrixView.GetButton (0));
	if (pButton != NULL)
	{
		pButton->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_SEISMONITORGRAPH));
		m_wndToolBarMatrixView.Invalidate ();
		m_wndToolBarMatrixView.UpdateWindow ();
	}

	CLineApp* pApp = (CLineApp*)AfxGetApp();
	POSITION posDoc = NULL;
	// 打开噪声监测图形视图
	posDoc = pApp->m_pDocTemplateSeismonitorGraph->GetFirstDocPosition();
	if(NULL == posDoc)
	{
		((CLineApp*)AfxGetApp())->m_pDocTemplateSeismonitorGraph->OpenDocumentFile(NULL);
	}
}
// 仪器列表视图菜单命令
void CMainFrame::OnMatrixInstrumentlist()
{
	// TODO: 在此添加命令处理程序代码
	CBCGPToolbarMenuButton* pButton =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton,
		m_wndToolBarMatrixView.GetButton (1));
	if (pButton != NULL)
	{
		pButton->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_INSTRUMENTLIST));
		m_wndToolBarMatrixView.Invalidate ();
		m_wndToolBarMatrixView.UpdateWindow ();
	}

	CLineApp* pApp = (CLineApp*)AfxGetApp();
	POSITION posDoc = NULL;
	// 打开仪器列表视图
	posDoc = pApp->m_pDocTemplateInstrumentList->GetFirstDocPosition();
	if(NULL == posDoc)
	{
		((CLineApp*)AfxGetApp())->m_pDocTemplateInstrumentList->OpenDocumentFile(NULL);
	}	
}
// 检波器列表视图菜单命令
void CMainFrame::OnMatrixSensorlist()
{
	// TODO: 在此添加命令处理程序代码
	CBCGPToolbarMenuButton* pButton =  DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton,
		m_wndToolBarMatrixView.GetButton (1));
	if (pButton != NULL)
	{
		pButton->SetImage (CImageHash::GetImageOfCommand (ID_MATRIX_SENSORLIST));
		m_wndToolBarMatrixView.Invalidate ();
		m_wndToolBarMatrixView.UpdateWindow ();
	}

	CLineApp* pApp = (CLineApp*)AfxGetApp();
	POSITION posDoc = NULL;
	// 打开检波器列表视图
	posDoc = pApp->m_pDocTemplateSensorList->GetFirstDocPosition();
	if(NULL == posDoc)
	{
		((CLineApp*)AfxGetApp())->m_pDocTemplateSensorList->OpenDocumentFile(NULL);
	}
}

/**
@fn LRESULT OnLoginServer(WPARAM w_param, LPARAM l_param)
@detail 客户端和服务器连接成功后, 客户端登录服务器
@param[in]	w_param 消息WPARAM参数
@param[in]	l_param  消息LPARAM参数
@return void
@note	
*/
LRESULT CMainFrame::OnLoginServer(WPARAM w_param, LPARAM l_param)
{
	((CLineApp*)AfxGetApp())->login_server();
	return 0L;
}

/**
@fn LRESULT CMainFrame::OnQueryRecvSection(WPARAM,LPARAM); 
@detail 客户端和服务器登录成功后, 客户端从服务器查询接收区域
@param[in]	w_param 消息WPARAM参数
@param[in]	l_param  消息LPARAM参数
@return void
@note	
*/
LRESULT CMainFrame::OnQueryRecvSection(WPARAM w_param, LPARAM l_param) 
{
	((CLineApp*)AfxGetApp())->query_recv_section();
	return 0L;
}

/**
@fn LRESULT CMainFrame::OnQueryRecvSectionResult(WPARAM,LPARAM); 
@detail 服务器返回查询区域信息给客户端
@param[in]	w_param 行数
@param[in]	l_param  区域数
@return void
@note	
*/
LRESULT CMainFrame::OnQueryRecvSectionResult(WPARAM w_param, LPARAM l_param) 
{
	((CLineApp*)AfxGetApp())->set_line_number(w_param);
	((CLineApp*)AfxGetApp())->set_section_number(l_param);
	((CLineApp*)AfxGetApp())->get_data_base_helper()->set_point_nb_to_sn_map();
	set_paper_size( w_param,  l_param);
	return 0L;
}

/**
@fn LRESULT OnFieldOnCompletely(WPARAM,LPARAM)
@detail 上电过程完成通知消息
@param[in]	w_param 行数
@param[in]	l_param  区域数
@return void
@note	
*/
LRESULT CMainFrame::OnFieldOnCompletely(WPARAM w_param, LPARAM l_param)
{
	m_byFieldOperation = 1;
	CBCGPToolbarButton* pBN = NULL;
	pBN = m_wndToolBarMatrixField.GetButton(0);	
	pBN->SetImage(CImageHash::GetImageOfCommand (ID_BUTTON_MATRIX_FIELD_ON_GRAPH, FALSE));

	pBN = m_wndToolBarMatrixField.GetButton(1);	
	pBN->SetImage(CImageHash::GetImageOfCommand (ID_BUTTON_MATRIX_FIELD_OFF_GRAPH, FALSE));

	m_wndToolBarMatrixField.InvalidateButton(0);
	m_wndToolBarMatrixField.InvalidateButton(1);
	m_wndToolBarMatrixField.RedrawWindow();

	return 0L;
}

/**
@fn LRESULT OnFieldOffCompletely(WPARAM w_param, LPARAM l_param)
@detail 下电完成通知消息
@param[in]	w_param 行数
@param[in]	l_param  区域数
@return void
@note	
*/
LRESULT CMainFrame::OnFieldOffCompletely(WPARAM w_param, LPARAM l_param)
{
	m_byFieldOperation = 0;
	CBCGPToolbarButton* pBN = NULL;
	pBN = m_wndToolBarMatrixField.GetButton(1);	
	pBN->SetImage(CImageHash::GetImageOfCommand (ID_BUTTON_MATRIX_FIELD_ON_GRAPH, FALSE));

	pBN = m_wndToolBarMatrixField.GetButton(0);	
	pBN->SetImage(CImageHash::GetImageOfCommand (ID_BUTTON_MATRIX_FIELD_OFF_GRAPH, FALSE));

	m_wndToolBarMatrixField.InvalidateButton(0);
	m_wndToolBarMatrixField.InvalidateButton(1);
	m_wndToolBarMatrixField.RedrawWindow();
	return 0L;
}

/**
@fn LRESULT OnFieldOffCompletely(WPARAM w_param, LPARAM l_param)
@detail 更新设备列表
@param[in]	w_param 设备信息 
@param[in]	l_param  设备信息大小 
@return void
@note	
*/
LRESULT CMainFrame::OnUpdateDeviceTable(WPARAM w_param, LPARAM l_param)
{
	char* buff_ptr = reinterpret_cast<char*>(w_param);
	unsigned int size = static_cast<unsigned int>(l_param);
	theApp.update_device_table(buff_ptr,  size);
	CBCGPMDIChildWnd* pframe = (CBCGPMDIChildWnd*)GetActiveFrame();
	pframe->Invalidate(TRUE);
	theApp.show_message("updateDeviceTable");
	return 0L;
}

/**
@fn LRESULT OnQueryInstrumentTestBaseLimit(WPARAM w_param, LPARAM l_param)
@detail 返回查询 InstrumentTestBase XML文件信息（帧内容为空）
@param[in]	w_param 信息 
@param[in]	l_param  信息大小 
@return void
@note	
*/
LRESULT CMainFrame::OnQueryInstrumentTestBaseLimit(WPARAM w_param, LPARAM l_param)
{
	char* buff_ptr = reinterpret_cast<char*>(w_param);
	unsigned int size = static_cast<unsigned int>(l_param);
	theApp.query_instrument_test_base_limit_result (buff_ptr,  size);

	//显示query instrument view
	show_matrix_instrument_graph_view();

	return 0L;
}

// 画测线标尺
void CMainFrame::draw_mark_line_and_point(CDC* pDC, int iGridX, int iGridY)
{
	// 画测线标尺
	draw_mark_line(pDC, iGridY);
	// 画测点标尺
	draw_mark_point(pDC, iGridX);
}

// 画测线标尺和测点标尺
void CMainFrame::draw_mark_line(CDC* pDC, int iGridY)
{
	CPen oPen;
	CPen* pOldPen;

	oPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&oPen);

	int iLineNb = 0;
	int loop_size = ((CLineApp*)AfxGetApp())->get_line_number() * 2 - 1;
	CString strLineNb;
	CRect oRect;

	for(unsigned int i =0; i < loop_size; i++)
	{
		iLineNb = i;
		oRect.left = -50;
		oRect.right = 0;
		oRect.top = i * iGridY- unit_hight_min_ / 2;
		oRect.bottom = oRect.top + unit_hight_max_;
		if(iLineNb == 0){
			strLineNb.Format("%d", iLineNb + 1);
			center_grid_.cy= oRect.top; //+ (oRect.bottom - oRect.top)/2 ;
		}
		else{	strLineNb.Format("%d", iLineNb + 1);}

		pDC->DrawText(strLineNb, &oRect, DT_LEFT | DT_VCENTER);
	}
	pDC->SelectObject(pOldPen);
}

// 画测点标尺
void CMainFrame::draw_mark_point(CDC* pDC, int iGridX)
{
	int iStep = 1;
	if(iGridX <= 2 * grid_x_min_)	iStep = 5;

	CPen oPen;
	CPen* pOldPen;
	oPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&oPen);

	int iPointNb;
	CString strPointNb;
	CRect oRect;
	int col = ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_section_number();
	int i = 0;

	for(i = 0, iPointNb = 1-col; iPointNb < 0; ++iPointNb, ++i){
		if(((iPointNb%iStep) == 0) || (iPointNb == (1 - col))){
			oRect.left = i * iGridX -  grid_x_min_/ 2;
			oRect.right = oRect.left + grid_x_max_ ;
			oRect.top = -50;
			oRect.bottom = 0;
			strPointNb.Format("%d", iPointNb);
			pDC->DrawText(strPointNb, &oRect, DT_LEFT);
		}
	}

	for(iPointNb = 0; iPointNb < col; ++i, ++iPointNb){
		if(((iPointNb % iStep) == 0) || (iPointNb == 0) || (iPointNb == col - 1)){
			oRect.left = i * iGridX -  grid_x_min_/ 2;
			oRect.right = oRect.left + grid_x_max_ ;
			oRect.top = -50;
			oRect.bottom = 0;
			strPointNb.Format("%d", iPointNb);
			pDC->DrawText(strPointNb, &oRect, DT_LEFT);
			if(iPointNb == 0){	center_grid_.cx = oRect.left; }
		}
	}

	pDC->SelectObject(pOldPen);
}
//设置当前滚动屏幕大小
void CMainFrame::set_scroll_size(CScrollView* view_ptr)
{
	view_ptr->SetScrollSizes(MM_TEXT, paper_size_);
}

HICON CMainFrame::get_icon(int grid_x, int type, int status)
{
	HICON handle = 0x0; 
	switch(type){
	case 1:
		handle = (HICON)LoadImage(NULL, __TEXT("E:\\works\\line\\line3\\res\\交叉站.ico"), IMAGE_ICON, 8,8, 
															LR_DEFAULTCOLOR|LR_LOADFROMFILE);
		break;
	case 2:
		handle = (HICON)LoadImage(NULL, __TEXT("E:\\works\\line\\line3\\res\\电源站.ico"), IMAGE_ICON, 8,8, 
			LR_DEFAULTCOLOR|LR_LOADFROMFILE);
		break;
	case 3:
		handle = (HICON)LoadImage(NULL, __TEXT("E:\\works\\line\\line3\\res\\采集站.ico"), IMAGE_ICON, 8,8, 
			LR_DEFAULTCOLOR|LR_LOADFROMFILE);
		break;
	}

	return handle;
}

CBitmap* CMainFrame::get_bitmap_ptr(int grid_x/*x方向大小*/, 
	int type/*图表类型 1.aux 2.aul 3.fdu*/, 
	int status/*当前设备状态 1: OK 2: error*/)
{
	current_bitmap_array_index_ = grid_x / 8;
	if(current_bitmap_array_index_ > bitmap_array_size_) current_bitmap_array_index_ = bitmap_array_size_;
	else if(current_bitmap_array_index_ < 0) current_bitmap_array_index_ = 0;

	CBitmap* handle = NULL;
	--current_bitmap_array_index_; 
	switch(type){
	case 1:
		handle = aux_bitmap_ + current_bitmap_array_index_;
		break;
	case 2:
		handle = aul_bitmap_ + current_bitmap_array_index_;
		break;
	case 3:
		handle = fdu_bitmap_ + current_bitmap_array_index_;
		break;
	}
	return handle;
}

//加入设备到视图中
void CMainFrame::draw_devices(CDC* pDC, int iGridX, int iGridY, int test_type, bool aux_be_shown, bool aul_be_shown, bool fdu_be_shown)
{
	CBitmap* cbitmap_ptr = NULL;
	BITMAP bitmap;
	matrix_data::data_base_helper::device_info* di = NULL;
	matrix_data::data_base_helper::device_info_map::iterator itr;
	matrix_data::data_base_helper::device_info_map& devices_map = ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_devices_info();
	int cx = 0x0,
		cy = 0x0;
	int line_pre= -1;
	
	for(itr = devices_map.begin(); itr != devices_map.end(); ++itr){
		di = &itr->second;
		//get the cx, cy
		if(line_pre != di->line_index_){
			cx = center_grid_.cx;
			line_pre = di->line_index_;
		}

		cy = center_grid_.cy + (di->line_index_ - 1) * (grid_y_max_ - grid_y_min_) ;
		switch(di->sn_ & 0x3){
		case 0x1:
			if(aux_be_shown){
				cbitmap_ptr = get_bitmap_ptr(iGridX, 1, 1);
				cbitmap_ptr->GetBitmap(&bitmap);
				cx = center_grid_.cx + di->m_uiPointNb * bitmap.bmWidth;
				draw_device_unit(pDC, cx, cy, test_type, cbitmap_ptr);
			}
			break;
		case 0x2:
			if(aul_be_shown){
				cbitmap_ptr = get_bitmap_ptr(iGridX, 2, 1);
				cbitmap_ptr->GetBitmap(&bitmap);
				cx = center_grid_.cx + di->m_uiPointNb * bitmap.bmWidth;
				draw_device_unit(pDC, cx, cy, test_type, cbitmap_ptr);
			}
			break;
		case 0x3:
			if(fdu_be_shown){
				cbitmap_ptr = get_bitmap_ptr(iGridX, 3, 1);
				cbitmap_ptr->GetBitmap(&bitmap);
				cx = center_grid_.cx + di->m_uiPointNb * bitmap.bmWidth;
				draw_device_unit(pDC, cx, cy, test_type, cbitmap_ptr);
			}
			break;
		default:
			break;
		}
	}
}

//画出每一个设备
void CMainFrame::draw_device_unit(CDC* pDC, int iGridX, int iGridY, int test_type, CBitmap* bitmap_ptr)
{
	CDC dc;
	BITMAP bm;

	dc.CreateCompatibleDC(pDC);
	bitmap_ptr->GetBitmap(&bm);
	CBitmap* pOldBitmap = dc.SelectObject(bitmap_ptr);
	int nX = iGridX - bm.bmWidth / 2;
	int nY = iGridY - bm.bmHeight / 2;
	pDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dc, 	0, 0, SRCCOPY);
	dc.SelectObject(pOldBitmap);
}

void CMainFrame::load_bitmaps()
{
	for(int i = 0; i < bitmap_array_size_; ++i){
		aux_bitmap_[i].LoadBitmap(IDB_AUX_BITMAP8 + i);
		aul_bitmap_[i].LoadBitmap(IDB_AUL_BITMAP8 + i);
		fdu_bitmap_[i].LoadBitmap(IDB_FDU_BITMAP_8 + i);
	}
}

void CMainFrame::draw_devices_link_line(CDC* pDC, int iGridX, int iGridY)
{
	matrix_data::data_base_helper::device_info* di = NULL;
	matrix_data::data_base_helper::device_info_map::iterator itr;
	matrix_data::data_base_helper::device_info_map& devices_map = ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_devices_info();

	for(itr = devices_map.begin(); itr != devices_map.end(); ++itr){
		di = &itr->second;
		draw_device_link_line(di, pDC, iGridX, iGridY);
	}
}

// 画图，单元连接线
void CMainFrame::draw_device_link_line(matrix_data::data_base_helper::device_info* di, CDC* pDC, int iGridX, int iGridY)
{
	CPen oPenLine;
	CPen* pOldPen;

	int iCenterX = center_grid_.cx + di->m_uiPointNb * iGridX;
	int iCenterY = center_grid_.cy + (di->line_index_ - 1) * iGridY;

	// 画连接线，向右画
	// 判断：迂回道低端
	if(di->m_bDetour)		oPenLine.CreatePen(PS_DOT, 5, RGB(0, 0, 255));	// 蓝色
	else		oPenLine.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));	// 绿色

	pOldPen = pDC->SelectObject(&oPenLine);
	if(di->m_uiSNInstrumentRight > 0)
	{
		pDC->MoveTo(CPoint(iCenterX, iCenterY));
		pDC->LineTo(CPoint(iCenterX + iGridX, iCenterY));
	}
	// 画连接线，向下画
	if(di->m_uiSNCrossDown > 0)
	{
		pDC->MoveTo(CPoint(iCenterX, iCenterY));
		pDC->LineTo(CPoint(iCenterX, iCenterY + iGridY));
	}
	
	pDC->SelectObject(pOldPen);
}

/**
* 放大1倍
* @param void
* @return void
*/
void CMainFrame::zoom_in(CScrollView* view_ptr, CPoint& point_scroll_pos) 
{
	float fGridXPre = (float)grid_x_prev_;
	float fGridYPre = (float)grid_y_prev_;

	zoom_in_rate_ = 2.0;	// 放大倍数
	// 缩放图形
	zoom_graph();
	// 得到滚动条位置
	point_scroll_pos= view_ptr->GetScrollPosition();
	float fZoomRateX = grid_x_ / fGridXPre;
	float fZoomRateY = grid_y_ / fGridYPre;
	point_scroll_pos.x = (int)((float)(point_scroll_pos.x - paper_head_left_) * fZoomRateX) + paper_head_left_;
	point_scroll_pos.y = (int)((float)(point_scroll_pos.y - paper_head_top_) * fZoomRateY) + paper_head_top_;
	// 绘图前设置绘图参数
	//SetupPageSize(view_ptr);
	set_scroll_size(view_ptr);
	// 设置滚动条新位置
	view_ptr->ScrollToPosition(point_scroll_pos);	
	be_draw_select_area_ = false;	// 是否画鼠标中间键选中区域位置
//	m_strAbsoluteSpread = "all";	// 选中区域对应的绝对排列
	Invalidate(TRUE);
}

void CMainFrame::zoom_graph()
{
	grid_y_max_ *= zoom_in_rate_;
	grid_x_max_ *= zoom_in_rate_;

	grid_x_ = (int)((float)grid_x_ * zoom_in_rate_);	// 单元格X方向尺寸
	grid_y_ = (int)((float)grid_y_ * zoom_in_rate_);	// 单元格Y方向尺寸
	if(grid_x_ > grid_x_max_)
	{
		grid_x_ = grid_x_max_;
	}
	if(grid_y_ > grid_y_max_)
	{
		grid_y_ = grid_y_max_;
	}
	if(grid_x_ < grid_x_min_)
	{
		grid_x_ = grid_x_min_;
	}
	if(grid_y_ < grid_y_min_)
	{
		grid_y_ = grid_y_min_;
	}
	set_paper_size(((CLineApp*)AfxGetApp())->get_data_base_helper()->get_line_number(), 
		((CLineApp*)AfxGetApp())->get_data_base_helper()->get_section_number());
	grid_y_prev_ = grid_y_;	// 单元格X方向尺寸，上一次
	grid_x_prev_ = grid_x_;	// 单元格Y方向尺寸，上一次
}

/**
* 缩小1倍
* @param void
* @return void
*/
void CMainFrame::zoom_out(CScrollView* view_ptr)
{
	zoom_in_rate_ = 0.5;	// 放大倍数
	// 缩放图形
	zoom_graph();
	// 绘图前设置绘图参数
	set_scroll_size(view_ptr);	
	be_draw_select_area_ = false;	// 是否画鼠标中间键选中区域位置
}

/**
* 判断点是否在绘图区域内
* @param CPoint& point 鼠标点
* @return void
*/
bool CMainFrame::judge_if_point_in_paper(CPoint& point)
{
	// 得到图形单元尺寸
	if(((point.x >= (-paper_head_left_ / 2)) && (point.x <= paper_size_.cx )) 	&& 
		((point.y >= (-paper_head_top_ / 2)) && (point.y <= paper_size_.cy)))
	{
		return true;
	}
	return false;
}

/**
根据当前逻辑坐标点得到行号和点号
*/
bool CMainFrame::get_line_and_point_nb_according_to_point(CPoint& point, int& line_idx, int& point_nb)
{
	bool is_ok = false;
	// 判断点是否在绘图区域内
	if(judge_if_point_in_paper(point)){	
		int i =0,
			tmp_min = 0,
			tmp_max = 0,
			loop_size = ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_line_number() * 2 - 1;
		BITMAP bm;
		line_idx = -1;
		point_nb = -1;

		fdu_bitmap_[current_bitmap_array_index_].GetBitmap(&bm);
		for(i =0; i < loop_size; ++i)	{
			tmp_min = center_grid_.cy + i * (grid_y_max_ - grid_y_min_) - bm.bmHeight / 2;
			tmp_max = tmp_min + bm.bmHeight;
			if(point.y >= tmp_min && point.y <= tmp_max){
				line_idx = i;
				break;
			}
		}
		if(line_idx == -1)  goto GOTO;

		i = 0;
		loop_size =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_section_number() - 1;
		for (i -= loop_size; i < loop_size; ++i){
			tmp_min = center_grid_.cx + i * bm.bmWidth - bm.bmWidth / 2;
			tmp_max = tmp_min + bm.bmWidth;
			if(point.x >= tmp_min && point.x <= tmp_max){
				point_nb = i;
				is_ok = true;
				break;
			}
		}
		if(!is_ok)	goto GOTO;
	}
GOTO:
	return is_ok;
}

/**
* 显示设备信息在属性对话框
*/
void CMainFrame::show_device_info(CMyBCGPDockingControlBar* control_bar, 
																CPoint& point, 
																enum __enuLINE_GRAPH_VIEW_TYPE type)
{
	matrix_data::data_base_helper::device_info di;
	int line_num = -1,
		point_nb = -1;

	// 判断点是否在绘图区域内
	if(get_line_and_point_nb_according_to_point(point, line_num, point_nb)){	
		di.sn_ =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->find_point_nb_and_return_sn(point_nb * 1000 + line_num+1);
		if(di.sn_ != 0)	
			if(((CLineApp*)AfxGetApp())->get_data_base_helper()->find_device(di))	show_property(control_bar, di, type);
	}
}

void CMainFrame::show_property(CMyBCGPDockingControlBar* control_bar, 
															matrix_data::data_base_helper::device_info& di, 
															enum __enuLINE_GRAPH_VIEW_TYPE type)
{
	switch(type){
	case INSTRUMENT_GRAPH_VIEW_TYPE:
		control_bar->OnShowProperty(di);
		break;
	case SENSOR_GRAPH_VIEW_TYPE:
		control_bar->OnShowProperty(di);
		break;
	case SEISMONITOR_GRAPH_VIEW_TYPE:
		control_bar->OnShowProperty(di);
		break;
	}
};

void CMainFrame::show_context_menu(CScrollView* view_ptr, CPoint& point, CPoint& client_point)
{
	int line_num = -1,
		point_nb = -1, 
		sn = 0x0;

	// 判断点是否在绘图区域内
	if(get_line_and_point_nb_according_to_point(client_point, line_num, point_nb)){	
		sn =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->find_point_nb_and_return_sn(point_nb * 1000 + line_num+1);
		if(sn != 0){
			device_info_.clear();
			device_info_.sn_ = sn;
			((CLineApp*)AfxGetApp())->get_data_base_helper()->find_device(device_info_);
			CMenu menu;
			menu.LoadMenu (IDR_POPUP_GRAPHVIEW_INSTRUMENT);
			CMenu* pSumMenu = menu.GetSubMenu(0);
			// 判断：不是交叉站
			if((device_info_.sn_ & 0x3) != 0x1){	// 仪器类型 1-交叉站；2-电源站；3-采集站
				pSumMenu->DeleteMenu(IDR_POPUP_GRAPHVIEW_INSTRUMENT_FORMLINE, MF_BYCOMMAND);
			}

			if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd))){
				CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
				if (!pPopupMenu->Create(view_ptr, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))	return;
				((CBCGPMDIFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu (pPopupMenu);
				view_ptr->UpdateDialogControls(this, FALSE);
			}
		}	
	}
}

bool CMainFrame::get_device_info_from_point(CPoint& point, matrix_data::data_base_helper::device_info& di)
{
	bool is_ok = false;
	int line_num = -1,
		point_nb = -1;

	if(get_line_and_point_nb_according_to_point(point, line_num, point_nb)){
		di.sn_ =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->find_point_nb_and_return_sn(point_nb * 1000 + line_num+1);
		if(di.sn_ != 0){	
			((CLineApp*)AfxGetApp())->get_data_base_helper()->find_device(di);
			is_ok = true;
		}
	}
	return is_ok;
}

void CMainFrame::zoom_in_drag( const CRect& client_rect, const CRect& select_area, CPoint& point_scroll_pos)
{
	float fWidthDrag= (float)select_area.Width();

	float fGridXPre = (float)grid_x_prev_;
	float fGridYPre = (float)grid_y_prev_;

	float fWidthClient = (float)client_rect.Width();
	zoom_in_rate_ = fWidthClient / fWidthDrag;
	// 缩放图形
	zoom_graph();

	// 得到滚动条位置
	point_scroll_pos.x = select_area.left;
	point_scroll_pos.y = select_area.top;

	float fZoomRateX = grid_x_ / fGridXPre;
	float fZoomRateY = grid_y_ / fGridYPre;
	point_scroll_pos.x = (int)((float)point_scroll_pos.x * fZoomRateX) + paper_head_left_;
	point_scroll_pos.y = (int)((float)point_scroll_pos.y * fZoomRateY) + paper_head_top_;
	if((paper_size_.cy + paper_head_top_ * 2) <= client_rect.Height())	{
		point_scroll_pos.y = 0;
	}
	else	{
		if(point_scroll_pos.y > ((paper_size_.cy + paper_head_top_ * 2) - client_rect.Height()))
			point_scroll_pos.y = (paper_size_.cy + paper_head_top_ * 2) - client_rect.Height();
	}
}

void CMainFrame::zoom_in_navigator(const CRect& client_rect, CPoint& point_scroll_pos, float zoom_rate, float x_rate, float y_rate)
{
	zoom_in_rate_ = zoom_rate;
	// 缩放图形
	zoom_graph();
	point_scroll_pos.x = (int)((float)paper_size_.cx * x_rate) + paper_head_left_;
	point_scroll_pos.y = (int)((float)paper_size_.cy * y_rate) + paper_head_top_;
	if((paper_size_.cy + paper_head_top_ * 2) <= client_rect.Height()){
		point_scroll_pos.y = 0;
	}
	else{
		if(point_scroll_pos.y > ((paper_size_.cy + paper_head_top_ * 2) - client_rect.Height()))
			point_scroll_pos.y = (paper_size_.cy + paper_head_top_ * 2) - client_rect.Height();
	}
}