#pragma once
#include "afxwin.h"
#include "mscomm1.h"
#include "UartToUdpSocket.h"
#include "Parameter.h"

//__________________串口端口定义__________________
#define COM0 "COM1"
#define COM1 "COM2"
#define COM2 "COM3"
#define COM3 "COM4"
#define COM4 "COM5"
#define COM5 "COM6"
#define COM6 "COM7"
#define COM7 "COM8"
#define COM8 "COM9"
#define COM16	"COM17"

//__________________串口波特率定义________________
#define BAUD0 "300"
#define BAUD1 "600"
#define BAUD2 "1200"
#define BAUD3 "2400"
#define BAUD4 "4800"
#define BAUD5 "9600"
#define BAUD6 "19200"
#define BAUD7 "38400"
#define BAUD8 "43000"
#define BAUD9 "56000"
#define BAUD10 "57600"
#define BAUD11 "115200"

// CTabUartToUdp 对话框

class CTabUartToUdp : public CDialog
{
	DECLARE_DYNAMIC(CTabUartToUdp)

public:
	CTabUartToUdp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabUartToUdp();

// 对话框数据
	enum { IDD = IDD_DIALOG_UARTTOUDP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	// 控件IDC_COMBO_SERIALPORT_COM的控制变量
	CComboBox m_comboSerialPortCom;
	// 控件IDC_COMBO_SERIALPORT_BAUD的控制变量
	CComboBox m_comboSerialPortBaud;
	// 串口通讯控件的控制变量
//	CMscomm1 m_ctrlMSComm1;
	// 得到当前选择的串口号
	int OnGetSerialPortCom(void);
	// 得到当前选择的波特率
	int OnGetSerialPortBaud(void);
	// 串口转UDP通讯Socket
	CUartToUdpSocket m_socketUartToUdp;
	// 串口数据接收数组
	unsigned char m_ucUartBuf[8][RcvFrameSize];
	// 串口接收缓冲区计数
	unsigned short m_usUartBufCount;
	// 串口成功接收数据计数
	unsigned int m_uiUartCount;
	// 从UDP发送数据
	void OnSendFromUDP(unsigned char* TXDATA, unsigned int uiCount);
public:
	// 初始化设置
	void OnInit(void);
	afx_msg void OnBnClickedButtonSerialportOpen();
	afx_msg void OnBnClickedButtonSerialportClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 使能控件
	void ControlEnable(void);
	// 禁用控件
	void ControlDisable(void);
	// 打开串口
	void OnOpenCom(void);
	// 关闭串口
//	void OnCloseCom(void);
	// 串口接收数据
//	void OnUartReceiveData(unsigned char* lpbuffer, unsigned int dwBytes);
	// 关闭
	void OnClose(void);
// 	// 串口接收线程函数结束标志位
// 	BOOL m_bUartThreadclose;
	// 串口通讯控件的控制变量
	CMscomm1 m_ctrlMSComm1;
	// 选择串口端口的复选框序号
	unsigned int m_uiSerialPortComCurSel;
	// 选择串口波特率的复选框序号
	unsigned int m_uiSerialPortBaudCurSel;
	// UDP发送目标端口
	unsigned int m_uiSendPort;
	// UDP接收端口
	unsigned int m_uiRecPort;
	// 服务器的IP地址
	CString m_csIPAddress;
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};
