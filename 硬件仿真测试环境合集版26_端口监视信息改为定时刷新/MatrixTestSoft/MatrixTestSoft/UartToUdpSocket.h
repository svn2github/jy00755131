#pragma once
#include "mscomm1.h"
#include "Parameter.h"
// CUartToUdpSocket 命令目标

class CUartToUdpSocket : public CSocket
{
public:
	CUartToUdpSocket();
	virtual ~CUartToUdpSocket();
	virtual void OnReceive(int nErrorCode);
protected:
	// UDP接收缓冲区计数
	unsigned char m_ucUdpBuf[RcvBufNum][UartToUdpRcvSize];
	unsigned short m_usUdpBufCount;
	// 从串口发送数据
	void OnSendFromSerialPort(unsigned char* TXDATA, unsigned int uiCount);
public:
	// 串口通讯控件的控制变量
	CMscomm1* m_pctrlMSComm1;
};


