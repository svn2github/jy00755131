// SensorGraphChildFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "SeismonitorGraphChildFrame.h"
#include "SeismonitorGraphView.h"

// CSeismonitorGraphChildFrame

IMPLEMENT_DYNCREATE(CSeismonitorGraphChildFrame, CBCGPMDIChildWnd)

CSeismonitorGraphChildFrame::CSeismonitorGraphChildFrame()
{
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pSiteData = &pMainFrame->m_oSiteData;

	m_uiSensorNbCount = m_pSiteData->m_oSensorList.m_uiCountAll;	// 检波器Nb数量
	m_uiSensorNb = 100;	// 要显示测试结果的检波器Nb

	m_uiTestType = 22;
	// 得到噪声监测判据
	m_fSeismonitorLimit = m_pSiteData->GetSeismonitorLimit();
	// 得到噪声监测判据字符串
	SetSeismonitorLimitString();

	// 野外噪声监测操作 1-启动测试；0-停止测试
	m_byTestOperation = 0;
}

CSeismonitorGraphChildFrame::~CSeismonitorGraphChildFrame()
{
}


BEGIN_MESSAGE_MAP(CSeismonitorGraphChildFrame, CBCGPMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SENSORQCTYPELIST, OnSelectSensorNb)
	ON_COMMAND(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SEISMONITOR, OnTestSeismonitor)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SEISMONITOR, OnUpdateUITestSeismonitor)
	ON_COMMAND(ID_BUTTON_MATRIX_GRAPH_ZOOMIN, OnZoomIn)
	ON_COMMAND(ID_BUTTON_MATRIX_GRAPH_ZOOMOUT, OnZoomOut)
	ON_COMMAND_RANGE(ID_BUTTON_MATRIX_SENSOR_GRAPH_NBSTART, ID_BUTTON_MATRIX_SENSOR_GRAPH_NBSTART + 100, OnShowSensorNb)
	ON_CBN_SELCHANGE(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SENSORQCTYPELIST, OnSelectSensorNb)
	ON_COMMAND(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMIT, OnSliderTestLimit)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
END_MESSAGE_MAP()


// CSeismonitorGraphChildFrame 消息处理程序

int CSeismonitorGraphChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableDocking(CBRS_ALIGN_ANY);

	// 左导航条上部
	m_oLeftViewBarSeismonitorTop.Create (("Garph Navigator View"), this, CRect (0, 0, 0, 0), TRUE, 1,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT);
	m_oLeftViewBarSeismonitorTop.EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_oLeftViewBarSeismonitorTop);
	// 左导航条下部
	m_oLeftViewBarSensorBottom.Create ((""), this, CRect (0, 0, 0, 0), TRUE, 2,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM);
	m_oLeftViewBarSensorBottom.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_oLeftViewBarSensorBottom);
	m_oLeftViewBarSensorBottom.DockToWindow(&m_oLeftViewBarSeismonitorTop, CBRS_ALIGN_BOTTOM);

	// 上工具条
	m_oGraphViewTopToolBar.Create (this, WS_CHILD | WS_VISIBLE | CBRS_TOP, IDR_TOOLBAR_MATRIX_SEISMONITOR_GRAPH_TOP);
	m_oGraphViewTopToolBar.LoadToolBar (IDR_TOOLBAR_MATRIX_SEISMONITOR_GRAPH_TOP, 0, 0, TRUE);

	// 下工具条
	m_oGraphViewBottomToolBar.Create (this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, 2);
	m_oGraphViewBottomToolBar.LoadToolBar (IDR_TOOLBAR_MATRIX_GRAPH_BOTTOM, 0, 0, TRUE);

	m_oGraphViewTopToolBar.SetOwner (this);
	m_oGraphViewTopToolBar.SetRouteCommandsViaFrame(FALSE);

	m_oGraphViewBottomToolBar.SetOwner (this);
	m_oGraphViewBottomToolBar.SetRouteCommandsViaFrame(FALSE);

	EnableDocking(CBRS_ALIGN_ANY);
	m_oGraphViewTopToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_oGraphViewTopToolBar);
	m_oGraphViewBottomToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_oGraphViewBottomToolBar);

	return 0;
}

