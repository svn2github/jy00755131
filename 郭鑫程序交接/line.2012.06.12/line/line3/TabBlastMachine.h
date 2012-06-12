#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabBlastMachine Tab属性页爆炸机控制器设置类
*/
class CTabBlastMachine : public CTabPage
{
	DECLARE_DYNAMIC(CTabBlastMachine)

public:
	CTabBlastMachine();
	virtual ~CTabBlastMachine();

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
	// 初始化BoxType单元格
	void InitBoxTypeGridItem(CBCGPGridItem* pItem);
	// 初始化Gain单元格
	void InitGainGridItem(CBCGPGridItem* pItem);
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
};