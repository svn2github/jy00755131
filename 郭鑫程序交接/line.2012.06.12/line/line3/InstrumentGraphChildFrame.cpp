// InstrumentGraphChildFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "InstrumentGraphChildFrame.h"
#include "InstrumentGraphView.h"

// CInstrumentGraphChildFrame

IMPLEMENT_DYNCREATE(CInstrumentGraphChildFrame, CBCGPMDIChildWnd)

#ifdef _DEBUG

void test_driver_for_instrument_graph_view()
{
	return;

	int loop = 0x0;
	DWORD tmp = 0x0;
	DWORD dw = 10000;
	matrix_data::data_base_helper* helper = ((CLineApp*)AfxGetApp())->get_data_base_helper();
	matrix_data::data_base_helper::device_info di;
	matrix_data::data_base_helper::sensor si;

	for(int i = 0; i < 5; ++i){//line
		for(int j = 0; j < 1; ++j){//section
			for(int k = 0; k <10; ++k){
				di.clear();
				di.line_index_ =  5 + i ;
				di.section_index_ = j;
				dw += (i * 1000 + k* 100 + j *10);
				if(k == 0){ 
					di.sn_ = dw + 1;
					if(i != 4) di.m_uiSNCrossDown = 1;
					di.m_uiSNInstrumentRight = 1;
				}
				else if(k % 5 == 0) {
					di.sn_ = dw + 2;
					di.m_uiSNInstrumentRight = 1;
				}
				else{
					di.sn_ = dw + 3;
					if(k != 9) di.m_uiSNInstrumentRight = 1;
				}
				di.m_uiPointNb = k -249;
				int tmp = di.m_uiPointNb * 1000 + di.line_index_;
				di.sensors_.push_back(si);
				helper->add_device_info(di);
				helper->add_point_nb_to_sn_info(tmp, di.sn_);
			}
		}
	}
}

#endif //_DEBUG

CInstrumentGraphChildFrame::CInstrumentGraphChildFrame()
{
	//CMainFrame* pMainFrame = NULL;
	//pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//m_pSiteData = &pMainFrame->m_oSiteData;
	data_base_helper_ = ((CLineApp*)AfxGetApp())->get_data_base_helper();

	matrix_data::data_base_helper::instrument_limit_vector const& ilv = data_base_helper_->get_instrument_limit_vector();
	matrix_data::data_base::instrument_limit_vector::const_iterator itr = ilv.begin();
	m_strTestType.Format("%.2f", itr->m_fLimit);
	m_uiTestType = itr->m_uiTestType;
//	m_strTestType = m_pSiteData->GetInsarumentTestTypeLimitStringByID(m_uiTestType);

	/**是否显示测区坐标*/
	m_bShowLineNbPointNb = true;
	/**是否显示连接线*/
	m_bShowLinkLine = true;
	/**是否显示交叉站*/
	m_bShowInstrumentCross = true;
	/**是否显示电源站*/
	m_bShowInstrumentPower = true;
	/**是否显示采集站*/
	m_bShowInstrumentCollect = true;

#ifdef _DEBUG
	test_driver_for_instrument_graph_view();
#endif // _DEBUG
}

CInstrumentGraphChildFrame::~CInstrumentGraphChildFrame()
{
}


BEGIN_MESSAGE_MAP(CInstrumentGraphChildFrame, CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_POINT, OnShowLineNbPointNb)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_LINE, OnShowLinkLine)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_CROSS, OnShowInstrumentCross)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_POWER, OnShowInstrumentPower)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_COLLECT, OnShowInstrumentCollect)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_LOOK, OnLook)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCLIST, OnSelectTestType)
	ON_COMMAND(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCGO, OnTestGo)
	ON_COMMAND(ID_BUTTON_MATRIX_GRAPH_ZOOMIN, OnZoomIn)
	ON_COMMAND(ID_BUTTON_MATRIX_GRAPH_ZOOMOUT, OnZoomOut)
	ON_CBN_SELCHANGE(ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCLIST, OnSelectTestType)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()

// CInstrumentGraphChildFrame 消息处理程序

int CInstrumentGraphChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableDocking(CBRS_ALIGN_ANY);

	// 左导航条上部
	m_oLeftViewBarInstrumentTop.Create (("Garph Navigator View"), this, CRect (0, 0, 0, 0), TRUE, 1,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT);
	m_oLeftViewBarInstrumentTop.EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_oLeftViewBarInstrumentTop);
	// 左导航条下部
	m_oLeftViewBarInstrumentBottom.Create ((""), this, CRect (0, 0, 0, 0), TRUE, 2,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM);
	m_oLeftViewBarInstrumentBottom.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_oLeftViewBarInstrumentBottom);
	m_oLeftViewBarInstrumentBottom.DockToWindow(&m_oLeftViewBarInstrumentTop, CBRS_ALIGN_BOTTOM);

	// 上工具条
	m_oGraphViewTopToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP, IDR_TOOLBAR_MATRIX_INSTRUMENT_GRAPH_TOP);
	m_oGraphViewTopToolBar.LoadToolBar (IDR_TOOLBAR_MATRIX_INSTRUMENT_GRAPH_TOP, 0, 0, TRUE);

	// 下工具条
	m_oGraphViewBottomToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, 1);
	m_oGraphViewBottomToolBar.LoadToolBar (IDR_TOOLBAR_MATRIX_GRAPH_BOTTOM, 0, 0, TRUE);

	m_oGraphViewTopToolBar.SetOwner(this);
	m_oGraphViewTopToolBar.SetRouteCommandsViaFrame(FALSE);

	m_oGraphViewBottomToolBar.SetOwner(this);
	m_oGraphViewBottomToolBar.SetRouteCommandsViaFrame(FALSE);

	m_oGraphViewTopToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_oGraphViewTopToolBar);
	m_oGraphViewBottomToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_oGraphViewBottomToolBar);

	return 0;
}

afx_msg LRESULT CInstrumentGraphChildFrame::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_TOOLBAR_MATRIX_GRAPH_BOTTOM:
		m_oGraphViewBottomToolBar.ReplaceButton (ID_BUTTON_MATRIX_GRAPH_LINENAMELABEL, CToolbarLabel (ID_BUTTON_MATRIX_GRAPH_LINENAMELABEL, _T("Line Name:")));
		m_oGraphViewBottomToolBar.ReplaceButton (ID_BUTTON_MATRIX_GRAPH_POINTNBLABEL, CToolbarLabel (ID_BUTTON_MATRIX_GRAPH_POINTNBLABEL, _T("Point Number:")));
		m_oGraphViewBottomToolBar.ReplaceButton (ID_BUTTON_MATRIX_GRAPH_LINENAME, CBCGPToolbarEditBoxButton(ID_BUTTON_MATRIX_GRAPH_LINENAME, -1, WS_CHILD | WS_VISIBLE));
		m_oGraphViewBottomToolBar.ReplaceButton (ID_BUTTON_MATRIX_GRAPH_POINTNB, CBCGPToolbarEditBoxButton(ID_BUTTON_MATRIX_GRAPH_POINTNB, -1, WS_CHILD | WS_VISIBLE));
		break;
	case IDR_TOOLBAR_MATRIX_INSTRUMENT_GRAPH_TOP:
		CBCGPToolbarButton* pBN = NULL;
		pBN = m_oGraphViewTopToolBar.GetButton(0);
		pBN->m_nStyle = TBBS_CHECKBOX | TBBS_CHECKED;
		pBN = m_oGraphViewTopToolBar.GetButton(1);
		pBN->m_nStyle = TBBS_CHECKBOX | TBBS_CHECKED;
		pBN = m_oGraphViewTopToolBar.GetButton(2);
		pBN->m_nStyle = TBBS_CHECKBOX | TBBS_CHECKED;
		pBN = m_oGraphViewTopToolBar.GetButton(3);
		pBN->m_nStyle = TBBS_CHECKBOX | TBBS_CHECKED;
		pBN = m_oGraphViewTopToolBar.GetButton(4);
		pBN->m_nStyle = TBBS_CHECKBOX | TBBS_CHECKED;

		CToolbarGraph oBNGraph (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_TESTMARKER);
		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_TESTMARKER, oBNGraph);
		CToolbarGraph* pBNGraph = NULL;
		pBNGraph = (CToolbarGraph*)m_oGraphViewTopToolBar.GetButton(7);
		pBNGraph->m_iMode = 1;
		pBNGraph->m_strLabel = m_strTestType;

		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCLABEL, CToolbarLabel (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCLABEL, _T("Qc:")));

		CBCGPToolbarComboBoxButton comboButton (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCLIST, -1, CBS_DROPDOWNLIST);
		// 初始化下拉框按钮测试类型下拉框
		InitButtonForTestTyperComboBox(&comboButton);
		comboButton.SelectItem(0);
		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCLIST, comboButton);

		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCGO, CToolbarLabel (ID_BUTTON_MATRIX_INSTRUMENT_GRAPH_QCGO, "  GO  "));
		break;
	}

	return 0;
}