afx_msg LRESULT CSeismonitorGraphChildFrame::OnToolbarReset(WPARAM wp, LPARAM)
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
	case IDR_TOOLBAR_MATRIX_SEISMONITOR_GRAPH_TOP:

		POSITION pos;
		CSensorData oSensorData;
		CString strSensorNb;
		for(unsigned int i = 0; i < m_pSiteData->m_oSensorList.m_uiCountAll; i++)
		{
			pos = m_pSiteData->m_oSensorList.m_olsSensor.FindIndex(i);
			oSensorData = m_pSiteData->m_oSensorList.m_olsSensor.GetAt(pos);

			strSensorNb.Format("%d", oSensorData.m_uiNb);
			m_oGraphViewTopToolBar.InsertButton(CToolbarLabel(ID_BUTTON_MATRIX_SENSOR_GRAPH_NBSTART + i, strSensorNb), i);

			CToolbarLabel* pBNSensorNb = NULL;
			pBNSensorNb = (CToolbarLabel*)m_oGraphViewTopToolBar.GetButton(i);
			pBNSensorNb->m_nStyle = TBBS_CHECKBOX;
		}

		CToolbarGraph oBNGraph (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_TESTMARKER);
		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_TESTMARKER, oBNGraph);
		CToolbarGraph* pBNGraph = NULL;
		pBNGraph = (CToolbarGraph*)m_oGraphViewTopToolBar.GetButton(6 + m_uiSensorNbCount);
		pBNGraph->m_iMode = 3;
		pBNGraph->m_strLabel = m_strSeismonitorLimitA;

		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SENSORQCTYPELABEL, CToolbarLabel (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SENSORQCTYPELABEL, "Sensor Qc type:"));

		CBCGPToolbarComboBoxButton comboButtonSensorNb(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SENSORQCTYPELIST, -1, CBS_DROPDOWNLIST);
		// 初始化下拉框按钮检波器Nb下拉框
		InitButtonForSensorNbComboBox(&comboButtonSensorNb);
		comboButtonSensorNb.SelectItem(0);
		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SENSORQCTYPELIST, comboButtonSensorNb);

		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMITLABEL, CToolbarLabel (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMITLABEL, "Gain:"));

		CBCGPSliderButton btnSlider (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMIT);
		int SeismonitorLimit = int(m_fSeismonitorLimit);
		btnSlider.SetRange (SeismonitorLimit - 48, SeismonitorLimit + 48);
		btnSlider.SetValue (SeismonitorLimit);
		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMIT, btnSlider);

		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMITVALUE, CToolbarLabel (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMITVALUE, m_strSeismonitorLimitB));

		m_oGraphViewTopToolBar.ReplaceButton (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SEISMONITOR, CToolbarLabel (ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_SEISMONITOR, "Seismonitor"));
/*
		CBCGPToolbarButton* pBNSeismonitor = NULL;
		pBNSeismonitor = m_oGraphViewTopToolBar.GetButton(10 + m_uiSensorNbCount);
		pBNSeismonitor->m_nStyle = TBBS_CHECKBOX;
*/
		break;
	}

	return 0;
}

/**
* 响应上工具条按钮；选择检波器Nb
* @param void
* @return void
*/
void CSeismonitorGraphChildFrame::OnShowSensorNb(UINT nID) 
{
	CSeismonitorGraphView* pView = (CSeismonitorGraphView*)GetActiveView();

	unsigned int uiIndex = 0;
	uiIndex = nID - ID_BUTTON_MATRIX_SENSOR_GRAPH_NBSTART;

	CToolbarGraphButton* pBNSensorNb = NULL;
	pBNSensorNb = (CToolbarGraphButton*)m_oGraphViewTopToolBar.GetButton(uiIndex);

	POSITION pos;
	CSensorData oSensorData;
	pos = m_pSiteData->m_oSensorList.m_olsSensor.FindIndex(uiIndex);
	oSensorData = m_pSiteData->m_oSensorList.m_olsSensor.GetAt(pos);

	if(1 == pBNSensorNb->m_iMode)
	{
		pBNSensorNb->m_iMode = 0;
		m_oSensorNbMap.RemoveKey(oSensorData.m_uiNb);
	}
	else
	{
		m_oSensorNbMap.SetAt(oSensorData.m_uiNb, oSensorData.m_uiNb);
		pBNSensorNb->m_iMode = 1;
	}
	m_oGraphViewTopToolBar.InvalidateButton(uiIndex);

	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；选择检波器Nb
* @param void
* @return void
*/
void CSeismonitorGraphChildFrame::OnSelectSensorNb() 
{
	CSeismonitorGraphView* pView = (CSeismonitorGraphView*)GetActiveView();

	CBCGPToolbarComboBoxButton* pComboButton = NULL;
	int iCurSel = 0;
	pComboButton = (CBCGPToolbarComboBoxButton*)m_oGraphViewTopToolBar.GetButton(3 + m_uiSensorNbCount);
	iCurSel = pComboButton->GetCurSel();
	m_uiSensorNb = pComboButton->GetItemData(iCurSel);

	pView->RedrawWindow();
}

/**
* 响应上工具条按钮；滑动测试门限
* @param void
* @return void
*/
void CSeismonitorGraphChildFrame::OnSliderTestLimit() 
{
	if (GetAsyncKeyState (VK_LBUTTON) == 0)
	{
		m_fSeismonitorLimit = (float)CBCGPSliderButton::GetPos(ID_BUTTON_MATRIX_SEISMONITOR_GRAPH_LIMIT);
		// 得到噪声监测判据字符串
		SetSeismonitorLimitString();		

		CToolbarGraph* pBNGraph = NULL;
		pBNGraph = (CToolbarGraph*)m_oGraphViewTopToolBar.GetButton(6 + m_uiSensorNbCount);
		pBNGraph->m_strLabel = m_strSeismonitorLimitA;
		m_oGraphViewTopToolBar.InvalidateButton(6 + m_uiSensorNbCount);

		CBCGPToolbarButton* pBNSeismonitorLimitLabel = NULL;
		pBNSeismonitorLimitLabel = m_oGraphViewTopToolBar.GetButton(9 + m_uiSensorNbCount);
		pBNSeismonitorLimitLabel->m_strText = m_strSeismonitorLimitB;
		m_oGraphViewTopToolBar.InvalidateButton(9 + m_uiSensorNbCount);
	}
}

/**
* 响应上工具条按钮；选择测试Seismonitor
* @param void
* @return void
*/
void CSeismonitorGraphChildFrame::OnTestSeismonitor() 
{
	CSeismonitorGraphView* pView = (CSeismonitorGraphView*)GetActiveView();

	// 野外噪声监测操作 1-启动测试；0-停止测试
	if(0 == m_byTestOperation)
	{
		if("" != pView->m_strAbsoluteSpread)
		{
			m_byTestOperation = 1;
			// 响应噪声监测图形视图Seismonitor按钮
			m_pSiteData->OnSendCMDFrameForGraphViewNoiseSeismonitorButton(m_byTestOperation, pView->m_strAbsoluteSpread);
		}
		else
		{
			AfxMessageBox("Absolute Spread Not Defined.", MB_OK | MB_ICONSTOP);
		}
	}
	else if(1 == m_byTestOperation)
	{
		m_byTestOperation = 0;
		// 响应噪声监测图形视图Seismonitor按钮
		m_pSiteData->OnSendCMDFrameForGraphViewNoiseSeismonitorButton(m_byTestOperation, pView->m_strAbsoluteSpread);
	}
}

// 放大
void CSeismonitorGraphChildFrame::OnZoomIn() 
{
	CSeismonitorGraphView* pView = (CSeismonitorGraphView*)GetActiveView();
	pView->OnZoomIn();	// 放大
}

// 缩小
void CSeismonitorGraphChildFrame::OnZoomOut() 
{
	CSeismonitorGraphView* pView = (CSeismonitorGraphView*)GetActiveView();	
	pView->OnZoomOut();	// 缩小
}

// 显示单元的测线号和测点号
void CSeismonitorGraphChildFrame::OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint)
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
void CSeismonitorGraphChildFrame::OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY)
{
	CSeismonitorGraphView* pView = (CSeismonitorGraphView*)GetActiveView();
	// 导航局部放大
	pView->OnZoomInNavigater(fZoomRate, fRateX, fRateY);
}

