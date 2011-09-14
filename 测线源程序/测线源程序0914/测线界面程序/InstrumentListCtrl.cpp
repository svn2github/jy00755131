// InstrumentListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "BCGPVisualStudioGUIDemo.h"
#include "InstrumentListCtrl.h"

// CInstrumentListCtrl

IMPLEMENT_DYNCREATE(CInstrumentListCtrl, CBCGPGridCtrl)

CInstrumentListCtrl::CInstrumentListCtrl()
{

}

CInstrumentListCtrl::~CInstrumentListCtrl()
{
}

BEGIN_MESSAGE_MAP(CInstrumentListCtrl, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CInstrumentListCtrl 消息处理程序

int CInstrumentListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	// 加载现场仪器数据队列
	OnLoadSiteDataAndFillToInstrumentList();

	return 0;
}

void CInstrumentListCtrl::OnDestroy()
{
	CBCGPGridCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//	SaveState (_T("BasicGrid"));
}


// 现场数据对象初始化
void CInstrumentListCtrl::OnSiteDataInit()
{
	CMainFrame* pMainFrame = NULL;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pSiteData = &pMainFrame->m_oSiteData;
}

// 加载现场仪器数据队列
void CInstrumentListCtrl::OnLoadSiteDataAndFillToInstrumentList()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 生成表格头
	CreateColumTitle();

	CInstrument* pInstrument = NULL;
	CBCGPGridRow* pRow = NULL;
	POSITION pos = m_pSiteData->m_oInstrumentList.GetFirstInstrumentPosition();
	while(NULL != pos)
	{
		m_pSiteData->m_oInstrumentList.GetNextInstrumentByPosition(pos, pInstrument);
		if(NULL != pInstrument)
		{
			pRow = CreateRow(GetColumnCount());
			// 设置一行的数据
			SetRowData(pRow, pInstrument);
			AddRow(pRow, FALSE);
		}		
	}
	//	LoadState (_T("BasicGrid"));
	AdjustLayout ();
}

// 重新加载现场仪器数据
void CInstrumentListCtrl::OnReloadSiteDataCMD()
{
	// 现场数据对象初始化
	OnSiteDataInit();

	CInstrument* pInstrument = NULL;
	CBCGPGridRow* pRow = NULL;
	POSITION pos = m_pSiteData->m_oInstrumentList.GetFirstInstrumentPosition();
	while(NULL != pos)
	{
		// 填充本次数据
		m_pSiteData->m_oInstrumentList.GetNextInstrumentByPosition(pos, pInstrument);
		if(NULL != pInstrument)
		{
			pRow = FindRowByData(pInstrument->m_uiSN, FALSE);
			if(NULL != pRow)	// 找到行
			{
				SetRowData(pRow, pInstrument);	// 设置一行的数据
			}
			else
			{
				pRow = CreateRow(GetColumnCount());
				// 设置一行的数据
				SetRowData(pRow, pInstrument);
				AddRow(pRow, FALSE);
			}
		}
	}
	// 删除没有用到的行
	for(int i = (GetRowCount() - 1); i >= 0; i--)
	{
		pRow = GetRow(i);
		if(NULL != pRow)	// 找到行
		{
			if(FALSE == m_pSiteData->m_oInstrumentList.m_oSNInstrumentMap.IfIndexExistInMap(pRow->GetData()))
			{
				RemoveRow(i, FALSE);
			}
		}
		else
		{
			TRACE1("Instrument行不存在%d\r\n", i);
		}

	}
	//	LoadState (_T("BasicGrid"));
	AdjustLayout ();
}

// 为新的测试数据刷新显示
void CInstrumentListCtrl::OnRefreshForNewTestValue(int iTestType)
{
	unsigned int uiIP = 0;
	CInstrument* pInstrument = NULL;
	CBCGPGridRow* pRow = NULL;
	while(true)
	{
		if(TRUE == m_pSiteData->m_olsIPInstrument.IsEmpty())
		{
			break;
		}
		uiIP = m_pSiteData->m_olsIPInstrument.RemoveHead();
		if(TRUE == m_pSiteData->m_oInstrumentList.m_oIPInstrumentMap.GetInstrument(uiIP, pInstrument))
		{
			pRow = FindRowByData(pInstrument->m_uiSN, FALSE);
			if(NULL != pRow)	// 找到行
			{
				SetRowDataOfTestByTestType(pRow, pInstrument, iTestType);
				SetRowDataOfTestByTestType(pRow, pInstrument, 30);
			}
		}
	}	
}

// 生成表格头
void CInstrumentListCtrl::CreateColumTitle()
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

	strColumn = "Type";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "IP";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Test";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Noise";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Distortion";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Crosstalk";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Gain";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Phase";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Gain/Phase";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "CMRR";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Rout IP";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Direction";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Rout Top";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Rout Down";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Rout Left";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Rout Right";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Time Head";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Time Delay";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Jump Channel";
	InsertColumn (nColumns, strColumn, 100);
	nColumns++;

	strColumn = "Sensor";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Blast";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Aux";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Detour";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Detour L";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Detour H";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	strColumn = "Marker";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;
}

