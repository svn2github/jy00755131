#pragma once

// CUartToUdpSocket 命令目标

class CUartToUdpSocket : public CSocket
{
public:
	CUartToUdpSocket();
	virtual ~CUartToUdpSocket();
	virtual void OnReceive(int nErrorCode);
protected:
	// UDP接收缓冲区计数
	unsigned char m_ucUdpBuf[8][1024];
	unsigned short m_usUdpBufCount;
	// 从串口发送数据
	void OnSendFromSerialPort(unsigned char* TXDATA, unsigned int uiCount);
};


