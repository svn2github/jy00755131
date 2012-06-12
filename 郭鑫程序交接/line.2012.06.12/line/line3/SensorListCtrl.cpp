// SensorListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "SensorListCtrl.h"

// CSensorListCtrl

IMPLEMENT_DYNAMIC(CSensorListCtrl, CBCGPGridCtrl)

CSensorListCtrl::CSensorListCtrl():data_base_helper_(NULL)
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
	data_base_helper_ = ((CLineApp*)AfxGetApp())->get_data_base_helper();
}

// 加载现场测道数据队列
void CSensorListCtrl::OnLoadSiteDataAndFillToChannelList()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 生成表格头
	CreateColumTitle();

	matrix_data::data_base_helper::device_info_map& map = data_base_helper_->get_devices_info();
	matrix_data::data_base_helper::device_info_map::const_iterator itr;
	CBCGPGridRow* pRow = NULL;

	data_base_helper_->devices_info_lock();
	for(itr = map.begin(); itr != map.end(); ++itr){
		pRow = CreateRow(GetColumnCount());
		SetRowData(pRow, itr->second);
		AddRow(pRow, FALSE);
	}
	data_base_helper_->devices_info_unlock();
	AdjustLayout ();
}

// 重新加载现场测道数据
void CSensorListCtrl::OnReloadSiteDataCMD()
{
	// 现场数据对象初始化
	OnSiteDataInit();

	matrix_data::data_base_helper::device_info_map& map = data_base_helper_->get_devices_info();
	matrix_data::data_base_helper::device_info_map::const_iterator itr;
	matrix_data::data_base_helper::device_info di;
	CBCGPGridRow* pRow = NULL;

	data_base_helper_->devices_info_lock();
	for(itr = map.begin(); itr != map.end(); ++itr){

		pRow = FindRowByData(itr->second.ip_, FALSE);
		// 填充本次数据
		if(pRow != NULL){
			SetRowData(pRow, itr->second);	// 设置一行的数据
		}
		else{
			pRow = CreateRow(GetColumnCount());
			SetRowData(pRow, itr->second);
			AddRow(pRow, FALSE);
		}
	}
	// 删除没有用到的行
	for(int i = (GetRowCount() - 1); i >= 0; i--){
		pRow = GetRow(i);
		if(pRow != NULL){	// 找到行
			di.ip_ = pRow->GetData();
			if(data_base_helper_->find_device(di.ip_, di))	RemoveRow(i, FALSE);
		}
	}
	data_base_helper_->devices_info_unlock();
	AdjustLayout ();
}

// 为新的测试数据刷新显示
void CSensorListCtrl::OnRefreshForNewTestValue(int iTestType)
{
	CBCGPGridRow* pRow = NULL;
	matrix_data::data_base_helper::device_info_map map = data_base_helper_->get_devices_info();
	matrix_data::data_base_helper::device_info_map::const_iterator itr;
	for(itr = map.begin(); itr != map.end(); ++itr){
		pRow = FindRowByData(itr->second.ip_, FALSE);
		if(pRow != NULL){	// 找到行
			SetRowDataOfTestByTestType(pRow, itr->second, iTestType);
			// 判断：不是新加检波器的初始测试
			if(iTestType != 45)		SetRowDataOfTestByTestType(pRow, itr->second, 40);
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
void CSensorListCtrl::SetRowData(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di)
{
	SetRowDataOfBase(pRow, di);
	SetRowDataOfTest(pRow, di);
}

/**
* 设置一行的基本数据
* @param CBCGPGridRow* pRow 行指针
* @param CChannelSetupData* pChannelSetupData 测道指针
* @return void
*/
void CSensorListCtrl::SetRowDataOfBase(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di)
{
	pRow->SetData(di.ip_);

	matrix_data::data_base_helper::sensor_type st;
	st.nb_ = di.sensors_.begin()->type_;
	data_base_helper_->find_sensor_type(st);

	int nItems = 0;
	pRow->GetItem(nItems)->SetValue(di.sn_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(di.line_index_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(di.m_uiPointNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(di.m_uiLineNb, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(st.nb_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(st.segd_code_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(di.ip_, FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bJumpedChannel), FALSE);
	nItems++;
	pRow->GetItem(nItems)->SetValue(GetMarkerStringByLogic(di.m_bMute), FALSE);
}

/**
* 设置一行的测试数据
* @param CBCGPGridRow* pRow 行指针
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CSensorListCtrl::SetRowDataOfTest(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di)
{
	SetRowDataOfTestByTestType(pRow, di, 40);
	SetRowDataOfTestByTestType(pRow, di, 45);
	SetRowDataOfTestByTestType(pRow, di, 15);
	SetRowDataOfTestByTestType(pRow, di, 14);
	SetRowDataOfTestByTestType(pRow, di, 1);
	SetRowDataOfTestByTestType(pRow, di, 20);
	SetRowDataOfTestByTestType(pRow, di, 21);
}

/**
* 设置一行的测试数据，依据测试类型
* @param CBCGPGridRow* pRow 行指针
* @param CChannelSetupData* pChannelSetupData 测道指针
* @return void
*/
void CSensorListCtrl::SetRowDataOfTestByTestType(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di, int iTestType)
{
	unsigned int nItems = 0;
	switch(iTestType)
	{
		// 野外电阻
	case 15:
		nItems = 11;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.sensors_[0].resistance_check_, di.sensors_[0].resistance_), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.sensors_[0].resistance_check_));
		break;
		// 野外漏电
	case 14:
		nItems = 12;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.sensors_[0].leakage_check_, di.sensors_[0].leakage_), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.sensors_[0].leakage_check_));
		break;
		// 野外噪声
	case 1:
		nItems = 13;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.sensors_[0].noise_check_, di.sensors_[0].noise_), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.sensors_[0].noise_check_));
		break;
		// 野外倾斜度
	case 20:
		nItems = 14;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.sensors_[0].tilt_check_, di.sensors_[0].tilt_), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.sensors_[0].tilt_check_));
		break;
		// 野外倾斜度模型
	case 21:
		nItems = 15;
		pRow->GetItem(nItems)->SetValue(GetTestResultString(di.sensors_[0].tilt_model_check_, di.sensors_[0].tilt_model_), FALSE);
		pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(di.sensors_[0].tilt_model_check_));
		break;
		// 全部野外测试
	//case 40:
	//	nItems = 9;
	//	pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byCheck), FALSE);
	//	pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byCheck));
	//	break;
	//	// 新加检波器的初始测试
	//case 45:
	//	nItems = 10;
	//	pRow->GetItem(nItems)->SetValue(GetTestResultString(pChannelSetupData->m_byOnFieldCheck), FALSE);
	//	pRow->GetItem(nItems)->SetBackgroundColor(GetColourByCheck(pChannelSetupData->m_byOnFieldCheck));
	//	break;
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