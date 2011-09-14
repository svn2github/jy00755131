//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2010 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1E4CD94D_CB6E_4FAD_9DDE_3E2B96D11CE2__INCLUDED_)
#define AFX_MAINFRM_H__1E4CD94D_CB6E_4FAD_9DDE_3E2B96D11CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CMDIFrameWnd CBCGPMDIFrameWnd

#include "OutputView.h"
#include "SiteData.h"

#include "DlgSurveySetup.h"
#include "DlgLayoutSetup.h"
#include "DlgSpreadSetup.h"
#include "DlgTestBaseSetup.h"
#include "DlgTestSetup.h"
#include "DlgLookSetup.h"
#include "DlgFormLineSetup.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
	void OnChangeLook (BOOL bOneNoteTabs, BOOL bMDITabColors, BOOL bIsVSDotNetLook, 
		BOOL bDockTabColors, BOOL bMDITabsVS2005Look, BOOL bIsToolBoxEx,
		BOOL bActiveTabCloseButton, BOOL bDocumentMenu);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* pObj);
	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPStatusBar			m_wndStatusBar;
	COutputViewBar			m_wndOutputView;
	CBCGPToolBarImages		m_UserImages;

	/**File工具条*/
	CBCGPToolBar m_wndToolBarMatrixFile;
	/**Edit工具条*/
	CBCGPToolBar m_wndToolBarMatrixEdit;
	/**View工具条*/
	CBCGPToolBar m_wndToolBarMatrixView;
	/**Setup工具条*/
	CBCGPToolBar m_wndToolBarMatrixSetup;
	/**Setup工具条*/
	CBCGPToolBar m_wndToolBarMatrixField;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnHelpKeyboardmap();
	afx_msg void OnFileNewProject();
	afx_msg void OnFileNewBlankSolution();
	afx_msg void OnFileOpenSolution();
	afx_msg void OnFileCloseSolution();
	afx_msg void OnFileSaveAll();
	afx_msg void OnViewRefresh();
	afx_msg void OnToolsConnectToDatabase();
	afx_msg void OnToolsDebugProcesses();
	afx_msg void OnViewFullScreen();
	afx_msg void OnHelpWeb();
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
	afx_msg void OnClose();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnMdiTabbedDocument();
	afx_msg void OnUpdateMdiTabbedDocument(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnMatrixInstrumentList();	// 仪器列表视图菜单命令
	afx_msg void OnMatrixSensorList();	// 检波器列表视图菜单命令
	afx_msg void OnMatrixInstrumentGraph();	// 仪器图形视图菜单命令
	afx_msg void OnMatrixSensorGraph();	// 检波器图形视图菜单命令	
	afx_msg void OnMatrixSeismonitorGraph();	// 噪声监测图形视图菜单命令
	afx_msg void OnMatrixSurveySetup();	// 地形设置菜单命令
	afx_msg void OnMatrixLayoutSetup();	// 布局设置菜单命令
	afx_msg void OnMatrixSpreadSetup();	// 排列设置菜单命令
	afx_msg void OnMatrixTestBaseSetup();	// 测试基本设置菜单命令
	afx_msg void OnMatrixTestSetup();	// 测试设置菜单命令
	afx_msg void OnMatrixLookSetup();	// Look设置菜单命令
	afx_msg void OnMatrixFormLineSetup();	// FormLine设置菜单命令
	afx_msg void OnMatrixFieldOn();	// Field On按钮操作
	afx_msg void OnMatrixFieldOff();	// Field Off按钮操作
	afx_msg void OnToolbarMatrixViewGraph();	// 工具条图形视图按钮操作
	afx_msg void OnToolbarMatrixViewList();	// 工具条列表视图按钮操作	
	
	afx_msg void OnViewCustomize();
	afx_msg void OnUpdateCaretPos(CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnGetTabToolTip(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);	
	afx_msg LRESULT OnReloadSiteDataCMD(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingBars ();
	void SetDockingBarsIcons (BOOL bHiColorIcons);

public: // 属性
	/** 现场数据对象*/
	CSiteData m_oSiteData;

	/** 地形设置对话框*/
	CDlgSurveySetup* m_pDlgSurveySetup;
	/** 布局设置对话框*/
	CDlgLayoutSetup* m_pDlgLayoutSetup;
	/** 排列设置对话框*/
	CDlgSpreadSetup* m_pDlgSpreadSetup;
	/** 测试基本设置对话框*/
	CDlgTestBaseSetup* m_pDlgTestBaseSetup;
	/** 测试设置对话框*/
	CDlgTestSetup* m_pDlgTestSetup;
	/** Look设置对话框*/
	CDlgLookSetup* m_pDlgLookSetup;
	/** FormLine设置对话框*/
	CDlgFormLineSetup* m_pDlgFormLineSetup;

	/**Field ON*/
	byte m_byFieldOperation;

public: // 方法
	// 注册内部处理用消息
	void RegisterWindowMessageForProc();
	// 解除注册内部处理用消息
	void UnRegisterWindowMessageForProc();
	// 现场数据对象初始化
	void OnSiteDataInit();
	// 现场数据对象销毁
	void OnSiteDataClose();
	// 加载现场数据
	void LoadSiteData();

	// 处理内部网络命令帧
	void OnProcInterFaceCommand(CFrameInterface* pFrameInterface);

	// 处理命令	100-现场数据输出更新通知命令
	void ProcInterFaceCommandForSiteDataOutput();

	// 处理命令	101-Server Field ON/OFF
	void ProcInterFaceCommandForServerField(byte byFieldOperation);
	// 处理命令	101-Server Field ON
	void ProcInterFaceCommandForServerFieldOn();
	// 处理命令	101-Server Field OFF
	void ProcInterFaceCommandForServerFieldOff();

	// 处理测试数据，仪器测试
	void ProcInterFaceDataForInstrumentTest(CFrameInterface* pFrameInterface, int iTestType);
	// 收到新的测试数据后，更新仪器列表视图和仪器图形视图
	void OnRefreshInstrumentViewForNewTestValue(int iTestType);

	// 处理测试数据，检波器测试
	void ProcInterFaceDataForSensorTest(CFrameInterface* pFrameInterface, int iTestType);
	// 收到新的测试数据后，更新检波器列表视图和检波器图形视图
	void OnRefreshSensorViewForNewTestValue(int iTestType);

	// 处理测试数据，检波器噪声监测测试
	void ProcInterFaceDataForSeismonitorTest(CFrameInterface* pFrameInterface);
	// 收到新的测试数据后，更新检波器噪声监测图形视图
	void OnRefreshSeismonitorViewForNewTestValue();

	// 处理测试数据，检波器测试-新加检波器的初始测试
	void ProcInterFaceDataForOnFieldTest(CFrameInterface* pFrameInterface);
	// 收到新的测试数据后，更新检波器列表视图
	void OnRefreshSensorViewForOnFieldTest();

	// 响应仪器图形视图弹出菜单设置标记点
	void OnGraphViewInstrumentSetMarker(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应仪器图形视图弹出菜单设置辅助道
	void OnGraphViewInstrumentSetAuxiliary(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应仪器图形视图弹出菜单设置迂回道
	void OnGraphViewInstrumentSetDetour(unsigned int uiInstrumentTypeLow, unsigned int uiSNLow, unsigned int uiInstrumentTypeHigh, unsigned int uiSNHight);
	// 响应仪器图形视图弹出菜单FormLine设置
	void OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应检波器图形视图弹出菜单Mute设置
	void OnGraphViewSensorSetMute(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 响应仪器图形视图弹出菜单FieldOn/Off操作
	void OnToolBarFieldOperation();

	// 重置工具条
	void OnResetToolBarMatrix();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1E4CD94D_CB6E_4FAD_9DDE_3E2B96D11CE2__INCLUDED_)
