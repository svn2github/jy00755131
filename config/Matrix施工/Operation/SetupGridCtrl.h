#pragma once

#define		WM_SETUPGRID_DBCLICK		WM_USER+300
#define		WM_SETUPGRID_CLICK			WM_USER+301

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// CGridCtrlEdit /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	bool   IsColumnEmpty(void);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// CGridCtrlList ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg LRESULT  OnGridItemDblClk(WPARAM wParam, LPARAM lParam);
	void	CreateGrid(CRect& rc,CWnd* pParent);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////CGridCtrlOperation/////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
class CGridCtrlOperation
{
public:
	static void  OnGridItemClick(CGridCtrlEdit* pGridEdit,CGridCtrlList* pGridList);
	static void  OnGridItemDblClk(CGridCtrlEdit* pGridEdit,CGridCtrlList* pGridList);

	static void  OnBnClickedButtonAddA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	static void  OnBnClickedButtonAddB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	static void  OnBnClickedButtonDeleteA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	static void  OnBnClickedButtonChangeA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	static void  OnBnClickedButtonChangeB(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
	static void  OnBnClickedButtonReverseA(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList);
private:
	
	static COleVariant GetValueForTextField(COleVariant* pEditVariant,COleVariant* pListVariant,int nIndex);
	static bool  GetIndexListForAdd(CBCGPGridCtrl* pGridCtrlEdit, CList<int, int>* pListNb);
	static bool  GetIndexListForChangeOrDeleteOrReverse(CBCGPGridCtrl* pGridCtrlEdit, CBCGPGridCtrl* pGridCtrlList, CList<int, int>* pListNb);
};