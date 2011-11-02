#pragma once

// CInstrumentGraph

class CInstrumentGraph : public CWnd
{
	DECLARE_DYNAMIC(CInstrumentGraph)

public:
	CInstrumentGraph();
	virtual ~CInstrumentGraph();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 客户区尺寸
	CRect  m_rectClient;
	// 背景刷子
	CBrush m_brushBack;
	CBitmap *m_pbitmapOldGrid;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

	// 绘制所有仪器
	void DrawUnitAll(void);
	// 绘制仪器单元（包含连接线）
	void DrawUnit(int iUnitIndex, int iLineIndex, unsigned int uiLineDirection, unsigned int uiType, unsigned int uiSN, unsigned int uiOpt, bool bSet);
private:
	CDC m_dcGraph;
	// 绘图区尺寸
	CRect m_rectGraph;
	// 仪器图元结构体
	typedef struct InstrumentGraph
	{
		// 仪器绘图区域
		CRect oRect;
		// 点号
		int iUnitIndex;
		// 测线号
		int iLineIndex;
		// SN号
		unsigned int uiSN;
		// 连接线方向
		unsigned int uiLineDirection;
		// 仪器类型
		unsigned int uiType;
		// 仪器操作
		unsigned int uiOpt;
		bool operator == (const InstrumentGraph& v2) const
		{
			return ((iUnitIndex == v2.iUnitIndex)
				&& (iLineIndex == v2.iLineIndex));
		}
	}m_oInstrumentGraph;
	// 存储仪器图元绘图区
	CList <m_oInstrumentGraph, m_oInstrumentGraph> m_oInstrumentGraphRectList;
	// 纵向滚动条CWnd指针
	CWnd * m_pWndVScr;
	// 横向滚动条CWnd指针
	CWnd * m_pWndHScr;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	// 设备绘图单元X方向尺寸
	unsigned int m_uiGridX;
	// 设备Y方向绘图尺寸
	unsigned int m_uiGridY;
	// 连线X方向绘图尺寸
	unsigned int m_uiGridLineX;
	// 连线Y方向绘图尺寸
	unsigned int m_uiGridLineY;
	// 显示仪器坐标（包含测线号和点号）
	void OnShowInstrumentAxisPoint(CPoint oGraphPoint);
	// 滚动条拖动
	int moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	// 填充背景颜色
	void OnFillBkColor(CDC * pDC, CRect oRect);
	// 清除所有仪器图形
	void OnClearAllInstrumentGraph(void);
private:
	// 坐标信息
	int m_iPosShowInterval;
	// 纵向滚动条宽度
	int m_iVScrBarInterval;
	// 横向滚动条宽度
	int m_iHScrBarInterval;
	// 主交叉站仪器起始位置X轴坐标
	int m_iLauxPosX;
	// 主交叉站仪器起始位置Y轴坐标
	int m_iLauxPosY;
public:
	// 右侧增加仪器移动图形DC的大小
	int m_iRightMovePos;
	// 处理绘图区域
	void OnOptGraphRect(unsigned int uiOpt);
	// 在图形界面上输出文字信息
	void OnShowTextOut(CDC* pDC, CPoint point, CString str);
private:
	// 横向滚动条坐标
	int m_iHScrPos;
public:
	// 重置
	void OnReset(void);
};


