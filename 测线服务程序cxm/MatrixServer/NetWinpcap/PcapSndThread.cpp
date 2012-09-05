#include "StdAfx.h"
#include "PcapSndThread.h"


CPcapSndThread::CPcapSndThread(void)
{
	m_pNetPcapComm = NULL;
}


CPcapSndThread::~CPcapSndThread(void)
{
}
// 处理函数
void CPcapSndThread::OnProc(void)
{
	int iFrameNum = 0;
	CNetPcapComm::FrameDataStruct* pFrameData = NULL;
	EnterCriticalSection(&m_pNetPcapComm->m_oSec);
	iFrameNum = m_pNetPcapComm->m_olsFrameDataWork.size();
	for (int i=0; i<iFrameNum; i++)
	{
		pFrameData = *m_pNetPcapComm->m_olsFrameDataWork.begin();
		// 发送该帧
		m_pNetPcapComm->SndFrameData(pFrameData);
		m_pNetPcapComm->m_olsFrameDataWork.pop_front();
		m_pNetPcapComm->AddFreeFrameData(pFrameData);
	}
	LeaveCriticalSection(&m_pNetPcapComm->m_oSec);
}