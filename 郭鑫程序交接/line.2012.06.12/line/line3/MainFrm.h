// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#define CMDIFrameWnd CBCGPMDIFrameWnd

#include "SiteData.h"
#include "outputview.h"
#include "DlgFormLineSetup.h"
//2012-02-28
#include "DlgSurveySetup.h"
#include "DlgLayoutSetup.h"
#include "DlgSpreadSetup.h"
#include "DlgLookSetup.h"
#include "DlgTestBaseSetup.h"
#include "DlgTestSetup.h"
#include "data.base.helper.h"

class CMyBCGPDockingControlBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CMyBCGPDockingControlBar)
public:
	CMyBCGPDockingControlBar();
	virtual ~CMyBCGPDockingControlBar();
	virtual void OnShowProperty(matrix_data::data_base_helper::device_info& di);
};

class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	enum __enuLINE_GRAPH_VIEW_TYPE
	{
		INSTRUMENT_GRAPH_VIEW_TYPE,
		SENSOR_GRAPH_VIEW_TYPE,
		SEISMONITOR_GRAPH_VIEW_TYPE,
	};

public:
	CMainFrame();

// Attributes
public: // 属性
	/** 现场数据对象*/
	CSiteData m_oSiteData;

	/** FormLine设置对话框*/
	CDlgFormLineSetup* m_pDlgFormLineSetup;

 	/**Field ON*/
	byte m_byFieldOperation;

	//2012-02-28
	/** 地形设置对话框*/
	CDlgSurveySetup* m_pDlgSurveySetup;
    /** 布局设置对话框*/
	CDlgLayoutSetup* m_pDlgLayoutSetup;
	/** 排列设置对话框*/
	CDlgSpreadSetup* m_pDlgSpreadSetup;
	/** Look设置对话框*/
	CDlgLookSetup* m_pDlgLookSetup;
	/** 测试基本设置对话框*/
	CDlgTestBaseSetup* m_pDlgTestBaseSetup;
	/** 测试设置对话框*/
	CDlgTestSetup* m_pDlgTestSetup;

// Operations
public:

	void OnSiteDataInit();
	void OnSiteDataClose();
	void LoadSiteData();
// Overrides
public:

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;
	COutputViewBar			m_wndOutput;
	CBCGPToolBarImages		m_UserImages;

	/**File工具条*/
	CBCGPToolBar m_wndToolBarMatrixFile;
	CBCGPToolBar m_wndToolBarMatrixEdit;

	/**View工具条*/
	CBCGPToolBar m_wndToolBarMatrixView;
	/**Setup工具条*/
	CBCGPToolBar m_wndToolBarMatrixSetup;
	/**Setup工具条*/
	CBCGPToolBar m_wndToolBarMatrixField;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
	afx_msg void OnClose();

	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnMatrixFieldOn();	// Field On按钮操作
	afx_msg void OnMatrixFieldOff();	// Field Off按钮操作

	afx_msg void OnMatrixFormLineSetup();
	afx_msg void OnMatrixInstrumentGraph();
	afx_msg void OnMatrixSensorGraph();
	afx_msg void OnMatrixSurveySetup();	// 地形设置菜单命令
	afx_msg void OnMatrixLayoutSetup();	// 布局设置菜单命令
	afx_msg void OnMatrixSpreadSetup();	// 排列设置菜单命令
	afx_msg void OnMatrixLookSetup();	// Look设置菜单命令
	afx_msg void OnMatrixTestBaseSetup();	// 测试基本设置菜单命令
	afx_msg void OnMatrixTestSetup();	// 测试设置菜单命令
	afx_msg void OnMatrixSeismonitorGraph();	// 噪声监测图形视图菜单命令
	afx_msg void OnMatrixInstrumentlist();
	afx_msg void OnMatrixSensorlist();
	afx_msg LRESULT OnLoginServer(WPARAM,LPARAM); //定义开始登录服务器
	afx_msg LRESULT OnQueryRecvSection(WPARAM,LPARAM); //查询接收区域
	afx_msg LRESULT OnQueryRecvSectionResult(WPARAM,LPARAM); //查询接收区域
	afx_msg LRESULT OnFieldOnCompletely(WPARAM,LPARAM); //上电
	afx_msg LRESULT OnFieldOffCompletely(WPARAM,LPARAM); //下电
	afx_msg LRESULT OnUpdateDeviceTable(WPARAM,LPARAM); //更新设备列表
	afx_msg LRESULT OnQueryInstrumentTestBaseLimit(WPARAM,LPARAM); //查询 InstrumentTestBase XML文件信息（帧内容为空）
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);
	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/);
