// MonitorDlg.h : 头文件
//

#pragma once
#include "SocketSampData.h"
#include "iplotx1.h"

// CMonitorDlg 对话框
class CMonitorDlg : public CDialog
{
// 构造
public:
	CMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:

	CSocketSampData	m_SockSample;
	DWORD			m_dwXValue;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
	
public:
	void AnalysisProtocolOfSampleSock(void);

	
	CIplotx1 m_iPlotX;
	afx_msg void OnBnClickedButton1();
};
