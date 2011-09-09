// SocketAdminServer.cpp : 实现文件
//

#include "stdafx.h"
#include "Start.h"
#include "SocketAdminServer.h"
#include "StartDlg.h"

static UINT NEAR WM_NEWLOG = RegisterWindowMessage(MSG_LOG_NEW);
// CSocketAdminServer
CSocketAdminServer::CSocketAdminServer()
: m_pDlg(NULL)
{
}

CSocketAdminServer::~CSocketAdminServer()
{
}


// CSocketAdminServer 成员函数
/**
 * @brief 创建Socket
 * @note  创建Socket端口
 * @param  UINT wPort,socket端口
 * @param  LPCTSTR strHostIP，本机IP地址
 * @return 创建成功则返回true，失败则返回false
 */
bool CSocketAdminServer::CreateSocket(UINT wPort, LPCTSTR strHostIP)
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
 * @brief 接收数据
 * @note  接收并处理来自服务器端的用户数据。
 帧格式为：
     帧头（2byte）+帧长（2byte）+帧计数（4byte）+帧类型（1byte）+命令码/状态码（2byte）+用户个数（2byte）+用户数据（字节数不定）+校验位(1Byte)+帧尾(2Byte)
 * @param  int nErrorCode，socket通信错误码
 * @return void
 */
void CSocketAdminServer::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	static unsigned char	pBuf[SOCKET_USERINFOFRM_MAXLEN];	// 数据缓冲区最大长度是发送采集排列的数据帧
	WORD			wFrmLen = SOCKET_USERINFOFRM_MAXLEN;		// 帧长: 初始化为缓冲区的大小，被ReceiveFrom函数使用
	WORD			wFrmCmdCode;			// 命令码
	DWORD			dwFrmIndex;				// 帧计数	
	int				nSize,nReadSize;

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
			TRACE0("CSocketAdminServer's ReceiveFrom error ！");

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
		TRACE2("CSocketAdminServer's ReceiveFrom！Frame head(%d,%d)is wrong！",pBuf[0],pBuf[1]);		
		return;
	}
	// 计算帧长度
	wFrmLen = MAKEWORD(pBuf[2],pBuf[3]);	
	// 计算帧计数
	memcpy_s(&dwFrmIndex,sizeof(dwFrmIndex),pBuf+4,4);
	// 帧尾
	if(pBuf[wFrmLen-2]!=0x14 || pBuf[wFrmLen-1]!=0x6F)
	{
		// 帧尾错误，需要重新定位帧头
		// 帧长度超长，发生错误记录日志		
		TRACE2("CSocketAdminServer's ReceiveFrom！Frame end(%d,%d)is wrong！",pBuf[wFrmLen-2],pBuf[wFrmLen-1]);		
		return;
	}
	// -------------------解析命令码，按照命令码进行相应操作-------------------------------
	// 命令码
	wFrmCmdCode=MAKEWORD(pBuf[9],pBuf[10]);
	switch(wFrmCmdCode)
	{
	case USERINFOFRM_CMDCODE_AUTHENOK:	// 用户认证成功
		theApp.m_UserInfo.Read(pBuf+13,nReadSize);
		theApp.m_strAdminServerIP.Format(_T("%d.%d.%d.%d"),sockClient.sin_addr.S_un.S_un_b.s_b1
			,sockClient.sin_addr.S_un.S_un_b.s_b2,sockClient.sin_addr.S_un.S_un_b.s_b3,sockClient.sin_addr.S_un.S_un_b.s_b4);
		theApp.m_bUserLogin = TRUE;
		// 写Adminserver 服务器的IP地址
		theApp.m_LogMapping.WriteAdminServerHostIP(sockClient.sin_addr.S_un.S_addr );
		if(m_pDlg)
			m_pDlg->Login_Success();
		break;
	case USERINFOFRM_CMDCODE_AUTHENFAIL:	// // 用户认证失败
		theApp.m_bUserLogin = FALSE;
		if(m_pDlg)
			m_pDlg->Login_Fail();
		break;
	case LOGINFOFRM_CMDCODE_LOG:
		if(theApp.m_Log.ReadFromSocketFrame(pBuf))
		{
			nSize = theApp.m_LogMapping.WriteRecord(&theApp.m_Log);
			// 发送消息
			::SendMessage(HWND_BROADCAST,WM_NEWLOG,0,nSize);
		}
		break;
	default:
		break;
	}
	CSocket::OnReceive(nErrorCode);
}
/**
 * @brief 发送用户登录数据帧
 * @note  按照帧格式要求，发送用户登录的命令帧
 * @param LPCTSTR strUserName,
 * @param LPCTSTR strPassword,
 * @param DWORD dwServerIP
 * @return int
 */
