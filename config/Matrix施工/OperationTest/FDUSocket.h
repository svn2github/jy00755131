#pragma once

// CFDUSocket ÃüÁîÄ¿±ê

class COperationTestDlg;
class CFDUSocket : public CSocket
{
public:
	CFDUSocket();
	virtual ~CFDUSocket();
	virtual void OnReceive(int nErrorCode);
	COperationTestDlg* m_pDlg;
};


