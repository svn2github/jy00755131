#pragma once

#include "MainFrm.h"
#include "SiteData.h"

#include "GraphViewTopToolBarSensor.h"
#include "GraphViewBottomToolBar.h"
#include "LeftViewBarSensorTop.h"
#include "LeftViewBarSensorBottom.h"
#include "ToolbarLabel.h"
#include "ToolbarGraph.h"
#include "ToolbarImage.h"
#include "ToolbarGraphButton.h"

// CSensorGraphChildFrame 框架

class CSensorGraphChildFrame : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(CSensorGraphChildFrame)
protected:
	CSensorGraphChildFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CSensorGraphChildFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);	
	afx_msg void OnShowSensorNb(UINT nID);
	afx_msg void OnLook();
	afx_msg void OnSelectSensorNb();
	afx_msg void OnSelectTestType();
	afx_msg void OnTestGo();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();

public:	// 属性
	/** 现场数据对象*/
//	CSiteData*	m_pSiteData;
	matrix_data::data_base_helper* data_base_helper_;

	/** 检波器Nb数量*/
	unsigned int m_uiSensorNbCount;
	/** 要显示测试结果的检波器Nb*/
	unsigned int m_uiSensorNb;

	CMap<unsigned int, unsigned int, unsigned int, unsigned int> m_oSensorNbMap;

	/**测试类型*/
	unsigned int m_uiTestType;
	/**测试类型*/
	CString m_strTestType;

	CGraphViewTopToolBarSensor m_oGraphViewTopToolBar;	// 上工具条
	CGraphViewBottomToolBar m_oGraphViewBottomToolBar;	// 下工具条
	CLeftViewBarSensorTop m_oLeftViewBarSensorTop;	// 左导航条上部
	CLeftViewBarSensorBottom m_oLeftViewBarSensorBottom;	// 左导航条下部

public:	// 方法
	// 初始化下拉框按钮测试类型下拉框
	void InitButtonForTestTypeComboBox(CBCGPToolbarComboBoxButton* pComboButton);
	// 初始化下拉框按检波器Nb下拉框
	void InitButtonForSensorNbComboBox(CBCGPToolbarComboBoxButton* pComboButton);

	// 显示单元的测线号和测点号
	void OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 响应导航局部放大
	void OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY);
	// 响应显示属性值
	void OnShowProperty(matrix_data::data_base_helper::const_device_info& di);
	// 响应设置测线数量
	void OnSetLineCount(unsigned int uiLineCount);
};