/**
* 响应上工具条按钮；显示线号桩号
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnShowLineNbPointNb() 
{
	//是否显示测区坐标
	m_bShowLineNbPointNb = !m_bShowLineNbPointNb;
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->RedrawWindow();

}

/**
* 响应上工具条按钮；显示连接线
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnShowLinkLine() 
{
	//是否显示连接线
	m_bShowLinkLine = !m_bShowLinkLine;
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；显示交叉站
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnShowInstrumentCross() 
{
	//是否显示交叉站
	m_bShowInstrumentCross = !m_bShowInstrumentCross;
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；显示电源站
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnShowInstrumentPower() 
{
	//是否显示电源站
	m_bShowInstrumentPower = !m_bShowInstrumentPower;
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；显示采集站
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnShowInstrumentCollect() 
{
	//是否显示采集站
	m_bShowInstrumentCollect = !m_bShowInstrumentCollect;
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；选择Look
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnLook() 
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	// 处理命令	100-现场数据输出更新通知命令
	pMainFrame->ProcInterFaceCommandForSiteDataOutput();
}

/**
* 响应上工具条按钮；选择测试类型
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnSelectTestType() 
{
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();

	CBCGPToolbarComboBoxButton* pComboButton = NULL;
	pComboButton = (CBCGPToolbarComboBoxButton*)m_oGraphViewTopToolBar.GetButton(12);
	m_strTestType = pComboButton->GetText();


	matrix_data::data_base_helper::instrument_limit il;
	StrCpy(il.m_pcDescr, m_strTestType.LockBuffer());
	m_strTestType.UnlockBuffer();
	data_base_helper_->find_instrument_limit_by_name(il);
	m_uiTestType = il.m_uiTestType;
	CToolbarGraph* pBNGraph = NULL;
	pBNGraph = (CToolbarGraph*)m_oGraphViewTopToolBar.GetButton(7);
	pBNGraph->m_iMode = 1;
	pBNGraph->m_strLabel.Format(__TEXT("%.2f"), il.m_fLimit);
	m_oGraphViewTopToolBar.InvalidateButton(7);

	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；选择测试GO
* @param void
* @return void
*/
void CInstrumentGraphChildFrame::OnTestGo() 
{
	// cxm 2011-09-19 设备视图Go响应函数
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->OnClearCheckInstrumentTestResult(m_uiTestType);

	//if("" != pView->m_strAbsoluteSpread)
	//{
	//	// 响应仪器、检波器图形视图测试GO按钮
	//	m_pSiteData->OnSendCMDFrameForGraphViewInstrumentOrSensorTestGOButton(1, m_uiTestType, pView->m_strAbsoluteSpread);
	//}
	//else
	//{
	//	AfxMessageBox("Absolute Spread Not Defined.", MB_OK | MB_ICONSTOP);
	//}
}

// 放大
void CInstrumentGraphChildFrame::OnZoomIn() 
{
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->OnZoomIn();	// 放大
}

// 缩小
void CInstrumentGraphChildFrame::OnZoomOut() 
{
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	pView->OnZoomOut();	// 缩小

}

// 显示单元的测线号和测点号
void CInstrumentGraphChildFrame::OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	CString strData;
	CBCGPToolbarEditBoxButton* pEditBoxButton = NULL;
	pEditBoxButton = (CBCGPToolbarEditBoxButton*)m_oGraphViewBottomToolBar.GetButton(3);
	pEditBoxButton->EnableWindow(TRUE);
	strData.Format("%d", uiNbLine);
	pEditBoxButton->SetContents(strData);

	pEditBoxButton = (CBCGPToolbarEditBoxButton*)m_oGraphViewBottomToolBar.GetButton(5);
	pEditBoxButton->EnableWindow(TRUE);
	strData.Format("%d", uiNbPoint);
	pEditBoxButton->SetContents(strData);
}

// 响应导航局部放大
void CInstrumentGraphChildFrame::OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY)
{
	CInstrumentGraphView* pView = (CInstrumentGraphView*)GetActiveView();
	// 导航局部放大
	pView->OnZoomInNavigater(fZoomRate, fRateX, fRateY);
}

/**
* 响应显示属性值
* @param unsigned int uiIndex 仪器索引号
* @return void
*/
void CInstrumentGraphChildFrame::OnShowProperty(unsigned int uiIndex)
{
	//m_oLeftViewBarInstrumentBottom.OnShowProperty(uiIndex);
}

/**
* 响应显示迂回道图形视图
* @param unsigned int uiCount 迂回道中仪器数量
* @return void
*/
void CInstrumentGraphChildFrame::OnShowDetourGraphView(unsigned int uiCount)
{
	m_oLeftViewBarInstrumentBottom.OnShowDetourGraphView(uiCount);
}

/**
* 响应设置测线数量
* @param unsigned int uiLineCount 测线数量
* @return void
*/
void CInstrumentGraphChildFrame::OnSetLineCount(unsigned int uiLineCount)
{
	m_oLeftViewBarInstrumentTop.OnSetLineCount(uiLineCount);
}

/**
* // 初始化下拉框按钮测试类型下拉框
* @param CBCGPToolbarComboBoxButton* pComboButton
* @return void
*/
void CInstrumentGraphChildFrame::InitButtonForTestTyperComboBox(CBCGPToolbarComboBoxButton* pComboButton)
{
	CString strTestType;
	matrix_data::data_base_helper::instrument_limit_vector const& ilv = data_base_helper_->get_instrument_limit_vector();
	matrix_data::data_base::instrument_limit_vector::const_iterator itr;

	for(itr = ilv.begin(); itr != ilv.end(); ++itr){
		strTestType = itr->m_pcDescr;
		pComboButton->AddItem (strTestType);
	}
}