// OScopeCtrl.h : header file
//

#ifndef __OScopeCtrl_H__
#define __OScopeCtrl_H__

/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl window

class COScopeCtrl : public CWnd
{
	// Construction
public:
	COScopeCtrl();

	// Attributes
public:
	void AppendPoint(double* dpNewPoint, UINT uiNewPointSize);
	void SetRange(double dLower, double dUpper, int nDecimalPlaces=1);
	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void InvalidateCtrl(bool bXMaxLableDefault);
	void DrawPoint(double dNewPoint, UINT uiDrawPointMove);
	  // vivi 2010.11.29
	// 滚动条右移，图像左移
	void LeftDrawPoint(double* dSampleData, UINT uiLength, int iMathValueIdNum, UINT uiLeftDrawPointNum);
	// 滚动条左移，图像右移
	void RightDrawPoint(double* dSampleData, UINT uiLength, int iMathValueIdNum, UINT uiRightDrawPointNum);

	void Reset(bool bXMaxLableDefault);
	void DrawPoint(double dNewPoint);

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COScopeCtrl)
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
	//}}AFX_VIRTUAL

	// Implementation
public:
	int m_nShiftPixels;          // amount to shift with each new point 
	int m_nYDecimals;

	CString m_strXUnitsString;
	CString m_strYUnitsString;

	COLORREF m_crBackColor;        // background color
	COLORREF m_crGridColor;        // grid color
	COLORREF m_crPlotColor;        // data color  

	double m_dCurrentPosition;   // current position
	double m_dPreviousPosition;  // previous position

	// vivi 2010.11.29
	UINT m_uiXAxisLabelMin;	// X轴标签的最小值
	UINT m_uiXAxisLabelMax;	// X轴标签的最大值
	UINT m_uiXAxisPointNum;	// X轴可同时容纳点的个数
	UINT m_uiPointNum;			// 绘制了的点的个数
	CWnd* m_pParentWnd;	// 当前窗口句柄
	int m_iMousepointX;		// 鼠标点X轴的值
	double m_dMousepointY;	// 鼠标点Y轴的值

	virtual ~COScopeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COScopeCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	// vivi 2010.11.29
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nHalfShiftPixels;
	int m_nPlotShiftPixels;
	int m_nClientHeight;
	int m_nClientWidth;
	int m_nPlotHeight;
	int m_nPlotWidth;

	double m_dLowerLimit;        // lower bounds
	double m_dUpperLimit;        // upper bounds
	double m_dRange;
	double m_dVerticalFactor;

	CRect  m_rectClient;
	CRect  m_rectPlot;
	CPen   m_penPlot;
	CBrush m_brushBack;

	CDC     m_dcGrid;
	CDC     m_dcPlot;
	CBitmap* m_pbitmapOldGrid;
	CBitmap* m_pbitmapOldPlot;
	CBitmap m_bitmapGrid;
	CBitmap m_bitmapPlot;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	// 放大拖动的起始点坐标
	POINT m_pointStart;
	// 鼠标左键按下标志位，为TRUE=按下左键，为FALSE=没按下左键
	BOOL m_bLButtonDown;
	// 鼠标拖动的结束点坐标
	POINT m_pointEnd;
	// 判断鼠标拖动以绘制矩形，TRUE=绘制矩形，FALSE=不绘制
	BOOL m_bMouseDrag;
	// 鼠标拖动当前点坐标
	POINT m_pointMouseDragCur;
	// 绘制鼠标拖动放大矩形框
	void OnDrawMouseDragRect(CDC* pDC, POINT StartPoint, POINT EndPoint);
	// 采样停止标志位指针
	BOOL m_bStopSample;
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};

/////////////////////////////////////////////////////////////////////////////
#endif
