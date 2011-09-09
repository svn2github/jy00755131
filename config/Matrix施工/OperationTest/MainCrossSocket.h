#pragma once

// CMainCrossSocket ÃüÁîÄ¿±ê
class COperationTestDlg;
class CMainCrossSocket : public CSocket
{
public:
	CMainCrossSocket();
	virtual ~CMainCrossSocket();
	virtual void OnReceive(int nErrorCode);


	COperationTestDlg* m_pDlg;
};


