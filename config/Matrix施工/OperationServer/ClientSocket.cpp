#include "StdAfx.h"
#include "OperationServerDlg.h"
#include "ClientSocket.h"
#include "..//common/OperaStruct.h"
/**
 * @brief CSocketShotClient构造函数
 */
CSocketShotClient::CSocketShotClient(void)
: m_pDlg(NULL)
{
}
/**
 * @brief CSocketShotClient析构函数
 */
CSocketShotClient::~CSocketShotClient(void)
{
}
bool CSocketShotClient::CreateClientSocket(UINT wPort, LPCTSTR strHostIP,COperationServerDlg* pDlg)
{
	
	this->m_pDlg = pDlg;
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
 * @note  接收并处理来自客户端的数据。
 * @param  int nErrorCode，socket通信错误码
 * @return void
 */
void CSocketShotClient::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类	
	if(m_pDlg)
	{
		m_pDlg->AnalysisProtocolOfClientSock();
	}
	CSocket::OnReceive(nErrorCode);
}

/******************************************************************************
    函    数：SendDataToClient
    
    输    入：sockaddr_in* lpSockAddr      -- 客户端socket地址
	          unsigned char* lpBuf         -- 数据缓冲区
			  int nSize                    -- 数据缓冲区需要发送的字节数
    
    输    出：
  
    返 回 值： 发送的字节数
    
    功    能： 向客户端发送数据

    修改历史：

******************************************************************************
int CSocketShotClient::SendDataToClient(sockaddr_in* lpSockAddr,unsigned char* lpBuf ,int nSize)
{
	return  SendTo(lpBuf,nSize,(SOCKADDR*)lpSockAddr,sizeof(sockaddr_in));
}*/
/**
 * @brief 发送数据
 * @note  向客户端发送数据；该函数主要封装IP地址和网络端口的转换，被其他命令或者状态发送函数调用。
 * @param  DWORD dwSockAddr，接收端网络地址
 * @param  WORD wClientPort，接收端端口
 * @param  unsigned char* lpBuf，需要发送的数据缓冲区
 * @param  int nSize，缓冲区数据大小
 * @return 已发送的字节数
 */
int CSocketShotClient::SendDataToClient(DWORD dwSockAddr,WORD wClientPort,unsigned char* lpBuf ,int nSize)
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = dwSockAddr;
	service.sin_port = htons(wClientPort);
	return  SendTo(lpBuf,nSize,(SOCKADDR*)&service,sizeof(sockaddr_in));
}


/******************************************************************************
    函    数：SendAckOfRequest
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：施工客户端在放炮前发送命令，请求放炮服务。服务器端收到请求服务后，
	          向客户端返回当前放炮线程组状态信息。

			  数据帧格式：
			  帧头（2Byte）＋帧长度（2Byte）  ＋帧计数（4Byte）＋帧类型（1Byte）			   
			  ＋命令(2Byte)＋数据个数（2Byte）＋数据区（变长） ＋校验位（1Byte）＋帧尾（2Byte）

    修改历史：

******************************************************************************
int CSocketShotClient::SendAckOfRequest(sockaddr_in* lpSockAddr,DWORD dwFrmIndex,DWORD dwShotNb,DWORD dwSourceNb, int nThreadIndex)
{
	unsigned char   pBuf[21];

	pBuf[0] = 0xEB;
	pBuf[1] = 0x90;

	// 帧长
	pBuf[2] = 28;
	pBuf[3] = 0;
	// 帧计数
	memcpy_s(pBuf+4,4,&dwFrmIndex,4);
	// 帧类型
	pBuf[8] = 0xFF;
	// 命令码
	pBuf[9]  = LOBYTE(OPERATION_CMD_REQUEST);
	pBuf[10] = HIBYTE(OPERATION_CMD_REQUEST);
	// 数据个数
	pBuf[11] =12;
	pBuf[12] =0;
	// 空闲线程索引号
	memcpy_s(pBuf+13,4,&dwShotNb,4);
	memcpy_s(pBuf+17,4,&dwSourceNb,4);
	memcpy_s(pBuf+21,4,&nThreadIndex,4);
	// 帧校验
	pBuf[25]=0;
	// 帧尾
	pBuf[26] = 0x14;
	pBuf[27] = 0x6F;

	return  SendTo(pBuf,28,(SOCKADDR*)lpSockAddr,sizeof(sockaddr_in));
}*/