#pragma once


// CGridCtrlEdit

class CGridCtrlEdit : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CGridCtrlEdit)

public:
	CGridCtrlEdit();
	virtual ~CGridCtrlEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:	// 属性
	int m_iRowHeightAdd;	// 行附加高度

public:	// 方法
	virtual void SetRowHeight();
};


