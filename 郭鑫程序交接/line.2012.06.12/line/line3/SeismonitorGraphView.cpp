// SensorGraphView.cpp : 实现文件
//

#include "stdafx.h"
#include "line.h"
#include "SeismonitorGraphView.h"
#include "SeismonitorGraphChildFrame.h"

// CSeismonitorGraphView

IMPLEMENT_DYNCREATE(CSeismonitorGraphView, CScrollView)

CSeismonitorGraphView::CSeismonitorGraphView()
{
	ZeroMemory(&paper_size_, sizeof(paper_size_));
	ZeroMemory(&center_grid_, sizeof(center_grid_));
	grid_x_ = 8; //初始化时,图标为8*8 为5个设备
	grid_y_ = 32;//初始化时,,图标为8*8 为4个设备高度
	current_bitmap_array_index_ = 0x0;
	zoom_in_rate_ = 1.0;
	unit_width_min_ = 8;	//!< 图形单元宽度，最小尺寸模式
	unit_hight_min_ = 8;	//!< 图形单元高度，最小尺寸模式
	unit_width_max_ = 40;	//!< 图形单元宽度，最大尺寸模式	
	unit_hight_max_ = 40;	//!< 图形单元高度，最大尺寸模式

	grid_x_min_ = 8;	//!< 单元格X方向最小尺寸
	grid_y_min_ =   8;	//!< 单元格Y方向最小尺寸
	grid_x_max_ =  40;	//!< 单元格X方向最大尺寸
	grid_y_max_ =  40;//!< 单元格Y方向最大尺寸
	grid_y_prev_ = grid_y_;
	grid_x_prev_ = grid_x_;

	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bMouseDownMiddle = false;	// 鼠标中间键是否按下
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引
}

CSeismonitorGraphView::~CSeismonitorGraphView()
{
}


BEGIN_MESSAGE_MAP(CSeismonitorGraphView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_SENSOR_PROPERTIES, OnGraphViewSensorProperties)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_SENSOR_SETAUXILIARY, OnGraphViewSensorSetAuxiliary)
	ON_COMMAND(IDR_POPUP_GRAPHVIEW_SENSOR_MUTE, OnGraphViewSensorSetMute)
END_MESSAGE_MAP()


// CSeismonitorGraphView 绘图

