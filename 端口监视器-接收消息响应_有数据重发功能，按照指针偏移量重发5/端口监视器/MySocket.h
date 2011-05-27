#pragma once

// CMySocket 命令目标
class CPortMonitoringDlg;
class CMySocket : public CSocket
{
public:
	CMySocket(CPortMonitoringDlg* pTDoc);
	virtual ~CMySocket();
public:
	virtual void OnReceive(int nErrorCode);

protected:
	unsigned char udp_buf[8][1024];
	unsigned char udp_count;
	CPortMonitoringDlg* m_pTDoc;
	// UDP接收帧指针偏移量
	unsigned int m_uiUdpCount;
	// UDP接收缓冲区
	unsigned char m_ucUdpBuf[8192];
};


