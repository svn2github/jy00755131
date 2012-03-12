// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Operation.h"
#include "MainFrm.h"
#include "OperationParseXML.h"
#include "OperationDoc.h"
#include "OperationView.h"
#include "DlgResultsOfAckedVP.h"
#include "DlgAssociateVP.h"
#include "DlgAssociateSFLSFN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
//添加的标识只运行一次的属性名, 自己定义一个属性值
extern CString	 g_strProgName;
extern HANDLE    g_hProgValue;  

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	// cxm 2012.3.7
// 	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
// 	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_CARBON, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_CARBON, OnUpdateAppLook)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_ACTIVESOURCE, &CMainFrame::OnViewActivesource)
	ON_COMMAND(ID_VIEW_ALLVP, &CMainFrame::OnViewAllvp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALLVP, &CMainFrame::OnUpdateViewAllvp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTIVESOURCE, &CMainFrame::OnUpdateViewActivesource)
	ON_COMMAND(ID_VIEW_ALLDONE, &CMainFrame::OnViewAlldone)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALLDONE, &CMainFrame::OnUpdateViewAlldone)
	ON_COMMAND(ID_VIEW_ALLTODO, &CMainFrame::OnViewAlltodo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALLTODO, &CMainFrame::OnUpdateViewAlltodo)		
	ON_WM_TIMER()
	ON_COMMAND_RANGE(ID_VPSHOT_ONE, ID_VPDONESHOT_TEN, OnShotPoint)
	ON_COMMAND(ID_SETUP_SOURCE, &CMainFrame::OnSetupSource)
	ON_COMMAND(ID_SETUP_OPERATION, &CMainFrame::OnSetupOperation)
	ON_COMMAND(ID_SETUP_COMMENTS, &CMainFrame::OnSetupComments)
	ON_COMMAND(ID_SETUP_DELAYSTEUP, &CMainFrame::OnSetupDelaysteup)
	ON_COMMAND(ID_SETUP_PROCESSTYPE, &CMainFrame::OnSetupProcesstype)
	// cxm 2012.3.7
// 	ON_REGISTERED_MESSAGE(WM_NEWLOG, OnNewLog)
// 	ON_REGISTERED_MESSAGE(WM_NEWXMLFILE, OnNewXMLFile)
END_MESSAGE_MAP()

//ON_MESSAGE(WM_MSG_SHOT,OnStartShot)
static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构
CMainFrame::CMainFrame()
// cxm 2012.3.7
/*: m_pOperationSvrSock(NULL)*/
{
	// TODO: 在此添加成员初始化代码
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2007_1);
	
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	OnAppLook (m_nAppLook);
	
	CBCGPToolBar::EnableQuickCustomization ();
	CList<UINT, UINT>	lstBasicCommands;

	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
	lstBasicCommands.AddTail (ID_FILE_NEW);
	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
	lstBasicCommands.AddTail (ID_FILE_PRINT);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2000);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2003);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2005);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2008);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_WIN_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_1);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_2);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_3);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_4);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_CARBON);

	CBCGPToolBar::SetBasicCommands (lstBasicCommands);
	

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);
	m_wndMenuBar.SetShowAllCommands(TRUE);


	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	m_wndToolBarView.SetLockedSizes(CSize(25,16),CSize(25,16));
	UINT uiToolbarHotID = bIsHighColor ? IDB_VIEWBAR256 : 0;
	if (!m_wndToolBarView.CreateEx(this /*, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC*/) ||
		!m_wndToolBarView.LoadToolBar(IDR_VIEWWNDBAR, 0, 0, TRUE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//SIZE sizeButton;
	//SIZE sizeImage;

	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, 0))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}	
	m_wndToolBarSetup.SetLockedSizes(CSize(16,16),CSize(16,16));
	uiToolbarHotID = bIsHighColor ? IDB_SETUPBAR256 : 0;
	if (!m_wndToolBarSetup.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarSetup.LoadToolBar(IDR_SETUPBAR, 0, 0, TRUE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarSetup.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndActiveSource.EnableDocking(CBRS_ALIGN_ANY);//CBRS_ALIGN_BOTTOM	
	m_wndVPAll.EnableDocking(CBRS_ALIGN_ANY);
	m_wndVPToDo.EnableDocking(CBRS_ALIGN_ANY);
	m_wndVPDone.EnableDocking(CBRS_ALIGN_ANY);	// CBRS_ALIGN_BOTTOM
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBarView);
	DockControlBarLeftOf(&m_wndToolBarSetup,&m_wndToolBarView);
	DockControlBarLeftOf(&m_wndToolBar,&m_wndToolBarSetup);
	
	DockControlBar(&m_wndActiveSource);
	DockControlBar(&m_wndVPAll);
	m_wndVPToDo.DockToWindow (&m_wndVPAll, CBRS_ALIGN_BOTTOM);
	m_wndVPDone.DockToWindow (&m_wndVPToDo, CBRS_ALIGN_TOP);
	RecalcLayout();

	// cxm 2012.3.7
// 	theApp.WriteLog(_T("CMainFrame::OnCreate"),_T("Start initialization configuration parameters!"),LOGTYPE_NORMAL,TRUE);
// 	ParseXMLFile();
// 	// 创建客户端socket
// 	CreateShotSvrSocket();
	
	// 设置程序属性，确保程序仅允许一次	
	SetProp(m_hWnd,g_strProgName,g_hProgValue);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序
void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */
		);

	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT) wp;
	if (uiToolBarId == IDR_MAINFRAME)
	{
//		CBCGPToolbarButton buttonUpdate (ID_VIEW_UPDATEALLVIEWS, -1, _T("&Update All Views"));
//		m_wndToolBar.InsertButton (buttonUpdate, 3);
	}

	return 0;
}
// cxm 2012.3.7
// BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
// {
// 	//---------------------------------------------------------
// 	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
// 	//---------------------------------------------------------
// 
// 	CFrameWnd::OnShowPopupMenu (pMenuPopup);
// 
// 	if (pMenuPopup != NULL &&
// 		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
// 	{
// 		if (CBCGPToolBar::IsCustomizeMode ())
// 		{
// 			//----------------------------------------------------
// 			// Don't show toolbars list in the cuztomization mode!
// 			//----------------------------------------------------
// 			return FALSE;
// 		}
// 
// 		pMenuPopup->RemoveAllItems ();
// 
// 		CMenu menu;
// 		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));
// 
// 		CMenu* pPopup = menu.GetSubMenu(0);
// 		ASSERT(pPopup != NULL);
// 
// 		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
// 	}
// /**/
// 	return TRUE;
// }

