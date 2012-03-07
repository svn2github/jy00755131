// OperationView.h : COperationView 类的接口
//


#pragma once
#include "afxwin.h"
// #include "..\common\OperationLog.h"

class COperationView : public CBCGPFormView
{
protected: // 仅从序列化创建
	COperationView();
	DECLARE_DYNCREATE(COperationView)

// 属性
public:
	COperationDoc* GetDocument() const;

	// 记录操作日志
	// COperationLog	   m_OperationLog;
	CBCGPListBox m_ctrlListBox;
	CStatic m_ctrlStatic;
// 操作
public:
	enum { IDD = IDD_STATUSMAILVIE };
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~COperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AppendLog(LPCTSTR strLog);	
};

#ifndef _DEBUG  // OperationView.cpp 中的调试版本
inline COperationDoc* COperationView::GetDocument() const
   { return reinterpret_cast<COperationDoc*>(m_pDocument); }
#endif

