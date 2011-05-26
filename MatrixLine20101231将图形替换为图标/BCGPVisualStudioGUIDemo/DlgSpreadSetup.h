#pragma once

#include "TabSpreadAbsolute.h"
#include "TabSpreadGeneric.h"

#include "SiteData.h"

/**
CDlgSpreadSetup Spread设置对话框类
*/
class CDlgSpreadSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgSpreadSetup)

public:
	CDlgSpreadSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSpreadSetup();

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

	/** 属性页容器*/
	CBCGPTabWnd m_oTabWnd;
	/** 属性页仪器测试设置*/
	CTabSpreadAbsolute m_oTabSpreadAbsolute;
	/** 属性页检波器测试设置*/
	CTabSpreadGeneric m_oTabSpreadGeneric;

public:	// 方法
	afx_msg void OnBnClickedButtonApply();	
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
};
