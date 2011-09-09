// InstallView.h : CInstallView 类的接口
//


#pragma once
#include "afxwin.h"
#include "SetupMainView.h"
#include "SetupScalabilityView.h"
#include "../common/InstallParam.h"
#include "../common/InstallParseXML.h"
class CInstallView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CInstallView)

protected: // 仅从序列化创建
	CInstallView();
	virtual ~CInstallView();

// 属性
public:
	enum { IDD = IDD_INSTALLVIEW };
	CInstallParam				m_InstallParam;
	CInstallParseXML			m_ParseXML;
	CButton						m_BtnApply;
	CBCGPTabWnd					m_wndTab;
	CSetupMainView				m_MainView;
	CSetupScalabilityView		m_ScalabilityView;
// 操作
public:
	CInstallDoc* GetDocument() const;
// 重写
public:	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	void CreateTabWnd(void);

// 实现
public:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// 生成的消息映射函数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedBtnapply();
};

#ifndef _DEBUG  // InstallView.cpp 中的调试版本
inline CInstallDoc* CInstallView::GetDocument() const
   { return reinterpret_cast<CInstallDoc*>(m_pDocument); }
#endif

