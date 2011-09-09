// MainCrossSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationTest.h"
#include "MainCrossSocket.h"
#include "OperationTestDlg.h"

// CMainCrossSocket

CMainCrossSocket::CMainCrossSocket()
{
	m_pDlg = NULL;
}

CMainCrossSocket::~CMainCrossSocket()
{
}


// CMainCrossSocket 成员函数

void CMainCrossSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	unsigned char  pBuf[256];
	WORD   wPort;
	DWORD  dwIP;
	sockaddr_in MainCrsAddr;
	MainCrsAddr.sin_family = AF_INET;
		
	if(Receive(pBuf,256)>0)
	{
		wPort = MAKEWORD(pBuf[24],pBuf[25]);
		memcpy_s(&dwIP,4,pBuf+20,4);
		// if(dwIP ==0 || dwIP ==)
		if(m_pDlg)
		{
			MainCrsAddr.sin_port = htons(wPort);
			MainCrsAddr.sin_addr.s_addr = dwIP;
			SendTo(pBuf,256,(sockaddr*)&MainCrsAddr,sizeof(sockaddr_in));
			
			m_pDlg->SaveMatrixData(pBuf,256);
			m_pDlg->m_strMainIP.Format(_T("%d.%d.%d.%d"),MainCrsAddr.sin_addr.S_un.S_un_b.s_b1,MainCrsAddr.sin_addr.S_un.S_un_b.s_b2,
				MainCrsAddr.sin_addr.S_un.S_un_b.s_b3,MainCrsAddr.sin_addr.S_un.S_un_b.s_b4);
			m_pDlg->m_dwFrmCount++;
			m_pDlg->UpdateData(FALSE);
		}
	}
	CSocket::OnReceive(nErrorCode);
}