// cxm 2012.3.7
// void CMainFrame::OnViewWorkspace() 
// {
// 	/*
// 	ShowControlBar (&m_wndWorkSpace,
// 		!(m_wndWorkSpace.IsVisible ()),
// 		FALSE, TRUE);
// 	ShowControlBar (&m_wndGridWorkSpace,
// 		!(m_wndGridWorkSpace.IsVisible ()),
// 		FALSE, TRUE);
// 	RecalcLayout ();
// 	*/
// }
// 
// void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
// {
// //	pCmdUI->SetCheck (m_wndWorkSpace.IsVisible ());
// }

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS 2005 Look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
		CBCGPVisualManager::GetInstance ();
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2008:
		// enable VS 2008 Look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2008));
		CBCGPVisualManager::GetInstance ();
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_1:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_2:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_3:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007_4:
		// enable Office 2007 look:
		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_CARBON:
		// enable Carbon Look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerCarbon));
		CBCGPVisualManager::GetInstance ();
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	default:
		ASSERT (FALSE);
	}

	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CBCGPTabbedControlBar::ResetTabs ();

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!CFrameWnd::OnCreateClient(lpcs, pContext))
	{
		return FALSE;
	}
/*
	m_wndWorkSpace.SetContext (pContext);

	if (!m_wndWorkSpace.Create (_T("View  1"), this, CRect (0, 0, 300, 300),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return FALSE;      // fail to create
	}
*/
	m_wndActiveSource.SetContext (pContext);
	if (!m_wndActiveSource.Create (_T("Active Source"), this, CRect (0, 0, 1024, 200),
		TRUE, ID_VIEW_WORKSPACE+10,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create ActiveSource Workspace bar\n");
		return FALSE;      // fail to create
	}

	m_wndVPAll.SetContext (pContext);
	if (!m_wndVPAll.Create (_T("All VP"), this, CRect (0, 201, 1024, 500),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP  | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create VPAll Workspace bar\n");
		return FALSE;      // fail to create
	}

	m_wndVPToDo.SetContext (pContext);
	if (!m_wndVPToDo.Create (_T("VP To Do"), this, CRect (0, 501, 1024, 700),
		TRUE, ID_VIEW_WORKSPACE+2,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP  | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create VP To Do Workspace bar\n");
		return FALSE;      // fail to create
	}

	m_wndVPDone.SetContext (pContext);
	if (!m_wndVPDone.Create (_T("VP Done"), this, CRect (0, 701, 1024, 900),
		TRUE, ID_VIEW_WORKSPACE+3,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP| CBRS_FLOAT_MULTI  ))	// 
	{
		TRACE0("Failed to create VP Done Workspace bar\n");
		return FALSE;      // fail to create
	}
	return TRUE;
}

void CMainFrame::OnDestroy()
{
	CBCGPFrameWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	// cxm 2012.3.7
// 	if(m_pOperationSvrSock)
// 	{
// 		m_pOperationSvrSock->Close();
// 		delete m_pOperationSvrSock;
// 		m_pOperationSvrSock = NULL;
// 	}
	// 释放所有的采集道对象
	m_ChannelList.OnClose();

}

// cxm 2012.3.7
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：  解析XML配置文件，读入放炮表、震源、处理类型、采集排列
 
    修改历史：

*******************************************************************************/
// bool CMainFrame::ParseXMLFile(void)
// {
// 	//-----------------------------------------------------------------
// 	// 采集站配置信息
// 	CConfigParseXML		ConfigXML;
// 	CString    strPath = CLIENTDIR_XMLFILE;
// 	ConfigXML.ParseCrewConfig(strPath+CONFIG_XMLFILE,&m_FDUConfig);
// 	ConfigXML.ParseDiskRecord(strPath+CONFIG_XMLFILE,&m_DiskConfig);
// 
// 	//-----------------------------------------------------------------
// 	COperationParseXML		OperaXML;
// 	// 从测线定义文件中读取辅助道
// 	if(!OperaXML.ParseAuxChannel(strPath+MATRIX_XMLFILE,&m_AllAuxChannels))
// 	{
// 		// WriteLog(_T("Read Matrix Aux channel file error！"));
// 		return FALSE;
// 	}
// 	// 解析系统配置：IP地址、端口等
// 	// OperaXML.ParseClientSetup(OPERATION_XMLFILE);	
// 	// 解析配置信息
// 	OperaXML.ParseComments(theApp.m_strLocalXMLFile,&m_AllComments);
// 	// 解析放炮表
// 	OperaXML.ParseOperationTable(theApp.m_strLocalXMLFile,&m_AllShotPoints);	
// 	m_wndVPAll.LoadShotPoints(&m_AllShotPoints);
// 	m_wndVPToDo.LoadShotPoints(&m_AllShotPoints);
// 	m_wndVPDone.LoadShotPoints(&m_AllShotPoints);
// 	// 解析震源
// 	OperaXML.ParseSourceType(theApp.m_strLocalXMLFile,&m_AllShotSources);
// 	m_wndActiveSource.LoadShotSources(&m_AllShotSources);	
// 	// 解析处理类型
// 	OperaXML.ParseProcessType(theApp.m_strLocalXMLFile,&m_AllProcesses);
// 
// 	// 读取采集排列
// 	m_ChannelList.OnInit();
// 	m_ChannelList.LoadSiteData();
// 	strPath = CLIENTDIR_XMLFILE;
// 	OperaXML.ParseAllAbsoluteSpread(strPath+MATRIX_XMLFILE,&m_AllSpreads,&m_ChannelList);
// 	theApp.WriteLog(_T("CMainFrame::ParseXMLFile"),_T("Complete the initialization configuration parameters!"),LOGTYPE_NORMAL,TRUE);
// 	return false;
// }
// cxm 2012.3.7
/**
 * @brief 响应“WM_NEWXMLFILE”消息
 * @note  响应“WM_NEWXMLFILE”消息，判断哪一个XML配置文件被修改，并更新相应的参数。
 * @param  WPARAM wParam, 被修改的XML文件编号，在MatrixDef.h文件中定义各程序编号
 * @param  LPARAM lParam，定义配置文件中那一部分内容修改
 * @return LRESULT
 */
// LRESULT CMainFrame::OnNewXMLFile(WPARAM wParam, LPARAM lParam)
// {
// 	CString    strPath= CLIENTDIR_XMLFILE;
// 	// ----------------------------------------------------------
// 	//  Config程序配置文件修改
// 	if(wParam == MATRIX_CONFIG)
// 	{
// 		CConfigParseXML		ConfigXML;
// 		strPath = CLIENTDIR_XMLFILE;
// 		ConfigXML.ParseCrewConfig(strPath+CONFIG_XMLFILE,&m_FDUConfig);
// 		ConfigXML.ParseDiskRecord(strPath+CONFIG_XMLFILE,&m_DiskConfig);
// 		return 0;
// 	}
// 	// -------------------------------------------------------
// 	// 测线程序配置文件修改
// 	if(wParam == MATRIX_LINE)
// 	{
// 		// 读辅助道信息
// 		COperationParseXML		OperaXML;
// 		switch(lParam)
// 		{
// 		case MATRIX_LINE_AUXI:			// 更新辅助道设置
// 			strPath = CLIENTDIR_XMLFILE;
// 			m_AllAuxChannels.RemoveAll();
// 			if(OperaXML.ParseAuxChannel(strPath+MATRIX_XMLFILE,&m_AllAuxChannels))
// 			{			
// 				theApp.WriteLog(_T("CMainFrame::OnNewXMLFile"),_T("Matrix Auxiliary channels has been modified."),LOGTYPE_NORMAL,TRUE);			
// 			}
// 			else
// 			{
// 				theApp.WriteLog(_T("CMainFrame::OnNewXMLFile"),_T("Fail to read Matrix Aux channel file!"),LOGTYPE_ERROR,TRUE);
// 				return 0;
// 			}
// 			break;
// 		case MATRIX_LINE_SPREAD:			// 读取排列
// 			// 读取采集排列
// 			m_ChannelList.OnReset();
// 			m_ChannelList.LoadSiteData();
// 			m_AllSpreads.RemoveAll();
// 			strPath = CLIENTDIR_XMLFILE;
// 			if(OperaXML.ParseAllAbsoluteSpread(strPath+MATRIX_XMLFILE,&m_AllSpreads,&m_ChannelList)<0)
// 			{
// 				// 发生解析错误
// 				theApp.WriteLog(_T("CMainFrame::OnNewXMLFile"),_T("Fail to read Matrix spread type file!"),LOGTYPE_ERROR,TRUE);
// 				return 0;
// 			}
// 			break;
// 		}	
// 	}
// 	return 0;
// }
// cxm 2012.3.7
/******************************************************************************
    函    数：CreateShotSvrSocket
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 创建与施工服务器通信的socket

    修改历史：

*******************************************************************************/
// bool CMainFrame::CreateShotSvrSocket(void)
// {
// 	if(m_pOperationSvrSock)
// 	{
// 		m_pOperationSvrSock->Close();
// 		delete m_pOperationSvrSock;
// 		m_pOperationSvrSock = NULL;
// 	}
// 	m_pOperationSvrSock = new CSocketShotServer;
// 	// 创建与施工服务器通信的Socket
// 	if(m_pOperationSvrSock)
// 	{
// 		if(!m_pOperationSvrSock->CreateSocket(OPERATION_CLIENTPORT,theApp.m_strHostIP))
// 		{
// 			AfxMessageBox(_T("创建与施工服务器通信的Socket失败，检查IP地址设置！"));
// 			delete m_pOperationSvrSock;
// 			m_pOperationSvrSock = NULL;
// 			return false;
// 		}
// 	}
// 	theApp.WriteLog(_T("CMainFrame::CreateShotSvrSocket"),_T("Successfully created a client communication interface!"),LOGTYPE_NORMAL,TRUE);
// 	return true;
// }
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 增加一个

    修改历史：

*******************************************************************************/

void CMainFrame::OnViewActivesource()
{
	// TODO: 在此添加命令处理程序代码
	ShowControlBar (&m_wndActiveSource,	!(m_wndActiveSource.IsVisible ()),	FALSE, TRUE);
}
void CMainFrame::OnUpdateViewActivesource(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_wndActiveSource.IsVisible ());
}

void CMainFrame::OnViewAllvp()
{
	// TODO: 在此添加命令处理程序代码
	ShowControlBar (&m_wndVPAll, !(m_wndVPAll.IsVisible ()),	FALSE, TRUE);
	// cxm 2012.3.7
//	theApp.WriteLog(_T("CMainFrame::OnViewAllvp"),_T("Show All VP succeed!"),LOGTYPE_NORMAL,TRUE);
}

void CMainFrame::OnUpdateViewAllvp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_wndVPAll.IsVisible ());
}

