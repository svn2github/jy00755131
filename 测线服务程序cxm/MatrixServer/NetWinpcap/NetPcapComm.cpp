#include "StdAfx.h"
#include "NetPcapComm.h"
#include "NetFrameHeader.h"
#pragma comment(lib, "wpcap")
#pragma comment(lib,"winmm")  //lib file
//#pragma comment(lib,"MmTimer")  //lib file, WinCE
CNetPcapComm::CNetPcapComm(void)
{
	m_ptrPcap = NULL;
	m_uiNetCardId = 0;
	m_uiPcapPacketMaxSize = 0;
	m_uiPcapTimeOut = 0;
	m_uiPcapBufSize = 0;
	memset(m_cPcapfilter, 0, sizeof(m_cPcapfilter));
	m_oDownStreamRcvSndPortMap.clear();
	m_oUpStreamRcvSndPortMap.clear();
	m_usPcapDownStreamSrcPort = 0;
	m_usPcapUpStreamSrcPort = 0;
	m_uiDownStreamSndBufSize = 0;
	m_uiUpStreamSndBufSize = 0;
	m_uiHighStreamIP = 0;
	m_uiLowStreamIP = 0;
//	m_uiNetIP = 0;
// 	memset(m_ucHighMacAddr, 0, sizeof(m_ucHighMacAddr));
// 	memset(m_ucLowMacAddr, 0, sizeof(m_ucLowMacAddr));
// 	memset(m_ucNetMacAddr, 0, sizeof(m_ucNetMacAddr));
	m_uiPcapQueueSize = 0;
	m_pFrameDataArray = NULL;
	m_uiFreeCount = 0;
	m_olsFrameDataFree.clear();
	m_olsFrameDataUpStream.clear();
	m_olsFrameDataDownStream.clear();
}

