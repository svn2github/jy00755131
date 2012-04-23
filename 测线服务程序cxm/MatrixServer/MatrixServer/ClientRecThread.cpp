#include "StdAfx.h"
#include "ClientRecThread.h"


CClientRecThread::CClientRecThread(void)
{
	m_pClientRecFrame = NULL;
	m_pClientSndFrame = NULL;
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
	EnterCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
	iFrameNum = m_pClientRecFrame->m_olsCommWorkFrame.size();
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrame = *m_pClientRecFrame->m_olsCommWorkFrame.begin();
		// 如果为设置帧
		if (ptrFrame->m_cCmdType == CmdTypeSet)
		{
			// 将帧内容加入待处理任务中

			// 发送应答帧
			m_pClientSndFrame->MakeReturnFrame(ptrFrame);
		}
		// 如果为应答帧
		else if (ptrFrame->m_cCmdType == CmdTypeReturn)
		{
			// 在已发送帧索引中找到该帧并删除
			if (FALSE == m_pClientSndFrame->DeleteFramePtrFromSndMap(ptrFrame->m_usCmd, 
				ptrFrame->m_uiServerTimeStep, ptrFrame->m_uiPacketIndex, &m_pClientSndFrame->m_oSndFrameMap))
			{
				// 加入错误日志
			}
		}
		m_pClientRecFrame->m_olsCommWorkFrame.pop_front();
		m_pClientRecFrame->AddFreeFrameStruct(ptrFrame);
	}
	LeaveCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
	LeaveCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
}
