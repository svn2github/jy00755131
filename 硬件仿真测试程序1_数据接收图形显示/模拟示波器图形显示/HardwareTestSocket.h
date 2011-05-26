#pragma once

// CHardwareTestSocket ÃüÁîÄ¿±ê
class CHardwareTestDlg;
class CHardwareTestSocket : public CSocket
{
public:
	CHardwareTestSocket(CHardwareTestDlg* pDlg);
	virtual ~CHardwareTestSocket();
public:
	virtual void OnReceive(int nErrorCode);
protected:
	unsigned char udp_buf[8][256];
	unsigned char udp_count;
	CHardwareTestDlg* m_pDlg;
};


