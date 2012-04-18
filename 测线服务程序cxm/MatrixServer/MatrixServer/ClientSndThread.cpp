#include "StdAfx.h"
#include "ClientSndThread.h"


CClientSndThread::CClientSndThread(void)
{
	m_pClientSndFrame = NULL;
}


CClientSndThread::~CClientSndThread(void)
{
}

// 处理函数
void CClientSndThread::OnProc(void)
{
	int iFrameNum = 0;
	m_oCommFrameStructPtr ptrFrame = NULL;
	EnterCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);

	iFrameNum = m_pClientSndFrame->m_olsCommWorkFrame.size();
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrame = *m_pClientSndFrame->m_olsCommWorkFrame.begin();
		// 发送该帧
		ptrFrame->m_uiTimeOutCount = 0;
		ptrFrame->m_uiSndCount = 1;
		m_pClientSndFrame->MakeSendFrame(ptrFrame);
		m_pClientSndFrame->m_olsCommWorkFrame.pop_front();
		// 如果为设置帧则将其加入发送帧索引
		if (ptrFrame->m_cCmdType == CmdTypeSet)
		{
			
		}
		// 如果为应答帧则发送后就无需关注应答
		else if (ptrFrame->m_cCmdType == CmdTypeReturn)
		{
			m_pClientSndFrame->AddFreeFrameStruct(ptrFrame);
		}
	}
	LeaveCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
}