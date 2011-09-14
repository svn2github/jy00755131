#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabTestSeisMonitor Tab属性页检波器测试设置类
*/
class CTabTestSeisMonitor : public CTabPage
{
	DECLARE_DYNAMIC(CTabTestSeisMonitor)

public:
	CTabTestSeisMonitor();
	virtual ~CTabTestSeisMonitor();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

public:	// 属性
	/** 现场数据*/
	CSiteData* m_pSiteData;
	/** 绝对排列组框*/
	CButton m_oStaticAbsoluteSpread;
	/** 绝对排列编辑框*/
	CEdit m_oEditAbsoluteSpread;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 初始化TestType单元格
	void InitTestTypeGridItem(CBCGPGridItem* pItem);
	// 响应鼠标单击按钮；Apply
	bool OnBnClickedButtonApply();
	// 响应鼠标单击按钮；Reset
	void OnBnClickedButtonReset();
	// 响应鼠标单击按钮；GO
	bool OnBnClickedButtonGO();
	// 响应鼠标单击按钮；Add
	void OnBnClickedButtonAdd();
	// 响应鼠标单击按钮；Change
	void OnBnClickedButtonChange();
	// 响应鼠标单击按钮；Delete
	void OnBnClickedButtonDelete();
	// 响应鼠标单击按钮；Clear
	void OnBnClickedButtonClear();
};