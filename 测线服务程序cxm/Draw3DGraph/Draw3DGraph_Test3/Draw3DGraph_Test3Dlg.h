// Draw3DGraph_Test3Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "nigraph3d.h"

// 采样数据最大值
#define SampleAmpMax		100.0
// 采样数据最大值
#define SampleAmpMin		-100.0
// 采样时间
#define SampleTime			100
// 定时器序号
#define TimerID				1
// 定时器延时
#define TimerDelay			500
// CDraw3DGraph_Test3Dlg 对话框
class CDraw3DGraph_Test3Dlg : public CDialog
{
// 构造
public:
	CDraw3DGraph_Test3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAW3DGRAPH_TEST3_DIALOG };

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
	// 选择要打开的文件
	CString SelectOpenFile(void);
	// 打开文件路径
	CString m_strFilePath;
	// 选择数据文件控制变量
	CButton m_ctrlBtnSelectFile;
	afx_msg void OnBnClickedBtnSelectfile();
	// 3D绘图控件控制变量
	NI::CNiGraph3D m_ctrlGraph3D;
	// 开始按键控制变量
	CButton m_ctrlBtnStart;
	// 停止按键控制变量
	CButton m_ctrlBtnStop;
	CNiReal64Vector m_xTimeData;
	CNiReal64Vector m_yTraceData;
	CNiReal64Matrix m_zAmpData;
	// 计数
	int m_iCounter;
	CNiAxis3D m_Axis3D;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedRadioSurface();
	afx_msg void OnBnClickedRadioSurfaceline();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
