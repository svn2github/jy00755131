#pragma once

#include "InstrumentListCtrl.h"

// CInstrumentListView 视图

class CInstrumentListView : public CBCGPGridView
{
	DECLARE_DYNCREATE(CInstrumentListView)

protected:
	CInstrumentListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CInstrumentListView();
	virtual void OnInitialUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:	
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	
	
public:	// 属性
	CInstrumentListCtrl* m_pInstrumentListCtrl;	// 仪器列表网格控件

public:	// 方法
	void AdjustLayout();	// 布局

	// 重新加载现场仪器数据
	void OnReloadSiteDataCMD();

	// 为新的测试数据刷新显示
	void OnRefreshForNewTestValue(int iTestType);
};


