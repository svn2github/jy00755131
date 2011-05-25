// 定制浮动共具条View.h : C定制浮动共具条View 类的接口
//


#pragma once


class C定制浮动共具条View : public CView
{
protected: // 仅从序列化创建
	C定制浮动共具条View();
	DECLARE_DYNCREATE(C定制浮动共具条View)

// 属性
public:
	C定制浮动共具条Doc* GetDocument() const;

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
	virtual ~C定制浮动共具条View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 定制浮动共具条View.cpp 中的调试版本
inline C定制浮动共具条Doc* C定制浮动共具条View::GetDocument() const
   { return reinterpret_cast<C定制浮动共具条Doc*>(m_pDocument); }
#endif