void CMainFrame::OnViewAlldone()
{
	// TODO: 在此添加命令处理程序代码
	ShowControlBar (&m_wndVPDone, !(m_wndVPDone.IsVisible ()),	FALSE, TRUE);
}

void CMainFrame::OnUpdateViewAlldone(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_wndVPDone.IsVisible ());
}

void CMainFrame::OnViewAlltodo()
{
	// TODO: 在此添加命令处理程序代码
	ShowControlBar (&m_wndVPToDo, !(m_wndVPToDo.IsVisible ()),	FALSE, TRUE);	
}

void CMainFrame::OnUpdateViewAlltodo(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_wndVPToDo.IsVisible ());
}
// cxm 2012.3.7
// LRESULT CMainFrame::OnNewLog(WPARAM wParam, LPARAM lParam)
// {
// 	CLogBase   LogRecord;
// 	// 从内存中读取日志
// 	if(!theApp.m_LogMapping.ReadRecord((int)lParam,&LogRecord))
// 	{
// 		return -1;
// 	}
// 	CString  strLog;
// 	// 生成需要显示的日志文本字符串
// 	LogRecord.GetShowString(strLog);
// 	COperationView *pView = (COperationView*)GetActiveView();
// 	// 显示在试图中
// 	pView->AppendLog(strLog);
// 	return 0;
// }
/**
 * @brief 设置炮点的状态
 * @note  设置炮点的状态，需要更新的状态包括：
			1、m_AllShotPoints中保存的对象；
			2、m_wndVPAll中网格显示；
 * @param  CShotSource* pShotSource, 震源对象指针
 * @param  unsigned char byVPState，新状态
 * @return void
 */
void CMainFrame::SetShotPointState(CShotPoint* pShotPoint,unsigned char byVPState)
{	
	// int i,nCount;
	if(!pShotPoint)
	{
		return ;
	}
	// 更新状态
	pShotPoint->m_VPState = byVPState;
	// 更新All VP 窗口
	m_wndVPAll.RefreshShotPointState(pShotPoint);
	// 判断是否已经放炮完成
	if(byVPState == VP_DONE)
	{
		m_wndVPDone.RefreshShotPointState(pShotPoint);
	}
	return ;
}
/**
 * @brief 设置震源的状态
 * @note  设置某一震源的状态，并刷新显示
 * @param  CShotSource* pShotSource, 震源对象指针
 * @param  unsigned char byVPState，新状态
 * @return void
 */
