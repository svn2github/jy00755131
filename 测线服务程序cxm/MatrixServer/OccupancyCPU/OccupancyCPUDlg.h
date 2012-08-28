
// OccupancyCPUDlg.h : 头文件
//
#include "Thread.h"
#pragma once


// COccupancyCPUDlg 对话框
class COccupancyCPUDlg : public CDialogEx
{
// 构造
public:
	COccupancyCPUDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OCCUPANCYCPU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	CThread* m_pThreadArray;
	// 线程个数
	unsigned int m_uiThreadNum;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
};
