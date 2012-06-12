// TabAux.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "TabAux.h"


// CTabAux

IMPLEMENT_DYNAMIC(CTabAux, CTabPage)

CTabAux::CTabAux()
{
}

CTabAux::~CTabAux()
{
}


BEGIN_MESSAGE_MAP(CTabAux, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabAux 消息处理程序

int CTabAux::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabAux::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_oGridCtrlEdit.RemoveAll();	// 编辑行
	m_oGridCtrlList.RemoveAll();	// 列表
	m_olsNb.RemoveAll();	// 索引号队列
}

/**
* 生成编辑界面
* @param LPCREATESTRUCT lpCreateStruct
* @return void
*/
void CTabAux::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
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
	strColumn = "Label";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 50);
	strColumn = "BoxType";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 50);
	strColumn = "S.N.";
	m_oGridCtrlEdit.InsertColumn (3, strColumn, 50);
	strColumn = "ChannelNb";
	m_oGridCtrlEdit.InsertColumn (4, strColumn, 50);
	strColumn = "Gain(mV)";
	m_oGridCtrlEdit.InsertColumn (5, strColumn, 50);
	strColumn = "DPG Nb";
	m_oGridCtrlEdit.InsertColumn (6, strColumn, 50);
	strColumn = "Comments";
	m_oGridCtrlEdit.InsertColumn (7, strColumn, 150);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	// 初始化BoxType单元格
	InitBoxTypeGridItem(pRow->GetItem(2));
	pRow->GetItem(3)->SetValue("");
	pRow->GetItem(4)->SetValue("");
	pRow->GetItem(5)->SetValue("");
	// 初始化Gain单元格
	InitGainGridItem(pRow->GetItem(5));
	pRow->GetItem(6)->SetValue("");
	pRow->GetItem(7)->SetValue("");
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
	strColumn = "Nb";
	m_oGridCtrlList.InsertColumn (0, strColumn, 50);
	strColumn = "Label";
	m_oGridCtrlList.InsertColumn (1, strColumn, 50);
	strColumn = "BoxType";
	m_oGridCtrlList.InsertColumn (2, strColumn, 50);
	strColumn = "S.N.";
	m_oGridCtrlList.InsertColumn (3, strColumn, 50);
	strColumn = "ChannelNb";
	m_oGridCtrlList.InsertColumn (4, strColumn, 50);
	strColumn = "Gain(mV)";
	m_oGridCtrlList.InsertColumn (5, strColumn, 50);
	strColumn = "DPG Nb";
	m_oGridCtrlList.InsertColumn (6, strColumn, 50);
	strColumn = "Comments";
	m_oGridCtrlList.InsertColumn (7, strColumn, 150);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabAux::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabAux::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabAux::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CAuxData oAuxData;
	CString strData;

	m_pSiteData->m_oAuxList.m_olsAux.RemoveAll();
	m_pSiteData->m_oAuxList.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);
		oAuxData.m_uiNb = pRow->GetData();
		
		strData = pRow->GetItem(1)->GetValue();
		oAuxData.m_strLabel = strData;

		strData = pRow->GetItem(2)->GetValue();
		oAuxData.m_uiBoxType = m_pSiteData->GetBoxTypeIDByString(strData);

		strData = pRow->GetItem(3)->GetValue();
		oAuxData.m_uiSN = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(4)->GetValue();
		oAuxData.m_uiChannelNb = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(5)->GetValue();
		oAuxData.m_uiGain = m_pSiteData->GetGainByString(strData);

		strData = pRow->GetItem(6)->GetValue();
		oAuxData.m_uiDPGNb = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(7)->GetValue();
		oAuxData.m_strComments = strData;

		m_pSiteData->m_oAuxList.m_olsAux.AddTail(oAuxData);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabAux::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CAuxData oAuxData;
	COleVariant oVariant;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(3)->SetValue("");
	pRow->GetItem(4)->SetValue("");
	pRow->GetItem(5)->SetValue("");
	pRow->GetItem(6)->SetValue("");
	pRow->GetItem(7)->SetValue("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oAuxList.m_olsAux.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oAuxList.m_olsAux.FindIndex(i);
		oAuxData = m_pSiteData->m_oAuxList.m_olsAux.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(oAuxData.m_uiNb);
		pRow->GetItem(0)->SetValue((int)oAuxData.m_uiNb);

		oVariant = oAuxData.m_strLabel;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(1)->SetValue(oVariant);

		oVariant = m_pSiteData->GetBoxTypeStringByID(oAuxData.m_uiBoxType);
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(2)->SetValue(oVariant);

		CString strSN;
		strSN.Format("%d", oAuxData.m_uiSN);
		oVariant = strSN;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(3)->SetValue(oVariant);

		oVariant = (long)oAuxData.m_uiChannelNb;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(4)->SetValue(oVariant);

		oVariant = m_pSiteData->GetGainStringByGainData(oAuxData.m_uiGain);
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(5)->SetValue(oVariant);

		oVariant = (long)oAuxData.m_uiDPGNb;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(6)->SetValue(oVariant);

		oVariant = oAuxData.m_strComments;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(7)->SetValue(oVariant);

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
void CTabAux::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabAux::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabAux::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Clear
* @param void
* @return void
*/
void CTabAux::OnBnClickedButtonClear()
{
	OnBnClickedButtonClearA(&m_oGridCtrlList);
}

/**
* 初始化BoxType单元格
* @param CBCGPGridItem* pItem BoxType单元格
* @return void
*/
void CTabAux::InitBoxTypeGridItem(CBCGPGridItem* pItem)
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
* 初始化Gain单元格
* @param CBCGPGridItem* pItem Gain单元格
* @return void
*/
void CTabAux::InitGainGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strGain;
	for(int i = 0; i < m_pSiteData->m_olsGain.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsGain.FindIndex(i);
		strGain = m_pSiteData->m_olsGain.GetAt(pos);
		pItem->AddOption(strGain);
	}
}
/**
* 响应仪器图形视图弹出菜单设置辅助道
* @param unsigned int uiInstrumentType 仪器类型 1-交叉站；2-电源站；3-采集站
* @param unsigned int uiSN 仪器设备号
* @return void
*/
void CTabAux::OnGraphViewInstrumentSetAuxiliary(unsigned int uiInstrumentType, unsigned int uiSN)
{
	CBCGPGridRow* pRow = NULL;
	pRow = m_oGridCtrlEdit.GetRow(0);


	// 得到BoxType字符串
	CString strBoxType = m_pSiteData->GetBoxTypeStringByID(uiInstrumentType);
	COleVariant oVariant = strBoxType;
	pRow->GetItem(2)->SetValue(oVariant);
	CString strSN;
	strSN.Format("%d", uiSN);
	oVariant = strSN;
	pRow->GetItem(3)->SetValue(oVariant);
}