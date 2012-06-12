#pragma once

#include "MainFrm.h"
#include "SiteData.h"

#include "GraphViewTopToolBarInstrument.h"
#include "GraphViewBottomToolBar.h"
#include "LeftViewBarInstrumentTop.h"
#include "LeftViewBarInstrumentBottom.h"
#include "ToolbarLabel.h"
#include "ToolbarGraph.h"
#include "ToolbarImage.h"

// CInstrumentGraphChildFrame 框架

class CInstrumentGraphChildFrame : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(CInstrumentGraphChildFrame)
protected:
	CInstrumentGraphChildFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CInstrumentGraphChildFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnShowLineNbPointNb();
	afx_msg void OnShowLinkLine();
	afx_msg void OnShowInstrumentCross();
	afx_msg void OnShowInstrumentPower();
	afx_msg void OnShowInstrumentCollect();
	afx_msg void OnLook();
	afx_msg void OnSelectTestType();
	afx_msg void OnTestGo();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
protected:

public:	// 属性
	/** 现场数据对象*/
	//CSiteData*	m_pSiteData;
	matrix_data::data_base_helper* data_base_helper_;

	/**测试类型*/
	unsigned int m_uiTestType;
	/**测试类型*/
	CString m_strTestType;
	/**是否显示测区坐标*/
	bool m_bShowLineNbPointNb;
	/**是否显示连接线*/
	bool m_bShowLinkLine;
	/**是否显示交叉站*/
	bool m_bShowInstrumentCross;
	/**是否显示电源站*/
	bool m_bShowInstrumentPower;
	/**是否显示采集站*/
	bool m_bShowInstrumentCollect;

	CGraphViewTopToolBarInstrument m_oGraphViewTopToolBar;	// 上工具条
	CGraphViewBottomToolBar m_oGraphViewBottomToolBar;	// 下工具条
	CLeftViewBarInstrumentTop m_oLeftViewBarInstrumentTop;	// 左导航条上部
	CLeftViewBarInstrumentBottom m_oLeftViewBarInstrumentBottom;	// 左导航条下部

public:	// 方法
	// 初始化下拉框按钮测试类型下拉框
	void InitButtonForTestTyperComboBox(CBCGPToolbarComboBoxButton* pComboButton);

	// 显示单元的测线号和测点号
	void OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 响应导航局部放大
	void OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY);
	// 响应显示属性值
	void OnShowProperty(unsigned int uiIndex);
	// 响应显示迂回道图形视图
	void OnShowDetourGraphView(unsigned int uiCount);
	// 响应设置测线数量
	void OnSetLineCount(unsigned int uiLineCount);
};


