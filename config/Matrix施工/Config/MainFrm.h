// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	// 拆分窗口
	CBCGPSplitterWnd	m_wndSplitter;
	BOOL				m_bSplitter;
protected:  // control bar embedded members
	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
	CBCGPToolBar		m_wndToolBarLine;
private:
	/** 显示模式：TRUE=紧凑；FALSE=正常 */
	BOOL				m_bCompactMode;
// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetupCrew();
	afx_msg void OnSetupUserinfo();
	afx_msg void OnSetupSegd();
	afx_msg void OnSetupDiskrecord();
	afx_msg void OnSetupSps();
	afx_msg void OnLineOn();
	afx_msg void OnLineOff();
	afx_msg LRESULT OnNewLog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void ShowCompactMode();
};


