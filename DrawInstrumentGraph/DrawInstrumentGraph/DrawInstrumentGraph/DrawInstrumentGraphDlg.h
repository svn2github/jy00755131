// DrawInstrumentGraphDlg.h : 头文件
//

#pragma once
#include "InstrumentGraph.h"
// CDrawInstrumentGraphDlg 对话框
class CDrawInstrumentGraphDlg : public CDialog
{
// 构造
public:
	CDrawInstrumentGraphDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAWINSTRUMENTGRAPH_DIALOG };

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
	CInstrumentGraph m_oInstrumentGraph;
public:
	// 绘制设备连接图
	void OnDrawInstrumentGraph(void);
};