int CSocketAdminServer::SendFrm_UserLogin(LPCTSTR strUserName,LPCTSTR strPassword,LPCTSTR strServerIP)
{
	CUserInfo	UserInfo;
	UserInfo.m_strUserName = strUserName;
	UserInfo.m_strPassword = strPassword;
	// 计算用户对象的字节大小，便于动态分配缓冲区
	int    nSize = UserInfo.GetSizeOfBytes()+16;
	BYTE*	pBuf = new BYTE[nSize];
	WORD	wTemp;
	if(!pBuf)
		return 0;
	// 帧头
	pBuf[0]= 0xEB;
	pBuf[1]= 0x90;
	// 帧长
	wTemp = (WORD)nSize;
	pBuf[2] = LOBYTE(wTemp);
	pBuf[3] = HIBYTE(wTemp);
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 1;
	// 命令码
	pBuf[9] = LOBYTE(USERINFOFRM_CMDCODE_LOGIN);
	pBuf[10] = HIBYTE(USERINFOFRM_CMDCODE_LOGIN);
	// 数据个数
	pBuf[11]= 1;
	pBuf[12]= 0;
	// 用户属性
	UserInfo.Write(pBuf+13,nSize);
	// 帧校验
	// 帧尾
	pBuf[nSize-2]= 0x14;
	pBuf[nSize-1]= 0x6F;
	SendTo(pBuf,nSize,ADMINSERVER_USERINFOPORT,strServerIP);
	delete []pBuf;
	return nSize;
}

/**
 * @brief 发送用户登录数据帧
 * @note  按照帧格式要求，发送用户登录的命令帧
 * @param LPCTSTR strUserName,
 * @param LPCTSTR strPassword,
 * @param DWORD dwServerIP
 * @return int
 */
int CSocketAdminServer::SendFrm_UserLogout(CUserInfo* pUserInfo)
{	
	if(!pUserInfo)
		return 0;
	// 计算用户对象的字节大小，便于动态分配缓冲区
	int    nSize = pUserInfo->GetSizeOfBytes()+16;
	BYTE*	pBuf = new BYTE[nSize];
	WORD	wTemp;
	if(!pBuf)
		return 0;
	// 帧头
	pBuf[0]= 0xEB;
	pBuf[1]= 0x90;
	// 帧长
	wTemp = (WORD)nSize;
	pBuf[2] = LOBYTE(wTemp);
	pBuf[3] = HIBYTE(wTemp);
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型
	pBuf[8] = 1;
	// 命令码
	pBuf[9] = LOBYTE(USERINFOFRM_CMDCODE_LOGOUT);
	pBuf[10] = HIBYTE(USERINFOFRM_CMDCODE_LOGOUT);
	// 数据个数
	pBuf[11]= 1;
	pBuf[12]= 0;
	// 用户属性
	pUserInfo->Write(pBuf+13,nSize);
	// 帧校验
	// 帧尾
	pBuf[nSize-2]= 0x14;
	pBuf[nSize-1]= 0x6F;
	SendTo(pBuf,nSize,ADMINSERVER_USERINFOPORT,theApp.m_strAdminServerIP);
	delete []pBuf;
	return nSize;
}