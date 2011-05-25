// 网络压力测试Dlg.h : 头文件
//

#pragma once
#include "afxsock.h"

#define SendPort	0x9001	// 发送端口
#define RcvPort		0x7200	// 接收端口
#define SndBufSize	655360	// 设置发送缓冲区大小
// C网络压力测试Dlg 对话框
class C网络压力测试Dlg : public CDialog
{
// 构造
public:
	C网络压力测试Dlg(CWnd* pParent = NULL);	// 标准构造函数

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
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	// CSocket对象
	CSocket m_hSocket;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 发送帧的时间间隔
	unsigned int m_uiTimeDelay;
	// 每次发送帧数
	unsigned int m_uiSendFrameNum;
	// 目标IP地址
	CString m_csIPAim;
	// 目标端口
	unsigned int m_uiAimPort;
	// 真实的重发时间间隔
	unsigned char m_ucSendFrame[256];
	unsigned int m_uiRealTimeDelay;
	// 真实发送帧数
	unsigned int m_uiRealSendFrameNum;
};
