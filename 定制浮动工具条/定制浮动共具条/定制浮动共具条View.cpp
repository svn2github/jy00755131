// 定制浮动共具条View.cpp : C定制浮动共具条View 类的实现
//

#include "stdafx.h"
#include "定制浮动共具条.h"

#include "定制浮动共具条Doc.h"
#include "定制浮动共具条View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C定制浮动共具条View

IMPLEMENT_DYNCREATE(C定制浮动共具条View, CView)

BEGIN_MESSAGE_MAP(C定制浮动共具条View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// C定制浮动共具条View 构造/析构

C定制浮动共具条View::C定制浮动共具条View()
{
	// TODO: 在此处添加构造代码

}

C定制浮动共具条View::~C定制浮动共具条View()
{
}

BOOL C定制浮动共具条View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// C定制浮动共具条View 绘制

void C定制浮动共具条View::OnDraw(CDC* /*pDC*/)
{
	C定制浮动共具条Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// C定制浮动共具条View 打印

BOOL C定制浮动共具条View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void C定制浮动共具条View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void C定制浮动共具条View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// C定制浮动共具条View 诊断

#ifdef _DEBUG
void C定制浮动共具条View::AssertValid() const
{
	CView::AssertValid();
}

void C定制浮动共具条View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C定制浮动共具条Doc* C定制浮动共具条View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C定制浮动共具条Doc)));
	return (C定制浮动共具条Doc*)m_pDocument;
}
#endif //_DEBUG


// C定制浮动共具条View 消息处理程序
