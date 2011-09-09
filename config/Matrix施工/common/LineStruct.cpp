#include "stdafx.h"
#include "LineStruct.h"

CIPList::CIPList()
{

}

CIPList::~CIPList()
{

}

// 得到一个采集站IP地址
unsigned int CIPList::GetCollectIP(unsigned int uiLineNb, unsigned int uiPointNb, unsigned int uiChannelNb)
{
	//测道号小于；测点号小于；测线号小于
	return uiChannelNb + uiPointNb * 10 + uiLineNb * 1000000;
}

// 得到一个爆炸机IP地址
unsigned int CIPList::GetBlastMachineIP(unsigned int uiNb)
{
	//20亿+ 爆炸机号
	return uiNb + 2000000000;
}

// 得到一个辅助道IP地址
unsigned int CIPList::GetAuxIP(unsigned int uiNb)
{
	//20亿+ 100 + 辅助道号
	return uiNb + 2000000000 + 100;
}
// 得到当前主机的一个IP地址
DWORD CIPList::GetLocalIP()
{
    char szhn[256]; 
	int nStatus = gethostname(szhn, sizeof(szhn));
    if (nStatus == SOCKET_ERROR ) 
	{
		return false; 
	}
	
	struct addrinfo aiHints;
	struct addrinfo *aiList = NULL;

	memset(&aiHints, 0, sizeof(aiHints));
	aiHints.ai_family = AF_INET;
	if ((getaddrinfo(szhn, NULL, &aiHints, &aiList)) != 0) 
	{
		return 0;
	}

	sockaddr_in *pAddr = (sockaddr_in*)(aiList->ai_addr);
	do 
	{
		if(pAddr->sin_addr.S_un.S_addr == 0x0100007f)
		{
			aiList = aiList->ai_next;
			pAddr = (sockaddr_in*)(aiList->ai_addr);
		}
		else
		{
			return pAddr->sin_addr.S_un.S_addr;
		}
	} while (pAddr!=NULL);
	
	return  0;

}