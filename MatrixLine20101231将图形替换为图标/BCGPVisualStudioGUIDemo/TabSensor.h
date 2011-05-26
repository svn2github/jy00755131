#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabSensor Tab属性页检波器设置类
*/
class CTabSensor : public CTabPage
{
	DECLARE_DYNAMIC(CTabSensor)

public:
	CTabSensor();
	virtual ~CTabSensor();

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
	/** 输入行*/
	CGridCtrlEdit m_oGridCtrlEdit;
	/** 列表*/
	CGridCtrlList m_oGridCtrlList;
	/** 索引号队列*/
	CList<int, int> m_olsNb;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 初始化SegdCode单元格
	void InitSegdCodeGridItem(CBCGPGridItem* pItem);
	// 响应鼠标单击按钮；ApplyAll
	void OnBnClickedButtonApplyAll();
	// 响应鼠标单击按钮；ApplySensor
	void OnBnClickedButtonApplySensor();
	// 响应鼠标单击按钮；Reset
	void OnBnClickedButtonReset();
	// 响应鼠标单击按钮；Add
	void OnBnClickedButtonAdd();
	// 响应鼠标单击按钮；Change
	void OnBnClickedButtonChange();
	// 响应鼠标单击按钮；Delete
	void OnBnClickedButtonDelete();
	// 响应鼠标单击按钮；Reverse
	void OnBnClickedButtonReverse();
};