void CMainFrame::SetShotSourceState(CShotSource* pShotSource,unsigned char byVPState)
{
	if(!pShotSource)
		return ;
	// 更新状态
	pShotSource->m_bySourceState = byVPState;
	// 刷新界面
	m_wndActiveSource.RefrestShotSourceState(pShotSource);
	return ;
}

/**
 * @brief 解析来自服务器的数据帧
 * @note  解析从服务器接受的数据,被CSocketShotServer::OnReceive函数调用

 数据帧格式：
			帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
			＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）
 * @param  int nActiveShotIndex, 激活放炮对象索引
 * @param  unsigned char *pBuf，应答结果数据帧指针
 * @return void
 */
// cxm 2012.3.7
// void CMainFrame::AnalysisProtocolOfServerSock(void)
// {
// 	unsigned char	pBuf[8196];
// 	WORD		wFrmLen;			// 帧长
// 	WORD		wFrmCmdCode;		// 命令码
// 	DWORD		dwFrmIndex;			// 帧计数
// 	DWORD		dwNb1;				// 炮号
// 	DWORD		dwNb2;				// 震源号
// 	int			nSize,nReadSize;
// 	CString		strTemp;
// 
// 	if(!m_pOperationSvrSock)
// 		return;
// 
// 	sockaddr_in sockShotServer;
// 	sockShotServer.sin_family = AF_INET;
// 	nSize = sizeof(sockShotServer);
// 	// 接收数据,nReadSize为接收到数据字节数
// 	nReadSize = m_pOperationSvrSock->ReceiveFrom( pBuf, 8196, (SOCKADDR*)&sockShotServer , &nSize );
// 	switch (nReadSize)
// 	{
// 	case 0:		// 没有收到数据 
// 		return;
// 		break;
// 	case SOCKET_ERROR:	// 发生错误，记录日志
// 		if (GetLastError() != WSAEWOULDBLOCK) 
// 		{  //发生socket错误 
// 			return;
// 		}
// 		break;
// 	default:
// 		break;
// 	}
// 	// -----------解析帧格式---------------------------------------------------------
// 	// 判断帧头是否0xEB, 0x90
// 	if (pBuf[0]!=0xEB || pBuf[1]!=0x90)
// 	{		
// 		theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("Error Frame Head!"),LOGTYPE_ERROR,FALSE);
// 		return;
// 	}
// 	// 计算帧长度
// 	wFrmLen = MAKEWORD(pBuf[2],pBuf[3]);
// // 	if(wFrmLen>8196)
// // 	{	// 帧长度超长，发生错误记录日志	
// // 		WriteLog(_T("Error Frame Length!"),3);
// // 		return;
// // 	}
// 	// 计算帧计数
// 	memcpy_s(&dwFrmIndex,sizeof(dwFrmIndex),pBuf+4 , 4);	
// 	// 帧尾
// 	if(pBuf[wFrmLen-2]!=0x14 || pBuf[wFrmLen-1]!=0x6F)
// 	{
// 		// 帧尾错误，需要重新定位帧头
// 		theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("Error Frame Tail!"),LOGTYPE_ERROR,FALSE);
// 		return;
// 	}
// 	// 命令码
// 	wFrmCmdCode=MAKEWORD(pBuf[9],pBuf[10]);
// 	// 可能是炮号和震源号，也可能是通信ID和线程ID
// 	memcpy_s(&dwNb1,4,pBuf+13,4);
// 	memcpy_s(&dwNb2,4,pBuf+17,4);
// 	// 当前激活的放炮对象	
// 	CActiveShot *pActiveShot=NULL;
// 	switch(wFrmCmdCode)
// 	{
// 	case OPERATION_CMD_REQUEST:	// 收到放炮请求的应答
// 		{	// 通过Nb查找激活的放炮管理对象			
// 			pActiveShot = m_AllActiveShots.FindActiveShotByNb(dwNb1,dwNb2);// ||pActiveShot->m_dwServerIP != inet_addr(("255.255.255.255"))
// 			if(!pActiveShot )
// 			{	// 已经接收到服务器的应答，属于其他施工客户端的应答，不需要处理。
// 				strTemp.Format(_T("Error Shot Nb# %d or Source Nb# %d ."),dwNb1,dwNb2);			
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_WARNING,TRUE);
// 				return;
// 			}
// 			if( pActiveShot->m_dwServerID)
// 			{	// 激活放炮管理对象的服务器ID不为0，表示已有服务器线程正在处理
// 				strTemp.Format(_T("Error Shot Nb# %d ,Source Nb# %d,Thread ID# %d already selected."),dwNb1,dwNb2,pActiveShot->m_dwServerID);
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_WARNING,TRUE);
// 				return;
// 			}
// 			// 保存服务器的IP地址和端口，此时可能有多个服务器应答，只能保存最后一个
// 			pActiveShot->m_dwServerIP = sockShotServer.sin_addr.s_addr;
// 			pActiveShot->m_wServerPort = ntohs(sockShotServer.sin_port);
// 			KillTimer(pActiveShot->m_dwTimerID);
// 			// 发送确认命令
// 			pBuf[4] = 0x01;								// 帧计数
// 			pBuf[8] = 0x01;								// 帧类型
// 			pBuf[9] = LOBYTE(OPERATION_CMD_CONFIRM);	// 命令码低位
// 			pBuf[10]= HIBYTE(OPERATION_CMD_CONFIRM);	// 命令码高位
// 			pActiveShot->SendToServer(pBuf,nReadSize);
// 			strTemp.Format(_T("Shot Nb# %d, Source Nb# %d, Received Server Ack! "),dwNb1,dwNb2);			
// 			theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_NORMAL,TRUE);
// 			return;			
// 		}
// 		break;
// 	case OPERATION_CMD_CONFIRM: // 找到施工服务及线程，下发数据
// 		{	// 根据炮点编号和震源编号查找当前激活的放炮对象
// 			pActiveShot = m_AllActiveShots.FindActiveShotByNb(dwNb1,dwNb2);
// 			if(!pActiveShot )
// 			{
// 				strTemp.Format(_T("Shot Nb# %d or Source Nb# %d is wrong."),dwNb1,dwNb2);			
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_ERROR,TRUE);
// 				return;
// 			}
// 			if( pActiveShot->m_dwServerID)
// 			{	// 激活放炮管理对象的服务器ID不为0，表示已有服务器线程正在处理
// 				strTemp.Format(_T("Shot Nb# %d ,Source Nb# %d,Server Thread ID# %d already selected."),dwNb1,dwNb2,pActiveShot->m_dwServerID);
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_ERROR,TRUE);
// 				return;
// 			}
// 			// 保存服务器提供的通信ID
// 			memcpy_s(&pActiveShot->m_dwServerID,4,pBuf+25,4);		
// 			// 线程ID
// 			memcpy_s(&pActiveShot->m_dwThreadID,4,pBuf+29,4);
// 			// 保存服务器的IP地址和端口，此时保存第一个确认应答的服务器地址
// 			pActiveShot->m_dwServerIP = sockShotServer.sin_addr.s_addr;
// 			pActiveShot->m_wServerPort = ntohs(sockShotServer.sin_port);
// 			// 发送炮号及 震源
// 			pActiveShot->SendShotPointAndSource();
// 			// 发送处理类型对象
// 			pActiveShot->SendProcessType();
// 			// 发送采集站配置信息
// 			pActiveShot->SendFDUConfig(&m_FDUConfig,&m_DiskConfig);
// 			// 发送采集排列
// 			pActiveShot->SendShotSpreadLabel();
// 			//pActiveShot->SendShotSpreadChannel();
// 			pActiveShot->SendShotSpreadChannel(&m_AllSpreads,pActiveShot->m_dwSpreadSFL);
// 			// 启动放炮
// 			pActiveShot->SendGeneralCMD(OPERATION_CMD_START);
// 			strTemp.Format(_T("Received Server confired ack frame: Shot Nb# %d ,Source Nb# %d,Thread ID# %d."),dwNb1,dwNb2,pActiveShot->m_dwThreadID);			
// 			theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_NORMAL,TRUE);
// 			return;			
// 		}
// 		break;
// 	case OPERATION_ACK_NAME:		// 收到采集站点名结果
// 		{
// 			// 根据通信ID（ServerID）和线程编号（ThreadID）查找当前激活的放炮对象
// 			nSize=m_AllActiveShots.FindActiveShotByID(dwNb1,dwNb2);
// 			if(nSize<0)
// 			{				
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("1 ServerID or Thread ID is wrong!"),LOGTYPE_ERROR,FALSE);
// 				return;
// 			}
// 			// 分析来自服务器的采集站点名结果
// 			AnalysisProtocolOfAckResultFrm(nSize,pBuf);
// 		}
// 		break;
// 	case OPERATION_CMD_SRCRDY:	// 爆炸机控制器准备好信号（Ready）
// 		{
// 			// 收到爆炸机控制器准备好信号: 应该先更新震源的图标，再判断该震源是否分配炮号，如果分配则更新炮号的图标。
// 			nSize=m_AllActiveShots.FindActiveShotByID(dwNb1,dwNb2);
// 			if(nSize<0)
// 			{
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("2 ServerID or Thread ID is wrong!"),LOGTYPE_ERROR,FALSE);
// 				return;
// 			}
// 			pActiveShot = m_AllActiveShots.GetActiveShot(nSize);
// 			// 改变震源的状态
// 			SetShotSourceState(pActiveShot->m_pShotSource,SHOTSOURCE_READY);
// 
// 		}
// 		break;
// 	case OPERATION_CMD_TBUPHOLE:	// 收到爆炸机状态参数: TB、井口时间
// 		{
// 			// 收到爆炸机控: 应该先更新震源的图标，再判断该震源是否分配炮号，如果分配则更新炮号的图标。
// 			nSize=m_AllActiveShots.FindActiveShotByID(dwNb1,dwNb2);
// 			if(nSize<0)
// 			{
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("2 ServerID or Thread ID is wrong!"),LOGTYPE_ERROR,FALSE);
// 				return;
// 			}
// 			pActiveShot = m_AllActiveShots.GetActiveShot(nSize);
// 			if(!pActiveShot)
// 			{
// 				return;
// 			}
// 			// 解析确认TB、井口时间、爆炸机状态等数据
// 			pActiveShot->m_byBlasterState = pBuf[29];
// 			memcpy_s(&pActiveShot->m_fConfirmedTB,4,pBuf+33,4);
// 			memcpy_s(&pActiveShot->m_fUpholeTime,4,pBuf+37,4);
// 			pActiveShot->m_bNewBlaster = TRUE;
// 			// 更新显示界面
// 			m_wndActiveSource.RefreshBlasterState();
// 		}
// 		break;
// 	case OPERATION_CMD_ITBERR:   // ITB错误
// 		{
// 			nSize=m_AllActiveShots.FindActiveShotByID(dwNb1,dwNb2);
// 			if(nSize<0)
// 			{
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("2 ServerID or Thread ID is wrong!"),LOGTYPE_ERROR,FALSE);
// 				return;
// 			}
// 			pActiveShot = m_AllActiveShots.GetActiveShot(nSize);
// 			if(!pActiveShot)
// 			{
// 				return;
// 			}
// 			pActiveShot->m_bITBErr = TRUE;
// 			pActiveShot->m_bNewBlaster = TRUE;
// 			// 更新显示界面
// 			m_wndActiveSource.RefreshBlasterState();
// 		}
// 		break;
// 	case OPERATION_CMD_RCVING:	// 开始数据采样
// 		{
// 			// 刷新震源和炮点的显示状态
// 			strTemp.Format(_T("Start Sampling: Server ID# %d, Thread ID# %d "),dwNb1,dwNb2);			
// 			theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_NORMAL,TRUE);
// 		}
// 		break;
// 	case OPERATION_CMD_RCVED:   // 数据采样结束
// 		{
// 			nSize=m_AllActiveShots.FindActiveShotByID(dwNb1,dwNb2);
// 			if(nSize<0)
// 			{
// 				theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),_T("2 ServerID or Thread ID is wrong!"),LOGTYPE_ERROR,FALSE);
// 				return;
// 			}
// 			pActiveShot = m_AllActiveShots.GetActiveShot(nSize);			
// 			// 依据ITB错误弹出对话框，由客户判断是否存盘
// /*			if(pActiveShot->m_bITBErr)
// 			{
// 				if (MessageBox(_T("Internal Time break! \n Do you want to record?"),_T("ACQUISITION"),MB_OKCANCEL)==IDCANCEL)
// 				{	// 取消数据记录，通知服务器终止此次数据采样
// 					pActiveShot->SendGeneralCMD(OPERATION_CMD_CANCEL);
// 					strTemp.Format(_T("Server ID# %d, Thread ID# %d: Internal Time break! Cancel to record."),dwNb1,dwNb2);
// 					theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_WARNING,TRUE);
// 					return;
// 				}
// 			}
// */			// 发送开始处理的命令，通知服务器将采样原始数据转化为SEGD文件
// 			pActiveShot->SendGeneralCMD(OPERATION_CMD_PROCGO);
// 			
// 		 }
// 		break;
// 	case OPERATION_CMD_PROCING:	// 数据处理开始
// 		{
// 			// 刷新震源和炮点的显示状态
// 			strTemp.Format(_T("Start Processing: Server ID# %d, Thread ID# %d "),dwNb1,dwNb2);			
// 			theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_NORMAL,TRUE);
// 		}
// 		break;
// 	case OPERATION_CMD_PROCED:	// 数据处理结束
// 		{
// 			// 刷新震源和炮点的显示状态
// 			strTemp.Format(_T("End of Process: Server ID# %d, Thread ID# %d "),dwNb1,dwNb2);			
// 			theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_NORMAL,TRUE);
// 		}
// 		break;
// 	case OPERATION_CMD_OUTING:	// 输出开始
// 		break;
// 	case OPERATION_CMD_OUTED:	// 输出结束
// 		break;
// 	case OPERATION_CMD_SHOTEND:	// 放炮结束，由服务器通知客户端
// 		break;
// 	default:	// 命令码错误
// 		{
// 			strTemp.Format(_T("Command Code 0x%X is wrong!"),wFrmCmdCode);
// 			theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfServerSock"),strTemp,LOGTYPE_ERROR,FALSE);
// 		}
// 		break;
// 	}
// 	
// }
/**
 * @brief 解析采集排列点名应答结果帧
 * @note  采集排列放炮前先点名，设置相应属性；施工服务器收到点名的结果后，将结果打包传送给客户端。
 该函数显示点名中无应答的采集站信息，并提示用户继续放炮还是停止放炮
 * @param  int nActiveShotIndex, 激活放炮对象索引
 * @param  unsigned char *pBuf，应答结果数据帧指针
 * @return void
 */
