#pragma once
#include "DlgSourceExploSetup.h"
#include "DlgSourceVibroSetup.h"
// CDlgSourceTypeSetup 对话框

class CDlgSourceTypeSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSourceTypeSetup)

public:
	CDlgSourceTypeSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSourceTypeSetup();

// 对话框数据
	enum { IDD = IDD_DIALOG_SOURCESETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBCGPTabWnd		m_wndTab;
	CDlgSourceExploSetup	m_DlgExplo;
	CDlgSourceVibroSetup	m_DlgVibro;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
};