/**
* 响应显示属性值
* @param unsigned int uiIndex 仪器索引号
* @return void
*/
void CSeismonitorGraphChildFrame::OnShowProperty(unsigned int uiIndex)
{
	m_oLeftViewBarSensorBottom.OnShowProperty(uiIndex);
}

/**
* 响应设置测线数量
* @param unsigned int uiLineCount 测线数量
* @return void
*/
void CSeismonitorGraphChildFrame::OnSetLineCount(unsigned int uiLineCount)
{
	m_oLeftViewBarSeismonitorTop.OnSetLineCount(uiLineCount);
}

/**
* 初始化下拉框按检波器Nb下拉框
* @param CBCGPToolbarComboBoxButton* pComboButton
* @return void
*/
void CSeismonitorGraphChildFrame::InitButtonForSensorNbComboBox(CBCGPToolbarComboBoxButton* pComboButton)
{
	POSITION pos;
	CSensorData oSensorData;
	CString strSensorNb;
	pComboButton->AddItem ("All", 100);
	for(unsigned int i = 0; i < m_pSiteData->m_oSensorList.m_uiCountAll; i++)
	{
		pos = m_pSiteData->m_oSensorList.m_olsSensor.FindIndex(i);
		oSensorData = m_pSiteData->m_oSensorList.m_olsSensor.GetAt(pos);
		strSensorNb.Format("%d", oSensorData.m_uiNb);
		pComboButton->AddItem (strSensorNb, oSensorData.m_uiNb);
	}
}

/**
* 得到噪声监测判据字符串
* @param void
* @return void
*/
void CSeismonitorGraphChildFrame::SetSeismonitorLimitString()
{
	CString strData;
	m_strSeismonitorLimitA.Format("%3.0f", m_fSeismonitorLimit);
	for(int i = 1; i < 7; i++)
	{
		strData = m_strSeismonitorLimitA;
		m_strSeismonitorLimitA.Format("%3.0f %s", (m_fSeismonitorLimit - (float)(i * 6)), strData);
	}

	m_strSeismonitorLimitB.Format("%3.0f(db)", m_fSeismonitorLimit);
}

void CSeismonitorGraphChildFrame::OnUpdateUITestSeismonitor(CCmdUI* pCmdUI) 
{
	// 判断：野外噪声监测操作 0-停止测试
	if(0 == m_byTestOperation)
	{
		pCmdUI->SetCheck(0);
	}
	// 判断：野外噪声监测操作 1-启动测试
	else if(1 == m_byTestOperation)
	{
		pCmdUI->SetCheck(1);
	}
	
}