#pragma once

// CRecSocket428XL 命令目标

class CRecSocket428XL : public CSocket
{
public:
	CRecSocket428XL();
	virtual ~CRecSocket428XL();
	virtual void OnReceive(int nErrorCode);
public:
	// 428XL端接收缓冲区
	unsigned char m_ucRecBuf428XL[256];
};


