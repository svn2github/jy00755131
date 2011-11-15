// InstrumentGraph.cpp : 实现文件
//

#include "stdafx.h"
#include "TabSample.h"
#include "InstrumentGraph.h"
#include "Parameter.h"
// 绘图背景颜色
#define GraphBkColor		RGB(  255,   255,   255)
// 绘图连线颜色
#define GraphLineColor		RGB(	0,	0,	255	)
// 仪器绘图单元X轴尺寸
#define GraphGridInstrumentSizeX		16
// 仪器绘图单元Y轴尺寸
#define GraphGridInstrumentSizeY		16
// 连接线绘图单元X轴尺寸
#define GraphGridLineSizeX					10
// 连接线绘图单元Y轴尺寸
#define GraphGridLineSizeY					10
// 在视图上侧增加仪器超出绘图区域操作
#define OptGraphRectAddTop						1
// 在视图下侧增加仪器超出绘图区域操作
#define OptGraphRectAddBottom					2
// 在视图左侧增加仪器超出绘图区域操作
#define OptGraphRectAddLeft						3
// 在视图右侧增加仪器超出绘图区域操作
#define OptGraphRectAddRight						4
// 在视图上侧删除仪器超出绘图区域操作
#define OptGraphRectDelTop							5
// 在视图下侧删除仪器超出绘图区域操作
#define OptGraphRectDelBottom					6
// 在视图左侧删除仪器超出绘图区域操作
#define OptGraphRectDelLeft							7
// 在视图右侧删除仪器超出绘图区域操作
#define OptGraphRectDelRight						8
// CInstrumentGraph

IMPLEMENT_DYNAMIC(CInstrumentGraph, CWnd)

CInstrumentGraph::CInstrumentGraph()
{
	m_brushBack.CreateSolidBrush(GraphBkColor) ;
	// protected bitmaps to restore the memory DC's
	m_pbitmapOldGrid = NULL ;
	m_uiGridX = GraphGridInstrumentSizeX;
	m_uiGridY = GraphGridInstrumentSizeY;
	m_uiGridLineX = GraphGridLineSizeX;
	m_uiGridLineY = GraphGridLineSizeY;
	m_iPosShowInterval = 30;
	m_iVScrBarInterval = 20;
	m_iHScrBarInterval = 20;
	m_iLauxPosX = 200;
	m_iLauxPosY = 100;
	m_pWndVScr = NULL;
	m_pWndHScr = NULL;
}

CInstrumentGraph::~CInstrumentGraph()
{
	if (m_pbitmapOldGrid != NULL)
		m_dcGraph.SelectObject(m_pbitmapOldGrid) ; 
	m_oInstrumentGraphRectList.RemoveAll();
}


