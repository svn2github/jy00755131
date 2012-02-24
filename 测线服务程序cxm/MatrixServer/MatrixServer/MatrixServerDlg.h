
// MatrixServerDlg.h : 头文件
//

#pragma once
#include "..\\MatrixServerDll\\MatrixServerDll.h"
// CMatrixServerDlg 对话框
class CMatrixServerDlg : public CDialogEx
{
// 构造
public:
	CMatrixServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MATRIXSERVER_DIALOG };

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
	afx_msg void OnBnClickedBnStart();
	afx_msg void OnBnClickedBnStop();
	afx_msg void OnBnClickedBnClose();
	afx_msg void OnBnClickedBnRefresh();
	afx_msg void OnBnClickedBnSave();
public:
	HMODULE hMod;
	m_oEnvironmentStruct* pEnv;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonStartsample();
	afx_msg void OnBnClickedButtonStopsample();
};
