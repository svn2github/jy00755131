#pragma once
#include "afxsock.h"

//////////////////////////////////////////////////////////////////////////
/**
 * @class CSocketShotClient
 * @brief 与客户端通信类
 *
 * 与客户端的通信接口。接受客户端的指令，返回放炮状态。
 */
class COperationServerDlg;
class CSocketShotClient :
	public CSocket
{
public:
	COperationServerDlg* m_pDlg;

	CSocketShotClient(void);
	~CSocketShotClient(void);
	virtual void OnReceive(int nErrorCode);

	bool CreateClientSocket(UINT wPort, LPCTSTR strHostIP,COperationServerDlg* pDlg);
	// int SendAckOfRequest(sockaddr_in* lpSockAddr,DWORD dwFrmIndex,DWORD dwShotNb,DWORD dwSourceNb, int nThreadIndex);
	// int SendDataToClient(sockaddr_in* lpSockAddr,unsigned char* lpBuf ,int nSize);
	int SendDataToClient(DWORD dwSockAddr,WORD wClientPort,unsigned char* lpBuf ,int nSize);
};