CNetPcapComm::~CNetPcapComm(void)
{
}
/** 创建Socket*/
SOCKET CNetPcapComm::CreateSocket(unsigned int uiSrcPort, int iSndBufferSize)
{
	SOCKET oSocket = INVALID_SOCKET;
	sockaddr_in oAddr;
	DWORD bytes_returned = 0;
	BOOL new_behavior = FALSE;
	unsigned long arg = 1;

	// 填充套接字地址结构
	oSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	oAddr.sin_family = AF_INET;
	oAddr.sin_port = htons(uiSrcPort);
	oAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//bind socket
	bind(oSocket, (SOCKADDR*)&oAddr, sizeof(oAddr));
	//set send buff
	setsockopt(oSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&iSndBufferSize, 
		sizeof(int));
	//set io control
	ioctlsocket(oSocket, FIONBIO, &arg);
	// disable  new behavior using IOCTL: SIO_UDP_CONNRESET
	WSAIoctl(oSocket, SIO_UDP_CONNRESET,	&new_behavior, sizeof(new_behavior), 
		NULL, 0, &bytes_returned,	NULL, NULL);
	return oSocket;
}
void CALLBACK TimerHandler(UINT id, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNetPcapComm* pThis = (CNetPcapComm*)dwUser;
	int iFrameNum = 0;
	CNetPcapComm::FrameDataStruct* pFrameData = NULL;
	EnterCriticalSection(&pThis->m_oSec);
	iFrameNum = pThis->m_olsFrameDataDownStream.size();
	if (iFrameNum > 4)
	{
		iFrameNum = 4;
	}
	for (int i=0; i<iFrameNum; i++)
	{
		pFrameData = *pThis->m_olsFrameDataDownStream.begin();
		// 发送该帧
		if (pFrameData->m_uiLength == pThis->SocketSndFrameData(pFrameData))
		{
			_InterlockedIncrement(&pThis->m_lDownStreamNetSndFrameNum);
		}
		else
		{
			OutputDebugString(_T("下行帧发送失败！"));
		}
		pThis->m_olsFrameDataDownStream.pop_front();
		pThis->AddFreeFrameData(pFrameData);
	}
	LeaveCriticalSection(&pThis->m_oSec);
	
}
void CNetPcapComm::OnInit()
{
	pcap_if_t *alldevs;
	pcap_if_t *dev;
	char errbuf[PCAP_ERRBUF_SIZE+1];
	unsigned int netmask;
	struct bpf_program fcode;
	m_SocketDownStream = CreateSocket(m_usPcapDownStreamSrcPort, m_uiDownStreamSndBufSize);
	m_SocketUpStream = CreateSocket(m_usPcapUpStreamSrcPort, m_uiUpStreamSndBufSize);

	/* Retrieve the interfaces list */
	pcap_findalldevs(&alldevs, errbuf);
	dev = alldevs;
	for (unsigned i = 0; i < m_uiNetCardId; i++)
	{
		dev = dev->next;
	}
	m_ptrPcap  = pcap_open_live(dev->name, m_uiPcapPacketMaxSize, 
		PCAP_OPENFLAG_PROMISCUOUS, m_uiPcapTimeOut, errbuf);
	pcap_setbuff(m_ptrPcap, m_uiPcapBufSize);

	if(dev->addresses != NULL)
	{
		netmask=((struct sockaddr_in *)(dev->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else
	{
		netmask=0xffffff; 
	}
	strcpy_s(m_cPcapfilter, strlen("udp") + 1, "udp");
	//compile the filter
	pcap_compile(m_ptrPcap, &fcode, m_cPcapfilter, 1, netmask);
	pcap_setfilter(m_ptrPcap, &fcode);
	pcap_freealldevs(alldevs);
	InitializeCriticalSection(&m_oSec);
	if (m_uiPcapQueueSize == 0)
	{
		return;
	}
	m_pFrameDataArray = new m_oFrameData[m_uiPcapQueueSize];
	m_uiFreeCount = m_uiPcapQueueSize;
	for (unsigned int i=0; i<m_uiPcapQueueSize; i++)
	{
		OnResetFrameData(&m_pFrameDataArray[i]);
		m_olsFrameDataFree.push_back(&m_pFrameDataArray[i]);
	}
	_InterlockedExchange(&m_lDownStreamNetRevFrameNum, 0);
	_InterlockedExchange(&m_lDownStreamNetSndFrameNum, 0);
	_InterlockedExchange(&m_lUpStreamNetRevFrameNum, 0);
	_InterlockedExchange(&m_lUpStreamNetSndFrameNum, 0);

	m_uiTimeRes = 1;
	// 设置定时器分辨率，1ms
	timeBeginPeriod(m_uiTimeRes);
	// 开启定时器
	// 该函数定时精度为ms级，利用该函数可以实现周期性的函数调用。
	// 该函数设置一个定时回调事件，此事件可以是一个一次性事件或周期性事件。事件一旦被激活，便调用指定的回调函数，
	// 成功后返回事件的标识符代码，否则返回NULL
	m_TimerID = timeSetEvent(
		1,	// 以毫秒指定事件的周期
		m_uiTimeRes,	// 以毫秒指定延时的精度，数值越小定时器事件分辨率越高，缺省值为1ms。
		TimerHandler,	// 指向一个回调函数
		(DWORD)this,	// 存放用户提供的回调数据
		TIME_PERIODIC);	// 指定定时器事件类型：
	// TIME_ONESHOT：uDelay毫秒后只产生一次事件
	// TIME_PERIODIC ：每隔uDelay毫秒周期性地产生事件。
}
/** 解析接收发送端口*/
void CNetPcapComm::PhraseRcvSndPort(CString str, hash_map<unsigned short, unsigned short>* pMap)
{
	int iPos = 0;
	CString strTemp = _T("");
	iPos = str.Find('_');
	if (iPos == -1)
	{
		return;
	}
	strTemp = str.Left(iPos);
	str = str.Right(str.GetLength() - (iPos + 1));
	AddPortToMap(htons(_ttoi(strTemp)), htons(_ttoi(str)), pMap);
}
/** 判断接收端口是否已加入索引表*/
BOOL CNetPcapComm::IfRcvPortExistInMap(unsigned short usRcvPort, 
	hash_map<unsigned short, unsigned short>* pMap)
{
	ASSERT(pMap != NULL);
	hash_map<unsigned short, unsigned short>::iterator iter;
	iter = pMap->find(usRcvPort);
	if (iter != pMap->end())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/** 增加对象到索引表*/
void CNetPcapComm::AddPortToMap(unsigned short usRcvPort, unsigned short usSndPort, 
	hash_map<unsigned short, unsigned short>* pMap)
{
	ASSERT(pMap != NULL);
	if (FALSE == IfRcvPortExistInMap(usRcvPort, pMap))
	{
		pMap->insert(hash_map<unsigned short, unsigned short>::value_type (usRcvPort, usSndPort));
	}
}
/** 根据接收端口，得到发送*/
unsigned int CNetPcapComm::GetSndPortFromMap(unsigned short usRcvPort, 
	hash_map<unsigned short, unsigned short>* pMap)
{
	ASSERT(pMap != NULL);
	hash_map<unsigned short, unsigned short>::iterator iter;
	iter = pMap->find(usRcvPort);
	return iter->second;
}
/** 重置存储的数据帧*/
void CNetPcapComm::OnResetFrameData(m_oFrameData* pFrameData)
{
	ASSERT(pFrameData != NULL);
	pFrameData->m_bDownStream = false;
	memset(pFrameData->m_ucData, 0, FrameDataSize * sizeof(char));
	pFrameData->m_uiLength = 0;
	pFrameData->m_usDstPort = 0;
}
/** 得到一个空闲的存储数据帧*/
CNetPcapComm::m_oFrameData* CNetPcapComm::GetFreeFrameData()
{
	m_oFrameData* pFrameData = NULL;
	list <m_oFrameData*>::iterator iter;
	if(m_uiFreeCount > 0)
	{
		iter = m_olsFrameDataFree.begin();
		pFrameData = *iter;
		m_olsFrameDataFree.pop_front();
		m_uiFreeCount--;
	}
	else
	{
		AfxMessageBox(_T("PcapQueueSize is not big enough!"));
	}
	return pFrameData;
}
/** 将存储数据帧加入空闲列表*/
void CNetPcapComm::AddFreeFrameData(m_oFrameData* pFrameData)
{
	OnResetFrameData(pFrameData);
	m_olsFrameDataFree.push_back(pFrameData);
	m_uiFreeCount++;
}
// unsigned short CNetPcapComm::check_sum (unsigned short * addr, int len)
// {
// 	int nleft = len;
// 	unsigned sum = 0;
// 	unsigned short *w = addr;
// 	unsigned short answer = 0;
// 
// 	/*
// 	* Our algorithm is simple, using a 32 bit accumulator (sum), we add
// 	* sequential 16 bit words to it, and at the end, fold back all the
// 	* carry bits from the top 16 bits into the lower 16 bits.
// 	*/
// 	while (nleft > 1) {
// 		sum += *w++;
// 		nleft -= 2;
// 	}
// 	/* mop up an odd byte, if necessary */
// 	if (nleft == 1) {
// 		* (unsigned char *) (&answer) = * (unsigned char *) w;
// 		sum += answer;
// 	}
// 
// 	/* add back carry outs from top 16 bits to low 16 bits */
// 	sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
// 	sum += (sum >> 16);     /* add carry */
// 	answer = ~sum;     /* truncate to 16 bits */
// 	return (answer);
// }
/** Pcap发送帧，缺点：发送速度慢！*/
// bool CNetPcapComm::PcapSndFrameData(m_oFrameData* pFrameData)
// {
// 	unsigned char tmp_buf[FrameDataSize] = {0};
// 	IP_Header ip = {0};
// 	Ethernet_Header eh = {0};
// 	UDP_Header dh = {0};
// 	int iPos = 0;
// 
// 	eh.type = 0x08;
// 	eh.source[0] = m_ucNetMacAddr[0];
// 	eh.source[1] = m_ucNetMacAddr[1];
// 	eh.source[2] = m_ucNetMacAddr[2];
// 	eh.source[3] = m_ucNetMacAddr[3];
// 	eh.source[4] = m_ucNetMacAddr[4];
// 	eh.source[5] = m_ucNetMacAddr[5];
// 	if (pFrameData->m_bDownStream == true)
// 	{
// 		eh.dest[0] = m_ucLowMacAddr[0];
// 		eh.dest[1] = m_ucLowMacAddr[1];
// 		eh.dest[2] = m_ucLowMacAddr[2];
// 		eh.dest[3] = m_ucLowMacAddr[3];
// 		eh.dest[4] = m_ucLowMacAddr[4];
// 		eh.dest[5] = m_ucLowMacAddr[5];
// 		ip.daddr = m_uiLowStreamIP;
// 		dh.sport = m_usPcapDownStreamSrcPort;
// 		_InterlockedIncrement(&m_lDownStreamNetSndFrameNum);
// 	}
// 	else
// 	{
// 		eh.dest[0] = m_ucHighMacAddr[0];
// 		eh.dest[1] = m_ucHighMacAddr[1];
// 		eh.dest[2] = m_ucHighMacAddr[2];
// 		eh.dest[3] = m_ucHighMacAddr[3];
// 		eh.dest[4] = m_ucHighMacAddr[4];
// 		eh.dest[5] = m_ucHighMacAddr[5];
// 		ip.daddr = m_uiHighStreamIP;
// 		dh.sport = m_usPcapUpStreamSrcPort;
// 		_InterlockedIncrement(&m_lUpStreamNetSndFrameNum);
// 	}
// 	ip.ver_ihl = 0x45;		// Version (4 bits) + Internet header length (4 bits)
// 	ip.tos = 0x0;			// Type of service 
// 	ip.tlen = htons(pFrameData->m_uiLength + sizeof(IP_Header) + sizeof(UDP_Header));			// Total length 
// 	ip.identification = htons(3347); // Identification
// 	ip.flags_fo = 0x40;		// Flags (3 bits) + Fragment offset (13 bits)
// 	ip.ttl = 0x80;			// Time to live
// 	ip.proto = 0x11;			// Protocol
// 	ip.saddr = m_uiNetIP;// Source address
// 	ip.crc = check_sum((unsigned short*)&ip, sizeof(ip));// Header checksum
// 	
// 
// 	dh.crc = 0;
// 	dh.dport = pFrameData->m_usDstPort;
// 	dh.len = htons(pFrameData->m_uiLength + sizeof(UDP_Header));
// 	memcpy(tmp_buf, &eh, sizeof(eh));
// 	iPos += sizeof(eh);
// 	memcpy(tmp_buf + iPos, &ip, sizeof(ip));
// 	iPos += sizeof(ip);
// 	memcpy(tmp_buf + iPos, &dh, sizeof(dh));
// 	iPos += sizeof(dh);
// 	memcpy(tmp_buf + iPos, pFrameData->m_ucData, pFrameData->m_uiLength);
// 	iPos += pFrameData->m_uiLength;
// 	return pcap_sendpacket(m_ptrPcap, tmp_buf, iPos) == 0 ? true : false;
// }
/** Socket发送帧*/
int CNetPcapComm::SocketSndFrameData(m_oFrameData* pFrameData)
{
	int ret = 0;
	timeval time_val = {0};
	fd_set write_fds;
	SOCKADDR_IN socketaddr;
	SOCKET oSocket;
	SecureZeroMemory(&socketaddr, sizeof(socketaddr));
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = pFrameData->m_usDstPort;
	if (pFrameData->m_bDownStream == true)
	{
		socketaddr.sin_addr.s_addr = m_uiLowStreamIP;
		oSocket = m_SocketDownStream;
	}
	else
	{
		socketaddr.sin_addr.s_addr = m_uiHighStreamIP;
		oSocket = m_SocketUpStream;
	}
	FD_ZERO(&write_fds);
	FD_SET(oSocket, &write_fds);
	ret = select(NULL, NULL, &write_fds, NULL, &time_val);
	if(ret == 0) return 0;
	else if(ret == SOCKET_ERROR) return -1;
	return sendto(oSocket, &pFrameData->m_ucData[0], 
		pFrameData->m_uiLength, 0, (SOCKADDR*)&socketaddr, sizeof(SOCKADDR));
}
void CNetPcapComm::OnClose()
{
	m_oDownStreamRcvSndPortMap.clear();
	m_oUpStreamRcvSndPortMap.clear();
	EnterCriticalSection(&m_oSec);
	m_olsFrameDataFree.clear();
	m_olsFrameDataDownStream.clear();
	m_olsFrameDataUpStream.clear();
	delete[] m_pFrameDataArray;
	m_pFrameDataArray = NULL;
	LeaveCriticalSection(&m_oSec);
	DeleteCriticalSection(&m_oSec);
	pcap_close(m_ptrPcap);
	timeKillEvent(m_TimerID);
	timeEndPeriod(m_uiTimeRes);
}