#pragma once

// CGridCtrlList

class CGridCtrlList : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CGridCtrlList)

public:
	CGridCtrlList();
	virtual ~CGridCtrlList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:	// 属性
	int m_iRowHeightAdd;	// 行附加高度

public:	// 方法
	virtual void SetRowHeight();
};


