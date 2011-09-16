#pragma once
#include "Resource.h"
#include "TabSelectShotPoint.h"
#include "SiteData.h"
// CDlgSelectShotPoint 对话框

class CDlgSelectShotPoint : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectShotPoint)

public:
	CDlgSelectShotPoint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelectShotPoint();

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECTSHOTPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	/** 属性页容器*/
	CBCGPTabWnd m_oTabWnd;
	/** 属性页仪器测试设置*/
	CTabSelectShotPoint m_oTabSelectShotPoint;
	/** 现场数据*/
	CSiteData* m_pSiteData;
	// 绝对排列页面指针
	CTabSpreadAbsolute* m_pTabSpreadAbsolute;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
