#include "StdAfx.h"
#include "ClientRecThread.h"


CClientRecThread::CClientRecThread(void)
{
	m_pClientRecFrame = NULL;
}


CClientRecThread::~CClientRecThread(void)
{
}

// 处理函数
void CClientRecThread::OnProc(void)
{
	int iFrameNum = 0;
	m_oCommFrameStructPtr ptrFrame = NULL;
	EnterCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
	iFrameNum = m_pClientRecFrame->m_olsCommWorkFrame.size();
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrame = *m_pClientRecFrame->m_olsCommWorkFrame.begin();
		// 如果为设置帧
		if (ptrFrame->m_cCmdType == CmdTypeSet)
		{
			// 将帧内容加入待处理任务中
		}
		// 如果为应答帧
		else if (ptrFrame->m_cCmdType == CmdTypeReturn)
		{
			// 在已发送帧索引中找到该帧
		}
		m_pClientRecFrame->m_olsCommWorkFrame.pop_front();
	}
	LeaveCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
}
