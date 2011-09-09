// SocketUserInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "AdminServer.h"
#include "SocketUserInfo.h"
#include "..\common\UserInfo.h"
#include "..\common\AdminParseXML.h"
// CSocketUserInfo

CSocketUserInfo::CSocketUserInfo()
{
}

CSocketUserInfo::~CSocketUserInfo()
{
}


// CSocketUserInfo 成员函数
/**
 * @brief 创建Socket
 * @note  创建Socket端口
 * @param  UINT wPort,socket端口
 * @param  LPCTSTR strHostIP，本机IP地址
 * @return 创建成功则返回true，失败则返回false
 */
bool CSocketUserInfo::CreateSocket(UINT wPort, LPCTSTR strHostIP)
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
 * @note  接收并处理来自客户端的用户数据。
 帧格式为：
     帧头（2byte）+帧长（2byte）+帧计数（4byte）+帧类型（1byte）+命令码/状态码（2byte）+用户个数（2byte）+用户数据（字节数不定）+校验位(1Byte)+帧尾(2Byte)
 * @param  int nErrorCode，socket通信错误码
 * @return void
 */
void CSocketUserInfo::OnReceive(int nErrorCode)
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
			TRACE0("CSocketUserInfo's ReceiveFrom error ！");
			
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
		TRACE2("CSocketUserInfo's ReceiveFrom！Frame head(%d,%d)is wrong！",pBuf[0],pBuf[1]);		
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
	case USERINFOFRM_CMDCODE_LOGIN:	// 请求放炮服务帧
		AnalysisCmd_UserLogin(pBuf,nReadSize,&sockClient);
		break;
	case USERINFOFRM_CMDCODE_LOGOUT:	// 用户注销退出
		AnalysisCmd_UserLogout(pBuf,nReadSize);
		break;
	case USERINFOFRM_CMDCODE_QUERY:		// 查询所有用户
		AnalysisCmd_ReqUserInfo(&sockClient);
		break;
	case  USERINFOFRM_CMDCODE_ALLUSERS:	// 客户端发送修改所有用户信息
		AnalysisCmd_AllUserInfo(pBuf,nReadSize);
		break;
	default:
		break;
	}
	CSocket::OnReceive(nErrorCode);
}
/**
 * @brief 解析用户登录数据帧
 * @note  按照帧格式要求，解析用户登录的命令帧，并依据用户名及密码进行校验，将结果返回给客户端
 * @param  unsigned char* pBuf,数据帧缓冲区
 * @param int nSize,数据帧大小
 * @param sockaddr_in* psockaddr,客户端Socket信息
 * @return 成功，返回true，失败返回false
 */
bool  CSocketUserInfo::AnalysisCmd_UserLogin(unsigned char* pBuf,int nSize,sockaddr_in* psockaddr)
{
	if(!psockaddr)
		return false;
	CString		strName,strPassword;
	strName = LPCTSTR(pBuf+13);
	int			nPos;
	nPos = 13+strName.GetLength()*sizeof(TCHAR)+2;
	strPassword = LPCTSTR(pBuf+nPos);

	CUserInfo*	pUserInfo=NULL;
	pUserInfo = theApp.m_AllUserInfo.FindUserByNameAndPassWord(strName,strPassword);
	if(!pUserInfo)
	{
		// 没有找到用户
		pBuf[9] = LOBYTE(USERINFOFRM_CMDCODE_AUTHENFAIL);
		pBuf[10] = HIBYTE(USERINFOFRM_CMDCODE_AUTHENFAIL);
		pBuf[11] = 1;
		pBuf[12] = 0;
		SendTo(pBuf,nSize,(SOCKADDR*)psockaddr,sizeof(sockaddr_in));
		return false;
	}
	// 找到用户后，更新 
	pUserInfo->m_dwSessionID = (++theApp.m_AllUserInfo.m_dwSession);
	pUserInfo->m_nState = USERINFO_STATE_ALINE;
	pUserInfo->m_dwClientIP = psockaddr->sin_addr.s_addr;
	pUserInfo->m_nApplication = 0;
	GetLocalTime(&pUserInfo->m_tmLastUsed);
	// 将用户信息重新发送到客户端，通知客户端认证通过
	pUserInfo->Write(pBuf+13,nSize-13);
	pBuf[9] = LOBYTE(USERINFOFRM_CMDCODE_AUTHENOK);
	pBuf[10] = HIBYTE(USERINFOFRM_CMDCODE_AUTHENOK);
	pBuf[11] = 1;
	pBuf[12] = 0;
	SendTo(pBuf,nSize,(SOCKADDR*)psockaddr,sizeof(sockaddr_in));
	return true;
}

/**
 * @brief 解析用户注销数据帧
 * @note  按照帧格式要求，解析用户登录的命令帧，并依据用户名及密码进行校验，将结果返回给客户端
 * @param  unsigned char* pBuf,数据帧缓冲区
 * @param int nSize,数据帧大小
 * @param sockaddr_in* psockaddr,客户端Socket信息
 * @return 成功，返回true，失败返回false
 */
bool  CSocketUserInfo::AnalysisCmd_UserLogout(unsigned char* pBuf,int nSize)
{	
	CString		strName,strPassword;
	strName = LPCTSTR(pBuf+13);
	int			nPos;
	nPos = 13+strName.GetLength()*sizeof(TCHAR)+2;
	strPassword = LPCTSTR(pBuf+nPos);


	CUserInfo*	pUserInfo=NULL;
	pUserInfo = theApp.m_AllUserInfo.FindUserByNameAndPassWord(strName,strPassword);
	if(!pUserInfo)
	{
		// 没有找到用户		
		return false;
	}
	// 找到用户后，更新 
	pUserInfo->m_dwSessionID = 0;
	pUserInfo->m_nState = USERINFO_STATE_OFFLINE;
	pUserInfo->m_dwClientIP = 0;
	pUserInfo->m_nApplication = 0;
	// GetLocalTime(&pUserInfo->m_tmLastUsed);
	return true;
}
/**
 * @brief 发送全部用户信息
 * @note  按照帧格式要求，发送用户信息
 * @param sockaddr_in* psockaddr,客户端Socket信息
 * @return 成功，返回true，失败返回false
 */
bool  CSocketUserInfo::AnalysisCmd_ReqUserInfo(sockaddr_in* psockaddr)
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
	SendTo(pBuf,wFrmLen,(SOCKADDR*)psockaddr,sizeof(sockaddr_in));
	return true;
}

/**
 * @brief 解析所有用户信息
 * @note  按照帧格式要求，解析所有用户信息，保存在theApp.m_AllUserInfo变量中
 * @param  unsigned char* pBuf,数据帧缓冲区
 * @param int nSize,数据帧大小
 * @return 解析成功，返回true，失败返回false
 */
bool CSocketUserInfo::AnalysisCmd_AllUserInfo(BYTE* pBuf,int nSize)
{
	int nUserCount;
	int nlen,nPos=0;
	// 得到用户记录的总数
	nUserCount = MAKEWORD(pBuf[11],pBuf[12]);
	if(!nUserCount)
		return true;
	// 删除原来的对象
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
	// 保存到XML文件中
	CAdminParseXML		ParseXML;
	ParseXML.WriteAllUserInfo(theApp.m_strLocalXMLFile,&theApp.m_AllUserInfo);
	return true;	
}