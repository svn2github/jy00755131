#include "StdAfx.h"
#include "OperationServer.h"
#include "FDUSocket.h"
/**
 * @brief CSocketFDU构造函数
 */
CSocketFDU::CSocketFDU(void)
: m_FDUSocket(0)
{
	memset(m_pBuf,0xAA,16);
	m_pBuf[0]=0x11;
	m_pBuf[1]=0x22;
	m_pBuf[2]=0x33;
	m_pBuf[3]=0x44;
}
/**
 * @brief CSocketFDU析构函数
 */
CSocketFDU::~CSocketFDU(void)
{
}

/**
 * @brief 创建与采集站通信的socket
 * @note  创建与采集站通信的Socket
 * @param  DWORD dwLocalIP，主机IP
 * @param  WORD  wLocalPort，主机端口
 * @param  DWORD dwMainCrossIP，主交叉站IP
 * @param  WORD wMainCrossPort，主交叉站端口
 * @return 成功则返回true；反之则返回false。
 */
bool CSocketFDU::CreateSocket(DWORD dwLocalIP,WORD wLocalPort)
{
	if(m_FDUSocket)
		return false;
	m_dwLocalIP = dwLocalIP;
	m_wLocalPort = wLocalPort;
	//m_dwMainCrossIP=dwMainCrossIP;
	//m_wMainCrossPort = wMainCrossPort;

	m_FDUSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_FDUSocket == INVALID_SOCKET) 
	{
		return false;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = m_dwLocalIP;
	service.sin_port = htons(m_wLocalPort);

	if (bind( m_FDUSocket, (SOCKADDR*) &service,sizeof(service)) == SOCKET_ERROR)
	{
		closesocket(m_FDUSocket);
		m_FDUSocket = NULL;
		return false;
	}
	// 设置接收缓冲区大小
	int RevBuf = 2048*1024;
	if (setsockopt(m_FDUSocket, SOL_SOCKET, SO_RCVBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		closesocket(m_FDUSocket);
		m_FDUSocket = NULL;
		return false;
	}
	// 设置发送缓冲区大小
	if (setsockopt(m_FDUSocket, SOL_SOCKET, SO_SNDBUF, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		closesocket(m_FDUSocket);
		m_FDUSocket = NULL;
		return false;
	}
	// 设置允许接受广播信息
	RevBuf = 1;
	if (setsockopt(m_FDUSocket, SOL_SOCKET, SO_BROADCAST, (char*)&RevBuf, sizeof(RevBuf)) == SOCKET_ERROR) 
	{
		closesocket(m_FDUSocket);
		m_FDUSocket = NULL;
		return false;
	}
	return true;

}

/**
 * @brief 关闭socket
 * @note  关闭与采集站通信的Socket
 * @param  void
 * @return 成功则返回true；反之则返回false。
 */
bool CSocketFDU::CloseSocket(void)
{
	if(m_FDUSocket)
	{
		closesocket(m_FDUSocket);
		m_FDUSocket = NULL;
	}
	return true;

}
/**
 * @brief 清空接收缓冲区
 * @note  读空与采集站通信的Socket缓冲区。采集站对所有下发的指令都自动回复，对于不需要处理的回复，
 调用该函数可清空接收缓冲区
 * @param  void
 * @return 成功则返回true；反之则返回false。
 */
bool CSocketFDU::ClearRevBufOfSocket()
{
	char  byBuf[256];
	if(m_FDUSocket==NULL)
		return false;
// 	fd_set    readfs;
// 	timeval  timeout;
// 	timeout.tv_sec = 0;
// 	timeout.tv_usec =0;		// 等待超时：300ms

	ULONG	dwRevBufSize;
	// 读空缓冲区
	while(1)
	{
// 		readfs.fd_count =1;
// 	//	FD_SET(readfs,&m_FDUSocket);
// 		readfs.fd_array[0]=m_FDUSocket;
		// 判断接收缓冲区是否有数据
		if(ioctlsocket(m_FDUSocket,FIONREAD,&dwRevBufSize))
		{
			return true;
		}
		if(dwRevBufSize==0)
			return true;	
		// 接收数据
		if(recvfrom(m_FDUSocket, byBuf, 256, 0,NULL,NULL)<=0)
			return true;		
	}
	return true;

}

/**
 * @brief 从上行数据包解析出采集站IP
 * @note  从上行数据包帧格式中解析出采集站的IP地址。采集站IP地址位于帧格式中固定的位置。
 * @param  pBuf，数据指针，指向接收数据包缓冲区首地址，对应帧格式中同步码位置
 * @param  nSize，数据缓冲区的大小 
 * @return 成功IP地址。
 */
DWORD  CSocketFDU::GetFDUIPFromUplinkPack(unsigned char* pBuf,int  nSize)
{
	DWORD  dwIP=0;
	if(nSize<19)
		return 0;
	memcpy_s(&dwIP,4,pBuf+16,4);
	return dwIP;
}
/**
 * @brief 向采集站发送数据
 * @note  基础的发送函数，主要封装IP地址和端口的转换。
 * @param  pBuf，发送数据缓冲区指针
 * @param  nSize，数据缓冲区的大小 
 * @return 返回已发送的字节数
 */
int  CSocketFDU::SendToFDU(unsigned char* pBuf,int nSize)
{
	sockaddr_in MainCrsAddr;
	MainCrsAddr.sin_family = AF_INET;
	MainCrsAddr.sin_port = htons(theApp.m_wMainCrossPort);
	MainCrsAddr.sin_addr.s_addr = theApp.m_dwMainCrossIP;
	return sendto(m_FDUSocket,(char*)pBuf,nSize,0,(sockaddr*)&MainCrsAddr,sizeof(sockaddr_in));	

}

/**
 * @brief 向采集站发送数据重传指令
 * @note  向单个采集站发送数据重传指令。
 * @param DWORD dwFDUIP，需要重传的采集站IP地址 
 * @param WORD wHostPort，接收重传数据的主机端口 
 * @param WORD wFrmIndex，需要重传的AD数据索引。采集站中有2KByte的缓冲区，每帧有74*24Byte的数据。
 * @return 返回已发送的字节数
 */
int CSocketFDU::SendRetranCmdToFDU(DWORD dwFDUIP, WORD wHostPort, WORD wFrmIndex)
{
	unsigned char*   pBuf = m_pBuf;	
	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	memcpy_s(pBuf+20,4,&dwFDUIP,sizeof(DWORD));
	memcpy_s(pBuf+24,2,&wHostPort,sizeof(WORD));
	// 数据查询命令
	pBuf[26]=0x03;
	pBuf[27]=0;
	// 包时间
	memset(pBuf+28,0,4);
	// AD数据指针
	memcpy_s(pBuf+32,2,&wFrmIndex,sizeof(WORD));
	return SendToFDU(pBuf,128);
}
/**
 * @brief 向采集站发送配置命令
 * @note  向采集站发送配置命令,作为点名命令，设置的配置命令包括：
 * @note  网络等待端口和命令,命令码为0x0A；命令格式为16bits端口+16bits命令，
				16位端口为接收上位机广播命令的端口，低位在前，高位在后；
				16位命令码默认为0x0003,顺序为03 00。
 * @note  采样参数配置命令,命令码为0x3d；
 * @note  滤波器设置命令,命令码为0x17,0x18； 
 * @param  DWORD dwFDUIP, 采集站IP地址
 * @param  WORD wBroadcastPort,采样站监听广播命令的端口
 * @param  DWORD dwConfig, 采样参数配置
 * @param  DWORD dwFilter17, 滤波器
 * @param  DWORD dwFilter18, 滤波器
 * @return 成功返回true；失败则返回false。
 */
int CSocketFDU::SendConfigCmdToFDU(DWORD dwFDUIP,WORD wBroadcastPort,DWORD dwConfig,DWORD dwFilter17,DWORD dwFilter18)
{
	BYTE* pBuf = m_pBuf;
	int		nIndex;

	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	memcpy_s(pBuf+20,4,&dwFDUIP,sizeof(DWORD));
	memcpy_s(pBuf+24,2,&m_wLocalPort,sizeof(WORD));
	pBuf[26]=0x01;
	pBuf[27]=0;		// 设置命令

	pBuf[32]=0x0A;  // 命令码： 网络等待端口和命令
	pBuf[33]=LOBYTE(m_wLocalPort);
	pBuf[34]=HIBYTE(m_wLocalPort);
	pBuf[35]=0x03;
	pBuf[36]=0x00;
	
	nIndex = 37;
	
	pBuf[nIndex++]=0x3d;  // 命令码： 采样参数配置
	memcpy_s(pBuf+nIndex,4,&dwConfig,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++]=0x17;  // 命令码： 滤波器参数设置0
	memcpy_s(pBuf+nIndex,4,&dwFilter17,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++]=0x18;  // 命令码： 滤波器参数设置1
	memcpy_s(pBuf+nIndex,4,&dwFilter18,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++]=0x00;	// 停止标识
	return SendToFDU(pBuf,128);
}
/**
 * @brief 向采集站发送0x0A命令
 * @note  向单个采集站发送网络等待端口和命令，命令码为0x0A；
 命令格式为16bits端口+16bits命令，
 16位端口为接收上位机广播命令的端口，低位在前，高位在后；
 16位命令码默认为0x0003,顺序为03 00。
 * @note 在采集排列完成采集后，发送该命令用于改变该采集排列的广播端口。避免与下一次的放炮采集冲突。
 * @param DWORD dwFDUIP，采集站IP地址 
 * @param WORD wBroadcastPort，采集站等待上位机广播命令的端口 
 * @return 返回已发送的字节数
 */
int CSocketFDU::SendBroadcastPortCmdToFDU(DWORD dwFDUIP,WORD wBroadcastPort)
{
	BYTE* pBuf = m_pBuf;
	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	memcpy_s(pBuf+20,4,&dwFDUIP,sizeof(DWORD));
	memcpy_s(pBuf+24,2,&m_wLocalPort,sizeof(WORD));
	pBuf[26]=0x01;
	pBuf[27]=0;		// 设置命令
	
	pBuf[32]=0x0A;  // 命令码： 网络等待端口和命令
	pBuf[33]=LOBYTE(wBroadcastPort);
	pBuf[34]=HIBYTE(wBroadcastPort);
	pBuf[35]=0x03;
	pBuf[36]=0x00;

	pBuf[37]=0x00;	// 停止标识
	return SendToFDU(pBuf,128);
}
/**
 * @brief 向采集站发送点名指令
 * @note  向单个采集站发送点名指令，点名指令发送的命令码包括四个:0x07,0x08,0x09,0x0A。
 * @param DWORD dwFDUIP，采集站IP地址 
 * @param WORD wMinPort，数据返回端口的最小值
 * @param WORD wMaxPort，数据返回端口的最大值
 * @return 返回已发送的字节数

int  CSocketFDU::SendCallCmdToFDU(DWORD dwFDUIP,WORD wMinPort,WORD wMaxPort)
{
	unsigned char*   pBuf = m_pBuf;	
	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	memcpy_s(pBuf+20,4,&dwFDUIP,sizeof(DWORD));
	memcpy_s(pBuf+24,2,&m_wLocalPort,sizeof(WORD));
	pBuf[26]=0x01;
	pBuf[27]=0;	// 设置命令

	pBuf[32]=0x07; // 命令码:自动数据返回地址
	memcpy_s(pBuf+33,4,&m_dwLocalIP,sizeof(DWORD));

	pBuf[37]=0x08; // 命令码
	pBuf[38]=0x00; // 自动数据返回端口(SOCKET_REV8PORT: 0xB000)
	pBuf[39]=0xB0;
	pBuf[40]=0x03;//  命令2Byte,( 命令最高位为1时标识采用循环端口,低位用0x03表示采样数据命令)
	pBuf[41]=0x80;

	pBuf[42]=0x09;  // 命令码： 端口递增下限和上线
 	pBuf[43]=LOBYTE(wMinPort);
	pBuf[44]=HIBYTE(wMinPort);
	pBuf[45]=LOBYTE(wMaxPort);
	pBuf[46]=HIBYTE(wMaxPort);

	pBuf[47]=0x0A;  // 命令码： 网络等待端口和命令
	pBuf[48]=LOBYTE(m_wLocalPort);
	pBuf[49]=HIBYTE(m_wLocalPort);
	pBuf[50]=0x03;
	pBuf[51]=0x00;

	pBuf[52] = 0;

	return SendToFDU(pBuf,128);

} */

/**
 * @brief 发送查询系统时间命令
 * @note  向指定的采集站（IP地址）发送查询测网系统时间命令。
 * @param  DWORD dwFDUIP，采集站IP地址 
 * @return 返回已发送的字节数
 */
int CSocketFDU::SendQueryTimeCmd(DWORD dwFDUIP)
{
	unsigned char*   pBuf = m_pBuf;		
	// 源地址
	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	// 目的地址
	memcpy_s(pBuf+20,4,&dwFDUIP,sizeof(DWORD));
	// 源地址端口
	pBuf[24] = LOBYTE(OPERATION_LOCALTIMEPORT);
	pBuf[25] = HIBYTE(OPERATION_LOCALTIMEPORT);
	//memcpy_s(pBuf+24,2,&m_wLocalPort,sizeof(WORD));
	// 查询命令
	pBuf[26]=0x02;
	pBuf[27]=0;
	// 命令码0x04表示本地系统时间
	pBuf[32] = 0x04;	
	memset(pBuf+33,0,4);
	pBuf[37] = 0x3D;	
	memset(pBuf+38,0,5);
	return SendToFDU(pBuf,128);
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值： 0: 没有接收到数据
	           >0 : 当前系统时间
    
    功    能： 接收系统时间，等待500ms无数据，返回0

    修改历史：

******************************************************************************
DWORD CSocketFDU::RevSystemTime(void)
{	
	if(m_FDUSocket==NULL)
		return false;

	char  byBuf[256];
	fd_set    readfs;
	timeval  timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec =500;		// 等待超时
	DWORD    dwTime;
	
	readfs.fd_count =1;
	FD_SET(m_FDUSocket,&readfs);
	//	readfs.fd_array[0]=m_FDUSocket;
	if(select(0,&readfs,NULL,NULL,&timeout)<=0)			
	{
		// 没有收到数据
		return 0;
	}
	if(FD_ISSET(m_FDUSocket,&readfs))
	{
		if(recvfrom(m_FDUSocket, byBuf, 256, 0,NULL,NULL)<=0)
			return 0;
		// 判断是否为查询码，且命令码为0x04
		if(byBuf[26]!=0x02 || byBuf[27]!=0x00 || byBuf[28]!=0x04 )
			return 0;
		// 解析本地系统时间
		memcpy_s(&dwTime,4,byBuf+29,4);
		return dwTime;
	}
	return 0;
}*/

/**
 * @brief 向采集站发送停止采样指令TE
 * @note  用广播地址发送停止数据采样，此命令是向广播地址发送，采集站端口为0x0A指令所指定的端口；
 采集指令为TB、TE、0x17、0x18命令，启动采集站的数据采样。
 一般在采集排列准备开始放炮和启动数据采集之前，先停止噪声采集。
 * @param DWORD dwTEH，TE时刻高位
 * @param DWORD dwTEL，TE时刻低位
 * @return 返回已发送的字节数
 */

int CSocketFDU::SendTEBroadcastCmdToFDU(DWORD dwTEH, DWORD dwTEL)
{
	BYTE* pBuf = m_pBuf;
	DWORD nIndex;
	// 源地址：本机IP
	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	// 采集站广播地址0XFFFFFFFF
	//memcpy_s(pBuf+20,4,&dwIP,sizeof(DWORD));
	memset(pBuf+20,0xFF,sizeof(DWORD));	
	// 源地址的端口
	memcpy_s(pBuf+24,2,&m_wLocalPort,sizeof(WORD));
	pBuf[26]=0x01;
	pBuf[27]=0;	// 设置命令
	
	nIndex = 32;
	pBuf[nIndex++]=0x3E; // 命令码:广播端口匹配,数据区为16位端口+16位命令 
	pBuf[nIndex++]=LOBYTE(m_wLocalPort);
	pBuf[nIndex++]=HIBYTE(m_wLocalPort);
	pBuf[nIndex++]=0x03;
	pBuf[nIndex++]=00;
	
	pBuf[nIndex++]=0x10;  // 命令码：TE_H				
	memcpy_s(pBuf+nIndex,4,&dwTEH,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++]=0x11;  // 命令码：TE_L				
	memcpy_s(pBuf+nIndex,4,&dwTEL,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++] = 0;
	return SendToFDU(pBuf,128);
}

/**
 * @brief 向采集站发送采样指令
 * @note  向采集站发送采样指令，此命令是向广播地址发送，采集站端口为0x0A指令所指定的端口；
 采集命令包括设置循环广播端口命令（0x07/0x08/0x09）、采集指令TB、TE（0x0C、0x0D、0x10、0x11)命令。
 * @param DWORD dwIP，IP地址
 * @param WORD wMinPort，最小端口
 * @param WORD wMaxPort，最大端口
 * @param DWORD dwTBH，TB时刻高位
 * @param DWORD dwTBL，TB时刻低位
 * @param DWORD dwTEH，TE时刻高位
 * @param DWORD dwTEL，TE时刻低位
 * @return 返回已发送的字节数
 */
int CSocketFDU::SendSampParamCmdToFDU(DWORD dwIP,WORD wMinPort,WORD wMaxPort,DWORD dwTBH,DWORD dwTBL,
									  DWORD dwTEH,DWORD dwTEL)
{
	BYTE* pBuf = m_pBuf;
	DWORD nIndex;
	// 源地址：本机IP
	memcpy_s(pBuf+16,4,&m_dwLocalIP,sizeof(DWORD));
	// 采集站广播地址0XFFFFFFFF
	memcpy_s(pBuf+20,4,&dwIP,sizeof(DWORD));
	// memset(pBuf+20,0xFF,sizeof(DWORD));
	// 源地址的端口
	memcpy_s(pBuf+24,2,&m_wLocalPort,sizeof(WORD));
	pBuf[26]=0x01;
	pBuf[27]=0;	// 设置命令

	nIndex = 32;
	pBuf[nIndex++]=0x3E; // 命令码:广播端口匹配,数据区为16位端口+16位命令 
	pBuf[nIndex++]=LOBYTE(m_wLocalPort);
	pBuf[nIndex++]=HIBYTE(m_wLocalPort);
	pBuf[nIndex++]=0x03;
	pBuf[nIndex++]=00;

	pBuf[nIndex++]=0x07; // 命令码:自动数据返回地址
	memcpy_s(pBuf+nIndex,4,&m_dwLocalIP,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++]=0x08; // pBuf[42]命令码
	pBuf[nIndex++]=LOBYTE(wMinPort); // 自动数据返回端口(SOCKET_REV8PORT: 0xB000)
	pBuf[nIndex++]=HIBYTE(wMinPort);
	pBuf[nIndex++]=0x03;//  命令2Byte,( 命令最高位为1时标识采用循环端口,低位用0x03表示采样数据命令)
	pBuf[nIndex++]=0x80;

	pBuf[nIndex++]=0x09;  // pBuf[47] 命令码： 端口递增下限和下限
	pBuf[nIndex++]=LOBYTE(wMinPort);
	pBuf[nIndex++]=HIBYTE(wMinPort);
	pBuf[nIndex++]=LOBYTE(wMaxPort);
	pBuf[nIndex++]=HIBYTE(wMaxPort);

	pBuf[nIndex++]=0x0C; // 命令码: TB_H				
	memcpy_s(pBuf+nIndex,4,&dwTBH,sizeof(DWORD));
	nIndex+=4;

	pBuf[nIndex++]=0x0D; // 命令码: TB_L				
	memcpy_s(pBuf+nIndex,4,&dwTBL,sizeof(DWORD));
	nIndex+=4;
	
	// pBuf[nIndex-2] = 0x80;// 测试代码
	// pBuf[nIndex-1] = 0x92;// 测试代码

	pBuf[nIndex++]=0x10;  // 命令码：TE_H				
	memcpy_s(pBuf+nIndex,4,&dwTEH,sizeof(DWORD));
	nIndex+=4;
	
	pBuf[nIndex++]=0x11;  // 命令码：TE_L				
	memcpy_s(pBuf+nIndex,4,&dwTEL,sizeof(DWORD));
	nIndex+=4;
	
	pBuf[nIndex++] = 0;
	return SendToFDU(pBuf,128);

}