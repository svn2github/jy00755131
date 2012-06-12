#pragma once

#include "SiteData.h"

/**
CDlgLookSetup Look设置对话框类
*/
class CDlgLookSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgLookSetup)

public:
	CDlgLookSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLookSetup();

// 对话框数据
	enum {IDD = 0};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

public:	// 属性
	/** 现场数据*/
	CSiteData* m_pSiteData;

public:	// 方法
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
};
