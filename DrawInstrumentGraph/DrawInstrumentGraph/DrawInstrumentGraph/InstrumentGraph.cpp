// InstrumentGraph.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawInstrumentGraph.h"
#include "InstrumentGraph.h"


// CInstrumentGraph

IMPLEMENT_DYNAMIC(CInstrumentGraph, CWnd)

CInstrumentGraph::CInstrumentGraph()
{
	m_brushBack.CreateSolidBrush(RGB(  0,   0,   64)) ;

	// protected bitmaps to restore the memory DC's
	m_pbitmapOldGrid = NULL ;
}

CInstrumentGraph::~CInstrumentGraph()
{
	if (m_pbitmapOldGrid != NULL)
		m_dcGraph.SelectObject(m_pbitmapOldGrid) ; 
}


BEGIN_MESSAGE_MAP(CInstrumentGraph, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CInstrumentGraph 消息处理程序

int CInstrumentGraph::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectClient.left = 0;
	m_rectClient.right = lpCreateStruct->cx;
	m_rectClient.top = 0;
	m_rectClient.bottom = lpCreateStruct->cy;
	OnDrawInstrumentGraph();
	return 0;
}

// 绘制仪器图形
void CInstrumentGraph::OnDrawInstrumentGraph(void)
{
	CClientDC dc(this) ;  

	// if we don't have one yet, set up a memory dc for the grid
	if (m_dcGraph.GetSafeHdc() == NULL)
	{
		m_dcGraph.CreateCompatibleDC(&dc) ;
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height()) ;
		m_pbitmapOldGrid = m_dcGraph.SelectObject(&m_bitmapGrid) ;
	}

	m_dcGraph.SetBkColor (RGB(  255,   255,   255)) ;

	// fill the grid background
	m_dcGraph.FillRect(m_rectClient, &m_brushBack) ;
	InvalidateRect(m_rectClient) ;
}


void CInstrumentGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC

	// no real plotting work is performed here, 
	// just putting the existing bitmaps on the client

	// to avoid flicker, establish a memory dc, draw to it 
	// and then BitBlt it to the client
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	if (memDC.GetSafeHdc() != NULL)
	{
		// first drop the grid on the memory dc
		memDC.BitBlt(0, 0, m_rectClient.Width(), m_rectClient.Height(), 
			&m_dcGraph, 0, 0, SRCCOPY) ;
		// finally send the result to the display
		dc.BitBlt(0, 0, m_rectClient.Width(), m_rectClient.Height(), 
			&memDC, 0, 0, SRCCOPY) ;
	}
	memDC.SelectObject(oldBitmap) ;
}
