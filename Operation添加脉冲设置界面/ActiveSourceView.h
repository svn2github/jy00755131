#pragma once
#include "afxwin.h"
#include "bcgpdockingcontrolbar.h"
#include "Shotsources.h"

/**
 * @class CActiveSourceGridCtrl
 * @brief 显示放炮震源的网格
 *
 * 显示用于放炮的震源表格，该表格应用于Active Source子视图中。
 */
class CActiveSourceGridCtrl : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CActiveSourceGridCtrl)

public:
	CActiveSourceGridCtrl();
	virtual ~CActiveSourceGridCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnChangeSelection(CBCGPGridRow* pNewRow,CBCGPGridRow* pOldRow);
	virtual void SetRowHeight(void);
};



// CActiveSourceView 窗体视图

class CActiveSourceView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CActiveSourceView)
public:
	/** 显示震源的网格 */
	CActiveSourceGridCtrl	m_wndGrid;
	CButton m_btnGo;
	CButton m_btnStop;
	CButton m_btnAbort;

private:	
	CImageList		m_ImageList;
protected:
	CActiveSourceView();           // 动态创建所使用的受保护的构造函数
	virtual ~CActiveSourceView();

public:
	enum { IDD = IDD_ACTIVESOURCEVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:	
	BOOL CreateView(CWnd* pParent, CCreateContext* pContext);
	bool SetGridHead(void);
	virtual void OnInitialUpdate();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();	
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedButtongo();
};
/******************************************************************************
    类    型：  
    
    功    能：

    修改历史：

*******************************************************************************/
class CActiveSourceWnd :
	public CBCGPDockingControlBar
{
public:
	CActiveSourceWnd(void);
	~CActiveSourceWnd(void);
	// Attributes
protected:
	CActiveSourceView* m_pFormView;
	CCreateContext* m_pContext;
	
	// Operations
public:
	void SetContext (CCreateContext* pContext)
	{
		m_pContext = pContext;
	}
	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadShotSource(CShotSource* pShotSrc);
public:
	virtual BOOL Create(LPCTSTR lpszCaption,CWnd*pParentWnd,const RECT&rect,BOOL bHasGripper,UINT nID,DWORD dwStyle,DWORD dwTabbedStyle=CBRS_BCGP_REGULAR_TABS,DWORD dwBCGStyle=dwDefaultBCGDockingBarStyle,CCreateContext*pContext=NULL);
	void LoadShotSources(CShotSources* pShotSrcs);
	CBCGPGridCtrl* GetGridCtrl(void);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	int LoadShotPointBySource(CShotSource* pSource, CShotPoint* pShotPoint);
};

