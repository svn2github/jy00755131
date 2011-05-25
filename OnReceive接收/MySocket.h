#pragma once

// CMySocket 命令目标

class CMySocket : public CSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	virtual void OnReceive(int nErrorCode);
	// 接收到的字节数
	unsigned int m_uiRcvByteNum;
};


