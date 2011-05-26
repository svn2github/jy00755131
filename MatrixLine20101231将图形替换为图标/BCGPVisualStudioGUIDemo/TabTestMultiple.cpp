// TabTestMultiple.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabTestMultiple.h"


// CTabTestMultiple

IMPLEMENT_DYNAMIC(CTabTestMultiple, CTabPage)

CTabTestMultiple::CTabTestMultiple()
{
}

CTabTestMultiple::~CTabTestMultiple()
{
}


BEGIN_MESSAGE_MAP(CTabTestMultiple, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabTestMultiple 消息处理程序

int CTabTestMultiple::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabTestMultiple::OnDestroy()
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
void CTabTestMultiple::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
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
	strColumn = "Line Nb";
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 30);
	strColumn = "Test Type";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 80);
	strColumn = "Gain";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 30);
	strColumn = "Loop Line Nb";
	m_oGridCtrlEdit.InsertColumn (3, strColumn, 50);
	strColumn = "Nb Loops";
	m_oGridCtrlEdit.InsertColumn (4, strColumn, 50);
	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	// 初始化TestType队列
	InitTestTypeGridItem(pRow->GetItem(1));
	// 初始化Gain单元格
	InitGainGridItem(pRow->GetItem(2));
	pRow->GetItem(3)->SetValue("0");
	pRow->GetItem(4)->SetValue("0");
	m_oGridCtrlEdit.AddRow(pRow, FALSE);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 60;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 20;
	oRect.bottom = lpCreateStruct->y + 270;
	m_oGridCtrlList.Create(WS_CHILD | WS_VISIBLE, oRect, this, IDC_GRID_LIST);	// 列表
	m_oGridCtrlList.m_iRowHeightAdd = 5;
	m_oGridCtrlList.ShowVertScrollBar();
	m_oGridCtrlList.EnableColumnAutoSize(TRUE);
	m_oGridCtrlList.EnableDragHeaderItems(FALSE);
	m_oGridCtrlList.EnableHeader(FALSE, 0);
	m_oGridCtrlList.SetReadOnly(TRUE);
	m_oGridCtrlList.SetWholeRowSel(TRUE);
	m_oGridCtrlList.SetSingleSel(TRUE);
	strColumn = "Line Nb";
	m_oGridCtrlList.InsertColumn (0, strColumn, 30);
	strColumn = "Test Type";
	m_oGridCtrlList.InsertColumn (1, strColumn, 80);
	strColumn = "Gain";
	m_oGridCtrlList.InsertColumn (2, strColumn, 30);
	strColumn = "Loop Line Nb";
	m_oGridCtrlList.InsertColumn (3, strColumn, 50);
	strColumn = "Nb Loops";
	m_oGridCtrlList.InsertColumn (4, strColumn, 50);

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 280;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx / 2 - 10;
	oRect.bottom = lpCreateStruct->y + 480;
	m_oStaticAuxiliaryDescr.Create("Auxiliary Descr:", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX | WS_DISABLED, oRect, this, 0);	// 辅助道组框

	oRect.left = oRect.left + 10;
	oRect.top = oRect.top + 25;
	oRect.right = oRect.right - 10;
	oRect.bottom = oRect.bottom - 10;
	m_oEditAuxiliaryDescr.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		oRect, this, IDC_EDIT_AUXILIARYDESCR);	// 辅助道编辑框

	oRect.left = lpCreateStruct->x + lpCreateStruct->cx / 2 + 10;
	oRect.top = lpCreateStruct->y + 280;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 30;
	oRect.bottom = lpCreateStruct->y + 480;
	m_oStaticAbsoluteSpread.Create("Absolute Spread:", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX | WS_DISABLED, oRect, this, 0);	// 绝对排列组框
	oRect.left = oRect.left + 10;
	oRect.top = oRect.top + 25;
	oRect.right = oRect.right - 10;
	oRect.bottom = oRect.bottom - 10;
	m_oEditAbsoluteSpread.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		oRect, this, IDC_EDIT_ABSOLUTESPREAD);	// 绝对排列编辑框

	oRect.left = lpCreateStruct->x;
	oRect.top = lpCreateStruct->y + 480;
	oRect.right = lpCreateStruct->x + lpCreateStruct->cx - 30;
	oRect.bottom = lpCreateStruct->y + lpCreateStruct->cy - 70;	
	m_oStaticCommonParam.Create("Common Parameters:", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX | WS_DISABLED, oRect, this, 0);	// 通用组框

	CRect oRectItem;
	oRectItem = oRect;

	oRectItem.left = oRectItem.left + 30;
	oRectItem.top = oRectItem.top + 30;
	oRectItem.right = oRectItem.left + 170;
	oRectItem.bottom = oRectItem.top + 20;
	m_oStaticDelayBetweenTest.Create("Delay Between Test(ms):", WS_CHILD | WS_VISIBLE, oRectItem, this, 0);	// 每次测试之间的间隔（毫秒）

	oRectItem.top = oRectItem.bottom + 5;
	oRectItem.bottom = oRectItem.top + 20;
	m_oStaticRecordResult.Create("Record Results:", WS_CHILD | WS_VISIBLE, oRectItem, this, 0);	// 是否记录测试数据

	oRectItem.top = oRectItem.bottom + 5;
	oRectItem.bottom = oRectItem.top + 20;
	m_oStaticRecordLength.Create("Record Length(ms):", WS_CHILD | WS_VISIBLE, oRectItem, this, 0);	// 测试记录时间（毫秒）

	oRectItem = oRect;
	oRectItem.left = oRectItem.left + 200;
	oRectItem.top = oRectItem.top + 30;
	oRectItem.right = oRectItem.left + 120;
	oRectItem.bottom = oRectItem.top + 20;
	m_oEditDelayBetweenTest.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, oRectItem, this, IDC_EDIT_DELAYBETWEENTEST);	// 每次测试之间的间隔（毫秒）

	oRectItem.top = oRectItem.bottom + 5;
	oRectItem.bottom = oRectItem.top + 20;
	m_oButtonRecordResult.Create("", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_AUTOCHECKBOX, oRectItem, this, IDC_CHECK_RECORDRESULTS);	// 是否记录测试数据

	oRectItem.top = oRectItem.bottom + 5;
	oRectItem.bottom = oRectItem.top + 20;
	m_oEditRecordLength.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, oRectItem, this, IDC_EDIT_RECORDLENGTH);	// 测试记录时间（毫秒）

	oRectItem.left = oRectItem.right + 180;
	oRectItem.right = oRectItem.left + 100;
	m_oStaticTestFileNb.Create("Test File Nb:", WS_CHILD | WS_VISIBLE, oRectItem, this, 0);	// 测试文件号

	oRectItem.left = oRectItem.right;
	oRectItem.right = oRectItem.left + 120;
	m_oEditTestFileNb.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, oRectItem, this, IDC_EDIT_TESTFILENB);	// 测试记录时间（毫秒）
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabTestMultiple::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListLButtonDownA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabTestMultiple::OnGridCtrlListDblClk()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabTestMultiple::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CTestProject* pTestProject = NULL;
	CString strData;

	POSITION pos = m_pSiteData->m_oTestProjectListMultiple.m_olsTestProject.FindIndex(m_uiTestNb - 1);
	pTestProject = m_pSiteData->m_oTestProjectListMultiple.m_olsTestProject.GetAt(pos);
	pTestProject->OnClose();

	GetDlgItem(IDC_EDIT_AUXILIARYDESCR)->GetWindowText(pTestProject->m_strAuxiliaryDescr);
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->GetWindowText(pTestProject->m_strAbsoluteSpread);
	pTestProject->m_strAbsoluteSpread.Replace("\r\n", ";");
	

	pTestProject->m_uiDelayBetweenTest = GetDlgItemInt(IDC_EDIT_DELAYBETWEENTEST);
	pTestProject->m_uiRecordLength = GetDlgItemInt(IDC_EDIT_RECORDLENGTH);
	pTestProject->m_uiTestFileNb = GetDlgItemInt(IDC_EDIT_TESTFILENB);
	if(BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_RECORDRESULTS))->GetCheck())
	{
		pTestProject->m_uiRecordResult = 0;
	}
	else if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_RECORDRESULTS))->GetCheck())
	{
		pTestProject->m_uiRecordResult = 1;
	}

	pTestProject->m_uiCountAll = m_oGridCtrlList.GetRowCount();
	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{
		pRow = m_oGridCtrlList.GetRow(i);
		CTestRequest oTestRequest;
		oTestRequest.m_uiIndex = pRow->GetData();
		strData = pRow->GetItem(1)->GetValue();
		oTestRequest.m_uiType = m_pSiteData->GetTestTypeIDByString(strData);
		strData = pRow->GetItem(2)->GetValue();
		// 判断：循环控制
		if(60 == oTestRequest.m_uiType)
		{
			oTestRequest.m_uiGain = 0;
			strData = pRow->GetItem(3)->GetValue();
			oTestRequest.m_uiLoopNb = CXMLDOMTool::ConvertStringToUnsignedInt(strData);
			strData = pRow->GetItem(4)->GetValue();
			oTestRequest.m_uiLoopCount = CXMLDOMTool::ConvertStringToUnsignedInt(strData);
		}
		else
		{
			oTestRequest.m_uiGain = m_pSiteData->GetGainByString(strData);
			oTestRequest.m_uiLoopNb = 0;
			oTestRequest.m_uiLoopCount = 0;
		}
		pTestProject->m_olsTestRequest.AddTail(oTestRequest);
	}	
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabTestMultiple::OnBnClickedButtonReset()
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
	pRow->GetItem(3)->SetValue("0");
	pRow->GetItem(3)->SetValue("0");

	pos = m_pSiteData->m_oTestProjectListMultiple.m_olsTestProject.FindIndex(m_uiTestNb - 1);
	pTestProject = m_pSiteData->m_oTestProjectListMultiple.m_olsTestProject.GetAt(pos);

	GetDlgItem(IDC_EDIT_AUXILIARYDESCR)->SetWindowText(pTestProject->m_strAuxiliaryDescr);
	pTestProject->m_strAbsoluteSpread.Replace(";", "\r\n");
	GetDlgItem(IDC_EDIT_ABSOLUTESPREAD)->SetWindowText(pTestProject->m_strAbsoluteSpread);
	SetDlgItemInt(IDC_EDIT_DELAYBETWEENTEST, pTestProject->m_uiDelayBetweenTest);
	SetDlgItemInt(IDC_EDIT_RECORDLENGTH, pTestProject->m_uiRecordLength);
	SetDlgItemInt(IDC_EDIT_TESTFILENB, pTestProject->m_uiTestFileNb);
	if(0 == pTestProject->m_uiRecordResult)
	{
		((CButton*)GetDlgItem(IDC_CHECK_RECORDRESULTS))->SetCheck(BST_UNCHECKED);
	}
	else if(1 == pTestProject->m_uiRecordResult)
	{
		((CButton*)GetDlgItem(IDC_CHECK_RECORDRESULTS))->SetCheck(BST_CHECKED);
	}

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = pTestProject->m_olsTestRequest.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = pTestProject->m_olsTestRequest.FindIndex(i);
		oTestRequest = pTestProject->m_olsTestRequest.GetAt(pos);

		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());
		pRow->SetData(oTestRequest.m_uiIndex);

		oVariantString = (long)oTestRequest.m_uiIndex;
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(0)->SetValue(oVariantString);

		oVariantString = m_pSiteData->GetTestTypeStringByID(oTestRequest.m_uiType);
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(1)->SetValue(oVariantString);

		oVariantString = m_pSiteData->GetGainStringByGainData(oTestRequest.m_uiGain);
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(2)->SetValue(oVariantString);

		oVariantString = (long)oTestRequest.m_uiLoopNb;
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(3)->SetValue(oVariantString);

		oVariantString = (long)oTestRequest.m_uiLoopCount;
		oVariantString.ChangeType(VT_BSTR);
		pRow->GetItem(4)->SetValue(oVariantString);

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
bool CTabTestMultiple::OnBnClickedButtonGO()
{
	return true;
}

/**
* 响应鼠标单击按钮；Add
* @param void
* @return void
*/
void CTabTestMultiple::OnBnClickedButtonAdd()
{
	OnBnClickedButtonAddA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabTestMultiple::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Delete
* @param void
* @return void
*/
void CTabTestMultiple::OnBnClickedButtonDelete()
{
	OnBnClickedButtonDeleteA(&m_oGridCtrlEdit, &m_oGridCtrlList, &m_olsNb);
}

/**
* 响应鼠标单击按钮；Clear
* @param void
* @return void
*/
void CTabTestMultiple::OnBnClickedButtonClear()
{
	OnBnClickedButtonClearA(&m_oGridCtrlList);
}

/**
* 初始化Gain单元格
* @param CBCGPGridItem* pItem Gain单元格
* @return void
*/
void CTabTestMultiple::InitGainGridItem(CBCGPGridItem* pItem)
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
void CTabTestMultiple::InitTestTypeGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strTestType;
	for(int i = 0; i < m_pSiteData->m_olsTestTypeAll.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsTestTypeAll.FindIndex(i);
		strTestType = m_pSiteData->m_olsTestTypeAll.GetAt(pos);
		pItem->AddOption(strTestType);
	}
}