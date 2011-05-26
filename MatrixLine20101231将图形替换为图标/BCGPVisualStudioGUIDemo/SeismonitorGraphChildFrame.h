#pragma once

#include "MainFrm.h"
#include "SiteData.h"

#include "GraphViewTopToolBarSeismonitor.h"
#include "GraphViewBottomToolBar.h"
#include "LeftViewBarSeismonitorTop.h"
#include "LeftViewBarSensorBottom.h"
#include "ToolbarLabel.h"
#include "ToolbarGraph.h"
#include "ToolbarImage.h"
#include "ToolbarGraphButton.h"
#include "BCGPSliderButton.h"

// CSeismonitorGraphChildFrame 框架

class CSeismonitorGraphChildFrame : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(CSeismonitorGraphChildFrame)
protected:
	CSeismonitorGraphChildFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CSeismonitorGraphChildFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);	
	afx_msg void OnShowSensorNb(UINT nID);
	afx_msg void OnSelectSensorNb();
	afx_msg void OnSliderTestLimit();
	afx_msg void OnTestSeismonitor();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();

	afx_msg void OnUpdateUITestSeismonitor(CCmdUI* pCmdUI);

public:	// 属性
	/** 现场数据对象*/
	CSiteData*	m_pSiteData;

	/** 检波器Nb数量*/
	unsigned int m_uiSensorNbCount;
	/** 要显示测试结果的检波器Nb*/
	unsigned int m_uiSensorNb;

	CMap<unsigned int, unsigned int, unsigned int, unsigned int> m_oSensorNbMap;

	/**测试类型*/
	unsigned int m_uiTestType;
	/**噪声监测判据*/
	float m_fSeismonitorLimit;
	/**噪声监测判据*/
	CString m_strSeismonitorLimitA;
	/**噪声监测判据*/
	CString m_strSeismonitorLimitB;
	/** 野外噪声监测操作 1-启动测试；0-停止测试*/
	byte m_byTestOperation;

	CGraphViewTopToolBarSeismonitor m_oGraphViewTopToolBar;	// 上工具条
	CGraphViewBottomToolBar m_oGraphViewBottomToolBar;	// 下工具条
	CLeftViewBarSeismonitorTop m_oLeftViewBarSeismonitorTop;	// 左导航条上部
	CLeftViewBarSensorBottom m_oLeftViewBarSensorBottom;	// 左导航条下部

public:	// 方法
	// 初始化下拉框按检波器Nb下拉框
	void InitButtonForSensorNbComboBox(CBCGPToolbarComboBoxButton* pComboButton);

	// 显示单元的测线号和测点号
	void OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 响应导航局部放大
	void OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY);
	// 响应显示属性值
	void OnShowProperty(unsigned int uiIndex);
	// 响应设置测线数量
	void OnSetLineCount(unsigned int uiLineCount);
	// 得到噪声监测判据字符串
	void SetSeismonitorLimitString();
};