void CSeismonitorGraphView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CDocument* pDoc = GetDocument ();
	ASSERT_VALID (pDoc);
	pDoc->SetTitle (_T("Seismonitor Graph View"));
	// 绘图前设置绘图参数
	set_scroll_size();
}
// 画测线标尺
void CSeismonitorGraphView::draw_mark_line_and_point(CDC* pDC, int iGridX, int iGridY)
{
	// 画测线标尺
	draw_mark_line(pDC, iGridY);
	// 画测点标尺
	draw_mark_point(pDC, iGridX);
}
// 画测线标尺和测点标尺
void CSeismonitorGraphView::draw_mark_line(CDC* pDC, int iGridY)
{
	CPen oPen;
	CPen* pOldPen;

	oPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&oPen);

	int iLineNb = 0;
	int loop_size = ((CLineApp*)AfxGetApp())->get_line_number() * 2 - 1;
	CString strLineNb;
	CRect oRect;

	for(unsigned int i =0; i < loop_size; i++)
	{
		iLineNb = i;
		oRect.left = -50;
		oRect.right = 0;
		oRect.top = i * iGridY- unit_hight_min_ / 2;
		oRect.bottom = oRect.top + unit_hight_max_;
		if(iLineNb == 0){
			strLineNb.Format("%d", iLineNb + 1);
			center_grid_.cy= oRect.top; //+ (oRect.bottom - oRect.top)/2 ;
		}
		else{	strLineNb.Format("%d", iLineNb + 1);}

		pDC->DrawText(strLineNb, &oRect, DT_LEFT | DT_VCENTER);
	}
	pDC->SelectObject(pOldPen);
}
// 画测点标尺
void CSeismonitorGraphView::draw_mark_point(CDC* pDC, int iGridX)
{
	int iStep = 1;
	if(iGridX <= 2 * grid_x_min_)	iStep = 5;

	CPen oPen;
	CPen* pOldPen;
	oPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&oPen);

	int iPointNb;
	CString strPointNb;
	CRect oRect;
	int col = ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_section_number();
	int i = 0;

	for(i = 0, iPointNb = 1-col; iPointNb < 0; ++iPointNb, ++i){
		if(((iPointNb%iStep) == 0) || (iPointNb == (1 - col))){
			oRect.left = i * iGridX -  grid_x_min_/ 2;
			oRect.right = oRect.left + grid_x_max_ ;
			oRect.top = -50;
			oRect.bottom = 0;
			strPointNb.Format("%d", iPointNb);
			pDC->DrawText(strPointNb, &oRect, DT_LEFT);
		}
	}

	for(iPointNb = 0; iPointNb < col; ++i, ++iPointNb){
		if(((iPointNb % iStep) == 0) || (iPointNb == 0) || (iPointNb == col - 1)){
			oRect.left = i * iGridX -  grid_x_min_/ 2;
			oRect.right = oRect.left + grid_x_max_ ;
			oRect.top = -50;
			oRect.bottom = 0;
			strPointNb.Format("%d", iPointNb);
			pDC->DrawText(strPointNb, &oRect, DT_LEFT);
			if(iPointNb == 0){	center_grid_.cx = oRect.left; }
		}
	}
	pDC->SelectObject(pOldPen);
}
//加入设备到视图中
void CSeismonitorGraphView::draw_devices(CDC* pDC, int iGridX, int iGridY, int test_type, float sensmonitor_limit)
{
	CBitmap* cbitmap_ptr = NULL;
	BITMAP bitmap;
	matrix_data::data_base_helper* helper = ((CLineApp*)AfxGetApp())->get_data_base_helper();
	matrix_data::data_base_helper::device_info di;
	matrix_data::data_base_helper::device_info_map::iterator itr;
	matrix_data::data_base_helper::device_info_map& devices_map = helper->get_devices_info();
	matrix_data::data_base_helper::point_nb_to_sn_map& pn_sn_map = helper->get_point_nb_to_sn_map();
	matrix_data::data_base_helper::point_nb_to_sn_map::const_iterator pns_itr;
	int cx = 0x0,
		cy = 0x0;
	int line_pre= -1;
	int line_num = 0x0;
	int point_nb = 0x0;
	byte check_value = 0x0;
	int section_num = helper->get_section_number() - 1;
	cbitmap_ptr = get_bitmap_ptr(iGridX, 3, 1);
	cbitmap_ptr->GetBitmap(&bitmap);

	for(pns_itr = pn_sn_map.begin(); pns_itr != pn_sn_map.end(); ++pns_itr){

		if(pns_itr->first > 0){
			line_num = pns_itr->first % 1000;
			point_nb = pns_itr->first / 1000;
		}
		else{
			point_nb = pns_itr->first /1000 - 1;
			line_num = pns_itr->first - point_nb * 1000 ;
		}

		cy = center_grid_.cy + (line_num - 1) * (grid_y_max_ - grid_y_min_) ;
		cx = center_grid_.cx + point_nb * bitmap.bmWidth;
		if(pns_itr->second != 0){
			//找到元素
			di.clear();
			di.sn_ = pns_itr->second;
			if(helper->find_device(di)){

				check_value = helper->get_test_result_by_type(di, test_type, sensmonitor_limit);
				switch(check_value){
				case 0:// 0：未测试
					cbitmap_ptr = get_bitmap_ptr(iGridX, 3, 1);
					break;
				case 1:// 1：测试合格
					cbitmap_ptr = get_bitmap_ptr(iGridX, 3, 1);
					break;
				case 2:// 2：测试不合格
					cbitmap_ptr = get_bitmap_ptr(iGridX, 3, 1);
					break;
				case 3:// 3：无信号
				case 4:// 4：未判读
					cbitmap_ptr = get_bitmap_ptr(iGridX, 3, 1);
					break;
				}

				if((di.sn_ & 0x3) == 0x3){	draw_device_unit(pDC, cx, cy, test_type, cbitmap_ptr);}
				else{									goto DRAW_CROSS;	}
			}
		}
		else{//画十字
DRAW_CROSS:
			CPen oPenUnit;
			CPen* pOldPen;
			CBrush oBrushUnit;
			CBrush* pOldPenBrush;

			CRect oRect;
			oRect.left = cx - bitmap.bmWidth /2;
			oRect.top = cy - bitmap.bmHeight / 2;
			oRect.right = oRect.left + bitmap.bmWidth;
			oRect.bottom = oRect.top + bitmap.bmHeight;

			oPenUnit.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));	// 黄色
			oBrushUnit.CreateSolidBrush(RGB(255, 255, 0));	// 黄色
			pOldPen = pDC->SelectObject(&oPenUnit);
			pOldPenBrush = pDC->SelectObject(&oBrushUnit);

			// 画十字
			pDC->MoveTo(CPoint(oRect.left, cy));
			pDC->LineTo(CPoint(oRect.right, cy));
			pDC->MoveTo(CPoint(cx, oRect.top));
			pDC->LineTo(CPoint(cx, oRect.bottom));
		}
	}
}

