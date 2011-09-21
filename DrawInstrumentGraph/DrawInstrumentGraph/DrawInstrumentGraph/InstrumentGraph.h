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
	// »æÖÆÒÇÆ÷Í¼ÐÎ
	void OnDrawInstrumentGraph(void);
	afx_msg void OnPaint();

};


