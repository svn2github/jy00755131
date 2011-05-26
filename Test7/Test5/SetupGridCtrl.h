#pragma once


//////////////////////////////////////////////////////////////////////////
////////////////////////// CGridCtrlEdit ///////////////////////////
//////////////////////////////////////////////////////////////////////////
class CGridCtrlEdit : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CGridCtrlEdit)

public:
	CGridCtrlEdit();
	virtual ~CGridCtrlEdit();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void SetRowHeight(void);
public:
	void   CreateGrid(CRect& rc,CWnd* pParent);
};


//////////////////////////////////////////////////////////////////////////
////////////////////////// CGridCtrlList ///////////////////////////
//////////////////////////////////////////////////////////////////////////
class CGridCtrlList : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CGridCtrlList)

public:
	CGridCtrlList();
	virtual ~CGridCtrlList();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void SetRowHeight(void);
public:
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg LRESULT  OnGridItemDblClk(WPARAM wParam, LPARAM lParam);
	void	CreateGrid(CRect& rc,CWnd* pParent);
};
