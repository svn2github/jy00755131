// PortMonitoringDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#define RcvPort1	0x8202	// 目标1接收端口
#define SendPort1	0x9002	// 目标1发送端口
#define RcvPort2	0x9001	// 目标2接收端口
#define SendPort2	0x8201	// 目标2发送端口
#define UDPPortNb	18		// 目标2发送的端口数
class CMySocket;
class CMySocket2;
// CPortMonitoringDlg 对话框
class CPortMonitoringDlg : public CDialog
{
// 构造
public:
	CPortMonitoringDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPortMonitoringDlg();
// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	// 保存通讯文件
	CString m_csSaveFile;
	// 保存接收数据
	void OnSaveReceiveData(unsigned char* buf, int iRecLength);
	// 保存发送数据
	void OnSaveSendData(unsigned char* buf, int iSendLength, bool berror);
	// 保存到文件中
	void OnSaveToFile();
	// 保存文件
	FILE* m_file;
	// 保存采样数据文件的路径
	char cSaveToFilePath[MAX_PATH];
	CString strFilePath;
	// 保存通讯数据开始标志位
	BOOL m_bStartSave;
	// 接收方IP地址
	CString m_strReceiveIP;
	// 发送方IP地址
	CString m_strSendIP;
	CMySocket* m_hSocket1;
	CMySocket2* m_hSocket2;



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
	afx_msg void OnBnClickedButtonSavefilepath();
	afx_msg void OnBnClickedButtonStartsave();
	afx_msg void OnBnClickedButtonStopsave();
	// 显示收到和发送帧
	CString m_cseditshow;
	afx_msg void OnBnClickedButtonClearscreen();
	// 自动保存文件大小
	int m_iSaveSize;
	// 接收方IP地址控制变量
	CIPAddressCtrl m_IPreceive;
	// 发送方IP地址控制变量
	CIPAddressCtrl m_IPsend;
	// 目标1接收端口
	CString m_csRcvPort1;
	// 目标1发送端口
	CString m_csSendPort1;
	// 目标2接收端口
	CString m_csRcvPort2;
	// 目标2发送端口
	CString m_csSendPort2;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	// 采样开始标志位
	BOOL m_bStartSample;
};