// cxm 2012.3.7
// void CMainFrame::AnalysisProtocolOfAckResultFrm(int nActiveShotIndex,unsigned char *pBuf)
// {
// 	CActiveShot *pActiveShot = m_AllActiveShots.GetActiveShot(nActiveShotIndex);
// 	if(!pActiveShot)
// 		return;
// 	
// 	int			i,nPos;
// 	WORD		wNoAckedFrmLen;
// 	DWORD		dwNoAcked,dwSpreadCount,dwIP;
// 	CString		strTemp;	
// 	CShotSpread*	pShotSpread =pActiveShot->m_pSpread;
// 	// 解析出总的无应答采集道个数
// 	memcpy_s(&dwNoAcked,4,pBuf+21,4);
// 	// 解析出数据帧中传送过来的无应答采集道所占字节数
// 	wNoAckedFrmLen = MAKEWORD(pBuf[11],pBuf[12]);
// 	// wNoAckedInFrm = (wNoAckedInFrm-12)/4;		// 依据帧格式计算出工时公式
// 	wNoAckedFrmLen -=12;
// 	// 采集排列总数
// 	int nIndex=0;
// 	int k= m_AllSpreads.GetCount();
// 	for(int j=0;j<k;j++)
// 	{
// 		CShotSpread *pSpread = m_AllSpreads.GetShotSpreadBySN(j+1);
// 		nIndex += pSpread->GetCount();
// 	}
// 	dwSpreadCount = (DWORD)nIndex;
// 	//dwSpreadCount = (DWORD)(pShotSpread->GetCount());			
// 
// 	strTemp.Format(_T("Response Results: Shot Nb# %d ,Source Nb# %d, Spread Count %d, No Acked Count %d"),
// 						pActiveShot->GetShotPointNb(),pActiveShot->GetShotSourceNb(), dwSpreadCount, dwNoAcked);
// 	theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfAckResultFrm"),strTemp,LOGTYPE_WARNING,TRUE);
// 	TRACE2("Response Results: Spread Count=%d,dwNoAcked=%d \r\n",dwSpreadCount,dwNoAcked);
// 	// WriteLog(strTemp);
// 	// 采集排列和应答的道数相等则直接刷新界面，准备放炮
// 	if(dwNoAcked == 0)
// 	{
// 		// 设置状态，刷新显示界面
// 		m_wndActiveSource.LoadShotPointBySource(pActiveShot->m_pShotSource, pActiveShot->m_pShotPoint);		
// 		return;
// 	}
// 	// 向用户显示无应答采集站信息对话框
// 	CDlgResultsOfAckedVP	DlgResult;
// 	DlgResult.m_strResult.Format(_T("%d No Response"),dwNoAcked);
// 	// 从数据帧中找出所有无应答的采集站IP地址
// 	wNoAckedFrmLen +=24;
// 	for (i=25;i<wNoAckedFrmLen;i+=4)
// 	{
// 		memcpy_s(&dwIP,4,pBuf+i,4);
// 		// 通过IP地址查找对象
// 		nPos = pShotSpread->Find(dwIP);
// 		if(nPos>=0)
// 		{	// 生成显示信息
// 			strTemp.Format(_T("Line Name:% 5d , Point Nb:% 5d  No Response"),pShotSpread->m_arrSpread[nPos].m_dwLineNb,
// 																		pShotSpread->m_arrSpread[nPos].m_dwPointNb);
// 			DlgResult.m_arrResult.Add(strTemp);
// 			// 
// 		}
// 	}
// 	// 采集排列和应答的道数不相等，则询问用户进行选择
// 	//if(MessageBox(strTemp,_T("Caution"),MB_YESNO|MB_ICONQUESTION)==IDYES)
// 	if(DlgResult.DoModal()==IDOK)
// 	{	// 确认放炮，将炮点属性添加到激活震源属性表中
// 		m_wndActiveSource.LoadShotPointBySource(pActiveShot->m_pShotSource, pActiveShot->m_pShotPoint);
// 	}
// 	else
// 	{
// 		// 通知服务器，取消此次放炮
// 		pActiveShot->SendGeneralCMD(OPERATION_CMD_CANCEL);
// 		// 改变震源的状态
// 		SetShotSourceState(pActiveShot->m_pShotSource,SHOTSOURCE_UNUSED);		
// 		strTemp.Format(_T("Cancel Shotting: Shot Nb# %d,Source Nb# %d, Spread Count %d, No Acked Count %d !"),
// 						pActiveShot->GetShotPointNb(),pActiveShot->GetShotSourceNb(),dwSpreadCount, dwNoAcked);
// 		theApp.WriteLog(_T("CMainFrame::AnalysisProtocolOfAckResultFrm"),strTemp,LOGTYPE_WARNING,TRUE);
// 		// 删除当前激活的放炮管理对象
// 		m_AllActiveShots.RemoveActiveShotAt(nActiveShotIndex);
// 		return;
// 	}
// }
/**
 * @brief 响应鼠标右键放炮的消息
 * @note  用户在放炮表的窗口点击鼠标右键后，根据震源动态创建右击菜单的内容，用户选择震源进行放炮。
 * @note  在 VP All窗口， 对应的菜单 从 ID_VPSHOT_ONE 到 ID_VPSHOT_TEN
 * @note  在 VP TODO窗口，对应的菜单从 ID_VPTODOSHOT_ONE 到 ID_VPTODOSHOT_TEN  
 * @note  在 VP DONE窗口，对应的菜单从 ID_VPDONESHOT_ONE 到 ID_VPDONESHOT_TEN  
 * @param  UINT id, 菜单的ID号
 * @return void
 */