//画出每一个设备
void CSeismonitorGraphView::draw_device_unit(CDC* pDC, int iGridX, int iGridY, int test_type, CBitmap* bitmap_ptr)
{
	CDC dc;
	BITMAP bm;

	dc.CreateCompatibleDC(pDC);
	bitmap_ptr->GetBitmap(&bm);
	CBitmap* pOldBitmap = dc.SelectObject(bitmap_ptr);
	int nX = iGridX - bm.bmWidth / 2;
	int nY = iGridY - bm.bmHeight / 2;
	pDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dc, 	0, 0, SRCCOPY);
	dc.SelectObject(pOldBitmap);
}

void CSeismonitorGraphView::OnDraw(CDC* pDC)
{
	// 画网格图
	//	m_pSiteData->m_oGraphSensorList.DrawGrid(pDC, m_iPaperX, m_iPaperY, m_iGridX, m_iGridY);
	// 画鼠标中间键选中区域
	pDC->SetBkColor(RGB(99,99,99));

	CBrush oBrush;
	CBrush OldPenBrush;
	CRect m_oRect;
	GetClientRect(m_oRect);
	OldPenBrush.CreateSolidBrush(RGB(99,99,99));
	pDC->DPtoLP(m_oRect);
	pDC->FillRect(m_oRect, &OldPenBrush);
	DrawSelectArea(pDC);

	// 画测线标尺和测点标尺
	draw_mark_line_and_point(pDC, grid_x_, grid_y_);
	// 画图形单元
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	draw_devices(pDC, grid_x_, grid_y_, pParentFrame->m_uiTestType, pParentFrame->m_fSeismonitorLimit);
}

// CSeismonitorGraphView 诊断
#ifdef _DEBUG
void CSeismonitorGraphView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CSeismonitorGraphView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CSeismonitorGraphView 打印

void CSeismonitorGraphView::OnFilePrintPreview()
{
	BCGPPrintPreview(this);
}

void CSeismonitorGraphView::OnFilePrint()
{
	CScrollView::OnFilePrint();
}

BOOL CSeismonitorGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSeismonitorGraphView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSeismonitorGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

// CSeismonitorGraphView 消息处理程序

int CSeismonitorGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 加载现场数据
	OnLoadSiteData();
	// 生成测试值漂浮窗口
	m_wndStaticFloat.Create("", WS_CHILD, CRect(0, 0, 100, 20), this);
	//加入显示元素 
	load_bitmaps();
	return 0;
}

void CSeismonitorGraphView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetWindowOrg(-paper_head_left_, -paper_head_top_);	
}

void CSeismonitorGraphView::show_context_menu(CPoint& point, CPoint& client_point)
{
	int line_num = -1,
		point_nb = -1, 
		sn = 0x0;

	// 判断点是否在绘图区域内
	if(get_line_and_point_nb_according_to_point(client_point, line_num, point_nb)){	
		sn =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->find_point_nb_and_return_sn(point_nb * 1000 + line_num+1);
		if(sn != 0){
			device_info_.clear();
			device_info_.sn_ = sn;
			((CLineApp*)AfxGetApp())->get_data_base_helper()->find_device(device_info_);
			CMenu menu;
			menu.LoadMenu (IDR_POPUP_GRAPHVIEW_SENSOR); 
			CMenu* pSumMenu = menu.GetSubMenu(0);
			if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd))){
				CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
				if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))	return;
				((CBCGPMDIFrameWnd*)AfxGetMainWnd())->OnShowPopupMenu (pPopupMenu);
				UpdateDialogControls(this, FALSE);
			}
		}	
	}
}
void CSeismonitorGraphView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CRect oRect;
	GetWindowRect(oRect);
	CPoint ptClient;
	ptClient = point;
	ptClient.x = ptClient.x - oRect.left;
	ptClient.y = ptClient.y - oRect.top;	
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&ptClient);
	show_context_menu(point, ptClient);
}

/**
* 显示设备信息在属性对话框
*/
void CSeismonitorGraphView::show_device_info(CPoint& point)
{
	matrix_data::data_base_helper::device_info di;
	int line_num = -1,
		point_nb = -1;

	// 判断点是否在绘图区域内
	if(get_line_and_point_nb_according_to_point(point, line_num, point_nb)){	
		di.sn_ =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->find_point_nb_and_return_sn(point_nb * 1000 + line_num+1);
		if(di.sn_ != 0)	
			if(((CLineApp*)AfxGetApp())->get_data_base_helper()->find_device(di))	OnShowProperty(di);
	}
}

void CSeismonitorGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&point);
	show_device_info( point);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CSeismonitorGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);

	if(true == m_bDrawSelectArea)
	{
		m_oSelectAreaPoint.left = m_oSelectAreaPoint.left - 2;
		m_oSelectAreaPoint.top = m_oSelectAreaPoint.top - 2;
		m_oSelectAreaPoint.right = m_oSelectAreaPoint.right + 2;
		m_oSelectAreaPoint.bottom = m_oSelectAreaPoint.bottom + 2;
		oDC.LPtoDP(&m_oSelectAreaPoint);
		InvalidateRect(&m_oSelectAreaPoint, TRUE);
	}

	oDC.DPtoLP(&point);	
	m_oPointBase = point;	// 鼠标键按下时鼠标点
	m_oPointLast = m_oPointBase;	// 鼠标键释放时鼠标点
	m_bMouseDownLeft = true;	// 鼠标左键是否按下
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置

	CScrollView::OnLButtonDown(nFlags, point);
}

void CSeismonitorGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(true == m_bMouseDownLeft)	// 鼠标左键是否按下
	{
		CClientDC oDC(this);
		OnPrepareDC(&oDC, NULL);
		oDC.DPtoLP(&point);
		CRect oRect;
		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = m_oPointLast.x;
		oRect.bottom = m_oPointLast.y;
		oRect.NormalizeRect();	
		oDC.DrawFocusRect(&oRect);
		m_oPointLast = point;	// 鼠标键释放时鼠标点
		m_bMouseDownLeft = false;	// 鼠标左键是否按下

		// 得到选中的区域
		if(true == GetSelectAreaPoint())
		{
			// 局部放大
			OnZoomInDrag();
			Invalidate(TRUE);
		}
		else
		{
			oRect.left = oRect.left - 2;
			oRect.top = oRect.top - 2;
			oRect.right = oRect.right + 2;
			oRect.bottom = oRect.bottom + 2;
			oDC.LPtoDP(&oRect);
			InvalidateRect(&oRect, TRUE);
		}
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CSeismonitorGraphView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);

	if(true == m_bDrawSelectArea)	// 是否画鼠标中间键选中区域位置
	{
		m_oSelectAreaPoint.left = m_oSelectAreaPoint.left - 2;
		m_oSelectAreaPoint.top = m_oSelectAreaPoint.top - 2;
		m_oSelectAreaPoint.right = m_oSelectAreaPoint.right + 2;
		m_oSelectAreaPoint.bottom = m_oSelectAreaPoint.bottom + 2;
		oDC.LPtoDP(&m_oSelectAreaPoint);
		InvalidateRect(&m_oSelectAreaPoint, TRUE);
		m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	}

	oDC.DPtoLP(&point);	
	m_oPointBase = point;	// 鼠标键按下时鼠标点
	m_oPointLast = m_oPointBase;	// 鼠标键释放时鼠标点
	m_bMouseDownMiddle = true;	// 鼠标中间键是否按下

	CScrollView::OnMButtonDown(nFlags, point);
}

void CSeismonitorGraphView::OnMButtonUp(UINT nFlags, CPoint point)
{
	if(true == m_bMouseDownMiddle)	// 鼠标中间键是否按下
	{
		CClientDC oDC(this);
		OnPrepareDC(&oDC, NULL);
		oDC.DPtoLP(&point);

		CRect oRect;
		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = m_oPointLast.x;
		oRect.bottom = m_oPointLast.y;
		oRect.NormalizeRect();	
		oDC.DrawFocusRect(&oRect);
		m_oPointLast = point;	// 鼠标键释放时鼠标点
		m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

		// 得到选中的区域
		if(true == GetSelectAreaPoint())
		{
			// 得到选中区域的行坐标和列坐标区域
			GetSelectAreaIndex();
			// 把选中区域转换为绝对排列字符串
			ConvertSelectAreaToAbsoluteSpreadString();
			m_bDrawSelectArea = true;	// 是否画鼠标中间键选中区域位置
			Invalidate(TRUE);
		}
		else
		{
	//		m_strAbsoluteSpread = "all";
			oRect.left = oRect.left - 2;
			oRect.top = oRect.top - 2;
			oRect.right = oRect.right + 2;
			oRect.bottom = oRect.bottom + 2;
			oDC.LPtoDP(&oRect);
			InvalidateRect(&oRect, TRUE);
		}
	}

	CScrollView::OnMButtonUp(nFlags, point);
}

void CSeismonitorGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pointClient = point;
	CClientDC oDC(this);
	OnPrepareDC(&oDC, NULL);
	oDC.DPtoLP(&point);
	if((true == m_bMouseDownLeft) || (true == m_bMouseDownMiddle))	// 鼠标左键是否按下或鼠标中间键是否按下
	{
		CRect oRect;
		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = m_oPointLast.x;
		oRect.bottom = m_oPointLast.y;
		oRect.NormalizeRect();	
		oDC.DrawFocusRect(&oRect);

		oRect.left = m_oPointBase.x;
		oRect.top = m_oPointBase.y;
		oRect.right = point.x;
		oRect.bottom = point.y;
		oRect.NormalizeRect();
		oDC.DrawFocusRect(&oRect);

		m_oPointLast = point;	// 鼠标键释放时鼠标点
	}
	else
	{
		matrix_data::data_base_helper::device_info di;
		// 判断点是否在绘图区域内
		if(get_device_info_from_point(point, di)){
			// 显示单元的测线号和测点号
			OnShowUnitPosition(di.line_index_, di.m_uiPointNb);
			// 在浮动窗口中显示测试值
			OnShowTestValueInFloatWindow(di, pointClient);
		}
		else{
			m_wndStaticFloat.ShowWindow(SW_HIDE);
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

/**
* 函数说明
* @param void
* @return void
*/
void CSeismonitorGraphView::OnGraphViewSensorProperties()
{
	OnShowProperty(device_info_);
}

/**
* 函数说明
* @param void
* @return void
*/
void CSeismonitorGraphView::OnGraphViewSensorSetAuxiliary()
{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应仪器图形视图弹出菜单设置辅助道
		pMainFrame->OnGraphViewInstrumentSetAuxiliary(3, device_info_.sn_);
}

/**
* 函数说明
* @param void
* @return void
*/
void CSeismonitorGraphView::OnGraphViewSensorSetMute()
{
		CMainFrame* pMainFrame = NULL;
		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		// 响应检波器图形视图弹出菜单Mute设置
		pMainFrame->OnGraphViewSensorSetMute(device_info_.line_index_, device_info_.m_uiPointNb);
}

// 现场数据对象初始化
void CSeismonitorGraphView::OnSiteDataInit()
{
	zoom_in_rate_ = 1.0;// 放大倍数
	set_paper_size();
	grid_x_ = 8; //初始化时,图标为8*8 为5个设备
	grid_y_ = 32;//初始化时,,图标为8*8 为4个设备高度
	grid_x_prev_ = grid_x_;
	grid_y_prev_ = grid_y_;

	m_oPointScrollPos.SetPoint(0, 0);	// 滚动条位置
	m_oPointBase.SetPoint(0, 0);	// 鼠标键按下时鼠标点
	m_oPointLast.SetPoint(0, 0);	// 鼠标键释放时鼠标点
	m_bMouseDownLeft = false;	// 鼠标左键是否按下
	m_bMouseDownMiddle = false;	// 鼠标中间键是否按下

	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	m_oSelectAreaPoint.SetRectEmpty();	// 鼠标选中区域位置，单位像素
	m_oSelectAreaIndex.SetRectEmpty();	// 鼠标中间键选中区域位置，单位单元格

	m_iSelectUnitColumnIndex = -1;	// 选中单元列索引
	m_iSelectUnitRowIndex = -1;	// 选中单元行索引
}

/**
* 响应设置测线数量
* @param void
* @return void
*/
void CSeismonitorGraphView::OnSetLineCount()
{
	matrix_data::data_base_helper* helper = ((CLineApp*)AfxGetApp())->get_data_base_helper();
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	pParentFrame->OnSetLineCount(helper->get_line_number() * 2 -1);
}

// 加载现场数据
void CSeismonitorGraphView::OnLoadSiteData()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 响应设置测线数量
	OnSetLineCount();
}

// 重新加载现场数据
void CSeismonitorGraphView::OnReloadSiteDataCMD()
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 响应设置测线数量
	OnSetLineCount();
	// 绘图前设置绘图参数
	SetupPageSize();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);
	Invalidate(TRUE);
}

// 绘图前设置绘图参数
void CSeismonitorGraphView::SetupPageSize()
{
	SetScrollSizes(MM_TEXT, paper_size_);
}

void CSeismonitorGraphView::zoom_graph()
{
	grid_y_max_ *= zoom_in_rate_;
	grid_x_max_ *= zoom_in_rate_;

	grid_x_ = (int)((float)grid_x_ * zoom_in_rate_);	// 单元格X方向尺寸
	grid_y_ = (int)((float)grid_y_ * zoom_in_rate_);	// 单元格Y方向尺寸
	if(grid_x_ > grid_x_max_)
	{
		grid_x_ = grid_x_max_;
	}
	if(grid_y_ > grid_y_max_)
	{
		grid_y_ = grid_y_max_;
	}
	if(grid_x_ < grid_x_min_)
	{
		grid_x_ = grid_x_min_;
	}
	if(grid_y_ < grid_y_min_)
	{
		grid_y_ = grid_y_min_;
	}
	set_paper_size();
	grid_y_prev_ = grid_y_;	// 单元格X方向尺寸，上一次
	grid_x_prev_ = grid_x_;	// 单元格Y方向尺寸，上一次
}

