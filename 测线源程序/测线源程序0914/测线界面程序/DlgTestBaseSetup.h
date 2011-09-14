#pragma once

#include "TabTestBase.h"
#include "TabTestLimit.h"

#include "SiteData.h"

/**
CDlgTestBaseSetup TestBase设置对话框类
*/
class CDlgTestBaseSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestBaseSetup)

public:
	CDlgTestBaseSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTestBaseSetup();

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
	/** 属性页测试基本设置，仪器*/
	CTabTestBase m_oTabTestBaseInstrument;
	/** 属性页测试基本设置，检波器*/
	CTabTestBase m_oTabTestBaseSensor;
	/** 属性页测试判据设置，仪器*/
	CTabTestLimit m_oTabTestLimitInstrument;
	/** 属性页测试判据设置，检波器*/
	CTabTestLimit m_oTabTestLimitSensor;

public:	// 方法
	afx_msg void OnBnClickedButtonApply();	
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonChange();
};