public:
	// 处理内部网络命令帧
	void OnProcInterFaceCommand(CFrameInterface* pFrameInterface);
	// 处理命令	100-现场数据输出更新通知命令
	void ProcInterFaceCommandForSiteDataOutput();
	// 响应仪器图形视图弹出菜单设置标记点
	void OnGraphViewInstrumentSetMarker(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应仪器图形视图弹出菜单设置辅助道
	void OnGraphViewInstrumentSetAuxiliary(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应仪器图形视图弹出菜单设置迂回道
	void OnGraphViewInstrumentSetDetour(unsigned int uiInstrumentTypeLow, unsigned int uiSNLow, unsigned int uiInstrumentTypeHigh, unsigned int uiSNHight);
	// 响应仪器图形视图弹出菜单FormLine设置
	void OnGraphViewInstrumentFormLine(unsigned int uiInstrumentType, unsigned int uiSN);
	// 响应检波器图形视图弹出菜单Mute设置
	void OnGraphViewSensorSetMute(unsigned int uiNbLine, unsigned int uiNbPoint);

	// 响应仪器图形视图弹出菜单FieldOn/Off操作
	void OnToolBarFieldOperation();

	// 重置工具条
	void OnResetToolBarMatrix();
	void output_log(LPTSTR msg){m_wndOutput.output(msg);};
public:
	//视图通用函数
	int get_current_line_index(int max_size, int current_size){ return (current_size - max_size /2);};
	int get_current_section_index(int max_size, int current_size){return (current_size - max_size /2) ;};
	void draw_mark_line_and_point(CDC* pDC, int iGridX, int iGridY);// 画测线标尺
	void draw_mark_line(CDC* pDC, int iGridY);// 画测线标尺和测点标尺
	void draw_mark_point(CDC* pDC, int iGridX);// 画测点标尺
	void set_scroll_size(CScrollView* view_ptr);//设置整个滚动屏幕大小
	void draw_devices(CDC* pDC, int iGridX, int iGridY, int test_type, bool aux_be_shown, bool aul_be_shown, bool fdu_be_shown);//加入设备到视图中
	void draw_device_unit(CDC* pDC, int iGridX, int iGridY, int test_type,  CBitmap* bitmap_ptr);//画出每一个设备
	void draw_devices_link_line(CDC* pDC, int iGridX, int iGridY);// 画图，单元连接线
	void draw_device_link_line(matrix_data::data_base_helper::device_info* di, CDC* pDC, int iGridX, int iGridY);// 画图，单元连接线rix
	void get_grid_size(int& grid_x, int& grid_y){grid_x = grid_x_; grid_y = grid_y_;};
	void zoom_in(CScrollView* , CPoint& );
	void zoom_out(CScrollView* view_ptr);
	void show_device_info(CMyBCGPDockingControlBar*, CPoint&, 	enum __enuLINE_GRAPH_VIEW_TYPE);
	void show_context_menu(CScrollView* view_ptr, CPoint& point, CPoint& client_point);
	bool get_device_info_from_point(CPoint& point, matrix_data::data_base_helper::device_info& di);
	void zoom_graph();
	void zoom_in_drag( const CRect& client_rect, const CRect& select_area, CPoint& point_scroll_pos);
	void show_property(CMyBCGPDockingControlBar* control_bar, matrix_data::data_base_helper::device_info& di, enum __enuLINE_GRAPH_VIEW_TYPE type);
	matrix_data::data_base_helper::device_info* get_device_info_ptr(){return &device_info_;};
	SIZE get_paper_size(){return paper_size_;};
	void zoom_in_navigator(const CRect& client_rect, CPoint& point_scroll_pos, float zoom_rate, float x_rate, float y_rate);
	SIZE get_grid_size(){	SIZE sz; sz.cx = grid_x_; sz.cy=grid_y_;	return sz;};

protected:
	void show_matrix_instrument_graph_view();
	void set_paper_size(unsigned int line_number, unsigned int section_number){
		paper_size_.cx = (section_number*2 - 1) * grid_x_ + paper_head_left_ * 2 + grid_x_max_;
		paper_size_.cy = (line_number *2 - 1) * grid_y_ + paper_head_top_ *  2 + grid_y_max_;
	};
	//根据传入宽带,返回所要使用的图标: 	1: 8*8		2: 16*16		3: 32*32		4: 48*48
	HICON get_icon(int grid_x/*x方向大小*/, 
		int type/*图表类型 1.aux 2.aul 3.fdu*/, 
		int status/*当前设备状态 1: OK 2: error*/); 

	CBitmap* get_bitmap_ptr(int grid_x/*x方向大小*/, 
		int type/*图表类型 1.aux 2.aul 3.fdu*/, 
		int status/*当前设备状态 1: OK 2: error*/); 

	bool judge_if_point_in_paper(CPoint& point);
	bool get_line_and_point_nb_according_to_point(CPoint& point, int& line_idx, int& point_nb);

private: // function
	void load_bitmaps();

public:
	static const int paper_head_left_ = 50;	//!< 图形区左边预留尺寸
	static const int paper_head_top_ = 50;//!< 图形区左边预留尺寸
	static const int bitmap_array_size_ = 4;//!<存放设备图片数组大小

private:

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
	/**是否画鼠标中间键选中区域位置*/
	bool be_draw_select_area_;
	matrix_data::data_base_helper::device_info device_info_;//!<当前使用上下文菜单所选中的设备信息
};