BEGIN_MESSAGE_MAP(CInstrumentGraph, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
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
	static HWND childhwnd1=CreateWindow(TEXT("scrollbar"),NULL,WS_CHILD|WS_VISIBLE|SBS_VERT,
		m_rectClient.right - m_iVScrBarInterval,m_rectClient.top,m_iVScrBarInterval,m_rectClient.bottom,
		this->m_hWnd,(HMENU)1,(HINSTANCE)GetWindowLong(this->m_hWnd,GWL_HINSTANCE),NULL);
	static HWND childhwnd2=CreateWindow(TEXT("scrollbar"),NULL,WS_CHILD|WS_VISIBLE|SBS_HORZ,
		m_rectClient.left,m_rectClient.bottom - m_iHScrBarInterval,m_rectClient.right - m_iVScrBarInterval,m_iHScrBarInterval,
		this->m_hWnd,(HMENU)1,(HINSTANCE)GetWindowLong(this->m_hWnd,GWL_HINSTANCE),NULL);
	m_pWndVScr = FromHandle(childhwnd1);
	m_pWndHScr = FromHandle(childhwnd2);
	m_oBmpFDU1.LoadBitmap(IDB_BITMAP_FDU1);
	m_oBmpFDU2.LoadBitmap(IDB_BITMAP_FDU2);
	m_oBmpLAUL1.LoadBitmap(IDB_BITMAP_LAUL1);
	m_oBmpLAUL2.LoadBitmap(IDB_BITMAP_LAUL2);
	m_oBmpLAUX1.LoadBitmap(IDB_BITMAP_LAUX1);
	m_oBmpLAUX2.LoadBitmap(IDB_BITMAP_LAUX2);
	m_oBmpLCI1.LoadBitmap(IDB_BITMAP_LCI1);
	m_oBmpLCI2.LoadBitmap(IDB_BITMAP_LCI2);
	OnReset();
	return 0;
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
	memBitmap.CreateCompatibleBitmap(&dc, m_rectGraph.Width() - m_iHScrPos, m_rectGraph.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	if (memDC.GetSafeHdc() != NULL)
	{
		// first drop the grid on the memory dc
		memDC.BitBlt(0, 0, m_rectGraph.Width() - m_iHScrPos, m_rectGraph.Height(), 
			&m_dcGraph, m_iHScrPos, 0, SRCCOPY) ;
		// finally send the result to the display
		dc.BitBlt(0, 0, m_rectGraph.Width() - m_iHScrPos, m_rectGraph.Height(), 
			&memDC, 0, 0, SRCCOPY) ;
	}
	memDC.SelectObject(oldBitmap) ;
}

// 绘制所有仪器
// pDC	CDC指针
// uiGridX	单元格X轴方向大小
// uiGridY	单元格Y轴方向大小
void CInstrumentGraph::DrawUnitAll(void)
{
	POSITION pos = m_oInstrumentGraphRectList.GetHeadPosition();
	m_oGraph oInstrumentGraph;
	// 显示仪器的线号和点号
	for (int i=0; i<m_oInstrumentGraphRectList.GetCount(); i++)
	{
		oInstrumentGraph = m_oInstrumentGraphRectList.GetNext(pos);
		DrawUnit(oInstrumentGraph.iUnitIndex, oInstrumentGraph.iLineIndex, oInstrumentGraph.uiLineDirection, 
			oInstrumentGraph.uiType, oInstrumentGraph.uiSN, oInstrumentGraph.uiOpt, false);
	}
}
// 绘制仪器单元（包含连接线）
// pDC								CDC指针
// uiGridX						单元格X轴方向大小
// uiGridY						单元格Y轴方向大小
// uiUnitIndex					此仪器的图元序号
// uiLineNum					测线号
// uiLineDirection			连接线方向：1为上，2为下，3为左，4为右，无连接线为0
// uiType							仪器类型
void CInstrumentGraph::DrawUnit(int iUnitIndex, int iLineIndex, unsigned int uiLineDirection, unsigned int uiType, 
								unsigned int uiSN, unsigned int uiOpt, bool bSet)
{
	CPen oPenUnit;
	CPen* pOldPen;
	CDC memDC;			//定义一个DC
	CBitmap* pBkBmp = NULL;		//定义一个位图对象指针
	CBitmap* oldBkBmp = NULL;
	BITMAP BmpInfo;	//定义一个位图信息结构体
	CRect oRectLine;
	CRect oRectInstrument;
	oPenUnit.CreatePen(PS_SOLID, 2, GraphLineColor);
	pOldPen = m_dcGraph.SelectObject(&oPenUnit);
	if ((uiLineDirection == 1) || (uiLineDirection == 2))
	{
		oRectLine.left = m_iLauxPosX + iUnitIndex * (m_uiGridX + m_uiGridLineX);
		oRectLine.right = oRectLine.left + m_uiGridX;
		oRectInstrument.left = oRectLine.left;
		oRectInstrument.right = oRectLine.right;
		if (uiLineDirection == 1)
		{
			oRectLine.top = m_iLauxPosY + iLineIndex * (m_uiGridY + m_uiGridLineY) + m_uiGridY;
		}
		else
		{
			oRectLine.top = m_iLauxPosY + iLineIndex * (m_uiGridY + m_uiGridLineY) - m_uiGridLineY;
		}
		oRectLine.bottom = oRectLine.top + m_uiGridLineY;
		oRectInstrument.top = m_iLauxPosY + iLineIndex * (m_uiGridY + m_uiGridLineY);
		oRectInstrument.bottom = oRectInstrument.top + m_uiGridY;

		m_dcGraph.MoveTo(CPoint((oRectLine.left + oRectLine.right) / 2, oRectLine.top));
		m_dcGraph.LineTo(CPoint((oRectLine.left + oRectLine.right) / 2, oRectLine.bottom));
	}
	else if ((uiLineDirection == 3) || (uiLineDirection == 4))
	{
		// 连接线绘制区域
		if (uiLineDirection == 3)
		{
			oRectLine.left = m_iLauxPosX + iUnitIndex * (m_uiGridX + m_uiGridLineX) + m_uiGridX;
		}
		else
		{
			oRectLine.left = m_iLauxPosX + iUnitIndex * (m_uiGridX + m_uiGridLineX) - m_uiGridLineX;
		}
		oRectLine.right = oRectLine.left + m_uiGridLineX;
		oRectInstrument.left = m_iLauxPosX + iUnitIndex * (m_uiGridX + m_uiGridLineX);
		oRectInstrument.right = oRectInstrument.left + m_uiGridX;
		oRectLine.top = m_iLauxPosY + iLineIndex * (m_uiGridY + m_uiGridLineY);
		oRectLine.bottom = oRectLine.top + m_uiGridY;
		oRectInstrument.top = oRectLine.top;
		oRectInstrument.bottom = oRectLine.bottom;
		m_dcGraph.MoveTo(CPoint(oRectLine.left, (oRectLine.top + oRectLine.bottom) / 2));
		m_dcGraph.LineTo(CPoint(oRectLine.right, (oRectLine.top + oRectLine.bottom) / 2));
	}
	else
	{
		// 主交叉站不需要画连接线iUnitIndex = 0
		oRectInstrument.left = m_iLauxPosX;
		oRectInstrument.right = oRectInstrument.left + m_uiGridX;
		oRectInstrument.top = m_iLauxPosY;
		oRectInstrument.bottom = oRectInstrument.top + m_uiGridY;
	}
	if (bSet == true)
	{
		// 向上增加仪器
		if (uiLineDirection == 1)
		{
		}
		// 向下增加仪器
		else if (uiLineDirection == 2)
		{
		}
		// 向左增加仪器
		else if (uiLineDirection == 3)
		{
		}
		// 向右增加仪器
		else if (uiLineDirection == 4)
		{
			if (GraphInstrumentOnLine == uiOpt)
			{
				if (oRectInstrument.right + m_iVScrBarInterval > m_rectClient.right)
				{
					m_pWndHScr->EnableWindow(TRUE);
					OnOptGraphRect(OptGraphRectAddRight);
				}
			}
			if (GraphInstrumentOffLine == uiOpt)
			{
				// 部分删除仪器对图形DC的操作
				if (oRectInstrument.right + m_iVScrBarInterval > m_rectClient.right)
				{
					OnOptGraphRect(OptGraphRectDelRight);
				}
				else
				{
					m_pWndHScr->EnableWindow(FALSE);
				}
			}
		}
	}
	switch (uiOpt)
	{
	case GraphInstrumentOnLine:
		if (uiType == InstrumentTypeFDU)
		{
			pBkBmp = &m_oBmpFDU1;
		} 
		else if (uiType == InstrumentTypeLAUL)
		{
			pBkBmp = &m_oBmpLAUL1;
		}
		else if (uiType == InstrumentTypeLAUX)
		{
			pBkBmp = &m_oBmpLAUX1;
		}
		else if (uiType == InstrumentTypeLCI)
		{
			pBkBmp = &m_oBmpLCI1;
		}
		break;
	case GraphInstrumentIPSet:
		if (uiType == InstrumentTypeFDU)
		{
			pBkBmp = &m_oBmpFDU2;
		} 
		else if (uiType == InstrumentTypeLAUL)
		{
			pBkBmp = &m_oBmpLAUL2;
		}
		else if (uiType == InstrumentTypeLAUX)
		{
			pBkBmp = &m_oBmpLAUX2;
		}
		else if (uiType == InstrumentTypeLCI)
		{
			pBkBmp = &m_oBmpLCI2;
		}
		break;
	case GraphInstrumentOffLine:
		// 填充背景颜色
		m_dcGraph.FillRect(oRectLine, &m_brushBack) ;
		m_dcGraph.FillRect(oRectInstrument, &m_brushBack) ;
		break;
	default:
		break;
	}
	if ((uiOpt == GraphInstrumentOnLine) || (uiOpt == GraphInstrumentIPSet))
	{
		pBkBmp->GetBitmap(&BmpInfo);				// 获取位图高宽等信息，保存在位图结构体中
		memDC.CreateCompatibleDC(&m_dcGraph);			// 创建一个兼容屏幕DC的内存DC：m_MemDc。
		oldBkBmp = memDC.SelectObject(pBkBmp);				// 将该位图选择到刚创建的内存DC中。

		/*将内存DC里的东西贴到屏幕DC上去*/
		m_dcGraph.BitBlt(oRectInstrument.left,oRectInstrument.top,BmpInfo.bmWidth,BmpInfo.bmHeight,&memDC,0,0,SRCCOPY);
		memDC.SelectObject(oldBkBmp);
	}
	m_dcGraph.SelectObject(pOldPen);
	if (bSet == true)
	{
		m_oGraph oInstrumentGraph;
		oInstrumentGraph.oRect = oRectInstrument;
		oInstrumentGraph.iUnitIndex = iUnitIndex;
		oInstrumentGraph.iLineIndex = iLineIndex;
		oInstrumentGraph.uiType = uiType;
		oInstrumentGraph.uiLineDirection = uiLineDirection;
		oInstrumentGraph.uiSN = uiSN;
//		oInstrumentGraph.uiFDUIndex = uiFDUIndex;
		oInstrumentGraph.uiOpt = uiOpt;
		if (uiOpt == GraphInstrumentOnLine)
		{
			if (NULL == m_oInstrumentGraphRectList.Find(oInstrumentGraph))
			{
				m_oInstrumentGraphRectList.AddTail(oInstrumentGraph);
			}
		}
		else if (uiOpt == GraphInstrumentIPSet)
		{
			POSITION pos = m_oInstrumentGraphRectList.Find(oInstrumentGraph);
			oInstrumentGraph = m_oInstrumentGraphRectList.GetAt(pos);
			oInstrumentGraph.uiOpt = GraphInstrumentIPSet;
			m_oInstrumentGraphRectList.SetAt(pos, oInstrumentGraph);
		}
		else if (uiOpt == GraphInstrumentOffLine)
		{
			m_oInstrumentGraphRectList.RemoveAt(m_oInstrumentGraphRectList.Find(oInstrumentGraph));
		}
	}
	InvalidateRect(m_rectClient, FALSE);
}

void CInstrumentGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint oGraphPoint;
	CString str = _T("");
	m_oGraph oInstrumentGraph;
	CPoint oTextOutPoint;
	m_dcGraph.SetTextAlign (TA_LEFT|TA_TOP) ;
	oTextOutPoint.x = m_rectGraph.left + m_iHScrPos;
	oTextOutPoint.y = m_rectGraph.bottom - m_iPosShowInterval;

	oGraphPoint.x = point.x + m_iHScrPos;
	oGraphPoint.y = point.y;
	if (TRUE == OnSelectInstrument(oGraphPoint, oInstrumentGraph))
	{
		str.Format(_T("线号 = %d    点号 = %d    SN = 0x%04x    "), oInstrumentGraph.iLineIndex, 
			oInstrumentGraph.iUnitIndex, oInstrumentGraph.uiSN);
		OnShowTextOut(&m_dcGraph, oTextOutPoint,  str);
	} 
	else
	{
		OnFillMsgAreaBkColor();
	}
	CWnd::OnMouseMove(nFlags, point);
}

