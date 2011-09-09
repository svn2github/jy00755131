// AdminView.h : CAdminView 类的接口
//


#pragma once


class CAdminView : public CBCGPTabView
{
protected: // 仅从序列化创建
	CAdminView();
	DECLARE_DYNCREATE(CAdminView)

// 属性
public:
	CAdminDoc* GetDocument() const;

// 操作
public:

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
	virtual ~CAdminView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void LoadAllUserInfo(void);
};

#ifndef _DEBUG  // AdminView.cpp 中的调试版本
inline CAdminDoc* CAdminView::GetDocument() const
   { return reinterpret_cast<CAdminDoc*>(m_pDocument); }
#endif

