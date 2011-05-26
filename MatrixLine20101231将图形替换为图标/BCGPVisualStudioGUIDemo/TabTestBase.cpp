// TabTestBase.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "TabTestBase.h"


// CTabTestBase

IMPLEMENT_DYNAMIC(CTabTestBase, CTabPage)

CTabTestBase::CTabTestBase()
{
}

CTabTestBase::~CTabTestBase()
{
}


BEGIN_MESSAGE_MAP(CTabTestBase, CTabPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GRID_LIST_LBBUTTONDOWN, OnGridCtrlListLButtonDown)
	ON_COMMAND(IDC_GRID_LIST_DBLCLK, OnGridCtrlListDblClk)
END_MESSAGE_MAP()

// CTabTestBase 消息处理程序

int CTabTestBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 生成编辑界面
	OnCreateCtrl(lpCreateStruct);

	return 0;
}    
void CTabTestBase::OnDestroy()
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
void CTabTestBase::OnCreateCtrl(LPCREATESTRUCT lpCreateStruct)
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
	m_oGridCtrlEdit.InsertColumn (0, strColumn, 10);
	strColumn = "Descr";
	m_oGridCtrlEdit.InsertColumn (1, strColumn, 80);
	strColumn = "TestType";
	m_oGridCtrlEdit.InsertColumn (2, strColumn, 35);
	strColumn = "ADC";
	m_oGridCtrlEdit.InsertColumn (3, strColumn, 35);
	strColumn = "Gain";
	m_oGridCtrlEdit.InsertColumn (4, strColumn, 40);
	strColumn = "DAC";
	m_oGridCtrlEdit.InsertColumn (5, strColumn, 35);
	strColumn = "Filter";
	m_oGridCtrlEdit.InsertColumn (6, strColumn, 40);
	strColumn = "SamplingRate(us)";
	m_oGridCtrlEdit.InsertColumn (7, strColumn, 60);
	strColumn = "SamplingLength(ms)";
	m_oGridCtrlEdit.InsertColumn (8, strColumn, 65);

	pRow = m_oGridCtrlEdit.CreateRow(m_oGridCtrlEdit.GetColumnCount());
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(0)->AllowEdit(FALSE);
	pRow->GetItem(0)->SetBackgroundColor(RGB(192, 192, 192));
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(1)->AllowEdit(FALSE);
	pRow->GetItem(1)->SetBackgroundColor(RGB(192, 192, 192));
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(2)->AllowEdit(FALSE);
	pRow->GetItem(2)->SetBackgroundColor(RGB(192, 192, 192));
	// 初始化ADCType单元格
	InitADCTypeGridItem(pRow->GetItem(3));
	// 初始化Gain单元格
	InitGainGridItem(pRow->GetItem(4));
	// 初始化DACType单元格
	InitDACTypeGridItem(pRow->GetItem(5));
	// 初始化FilterType单元格
	InitFilterTypeGridItem(pRow->GetItem(6));
	pRow->GetItem(7)->SetValue("");
	pRow->GetItem(8)->SetValue("");
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
	m_oGridCtrlList.InsertColumn (0, strColumn, 10);
	strColumn = "Descr";
	m_oGridCtrlList.InsertColumn (1, strColumn, 80);
	strColumn = "TestType";
	m_oGridCtrlList.InsertColumn (2, strColumn, 35);
	strColumn = "ADC";
	m_oGridCtrlList.InsertColumn (3, strColumn, 35);
	strColumn = "Gain";
	m_oGridCtrlList.InsertColumn (4, strColumn, 40);
	strColumn = "DAC";
	m_oGridCtrlList.InsertColumn (5, strColumn, 35);
	strColumn = "Filter";
	m_oGridCtrlList.InsertColumn (6, strColumn, 40);
	strColumn = "SamplingRate(us)";
	m_oGridCtrlList.InsertColumn (7, strColumn, 70);
	strColumn = "SamplingLength(ms)";
	m_oGridCtrlList.InsertColumn (8, strColumn, 70);
}

