#pragma once

// CSocketSampData ÃüÁîÄ¿±ê
class CMonitorDlg; 
class CSocketSampData : public CSocket
{
public:
	CMonitorDlg* m_pDlg;
public:
	CSocketSampData();
	virtual ~CSocketSampData();
	virtual void OnReceive(int nErrorCode);
	bool    CreateSocket(WORD  wPort,CDialog* pDlg);
};


