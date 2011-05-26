// TabMute.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabMute.h"


// CTabMute

IMPLEMENT_DYNAMIC(CTabMute, CTabPage)

CTabMute::CTabMute()
{
}

CTabMute::~CTabMute()
{
}


BEGIN_MESSAGE_MAP(CTabMute, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabMute 消息处理程序

int CTabMute::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabMute::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_oGridCtrlEdit.RemoveAll();	// 编辑行
	m_oGridCtrlList.RemoveAll();	// 列表
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabMute::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;
	CString strColumn;
	CBCGPGridRow* pRow = NULL;

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + 46;
	m_oGridCtrlEdit.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_EDIT);	// 编辑行
	m_oGridCtrlEdit.m_iRowHeightAdd = 5;
	m_oGridCtrlEdit.EnableColumnAutoSize(TRUE);
	m_oGridCtrlEdit.EnableDragHeaderItems(FALSE);
	m_oGridCtrlEdit.EnableHeader(TRUE, 0);
	strColumn = "Line Name";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 150);
	strColumn = "Point Nb";
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
	strColumn = "Line Name";
	m_oGridCtrlList.InsertColumn (0, strColumn, 150);
	strColumn = "Point Nb";
	m_oGridCtrlList.InsertColumn (1, strColumn, 300);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabMute::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabMute::OnGridCtrlListDblClk()
{

}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabMute::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CMuteData oMuteData;
	CString strData;

	m_pSiteData->m_oMuteList.m_olsMute.RemoveAll();
	m_pSiteData->m_oMuteList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);

		strData = pRow->GetItem(0)->GetValue();
		oMuteData.m_uiLineName = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(1)->GetValue();
		oMuteData.m_uiPointNb = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		m_pSiteData->m_oMuteList.m_olsMute.AddTail(oMuteData);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabMute::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CMuteData oMuteData;
	COleVariant oVariant;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oMuteList.m_olsMute.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oMuteList.m_olsMute.FindIndex(i);
		oMuteData = m_pSiteData->m_oMuteList.m_olsMute.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());

		oVariant = (long)oMuteData.m_uiLineName;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(0)->SetValue(oVariant);

		oVariant = (long)oMuteData.m_uiPointNb;
		oVariant.ChangeType(VT_BSTR);
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
void CTabMute::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddB(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabMute::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeB(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabMute::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteB(&m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Clear
* @param void
* @return void
*/
void CTabMute::OnBnClickedButtonClear()
{
	OnBnClickedButtonClearA(&m_oGridCtrlList);
}

/**
* 响应检波器图形视图弹出菜单Mute设置
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint	测点号
* @return void
*/
void CTabMute::OnGraphViewSensorSetMute(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	CBCGPGridRow* pRow = NULL;
	COleVariant oVariant;
	pRow = m_oGridCtrlEdit.GetRow(0);

	oVariant = (long)uiNbLine;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem(0)->SetValue(oVariant);

	oVariant = (long)uiNbPoint;
	oVariant.ChangeType(VT_BSTR);
	pRow->GetItem(1)->SetValue(oVariant);
}