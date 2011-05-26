// TabSpreadGeneric.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabSpreadGeneric.h"


// CTabSpreadGeneric

IMPLEMENT_DYNAMIC(CTabSpreadGeneric, CTabPage)

CTabSpreadGeneric::CTabSpreadGeneric()
{
}

CTabSpreadGeneric::~CTabSpreadGeneric()
{
}


BEGIN_MESSAGE_MAP(CTabSpreadGeneric, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabSpreadGeneric 消息处理程序

int CTabSpreadGeneric::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabSpreadGeneric::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_oGridCtrlEdit.RemoveAll();	// 输入行
	m_oGridCtrlList.RemoveAll();	// 列表
	m_olsNb.RemoveAll();	// 索引号队列
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabSpreadGeneric::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;
	CString strColumn;
	CBCGPGridRow* pRow = NULL;

	oRect.left = lpCreateStruct->x;
	oRect.top = 160;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = oRect.top + 46;
	m_oGridCtrlEdit.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_EDIT);	// 检波器
	m_oGridCtrlEdit.m_iRowHeightAdd = 5;
	m_oGridCtrlEdit.EnableColumnAutoSize(TRUE);
	m_oGridCtrlEdit.EnableDragHeaderItems(FALSE);
	m_oGridCtrlEdit.EnableHeader(TRUE, 0);
	strColumn = "Nb";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 50);
	strColumn = "Label";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 80);
	strColumn = "Line";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 100);
	strColumn = "Spread";
	m_oGridCtrlEdit.InsertColumn (3, strColumn, 100);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetBackgroundColor(RGB(192, 192, 192));
	pRow->GetItem(3)->SetBackgroundColor(RGB(192, 192, 192));
	m_oGridCtrlEdit.AddRow(pRow, FALSE);

	oRect.top = oRect.bottom + 14;
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
	strColumn = "Nb";
	m_oGridCtrlList.InsertColumn (0, strColumn, 50);
	strColumn = "Label";
	m_oGridCtrlList.InsertColumn (1, strColumn, 80);
	strColumn = "Line";
	m_oGridCtrlList.InsertColumn (2, strColumn, 100);
	strColumn = "pread";
	m_oGridCtrlList.InsertColumn (3, strColumn, 100);

	oRect.top = lpCreateStruct->y;
	oRect.bottom = 150;
	m_oTabWnd.Create(CBCGPTabWnd::STYLE_3D, oRect, this, 1, CBCGPTabWnd::LOCATION_TOP);	// 属性页容器

	m_oEditLine.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		oRect, &m_oTabWnd, 11);	// 测线编辑框
	m_oEditSpread.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		oRect, &m_oTabWnd, 12);	// 排列编辑框
	m_oTabWnd.AddTab(&m_oEditLine, "Line");	// 属性页Line
	m_oTabWnd.AddTab(&m_oEditSpread, "Spread");	// 属性页Spread
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabSpreadGeneric::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabSpreadGeneric::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);

	CBCGPGridRow* pRow = m_oGridCtrlList.GetCurSel();
	if(NULL != pRow)
	{
		COleVariant oVariantLine = pRow->GetItem(2)->GetValue();
		oVariantLine.ChangeType(VT_BSTR);
		CString strLine = oVariantLine;
		strLine.Replace(";", "\r\n");
		m_oEditLine.SetWindowText(strLine);

		COleVariant oVariantSpread = pRow->GetItem(3)->GetValue();
		oVariantSpread.ChangeType(VT_BSTR);
		CString strSpread = oVariantSpread;
		strSpread.Replace(";", "\r\n");
		m_oEditSpread.SetWindowText(strSpread);
	}
}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabSpreadGeneric::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CSpreadGenericData oSpread;
	CString strData;

	m_pSiteData->m_oSpreadGenericList.OnClose();
	m_pSiteData->m_oSpreadGenericList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);

		oSpread.m_uiNb = pRow->GetData();
		strData = pRow->GetItem(1)->GetValue();
		oSpread.m_strLabel = strData;

		strData = pRow->GetItem(2)->GetValue();
		oSpread.m_strLine = strData;

		strData = pRow->GetItem(3)->GetValue();
		oSpread.m_strSpread = strData;

		m_pSiteData->m_oSpreadGenericList.m_olsSpreadGeneric.AddTail(oSpread);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabSpreadGeneric::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos = NULL;
	CSpreadGenericData oSpread;
	COleVariant oVariantBool;
	COleVariant oVariantString;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(3)->SetValue("");

	m_oEditLine.SetWindowText("");
	m_oEditSpread.SetWindowText("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oSpreadGenericList.m_olsSpreadGeneric.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oSpreadGenericList.m_olsSpreadGeneric.FindIndex(i);
		oSpread = m_pSiteData->m_oSpreadGenericList.m_olsSpreadGeneric.GetAt(pos);

		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(oSpread.m_uiNb);

		oVariantString = (long)oSpread.m_uiNb;
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(0)->SetValue(oVariantString);

		oVariantString = oSpread.m_strLabel;
		pRow->GetItem(1)->SetValue(oVariantString);

		oVariantString = oSpread.m_strLine;
		pRow->GetItem(2)->SetValue(oVariantString);

		oVariantString = oSpread.m_strSpread;
		pRow->GetItem(3)->SetValue(oVariantString);

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
void CTabSpreadGeneric::OnBnClickedButtonAdd()
{
	CString strLine;
	m_oEditLine.GetWindowText(strLine);
	strLine.Replace("\r\n", ";");
	COleVariant oVariantStringLine= strLine;
	m_oGridCtrlEdit.GetRow(0)->GetItem(2)->SetValue(oVariantStringLine);

	CString strSpread;
	m_oEditSpread.GetWindowText(strSpread);
	strSpread.Replace("\r\n", ";");
	COleVariant oVariantStringSpread = strSpread;
	m_oGridCtrlEdit.GetRow(0)->GetItem(3)->SetValue(oVariantStringSpread);

	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabSpreadGeneric::OnBnClickedButtonChange()
{
	CString strLine;
	m_oEditLine.GetWindowText(strLine);
	strLine.Replace("\r\n", ";");
	COleVariant oVariantStringLine = strLine;
	m_oGridCtrlEdit.GetRow(0)->GetItem(2)->SetValue(oVariantStringLine);

	CString strSpread;
	m_oEditSpread.GetWindowText(strSpread);
	strSpread.Replace("\r\n", ";");
	COleVariant oVariantStringSpread = strSpread;
	m_oGridCtrlEdit.GetRow(0)->GetItem(3)->SetValue(oVariantStringSpread);

	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabSpreadGeneric::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}