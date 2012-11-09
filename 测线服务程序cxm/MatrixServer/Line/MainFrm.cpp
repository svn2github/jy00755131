
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Line.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_MESSAGE(WM_MSG_CLIENT, &CMainFrame::OnMsgClient)
	ON_WM_SETTINGCHANGE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_uiServerPort = ServerListenPort;
	m_strServerIP = ServerIP;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);


	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	// @@@端口和IP的命令行参数解析

	m_oComDll.OnInit(_T("MatrixCommDll.dll"), m_uiServerPort, m_strServerIP, this->m_hWnd);
	m_oComDll.m_oXMLDllLine.OnInit(_T("MatrixServerDll.dll"));
	m_oComDll.m_hWnd = m_hWnd;
//	char pParameter[256];
// 	CString str = _T("");
// 	pParameter[0] = '0';
// 	pParameter[1] = '3';
// 	pParameter[2] = '2';
// 	pParameter[3] = '7';
// 	str = CString(pParameter);
//	memcpy(pParameter, "0327", 4);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“类视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	// 创建属性窗口
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“属性”窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_oComDll.m_oXMLDllLine.OnClose();
	m_oComDll.OnClose();
	CFrameWndEx::OnClose();
}
// 查询 SurveyXML 文件信息
// void QuerySurverySetupData(char* cProcBuf, int& iPos, m_oLineSetupDataStruct* pLineSetupData)
// {
// 	list<m_oSurveryStruct>::iterator iter;
// 	EnterCriticalSection(&pLineSetupData->m_oSecCommInfo);
// 	for (iter = pLineSetupData->m_olsSurveryStruct.begin();
// 		iter != pLineSetupData->m_olsSurveryStruct.end(); iter++)
// 	{
// 		memcpy(&cProcBuf[iPos], &iter->m_uiLine, 4);
// 		iPos += 4;
// 		memcpy(&cProcBuf[iPos], &iter->m_usReceiverSectionSize, 2);
// 		iPos += 2;
// 		memcpy(&cProcBuf[iPos], iter->m_pcReceiverSection, iter->m_usReceiverSectionSize);
// 		iPos += iter->m_usReceiverSectionSize;
// 	}
// 	LeaveCriticalSection(&pLineSetupData->m_oSecCommInfo);
// }
LRESULT CMainFrame::OnMsgClient(WPARAM wParam, LPARAM lParam)
{
	CString str = _T("");
// 	int iPos = 0;
// 	char cp[20000];
	switch(wParam)
	{
	case CmdClientConnect:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Connect to Server!"));
		// 查询接收区域（帧内容为行数（4个字节）+列数（4个字节））
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryRevSection, NULL, 0);
		// 查询 SurveyXML 文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySurveyXMLInfo, NULL, 0);
		// 查询 PointCode 文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryPointCodeXMLInfo, NULL, 0);
		// 查询 Sensor XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorXMLInfo, NULL, 0);
		// 查询 Marker XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryMarkerXMLInfo, NULL, 0);
		// 查询 Aux XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryAuxXMLInfo, NULL, 0);
		// 查询 Detour XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryDetourXMLInfo, NULL, 0);
		// 查询 Mute XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryMuteXMLInfo, NULL, 0);
		// 查询 BlastMachine XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryBlastMachineXMLInfo, NULL, 0);
		// 查询 Absolute XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryAbsoluteXMLInfo, NULL, 0);
		// 查询 Generic XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryGenericXMLInfo, NULL, 0);
		// 查询 Look XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryLookXMLInfo, NULL, 0);
		// 查询 InstrumentTestBase XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrumentTestBaseXMLInfo, NULL, 0);
		// 查询 SensorTestBase XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorTestBaseXMLInfo, NULL, 0);
		// 查询 InstrumentTestLimit XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrumentTestLimitXMLInfo, NULL, 0);
		// 查询 SensorTestLimit XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorTestLimitXMLInfo, NULL, 0);
		// 查询 InstrumentTest XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryInstrumentTestXMLInfo, NULL, 0);
		// 查询 SensorTest XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySensorTestXMLInfo, NULL, 0);
		// 查询 MultipleTest XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryMultipleTestXMLInfo, NULL, 0);
		// 查询 SeisMonitorTest XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQuerySeisMonitorTestXMLInfo, NULL, 0);
		// 查询 LAULeakage XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryLAULeakageXMLInfo, NULL, 0);
		// 查询 FormLine XML文件信息
		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdQueryFormLineXMLInfo, NULL, 0);
		break;
		// 查询接收区域
	case CmdQueryRevSection:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the size of receive section!"));
		break;
	case CmdSetFieldOn:
		// 上电
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the command of Field On return!"));
		break;
	case CmdSetFieldOff:
		// 断电
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the command of Field On return!"));
		break;
		// Field On需要等待的时间（帧内容为执行FieldOn剩余时间，为0表示无需等待）
	case CmdFieldOnWaitTime:
		str.Format(_T("Field on needs to wait about %ds!"), lParam);
		m_wndOutput.m_wndOutputBuild.AddString(str);
		if (lParam != 0)
		{
			AfxMessageBox(str);
		}
		break;
		// 设置 SurveyXML 文件信息（帧内容为信息结构体）
	case CmdSetSurveyXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of survey setup!"));
		break;
		// 设置 PointCode XML文件信息（帧内容为信息结构体）
	case CmdSetPointCodeXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of point code setup!"));
		break;
		// 设置 Sensor XML文件信息（帧内容为信息结构体）
	case CmdSetSensorXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of sensor setup!"));
		break;
		// 设置 Marker XML文件信息（帧内容为信息结构体）
	case CmdSetMarkerXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of marker setup!"));
		break;
		// 设置 Aux XML文件信息（帧内容为信息结构体）
	case CmdSetAuxXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of aux setup!"));
		break;
		// 设置 Detour XML文件信息（帧内容为信息结构体）
	case CmdSetDetourXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of detour setup!"));
		break;
		// 设置 Mute XML文件信息（帧内容为信息结构体）
	case CmdSetMuteXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of mute setup!"));
		break;
		// 设置 BlastMachine XML文件信息（帧内容为信息结构体）
	case CmdSetBlastMachineXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of blastmachine setup!"));
		break;
		// 设置 Absolute XML文件信息（帧内容为信息结构体）
	case CmdSetAbsoluteXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of absolute setup!"));
		break;
		// 设置 Generic XML文件信息（帧内容为信息结构体）
	case CmdSetGenericXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of generic setup!"));
		break;
		// 设置 Look XML文件信息（帧内容为信息结构体）
	case CmdSetLookXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of look setup!"));
		break;
		// 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）
	case CmdSetInstrumentTestBaseXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of instrument test base setup!"));
		break;
		// 设置 SensorTestBase XML文件信息（帧内容为信息结构体）
	case CmdSetSensorTestBaseXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of sensor test base setup!"));
		break;
		// 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）
	case CmdSetInstrumentTestLimitXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of instrument test limit setup!"));
		break;
	case CmdSetSensorTestLimitXMLInfo:
		// 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of sensor test limit setup!"));
		break;
		// 设置 InstrumentTest XML文件信息（帧内容为信息结构体）
	case CmdSetInstrumentTestXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of instrument test setup!"));
		break;
		// 设置 SensorTest XML文件信息（帧内容为信息结构体）
	case CmdSetSensorTestXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of sensor test setup!"));
		break;
		// 设置 MultipleTest XML文件信息（帧内容为信息结构体）
	case CmdSetMultipleTestXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of multiple test setup!"));
		break;
		// 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）
	case CmdSetSeisMonitorTestXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of seismonitor setup!"));
		break;
		// 设置 LAULeakage XML文件信息（帧内容为信息结构体）
	case CmdSetLAULeakageXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of lauleakage setup!"));
		break;
		// 设置 FormLine XML文件信息（帧内容为信息结构体）
	case CmdSetFormLineXMLInfo:
		m_wndOutput.m_wndOutputBuild.AddString(_T("Receive the informations of form line setup!"));
		// 测试写数据到服务器配置文件
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[0] = '2';
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[1] = '-';
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[2] = '1';
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[3] = '2';
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[4] = '5';
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[5] = 'p';
// 		m_oComDll.m_oXMLDllLine.m_pLineSetupData->m_olsSurveryStruct.begin()->m_pcReceiverSection[6] = '2';
// 		QuerySurverySetupData(cp, iPos, m_oComDll.m_oXMLDllLine.m_pLineSetupData);
// 		m_oComDll.m_pCommClient->m_oSndFrame.MakeSetFrame(CmdSetSurveyXMLInfo, cp, iPos);
		break;
	default:
		break;
	}

	return TRUE;
}