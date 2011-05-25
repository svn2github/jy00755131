#pragma once
#include "afxwin.h"
#include "DlgImpulsive.h"
#include "DlgStackImpulsive.h"

// CDlgProcessType 对话框
// vivi 2010.11.8

class CDlgProcessType : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgProcessType)

public:
	CDlgProcessType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProcessType();

// 对话框数据
	enum { IDD = IDD_PROCESSSETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog(void);
	// 控件IDC_COMBO_PROCESSTYPESELECT的控制变量
	int m_cbindexProcessType;

	afx_msg void OnCbnSelchangeProcessType();
	
	void OnShowWindow(BOOL bShow, UINT nStatus);
	CDlgImpulsive m_dlgImpulsive;
	CDlgStackImpulsive m_dlgStackImpulsive;
	int m_radioProcessSetup;
	afx_msg void OnBnClickedRadioStandard();
	afx_msg void OnBnClickedRadioAdvanced();
};
