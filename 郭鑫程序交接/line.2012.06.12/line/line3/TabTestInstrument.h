#pragma once

#include "GridCtrlEdit.h"
#include "GridCtrlList.h"
#include "SiteData.h"
#include "TabPage.h"

/**
CTabTestInstrument Tab属性页仪器测试设置类
*/
class CTabTestInstrument : public CTabPage
{
	DECLARE_DYNAMIC(CTabTestInstrument)

public:
	CTabTestInstrument();
	virtual ~CTabTestInstrument();

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
	/** 辅助道组框*/
	CButton m_oStaticAuxiliaryDescr;
	/** 绝对排列组框*/
	CButton m_oStaticAbsoluteSpread;
	/** 辅助道编辑框*/
	CEdit m_oEditAuxiliaryDescr;
	/** 绝对排列编辑框*/
	CEdit m_oEditAbsoluteSpread;

	/** 索引号队列*/
	CList<int, int> m_olsNb;
	/** 索引号*/
	unsigned int m_uiTestNb;

public:	// 方法
	// 生成编辑界面
	void OnCreateCtrl(LPCREATESTRUCT lpCreateStruct);
	// 初始化TestType单元格
	void InitTestTypeGridItem(CBCGPGridItem* pItem);
	// 初始化Gain单元格
	void InitGainGridItem(CBCGPGridItem* pItem);
	// 响应鼠标单击按钮；Apply
	void OnBnClickedButtonApply();
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