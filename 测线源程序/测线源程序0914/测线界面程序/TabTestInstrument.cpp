// TabTestInstrument.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabTestInstrument.h"


// CTabTestInstrument

IMPLEMENT_DYNAMIC(CTabTestInstrument, CTabPage)

CTabTestInstrument::CTabTestInstrument()
{
}

CTabTestInstrument::~CTabTestInstrument()
{
}


BEGIN_MESSAGE_MAP(CTabTestInstrument, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabTestInstrument 消息处理程序

int CTabTestInstrument::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabTestInstrument::OnDestroy()
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
void CTabTestInstrument::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
{
	CRect oRect;
	CString strColumn;
	CBCGPGridRow* pRow = NULL;

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + 46;
	m_oGridCtrlEdit.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_EDIT);	// 检波器
	m_oGridCtrlEdit.m_iRowHeightAdd = 5;
	m_oGridCtrlEdit.EnableColumnAutoSize(TRUE);
	m_oGridCtrlEdit.EnableDragHeaderItems(FALSE);
	m_oGridCtrlEdit.EnableHeader(TRUE, 0);
	strColumn = "Test Nb";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 30);
	strColumn = "Test Type";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 80);
	strColumn = "Gain";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 30);
	strColumn = "Record Length";
	m_oGridCtrlEdit.InsertColumn (3, strColumn, 50);
	strColumn = "Recorded";
	m_oGridCtrlEdit.InsertColumn (4, strColumn, 30);
	strColumn = "Auxiliary Descr";
	m_oGridCtrlEdit.InsertColumn (5, strColumn, 50);
	strColumn = "Absolute Spread";
	m_oGridCtrlEdit.InsertColumn (6, strColumn, 50);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	// 初始化TestType队列
	InitTestTypeGridItem(pRow->GetItem(1));
	// 初始化Gain单元格
	InitGainGridItem(pRow->GetItem(2));
	pRow->GetItem(3)->SetValue("");
	pRow->ReplaceItem (4, new CBCGPGridCheckItem (FALSE));
	pRow->GetItem(5)->SetValue("");
	pRow->GetItem(5)->AllowEdit(FALSE);
	pRow->GetItem(5)->SetBackgroundColor(RGB(192, 192, 192));
	pRow->GetItem(6)->SetValue("");
	pRow->GetItem(6)->AllowEdit(FALSE);
	pRow->GetItem(6)->SetBackgroundColor(RGB(192, 192, 192));
	m_oGridCtrlEdit.AddRow(pRow, FALSE);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 60;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 250;
	m_oGridCtrlList.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_LIST);	// 检波器列表
	m_oGridCtrlList.m_iRowHeightAdd = 5;
	m_oGridCtrlList.ShowVertScrollBar();
	m_oGridCtrlList.EnableColumnAutoSize(TRUE);
	m_oGridCtrlList.EnableDragHeaderItems(FALSE);
	m_oGridCtrlList.EnableHeader(FALSE, 0);
	m_oGridCtrlList.SetReadOnly(TRUE);
	m_oGridCtrlList.SetWholeRowSel(TRUE);
	m_oGridCtrlList.SetSingleSel(TRUE);
	strColumn = "Test Nb";
	m_oGridCtrlList.InsertColumn (0, strColumn, 30);
	strColumn = "Test Type";
	m_oGridCtrlList.InsertColumn (1, strColumn, 80);
	strColumn = "Gain";
	m_oGridCtrlList.InsertColumn (2, strColumn, 30);
	strColumn = "Record Length";
	m_oGridCtrlList.InsertColumn (3, strColumn, 50);
	strColumn = "Recorded";
	m_oGridCtrlList.InsertColumn (4, strColumn, 30);
	strColumn = "Auxiliary Descr";
	m_oGridCtrlList.InsertColumn (5, strColumn, 50);
	strColumn = "Absolute Spread";
	m_oGridCtrlList.InsertColumn (6, strColumn, 50);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->cy - 220;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx / 2 - 10;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 45;	
	m_oStaticAuxiliaryDescr.Create("Auxiliary Descr:", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX | WS_DISABLED, oRect, this, 0);	// 辅助道组框

	oRect.left = oRect.left + 10;
	oRect.top = oRect.top + 25;
	oRect.right = oRect.right - 10;
	oRect.bottom = oRect.bottom - 10;
	m_oEditAuxiliaryDescr.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		oRect, this, IDC_EDIT_AUXILIARYDESCR);	// 辅助道编辑框

	oRect.left = lpCreateStruct->x + lpCreateStruct->cx / 2 + 10;
	oRect.top = lpCreateStruct->cy - 220;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 45;	
	m_oStaticAbsoluteSpread.Create("Absolute Spread:", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX | WS_DISABLED, oRect, this, 0);	// 绝对排列组框
	oRect.left = oRect.left + 10;
	oRect.top = oRect.top + 25;
	oRect.right = oRect.right - 10;
	oRect.bottom = oRect.bottom - 10;
	m_oEditAbsoluteSpread.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		oRect, this, IDC_EDIT_ABSOLUTESPREAD);	// 绝对排列编辑框
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabTestInstrument::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabTestInstrument::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);

	CBCGPGridRow* pRow = m_oGridCtrlList.GetCurSel();
	if(NULL != pRow)
	{
		COleVariant oVariant = pRow->GetItem(5)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		CString strAuxiliaryDescr = oVariant;
		GetDlgItem(IDC_EDIT_AUXILIARYDESCR)->SetWindowText(strAuxiliaryDescr);

		oVariant = pRow->GetItem(6)->GetValue();
		oVariant.ChangeType(VT_BSTR);
		CString AbsoluteSpread = oVariant;
		AbsoluteSpread.Replace(";", "\r\n");
		GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->SetWindowText(AbsoluteSpread);
	}
}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabTestInstrument::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CTestProject* pTestProject = NULL;
	CString strData;

	m_pSiteData->m_oTestProjectListInstrument.OnClose();
	m_pSiteData->m_oTestProjectListInstrument.m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);

		pTestProject = new CTestProject;
		pTestProject->m_uiProjectType = 1;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		pTestProject->m_uiTestNb = pRow->GetData();

		strData = pRow->GetItem(3)->GetValue();
		pTestProject->m_uiRecordLength = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		COleVariant oVariant = pRow->GetItem(4)->GetValue();
		oVariant.ChangeType(VT_UINT);
		if(0 == oVariant.uintVal)
		{
			pTestProject->m_uiRecordResult = 0;
		}
		else
		{
			pTestProject->m_uiRecordResult = 1;
		}

		strData = pRow->GetItem(5)->GetValue();
		pTestProject->m_strAuxiliaryDescr = strData;

		strData = pRow->GetItem(6)->GetValue();
		pTestProject->m_strAbsoluteSpread = strData;

		CTestRequest oTestRequest;
		strData = pRow->GetItem(1)->GetValue();
		oTestRequest.m_uiType = m_pSiteData->GetTestTypeIDByString(strData);
		strData = pRow->GetItem(2)->GetValue();
		oTestRequest.m_uiGain = m_pSiteData->GetGainByString(strData);

		pTestProject->m_olsTestRequest.AddTail(oTestRequest);
		pTestProject->m_uiCountAll = 1;

		m_pSiteData->m_oTestProjectListInstrument.m_olsTestProject.AddTail(pTestProject);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabTestInstrument::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos = NULL;
	CTestProject* pTestProject = NULL;
	CTestRequest oTestRequest;
	COleVariant oVariantBool;
	COleVariant oVariantString;

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(3)->SetValue("");
	oVariantBool = "FALSE";
	oVariantBool.ChangeType(VT_BOOL);
	pRow->GetItem(4)->SetValue(oVariantBool);
	pRow->GetItem(5)->SetValue("");
	pRow->GetItem(6)->SetValue("");

	GetDlgItem(IDC_EDIT_AUXILIARYDESCR)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->SetWindowText("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = m_pSiteData->m_oTestProjectListInstrument.m_olsTestProject.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = m_pSiteData->m_oTestProjectListInstrument.m_olsTestProject.FindIndex(i);
		pTestProject = m_pSiteData->m_oTestProjectListInstrument.m_olsTestProject.GetAt(pos);

		pos = pTestProject->m_olsTestRequest.FindIndex(0);
		oTestRequest = pTestProject->m_olsTestRequest.GetAt(pos);

		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(pTestProject->m_uiTestNb);

		oVariantString = (long)pTestProject->m_uiTestNb;
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(0)->SetValue(oVariantString);

		oVariantString = m_pSiteData->GetTestTypeStringByID(oTestRequest.m_uiType);
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(1)->SetValue(oVariantString);

		oVariantString = m_pSiteData->GetGainStringByGainData(oTestRequest.m_uiGain);
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(2)->SetValue(oVariantString);

		oVariantString = (long)pTestProject->m_uiRecordLength;
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(3)->SetValue(oVariantString);

		if(0 == pTestProject->m_uiRecordResult)
		{
			oVariantBool = "FALSE";
		}
		if(1 == pTestProject->m_uiRecordResult)
		{
			oVariantBool = "TRUE";
		}
		oVariantBool.ChangeType(VT_BOOL);
		pRow->GetItem(4)->SetValue(oVariantBool);

		oVariantString = pTestProject->m_strAuxiliaryDescr;
		pRow->GetItem(5)->SetValue(oVariantString);

		oVariantString = pTestProject->m_strAbsoluteSpread;
		pRow->GetItem(6)->SetValue(oVariantString);

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
bool CTabTestInstrument::OnBnClickedButtonGO()
{
	CBCGPGridRow* pRow = m_oGridCtrlList.GetCurSel();
	if(NULL != pRow)
	{
		CString strData;
		m_uiTestNb = pRow->GetData();
		return true;
	}
	AfxMessageBox("Please Select A Record.", MB_OK | MB_ICONSTOP);
	return false;
}

/**
* 响应鼠标单击按钮；Add
* @param void
* @return void
*/
void CTabTestInstrument::OnBnClickedButtonAdd()
{
	CString strAuxiliaryDescr;
	GetDlgItem(IDC_EDIT_AUXILIARYDESCR)->GetWindowText(strAuxiliaryDescr);
	COleVariant oVariantString = strAuxiliaryDescr;
	m_oGridCtrlEdit.GetRow(0)->GetItem(5)->SetValue(oVariantString);

	CString strAbsoluteSpread;
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->GetWindowText(strAbsoluteSpread);
	strAbsoluteSpread.Replace("\r\n", ";");
	oVariantString = strAbsoluteSpread;
	m_oGridCtrlEdit.GetRow(0)->GetItem(6)->SetValue(oVariantString);

	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabTestInstrument::OnBnClickedButtonChange()
{
	CString strAuxiliaryDescr;
	GetDlgItem(IDC_EDIT_AUXILIARYDESCR)->GetWindowText(strAuxiliaryDescr);
	COleVariant oVariantString = strAuxiliaryDescr;
	m_oGridCtrlEdit.GetRow(0)->GetItem(5)->SetValue(oVariantString);

	CString strAbsoluteSpread;
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->GetWindowText(strAbsoluteSpread);
	strAbsoluteSpread.Replace("\r\n", ";");
	oVariantString = strAbsoluteSpread;
	m_oGridCtrlEdit.GetRow(0)->GetItem(6)->SetValue(oVariantString);

	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabTestInstrument::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Clear
* @param void
* @return void
*/
void CTabTestInstrument::OnBnClickedButtonClear()
{
	OnBnClickedButtonClearA(&m_oGridCtrlList);
}

/**
* 初始化Gain单元格
* @param CBCGPGridItem* pItem Gain单元格
* @return void
*/
void CTabTestInstrument::InitGainGridItem(CBCGPGridItem* pItem)
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
* // 初始化TestType单元格
* @param CBCGPGridItem* pItem Gain单元格
* @return void
*/
void CTabTestInstrument::InitTestTypeGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strTestType;
	for(int i = 0; i < m_pSiteData->m_olsTestTypeInstrument.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsTestTypeInstrument.FindIndex(i);
		strTestType = m_pSiteData->m_olsTestTypeInstrument.GetAt(pos);
		pItem->AddOption(strTestType);
	}
}