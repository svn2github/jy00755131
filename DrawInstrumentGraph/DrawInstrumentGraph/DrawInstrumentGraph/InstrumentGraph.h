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
	CRect  m_rectClient;
	CBrush m_brushBack;

	CDC     m_dcGraph;
	CBitmap *m_pbitmapOldGrid;
	CBitmap m_bitmapGrid;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 绘制仪器图形
	void OnDrawInstrumentGraph(void);
	afx_msg void OnPaint();

	// 绘制所有仪器
	void DrawUnitAll(CDC* pDC, unsigned int uiGridX, unsigned int uiGridY, unsigned int uiGridLineX, unsigned int uiGridLineY);
	// 绘制仪器单元（包含连接线）
	void DrawUnit(CDC* pDC, unsigned int uiGridX, unsigned int uiGridY, unsigned int uiGridLineX, unsigned int uiGridLineY, 
		int iUnitIndex, int iLineNum, unsigned int uiLineDirection, unsigned int uiType);
private:
	// 开始绘制仪器时X轴坐标
	int m_iPosX;
	// 开始绘制仪器时Y轴坐标
	int m_iPosY;
};


