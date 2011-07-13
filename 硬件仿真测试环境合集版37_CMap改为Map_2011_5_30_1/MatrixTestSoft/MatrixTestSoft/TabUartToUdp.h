#pragma once
#include "afxwin.h"
#include "mscomm1.h"
#include "Parameter.h"
#include "LogFile.h"

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
private:
	// 控件IDC_COMBO_SERIALPORT_COM的控制变量
	CComboBox m_comboSerialPortCom;
	// 控件IDC_COMBO_SERIALPORT_BAUD的控制变量
	CComboBox m_comboSerialPortBaud;
	// 串口数据接收数组
	unsigned char m_ucUartBuf[RcvBufNum][RcvFrameSize];
	// 串口接收缓冲区计数
	unsigned short m_usUartBufCount;
	// 串口成功接收数据计数
	unsigned int m_uiUartCount;
	// Socket套接字
	sockaddr_in m_RecvAddr, m_SendToAddr;
	SOCKET m_UdpSocket;
	// UDP接收缓冲区计数
	unsigned char m_ucUdpBuf[RcvBufNum][UartToUdpRcvSize];
	unsigned short m_usUdpBufCount;
public:
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
	// 日志类指针
	CLogFile* m_pLogFile;
private:
	// 得到当前选择的串口号
	int OnGetSerialPortCom(void);
	// 得到当前选择的波特率
	int OnGetSerialPortBaud(void);
	// 从UDP发送数据
	void OnSendFromUDP(unsigned char* TXDATA, unsigned int uiCount);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 避免端口阻塞
	void OnAvoidIOBlock(SOCKET socket);
	// 关闭串口
	void OnCloseCom(void);
	// 关闭UDP端口
	void OnCloseUDP(void);
	// 从串口发送数据
	void OnSendFromSerialPort(unsigned char* TXDATA, unsigned int uiCount);
	// 创建UDP的SOCKET套接字并设置
	void OnCreateUDPSocket(void);
public:
	// 初始化设置
	void OnInit(void);
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
	afx_msg void OnBnClickedButtonSerialportOpen();
	afx_msg void OnBnClickedButtonSerialportClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
};
