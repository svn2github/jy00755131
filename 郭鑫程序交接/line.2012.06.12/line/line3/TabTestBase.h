#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabTestBase Tab属性页TestBase设置类
*/
class CTabTestBase : public CTabPage
{
	DECLARE_DYNAMIC(CTabTestBase)

public:
	CTabTestBase();
	virtual ~CTabTestBase();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnGridCtrlListLButtonDown();	// 鼠标单击列表
	afx_msg void OnGridCtrlListDblClk();	// 鼠标单击双击列表

public:	// 属性
	/** 现场数据*/
	CSiteData* m_pSiteData;
	/** TestBase数据*/
	CTestBaseData m_oTestBaseData;
	/** 输入行*/
	CGridCtrlEdit m_oGridCtrlEdit;
	/** 列表*/
	CGridCtrlList m_oGridCtrlList;
	/** 索引号队列*/
	CList<int, int> m_olsNb;

	/** 测试对象 1-仪器；2-检波器*/
	unsigned int m_uiAim;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 初始化Gain单元格
	void InitGainGridItem(CBCGPGridItem* pItem);
	// 初始化FilterType单元格
	void InitFilterTypeGridItem(CBCGPGridItem* pItem);
	// 初始化ADCType单元格
	void InitADCTypeGridItem(CBCGPGridItem* pItem);
	// 初始化DACType单元格
	void InitDACTypeGridItem(CBCGPGridItem* pItem);
	// 响应鼠标单击按钮；Apply
	void OnBnClickedButtonApply();
	// 响应鼠标单击按钮；Reset
	void OnBnClickedButtonReset();
	// 响应鼠标单击按钮；Change
	void OnBnClickedButtonChange();
};