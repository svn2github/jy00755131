// StartDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "DlgLoginAdminServer.h"

// CStartDlg dialog
class CStartDlg : public CBCGPDialog
{
// Construction
public:
	CStartDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_START_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
public:
	CDlgLoginAdminServer*		m_pDlgLogin;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtninstall();
	
	DECLARE_MESSAGE_MAP()
private:
	void SetButtonStyle(CBCGPButton& ctrlButton,UINT nBitmapID,LPCTSTR strTooltip=NULL);
	void EnableProgramButton(bool bEnable);
	void DisplayLocalTime(void);
public:
	/** 显示本地时间 */
	CString m_strLocalTime;		
	
	// 连接服务器
	CBCGPButton m_btnConnect;
	// Administration
	CBCGPButton m_btnAdmin;
	CBCGPButton m_btnInstall;
	CBCGPButton m_btnConfig;
	CBCGPButton m_btnLine;
	CBCGPButton m_btnOperation;
	CBCGPButton m_btnPosition;
	CBCGPButton m_btnExport;
	CBCGPButton m_btnLog;
	CBCGPButton m_btnPlotter;
	CBCGPButton m_btnExplorer;
protected:
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnStartExit();
	afx_msg void OnBnClickedBtnconfig();
	afx_msg void OnBnClickedBtnline();
	afx_msg void OnBnClickedBtnoperation();
	afx_msg void OnStartConnection();
	afx_msg void OnBnClickedBtnconnect();
	afx_msg void OnBnClickedBtnexplorer();	
	afx_msg void OnBnClickedBtnadministration();
	afx_msg LRESULT  OnDlgLoginDestroy(WPARAM,LPARAM);
	afx_msg LRESULT OnNcHitTest(CPoint point);		
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:	
	void Login_Success();
	void Login_Fail();
	
};