// 放大
void CSeismonitorGraphView::OnZoomIn() 
{
	float fGridXPre = (float)grid_x_prev_;
	float fGridYPre = (float)grid_y_prev_;

	zoom_in_rate_ = 2.0;	// 放大倍数
	// 缩放图形
	zoom_graph();
	// 得到滚动条位置
	m_oPointScrollPos= GetScrollPosition();
	float fZoomRateX = grid_x_ / fGridXPre;
	float fZoomRateY = grid_y_ / fGridYPre;
	m_oPointScrollPos.x = (int)((float)(m_oPointScrollPos.x - paper_head_left_) * fZoomRateX) + paper_head_left_;
	m_oPointScrollPos.y = (int)((float)(m_oPointScrollPos.y - paper_head_top_) * fZoomRateY) + paper_head_top_;
	// 绘图前设置绘图参数
	set_scroll_size();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);	
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	Invalidate(TRUE);
}

void CSeismonitorGraphView::zoom_in_drag( const CRect& client_rect, const CRect& select_area, CPoint& point_scroll_pos)
{
	float fWidthDrag= (float)select_area.Width();

	float fGridXPre = (float)grid_x_prev_;
	float fGridYPre = (float)grid_y_prev_;

	float fWidthClient = (float)client_rect.Width();
	zoom_in_rate_ = fWidthClient / fWidthDrag;
	// 缩放图形
	zoom_graph();

	// 得到滚动条位置
	point_scroll_pos.x = select_area.left;
	point_scroll_pos.y = select_area.top;

	float fZoomRateX = grid_x_ / fGridXPre;
	float fZoomRateY = grid_y_ / fGridYPre;
	point_scroll_pos.x = (int)((float)point_scroll_pos.x * fZoomRateX) + paper_head_left_;
	point_scroll_pos.y = (int)((float)point_scroll_pos.y * fZoomRateY) + paper_head_top_;
	if((paper_size_.cy + paper_head_top_ * 2) <= client_rect.Height())	{
		point_scroll_pos.y = 0;
	}
	else	{
		if(point_scroll_pos.y > ((paper_size_.cy + paper_head_top_ * 2) - client_rect.Height()))
			point_scroll_pos.y = (paper_size_.cy + paper_head_top_ * 2) - client_rect.Height();
	}
}

bool CSeismonitorGraphView::get_device_info_from_point(CPoint& point, matrix_data::data_base_helper::device_info& di)
{
	bool is_ok = false;
	int line_num = -1,
		point_nb = -1;

	if(get_line_and_point_nb_according_to_point(point, line_num, point_nb)){
		di.sn_ =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->find_point_nb_and_return_sn(point_nb * 1000 + line_num+1);
		if(di.sn_ != 0){	
			((CLineApp*)AfxGetApp())->get_data_base_helper()->find_device(di);
			is_ok = true;
		}
	}
	return is_ok;
}

/**
* 左键拖放出选择框，框内区域局部放大
* @param void
* @return void
*/
void CSeismonitorGraphView::OnZoomInDrag() 
{
	CRect oRect;
	GetClientRect(&oRect);
	zoom_in_drag(oRect, m_oSelectAreaPoint, m_oPointScrollPos);
	// 绘图前设置绘图参数
	SetupPageSize();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);
	Invalidate(TRUE);
}

/**
* 导航局部放大
* @param float fZoomRate 放大倍数
* @param float fRateX 基准点X
* @param float fRateY 基准点Y
* @return void
*/
void CSeismonitorGraphView::OnZoomInNavigater(float fZoomRate, float fRateX, float fRateY) 
{
	// 现场数据对象初始化
	OnSiteDataInit();
	// 绘图前设置绘图参数
	SetupPageSize();
	// 设置滚动条新位置
	ScrollToPosition(m_oPointScrollPos);

	// 判断：放大系数大于1.0
	if((fZoomRate > 1.01) || (fRateY > 0.01))
	{
		CRect oRect;
		GetClientRect(&oRect);
		zoom_in_rate_ = fZoomRate;
		// 缩放图形
		zoom_graph();

		m_oPointScrollPos.x = (int)((float)paper_size_.cx * fRateX) + paper_head_left_;
		m_oPointScrollPos.y = (int)((float)paper_size_.cy * fRateY) + paper_head_top_;
		if((paper_size_.cy + paper_head_top_ * 2) <= oRect.Height())
		{
			m_oPointScrollPos.y = 0;
		}
		else
		{
			if(m_oPointScrollPos.y > ((paper_size_.cy + paper_head_top_ * 2) - oRect.Height()))
			{
				m_oPointScrollPos.y = (paper_size_.cy + paper_head_top_ * 2) - oRect.Height();
			}
		}
		// 绘图前设置绘图参数
		SetupPageSize();
		// 设置滚动条新位置
		ScrollToPosition(m_oPointScrollPos);
	}
	Invalidate(TRUE);	
}

