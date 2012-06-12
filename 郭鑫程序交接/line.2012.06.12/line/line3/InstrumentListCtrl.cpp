// InstrumentListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "InstrumentListCtrl.h"

// CInstrumentListCtrl

IMPLEMENT_DYNCREATE(CInstrumentListCtrl, CBCGPGridCtrl)

CInstrumentListCtrl::CInstrumentListCtrl()
{
	helper_ = NULL;
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
	helper_ = ((CLineApp*)AfxGetApp())->get_data_base_helper();
}

// 加载现场仪器数据队列
void CInstrumentListCtrl::OnLoadSiteDataAndFillToInstrumentList()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	if(helper_ == NULL) return;

	CBCGPGridRow* pRow = NULL;
	matrix_data::data_base_helper::const_device_info_map_reference& map = helper_->get_devices_info();
	matrix_data::data_base_helper::device_info_map::const_iterator itr;

	// 生成表格头
	CreateColumTitle();
	
	helper_->devices_info_lock();
	if(map.size() > 0){
		for(itr = map.begin(); itr != map.end(); ++itr){
			pRow = CreateRow(GetColumnCount());
			// 设置一行的数据
			SetRowData(pRow, itr->second);
			AddRow(pRow, FALSE);
		}
	}
	helper_->devices_info_unlock();
	
	AdjustLayout ();
}

// 重新加载现场仪器数据
void CInstrumentListCtrl::OnReloadSiteDataCMD()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	if(helper_ == NULL) return;

	unsigned int sn = 0x0;
	matrix_data::data_base_helper::const_device_info_map_reference map = helper_->get_devices_info();
	matrix_data::data_base_helper::device_info_map::const_iterator itr;
	CBCGPGridRow* pRow = NULL;
	
	helper_->devices_info_lock();
	if(map.size() > 0){
		for(itr = map.begin(); itr != map.end(); ++itr){
			pRow = FindRowByData(itr->second.sn_, FALSE);
			if(pRow != NULL){ 
				SetRowData(pRow, itr->second);	
			}
			else{
				pRow = CreateRow(GetColumnCount());
				// 设置一行的数据
				SetRowData(pRow, itr->second);
				AddRow(pRow, FALSE);
			}
		}
	}
	helper_->devices_info_unlock();

	// 删除没有用到的行
	for(int i = (GetRowCount() - 1); i >= 0; i--){
		pRow = GetRow(i);
		if(pRow != NULL){	// 找到行
			sn = pRow->GetData();
			helper_->devices_info_lock();
			itr = map.find(sn);
			helper_->devices_info_unlock();
			if(itr == map.end())	RemoveRow(i, FALSE);
		}
		else{
			TRACE1("Instrument行不存在%d\r\n", i);
		}
	}	
	AdjustLayout ();
}

// 为新的测试数据刷新显示
void CInstrumentListCtrl::OnRefreshForNewTestValue(int iTestType)
{
	unsigned int sn = 0x0;
	matrix_data::data_base_helper::const_device_info_map_reference map = helper_->get_devices_info();
	matrix_data::data_base_helper::device_info_map::const_iterator itr;
	CBCGPGridRow* pRow = NULL;

	helper_->devices_info_lock();
	if(map.size() > 0){
		for(itr = map.begin(); itr != map.end(); ++itr){
			pRow = FindRowByData(itr->second.sn_, FALSE);
			if(pRow != NULL){ 
				SetRowDataOfTestByTestType(pRow, itr->second, iTestType);
				SetRowDataOfTestByTestType(pRow, itr->second, 30);
			}
		}
	}
	helper_->devices_info_unlock();
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

	strColumn = "CMRR";
	InsertColumn (nColumns, strColumn, 80);
	nColumns++;

	//strColumn = "Rout IP";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Direction";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Rout Top";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Rout Down";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Rout Left";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Rout Right";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Time Head";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Time Delay";
	//InsertColumn (nColumns, strColumn, 80);
	//nColumns++;

	//strColumn = "Jump Channel";
	//InsertColumn (nColumns, strColumn, 100);
	//nColumns++;

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
* @param matrix_data::data_base_helper::device_info& di 仪器引用
* @return void
*/
void CInstrumentListCtrl::SetRowData(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di)
{
	SetRowDataOfBase(pRow, di);
	SetRowDataOfTest(pRow, di);
}

/**
* 设置一行的基本数据
* @param CBCGPGridRow* pRow 行指针
* @param matrix_data::data_base_helper::device_info& di 仪器引用
* @return void
*/
void CInstrumentListCtrl::SetRowDataOfBase(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di)
{
	pRow->SetData(di.sn_);

	unsigned int nItems = 0;
	pRow->GetItem(nItems)->SetValue(di.sn_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(di.line_index_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(di.m_uiPointNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(1, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetInstrumentTypeStringByInstrumentType(di.sn_), FALSE);
	nItems++;

	pRow->GetItem(nItems)->SetValue(di.ip_, FALSE);
	nItems = 6;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bJumpedChannel), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bSensor), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bBlastMachine), FALSE);

	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bAux), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bDetour), FALSE);
	nItems++;

	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bDetourMarkerLow), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bDetourMarkerHigh), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bMarker), FALSE);
	nItems++;
}

/**
* 设置一行的测试数据
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentListCtrl::SetRowDataOfTest(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di)
{
	SetRowDataOfTestByTestType(pRow, di, 30);
	SetRowDataOfTestByTestType(pRow, di, 4);
	SetRowDataOfTestByTestType(pRow, di, 5);
	SetRowDataOfTestByTestType(pRow, di, 7);
	SetRowDataOfTestByTestType(pRow, di, 6);
	SetRowDataOfTestByTestType(pRow, di, 8);
}

/**
* 设置一行的测试数据，依据测试类型
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentListCtrl::SetRowDataOfTestByTestType(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di, int iTestType)
{
	unsigned int nItems = 0;
	
	switch(iTestType)
	{
	case 4:	// 仪器噪声
		nItems = 7;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.m_byNoiseCheck, di.m_fNoiseValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.m_byNoiseCheck));
		break;
	case 5:	// 仪器畸变
		nItems = 8;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.m_byDistortionCheck, di.m_fDistortionValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.m_byDistortionCheck));
		break;
	case 7:	// 仪器串音
		nItems = 9;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.m_byCrosstalkCheck, di.m_fCrosstalkValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.m_byCrosstalkCheck));
		break;
	case 6:	// 仪器增益/相位
		nItems = 10;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.m_byGainCheck, di.m_fGainValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.m_byGainCheck));
		nItems++;
		break;
	case 8:	// 仪器共模
		nItems = 11;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.m_byCMRRCheck, di.m_fCMRRValue), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.m_byCMRRCheck));
		break;
	case 30:	// 所有仪器测试
		nItems = 6;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.m_byCheck), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.m_byCheck));
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
COleVariant CInstrumentListCtrl::GetInstrumentTypeStringByInstrumentType(unsigned int sn)
{
	COleVariant oVariant;
	CString strValue = "";
	// 判断方向
	switch(sn & 0x3)
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