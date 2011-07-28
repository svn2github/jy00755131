// 绘图程序Dlg.h : 头文件
//

#pragma once
#include "ChartViewer.h"
#include "afxwin.h"
#include "Parameter.h"
#include <vector>
using namespace std;
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
	//在窗口大小发生变化时，控件位置可以相应的进行调整
	typedef struct _dlgControlTag 
	{
		int iId;
		int iFlag;
		int iPercent;
	} DLGCTLINFO, *PDLGCTLINFO;
	enum
	{
		MOVEX = 0,
		MOVEY,
		MOVEXY,
		ELASTICX,
		ELASTICY,
		ELASTICXY
	};

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
	int m_extBgColor;						// 默认的背景颜色
	double m_minData;						// 横坐标的最小值
	double m_maxData;					// 横坐标的最大值
	double m_dateRange;					// 横坐标的数值变化范围
	double m_maxValue;					// 纵坐标的最大值
	double m_minValue;					// 纵坐标的最小值
	unsigned int m_uiIntervalNum;	// 绘图区域左侧间隔个数
	// 记录X轴坐标点信息
	vector <double>	m_DrawPoint_X;
	// 记录各条图线点的信息
	vector <vector <double>> m_DrawLine_Y;
private:
	int m_iClientWidth;				// 对话框client区域的宽度
	int m_iClientHeight;			// 对话框client区域的高度
	int m_iMinWidth;				// 对话框的最小宽度
	int m_iMinHeight;				// 对话框的最小高度
	PDLGCTLINFO m_pControlArray;	// 控件信息数组指针
	int m_iControlNumber;			// 设置控件信息的控件个数
	BOOL m_bShowSizeIcon;			// 是否显示表示可改变大小的图标
	CStatic m_wndSizeIcon;			// 放图标的静态控件
	// 保存图标的bitmap
	CBitmap m_bmpSizeIcon; 
	BITMAP m_bitmap; 
	// 打开文件的路径
	CString m_csOpenFilePath;
	// 采集站设备总数
	unsigned int m_uiInstrumentMaxNum;
	// 参与ADC数据采集的采集站设备数
	unsigned int m_uiInstrumentADCNum;
	// ADC数据开始的数据点数
	unsigned int m_uiADCStartNum;
	// ADC数据转换格式
	unsigned int m_uiADCDataCovNb;
	// 采集站采集到的ADC数据总数
	unsigned int m_uiADCDataNum;
	// 每个采集站采集到的ADC数据个数
	unsigned int m_uiADCDataFduNum;
	// 采集站ADC数据存储
	vector<double>* m_dbFduData;
	// ADC数据缓冲区指针数组
	double** m_viewPortDataSeries;
	// 记录第一行数据开辟的缓冲区
	double *m_dbDataTemp;
public:	
	CChartViewer	m_ChartViewer;	// 添加绘图控件的控制变量
	double m_currentDuration;			// 当前显示数据点的个数
	CButton m_PointerPB;				// 按键Pointer的控制变量
	CButton m_XZoomPB;				// 按键X Zoom/Y Auto的控制变量
	CScrollBar m_HScrollBar;			// 横向滚动条的控制变量
	CScrollBar m_VScrollBar;			// 纵向滚动条的控制变量
	CComboBox m_Duration;			// 绘图显示点数选项卡控制变量
	double m_minDuration;				// 最少显示点的个数
private:
	// 得到默认的背景颜色
	int getDefaultBgColor(void);
	// 载入数据
	BOOL OnOpenFile(void);
	// 载入一个图标资源到按钮
	void loadButtonIcon(int buttonId, int iconId, int width, int height);
	// 当用户选中时移动滚动条
	double moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// 绘制图形
	void drawChart(CChartViewer *viewer);
	// 重绘图片
	void updateImageMap(CChartViewer *viewer);
	// 验证用户输入的显示点数
	void validateDuration(const CString &text);
	// 设置控件信息
	BOOL SetControlProperty(PDLGCTLINFO lp, int nElements);
	// 是否在对话框右下角显示表示可改变大小的图标
	void ShowSizeIcon(BOOL bShow = TRUE);
	// 将对话框及其控件设为尺寸可变的
	void OnSiteSizeBox(void);
	// 从文件中载入数据
	BOOL LoadData(CString csOpenFilePath);
	// 解码第一行ADC数据
	void OnPhraseFirstLine(CString str);
public:
	afx_msg void OnBnClickedPointerpb();
	afx_msg void OnBnClickedZoominpb();
	afx_msg void OnBnClickedZoomoutpb();
	afx_msg void OnBnClickedXzoompb();
	afx_msg void OnBnClickedXyzoompb();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewPortChanged();
	afx_msg void OnBnClickedChartViewer();
	afx_msg void OnCbnSelchangeDuration();
	afx_msg void OnCbnKillfocusDuration();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedButtonSavechart();
	afx_msg void OnBnClickedButtonOpenadcfile();
};
