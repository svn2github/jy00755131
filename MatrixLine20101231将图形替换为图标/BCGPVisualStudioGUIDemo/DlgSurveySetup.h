#pragma once

#include "TabSurvey.h"
#include "TabPointCode.h"
#include "TabSensor.h"

#include "SiteData.h"

/**
CDlgSurveySetup 地形设置对话框类
*/
class CDlgSurveySetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgSurveySetup)

public:
	CDlgSurveySetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSurveySetup();

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
	/** 属性页地形*/
	CTabSurvey m_oTabSurvey;
	/** 属性页点代码*/
	CTabPointCode m_oTabPointCode;
	/** 属性页检波器*/
	CTabSensor m_oTabSensor;

public:	// 方法
	afx_msg void OnBnClickedButtonApplyAll();
	afx_msg void OnBnClickedButtonApplySensor();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReverse();
};
