#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CSessionsView 窗体视图

class CSessionsView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CSessionsView)

protected:
	CSessionsView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSessionsView();

public:
	enum { IDD = IDD_SESSIONSVIEW };
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
	// 显示当前连接会话用户
	CTreeCtrl m_ctrlSessionTree;
	// 显示详细的用户信息
	CBCGPListBox m_ctrlUserInfo;
private:
	CImageList		m_ImageList;
private:
	void   LoadSingleUserInfo(CUserInfo* pUserInfo);
public:
	virtual void OnInitialUpdate();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void   LoadAliveUserInfo(void);
	afx_msg void OnTvnSelchangedTreeuserinfos(NMHDR *pNMHDR, LRESULT *pResult);
};


