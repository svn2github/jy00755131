
// NetWinPcapDlg.h : 头文件
//

#pragma once


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
};
