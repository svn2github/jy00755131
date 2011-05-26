// PortMonitoringDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Resource.h"
#include "ThreadManage.h"
#include "Parameter.h"
#include <afxmt.h>

// CPortMonitoringDlg 对话框
class CPortMonitoringDlg : public CDialog
{
// 构造
public:
	CPortMonitoringDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPortMonitoringDlg(void);
// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:

	// 保存接收数据
	void OnSaveReceiveData(unsigned char* buf, int iRecLength);
	// 保存发送数据
	void OnSaveSendData(unsigned char* buf, int iSendLength);
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
	CThreadManage m_oThreadManage;

	// 显示收到和发送帧
	CString m_csEditShow;
	// 保存通讯文件
	CString m_csSaveFile;
	// 保存接收的数据到文件
	CString m_csSaveReceiveFile;
	// 保存发送的数据到文件
	CString m_csSaveSendFile;
	// 定义临界区全局变量
	CCriticalSection m_Sec;
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
	afx_msg void OnBnClickedButtonOnReset();
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
	afx_msg void OnClose();
};
