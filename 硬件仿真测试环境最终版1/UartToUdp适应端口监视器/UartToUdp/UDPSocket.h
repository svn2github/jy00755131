#pragma once

// CUDPSocket ÃüÁîÄ¿±ê
class CUartToUdpDlg;
class CUDPSocket : public CAsyncSocket
{
public:
	CUDPSocket(CUartToUdpDlg* pTDoc);
	virtual ~CUDPSocket();
	virtual void OnReceive(int nErrorCode);
	unsigned char udp_buf[8][1024];
	unsigned char udp_count;
	CUartToUdpDlg* m_pTDoc;
};


