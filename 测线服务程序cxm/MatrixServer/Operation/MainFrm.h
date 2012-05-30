// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "ActiveSourceBar.h"
#include "AllVPBar.h"
#include "VPDoneBar.h"
#include "VPToDoBar.h"
#include "OutputBar.h"
class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
	CAllVPBar				m_wndAllVP;
	CVPDoneBar				m_wndVPDone;
	CVPToDoBar				m_wndVPToDo;
	COutputBar				m_wndOutput;
public:
	CActiveSourceBar		m_wndActiveSource;
	CBCGPStatusBar			m_wndStatusBar;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnSelectActiveSource(UINT id);
	afx_msg void OnConnectServer(CCmdUI* pCmdUI);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	BOOL CreateStatusBar ();
};


