// AdminServerDlg.h : header file
//

#pragma once

#include "SocketUserInfo.h"
#include "afxwin.h"
// CAdminServerDlg dialog
class CAdminServerDlg : public CDialog
{
// Construction
public:
	CAdminServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ADMINSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CSocketUserInfo		m_sockUserInfo;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CListBox m_ctrlMsg;
};