void CMainFrame::OnShotPoint(UINT nMenuItemID)
{	
	int				nIndex;
	DWORD			dwShotNb;
	CShotSource*	pShotSource = NULL;
	// 从VP All的窗口右键菜单选择
	if(nMenuItemID>=ID_VPSHOT_ONE && nMenuItemID<=ID_VPSHOT_TEN)
	{
		if(!m_wndVPAll.GetActiveShotPointNb(&dwShotNb))
		{
			return;
		}
		// 计算所选择的震源在网格中位置
		nIndex = nMenuItemID-ID_VPSHOT_ONE;
		if(nIndex>m_AllShotSources.GetCount())
			return;
		// 获得震源编号
		pShotSource=m_AllShotSources.GetShotSource(nIndex);
		if(pShotSource==NULL)
			return;
		// 准备放炮
		OnStartShot(dwShotNb, pShotSource->m_dwSourceNb);				
		return;
	}
	// 从VP ToDo的窗口右键菜单选择
	if(nMenuItemID>=ID_VPTODOSHOT_ONE && nMenuItemID<=ID_VPTODOSHOT_ONE)
	{
		if(!m_wndVPToDo.GetActiveShotPointNb(&dwShotNb))
		{
			return;
		}
		// 计算所选择的震源在网格中位置
		nIndex = nMenuItemID-ID_VPTODOSHOT_ONE;
		if(nIndex>m_AllShotSources.GetCount())
			return;
		// 获得震源编号
		pShotSource = m_AllShotSources.GetShotSource(nIndex);
		if(pShotSource==NULL)
			return;
		// 准备放炮
		OnStartShot(dwShotNb, pShotSource->m_dwSourceNb);					
		return;
	}
	// 从VP Done的窗口右键菜单选择
	if(nMenuItemID>=ID_VPDONESHOT_ONE && nMenuItemID<=ID_VPDONESHOT_TEN)
	{
		if(!m_wndVPDone.GetActiveShotPointNb(&dwShotNb))
		{
			return;
		}
		// 计算所选择的震源在网格中位置
		nIndex = nMenuItemID-ID_VPDONESHOT_ONE;
		if(nIndex>m_AllShotSources.GetCount())
			return;
		// 获得震源编号
		pShotSource = m_AllShotSources.GetShotSource(nIndex);
		if(pShotSource==NULL)
			return;
		// 准备放炮
		OnStartShot(dwShotNb, pShotSource->m_dwSourceNb);					
		return;
	}
	
}
/******************************************************************************************
    函    数：
    
    输    入：DWORD dwShotNb, DWORD dwSourceNb
    
    输    出：
  
    返 回 值：
    
    功    能：

			   一般是用户在界面上点击右击菜单，选择某一炮号或者震源开始放炮，发送该消息；

			   或者在自动放炮模式下，由系统自动判断放炮。

    修改历史：

*******************************************************************************************/
bool CMainFrame::OnStartShot( DWORD dwShotNb, DWORD dwSourceNb)
{	
	// 查找炮点对象
	CShotPoint* pShotPoint = m_AllShotPoints.GetShotPointByNb(dwShotNb);
	// 查找震源对象
	CShotSource* pShotSrc = m_AllShotSources.GetShotSourceByNb(dwSourceNb); 
	if(!pShotPoint || !pShotSrc)
	{
		// 发生错误
		return false;
	}
	// 查找采集排列
	CShotSpread *pSpread = m_AllSpreads.GetShotSpreadBySN(pShotPoint->m_dwSpreadNb);
	// 查找处理类型对象
	CProcessType *pProcess = m_AllProcesses.GetProcessTypeByNb(pShotPoint->m_dwProcessNb);
	if(!pSpread || !pProcess)
	{
		// 发生错误
		return false;
	}
	// 创建放炮管理对象
	CActiveShot *pActiveShot = new CActiveShot;
	if(!pActiveShot)
	{	// 创建放炮管理对象失败
		return false;
	}
	// 设置放炮点、采集排列、处理类型
	pActiveShot->SetShotPoint(pShotPoint,pSpread,pProcess);
	// 设置震源、辅助道
	pActiveShot->SetShotSource( pShotSrc,&m_AllAuxChannels);
	m_AllActiveShots.Add(pActiveShot);
	pActiveShot->m_dwTimerID = m_AllActiveShots.GetCount();
	// cxm 2012.3.7
//  	// 设置与施工服务器通信的SOCKET
//  	pActiveShot->m_pSvrSocket= m_pOperationSvrSock;
	// 服务器地址，先设为广播地址，在收到服务器应答后记录施工服务器IP
	pActiveShot->m_dwServerIP = inet_addr(("255.255.255.255"));		
	pActiveShot->m_wServerPort = OPERATION_SVRPORT;
	// 向服务器发送请求放炮请求
	pActiveShot->SendRequestShotService();
	// 改变震源的状态
	SetShotSourceState(pShotSrc,SHOTSOURCE_NEXT);
	// 启动重发定时器
	SetTimer(pActiveShot->m_dwTimerID,1000,NULL);
	CString  strTemp;
	strTemp.Format(_T("Shot Nb#: %d, Source Nb#: %d, Request to Shot! "),dwShotNb,dwSourceNb);
	// cxm 2012.3.7
//	theApp.WriteLog(_T("CMainFrame::OnStartShot"),strTemp,LOGTYPE_NORMAL,TRUE);
	return true;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 定时器处理函数，

    修改历史：

*******************************************************************************/
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i;
	int nCount = m_AllActiveShots.GetCount();
	CActiveShot* pActiveShot = NULL;
	for (i=0;i<nCount;i++)
	{
		pActiveShot=m_AllActiveShots.GetActiveShot(i);
		if(pActiveShot->m_dwTimerID !=nIDEvent )
			continue;
		// 重发计数器
		pActiveShot->m_dwRetransCount++;
		if(pActiveShot->m_dwRetransCount>2)
		{
			KillTimer(pActiveShot->m_dwTimerID);
			// 重发三次无应答，显示错误信息，关闭定时器		
			// cxm 2012.3.7
//			theApp.WriteLog(_T("CMainFrame::OnTimer"),_T("Operation Server No Answer,Stop to Shot!"),LOGTYPE_WARNING,TRUE);
			m_AllActiveShots.RemoveActiveShotAt(i);			
		}
		else
		{
			// 重发请求
			pActiveShot->SendRequestShotService();		
			// cxm 2012.3.7
//			theApp.WriteLog(_T("CMainFrame::OnTimer"),_T("Operation Server No Answer, Request to Shot !!"),LOGTYPE_WARNING,TRUE);
		}
	}
	CBCGPFrameWnd::OnTimer(nIDEvent);
}
/**
 * @brief 设置震源
 * @note  响应Setup下的Source菜单，弹出震源设置对话框，添加删除震源并设置震源属性。
 * @param  void
 * @return void
 */
