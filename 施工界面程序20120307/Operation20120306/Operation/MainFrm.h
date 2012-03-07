// MainFrm.h : CMainFrame 类的接口
//
#pragma once

// #include "..\common\ChannelList.h"
// #include "..\common\FDUConfig.h"
#include "GridWorkspace.h"
#include "ActiveSourceView.h"
#include "ShotSvrSocket.h"
#include "shotpoints.h"
#include "shotsources.h"
#include "ActiveShot.h"
#include "ShotSpreads.h"
#include "ProcessTypes.h"
#include "ShotComment.h"

#include "DlgSourceType.h"
#include "DlgShotPoint.h"
#include "DlgCommentType.h"
#include "DlgDelaySetup.h"
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
	// 整个测区的定义对象，最大为20000道
	CChannelList	m_ChannelList;
	// 采集站配置
	CFDUConfig		m_FDUConfig;
	// 采集SEGD数据、测试数据存盘配置信息
	CDiskRecordConfig	m_DiskConfig;

	// 所有的辅助道
	CAuxChannels	m_AllAuxChannels;
	// 正在放炮的炮号、震源索引	
	CActiveShots   m_AllActiveShots;
	// 放炮表
	CShotPoints		m_AllShotPoints;
	// 震源
	CShotSources    m_AllShotSources;
	// 采集排列
	CShotSpreads	m_AllSpreads;
	// 处理类型对象
	CProcessTypes	m_AllProcesses;

	// 操作员注释
	CShotComments	m_AllComments;

	// 显示放炮表窗口
	CVPAllGridWnd	m_wndVPAll;
	CVPToDoGridWnd	m_wndVPToDo;
	CVPDoneGridWnd	m_wndVPDone;
	
	// 显示震源
	CActiveSourceWnd m_wndActiveSource;
	//CArray<CActiveSourceWnd*,CActiveSourceWnd*>	m_pwndActiveSource;

	// cxm 2012.3.7
// 	// 与施工服务器通信的socket
// 	CSocketShotServer* m_pOperationSvrSock;

protected:  // 控件条嵌入成员
// 	CStatusBar  m_wndStatusBar;
// 	CToolBar    m_wndToolBar;
	CBCGPMenuBar	m_wndMenuBar;
	CBCGPStatusBar	m_wndStatusBar;
	CBCGPToolBar	m_wndToolBar;
	CBCGPToolBar	m_wndToolBarSetup;
	CBCGPToolBar	m_wndToolBarView;

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

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);

	// cxm 2012.3.7
// 	afx_msg void OnViewWorkspace();
// 	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
// 	afx_msg void OnUpdateViewWorkspace2(CCmdUI* pCmdUI);
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnShotPoint(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	UINT	m_nAppLook;

	// cxm 2012.3.7
// 	// 创建与施工服务器通信的Socket
// 	bool CreateShotSvrSocket(void);	
	bool OnStartShot( DWORD dwShotNb , DWORD dwSrcNb);
private:	
	// cxm 2012.3.7
//	void AnalysisProtocolOfAckResultFrm(int nActiveShotIndex,unsigned char *pBuf);
public:	
	// cxm 2012.3.7
//	bool ParseXMLFile(void);
//	void AnalysisProtocolOfServerSock(void);
	// void WriteLog(LPCTSTR strLog, int nType=1);
	void SetShotPointState(CShotPoint* pShotPoint,unsigned char byVPState);
	void SetShotSourceState(CShotSource* pShotSource,unsigned char byVPState);
	/** 响应Disassociate VP 暂停使用该震源 */
	CShotSource* OnDisassociatevp(DWORD  dwShotSourceNb);
	/** 响应associate VP 使用该震源 */
	CShotSource*  OnAssociatevp(DWORD  dwShotSourceNb);
	/** 响应associate SFL SFN 使用该震源 */
	CShotSource*  OnAssociateSPLSPN(DWORD  dwShotSourceNb);
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
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetupSource();
	afx_msg void OnSetupOperation();
	afx_msg void OnSetupComments();
	afx_msg void OnSetupDelaysteup();
	afx_msg void OnSetupProcesstype();
	// cxm 2012.3.7
// 	afx_msg LRESULT OnNewLog(WPARAM wParam, LPARAM lParam);
// 	afx_msg LRESULT OnNewXMLFile(WPARAM wParam, LPARAM lParam);
};


