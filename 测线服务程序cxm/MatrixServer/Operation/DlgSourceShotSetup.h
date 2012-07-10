#pragma once
#include "SetupGridCtrl.h"

// CDlgSourceShotSetup 对话框

class CDlgSourceShotSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSourceShotSetup)

public:
	CDlgSourceShotSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSourceShotSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_SOURCESHOTSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlList		m_wndListGrid;
	CGridCtrlEdit		m_wndEditGrid;
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReverse();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};
