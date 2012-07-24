// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "ActiveSourceBar.h"
#include "AllVPBar.h"
#include "VPDoneBar.h"
#include "VPToDoBar.h"
#include "ActiveAcqBar.h"
#include "OutputBar.h"
#include "..\\MatrixCommDll\\MatrixCommDll.h"
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
	CBCGPToolBar			m_wndToolBarSetup;
	CBCGPToolBar			m_wndToolBarView;
	COutputBar				m_wndOutput;
	CImageList				m_imagesBarView;
public:
	CActiveSourceBar		m_wndActiveSource;
	CAllVPBar				m_wndAllVP;
	CVPDoneBar				m_wndVPDone;
	CVPToDoBar				m_wndVPToDo;
	CActiveAcqBar			m_wndActiveAcq;
	CBCGPStatusBar			m_wndStatusBar;
	CBitmap					m_bmpIconConnected;
	CBitmap					m_bmpIconDisConnected;
	bool					m_bServerConnected;
public:
	// 通讯类指针
	CMatrixCommDll*	m_pMatrixCommDll;
	// 客户端通讯类指针
	CCommClient* m_pCommClient;
	// Dll句柄
	HINSTANCE m_hCommDll;
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
	// 创建客户端通讯
	void OnCreateClientComm();
	// 释放客户端通讯
	void OnDeleteClientComm();
public:
	afx_msg void OnDestroy();
};


