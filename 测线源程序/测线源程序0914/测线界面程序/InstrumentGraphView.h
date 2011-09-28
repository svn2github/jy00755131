#pragma once

#include "MainFrm.h"
#include "SiteData.h"
#include "StaticFloat.h"

// CInstrumentGraphView 视图

class CInstrumentGraphView : public CScrollView
{
	DECLARE_DYNCREATE(CInstrumentGraphView)

protected:
	CInstrumentGraphView();           // 动态创建所使用的受保护的构造函数
	virtual ~CInstrumentGraphView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGraphViewInstrumentSetMarker();
	afx_msg void OnGraphViewInstrumentProperties();
	afx_msg void OnGraphViewInstrumentSetAuxiliary();
	afx_msg void OnGraphViewInstrumentSetLowBoxDetour();
	afx_msg void OnGraphViewInstrumentSetHighBoxDetour();
	afx_msg void OnGraphViewInstrumentFormLine();
	afx_msg void OnPopupGraphviewInstrumentSetinstrument();

	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

public: // 属性
	/**现场数据对象*/
	CSiteData*	m_pSiteData;

	/**单元格X方向尺寸*/
	int m_iGridX;
	/**单元格Y方向尺寸*/
	int m_iGridY;

	/**单元格X方向尺寸，上一次*/
	int m_iGridXPre;
	/**单元格Y方向尺寸，上一次*/
	int m_iGridYPre;

	/**图形区X方向尺寸*/
	int m_iPaperX;
	/**图形区Y方向尺寸*/
	int m_iPaperY;

	/**放大倍数*/
	float m_fZoomRate;

	/**滚动条位置*/
	CPoint m_oPointScrollPos;
	/**鼠标键按下时鼠标点*/
	CPoint m_oPointBase;
	/**鼠标键释放时鼠标点*/
	CPoint m_oPointLast;
	/**鼠标左键是否按下*/
	bool m_bMouseDownLeft;
	/**鼠标中间键是否按下*/
	bool m_bMouseDownMiddle;

	/**是否画鼠标中间键选中区域位置*/
	bool m_bDrawSelectArea;
	/**鼠标中间键选中区域位置，单位像素*/
	CRect m_oSelectAreaPoint;
	/**鼠标中间键选中区域位置，单位单元格*/
	CRect m_oSelectAreaIndex;

	/**选中单元列索引*/
	int m_iSelectUnitColumnIndex;
	/**选中单元行索引*/
	int m_iSelectUnitRowIndex;

	/**选中连接线列索引*/
	int m_iSelectLinkColumnIndex;
	/**选中连接线行索引*/
	int m_iSelectLinkRowIndex;
	/**选中连接线方向 2：下；4：右*/
	int m_iSelectLinkDirection;

	/**选中区域对应的绝对排列*/
	CString m_strAbsoluteSpread;

	/**迂回道低端仪器*/
	CGraphInstrumentData* m_pGraphInstrumentDataDetourLow;
	/**迂回道高端仪器*/
	CGraphInstrumentData* m_pGraphInstrumentDataDetourHigh;

	/**测试值漂浮窗口*/
	CStaticFloat m_wndStaticFloat;

public: // 方法
	// 放大
	void OnZoomIn();
	// 局部放大
	void OnZoomInDrag();
	// 导航局部放大
	void OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY);
	// 缩小
	void OnZoomOut();
	// 现场数据对象初始化
	void OnSiteDataInit();

	// 响应设置测线数量
	void OnSetLineCount();

	// 加载现场数据
	void OnLoadSiteData();
	// 重新加载现场数据
	void OnReloadSiteDataCMD();

	// 绘图前设置绘图参数
	void SetupPageSize();
	// 缩放图形
	void OnZoomGraph();
	// 得到点坐标对应的单元
	void GetUnitUnderPoint(CPoint* pPoint);
	// 得到选中的区域
	bool GetSelectAreaPoint();
	// 得到选中区域的行坐标和列坐标区域
	void GetSelectAreaIndex();
	// 把选中区域转换为绝对排列字符串
	void ConvertSelectAreaToAbsoluteSpreadString();
	// 判断点是否在绘图区域内
	bool JudgeIfPointInPaper(CPoint* pPoint);
	// 显示单元的测线号和测点号
	void OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 在浮动窗口中显示测试值
	void OnShowTestValueInFloatWindow(CInstrument* pInstrument, CPoint pointClient);
	// 响应显示属性值
	void OnShowProperty(unsigned int uiIndex);
	// 响应显示迂回道图形视图
	void OnShowDetourGraphView(unsigned int uiIndex);

	// 画鼠标中间键选中区域
	void DrawSelectArea(CDC* pDC);
	// 画迂回道标记
	void DrawDetourMarker(CDC* pDC);
	// 画迂回道标记
	void DrawDetourMarker(CDC* pDC, CGraphInstrumentData* m_pGraphInstrumentData);
	// 清除迂回道标记
	void ClearDetourMarker();
	// 清除迂回道标记
	void ClearDetourMarker(CGraphInstrumentData* m_pGraphInstrumentData);

	// 为新的测试数据刷新显示
	void OnRefreshForNewTestValue(int iTestType);
	// 点击GO按钮则清空选中设备的测试结果
	void OnClearCheckInstrumentTestResult(unsigned int uiTestType);
};


