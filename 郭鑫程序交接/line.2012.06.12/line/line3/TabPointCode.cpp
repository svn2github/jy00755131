// TabSurvey.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "TabPointCode.h"


// CTabPointCode

IMPLEMENT_DYNAMIC(CTabPointCode, CTabPage)

CTabPointCode::CTabPointCode()
{

}

CTabPointCode::~CTabPointCode()
{
}


BEGIN_MESSAGE_MAP(CTabPointCode, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabPointCode 消息处理程序

int CTabPointCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabPointCode::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_oGridCtrlEdit.RemoveAll();	// 点代码
	m_oGridCtrlList.RemoveAll();	// 点代码列表
	m_olsNb.RemoveAll();	// 点代码号队列
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabPointCode::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;
	CString strColumn;
	CBCGPGridRow* pRow = NULL;

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + 46;
	m_oGridCtrlEdit.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_EDIT);	// 点代码
	m_oGridCtrlEdit.m_iRowHeightAdd = 5;
	m_oGridCtrlEdit.EnableColumnAutoSize(TRUE);
	m_oGridCtrlEdit.EnableDragHeaderItems(FALSE);
	m_oGridCtrlEdit.EnableHeader(TRUE, 0);
	strColumn = "Nb";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 150);
	strColumn = "Label";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 150);
	strColumn = "Sensor Type";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 150);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	m_oGridCtrlEdit.AddRow(pRow, FALSE);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 60;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 45;
	m_oGridCtrlList.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_LIST);	// 点代码列表
	m_oGridCtrlList.m_iRowHeightAdd = 5;
	m_oGridCtrlList.ShowVertScrollBar();
	m_oGridCtrlList.EnableColumnAutoSize(TRUE);
	m_oGridCtrlList.EnableDragHeaderItems(FALSE);
	m_oGridCtrlList.EnableHeader(FALSE, 0);
	m_oGridCtrlList.SetReadOnly(TRUE);
	m_oGridCtrlList.SetWholeRowSel(TRUE);
	m_oGridCtrlList.SetSingleSel(TRUE);
	strColumn = "Nb";
	m_oGridCtrlList.InsertColumn (0, strColumn, 150);
	strColumn = "Label";
	m_oGridCtrlList.InsertColumn (1, strColumn, 150);
	strColumn = "Sensor Type";
	m_oGridCtrlList.InsertColumn (2, strColumn, 150);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabPointCode::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabPointCode::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；ApplyAll
* @param void
* @return void
*/
void CTabPointCode::OnBnClickedButtonApplyAll()
{
	CBCGPGridRow* pRow = NULL;
	CPointCodeData oPointCodeData;

	m_pSiteData->m_oPointCodeList.m_olsPointCode.RemoveAll();
	m_pSiteData->m_oPointCodeList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);
		oPointCodeData.m_uiNb = pRow->GetData();
		oPointCodeData.m_strLabel = pRow->GetItem(1)->GetValue();
		oPointCodeData.m_strSensorType = pRow->GetItem(2)->GetValue();
		m_pSiteData->m_oPointCodeList.m_olsPointCode.AddTail(oPointCodeData);
	}
}

/**
* 响应鼠标单击按钮；ApplySensor
* @param void
* @return void
*/
void CTabPointCode::OnBnClickedButtonApplySensor()
{

}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabPointCode::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CPointCodeData oPointCodeData;
	COleVariant oVariant;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oPointCodeList.m_olsPointCode.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oPointCodeList.m_olsPointCode.FindIndex(i);
		oPointCodeData = m_pSiteData->m_oPointCodeList.m_olsPointCode.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(oPointCodeData.m_uiNb);
		pRow->GetItem(0)->SetValue((int)oPointCodeData.m_uiNb);
		oVariant = oPointCodeData.m_strLabel;
		pRow->GetItem(1)->SetValue(oVariant);
		oVariant = oPointCodeData.m_strSensorType;
		pRow->GetItem(2)->SetValue(oVariant);
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
void CTabPointCode::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabPointCode::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabPointCode::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Reverse
* @param void
* @return void
*/
void CTabPointCode::OnBnClickedButtonReverse()
{
	OnBnClickedButtonReverseA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}




