// SocketSampData.cpp : 实现文件
//

#include "stdafx.h"
#include "monitor.h"
#include "SocketSampData.h"
#include "Monitordlg.h"

// CSocketSampData

CSocketSampData::CSocketSampData()
: m_pDlg(NULL)
{
}

CSocketSampData::~CSocketSampData()
{
}


// CSocketSampData 成员函数
bool    CSocketSampData::CreateSocket(WORD  wPort,CDialog* pDlg)
{
	this->m_pDlg = (CMonitorDlg*)pDlg;
	// 创建socket
	if(!this->Create(wPort,SOCK_DGRAM))
	{		
		return false;
	}
	// 设置接收缓冲区大小
	int RevBuf = 2048*1024;
	if (this->SetSockOpt( SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		this->Close();		
		return false;
	}
	// 设置发送缓冲区大小
	if (this->SetSockOpt(SO_SNDBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		this->Close();		
		return false;
	}
	// 设置允许接受广播信息
	RevBuf = 1;
	if (this->SetSockOpt(SO_BROADCAST, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		this->Close();		
		return false;
	}
	return true;
}
void CSocketSampData::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_pDlg)
	{
		m_pDlg->AnalysisProtocolOfSampleSock();
	}
	CSocket::OnReceive(nErrorCode);
}
