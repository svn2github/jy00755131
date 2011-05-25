// 线程接收Dlg.h : 头文件
//
#include "MyThread.h"
#include <afxmt.h>
#pragma once


// C线程接收Dlg 对话框
class C线程接收Dlg : public CDialog
{
// 构造
public:
	C线程接收Dlg(CWnd* pParent = NULL);	// 标准构造函数

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
public:
	CMyThread m_Thread;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 定义临界区变量
	CCriticalSection m_Sec;
};
