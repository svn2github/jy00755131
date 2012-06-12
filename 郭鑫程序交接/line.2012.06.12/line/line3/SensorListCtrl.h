#pragma once

#include "MainFrm.h"
#include "SiteData.h"

// CSensorListCtrl 视图

class CSensorListCtrl : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CSensorListCtrl)

public:
	CSensorListCtrl();
	virtual ~CSensorListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

public: // 属性
	//CSiteData*	m_pSiteData;	// 现场数据对象
	matrix_data::data_base_helper* data_base_helper_; 
public: // 方法
	// 现场数据对象初始化
	void OnSiteDataInit();
	// 加载现场测道数据队列
	void OnLoadSiteDataAndFillToChannelList();
	// 重新加载现场测道数据
	void OnReloadSiteDataCMD();
	// 为新的测试数据刷新显示
	void OnRefreshForNewTestValue(int iTestType);

	// 生成表格头
	void CreateColumTitle();
	// 设置一行的数据
	void SetRowData(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di);
	// 设置一行的基本数据
	void SetRowDataOfBase(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di);
	// 设置一行的测试数据
	void SetRowDataOfTest(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di);
	// 设置一行的测试数据，依据测试类型
	void SetRowDataOfTestByTestType(CBCGPGridRow* pRow, matrix_data::data_base_helper::const_device_info_reference di, int iTestType);
	// 得到背景颜色，依据测试检查结果
	COLORREF GetColourByCheck(byte byCheck);
	// 得到标记文字，依据逻辑值
	COleVariant GetMarkerStringByLogic(bool bValue);
	// 得到测试结果文字
	COleVariant GetTestResultString(byte byValue, float m_fValue);
	// 得到测试结果文字
	COleVariant GetTestResultString(byte byValue);
};


