// Matrix428Dlg.h : header file
//

#pragma once


// CMatrix428Dlg dialog
class CMatrix428Dlg : public CBCGPDialog
{
// Construction
public:
	CMatrix428Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MATRIX428_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CImageList m_ImageList;
	CToolBarCtrl m_ToolBar;
	TBBUTTON Buttons[5]; 


	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult); 


	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint pt);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMinmizebox();
	afx_msg void OnBnClickedButtonClosebox();
	afx_msg void OnStnDblclickStaticE428();
	afx_msg void OnBnClickedButtonLineClient();
	afx_msg void OnBnClickedButtonLineServer();
	afx_msg void OnBnClickedButtonOperationClient();
	afx_msg void OnBnClickedButtonOperationServer();
	afx_msg void OnBnClickedButtonSim();
	void DlgHorizontalShow(void);
	void DlgVerticalShow(void);
	bool m_bShowDirection;
	char CurrentPath[500];			//记录当前路径

	void LoadToolBar(void);
};