// 缩小
void CSeismonitorGraphView::OnZoomOut()
{
	zoom_in_rate_ = 0.5;	// 放大倍数
	// 缩放图形
	zoom_graph();
	// 绘图前设置绘图参数
	SetupPageSize();	
	m_bDrawSelectArea = false;	// 是否画鼠标中间键选中区域位置
	Invalidate(TRUE);
}

/**
* 得到选中的区域
* @param void
* @return void
*/
bool CSeismonitorGraphView::GetSelectAreaPoint()
{
	m_oSelectAreaPoint.left = m_oPointBase.x;
	m_oSelectAreaPoint.top = m_oPointBase.y;
	m_oSelectAreaPoint.right = m_oPointLast.x;
	m_oSelectAreaPoint.bottom = m_oPointLast.y;
	m_oSelectAreaPoint.NormalizeRect();

	float fWidthDrag= (float)m_oSelectAreaPoint.Width();

	if(fWidthDrag < 50.0)
	{
		return false;
	}

	if(m_oSelectAreaPoint.left < 0)
	{
		m_oSelectAreaPoint.left = 0;
	}
	if(m_oSelectAreaPoint.top < 0)
	{
		m_oSelectAreaPoint.top = 0;
	}
	if(m_oSelectAreaPoint.right > paper_size_.cx)
	{
		m_oSelectAreaPoint.right = paper_size_.cx;
	}
	if(m_oSelectAreaPoint.bottom > paper_size_.cy)
	{
		m_oSelectAreaPoint.bottom = paper_size_.cy;
	}
	return true;
}

/**
* 得到选中区域的行坐标和列坐标区域
* @param void
* @return void
*/
void CSeismonitorGraphView::GetSelectAreaIndex()
{
	int iAreaColumnIndex;	// 鼠标点所在区域的列索引
	int iAreaRowIndex;	// 鼠标点所在区域的行索引

	iAreaColumnIndex = (int)((float)m_oSelectAreaPoint.left / (float)grid_x_);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)m_oSelectAreaPoint.top / (float)grid_y_);	// 鼠标点所在区域的行索引
	m_oSelectAreaIndex.left = iAreaColumnIndex;	// 鼠标中间键选中区域位置
	m_oSelectAreaIndex.top = iAreaRowIndex;	// 鼠标中间键选中区域位置
	if(m_oSelectAreaPoint.left > 0)
	{
		m_oSelectAreaIndex.left = iAreaColumnIndex + 1;	// 鼠标中间键选中区域位置
	}	
	if(m_oSelectAreaPoint.top > 0)
	{
		m_oSelectAreaIndex.top = iAreaRowIndex + 1;	// 鼠标中间键选中区域位置
	}	

	iAreaColumnIndex = (int)((float)m_oSelectAreaPoint.right / (float)grid_x_);	// 鼠标点所在区域的列索引
	iAreaRowIndex = (int)((float)m_oSelectAreaPoint.bottom / (float)grid_y_);	// 鼠标点所在区域的行索引
	m_oSelectAreaIndex.right = iAreaColumnIndex;	// 鼠标中间键选中区域位置
	m_oSelectAreaIndex.bottom = iAreaRowIndex;	// 鼠标中间键选中区域位置
}

/**
* 把选中区域转换为绝对排列字符串
* @param void
* @return void
*/
void CSeismonitorGraphView::ConvertSelectAreaToAbsoluteSpreadString()
{
	// 把选中区域转换为绝对排列字符串
	//m_strAbsoluteSpread = m_pSiteData->m_oGraphSensorList.ConvertSelectAreaToAbsoluteSpreadString(&m_oSelectAreaIndex);
}

/**
* 显示单元的测线号和测点号
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @return void
*/
void CSeismonitorGraphView::OnShowUnitPosition(unsigned int uiNbLine, unsigned int uiNbPoint)
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	pParentFrame->OnShowUnitPosition(uiNbLine, uiNbPoint);
}

/**
* 在浮动窗口中显示测试值
* @param CChannelSetupData* pChannelSetupData 检波器
* @param CPoint pointClient 鼠标点
* @return void
*/
void CSeismonitorGraphView::OnShowTestValueInFloatWindow(matrix_data::data_base_helper::const_device_info& di, CPoint pointClient)
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	CString strTestUnit = "";
	CString strTestValue = "";
	CString strValue = "";

	strValue.Format("%d  %s", di.sn_, matrix_data::data_base_helper::get_device_test_result_by_test_type(di, pParentFrame->m_uiTestType));
	if("" != strValue)
	{
		m_wndStaticFloat.SetWindowText(strValue);
		m_wndStaticFloat.MoveWindow(pointClient.x + 20, pointClient.y, 200, 20);
		m_wndStaticFloat.ShowWindow(SW_SHOW);
	}
}

