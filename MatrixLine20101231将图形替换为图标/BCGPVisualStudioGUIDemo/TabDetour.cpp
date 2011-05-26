// TabDetour.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabDetour.h"


// CTabDetour

IMPLEMENT_DYNAMIC(CTabDetour, CTabPage)

CTabDetour::CTabDetour()
{

}

CTabDetour::~CTabDetour()
{
}


BEGIN_MESSAGE_MAP(CTabDetour, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabDetour 消息处理程序

int CTabDetour::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabDetour::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_oGridCtrlEdit.RemoveAll();	// 输入行
	m_oGridCtrlList.RemoveAll();	// 列表
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabDetour::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;
	CString strColumn;
	CBCGPGridRow* pRow = NULL;

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + 46;
	m_oGridCtrlEdit.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_EDIT);	// 输入行
	m_oGridCtrlEdit.m_iRowHeightAdd = 5;
	m_oGridCtrlEdit.EnableColumnAutoSize(TRUE);
	m_oGridCtrlEdit.EnableDragHeaderItems(FALSE);
	m_oGridCtrlEdit.EnableHeader(TRUE, 0);
	strColumn = "Low BoxType";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 50);
	strColumn = "Low S.N.";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 50);
	strColumn = "Low ChannelNb";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 50);
	strColumn = "High BoxType";
	m_oGridCtrlEdit.InsertColumn (3, strColumn, 50);
	strColumn = "High S.N.";
	m_oGridCtrlEdit.InsertColumn (4, strColumn, 50);
	strColumn = "High ChannelNb";
	m_oGridCtrlEdit.InsertColumn (5, strColumn, 50);
	strColumn = "Stop Marking";
	m_oGridCtrlEdit.InsertColumn (6, strColumn, 50);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	// 初始化BoxType单元格
	InitBoxTypeGridItem(pRow->GetItem(0));
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(3)->SetValue("");
	// 初始化BoxType单元格
	InitBoxTypeGridItem(pRow->GetItem(3));
	pRow->GetItem(4)->SetValue("");
	pRow->GetItem(5)->SetValue("");
	pRow->ReplaceItem (6, new CBCGPGridCheckItem (FALSE));
	m_oGridCtrlEdit.AddRow(pRow, FALSE);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 60;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 45;
	m_oGridCtrlList.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_LIST);	// 列表
	m_oGridCtrlList.m_iRowHeightAdd = 5;
	m_oGridCtrlList.ShowVertScrollBar();
	m_oGridCtrlList.EnableColumnAutoSize(TRUE);
	m_oGridCtrlList.EnableDragHeaderItems(FALSE);
	m_oGridCtrlList.EnableHeader(FALSE, 0);
	m_oGridCtrlList.SetReadOnly(TRUE);
	m_oGridCtrlList.SetWholeRowSel(TRUE);
	m_oGridCtrlList.SetSingleSel(TRUE);
	strColumn = "Low BoxType";
	m_oGridCtrlList.InsertColumn (0, strColumn, 50);
	strColumn = "Low S.N.";
	m_oGridCtrlList.InsertColumn (1, strColumn, 50);
	strColumn = "Low ChannelNb";
	m_oGridCtrlList.InsertColumn (2, strColumn, 50);
	strColumn = "High BoxType";
	m_oGridCtrlList.InsertColumn (3, strColumn, 50);
	strColumn = "High S.N.";
	m_oGridCtrlList.InsertColumn (4, strColumn, 50);
	strColumn = "High ChannelNb";
	m_oGridCtrlList.InsertColumn (5, strColumn, 50);
	strColumn = "Stop Marking";
	m_oGridCtrlList.InsertColumn (6, strColumn, 50);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabDetour::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabDetour::OnGridCtrlListDblClk()
{

}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabDetour::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CDetourData oDetourData;
	CString strData;

	m_pSiteData->m_oDetourList.m_olsDetour.RemoveAll();
	m_pSiteData->m_oDetourList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);

		strData = pRow->GetItem(0)->GetValue();
		// 得到BoxType
		oDetourData.m_uiBoxTypeLow = m_pSiteData->GetBoxTypeIDByString(strData);

		strData = pRow->GetItem(1)->GetValue();
		oDetourData.m_uiSNLow = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(2)->GetValue();
		oDetourData.m_uiChannelNbLow = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(3)->GetValue();
		// 得到BoxType
		oDetourData.m_uiBoxTypeHigh = m_pSiteData->GetBoxTypeIDByString(strData);

		strData = pRow->GetItem(4)->GetValue();
		oDetourData.m_uiSNHigh = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(5)->GetValue();
		oDetourData.m_uiChannelNbHigh = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		COleVariant oVariant = pRow->GetItem(6)->GetValue();
		oVariant.ChangeType(VT_UINT);
		if(0 == oVariant.uintVal)
		{
			oDetourData.m_uiStopMarking = 0;
		}
		else
		{
			oDetourData.m_uiStopMarking = 1;
		}
		m_pSiteData->m_oDetourList.m_olsDetour.AddTail(oDetourData);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabDetour::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CDetourData oDetourData;
	COleVariant oVariant;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(3)->SetValue("");
	pRow->GetItem(4)->SetValue("");
	pRow->GetItem(5)->SetValue("");
	oVariant = "FALSE";
	oVariant.ChangeType(VT_BOOL);
	pRow->GetItem(6)->SetValue(oVariant);

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oDetourList.m_olsDetour.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oDetourList.m_olsDetour.FindIndex(i);
		oDetourData = m_pSiteData->m_oDetourList.m_olsDetour.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());

		// 得到BoxType字符串
		CString strBoxType = m_pSiteData->GetBoxTypeStringByID(oDetourData.m_uiBoxTypeLow);
		oVariant = strBoxType;
		pRow->GetItem(0)->SetValue(oVariant);
		CString strSNLow;
		strSNLow.Format("%d", oDetourData.m_uiSNLow);
		oVariant = strSNLow;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(1)->SetValue(oVariant);
		oVariant = (long)oDetourData.m_uiChannelNbLow;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(2)->SetValue(oVariant);

		strBoxType = m_pSiteData->GetBoxTypeStringByID(oDetourData.m_uiBoxTypeHigh);
		oVariant = strBoxType;
		pRow->GetItem(3)->SetValue(oVariant);
		CString strSNHigh;
		strSNHigh.Format("%d", oDetourData.m_uiSNHigh);
		oVariant = strSNHigh;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(4)->SetValue(oVariant);
		oVariant = (long)oDetourData.m_uiChannelNbHigh;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(5)->SetValue(oVariant);

		if(0 == oDetourData.m_uiStopMarking)
		{
			oVariant = "FALSE";
		}
		if(1 == oDetourData.m_uiStopMarking)
		{
			oVariant = "TRUE";
		}
		oVariant.ChangeType(VT_BOOL);
		pRow->GetItem(6)->SetValue(oVariant);

		m_oGridCtrlList.AddRow(pRow, FALSE);
	}
	m_oGridCtrlEdit.AdjustLayout();
	m_oGridCtrlList.AdjustLayout();
}

