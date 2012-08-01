#include "stdafx.h"
#include "MatrixCommDll.h"

CClientSocket::CClientSocket()
{
	memset(m_cRecBuf, 0, ServerRecBufSize);
	m_iPosRec = 0;
	m_iPosProc = 0;
	m_usFrameInfoSize = 0;
	m_pComClient = NULL;
	m_strServerIP = ServerIP;
	m_uiServerPort = ServerListenPort;
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 成员函数

// 处理接收帧
void CClientSocket::OnProcRec(int iSize)
{
	unsigned char ch = 0;
	for (int i=0; i<iSize;)
	{
		ch = m_cRecBuf[i + m_iPosRec];
		switch(m_iPosProc)
		{
		case 0:
			if (ch != FrameHead1)
			{
				m_iPosProc = 0;
			}
			else
			{
				m_iPosProc++;
			}
			i++;
			break;
		case 1:
			if (ch != FrameHead2)
			{
				m_iPosProc = 0;
			}
			else
			{
				m_iPosProc++;
			}
			i++;
			break;
		case 2:
			if (ch != FrameHead3)
			{
				m_iPosProc = 0;
			}
			else
			{
				m_iPosProc++;
			}
			i++;
			break;
		case 3:
			if (ch != FrameHead4)
			{
				m_iPosProc = 0;
			}
			else
			{
				m_iPosProc++;
			}
			i++;
			break;
		case 4:
			m_usFrameInfoSize = ch;
			m_iPosProc++;
			i++;
			break;
		case 5:
			m_usFrameInfoSize += ch << 8;
			// 大于帧内容长度限制+帧尾
			if (m_usFrameInfoSize > (FrameInfoSizeLimit + FrameTailSize + FrameHeadInfoSize))
			{
				m_iPosProc = 0;
			}
			else if (m_usFrameInfoSize < (FrameHeadInfoSize + FrameTailSize))
			{
				m_iPosProc = 0;
			}
			else
			{
				m_iPosProc++;
			}
			i++;
			break;
		default:
			if (m_iPosProc == (m_usFrameInfoSize + FrameHeadSize + FrameLengthSize - FrameTailSize))
			{
				if (ch != FrameTail)
				{
					i = FrameHeadSize;
				}
				else
				{
					// 帧解析
					m_pComClient->m_oRecFrame.PhraseFrame(m_cRecBuf + (i + m_iPosRec + FrameTailSize - m_usFrameInfoSize), 
						m_usFrameInfoSize - FrameTailSize - FrameHeadInfoSize);
					i++;
				}
				m_iPosProc = 0;
			}
			else
			{
				m_iPosProc++;
				i++;
			}
			break;
		}
	}
	if (m_iPosProc != 0)
	{
		memmove_s(m_cRecBuf, ServerRecBufSize, m_cRecBuf + (m_iPosRec + iSize - m_iPosProc), m_iPosProc);
	}
	m_iPosRec = m_iPosProc;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int iSize = 0;
	iSize = Receive(m_cRecBuf + m_iPosRec, ServerRecBufSize - m_iPosRec);
	if(iSize == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
		{
			// 输出错误日志

		}
	}
	//没有数据
	else if(iSize == 0)
	{
		// 输出错误日志

		Close();
	}
	else
	{
		OnProcRec(iSize);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pComClient->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}
// 设置Socket缓冲区大小
void CClientSocket::SetSocketBuffer(int iSndBufferSize, int iRcvBufferSize)
{
	CString str = _T("");
	if (SOCKET_ERROR == setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF,  
		reinterpret_cast<const char *>(&iSndBufferSize), sizeof(int)))
	{
		str.Format(_T("Client Socket Set SndBuf Error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
	if (SOCKET_ERROR == setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF,  
		reinterpret_cast<const char *>(&iRcvBufferSize), sizeof(int)))
	{
		str.Format(_T("Client Socket Set RcvBuf Error %d"), WSAGetLastError());
		AfxMessageBox(str);
	}
}
// 初始化
void CClientSocket::OnInit(CCommClient* pComClient,int iSndBufferSize, int iRcvBufferSize)
{
	m_pComClient = pComClient;
	m_pComClient->m_pComClientMap->insert(hash_map<SOCKET, CCommClient*>::value_type (m_hSocket, pComClient));
	SetSocketBuffer(iSndBufferSize, iRcvBufferSize);
}
// 关闭
void CClientSocket::OnClose(void)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	iter = m_pComClient->m_pComClientMap->find(m_hSocket);
	if (iter != m_pComClient->m_pComClientMap->end())
	{
		m_pComClient->m_pComClientMap->erase(iter);
	}
}

// 连接服务器
void CClientSocket::ConnectServer(void)
{
	int iError = 0;
	CString str = _T("");
	if (FALSE == Connect(m_strServerIP, m_uiServerPort))
	{
		iError = GetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			str.Format(_T("Client can not connect to Server, Error is %d!"), iError);
			AfxMessageBox(str);
		}
	}
}