/**
* 响应显示属性值
* @param matrix_data::data_base_helper::const_device_info& di 检波器信息
* @return void
*/
void CSeismonitorGraphView::OnShowProperty(matrix_data::data_base_helper::const_device_info& di)
{
	CSeismonitorGraphChildFrame* pParentFrame = (CSeismonitorGraphChildFrame*)GetParentFrame();
	pParentFrame->OnShowProperty(di);
}

// 画鼠标中间键选中区域
void CSeismonitorGraphView::DrawSelectArea(CDC* pDC)
{
	if(true == m_bDrawSelectArea)	// 是否画鼠标中间键选中区域位置
	{
		CPen oPen;
		CPen* pOldPen;
		CBrush oBrush;
		CBrush* pOldPenBrush;

		oPen.CreatePen(PS_SOLID, 0, RGB(255, 255, 0));
		oBrush.CreateSolidBrush(RGB(255, 0, 0));
		pOldPen = pDC->SelectObject(&oPen);
		pOldPenBrush = pDC->SelectObject(&oBrush);

		pDC->FillRect(&m_oSelectAreaPoint, &oBrush);
		pDC->Rectangle(&m_oSelectAreaPoint);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldPenBrush);
	}
}

// 为新的测试数据刷新显示
void CSeismonitorGraphView::OnRefreshForNewTestValue()
{
}
/**
根据当前逻辑坐标点得到行号和点号
*/
bool CSeismonitorGraphView::get_line_and_point_nb_according_to_point(CPoint& point, int& line_idx, int& point_nb)
{
	bool is_ok = false;
	// 判断点是否在绘图区域内
	if(judge_if_point_in_paper(point)){	
		int i =0,
			tmp_min = 0,
			tmp_max = 0,
			loop_size = ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_line_number() * 2 - 1;
		BITMAP bm;
		line_idx = -1;
		point_nb = -1;

		fdu_bitmap_[current_bitmap_array_index_].GetBitmap(&bm);
		for(i =0; i < loop_size; ++i)	{
			tmp_min = center_grid_.cy + i * (grid_y_max_ - grid_y_min_) - bm.bmHeight / 2;
			tmp_max = tmp_min + bm.bmHeight;
			if(point.y >= tmp_min && point.y <= tmp_max){
				line_idx = i;
				break;
			}
		}
		if(line_idx == -1)  goto GOTO;

		i = 0;
		loop_size =  ((CLineApp*)AfxGetApp())->get_data_base_helper()->get_section_number() - 1;
		for (i -= loop_size; i < loop_size; ++i){
			tmp_min = center_grid_.cx + i * bm.bmWidth - bm.bmWidth / 2;
			tmp_max = tmp_min + bm.bmWidth;
			if(point.x >= tmp_min && point.x <= tmp_max){
				point_nb = i;
				is_ok = true;
				break;
			}
		}
		if(!is_ok)	goto GOTO;
		is_ok = true;
	}
GOTO:
	return is_ok;
}
void CSeismonitorGraphView::load_bitmaps()
{
	for(int i = 0; i < bitmap_array_size_; ++i){
		aux_bitmap_[i].LoadBitmap(IDB_AUX_BITMAP8 + i);
		aul_bitmap_[i].LoadBitmap(IDB_AUL_BITMAP8 + i);
		fdu_bitmap_[i].LoadBitmap(IDB_FDU_BITMAP_8 + i);
	}
}

CBitmap* CSeismonitorGraphView::get_bitmap_ptr(int grid_x/*x方向大小*/, 
	int type/*图表类型 1.aux 2.aul 3.fdu*/, 
	int status/*当前设备状态 1: OK 2: error*/)
{
	current_bitmap_array_index_ = grid_x / 8;
	if(current_bitmap_array_index_ > bitmap_array_size_) current_bitmap_array_index_ = bitmap_array_size_;
	else if(current_bitmap_array_index_ < 0) current_bitmap_array_index_ = 0;

	CBitmap* handle = NULL;
	--current_bitmap_array_index_; 
	switch(type){
	case 1:
		handle = aux_bitmap_ + current_bitmap_array_index_;
		break;
	case 2:
		handle = aul_bitmap_ + current_bitmap_array_index_;
		break;
	case 3:
		handle = fdu_bitmap_ + current_bitmap_array_index_;
		break;
	}
	return handle;
}
bool CSeismonitorGraphView::judge_if_point_in_paper(CPoint& point)
{
	// 得到图形单元尺寸
	if(((point.x >= (-paper_head_left_ / 2)) && (point.x <= paper_size_.cx )) 	&& 
		((point.y >= (-paper_head_top_ / 2)) && (point.y <= paper_size_.cy)))
	{
		return true;
	}
	return false;
}

