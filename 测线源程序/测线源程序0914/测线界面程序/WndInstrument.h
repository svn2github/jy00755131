#pragma once


// CWndInstrument

class CWndInstrument : public CBCGPStatic
{
	DECLARE_DYNAMIC(CWndInstrument)

public:
	CWndInstrument();
	virtual ~CWndInstrument();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

public:	// 属性
	/** 仪器总数*/
	unsigned int m_uiInstrumentCountAll;

	/** 图形单元间隔X*/
	int m_iGridX;
	/** 图形单元间隔Y*/
	int m_iGridY;
	/** 图形单元宽度*/
	int m_iUnitWidth;
	/** 图形单元高度*/
	int m_iUnitHight;
	/** 图形高度X*/
	int m_iPaperHight;
	/** 画图区域X*/
	CRect m_oRect;

public:	// 方法
	// 响应显示迂回道图形视图
	void OnShowDetourGraphView(unsigned int uiCount);

	// 画背景
	void DrawBackground(CDC* pDC);
	// 画连接线
	void DrawLinkLine(CDC* pDC);
	// 画单元图
	void DrawUnit(CDC* pDC, int iIndex);
};


