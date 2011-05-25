#pragma once

// CRecSocketShotPro 命令目标

class CRecSocketShotPro : public CSocket
{
public:
	CRecSocketShotPro();
	virtual ~CRecSocketShotPro();
	virtual void OnReceive(int nErrorCode);
public:
	// 428XL端接收缓冲区
	unsigned char m_ucRecBufShotPro[128];
};


