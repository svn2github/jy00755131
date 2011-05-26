#pragma once

#include "TabTestInstrument.h"
#include "TabTestSensor.h"
#include "TabTestMultiple.h"
#include "TabTestSeisMonitor.h"

#include "SiteData.h"

/**
CDlgTestSetup Test设置对话框类
*/
class CDlgTestSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestSetup)

public:
	CDlgTestSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTestSetup();

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
	CTabTestInstrument m_oTabTestInstrument;
	/** 属性页检波器测试设置*/
	CTabTestSensor m_oTabTestSensor;
	/** 属性页复合测试容器*/
	CBCGPTabWnd m_oTabWndTestMultiple;
	/** 属性页复合测试设置*/
	CTabTestMultiple* m_pTabTestMultiple;
	/** 属性页噪声监测设置*/
	CTabTestSeisMonitor m_oTabTestSeisMonitor;

public:	// 方法
	afx_msg void OnBnClickedButtonApply();	
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonGO();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
};
