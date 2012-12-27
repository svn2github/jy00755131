// Draw3DGraph_Test5Dlg.h : 头文件
//

#pragma once
#include "nigraph3d.h"


// CDraw3DGraph_Test5Dlg 对话框
class CDraw3DGraph_Test5Dlg : public CDialog
{
// 构造
public:
	CDraw3DGraph_Test5Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAW3DGRAPH_TEST5_DIALOG };

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
	// 绘图控件控制变量
	NI::CNiGraph3D m_ctrlGraph3D;
	NI::CNiAxis3D m_Axis3D;
};
