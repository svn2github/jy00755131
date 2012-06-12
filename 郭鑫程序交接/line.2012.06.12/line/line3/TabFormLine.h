#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabFormLine Tab属性页FormLine设置类
*/
class CTabFormLine : public CTabPage
{
	DECLARE_DYNAMIC(CTabFormLine)

public:
	CTabFormLine();
	virtual ~CTabFormLine();

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
	/** FormLine数据*/
	CFormLineData m_oFormLineData;
	/** 输入行*/
	CGridCtrlEdit m_oGridCtrlEdit;
	/** 列表*/
	CGridCtrlList m_oGridCtrlList;
	/** 索引号队列*/
	CList<int, int> m_olsNb;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 初始化BoxType单元格
	void InitBoxTypeGridItem(CBCGPGridItem* pItem);
	// 初始化Orientation单元格
	void InitOrientationGridItem(CBCGPGridItem* pItem);
	// 响应鼠标单击按钮；Apply
	void OnBnClickedButtonApply();
	// 响应鼠标单击按钮；GO
	bool OnBnClickedButtonGO();
	// 响应鼠标单击按钮；Reset
	void OnBnClickedButtonReset();
	// 响应鼠标单击按钮；Add
	void OnBnClickedButtonAdd();
	// 响应鼠标单击按钮；Change
	void OnBnClickedButtonChange();
	// 响应鼠标单击按钮；Delete
	void OnBnClickedButtonDelete();
	// 响应仪器图形视图弹出菜单FormLine设置
	void OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN);
};