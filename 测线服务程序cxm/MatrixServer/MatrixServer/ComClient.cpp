// ComClient.cpp : 实现文件
//

#include "stdafx.h"
#include "MatrixServer.h"
#include "ComClient.h"


// CComClient

CComClient::CComClient()
{
	m_pComClientMap = NULL;
	memset(m_cRecBuf, 0, RecBufSize);
	m_iPosRec = 0;
	m_iPosProc = 0;
	m_usFrameInfoSize = 0;
}

CComClient::~CComClient()
{
	m_oClientRecFrame.OnClose();
}


void CComClient::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int iSize = 0;
	unsigned char ch = 0;
	iSize = Receive(m_cRecBuf + m_iPosRec, RecBufSize - m_iPosRec);
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
				if (m_usFrameInfoSize > (FrameInfoSizeLimit + 1))
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
				if (m_iPosProc == (m_usFrameInfoSize + 5))
				{
					if (ch != FrameTail)
					{
						i = 3;
					}
					else
					{
						// 帧解析
						m_oClientRecFrame.PhraseFrame(m_cRecBuf + (i + m_iPosRec + 1 - m_usFrameInfoSize), 
							m_usFrameInfoSize - 1);
					}
					m_iPosProc = 0;
				}
				else
				{
					m_iPosProc++;
				}
				i++;
				break;
			}
		}
		if (m_iPosProc != 0)
		{
			memmove_s(m_cRecBuf, RecBufSize, m_cRecBuf + (m_iPosRec + iSize - m_iPosProc), m_iPosProc);
		}
		m_iPosRec = m_iPosProc;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void CComClient::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnSend(nErrorCode);
}


void CComClient::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	FreeSocketInformation();
	CAsyncSocket::OnClose(nErrorCode);
}
// 创建一个客户端连接信息
void CComClient::CreateSocketInformation(void)
{
	m_pComClientMap->insert(hash_map<SOCKET, CComClient*>::value_type (m_hSocket, this));
	m_oClientRecFrame.OnInit();
}

// 释放一个客户端连接信息
void CComClient::FreeSocketInformation(void)
{
	hash_map<SOCKET, CComClient*>::iterator iter;
	iter = m_pComClientMap->find(this->m_hSocket);
	if (iter != m_pComClientMap->end())
	{
		m_pComClientMap->erase(iter);
	}
	delete this;
}


