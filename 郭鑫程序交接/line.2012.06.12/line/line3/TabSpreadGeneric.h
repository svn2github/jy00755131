#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabSpreadGeneric Tab属性页绝对排列设置类
*/
class CTabSpreadGeneric : public CTabPage
{
	DECLARE_DYNAMIC(CTabSpreadGeneric)

public:
	CTabSpreadGeneric();
	virtual ~CTabSpreadGeneric();

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

	/** 属性页容器*/
	CBCGPTabWnd m_oTabWnd;
	/** 测线编辑框*/
	CEdit m_oEditLine;
	/** 排列编辑框*/
	CEdit m_oEditSpread;

	/** 索引号队列*/
	CList<int, int> m_olsNb;
	/** 索引号*/
	unsigned int m_uiTestNb;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 响应鼠标单击按钮；Apply
	void OnBnClickedButtonApply();
	// 响应鼠标单击按钮；Reset
	void OnBnClickedButtonReset();
	// 响应鼠标单击按钮；Add
	void OnBnClickedButtonAdd();
	// 响应鼠标单击按钮；Change
	void OnBnClickedButtonChange();
	// 响应鼠标单击按钮；Delete
	void OnBnClickedButtonDelete();
};