/**
* 设置一行的数据
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentListCtrl::SetRowData(CBCGPGridRow* pRow, CInstrument* pInstrument)
{
	SetRowDataOfBase(pRow, pInstrument);
	SetRowDataOfTest(pRow, pInstrument);
}

/**
* 设置一行的基本数据
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentListCtrl::SetRowDataOfBase(CBCGPGridRow* pRow, CInstrument* pInstrument)
{
	pRow->SetData(pInstrument->m_uiSN);

	unsigned int nItems = 0;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiSN, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiLineNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiPointNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiChannelNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetInstrumentTypeStringByInstrumentType(pInstrument->m_uiInstrumentType), FALSE);
	nItems++;

	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiIP, FALSE);
	nItems = 14;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiRoutIP, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetDirectionStringByRoutDirection(pInstrument->m_uiRoutDirection), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiRoutIPTop, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiRoutIPDown, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiRoutIPLeft, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiRoutIPRight, FALSE);
	nItems++;

	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiTimeHeadFrame, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(pInstrument->m_uiTimeDelay, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bJumpedChannel), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bSensor), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bBlastMachine), FALSE);

	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bAux), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bDetour), FALSE);
	nItems++;

	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bDetourMarkerLow), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bDetourMarkerHigh), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(pInstrument->m_bMarker), FALSE);
	nItems++;
}

/**
* 设置一行的测试数据
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentListCtrl::SetRowDataOfTest(CBCGPGridRow* pRow, CInstrument* pInstrument)
{
	SetRowDataOfTestByTestType(pRow, pInstrument, 30);
	SetRowDataOfTestByTestType(pRow, pInstrument, 4);
	SetRowDataOfTestByTestType(pRow, pInstrument, 5);
	SetRowDataOfTestByTestType(pRow, pInstrument, 7);
	SetRowDataOfTestByTestType(pRow, pInstrument, 6);
	SetRowDataOfTestByTestType(pRow, pInstrument, 8);
}

/**
* 设置一行的测试数据，依据测试类型
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentListCtrl::SetRowDataOfTestByTestType(CBCGPGridRow* pRow, CInstrument* pInstrument, int iTestType)
{
	unsigned int nItems = 0;
	
	switch(iTestType)
	{
	case 4:	// 仪器噪声
		nItems = 7;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byNoiseCheck, pInstrument->m_fNoiseValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byNoiseCheck));
		break;
	case 5:	// 仪器畸变
		nItems = 8;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byDistortionCheck, pInstrument->m_fDistortionValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byDistortionCheck));
		break;
	case 7:	// 仪器串音
		nItems = 9;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byCrosstalkCheck, pInstrument->m_fCrosstalkValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byCrosstalkCheck));
		break;
	case 6:	// 仪器增益/相位
		nItems = 10;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byGainCheck, pInstrument->m_fGainValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byGainCheck));
		nItems++;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byPhaseCheck, pInstrument->m_fPhaseValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byPhaseCheck));
		nItems++;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byGainAndPhaseCheck), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byGainAndPhaseCheck));
		break;
	case 8:	// 仪器共模
		nItems = 13;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byCMRRCheck, pInstrument->m_fCMRRValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byCMRRCheck));
		break;
	case 30:	// 所有仪器测试
		nItems = 6;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(pInstrument->m_byCheck), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pInstrument->m_byCheck));
		break;
	}
}

/**
* 得到背景颜色，依据测试检查结果
* @param byte byCheck 检查结果
* @return void
*/
COLORREF CInstrumentListCtrl::GetColourByCheck(byte byCheck)
{
	COLORREF crColor = RGB(255, 255, 255);	// 白色
	switch(byCheck)
	{
	case 0:	// 0：未测试
		crColor = RGB(0, 0, 255);	// 蓝色
		break;
	case 1:	// 1：测试合格
		crColor = RGB(0, 255, 0);	// 绿色
		break;
	case 2:	// 2：测试不合格
		crColor = RGB(255, 0, 0);	// 红色
		break;
	case 3:	// 3：无信号
		crColor = RGB(255, 255, 0);	// 黄色
		break;
	}
	return crColor;
}

/**
* 得到标记文字，依据逻辑值
* @param bool bValue 逻辑值
* @return COleVariant 标记文字
*/
COleVariant CInstrumentListCtrl::GetMarkerStringByLogic(bool bValue)
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
* 得到方向文字，依据方向值
* @param unsigned int uiRoutDirection 方向值
* @return COleVariant 方向文字
*/
COleVariant CInstrumentListCtrl::GetDirectionStringByRoutDirection(unsigned int uiRoutDirection)
{
	COleVariant oVariant;
	CString strValue = "";
	// 判断方向
	switch(uiRoutDirection)
	{
	case 1:	// 上
		{
			strValue = "↑";
			break;
		}
	case 2:	// 下
		{
			strValue = "↓";
			break;
		}
	case 3:	// 左
		{
			strValue = "←";
			break;
		}
	case 4:	// 右
		{
			strValue = "→";
			break;
		}
	}
	oVariant = strValue;
	return oVariant;
}

/**
* 得到仪器类型文字，依据仪器类型值
* @param unsigned int uiInstrumentType 仪器类型
* @return COleVariant 仪器类型文字
*/
COleVariant CInstrumentListCtrl::GetInstrumentTypeStringByInstrumentType(unsigned int uiInstrumentType)
{
	COleVariant oVariant;
	CString strValue = "";
	// 判断方向
	switch(uiInstrumentType)
	{
	case 1:	// 交叉站
		{
			strValue = "¤";
			break;
		}
	case 2:	// 电源站
		{
			strValue = "□";
			break;
		}
	case 3:	// 采集站
		{
			strValue = "⊙";
			break;
		}
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
COleVariant CInstrumentListCtrl::GetTestResultString(byte byValue, float m_fValue)
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
COleVariant CInstrumentListCtrl::GetTestResultString(byte byValue)
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