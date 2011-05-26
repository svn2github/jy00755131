// 绘图程序Dlg.h : 头文件
//

#pragma once
#include "ChartViewer.h"
#include "afxwin.h"
#include "Parameter.h"
// C绘图程序Dlg 对话框
class C绘图程序Dlg : public CDialog
{

// 构造
public:
	C绘图程序Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~C绘图程序Dlg();

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 默认的背景颜色
	int m_extBgColor;

	double m_minDate;			// The earliest date (in chartTime unit)
	double m_dateRange;			// The overall duration (in seconds)
	double m_maxValue;			// The maximum overall y scale
	double m_minValue;			// The minimum overall y scale

	DoubleArray m_timeStamps;
	DoubleArray m_dataSeriesA;
	DoubleArray m_dataSeriesB;
	DoubleArray m_dataSeriesC;
public:
	// 添加绘图控件的控制变量
	CChartViewer	m_ChartViewer;
private:
	// 载入一个图标资源到按钮
	void loadButtonIcon(int buttonId, int iconId, int width, int height);
	// Moves the scroll bar when the user clicks on it
	double moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// Draw chart
	void drawChart(CChartViewer *viewer);
	// Update image map 
	void updateImageMap(CChartViewer *viewer);
public:
	// 得到默认的背景颜色
	int getDefaultBgColor(void);
	// 载入数据
	void LoadData(void);
	// 当前显示数据点的个数
	double m_currentDuration;
	// 按键Pointer的控制变量
	CButton m_PointerPB;
	// 按键X Zoom/Y Auto的控制变量
	CButton m_XZoomPB;
	afx_msg void OnBnClickedPointerpb();
	afx_msg void OnBnClickedZoominpb();
	afx_msg void OnBnClickedZoomoutpb();
	afx_msg void OnBnClickedXzoompb();
	afx_msg void OnBnClickedXyzoompb();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewPortChanged();
	afx_msg void OnBnClickedChartViewer();
	// 横向滚动条的控制变量
	CScrollBar m_HScrollBar;
	// 纵向滚动条的控制变量
	CScrollBar m_VScrollBar;

	double timeTemp[2000];
	double data0[2000];
	double data1[2000];
	double data2[2000];
	// 最少显示点的个数
	double m_minDuration;
};
