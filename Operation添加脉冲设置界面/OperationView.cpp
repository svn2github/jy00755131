// OperationView.cpp : COperationView 类的实现
//

#include "stdafx.h"
#include "Operation.h"
#include "MainFrm.h"
#include "OperationDoc.h"
#include "OperationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COperationView

IMPLEMENT_DYNCREATE(COperationView, CBCGPFormView)

BEGIN_MESSAGE_MAP(COperationView, CBCGPFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CBCGPFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CBCGPFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBCGPFormView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// COperationView 构造/析构

COperationView::COperationView()
	: CBCGPFormView(COperationView::IDD)
{
	// TODO: 在此处添加构造代码
	EnableVisualManagerStyle();
}

COperationView::~COperationView()
{
}

void COperationView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);	

	DDX_Control(pDX, IDC_LISTSTATUSMAIL, m_ctrlListBox);
	DDX_Control(pDX, IDC_STATICSTATUSMAIL, m_ctrlStatic);
}

BOOL COperationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CBCGPFormView::PreCreateWindow(cs);
}

// COperationView 绘制

void COperationView::OnDraw(CDC* pDC)
{
	COperationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	RECT  rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(&rcClient,::GetSysColor(COLOR_3DFACE));

}


// COperationView 打印

BOOL COperationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COperationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COperationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// COperationView 诊断

#ifdef _DEBUG
void COperationView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void COperationView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

COperationDoc* COperationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COperationDoc)));
	return (COperationDoc*)m_pDocument;
}
#endif //_DEBUG


// COperationView 消息处理程序

void COperationView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_OperationLog.m_pListBox = &m_ctrlListBox;

	CRect rc,rcVPDone,rcVPToDo,rcVPAll,rcActive;
	GetWindowRect(&rc);
	SendMessage(WM_SIZE,SIZE_RESTORED,MAKELPARAM(rc.Width(),rc.Height()));
//	MoveWindow(	0,rc.bottom-100 ,rc.Width(),100 ,TRUE);
	
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	CBCGPDockBar *pBottomBar=NULL;	// 最下面的
	CBCGPDockBar *pUpBottomBar=NULL;		// 紧挨着最下面的
	CRect      rcBottom,rcUpBottom;
	if(    pFrm->m_wndVPDone.GetSafeHwnd() 	&& pFrm->m_wndVPToDo.GetSafeHwnd()
		&& pFrm->m_wndVPAll.GetSafeHwnd()   && pFrm->m_wndActiveSource.GetSafeHwnd())
	{
		/*
		pFrm->m_wndVPDone.GetWindowRect(&rcVPDone);
		pFrm->m_wndVPToDo.GetWindowRect(&rcVPToDo);
		pFrm->m_wndVPAll.GetWindowRect(&rcVPAll);
		pFrm->m_wndActiveSource.GetWindowRect(&rcActive);
		if(rcVPDone.bottom>rcVPToDo.bottom)
		{	
			
		}
		else
		{

		}
		
		
		pFrm->m_wndVPDone.GetWindowRect(&rcVPDone);
		pFrm->m_wndVPToDo.GetWindowRect(&rcVPToDo);
		pFrm->m_wndVPAll.GetWindowRect(&rcVPAll);
		if(rcVPAll.bottom >= rcVPToDo.bottom && rcVPAll.bottom >= rcVPDone.bottom )
		{
			rcVPAll.bottom = rc.bottom-100;
			pFrm->m_wndVPAll.MoveWindow(rcVPAll,TRUE);			
		}
		else if (rcVPToDo.bottom >= rcVPAll.bottom && rcVPToDo.bottom >= rcVPDone.bottom)
		{
			rcVPToDo.bottom = rc.bottom-100;
			pFrm->m_wndVPToDo.MoveWindow(rcVPToDo,TRUE);
			if(rcVPAll.bottom>=rcVPDone.bottom)
			{
				pFrm->m_wndVPToDo.DockToWindow(&pFrm->m_wndVPAll,CBRS_ALIGN_BOTTOM);
			}
			else
			{
				pFrm->m_wndVPToDo.DockToWindow(&pFrm->m_wndVPDone,CBRS_ALIGN_BOTTOM);
			}
		}
		else if(rcVPDone.bottom >= rcVPAll.bottom && rcVPDone.bottom >= rcVPToDo.bottom)
		{
			rcVPDone.bottom = rc.bottom-100;
			pFrm->m_wndVPDone.MoveWindow(rcVPDone,TRUE);
			if(rcVPAll.bottom>=rcVPToDo.bottom)
			{
				pFrm->m_wndVPDone.DockToWindow(&pFrm->m_wndVPAll,CBRS_ALIGN_BOTTOM);
			}
			else
			{
				pFrm->m_wndVPDone.DockToWindow(&pFrm->m_wndVPToDo,CBRS_ALIGN_BOTTOM);
			}

		}
		*/
	}
	
}

void COperationView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_ctrlListBox.GetSafeHwnd())
	{
		m_ctrlListBox.SetWindowPos (NULL, 0, 20, cx, cy-22, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if (m_ctrlStatic.GetSafeHwnd())
	{
		m_ctrlStatic.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

}
