#pragma once
#include "SetupGridCtrl.h"

// CDlgSourceExploSetup 对话框

class CDlgSourceExploSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSourceExploSetup)

public:
	CDlgSourceExploSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSourceExploSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_SOURCEEXPLO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlList		m_wndListGrid;
	CGridCtrlEdit		m_wndEditGrid;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnDestroy();
};