/**
* 响应鼠标单击列表
* @param void
* @return void
*/
void CTabTestBase::OnGridCtrlListLButtonDown()
{
	OnGridCtrlListDblClkA(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 响应鼠标单击双击列表
* @param void
* @return void
*/
void CTabTestBase::OnGridCtrlListDblClk()
{

}

/**
* 响应鼠标单击按钮；Apply
* @param void
* @return void
*/
void CTabTestBase::OnBnClickedButtonApply()
{
	CBCGPGridRow* pRow = NULL;
	CTestBaseList* pTestBaseList = NULL;
	CString strData;
	COleVariant oVariant;
	POSITION pos = NULL;

	if(1 == m_uiAim)	// 测试对象 1-仪器；2-检波器
	{
		pTestBaseList = &m_pSiteData->m_oTestBaseListInstrument;
	}
	else if(2 == m_uiAim)	// 测试对象 1-仪器；2-检波器
	{
		pTestBaseList = &m_pSiteData->m_oTestBaseListSensor;
	}

	for(int i = 0; i < m_oGridCtrlList.GetRowCount(); i++)
	{	
		pRow = m_oGridCtrlList.GetRow(i);

		pos = pTestBaseList->m_olsTestBase.FindIndex(i);
		CTestBaseData& oTestBaseData = pTestBaseList->m_olsTestBase.GetAt(pos);		

		strData = pRow->GetItem(3)->GetValue();
		oTestBaseData.m_uiADC = m_pSiteData->GetADCTypeIDByString(strData);

		strData = pRow->GetItem(4)->GetValue();
		oTestBaseData.m_uiGain = m_pSiteData->GetGainByString(strData);

		strData = pRow->GetItem(5)->GetValue();
		oTestBaseData.m_uiDAC = m_pSiteData->GetDACTypeIDByString(strData);

		strData = pRow->GetItem(6)->GetValue();
		oTestBaseData.m_uiFilter = m_pSiteData->GetFilterTypeIDByString(strData);

		strData = pRow->GetItem(7)->GetValue();
		oTestBaseData.m_uiSamplingRate = CXMLDOMTool::ConvertStringToUnsignedInt(strData);

		strData = pRow->GetItem(8)->GetValue();
		oTestBaseData.m_uiSamplingLength = CXMLDOMTool::ConvertStringToUnsignedInt(strData);
	}
}

/**
* 响应鼠标单击按钮；Reset
* @param void
* @return void
*/
void CTabTestBase::OnBnClickedButtonReset()
{
	CBCGPGridRow* pRow = NULL;
	POSITION pos= NULL;
	CTestBaseList* pTestBaseList = NULL;
	COleVariant oVariant;

	if(1 == m_uiAim)	// 测试对象 1-仪器；2-检波器
	{
		pTestBaseList = &m_pSiteData->m_oTestBaseListInstrument;
	}
	else if(2 == m_uiAim)	// 测试对象 1-仪器；2-检波器
	{
		pTestBaseList = &m_pSiteData->m_oTestBaseListSensor;
	}

	pRow = m_oGridCtrlEdit.GetRow(0);
	pRow->GetItem(0)->SetValue("");
	pRow->GetItem(1)->SetValue("");
	pRow->GetItem(2)->SetValue("");
	pRow->GetItem(3)->SetValue("");
	pRow->GetItem(4)->SetValue("");
	pRow->GetItem(5)->SetValue("");
	pRow->GetItem(6)->SetValue("");
	pRow->GetItem(7)->SetValue("");
	pRow->GetItem(8)->SetValue("");

	m_oGridCtrlList.RemoveAll();	// 列表
	int iCount = pTestBaseList->m_olsTestBase.GetCount();
	for(int i = 0; i < iCount; i++)
	{
		pos = pTestBaseList->m_olsTestBase.FindIndex(i);
		CTestBaseData& oTestBaseData = pTestBaseList->m_olsTestBase.GetAt(pos);
		pRow = m_oGridCtrlList.CreateRow(m_oGridCtrlList.GetColumnCount());

		oVariant = (long)oTestBaseData.m_uiNb;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(0)->SetValue(oVariant);

		oVariant = oTestBaseData.m_strDesc;
		pRow->GetItem(1)->SetValue(oVariant);

		oVariant = (long)oTestBaseData.m_uiType;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(2)->SetValue(oVariant);

		// 得到ADCType字符串
		CString strADCType = m_pSiteData->GetADCTypeStringByID(oTestBaseData.m_uiADC);
		oVariant = strADCType;
		pRow->GetItem(3)->SetValue(oVariant);

		// 得到Gain字符串
		CString strGain = m_pSiteData->GetGainStringByGainData(oTestBaseData.m_uiGain);
		oVariant = strGain;
		pRow->GetItem(4)->SetValue(oVariant);

		// 得到DACType字符串
		CString strDACType = m_pSiteData->GetDACTypeStringByID(oTestBaseData.m_uiDAC);
		oVariant = strDACType;
		pRow->GetItem(5)->SetValue(oVariant);

		// 得到Filter字符串
		CString strFilter = m_pSiteData->GetFilterTypeStringByID(oTestBaseData.m_uiFilter);
		oVariant = strFilter;
		pRow->GetItem(6)->SetValue(oVariant);

		// 采样率 单位微秒
		oVariant = (long)oTestBaseData.m_uiSamplingRate;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(7)->SetValue(oVariant);

		// 采样时长 单位毫秒
		oVariant = (long)oTestBaseData.m_uiSamplingLength;
		oVariant.ChangeType(VT_BSTR);
		pRow->GetItem(8)->SetValue(oVariant);

		m_oGridCtrlList.AddRow(pRow, FALSE);
	}
	m_oGridCtrlEdit.AdjustLayout();
	m_oGridCtrlList.AdjustLayout();
}

/**
* 响应鼠标单击按钮；Change
* @param void
* @return void
*/
void CTabTestBase::OnBnClickedButtonChange()
{
	OnBnClickedButtonChangeB(&m_oGridCtrlEdit, &m_oGridCtrlList);
}

/**
* 初始化Gain单元格
* @param CBCGPGridItem* pItem Gain单元格
* @return void
*/
void CTabTestBase::InitGainGridItem(CBCGPGridItem* pItem)
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
* 初始化FilterType单元格
* @param CBCGPGridItem* pItem FilterType单元格
* @return void
*/
void CTabTestBase::InitFilterTypeGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strFilterType;
	for(int i = 0; i < m_pSiteData->m_olsFilterType.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsFilterType.FindIndex(i);
		strFilterType = m_pSiteData->m_olsFilterType.GetAt(pos);
		pItem->AddOption(strFilterType);
	}
}

/**
* 初始化ADCType单元格
* @param CBCGPGridItem* pItem ADCType单元格
* @return void
*/
void CTabTestBase::InitADCTypeGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strADCType;
	for(int i = 0; i < m_pSiteData->m_olsADCType.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsADCType.FindIndex(i);
		strADCType = m_pSiteData->m_olsADCType.GetAt(pos);
		pItem->AddOption(strADCType);
	}
}

/**
* 初始化DACType单元格
* @param CBCGPGridItem* pItem DACType单元格
* @return void
*/
void CTabTestBase::InitDACTypeGridItem(CBCGPGridItem* pItem)
{
	POSITION pos;
	CString strDACType;
	for(int i = 0; i < m_pSiteData->m_olsDACType.GetCount(); i++)
	{
		pos = m_pSiteData->m_olsDACType.FindIndex(i);
		strDACType = m_pSiteData->m_olsDACType.GetAt(pos);
		pItem->AddOption(strDACType);
	}
}