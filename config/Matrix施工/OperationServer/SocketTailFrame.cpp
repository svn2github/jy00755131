// SocketTailFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "OperationServer.h"
#include "SocketTailFrame.h"


// CSocketTailFrame
/**
 * @brief CSocketTailFrame构造函数
 */
CSocketTailFrame::CSocketTailFrame()
{
}
/**
 * @brief CSocketTailFrame析构函数
 */
CSocketTailFrame::~CSocketTailFrame()
{
}
// CSocketTailFrame 成员函数
/**
 * @brief 创建接收尾包的Socket端口
 * @note  创建接收尾包的Socket端口，该端口接收尾包并从尾包帧中解析出测网当前的系统时间。
 * @param  UINT wPort, Socket端口；
 * @param  LPCTSTR strHostIP,主机IP地址
 * @return 成功则返回true，失败返回false
 */
bool CSocketTailFrame::CreateTailFrameSocket(UINT wPort, LPCTSTR strHostIP)
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

/**
 * @brief 尾包接收处理函数
 * @note  接收尾包并从尾包帧中解析出测网当前的系统时间。
 * @param  int nErrorCode，socket错误码
 * @return void
 */
void CSocketTailFrame::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	unsigned char pBuf[256];
	DWORD    dwTemp;
	int nReadSize = Receive( pBuf, 256);
	if(nReadSize>0)
	{
		// 解析数据包
		// memcpy_s(&dwTemp,4,pBuf+16,4);
		// 主交叉站
		//if(dwTemp==MAINCROSS_LOGICIP)
		{
			// 解析系统时间
			for(int i=0;i<8;i++)
			{		
				switch(pBuf[28+i*5])	// 命令字
				{
				case 0x00:
					i=10;  // 命令帧结束,退出
					break;
				case 0x04:// 得到系统时间
					memcpy_s(&dwTemp, 4,pBuf+29 + i* 5, 4);					
					theApp.SetFDULocalTime(dwTemp);
					TRACE1("0 FDU Time:%d\n",dwTemp);
					// return;
					i=10;	// 找到命令后退出
					break;
				default:
					break;
				}
			}
		}

	}
	CSocket::OnReceive(nErrorCode);
}
