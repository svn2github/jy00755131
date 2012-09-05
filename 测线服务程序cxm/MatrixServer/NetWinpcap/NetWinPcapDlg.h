// NetWinPcapDlg.h : 头文件
//

#pragma once
#include "PcapRcvThread.h"
#include "PcapSndThread.h"
#include "NetPcapComm.h"

// CNetWinPcapDlg 对话框
class CNetWinPcapDlg : public CDialogEx
{
// 构造
public:
	CNetWinPcapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NETWINPCAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	/** Pcap接收线程成员变量*/
	CPcapRcvThread m_oPcapRcvThread;
	/** Pcap发送线程成员变量*/
	CPcapSndThread m_oPcapSndThread;
	/** Pcap通讯类成员变量*/
	CNetPcapComm m_oNetPcapComm;
public:
	/** 命令行参数解析*/
	void PhraseCommandLine(CString str);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnClose();
};
