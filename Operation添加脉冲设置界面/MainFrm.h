// MainFrm.h : CMainFrame 类的接口
//
#pragma once

#include "GridWorkspace.h"
#include "ActiveSourceView.h"
#include "ShotSvrSocket.h"
#include "shotpoints.h"
#include "shotsources.h"
#include "ActiveShot.h"
#include "ShotSpreads.h"
#include "ProcessTypes.h"
#include "ChannelList.h"
#include "..\common\FDUConfig.h"
#include "DlgSourceType.h"
#include "DlgShotPoint.h"
#include "DlgCommentType.h"
#include "DlgProcessType.h"


#define CFrameWnd CBCGPFrameWnd
//////////////////////////////////////////////////////////////////////////
// 自定义消息，用户右击菜单选择放某一炮，WPARAM表示炮号，LPARAM表示震源号
//#define WM_MSG_SHOT (WM_USER + 100)


class CMainFrame : public CFrameWnd
{	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	// 所有的放炮对象道，最大为20000道
	CChannelList	m_ChannelList;
	// 采集站配置
	CFDUConfig		m_FDUConfig;

	// 正在放炮的炮号、震源索引	
	CActiveShots   m_AllActiveShots;
	// 放炮表
	CShotPoints		m_AllShotPoints;
	// 震源
	CShotSources    m_AllShotSrcs;
	// 采集排列
	CShotSpreads	m_AllSpreads;
	// 处理类型对象
	CProcessTypes	m_AllProcesses;

	// 显示放炮表窗口
	CVPAllGridWnd	m_wndVPAll;
	CVPToDoGridWnd	m_wndVPToDo;
	CVPDoneGridWnd	m_wndVPDone;
	
	// 显示震源
	CActiveSourceWnd m_wndActiveSource;
	//CArray<CActiveSourceWnd*,CActiveSourceWnd*>	m_pwndActiveSource;

	// 与施工服务器通信的socket
	CShotServerSocket* m_pOperationSvrSock;


// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
// 	CStatusBar  m_wndStatusBar;
// 	CToolBar    m_wndToolBar;
	CBCGPMenuBar	m_wndMenuBar;
	CBCGPStatusBar	m_wndStatusBar;
	CBCGPToolBar	m_wndToolBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewWorkspace();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnShotPoint(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	UINT	m_nAppLook;

	// 创建与施工服务器通信的Socket
	bool CreateShotSvrSocket(void);
	int  LookforActiveShotByID(DWORD dwServerID,DWORD dwThreadID);
	CActiveShot*  LookforActiveShotByNb(DWORD dwShotNb,DWORD dwSourceNb);
	bool OnStartShot( DWORD dwShotNb , DWORD dwSrcNb);
public:
	

	afx_msg void OnDestroy();
	afx_msg void OnViewActivesource();
	afx_msg void OnViewAllvp();
	afx_msg void OnUpdateViewAllvp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewActivesource(CCmdUI *pCmdUI);
	afx_msg void OnViewAlldone();
	afx_msg void OnUpdateViewAlldone(CCmdUI *pCmdUI);
	afx_msg void OnViewAlltodo();
	afx_msg void OnUpdateViewAlltodo(CCmdUI *pCmdUI);
	

public:	
	bool ParseXMLFile(void);
	void AnalysisProtocolOfServerSock(void);
	void WriteLog(LPCTSTR strLog, int nType=1);
	int SetShotPointStatus(CShotPoint* pShotPoint,unsigned char byVPState);
	int SetShotSourceStatus(CShotSource* pShotSource,unsigned char byVPState);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetupSource();
	afx_msg void OnSetupOperation();
	afx_msg void OnSetupComments();
	// vivi 2010.11.8
	// 加入脉冲类型设置界面
	afx_msg void OnSetupProcesstype();
};


