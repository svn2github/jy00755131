#pragma once
#include "pcap\pcap.h"
#include <hash_map>
#include <list>
using std::hash_map;
using std::list;
#define MaxStringSize	260
#define MacAddrSize		6
#define FrameDataSize	512

#pragma intrinsic (_InterlockedIncrement,	\
					_InterlockedDecrement,	\
					_InterlockedExchangeAdd,	\
					_InterlockedExchange,	\
					_InterlockedCompareExchange)
class CNetPcapComm
{
public:
	CNetPcapComm(void);
	~CNetPcapComm(void);
public:
	/** Pcap指针*/
	pcap_t* m_ptrPcap;
	/** 网卡序号*/
	unsigned int m_uiNetCardId;
	/** pcap网络数据包最大长度*/
	unsigned int m_uiPcapPacketMaxSize;
	/** pcap超时时间*/
	unsigned int m_uiPcapTimeOut;
	/** pcap缓冲区大小*/
	unsigned int m_uiPcapBufSize;
	/** pcap过滤器*/
	char m_cPcapfilter[MaxStringSize];
	/** 下行帧收发端口索引表，关键字为接收端口*/
	hash_map<unsigned short ,unsigned short> m_oDownStreamRcvSndPortMap;
	/** 上行帧收发端口索引表，关键字为接收端口*/
	hash_map<unsigned short ,unsigned short> m_oUpStreamRcvSndPortMap;
	/** Pcap下行帧发送的源端口*/
	unsigned short m_usPcapDownStreamSrcPort;
	/** Pcap上行帧发送的源端口*/
	unsigned short m_usPcapUpStreamSrcPort;
	/** 下行PC的IP地址*/
	unsigned int m_uiHighStreamIP;
	/** 上行PC的IP地址*/
	unsigned int m_uiLowStreamIP;
	/** Net程序所在的PC的IP地址*/
	unsigned int m_uiNetIP;
	/** 下行PC的Mac地址*/
	u_char m_ucHighMacAddr[MacAddrSize];
	/** 上行PC的Mac地址*/
	u_char m_ucLowMacAddr[MacAddrSize];
	/** Net程序所在PC的Mac地址*/
	u_char m_ucNetMacAddr[MacAddrSize];
	/** 数据缓冲队列大小*/
	unsigned int m_uiPcapQueueSize;
	/** 存储数据帧结构体*/
	typedef struct FrameDataStruct
	{
		/** 判断是否是下行帧*/
		bool m_bDownStream;
		/** 数据存储区指针*/
		char m_ucData[FrameDataSize];
		/** 存储的数据长度*/
		unsigned int m_uiLength;
		/** 目标端口*/
		unsigned short m_usDstPort;
	}m_oFrameData;
	/** 存储数据帧数组*/
	m_oFrameData* m_pFrameDataArray;
	/** 空闲数据帧个数*/
	unsigned int m_uiFreeCount;
	/** 空闲数据帧指针队列*/
	list<m_oFrameData*> m_olsFrameDataFree;
	/** 上行数据帧指针队列*/
	list<m_oFrameData*> m_olsFrameDataUpStream;
	/** 下行数据帧指针队列*/
	list<m_oFrameData*> m_olsFrameDataDownStream;
	/** 临界区变量*/
	CRITICAL_SECTION m_oSec;
	/** 下行数据Net程序接收帧*/
	volatile long m_lDownStreamNetRevFrameNum;
	/** 下行数据Net程序发送帧*/
	volatile long m_lDownStreamNetSndFrameNum;
	/** 上行数据Net程序接收帧*/
	volatile long m_lUpStreamNetRevFrameNum;
	/** 上行数据Net程序发送帧*/
	volatile long m_lUpStreamNetSndFrameNum;
	/** 下行Socket*/
	SOCKET m_SocketDownStream;
	/** 上行Socket*/
	SOCKET m_SocketUpStream;
public:
	/** 初始化*/
	void OnInit();
	/** 关闭*/
	void OnClose();
	/** 解析接收发送端口*/
	void PhraseRcvSndPort(CString str, hash_map<unsigned short, unsigned short>* pMap);
	/** 判断接收端口是否已加入索引表*/
	BOOL IfRcvPortExistInMap(unsigned short usRcvPort, 
		hash_map<unsigned short, unsigned short>* pMap);
	/** 增加对象到索引表*/
	void AddPortToMap(unsigned short usRcvPort, unsigned short usSndPort, 
		hash_map<unsigned short, unsigned short>* pMap);
	/** 根据接收端口，得到发送*/
	unsigned int GetSndPortFromMap(unsigned short usRcvPort, 
		hash_map<unsigned short, unsigned short>* pMap);
	/** 重置存储的数据帧*/
	void OnResetFrameData(m_oFrameData* pFrameData);
	/** 得到一个空闲的存储数据帧*/
	m_oFrameData* GetFreeFrameData();
	/** 将存储数据帧加入空闲列表*/
	void AddFreeFrameData(m_oFrameData* pFrameData);
	/** Pcap发送帧*/
	bool PcapSndFrameData(m_oFrameData* pFrameData);
	/** 求校验和*/
	unsigned short check_sum (unsigned short * addr, int len);
	/** Socket发送帧*/
	int SocketSndFrameData(m_oFrameData* pFrameData);
	/** 创建Socket*/
	SOCKET CreateSocket(unsigned int uiSrcPort, int iSndBufferSize);
};

