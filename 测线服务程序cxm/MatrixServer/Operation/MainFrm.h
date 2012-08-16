// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "ActiveSourceBar.h"
#include "AllVPBar.h"
#include "VPDoneBar.h"
#include "VPToDoBar.h"
#include "ActiveAcqBar.h"
#include "OutputBar.h"
#include "CommDll.h"
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
	/** 与客户端通讯类成员*/
	CCommDll m_oComDll;
	/** 服务器监听端口*/
	unsigned int m_uiServerPort;
	/** 服务器IP*/
	CString m_strServerIP;
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
	/**
	* @fn void OnInitSocketLib(void)
	* @detail 初始化套接字库
	*/
	void OnInitSocketLib(void);
	/**
	* @fn void OnCloseSocketLib(void)
	* @detail 释放套接字库
	*/
	void OnCloseSocketLib(void);
public:
	afx_msg void OnClose();
	afx_msg LRESULT OnConnectSuccess(WPARAM wParam, LPARAM lParam);
};