// 判断界面仪器是否被选中
BOOL CInstrumentGraph::OnSelectInstrument(CPoint oGraphPoint, m_oGraph &oInstrumentGraph)
{
	CString str = _T("");
	CRect oRect = 0;
	POSITION pos = m_oInstrumentGraphRectList.GetHeadPosition();
	// 显示仪器的线号和点号
	for (int i=0; i<m_oInstrumentGraphRectList.GetCount(); i++)
	{
		oInstrumentGraph = m_oInstrumentGraphRectList.GetNext(pos);
		oRect = oInstrumentGraph.oRect;
		if ((oRect.left <= oGraphPoint.x) && (oRect.right >= oGraphPoint.x)
			&&(oRect.top <= oGraphPoint.y) && (oRect.bottom >= oGraphPoint.y))
		{
			
			return TRUE;
		}
	}
	return FALSE;
}

// 填充背景颜色
void CInstrumentGraph::OnFillBkColor(CDC * pDC, CRect oRect)
{
	pDC->SetBkColor (GraphBkColor) ;
	// 填充背景颜色
	pDC->FillRect(oRect, &m_brushBack) ;
	// 刷新客户区界面
	InvalidateRect(oRect, TRUE) ;
}

// 清除所有仪器图形
void CInstrumentGraph::OnClearAllInstrumentGraph(void)
{
	OnReset();
}
void CInstrumentGraph::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int newPos = moveScrollBar(nSBCode, nPos, pScrollBar);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CInstrumentGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_iHScrPos = moveScrollBar(nSBCode, nPos, pScrollBar);
	InvalidateRect(m_rectClient, FALSE);
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
} 
int CInstrumentGraph::moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//
	// Get current scroll bar position
	//
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	pScrollBar->GetScrollInfo(&info);

	//
	// Compute new position based on the type of scroll bar events
	//
	int newPos = info.nPos;
	switch (nSBCode)
	{
	case SB_LEFT:
		newPos = info.nMin;
		break;
	case SB_RIGHT:
		newPos = info.nMax;
		break;
	case SB_LINELEFT:
		newPos -= 10;
		break;
	case SB_LINERIGHT:
		newPos += 10;
		break;
	case SB_PAGELEFT:
		newPos -= 30;
		break;
	case SB_PAGERIGHT:
		newPos += 30;
		break;
	case SB_THUMBTRACK:
		newPos = info.nTrackPos;
		break;
	}
	if (newPos < info.nMin) newPos = info.nMin;
	if (newPos > info.nMax) newPos = info.nMax;

	// Update the scroll bar with the new position
	pScrollBar->SetScrollPos(newPos);

	// Returns the position of the scroll bar as a ratio of its total length
	return newPos;
}
// 处理绘图区域
void CInstrumentGraph::OnOptGraphRect(unsigned int uiOpt)
{
	CDC TempDC ;
	CBitmap TempBitmap ;
	CBitmap* oldBitmap = NULL; 
	CBitmap bitmapGrid;
	CRect oShowRect;
	if (uiOpt == OptGraphRectAddRight)
	{
		m_rectGraph.right += m_uiGridX + m_uiGridLineX;
		m_iRightMovePos += m_uiGridX + m_uiGridLineX;
		m_pWndHScr->SetScrollRange(SB_CTL, 0,m_iRightMovePos,FALSE);
		m_pWndHScr->SetScrollPos(SB_CTL,m_iRightMovePos,TRUE);
		m_iHScrPos = m_iRightMovePos;
	}
	else if (uiOpt == OptGraphRectDelRight)
	{
		m_rectGraph.right -= m_uiGridX + m_uiGridLineX;
		m_iRightMovePos -= m_uiGridX + m_uiGridLineX;
		m_pWndHScr->SetScrollRange(SB_CTL, 0,m_iRightMovePos,FALSE);
		m_pWndHScr->SetScrollPos(SB_CTL,m_iRightMovePos,TRUE);
		m_iHScrPos = m_iRightMovePos;
	}
	TempDC.CreateCompatibleDC(&m_dcGraph) ;
	TempBitmap.CreateCompatibleBitmap(&m_dcGraph, m_rectGraph.Width(), m_rectGraph.Height()) ;
	oldBitmap = (CBitmap *)TempDC.SelectObject(&TempBitmap) ;

	// 填充背景颜色
	OnFillBkColor(&TempDC, m_rectGraph);
	if (TempDC.GetSafeHdc() != NULL)
	{
		if (uiOpt == OptGraphRectAddRight)
		{
			TempDC.BitBlt(0, 0, m_rectGraph.Width() - m_uiGridX - m_uiGridLineX, m_rectGraph.Height(), 
				&m_dcGraph, 0, 0, SRCCOPY) ;
		}
		else if (uiOpt == OptGraphRectDelRight)
		{
			TempDC.BitBlt(0, 0, m_rectGraph.Width(), m_rectGraph.Height(), 
				&m_dcGraph, 0, 0, SRCCOPY) ;
		}
		if (m_pbitmapOldGrid != NULL)
		{
			m_dcGraph.SelectObject(m_pbitmapOldGrid) ; 
		}
		m_dcGraph.DeleteDC();
		m_dcGraph.CreateCompatibleDC(&TempDC) ;
		bitmapGrid.CreateCompatibleBitmap(&TempDC, m_rectGraph.Width(), m_rectGraph.Height()) ;
		m_pbitmapOldGrid = m_dcGraph.SelectObject(&bitmapGrid) ;
		OnFillBkColor(&m_dcGraph, m_rectGraph);
		m_dcGraph.BitBlt(0, 0, m_rectGraph.Width(), m_rectGraph.Height(), 
			&TempDC, 0, 0, SRCCOPY) ;
	}
	TempDC.SelectObject(oldBitmap) ;
}

