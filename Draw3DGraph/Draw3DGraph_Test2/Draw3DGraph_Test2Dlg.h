// Draw3DGraph_Test2Dlg.h : 头文件
//

#pragma once
#include "nigraph3d.h"
#include "afxwin.h"
// 采样数据幅值
#define SampleAmpMax		2.5
// 采样时间
#define SampleTime			100
// 通道数
#define TraceNum			10
// 线数
#define LineNum				3
// 定时器序号
#define TimerID				1
// 定时器延时
#define TimerDelay			500

// CDraw3DGraph_Test2Dlg 对话框
class CDraw3DGraph_Test2Dlg : public CDialog
{
// 构造
public:
	CDraw3DGraph_Test2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAW3DGRAPH_TEST2_DIALOG };

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
public:
	// 3D绘图控件控制变量
	NI::CNiGraph3D m_ctrlGraph3D;
	// 开始显示按钮控制变量
	CButton m_ctrlBtnStart;
	// 停止显示按钮控制变量
	CButton m_ctrlBtnStop;
	CNiReal64Vector m_xTimeData;
	CNiReal64Vector m_yTraceData;
	CNiReal64Matrix m_zAmpData;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedRadioSurface();
	afx_msg void OnBnClickedRadioSurfaceline();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 计数
	int m_iCounter;
	CNiAxis3D m_Axis3D;
};
