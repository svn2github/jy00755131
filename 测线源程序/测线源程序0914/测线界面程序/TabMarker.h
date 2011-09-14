#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabMarker Tab属性页标记点设置类
*/
class CTabMarker : public CTabPage
{
	DECLARE_DYNAMIC(CTabMarker)

public:
	CTabMarker();
	virtual ~CTabMarker();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	// 鼠标单击列表
	afx_msg void OnGridCtrlListLButtonDown();
	// 鼠标单击双击列表
	afx_msg void OnGridCtrlListDblClk();

public:	// 属性
	/** 现场数据*/
	CSiteData* m_pSiteData;
	/** 输入行*/
	CGridCtrlEdit m_oGridCtrlEdit;
	/** 列表*/
	CGridCtrlList m_oGridCtrlList;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 初始化BoxType单元格
	void InitBoxTypeGridItem(CBCGPGridItem* pItem);

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
	// 响应鼠标单击按钮；Clear
	void OnBnClickedButtonClear();
	// 响应仪器图形视图弹出菜单设置标记点
	void OnGraphViewInstrumentSetMarker(unsigned int uiInstrumentType, unsigned int uiSN);
};