// 在图形界面上输出文字信息
void CInstrumentGraph::OnShowTextOut(CDC* pDC, CPoint point, CString str)
{
	CRect oRect;
	oRect.left = 0;;
	oRect.right = m_rectClient.right - m_iVScrBarInterval;
	oRect.top = m_rectClient.bottom - m_iHScrBarInterval - m_iPosShowInterval;
	oRect.bottom = oRect.top + m_iPosShowInterval;
	pDC->TextOut (point.x, point.y, str) ;
	InvalidateRect( oRect, FALSE );
}

// 重置
void CInstrumentGraph::OnReset(void)
{
	m_iRightMovePos = 0;
	m_iHScrPos = 0;
	m_rectGraph = m_rectClient;
	m_rectGraph.right -= m_iVScrBarInterval;
	m_rectGraph.bottom -= m_iHScrBarInterval;
	CClientDC dc(this) ;  
	CBitmap bitmapGrid;
	// 如果还没有绘图DC，则为其设置一个内存DC
	if (m_dcGraph.GetSafeHdc() != NULL)
	{
		if (m_pbitmapOldGrid != NULL)
		{
			m_dcGraph.SelectObject(m_pbitmapOldGrid) ; 
		}
		m_dcGraph.DeleteDC();
	}
	m_dcGraph.CreateCompatibleDC(&dc) ;
	bitmapGrid.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height()) ;
	m_pbitmapOldGrid = m_dcGraph.SelectObject(&bitmapGrid) ;
	OnFillBkColor(&m_dcGraph, m_rectGraph);
	m_oInstrumentGraphRectList.RemoveAll();
	m_pWndVScr->EnableWindow(FALSE);
	m_pWndHScr->EnableWindow(FALSE);
}

void CInstrumentGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint oGraphPoint;
	m_oGraph oInstrumentGraph;
	unsigned int uiIP = 0;
	oGraphPoint.x = point.x + m_iHScrPos;
	oGraphPoint.y = point.y;
	if (TRUE == OnSelectInstrument(oGraphPoint, oInstrumentGraph))
	{
		if (oInstrumentGraph.uiType == InstrumentTypeFDU)
		{
			uiIP = IPSetAddrStart + oInstrumentGraph.iUnitIndex * IPSetAddrInterval;
			::SendMessage(m_pWnd->m_hWnd, WM_DLGADCGRAPHSHOW, uiIP, oInstrumentGraph.uiFDUIndex);
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}
// 将信息显示区域填充为背景色
void CInstrumentGraph::OnFillMsgAreaBkColor(void)
{
	CRect oShowRect;
	oShowRect.left = 0;
	oShowRect.right = m_rectGraph.right;
	oShowRect.top = m_rectGraph.bottom - m_iPosShowInterval;
	oShowRect.bottom = m_rectGraph.bottom;
	OnFillBkColor(&m_dcGraph, oShowRect);
}