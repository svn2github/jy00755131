#include "stdafx.h"
#include "MatrixCommDll.h"

CCommServer::CCommServer()
{
	m_pComClientMap = NULL;
}

CCommServer::~CCommServer()
{
}


// CComServer 成员函数


void CCommServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	SOCKADDR addr;
	unsigned char IP4[4];
	int iaddrLen = sizeof(SOCKADDR);
	CCommClient* pComClient = NULL;
	pComClient = new CCommClient;
	CString strDstFolderPath = _T("");
	CString strSrcFolderPath = _T("");
	CString strDstFilePath = _T("");
	CString strSrcFilePath = _T("");
	CloseInvalidClient();
	if (CAsyncSocket::Accept(pComClient->m_oClientSocket, &addr, &iaddrLen))
	{
		memcpy(IP4, &addr.sa_data[2], 4);
		pComClient->m_strClientIP.Format(_T("%u.%u.%u.%u"), IP4[0], 
			IP4[1], IP4[2], IP4[3]);
		strDstFolderPath = ClientFolderPath;
		if (_taccess(strDstFolderPath, 0) == -1)
		{
			CreateDirectory(strDstFolderPath, NULL);
		}
		strDstFolderPath += _T("\\");
		strDstFolderPath += pComClient->m_strClientIP;
		strSrcFolderPath = ServerFolderPath;
		if (_taccess(strDstFolderPath, 0) == -1)
		{
			CreateDirectory(strDstFolderPath, NULL);
		}
		strDstFilePath = strDstFolderPath + LineXmlFileName;
		strSrcFilePath = strSrcFolderPath + LineXmlFileName;
		if (_taccess(strDstFilePath, 0) == -1)
		{
			CopyFile(strSrcFilePath, strDstFilePath, TRUE);
		}
		pComClient->m_strLinePath = strDstFilePath;
		strDstFilePath = strDstFolderPath + OptXmlFileName;
		strSrcFilePath = strSrcFolderPath + OptXmlFileName;
		if (_taccess(strDstFilePath, 0) == -1)
		{
			CopyFile(strSrcFilePath, strDstFilePath, TRUE);
		}
		pComClient->m_strOptPath = strDstFilePath;
		pComClient->m_oRecThread.m_pCommClient = pComClient;
		pComClient->m_pComClientMap = m_pComClientMap;
		pComClient->m_oProcRecCmdCallBack = m_oProcRecCmdCallBack;
		pComClient->m_oCloseClientXml = m_oProcCloseClientXML;
		pComClient->OnInit();
	}
	else
	{
		delete pComClient;
		pComClient = NULL;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


// 初始化
void CCommServer::OnInit(unsigned int uiSocketPort, int iSocketType, LPCTSTR lpszSocketAddress)
{
	BOOL bFlag = FALSE;
	bFlag = CAsyncSocket::Create(uiSocketPort, iSocketType, 
		FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE, lpszSocketAddress);
	if (!bFlag)
	{
		AfxMessageBox(_T("Server Socket Create Error or Server already exists!"));
		PostQuitMessage(0);
		return;
	}
	CAsyncSocket::Listen(ListenClientMaxNum);
}


// 关闭
void CCommServer::OnClose(void)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	int iSize = m_pComClientMap->size();
	for (int i=0; i<iSize; i++)
	{
		iter = m_pComClientMap->begin();
		iter->second->OnClose();
	}
	Close();
}

void CCommServer::CloseInvalidClient(void)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	list<CCommClient*>::iterator iterList;
	bool bConnectValid = true;
	for (iter = m_pComClientMap->begin(); iter != m_pComClientMap->end(); iter++)
	{
		EnterCriticalSection(&iter->second->m_oSndFrame.m_oSecClientFrame);
		bConnectValid = iter->second->m_oSndFrame.m_bConnectValid;
		LeaveCriticalSection(&iter->second->m_oSndFrame.m_oSecClientFrame);
		if (bConnectValid == false)
		{
			m_olsClientClose.push_back(iter->second);
		}
	}
	for (iterList = m_olsClientClose.begin(); iterList != m_olsClientClose.end(); iterList++)
	{
		(*iterList)->OnClose();
	}
	m_olsClientClose.clear();
}