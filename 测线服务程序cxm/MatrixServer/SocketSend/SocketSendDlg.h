
// SocketSendDlg.h : 头文件
//

#pragma once


// CSocketSendDlg 对话框
class CSocketSendDlg : public CDialogEx
{
// 构造
public:
	CSocketSendDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCKETSEND_DIALOG };

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
	// 目标端口
	UINT m_uiDstPort;
	// 是否广播
	BOOL m_bBroadCast;
	// 接收缓冲
	UINT m_uiRcvBuf;
	// 发送缓冲
	UINT m_uiSndBuf;
	// Socket
	SOCKET m_SocketSnd;
	// 目标
	sockaddr_in m_addrSend;
	// 发送缓存
	char m_cFrameData[256];
	// 发送计数
	unsigned int m_uiCount;
	// 发送帧数
	UINT m_uiSndNum;
public:
	afx_msg void OnBnClickedButtonStart();
};
