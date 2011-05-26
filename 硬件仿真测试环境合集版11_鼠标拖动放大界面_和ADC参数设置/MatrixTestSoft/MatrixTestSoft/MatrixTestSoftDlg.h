// MatrixTestSoftDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "TabUartToUdp.h"
#include "TabPortMonitoring.h"
#include "TabSample.h"
#include "TabADCSettings.h"


// CMatrixTestSoftDlg dialog
class CMatrixTestSoftDlg : public CDialog
{
// Construction
public:
	CMatrixTestSoftDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MATRIXTESTSOFT_DIALOG };

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
	// TabContrl控件的控制变量
	CTabCtrl m_cTabCtrlItems;
	// CTabUartToUdp类的变量
	CTabUartToUdp m_oTabUartToUdp;
	// CTabPortMonitoring类的变量
	CTabPortMonitoring m_oTabPortMonitoring;
	// CTabGraphView类的变量
	CTabSample m_oTabSample;
	// CTabADCSettings类的变量
	CTabADCSettings m_oTabADCSettings;

	// 初始化选项卡控件
	void InitTabControlItems(void);
	afx_msg void OnTcnSelchangeTabItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
