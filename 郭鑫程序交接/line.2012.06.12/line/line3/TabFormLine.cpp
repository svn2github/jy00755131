// TabFormLine.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "TabFormLine.h"


// CTabFormLine

IMPLEMENT_DYNAMIC(CTabFormLine, CTabPage)

CTabFormLine::CTabFormLine()
{
}

CTabFormLine::~CTabFormLine()
{
}


BEGIN_MESSAGE_MAP(CTabFormLine, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabFormLine 消息处理程序

int CTabFormLine::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabFormLine::OnDestroy()
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
void CTabFormLine::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
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
	strColumn = "Nb";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 50);
	strColumn = "BoxType";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 50);
	strColumn = "Serial Nb";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 50);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	// 初始化BoxType单元格
	InitBoxTypeGridItem(pRow->GetItem(1));
	pRow->GetItem(2)->SetValue("");
	m_oGridCtrlEdit.AddRow(pRow, FALSE);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 60;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 25;
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
	strColumn = "BoxType";
	m_oGridCtrlList.InsertColumn (1, strColumn, 50);
	strColumn = "Serial Nb";
	m_oGridCtrlList.InsertColumn (2, strColumn, 50);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabFormLine::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabFormLine::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabFormLine::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CFormLineData oFormLineData;
	CString strData;

	m_pSiteData->m_oFormLineList.m_olsFormLine.RemoveAll();
	m_pSiteData->m_oFormLineList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);
		oFormLineData.m_uiNb = pRow->GetData();
		
		strData = pRow->GetItem(1)->GetValue();;
		oFormLineData.m_uiBoxType = m_pSiteData->GetBoxTypeIDByString(strData);

		strData = pRow->GetItem(2)->GetValue();
		oFormLineData.m_uiSN = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		m_pSiteData->m_oFormLineList.m_olsFormLine.AddTail(oFormLineData);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabFormLine::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CFormLineData oFormLineData;
	COleVariant oVariant;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oFormLineList.m_olsFormLine.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oFormLineList.m_olsFormLine.FindIndex(i);
		oFormLineData = m_pSiteData->m_oFormLineList.m_olsFormLine.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(oFormLineData.m_uiNb);
		pRow->GetItem(0)->SetValue((int)oFormLineData.m_uiNb);

		// 得到BoxType字符串
		CString strBoxType = m_pSiteData->GetBoxTypeStringByID(oFormLineData.m_uiBoxType);
		oVariant = strBoxType;
		pRow->GetItem(1)->SetValue(oVariant);

		CString strSN;
		strSN.Format("%d", oFormLineData.m_uiSN);
		oVariant = strSN;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(2)->SetValue(oVariant);

		m_oGridCtrlList.AddRow(pRow, FALSE);
	}
	m_oGridCtrlEdit.AdjustLayout();
	m_oGridCtrlList.AdjustLayout();
}

/**
* 响应鼠标单击按钮；GO
* @param void
* @return bool true：成功；false：失败
*/
bool CTabFormLine::OnBnClickedButtonGO()
{
	CBCGPGridRow* pRow = m_oGridCtrlEdit.GetRow(0);
	CString strData;

	strData = pRow->GetItem(2)->GetValue();
	m_oFormLineData.m_uiSN = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

	return true;
}

/**
* 响应鼠标单击按钮；Add
* @param void
* @return void
*/
void CTabFormLine::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabFormLine::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabFormLine::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 初始化BoxType单元格
* @param CBCGPGridItem* pItem BoxType单元格
* @return void
*/
void CTabFormLine::InitBoxTypeGridItem(CBCGPGridItem* pItem)
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
* 初始化Orientation单元格
* @param CBCGPGridItem* pItem Orientation单元格
* @return void
*/
void CTabFormLine::InitOrientationGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strOrientation;
	for(int i = 0; i < m_pSiteData->m_olsOrientation.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsOrientation.FindIndex(i);
		strOrientation = m_pSiteData->m_olsOrientation.GetAt(pos);
		pItem->AddOption(strOrientation);
	}
}

/**
*  响应仪器图形视图弹出菜单FormLine设置
* @param unsigned int uiInstrumentType 仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 仪器设备号
* @return void
*/
void CTabFormLine::OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN)
{
	CBCGPGridRow* pRow = NULL;
	pRow = m_oGridCtrlEdit.GetRow(0);


	// 得到BoxType字符串
	CString strBoxType = m_pSiteData->GetBoxTypeStringByID(uiInstrumentType);
	COleVariant oVariant = strBoxType;
	pRow->GetItem(1)->SetValue(oVariant);
	CString strSN;
	strSN.Format("%d", uiSN);
	oVariant = strSN;
	pRow->GetItem(2)->SetValue(oVariant);
}