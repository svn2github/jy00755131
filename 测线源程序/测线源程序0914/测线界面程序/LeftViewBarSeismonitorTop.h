#pragma once

/**
*@brief 检波器视图左侧子视图上部类
*/
class CLeftViewBarSeismonitorTop : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CLeftViewBarSeismonitorTop)

public:
	CLeftViewBarSeismonitorTop();
	virtual ~CLeftViewBarSeismonitorTop();

protected:
	//{{AFX_MSG(CLeftViewBarSeismonitorTop)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	// 属性

public:	// 方法
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public: // 属性
	/** 画图区域X*/
	CRect m_oRect;

	/** 鼠标键按下时鼠标点*/
	CPoint m_oPointBase;
	/** 鼠标键释放时鼠标点*/
	CPoint m_oPointLast;
	/** 鼠标键选中区域位置，单位像素*/
	CRect m_oSelectArea;

	/** 鼠标左键是否按下*/
	bool m_bMouseDownLeft;
	/** 是否画鼠标选中区域位置*/
	bool m_bDrawSelectArea;

	/** 放大倍数*/
	float m_fZoomRate;
	/** 基准点X*/
	float m_fRateX;
	/** 基准点Y*/
	float m_fRateY;


	/** 单元格Y方向尺寸*/
	int m_iGridY;
	/** 单元格Y方向尺寸最大值*/
	int m_iGridYMax;
	/** 测线高度*/
	int m_iLineHight;
	/** 图形区X坐标预留尺寸*/
	int m_iPaperHeadX;
	/** 图形区左边预留尺寸*/
	int m_iPaperHeadTop;
	/** 是否可以画图*/
	bool m_bCanDrawGraph;

	/** 测线数量*/
	unsigned int m_uiLineCount;

public: // 方法
	// 导航局部放大
	void OnZoomInNavigater();
	// 响应设置测线数量
	void OnSetLineCount(unsigned int uiLineCount);

	// 得到选中的区域
	bool GetSelectAreaPoint();
	// 判断当前图形大小是否可以画图
	bool CheckIfCanDraw();

	// 画背景
	void DrawBackground(CDC* pDC);
	// 画鼠标选中区域
	void DrawSelectArea(CDC* pDC);
	// 画测线
	void DrawLines(CDC* pDC);
};


