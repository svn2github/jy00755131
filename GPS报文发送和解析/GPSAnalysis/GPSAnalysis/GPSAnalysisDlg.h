// GPSAnalysisDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "RecThread.h"


// CGPSAnalysisDlg 对话框
class CGPSAnalysisDlg : public CDialog
{
// 构造
public:
	CGPSAnalysisDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_GPSANALYSIS_DIALOG };

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
	// 帧格式选择
	CComboBox m_ctrlcomboFrameSelect;
	// 发送GPS报文帧内容
	CEdit m_editSendFrame;
	// 发送帧数据
	byte m_pSendFrameData[256]; 
	// 开启接收线程
	CRecThread m_RecThread;
public:
	afx_msg void OnCbnSelchangeComboFrameselect();
	afx_msg void OnBnClickedButtonSendgpsframe();
	// 生成帧头
	void MakeSendFrame(char* buf, int ilength);
	afx_msg void OnClose();
};
