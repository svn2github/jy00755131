// TabControlTestDlg.h : header file
//
#include "Tab1.h"
#include "Tab2.h"
#include "Tab3.h"
#pragma once#include "afxcmn.h"



// CTabControlTestDlg dialog
class CTabControlTestDlg : public CBCGPDialog
{
// Construction
public:
	CTabControlTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TABCONTROLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
	CTab1 m_tab1;
	CTab2 m_tab2;
	CTab3 m_tab3;
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
