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
	iFrameNum = m_pNetPcapComm->m_olsFrameDataUpStream.size();
	for (int i=0; i<iFrameNum; i++)
	{
		pFrameData = *m_pNetPcapComm->m_olsFrameDataUpStream.begin();
		// 发送该帧
		if (pFrameData->m_uiLength == m_pNetPcapComm->SocketSndFrameData(pFrameData))
		{
			_InterlockedIncrement(&m_pNetPcapComm->m_lUpStreamNetSndFrameNum);
		}
		else
		{
			OutputDebugString(_T("上行帧发送失败！"));
		}
		m_pNetPcapComm->m_olsFrameDataUpStream.pop_front();
		m_pNetPcapComm->AddFreeFrameData(pFrameData);
	}
	LeaveCriticalSection(&m_pNetPcapComm->m_oSec);
}