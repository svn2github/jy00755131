// SensorListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "SensorListCtrl.h"

// CSensorListCtrl

IMPLEMENT_DYNAMIC(CSensorListCtrl, CBCGPGridCtrl)

CSensorListCtrl::CSensorListCtrl()
{

}

CSensorListCtrl::~CSensorListCtrl()
{
}

BEGIN_MESSAGE_MAP(CSensorListCtrl, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSensorListCtrl 消息处理程序

int CSensorListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableMarkSortedColumn(TRUE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_SORT);
	EnableRowHeader(FALSE);
	EnableLineNumbers(FALSE);
	SetClearInplaceEditOnEnter(FALSE);
	EnableInvertSelOnCtrl();
	SetReadOnly(TRUE);
	SetWholeRowSel(TRUE);

	// 加载现场测道数据队列
	OnLoadSiteDataAndFillToChannelList();

	return 0;
}

void CSensorListCtrl::OnDestroy()
{
	CBCGPGridCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
//	SaveState (_T("BasicGrid"));
}


// 现场数据对象初始化
void CSensorListCtrl::OnSiteDataInit()
{
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pSiteData = &pMainFrame->m_oSiteData;
}

// 加载现场测道数据队列
void CSensorListCtrl::OnLoadSiteDataAndFillToChannelList()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 生成表格头
	CreateColumTitle();

	CChannelSetupData* pChannelSetupData = NULL;
	CBCGPGridRow* pRow = NULL;
	for (unsigned int nRow = 0; nRow < m_pSiteData->m_oChannelList.m_uiCountUsed; nRow++)
	{
		pRow = CreateRow(GetColumnCount());
		pChannelSetupData = m_pSiteData->m_oChannelList.GetChannelByIndex(nRow);
		// 设置一行的数据
		SetRowData(pRow, pChannelSetupData);
		AddRow(pRow, FALSE);
	}
	//	LoadState (_T("BasicGrid"));
	AdjustLayout ();
}

// 重新加载现场测道数据
void CSensorListCtrl::OnReloadSiteDataCMD()
{
	// 现场数据对象初始化
	OnSiteDataInit();

	CChannelSetupData* pChannelSetupData = NULL;
	CBCGPGridRow* pRow = NULL;
	for (unsigned int nRow = 0; nRow < m_pSiteData->m_oChannelList.m_uiCountUsed; nRow++)
	{
		// 填充本次数据
		pChannelSetupData = m_pSiteData->m_oChannelList.GetChannelByIndex(nRow);
		pRow = FindRowByData(pChannelSetupData->m_uiIP, FALSE);
		if(NULL != pRow)	// 找到行
		{
			SetRowData(pRow, pChannelSetupData);	// 设置一行的数据
		}
		else
		{
			pRow = CreateRow(GetColumnCount());
			// 设置一行的数据
			SetRowData(pRow, pChannelSetupData);
			AddRow(pRow, FALSE);
		}
	}
	// 删除没有用到的行
	for(int i = (GetRowCount() - 1); i >= 0; i--)
	{
		pRow = GetRow(i);
		if(NULL != pRow)	// 找到行
		{
			if(FALSE == m_pSiteData->m_oChannelList.IfIndexExistInMap(pRow->GetData()))
			{
				RemoveRow(i, FALSE);
			}
		}
		else
		{
			TRACE1("SensorList行不存在%d", i);
		}
	}
	//	LoadState (_T("BasicGrid"));
	AdjustLayout ();
}

// 为新的测试数据刷新显示
void CSensorListCtrl::OnRefreshForNewTestValue(int iTestType)
{
	unsigned int uiIP = 0;
	CChannelSetupData* pChannelSetupData = NULL;
	CBCGPGridRow* pRow = NULL;
	while(true)
	{
		if(TRUE == m_pSiteData->m_olsIPSensor.IsEmpty())
		{
			break;
		}
		uiIP = m_pSiteData->m_olsIPSensor.RemoveHead();
		if(TRUE == m_pSiteData->m_oChannelList.m_oChannelMap.Lookup(uiIP, pChannelSetupData))
		{
			pRow = FindRowByData(uiIP, FALSE);
			if(NULL != pRow)	// 找到行
			{
				SetRowDataOfTestByTestType(pRow, pChannelSetupData, iTestType);
				// 判断：不是新加检波器的初始测试
				if(45 != iTestType)
				{
					SetRowDataOfTestByTestType(pRow, pChannelSetupData, 40);
				}
			}
		}
	}	
}

// 生成表格头
void CSensorListCtrl::CreateColumTitle()
{
	unsigned int nColumns = 0;
	CString strColumn;

	strColumn = "Serial Nb";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Line Nb";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Point Nb";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Point Index";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Sensor Type";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Segd Code";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "IP";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Jump Channel";
	InsertColumn (nColumns, strColumn, 100);
	nColumns++;

	strColumn = "Mute";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Test";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Auto Test";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Resistance";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Leakage";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Noise";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Tilt";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Tilt Model";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;
}

