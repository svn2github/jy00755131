// ShotSvrSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Operation.h"
#include "ShotSvrSocket.h"
#include "MainFrm.h"

// CShotSvrSocket

CShotServerSocket::CShotServerSocket()
{
}

CShotServerSocket::~CShotServerSocket()
{
}


// CShotSvrSocket 成员函数
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 接收来自服务器的数据

    修改历史：
*******************************************************************************/
void CShotServerSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame*	pMainFram =(CMainFrame*)AfxGetMainWnd();
	pMainFram->AnalysisProtocolOfServerSock();
	CSocket::OnReceive(nErrorCode);
}
