#pragma once

// CMySocket2 ÃüÁîÄ¿±ê
class CPortMonitoringDlg;
class CMySocket2 : public CSocket
{
public:
	CMySocket2(CPortMonitoringDlg* pTDoc);
	virtual ~CMySocket2();
public:
	virtual void OnReceive(int nErrorCode);

protected:
	unsigned char udp_buf[8][1024];
	unsigned char udp_count;
	CPortMonitoringDlg* m_pTDoc;
};


