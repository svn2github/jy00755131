#pragma once

#include "MainFrm.h"
#include "SiteData.h"

/**
*@brief 检波器视图左侧子视图下部类
*/
class CLeftViewBarSensorBottom : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CLeftViewBarSensorBottom)

public:
	CLeftViewBarSensorBottom();
	virtual ~CLeftViewBarSensorBottom();

protected:
	//{{AFX_MSG(CLeftViewBarSensorBottom)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	// 属性
	CSiteData*	m_pSiteData;	// 现场数据对象

	/** 属性表字体*/
	CFont m_fntPropList;
	/** 属性表*/
	CBCGPPropList m_wndPropList;
	
public:	// 方法
	// 初始化属性表字体
	void SetPropListFont();
	// 初始化属性表
	void InitPropList();
	// 重置
	void OnResetBar();
	// 响应显示属性值
	void OnShowProperty(unsigned int uiIndex);
	// 得到测试结果文字
	COleVariant GetTestResultString(byte byValue);
};


