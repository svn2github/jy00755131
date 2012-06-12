#pragma once

#include "MainFrm.h"
#include "WndInstrument.h"

/**
*@brief 仪器视图左侧子视图下部类
*/
class CLeftViewBarInstrumentBottom : public CMyBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CLeftViewBarInstrumentBottom)

public:
	CLeftViewBarInstrumentBottom();
	virtual ~CLeftViewBarInstrumentBottom();

protected:
	//{{AFX_MSG(CLeftViewBarInstrumentBottom)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	// 属性
	/** 属性表字体*/
	CFont m_fntPropList;
	/** 属性表*/
	CBCGPPropList m_wndPropList;
	/** 迂回道图形视图*/
	CWndInstrument m_wndDetourGraphView;
	/** 迂回道图形视图滚动条*/
	CScrollBar m_wndDetourScrollBar;
	
public:	// 方法
	// 初始化属性表字体
	void SetPropListFont();
	// 初始化属性表
	void InitPropList();
	// 重置
	void OnResetBar();
	// 响应显示属性值
	void OnShowProperty(matrix_data::data_base_helper::device_info& di);
	// 响应显示迂回道图形视图
	void OnShowDetourGraphView(unsigned int uiCount);
	// 得到标记文字，依据逻辑值
	COleVariant GetMarkerStringByLogic(bool bValue);
};


