// SocketLogInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "AdminServer.h"
#include "SocketLogInfo.h"


// CSocketLogInfo

CSocketLogInfo::CSocketLogInfo()
{
}

CSocketLogInfo::~CSocketLogInfo()
{
}


// CSocketLogInfo 成员函数
/**
 * @brief 创建Socket
 * @note  创建Socket端口
 * @param  UINT wPort,socket端口
 * @param  LPCTSTR strHostIP，本机IP地址
 * @return 创建成功则返回true，失败则返回false
 */
bool CSocketLogInfo::CreateSocket(UINT wPort, LPCTSTR strHostIP)
{
	// 创建socket
	if(!this->Create(wPort,SOCK_DGRAM,strHostIP))
	{		
		return false;
	}
	// 设置接收缓冲区大小
	int RevBuf = 512*1024;
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
void CSocketLogInfo::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	static unsigned char	pBuf[SOCKET_LOGFRM_MAXLEN];	// 数据缓冲区最大长度是发送日志数据帧最大长度
	WORD			wFrmLen = SOCKET_LOGFRM_MAXLEN;		// 帧长: 初始化为缓冲区的大小，被ReceiveFrom函数使用
	WORD			wFrmCmdCode;						// 命令码
	DWORD			dwFrmIndex;							// 帧计数	
	int				nSize,nReadSize;
	sockaddr_in		sockClient;
	sockClient.sin_family = AF_INET;
	nSize = sizeof(sockClient);
	
	// 接收来自客户端的数据
	nReadSize = ReceiveFrom( pBuf, wFrmLen, (SOCKADDR*)&sockClient , &nSize );
	switch (nReadSize)
	{
	case 0:		// 没有收到数据 
		return;
		break;
	case SOCKET_ERROR:	// 发生错误，记录日志
		if (GetLastError() != WSAEWOULDBLOCK) 
		{  
			TRACE0("CSocketLog's ReceiveFrom error ！");
			return;
		}
		break;
	default:
		break;
	}
	// -----------解析帧格式---------------------------------------------------------
	// 判断帧头是否0xEB, 0x90
	if (pBuf[0]!=0xEB || pBuf[1]!=0x90)
	{
		TRACE2("CSocketLog's ReceiveFrom！Frame head(%d,%d)is wrong！",pBuf[0],pBuf[1]);		
		return;
	}
	// 计算帧长度
	wFrmLen = MAKEWORD(pBuf[2],pBuf[3]);	
	// 计算帧计数
	memcpy_s(&dwFrmIndex,sizeof(dwFrmIndex),pBuf+4 , 4);
	// 帧尾
	if(pBuf[wFrmLen-2]!=0x14 || pBuf[wFrmLen-1]!=0x6F)
	{
		// 帧尾错误，需要重新定位帧头,帧长度超长，发生错误记录日志		
		TRACE2("CSocketLog's ReceiveFrom！Frame end(%d,%d)is wrong！",pBuf[wFrmLen-2],pBuf[wFrmLen-1]);		
		return;
	}
	// -------------------解析命令码，按照命令码进行相应操作-------------------------------
	// 命令码
	wFrmCmdCode=MAKEWORD(pBuf[9],pBuf[10]);
	if(wFrmCmdCode==0xFFFF)
	{
		// 日志记录帧
		if(theApp.m_Log.ReadFromSocketFrame(pBuf)>0)
		{
			theApp.m_LogFile.WriteLog(&(theApp.m_Log));
			if(theApp.m_Log.IsShowed())
				SendTo(pBuf,nReadSize,START_USERANDLOGPORT,_T("255.255.255.255"));
		}
	}
	CSocket::OnReceive(nErrorCode);
}