void CMainFrame::OnSetupSource()
{
	// TODO: 在此添加命令处理程序代码
	CDlgSourceType	dlg;
	dlg.DoModal();
}
/**
 * @brief 设置放炮表
 * @note  弹出放炮表设置对话框
 * @param  void
 * @return void
 */
void CMainFrame::OnSetupOperation()
{
	// TODO: 在此添加命令处理程序代码
	CDlgShotPoint dlg;
	dlg.DoModal();
}

void CMainFrame::OnSetupComments()
{
	CDlgCommentType  dlg;
	dlg.DoModal();
}

void CMainFrame::OnSetupDelaysteup()
{
	CDlgDelaySetup dlg;
	dlg.DoModal();
}

void CMainFrame::OnSetupProcesstype()
{
	CDlgProcessType  dlg;
	dlg.DoModal();
}
/**
 * @brief Disassociate VP,暂停震源使用
 * @note  不再希望继续使用某一震源，或想要将其置入待命状态，或想要手工分配给它另一
 个 VP，则在Active Source窗口右键菜单中选择Disassociate VP，暂停该震源的施工。
 在滑动扫描等模式下以前分配给该未用震源的剩余 VP 将被忽略。
 * @param  void
 * @return 返回暂停的震源对象指针
 */
CShotSource* CMainFrame::OnDisassociatevp(DWORD  dwShotSourceNb)
{	
	// 通过震源Nb找到震源对象
	CShotSource* pSource=m_AllShotSources.GetShotSourceByNb(dwShotSourceNb);
	if(!pSource)
		return NULL;
	// 通过震源查找到放炮管理对象
	int iIndex = m_AllActiveShots.FindFirstActiveShotBySource(pSource);
	CActiveShot* pActiveShot = m_AllActiveShots.GetActiveShot(iIndex);
	if(pActiveShot)
	{	
		// 通知服务器停止放炮
		pActiveShot->SendGeneralCMD(OPERATION_CMD_CANCEL);
		// 如果找到对象，则删除该对象
		m_AllActiveShots.RemoveActiveShotAt(iIndex);
	}
	// 改变震源的状态，此时不需要刷新，该函数被Active Source视图调用时刷新	
	SetShotSourceState(pSource,SHOTSOURCE_UNUSED);
	return pSource;
}
/**
 * @brief "Associate VP",为震源分配炮点
 * @note  打开一个可指定要（通过输入其 Shot Number）分配给该震源的 VP 的对话框。这与用鼠标右键点击放炮表内
部所调出的“Start Seismonitor with Vib Source”命令效果基本相同。
 * @param  DWORD  dwShotSourceNb，震源编号
 * @return void
 */
