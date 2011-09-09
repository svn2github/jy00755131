// SocketShotBlaster.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationServer.h"
#include "OperationServerDlg.h"
#include "SocketShotBlaster.h"
#include "OperationServerParseXML.h"

// CSocketShotBlaster

CSocketShotBlaster::CSocketShotBlaster()
: m_pDlg(NULL)
{
}

CSocketShotBlaster::~CSocketShotBlaster()
{
}

// CSocketShotBlaster 成员函数
bool CSocketShotBlaster::CreateSocket(WORD wPort,LPCTSTR strIP,COperationServerDlg* pDlg)
{
	m_pDlg = pDlg;
	// 创建socket
	if(!this->Create(wPort,SOCK_DGRAM,strIP))	//,strHostIP
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
	CString   strHostIP = strIP;
	m_dwHostIP = COperationServerParseXML::StringIPToDWORDIP(strHostIP);
	m_wBlasterPort = wPort;
	return true;	
}

/**
 * @brief 向采集站发送数据
 * @note  基础的发送函数，主要封装IP地址和端口的转换。
 * @param  pBuf，发送数据缓冲区指针
 * @param  nSize，数据缓冲区的大小 
 * @return 返回已发送的字节数
 */
int  CSocketShotBlaster::SendToFDU(unsigned char* pBuf,int nSize)
{
	sockaddr_in MainCrsAddr;
	MainCrsAddr.sin_family = AF_INET;
	// 主交叉站端口
	MainCrsAddr.sin_port = htons(theApp.m_wMainCrossPort);
	// 主交叉站地址
	MainCrsAddr.sin_addr.s_addr = theApp.m_dwMainCrossIP;
	return SendTo((char*)pBuf,nSize,(sockaddr*)&MainCrsAddr,sizeof(sockaddr_in));	
}
/**
 * @brief 接收数据处理函数
 * @note  重载接收函数
 * @param  int nErrorCode,错误码 
 * @return void
 */
void CSocketShotBlaster::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	if(m_pDlg)
	{
		m_pDlg->AnalysisProtocolOfBlasterSock();
	}
	CSocket::OnReceive(nErrorCode);
}