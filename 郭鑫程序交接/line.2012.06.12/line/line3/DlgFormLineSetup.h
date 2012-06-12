#pragma once

#include "TabFormLine.h"

#include "SiteData.h"

/**
*@brief FormLine设置对话框类
*/
class CDlgFormLineSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgFormLineSetup)

public:
	CDlgFormLineSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFormLineSetup();

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
	CTabFormLine m_oTabFormLine;

public:	// 方法
	afx_msg void OnBnClickedButtonApply();	
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonGO();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonDelete();

	// 响应仪器图形视图弹出菜单FormLine设置
	void OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN);
};
