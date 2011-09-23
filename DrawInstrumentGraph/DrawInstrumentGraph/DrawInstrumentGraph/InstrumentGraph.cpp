// InstrumentGraph.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawInstrumentGraph.h"
#include "InstrumentGraph.h"

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
// 开始绘制仪器时X轴坐标
#define GraphPosX				100
// 开始绘制仪器时Y轴坐标
#define GraphPosY				100
// 仪器类型
// 仪器类型-采集站
#define InstrumentTypeFDU				3
// 仪器类型-电源站
#define InstrumentTypeLAUL			2 
// 仪器类型-交叉站
#define InstrumentTypeLAUX			1

// CInstrumentGraph

IMPLEMENT_DYNAMIC(CInstrumentGraph, CWnd)

CInstrumentGraph::CInstrumentGraph()
{
	m_brushBack.CreateSolidBrush(GraphBkColor) ;
	m_iPosX = GraphPosX;
	m_iPosY = GraphPosY;
	// protected bitmaps to restore the memory DC's
	m_pbitmapOldGrid = NULL ;
	m_oInstrumentGraphRectList.RemoveAll();
	m_oRectMove = 0;
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
	// 如果还没有绘图DC，则为其设置一个内存DC
	if (m_dcGraph.GetSafeHdc() == NULL)
	{
		m_dcGraph.CreateCompatibleDC(&dc) ;
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height()) ;
		m_pbitmapOldGrid = m_dcGraph.SelectObject(&m_bitmapGrid) ;
	}

	m_dcGraph.SetBkColor (GraphBkColor) ;
	// 填充背景颜色
	m_dcGraph.FillRect(m_rectClient, &m_brushBack) ;
	// 绘制仪器设备图
	DrawUnitAll(&m_dcGraph, GraphGridInstrumentSizeX, GraphGridInstrumentSizeY, GraphGridLineSizeX, GraphGridLineSizeY);
	// 刷新客户区界面
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

// 绘制所有仪器
// pDC	CDC指针
// uiGridX	单元格X轴方向大小
// uiGridY	单元格Y轴方向大小
void CInstrumentGraph::DrawUnitAll(CDC* pDC, unsigned int uiGridX, unsigned int uiGridY, unsigned int uiGridLineX, unsigned int uiGridLineY)
{
	int iUnitIndex = 0;
	int iLineNum = 0;
	unsigned int uiLineDirection = 0;
	unsigned int uiType = InstrumentTypeLAUX;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	uiType = InstrumentTypeFDU;
	iUnitIndex = 1;
	uiLineDirection = 4;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iUnitIndex = 2;
	uiLineDirection = 4;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iUnitIndex = -1;
	uiLineDirection = 3;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iUnitIndex = -2;
	uiLineDirection = 3;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);

	iLineNum = 1;
// 	iUnitIndex = 0;
// 	uiLineDirection = 0;
// 	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, uiLineNum, uiLineDirection, uiType);
	iUnitIndex = 1;
	uiLineDirection = 4;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iUnitIndex = 2;
	uiLineDirection = 4;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iUnitIndex = -3;
	uiLineDirection = 4;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iLineNum = -1;
	iUnitIndex = -3;
	uiLineDirection = 4;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iLineNum = 1;
	uiLineDirection = 2;
	iUnitIndex = 0;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iLineNum = -1;
	uiLineDirection = 1;
	iUnitIndex = 0;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iLineNum = -2;
	uiLineDirection = 1;
	iUnitIndex = 0;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
	iLineNum = 2;
	uiLineDirection = 2;
	iUnitIndex = 0;
	DrawUnit(pDC, uiGridX, uiGridY, uiGridLineX, uiGridLineY, iUnitIndex, iLineNum, uiLineDirection, uiType);
}