CShotSource* CMainFrame::OnAssociatevp(DWORD  dwShotSourceNb)
{
	// 通过震源Nb找到震源对象
	CShotSource* pSource=m_AllShotSources.GetShotSourceByNb(dwShotSourceNb);
	if(!pSource)
		return NULL;
	// 弹出对话框
	CDlgAssociateVP		DlgAssociateVP;
	if(DlgAssociateVP.DoModal()==IDCANCEL)
		return NULL;
	// 得到炮点Nb
	DWORD	dwShotNb = DlgAssociateVP.m_dwShotNb;
	// 得到炮点对象
	CShotPoint*		pShotPoint = m_AllShotPoints.GetShotPointByNb(dwShotNb);
	if(!pShotPoint)
	{	// 没有找到炮点对象，提示炮点Nb错误
		CString strTemp;
		strTemp.Format(_T("Shot Nb#%d ,No source point found!"),dwShotNb);
		// cxm 2012.3.7
//		theApp.WriteLog(_T("CMainFrame::OnAssociatevp"),strTemp,LOGTYPE_ERROR,TRUE);
		return NULL;
	}
	// 开始放炮
	OnStartShot(dwShotNb,dwShotSourceNb);
	return pSource;
}

CShotSource*  CMainFrame::OnAssociateSPLSPN(DWORD  dwShotSourceNb)
{
	// 通过震源Nb找到震源对象
	CShotSource* pSource=m_AllShotSources.GetShotSourceByNb(dwShotSourceNb);
	if(!pSource)
		return NULL;
	// 弹出对话框
	CDlgAssociateSFLSFN		Dlg;
	if(Dlg.DoModal()==IDCANCEL)
		return NULL;
	// 得到炮点对象
	CShotPoint*		pShotPoint = m_AllShotPoints.GetShotPointBySPLSPN(Dlg.m_fSourceLine,Dlg.m_fSourceNumber);
	if(!pShotPoint)
	{	// 没有找到炮点对象，提示炮点Nb错误
		CString strTemp;
		strTemp.Format(_T("SPL %.1f,SPN %.1f,No source point found!"),Dlg.m_fSourceLine,Dlg.m_fSourceNumber);
		// cxm 2012.3.7
//		theApp.WriteLog(_T("CMainFrame::OnAssociateSPLSPN"),strTemp,LOGTYPE_ERROR,TRUE);
		return NULL;
	}
	// 开始放炮
	OnStartShot(pShotPoint->m_dwShotNb,dwShotSourceNb);
	return pSource;
}