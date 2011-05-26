// UartToUdpDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Parameter.h"
#include "CMSComm.h"
// CUartToUdpDlg 对话框
#define UDPPortNb	18
class CUDPSocket;
class CUartToUdpDlg : public CDialog
{
// 构造
public:
	CUartToUdpDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CUartToUdpDlg();


// 对话框数据
	enum { IDD = IDD_UARTTOUDP_DIALOG };

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
	// 控件IDC_COMBO_SERIALPORT_BAUD的控制变量
	CComboBox m_comboSerialPortBaud;
	// 控件IDC_COMBO_SERIALPORT_COM的控制变量
	CComboBox m_comboSerialPortCom;
	// 得到当前选择的串口号
	int OnGetSerialPortCom(void);
	// 得到当前选择的波特率
	int OnGetSerialPortBaud(void);
	// 串口通讯控件的控制变量
	CMSComm m_ctrlMSComm1;
	afx_msg void OnBnClickedButtonOpenserialport();
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	
public:
	unsigned short *		m_uiPortNb;
	CUDPSocket* m_pUDPSocket[UDPPortNb];

	unsigned char uart_buf[8][1024];
	unsigned char uart_count;

	// 从串口发送数据
	void OnSendFromSerialPort(unsigned char* TXDATA, unsigned int uiCount);
	// 从UDP发送数据
	void OnSendFromUDP(unsigned char* TXDATA, unsigned int uiCount);
};
