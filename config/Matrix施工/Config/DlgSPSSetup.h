#pragma once


// DlgSPSSetup 对话框
class CDlgSPSSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgSPSSetup)

public:
	CDlgSPSSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSPSSetup();

	// 对话框数据
	enum { IDD = IDD_SPSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnloadsps();
	afx_msg void OnBnClickedBtngenoptxml();
	afx_msg void OnBnClickedBtngenlinexml();
};