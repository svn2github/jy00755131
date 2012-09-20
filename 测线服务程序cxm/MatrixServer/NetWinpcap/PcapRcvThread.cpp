#include "StdAfx.h"
#include "PcapRcvThread.h"
#include "pcap\pcap.h"

CPcapRcvThread::CPcapRcvThread(void)
{
	m_pNetPcapComm = NULL;
}


CPcapRcvThread::~CPcapRcvThread(void)
{
}
// ´¦Àíº¯Êý
void CPcapRcvThread::OnProc(void)
{
	IP_Header* ih = NULL;
	UDP_Header* uh = NULL;
	unsigned int uiIPlength = 0;
	struct pcap_pkthdr* pkt_header = NULL;
	const u_char* pkt_data = NULL;
	unsigned short usDstPort = 0;
	bool bDownStream = false;
	CNetPcapComm::FrameDataStruct* pFrameData = NULL;
	while(1 == pcap_next_ex(m_pNetPcapComm->m_ptrPcap, &pkt_header, &pkt_data))
	{
		ih = (IP_Header *)(pkt_data + sizeof(Ethernet_Header));
		uiIPlength = (ih->ver_ihl & 0xf) * 4;
		uh = (UDP_Header *)((u_char*)ih + uiIPlength);
		if ((TRUE == m_pNetPcapComm->IfRcvPortExistInMap(uh->dport, &m_pNetPcapComm->m_oDownStreamRcvSndPortMap))
			&& (ih->saddr == m_pNetPcapComm->m_uiHighStreamIP))
		{
			bDownStream = true;
			usDstPort = m_pNetPcapComm->GetSndPortFromMap(uh->dport, &m_pNetPcapComm->m_oDownStreamRcvSndPortMap);
			_InterlockedIncrement(&m_pNetPcapComm->m_lDownStreamNetRevFrameNum);
		}
		else if ((TRUE == m_pNetPcapComm->IfRcvPortExistInMap(uh->dport, &m_pNetPcapComm->m_oUpStreamRcvSndPortMap))
			&& (ih->saddr == m_pNetPcapComm->m_uiLowStreamIP))
		{
			bDownStream = false;
			usDstPort = m_pNetPcapComm->GetSndPortFromMap(uh->dport, &m_pNetPcapComm->m_oUpStreamRcvSndPortMap);
			_InterlockedIncrement(&m_pNetPcapComm->m_lUpStreamNetRevFrameNum);
		}
		else
		{
			continue;
		}
		EnterCriticalSection(&m_pNetPcapComm->m_oSec);
		pFrameData = m_pNetPcapComm->GetFreeFrameData();
		pFrameData->m_bDownStream = bDownStream;
		pFrameData->m_usDstPort = usDstPort;
		pFrameData->m_uiLength = htons(uh->len) - sizeof(UDP_Header);
		memcpy(pFrameData->m_ucData, pkt_data + sizeof(Ethernet_Header) + sizeof(IP_Header)
			+ sizeof(UDP_Header), pFrameData->m_uiLength);
		if (bDownStream == true)
		{
			m_pNetPcapComm->m_olsFrameDataDownStream.push_back(pFrameData);
		}
		else
		{
			m_pNetPcapComm->m_olsFrameDataUpStream.push_back(pFrameData);
		}
		LeaveCriticalSection(&m_pNetPcapComm->m_oSec);
	}
}