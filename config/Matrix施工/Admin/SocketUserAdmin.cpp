// SocketUserAdmin.cpp : 实现文件
//

#include "stdafx.h"
#include "Admin.h"
#include "SocketUserAdmin.h"
#include "..\common\UserInfo.h"
#include "MainFrm.h"
// CSocketUserAdmin

CSocketUserAdmin::CSocketUserAdmin()
{
}

CSocketUserAdmin::~CSocketUserAdmin()
{
}


// CSocketUserAdmin 成员函数
/**
 * @brief 创建Socket
 * @note  创建Socket端口
 * @param  UINT wPort,socket端口
 * @param  LPCTSTR strHostIP，本机IP地址
 * @return 创建成功则返回true，失败则返回false
 */
bool CSocketUserAdmin::CreateSocket(UINT wPort, LPCTSTR strHostIP)
{
// 	this->m_pDlg = pDlg;
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
/**
 * @brief 请求所有用户信息
 * @note  按照帧格式要求，发送请求所有用户信息的命令帧
 * @param null
 * @return int
 */
int CSocketUserAdmin::SendFrm_ReqUsersInfo()
{	
	BYTE	pBuf[16];
	
	// 帧头
	pBuf[0]= 0xEB;
	pBuf[1]= 0x90;
	// 帧长
	pBuf[2] = 16;
	pBuf[3] = 0;
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 1;
	// 命令码
	pBuf[9] = LOBYTE(USERINFOFRM_CMDCODE_QUERY);
	pBuf[10] = HIBYTE(USERINFOFRM_CMDCODE_QUERY);
	// 数据个数
	pBuf[11]= 0;
	pBuf[12]= 0;	
	
	// 帧校验
	pBuf[13]= 0x00;
	// 帧尾
	pBuf[14]= 0x14;
	pBuf[15]= 0x6F;
	return SendTo(pBuf,16,ADMINSERVER_USERINFOPORT,theApp.m_strAdminServerIP);
	
}
/**
 * @brief 发送全部用户信息
 * @note  按照帧格式要求，发送用户信息
 * @param sockaddr_in* psockaddr,客户端Socket信息
 * @return 成功，返回true，失败返回false
 */
bool  CSocketUserAdmin::SendFrm_SendAllUserInfo()
{	
	unsigned char	pBuf[SOCKET_USERINFOFRM_MAXLEN];
	WORD	wUserSize=0;
	WORD	wFrmLen=0;
	WORD	wUserCount = (WORD)theApp.m_AllUserInfo.GetCount();
	// 帧头
	pBuf[0]= 0xEB;
	pBuf[1]= 0x90;
	
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 1;
	// 命令码
	pBuf[9] = LOBYTE(USERINFOFRM_CMDCODE_ALLUSERS);
	pBuf[10] = HIBYTE(USERINFOFRM_CMDCODE_ALLUSERS);
	// 数据个数
	pBuf[11]= LOBYTE(wUserCount);
	pBuf[12]= HIBYTE(wUserCount);	
	wFrmLen = 13;
	CUserInfo* pUserInfo = NULL;
	for (int i=0;i<wUserCount;i++)
	{
		pUserInfo = theApp.m_AllUserInfo.GetUserInfo(i);
		if(!pUserInfo)
			continue;
		wUserSize = (WORD)pUserInfo->Write(pBuf+wFrmLen,SOCKET_USERINFOFRM_MAXLEN-wFrmLen);
		wFrmLen+=wUserSize;
	}	
	
	// 帧校验
	pBuf[wFrmLen++]= 0x00;
	// 帧尾
	pBuf[wFrmLen++]= 0x14;
	pBuf[wFrmLen++]= 0x6F;
	// 帧长
	pBuf[2] = LOBYTE(wFrmLen);
	pBuf[3] = HIBYTE(wFrmLen);
	SendTo(pBuf,wFrmLen,ADMINSERVER_USERINFOPORT,theApp.m_strAdminServerIP);
	return true;
}
/**
 * @brief 接收数据
 * @note  接收并处理来自客户端的用户数据。
 帧格式为：
     帧头（2byte）+帧长（2byte）+帧计数（4byte）+帧类型（1byte）+命令码/状态码（2byte）+用户个数（2byte）+用户数据（字节数不定）+校验位(1Byte)+帧尾(2Byte)
 * @param  int nErrorCode，socket通信错误码
 * @return void
 */
void CSocketUserAdmin::OnReceive(int nErrorCode)
{	
	static unsigned char	pBuf[SOCKET_USERINFOFRM_MAXLEN];	// 数据缓冲区最大长度是发送采集排列的数据帧
	WORD			wFrmLen = SOCKET_USERINFOFRM_MAXLEN;		// 帧长: 初始化为缓冲区的大小，被ReceiveFrom函数使用
	WORD			wFrmCmdCode;			// 命令码
	DWORD			dwFrmIndex;				// 帧计数	
	int				nSize,nReadSize;
	//DWORD			dwTemp;
	//CString         strTemp;

	sockaddr_in sockClient;
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
			TRACE0("CSocketUserAdmin's ReceiveFrom error ！");
			// WriteLogMsg(strTemp);
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
		TRACE2("CSocketUserAdmin's ReceiveFrom！Frame head(%d,%d)is wrong！",pBuf[0],pBuf[1]);		
		return;
	}
	// 计算帧长度
	wFrmLen = MAKEWORD(pBuf[2],pBuf[3]);
	if(wFrmLen>SOCKET_USERINFOFRM_MAXLEN)
	{	// 帧长度超长，发生错误记录日志		
		TRACE1("CSocketUserInfo's ReceiveFrom！Frame length(%d)is wrong！",wFrmLen);		
		return;
	}
	// 计算帧计数
	memcpy_s(&dwFrmIndex,sizeof(dwFrmIndex),pBuf+4 , 4);
	// 帧尾
	if(pBuf[wFrmLen-2]!=0x14 || pBuf[wFrmLen-1]!=0x6F)
	{
		// 帧尾错误，需要重新定位帧头
		// 帧长度超长，发生错误记录日志		
		TRACE2("CSocketUserInfo's ReceiveFrom！Frame end(%d,%d)is wrong！",pBuf[wFrmLen-2],pBuf[wFrmLen-1]);		
		return;
	}
	// -------------------解析命令码，按照命令码进行相应操作-------------------------------
	// 命令码
	wFrmCmdCode=MAKEWORD(pBuf[9],pBuf[10]);
	switch(wFrmCmdCode)
	{
	case USERINFOFRM_CMDCODE_ALLUSERS:	// 所有用户信息
		AnalysisCmd_AllUserInfo(pBuf,nReadSize);
		break;
	default:
		break;
	}

	CSocket::OnReceive(nErrorCode);
}
/**
 * @brief 解析所有用户信息
 * @note  按照帧格式要求，解析所有用户信息，保存在theApp.m_AllUserInfo变量中
 * @param  unsigned char* pBuf,数据帧缓冲区
 * @param int nSize,数据帧大小
 * @return 解析成功，返回true，失败返回false
 */
