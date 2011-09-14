// TestSiteManageDlg.h : 头文件
//

#pragma once
#include "sitemanage.h"


// CTestSiteManageDlg 对话框
class CTestSiteManageDlg : public CDialog
{
// 构造
public:
	CTestSiteManageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTSITEMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnOninit();
	afx_msg void OnBnClickedBnStart();
	afx_msg void OnBnClickedBnStop();
	afx_msg void OnBnClickedBnClose();

public: //属性
	/** 现场管理对象*/
	CSiteManage m_oSiteManage;
	afx_msg void OnDestroy();
};
