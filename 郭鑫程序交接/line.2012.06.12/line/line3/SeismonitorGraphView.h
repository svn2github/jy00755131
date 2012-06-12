#pragma once

#include "MainFrm.h"
#include "SiteData.h"
#include "StaticFloat.h"

// CSeismonitorGraphView 视图

class CSeismonitorGraphView : public CScrollView
{
	DECLARE_DYNCREATE(CSeismonitorGraphView)

protected:
	CSeismonitorGraphView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSeismonitorGraphView();

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
	afx_msg void OnGraphViewSensorProperties();
	afx_msg void OnGraphViewSensorSetAuxiliary();
	afx_msg void OnGraphViewSensorSetMute();

	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

public: // 属性
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

	/**选中区域对应的绝对排列*/
	CString m_strAbsoluteSpread;

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
	// 得到选中的区域
	bool GetSelectAreaPoint();
	// 得到选中区域的行坐标和列坐标区域
	void GetSelectAreaIndex();
	// 把选中区域转换为绝对排列字符串
	void ConvertSelectAreaToAbsoluteSpreadString();
	// 显示单元的测线号和测点号
	void OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint);
	// 在浮动窗口中显示测试值
	void OnShowTestValueInFloatWindow(matrix_data::data_base_helper::const_device_info& di, CPoint pointClient);
	// 响应显示属性值
	void OnShowProperty(matrix_data::data_base_helper::const_device_info& di);
	// 画鼠标中间键选中区域
	void DrawSelectArea(CDC* pDC);
	// 为新的测试数据刷新显示
	void OnRefreshForNewTestValue();
private:
	void load_bitmaps();
	void set_scroll_size(){SetScrollSizes(MM_TEXT, paper_size_);	};
	void draw_mark_line_and_point(CDC* pDC, int iGridX, int iGridY);
	void draw_mark_point(CDC* pDC, int iGridX);
	void draw_mark_line(CDC* pDC, int iGridY);
	void draw_device_unit(CDC* pDC, int iGridX, int iGridY, int test_type, CBitmap* bitmap_ptr);
	void draw_devices(CDC* pDC, int iGridX, int iGridY, int test_type,  float sensmonitor_limit);
	CBitmap* get_bitmap_ptr(int grid_x, 	int type, int status);
	void show_context_menu(CPoint& point, CPoint& client_point);
	bool get_line_and_point_nb_according_to_point(CPoint& point, int& line_idx, int& point_nb);
	bool judge_if_point_in_paper(CPoint& point);
	void show_device_info(CPoint& point);
	void zoom_in_drag( const CRect& client_rect, const CRect& select_area, CPoint& point_scroll_pos);
	void zoom_graph();
	void set_paper_size(){
		CLineApp* app = (CLineApp*)AfxGetApp();
		unsigned int line_number = app->get_data_base_helper()->get_line_number();
		unsigned int section_number = app->get_data_base_helper()->get_section_number();
		paper_size_.cx = (section_number*2 - 1) * grid_x_ + paper_head_left_ * 2 + grid_x_max_;
		paper_size_.cy = (line_number *2 - 1) * grid_y_ + paper_head_top_ *  2 + grid_y_max_;
	};
	bool get_device_info_from_point(CPoint& point, matrix_data::data_base_helper::device_info& di);
private:
	static const int paper_head_left_ = 50;	//!< 图形区左边预留尺寸
	static const int paper_head_top_ = 50;//!< 图形区左边预留尺寸
	static const int bitmap_array_size_ = 4;//!<存放设备图片数组大小

	CBitmap lci_bitmap_[bitmap_array_size_]; //!< 存放lci显示图片
	CBitmap aux_bitmap_[bitmap_array_size_];//!< 存放aux显示图片
	CBitmap aul_bitmap_[bitmap_array_size_];//!< 存放aul显示图片
	CBitmap fdu_bitmap_[bitmap_array_size_];//!< 存放fdu显示图片

	unsigned int grid_x_; //!<单元格X方向尺寸
	unsigned int grid_y_; //!< 单元格Y方向尺寸
	unsigned int grid_x_prev_; //!<单元格X方向尺寸
	unsigned int grid_y_prev_; //!< 单元格Y方向尺寸

	int unit_width_min_;	//!< 图形单元宽度，最小尺寸模式
	int unit_hight_min_;	//!< 图形单元高度，最小尺寸模式
	int unit_width_max_;	//!< 图形单元宽度，最大尺寸模式	
	int unit_hight_max_;	//!< 图形单元高度，最大尺寸模式

	int grid_x_min_;	//!< 单元格X方向最小尺寸
	int grid_y_min_;	//!< 单元格Y方向最小尺寸
	int grid_x_max_;	//!< 单元格X方向最大尺寸
	int grid_y_max_;//!< 单元格Y方向最大尺寸

	SIZE paper_size_;//!< 画图区域大小
	SIZE center_grid_;//!< 中心位置坐标
	unsigned int current_bitmap_array_index_; //!< 当前使用的显示图索引,使用它来计算标尺间隔
	float zoom_in_rate_; //!<当前缩放比例,参见matrix_data
	matrix_data::data_base_helper::device_info device_info_;//!<当前使用上下文菜单所选中的设备信息
};