bool CSocketUserAdmin::AnalysisCmd_AllUserInfo(BYTE* pBuf,int nSize)
{
	int nUserCount;
	int nlen,nPos=0;
	// 得到用户记录的总数
	nUserCount = MAKEWORD(pBuf[11],pBuf[12]);
	if(!nUserCount)
		return true;
	theApp.m_AllUserInfo.RemoveAll();
	CUserInfo* pUserinfo=NULL;
	
	// 从缓冲区依次读取用户对象CUserInfo,保存在管理对象中
	nPos = 13;
	for(int i=0;i<nUserCount;i++)
	{
		pUserinfo = new CUserInfo;
		if(!pUserinfo)
			continue;
		nlen = pUserinfo->Read(pBuf+nPos,nSize-nPos);
		nPos+=nlen;
		if(nlen)
		{
			theApp.m_AllUserInfo.Add(pUserinfo);
		}
		else
		{
			// 读缓冲区失败，则删除对象，不放入用户管理对象中
			delete pUserinfo;
			continue;
		}		
	}
	// 发送消息通知主窗口显示用户信息
	CMainFrame*	pFrm = (CMainFrame*)AfxGetMainWnd();
	if(!pFrm)
		return false;
	pFrm->SendMessage(WM_GETALLUSERINFO,0,0);
	return true;
	
}