// TabSurvey.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabSurvey.h"


// CTabSurvey

IMPLEMENT_DYNAMIC(CTabSurvey, CTabPage)

CTabSurvey::CTabSurvey()
{

}

CTabSurvey::~CTabSurvey()
{
}


BEGIN_MESSAGE_MAP(CTabSurvey, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabSurvey 消息处理程序

int CTabSurvey::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabSurvey::OnDestroy()
{
	CWnd::OnDestroy();

	m_oGridCtrlEdit.RemoveAll();	// 测线
	m_oGridCtrlList.RemoveAll();	// 测线列表
	m_olsNb.RemoveAll();	// 测线号队列
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabSurvey::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;
	CString strColumn;
	CBCGPGridRow* pRow = NULL;

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + 46;
	m_oGridCtrlEdit.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_EDIT);	// 编辑框
	m_oGridCtrlEdit.m_iRowHeightAdd = 5;
	m_oGridCtrlEdit.EnableColumnAutoSize(TRUE);
	m_oGridCtrlEdit.EnableDragHeaderItems(FALSE);
	m_oGridCtrlEdit.EnableHeader(TRUE, 0);
	strColumn = "Line";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 150);
	strColumn = "Receiver Section";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 300);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
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
	strColumn = "Line";
	m_oGridCtrlList.InsertColumn (0, strColumn, 150);
	strColumn = "Receiver Section";
	m_oGridCtrlList.InsertColumn (1, strColumn, 300);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabSurvey::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabSurvey::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；ApplyAll
* @param void
* @return void
*/
void CTabSurvey::OnBnClickedButtonApplyAll()
{
	CBCGPGridRow* pRow = NULL;
	CLineSetupData oLineSetupData;

	m_pSiteData->m_oLineList.m_olsLine.RemoveAll();
	m_pSiteData->m_oLineList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);
		oLineSetupData.m_uiNbLine = pRow->GetData();
		oLineSetupData.m_strReceiverSection = pRow->GetItem(1)->GetValue();
		m_pSiteData->m_oLineList.m_olsLine.AddTail(oLineSetupData);
	}
}

/**
* 响应鼠标单击按钮；ApplySensor
* @param void
* @return void
*/
void CTabSurvey::OnBnClickedButtonApplySensor()
{

}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabSurvey::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CLineSetupData oLineSetupData;
	COleVariant oVariant;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oLineList.m_olsLine.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oLineList.m_olsLine.FindIndex(i);
		oLineSetupData = m_pSiteData->m_oLineList.m_olsLine.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(oLineSetupData.m_uiNbLine);
		pRow->GetItem(0)->SetValue((int)oLineSetupData.m_uiNbLine);
		oVariant = oLineSetupData.m_strReceiverSection;
		pRow->GetItem(1)->SetValue(oVariant);
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
void CTabSurvey::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabSurvey::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabSurvey::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Reverse
* @param void
* @return void
*/
void CTabSurvey::OnBnClickedButtonReverse()
{
	OnBnClickedButtonReverseA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}