// 绘制仪器单元（包含连接线）
// pDC								CDC指针
// uiGridX						单元格X轴方向大小
// uiGridY						单元格Y轴方向大小
// uiUnitIndex					此仪器的图元序号
// uiLineNum					测线号
// uiLineDirection			连接线方向：1为上，2为下，3为左，4为右，无连接线为0
// 连接线图元尺寸和仪器图元尺寸相等
// uiType							仪器类型
void CInstrumentGraph::DrawUnit(CDC* pDC, unsigned int uiGridX, unsigned int uiGridY, unsigned int uiGridLineX, unsigned int uiGridLineY, 
								int iUnitIndex, int iLineNum, unsigned int uiLineDirection, unsigned int uiType)
{
	CPen oPenUnit;
	CPen* pOldPen;
	CDC m_MemDc;			//定义一个DC
	CBitmap m_BkBmp;		//定义一个位图对象
	BITMAP m_BmpInfo;	//定义一个位图信息结构体
	CRect oRectLine;
	CRect oRectInstrument;

	oPenUnit.CreatePen(PS_SOLID, 2, GraphLineColor);
	pOldPen = pDC->SelectObject(&oPenUnit);
	if ((uiLineDirection == 1) || (uiLineDirection == 2))
	{
		oRectLine.left = m_iPosX + iUnitIndex * (uiGridX + uiGridLineX);
		oRectLine.right = oRectLine.left + uiGridX;
		oRectInstrument.left = oRectLine.left;
		oRectInstrument.right = oRectLine.right;
		if (uiLineDirection == 1)
		{
			oRectLine.top = m_iPosY + iLineNum * (uiGridY + uiGridLineY) + uiGridY;
		}
		else
		{
			oRectLine.top = m_iPosY + iLineNum * (uiGridY + uiGridLineY) - uiGridLineY;
		}
		oRectLine.bottom = oRectLine.top + uiGridLineY;
		oRectInstrument.top = m_iPosY + iLineNum * (uiGridY + uiGridLineY);
		oRectInstrument.bottom = oRectInstrument.top + uiGridY;
		pDC->MoveTo(CPoint((oRectLine.left + oRectLine.right) / 2, oRectLine.top));
		pDC->LineTo(CPoint((oRectLine.left + oRectLine.right) / 2, oRectLine.bottom));
	}
	else if ((uiLineDirection == 3) || (uiLineDirection == 4))
	{
		// 连接线绘制区域
		if (uiLineDirection == 3)
		{
			oRectLine.left = m_iPosX + iUnitIndex * (uiGridX + uiGridLineX) + uiGridX;
		}
		else
		{
			oRectLine.left = m_iPosX + iUnitIndex * (uiGridX + uiGridLineX) - uiGridLineX;
		}
		oRectLine.right = oRectLine.left + uiGridLineX;
		oRectInstrument.left = m_iPosX + iUnitIndex * (uiGridX + uiGridLineX);
		oRectInstrument.right = oRectInstrument.left + uiGridX;
		oRectLine.top = m_iPosY + iLineNum * (uiGridY + uiGridLineY);
		oRectLine.bottom = oRectLine.top + uiGridY;
		oRectInstrument.top = oRectLine.top;
		oRectInstrument.bottom = oRectLine.bottom;
		pDC->MoveTo(CPoint(oRectLine.left, (oRectLine.top + oRectLine.bottom) / 2));
		pDC->LineTo(CPoint(oRectLine.right, (oRectLine.top + oRectLine.bottom) / 2));
	}
	else
	{
		// 主交叉站不需要画连接线iUnitIndex = 0
		oRectInstrument.left = m_iPosX;
		oRectInstrument.right = oRectLine.left + uiGridX;
		oRectInstrument.top = m_iPosY;
		oRectInstrument.bottom = oRectLine.top + uiGridY;
	}
	if (uiType == InstrumentTypeFDU)
	{
		m_BkBmp.LoadBitmap(IDB_BITMAP_FDU2);	// 此处为刚刚添加进来的位图资源ID
	}
	else if (uiType == InstrumentTypeLAUX)
	{
		m_BkBmp.LoadBitmap(IDB_BITMAP_LAUX2);
	}
	m_BkBmp.GetBitmap(&m_BmpInfo);				// 获取位图高宽等信息，保存在位图结构体中
	m_MemDc.CreateCompatibleDC(pDC);			// 创建一个兼容屏幕DC的内存DC：m_MemDc。
	m_MemDc.SelectObject(&m_BkBmp);				// 将该位图选择到刚创建的内存DC中。

	/*将内存DC里的东西贴到屏幕DC上去*/
	pDC->BitBlt(oRectInstrument.left,oRectInstrument.top,m_BmpInfo.bmWidth,m_BmpInfo.bmHeight,&m_MemDc,0,0,SRCCOPY);
	pDC->SelectObject(pOldPen);
	m_oInstrumentGraph oInstrumentGraph;
	oInstrumentGraph.oRect = oRectInstrument;
	oInstrumentGraph.iUnitIndex = iUnitIndex;
	oInstrumentGraph.iLineNum = iLineNum;
	m_oInstrumentGraphRectList.AddTail(oInstrumentGraph);
}

void CInstrumentGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	CRect oRect;
	POSITION pos = m_oInstrumentGraphRectList.GetHeadPosition();
	m_oInstrumentGraph oInstrumentGraph;
	if ((m_oRectMove.left <= point.x) && (m_oRectMove.right >= point.x)
		&&(m_oRectMove.top <= point.y) && (m_oRectMove.bottom >= point.y))
	{
	}
	else
	{
		for (int i=0; i<m_oInstrumentGraphRectList.GetCount(); i++)
		{
			oInstrumentGraph = m_oInstrumentGraphRectList.GetNext(pos);
			oRect = oInstrumentGraph.oRect;
			if ((oRect.left <= point.x) && (oRect.right >= point.x)
				&&(oRect.top <= point.y) && (oRect.bottom >= point.y))
			{
				m_dcGraph.SetTextAlign (TA_LEFT|TA_TOP) ;
				str.Format(_T("线号 = %d 点号 = %d"), oInstrumentGraph.iLineNum, oInstrumentGraph.iUnitIndex);
				m_dcGraph.TextOut (point.x, point.y, str) ;
				InvalidateRect( m_rectClient, FALSE );
				m_oRectMove = oInstrumentGraph.oRect;
				break;
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}
