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
// ´¦Àíº¯Êı
void CPcapRcvThread::OnProc(void)
{
	int status = 0;
	IP_Header* ih = NULL;
	UDP_Header* uh = NULL;
	unsigned int uiIPlength = 0;
	struct pcap_pkthdr* pkt_header = NULL;
	const u_char* pkt_data = NULL;
	unsigned short usDstPort = 0;
	bool bDownStream = false;
	CNetPcapComm::FrameDataStruct* pFrameData = NULL;
	status = pcap_next_ex(m_pNetPcapComm->m_ptrPcap, &pkt_header, &pkt_data);
	if(status == 1)
	{
		ih = (IP_Header *)(pkt_data + sizeof(Ethernet_Header));
		uiIPlength = (ih->ver_ihl & 0xf) * 4;
		uh = (UDP_Header *)((u_char*)ih + uiIPlength);
		if ((TRUE == m_pNetPcapComm->IfRcvPortExistInMap(uh->dport, &m_pNetPcapComm->m_oDownStreamRcvSndPortMap))
			&& (ih->saddr == m_pNetPcapComm->m_uiDownStreamIP))
		{
			bDownStream = true;
			usDstPort = m_pNetPcapComm->GetSndPortFromMap(uh->dport, &m_pNetPcapComm->m_oDownStreamRcvSndPortMap);
		}
		else if ((TRUE == m_pNetPcapComm->IfRcvPortExistInMap(uh->dport, &m_pNetPcapComm->m_oUpStreamRcvSndPortMap))
			&& (ih->saddr == m_pNetPcapComm->m_uiUpStreamIP))
		{
			bDownStream = false;
			usDstPort = m_pNetPcapComm->GetSndPortFromMap(uh->dport, &m_pNetPcapComm->m_oUpStreamRcvSndPortMap);
		}
		else
		{
			return;
		}
		EnterCriticalSection(&m_pNetPcapComm->m_oSec);
		pFrameData = m_pNetPcapComm->GetFreeFrameData();
		pFrameData->m_bDownStream = bDownStream;
		pFrameData->m_usDstPort = usDstPort;
		pFrameData->m_uiLength = uh->len;
		memcpy(pFrameData->m_ucData, pkt_data + sizeof(Ethernet_Header) + sizeof(IP_Header)
			+ sizeof(UDP_Header), uh->len);
		m_pNetPcapComm->m_olsFrameDataWork.push_back(pFrameData);
		LeaveCriticalSection(&m_pNetPcapComm->m_oSec);
// 		InterlockedIncrement(&pcap_service_ptr_->application_ptr_->pcap_data_inp_num_);
	}
}