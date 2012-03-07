// ShotSvrSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "ShotSvrSocket.h"
#include "MainFrm.h"

// CShotSvrSocket

CSocketShotServer::CSocketShotServer()
{
}

CSocketShotServer::~CSocketShotServer()
{
}


// CShotSvrSocket 成员函数
/**
 * @brief 创建Socket
 * @note  创建Socket端口
 * @param  UINT wPort,socket端口
 * @param  LPCTSTR strHostIP，本机IP地址
 * @return 创建成功则返回true，失败则返回false
 */
bool CSocketShotServer::CreateSocket(UINT wPort, LPCTSTR strHostIP)
{
	// 创建socket
	if(!this->Create(wPort,SOCK_DGRAM,strHostIP))
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
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 接收来自服务器的数据

    修改历史：
*******************************************************************************/
void CSocketShotServer::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	// cxm 2012.3.7
// 	CMainFrame*	pMainFram =(CMainFrame*)AfxGetMainWnd();
// 	if(pMainFram)
// 	{
// 		pMainFram->AnalysisProtocolOfServerSock();
// 	}
	CSocket::OnReceive(nErrorCode);
}