/**
* 设置一行的数据
* @param CBCGPGridRow* pRow 行指针
* @param CChannelSetupData* pChannelSetupData 测道指针
* @return void
*/
void CSensorListCtrl::SetRowData(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData)
{
	SetRowDataOfBase(pRow, pChannelSetupData);
	SetRowDataOfTest(pRow, pChannelSetupData);
}

/**
* 设置一行的基本数据
* @param CBCGPGridRow* pRow 行指针
* @param CChannelSetupData* pChannelSetupData 测道指针
* @return void
*/
void CSensorListCtrl::SetRowDataOfBase(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData)
{
	pRow->SetData(pChannelSetupData->m_uiIP);

	int nItems = 0;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiSN, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiNbLine, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiNbPoint, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiNbChannel, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiSensorNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiSegdCode, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pChannelSetupData->m_uiIP, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pChannelSetupData->m_bJumped), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pChannelSetupData->m_bMute), FALSE);
}

/**
* 设置一行的测试数据
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CSensorListCtrl::SetRowDataOfTest(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData)
{
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 40);
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 45);
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 15);
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 14);
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 1);
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 20);
	SetRowDataOfTestByTestType(pRow, pChannelSetupData, 21);
}

/**
* 设置一行的测试数据，依据测试类型
* @param CBCGPGridRow* pRow 行指针
* @param CChannelSetupData* pChannelSetupData 测道指针
* @return void
*/
void CSensorListCtrl::SetRowDataOfTestByTestType(CBCGPGridRow* pRow, CChannelSetupData* pChannelSetupData, int iTestType)
{
	unsigned int nItems = 0;

	switch(iTestType)
	{
		// 野外电阻
	case 15:
		nItems = 11;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byResistanceCheck, pChannelSetupData->m_fResistanceValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byResistanceCheck));
		break;
		// 野外漏电
	case 14:
		nItems = 12;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byLeakageCheck, pChannelSetupData->m_fLeakageValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byLeakageCheck));
		break;
		// 野外噪声
	case 1:
		nItems = 13;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byNoiseCheck, pChannelSetupData->m_fNoiseValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byNoiseCheck));
		break;
		// 野外倾斜度
	case 20:
		nItems = 14;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byTiltCheck, pChannelSetupData->m_fTiltValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byTiltCheck));
		break;
		// 野外倾斜度模型
	case 21:
		nItems = 15;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byTiltModelCheck, pChannelSetupData->m_fTiltModelValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byTiltModelCheck));
		break;
		// 全部野外测试
	case 40:
		nItems = 9;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byCheck), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byCheck));
		break;
		// 新加检波器的初始测试
	case 45:
		nItems = 10;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byOnFieldCheck), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byOnFieldCheck));
		break;
	}
}

/**
* 得到背景颜色，依据测试检查结果
* @param byte byCheck 检查结果
* @return void
*/
COLORREF CSensorListCtrl::GetColourByCheck(byte byCheck)
{
	COLORREF m_crColor = RGB(255, 255, 255);	// 白色
	switch(byCheck)
	{
	case 0:	// 0：未测试
		m_crColor = RGB(0, 0, 255);	// 蓝色
		break;
	case 1:	// 1：测试合格
		m_crColor = RGB(0, 255, 0);	// 绿色
		break;
	case 2:	// 2：测试不合格
		m_crColor = RGB(255, 0, 0);	// 红色
		break;
	case 3:	// 3：无信号
		m_crColor = RGB(255, 255, 0);	// 黄色
		break;
	}
	return m_crColor;
}

/**
* 得到标记文字，依据逻辑值
* @param bool bValue 逻辑值
* @return COleVariant 标记文字
*/
COleVariant CSensorListCtrl::GetMarkerStringByLogic(bool bValue)
{
	COleVariant oVariant;
	CString strValue = "";
	if(true == bValue)
	{
		strValue = "●";
	}
	oVariant = strValue;
	return oVariant;
}

/**
* 得到测试结果文字
* @param byte byValue 测试结果
* @param float m_fValue 测试值
* @return COleVariant 测试结果文字
*/
COleVariant CSensorListCtrl::GetTestResultString(byte byValue, float m_fValue)
{
	COleVariant oVariant;
	CString strValue = "";
	switch(byValue)
	{
	case 0:
		strValue = "No Test";
		break;
	case 1:
		strValue.Format("%3.2f", m_fValue);
		break;
	case 2:
		strValue.Format("%3.2f", m_fValue);
		break;
	case 3:
		strValue = "No signal";
		break;
	case 4:
		strValue = "No Judge";
		break;
	}
	oVariant = strValue;
	return oVariant;
}

/**
* 得到测试结果文字
* @param byte byValue 测试结果
* @return COleVariant 测试结果文字
*/
COleVariant CSensorListCtrl::GetTestResultString(byte byValue)
{
	COleVariant oVariant;
	CString strValue = "";
	switch(byValue)
	{
	case 0:
		strValue = "No Test";
		break;
	case 1:
		strValue = "OK";
		break;
	case 2:
		strValue = "Fail";
		break;
	case 3:
		strValue = "No signal";
		break;
	case 4:
		strValue = "No Judge";
		break;
	}
	oVariant = strValue;
	return oVariant;
}