/**
* 响应鼠标单击按钮；Add
* @param void
* @return void
*/
void CTabDetour::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddB(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabDetour::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeB(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabDetour::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteB(&m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Clear
* @param void
* @return void
*/
void CTabDetour::OnBnClickedButtonClear()
{
	OnBnClickedButtonClearA(&m_oGridCtrlList);
}

/**
* 初始化BoxType单元格
* @param CBCGPGridItem* pItem BoxType单元格
* @return void
*/
void CTabDetour::InitBoxTypeGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strBoxType;
	for(int i = 0; i < m_pSiteData->m_olsBoxType.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsBoxType.FindIndex(i);
		strBoxType = m_pSiteData->m_olsBoxType.GetAt(pos);
		pItem->AddOption(strBoxType);
	}
}

/**
* 响应仪器图形视图弹出菜单设置迂回道
* @param unsigned int uiInstrumentType 低端仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 低端仪器设备号
* @param unsigned int uiInstrumentType 高端仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 高端仪器设备号
* @return void
*/
void CTabDetour::OnGraphViewInstrumentSetDetour(unsigned int uiInstrumentTypeLow, unsigned int uiSNLow, unsigned int uiInstrumentTypeHigh, unsigned int uiSNHight)
{
	CBCGPGridRow* pRow = NULL;
	COleVariant oVariant;
	pRow = m_oGridCtrlEdit.GetRow(0);
	
	// 得到BoxType字符串
	CString strBoxType = m_pSiteData->GetBoxTypeStringByID(uiInstrumentTypeLow);
	oVariant = strBoxType;
	pRow->GetItem(0)->SetValue(oVariant);
	CString strSNLow;
	strSNLow.Format("%d", uiSNLow);
	oVariant = strSNLow;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem(1)->SetValue(oVariant);
	oVariant = (long)1;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem(2)->SetValue(oVariant);

	strBoxType = m_pSiteData->GetBoxTypeStringByID(uiInstrumentTypeHigh);
	oVariant = strBoxType;
	pRow->GetItem(3)->SetValue(oVariant);
	CString strSNHigh;
	strSNHigh.Format("%d", uiSNHight);
	oVariant = strSNHigh;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem(4)->SetValue(oVariant);
	oVariant = (long)1;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem(5)->SetValue(oVariant);
}