
// MatrixServerDlg.h : 头文件
//

#pragma once
#include "MatrixDllCall.h"
#include "ServerCommDll.h"

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
	/** DLL函数调用类成员*/
	CMatrixDllCall m_oMatrixDllCall;
	/** 与客户端通讯类成员*/
	CServerCommDll m_oComDll;
public:
	afx_msg void OnBnClickedBnStart();
	afx_msg void OnBnClickedBnStop();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonStartsampleAll();
	afx_msg void OnBnClickedButtonStopsampleAll();
	afx_msg void OnBnClickedBnAdcsetAll();
	afx_msg void OnBnClickedBnAdcsetByrout();
	afx_msg void OnBnClickedBnStartsampleByrout();
	afx_msg void OnBnClickedBnStopsampleByrout();
	afx_msg void OnBnClickedBtnOpenroutpower();
	afx_msg void OnBnClickedBtnGetroutinstrumentnum();
	afx_msg void OnBnClickedBtnGetsnbylocation();
};
