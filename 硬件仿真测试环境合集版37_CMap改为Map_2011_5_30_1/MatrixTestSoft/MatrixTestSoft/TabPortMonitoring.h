#pragma once
#include "afxcmn.h"
#include "PortMonitoringRec.h"
#include "PortMonitoringSend.h"
#include "SaveFile.h"
#include "PortMonitoringRecThread.h"
#include "PortMonitoringSendThread.h"
#include <afxmt.h>
// CPortMonitoring 对话框

class CTabPortMonitoring : public CDialog
{
	DECLARE_DYNAMIC(CTabPortMonitoring)

public:
	CTabPortMonitoring(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPortMonitoring();

// 对话框数据
	enum { IDD = IDD_DIALOG_PORTMONITORING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 接收方IP地址控制变量
	CIPAddressCtrl m_IPctrlRec;
	// 发送方IP地址控制变量
	CIPAddressCtrl m_IPctrlSend;
	// 定义临界区变量
	CCriticalSection m_Sec_PortMonitor;
	// 端口分发功能选择
	BOOL m_bPortDistribution;
	// 开始端口监视标志位
	BOOL m_bPortMonitoringStart;
public:
	// 类CSaveFile的变量
	CSaveFile m_oSaveFile;
	// TabControl控制变量
	CTabCtrl m_cTabCtrlItems;
	// 类CPortMonitoringRec的变量
	CPortMonitoringRec m_oPortMonitorRec;
	// 类CPortMonitoringSend的变量
	CPortMonitoringSend m_oPortMonitorSend;
 	// 端口监视发送线程
 	CPortMonitoringSendThread m_oThreadSend;
	// 端口监视接收线程
	CPortMonitoringRecThread m_oThreadRec;
	// 目标1接收端口
	unsigned int m_uiRcvPort1;
	// 目标1发送端口
	unsigned int m_uiSendPort1;
	// 目标2接收端口
	unsigned int m_uiRcvPort2;
	// 目标2发送端口
	unsigned int m_uiSendPort2;
	// 接收方IP地址
	CString m_csIPRec;
	// 发送方IP地址
	CString m_csIPSend;
	// 自动保存文件大小
	int m_iSaveSize;
	// 显示监测信息变量
	BOOL m_bShowMessage;

private:
	// 初始化选项卡控件
	void InitTabControlItems(void);
	// 防止程序在循环体中运行无法响应消息
	void ProcessMessages(void);
public:
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose();
public:
	afx_msg void OnTcnSelchangeTabPortmonitoring(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPortmonitoringOpen();
	afx_msg void OnBnClickedButtonPortmonitoringClose();
	afx_msg void OnBnClickedButtonStartsave();
	afx_msg void OnBnClickedButtonStopsave();
	afx_msg void OnBnClickedButtonSavefilepath();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckShowmessage();
};
