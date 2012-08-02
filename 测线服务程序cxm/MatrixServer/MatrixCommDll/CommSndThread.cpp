#include "stdafx.h"
#include "MatrixCommDll.h"

CCommSndThread::CCommSndThread(void)
{
	m_pCommSndFrame = NULL;
}


CCommSndThread::~CCommSndThread(void)
{
}

// 处理函数
void CCommSndThread::OnProc(void)
{
	int iFrameNum = 0;
	bool bReSend = true;
	m_oCommFrameStructPtr ptrFrame = NULL;
	EnterCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
	iFrameNum = m_pCommSndFrame->m_olsCommWorkFrame.size();
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrame = *m_pCommSndFrame->m_olsCommWorkFrame.begin();
		// 发送该帧
		ptrFrame->m_uiTimeOutCount = 0;
		ptrFrame->m_uiSndCount = 1;
		m_pCommSndFrame->MakeSendFrame(ptrFrame);
		m_pCommSndFrame->m_olsCommWorkFrame.pop_front();
		// 如果为设置帧则将其加入已发送帧索引
		if (ptrFrame->m_cCmdType == CmdTypeSet)
		{
			m_pCommSndFrame->AddFramePtrToSndMap(ptrFrame->m_usCmd, ptrFrame->m_uiSrcTimeStep, 
				ptrFrame->m_uiPacketIndex, ptrFrame, &m_pCommSndFrame->m_oSndFrameMap);
		}
		// 如果为应答帧则发送后就无需关注应答
		else if (ptrFrame->m_cCmdType == CmdTypeReturn)
		{
			m_pCommSndFrame->AddFreeFrameStruct(ptrFrame);
		}
	}
	bReSend = m_pCommSndFrame->OnReSendFrame();
	LeaveCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
	if (bReSend == false)
	{
		EnterCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
		m_pCommSndFrame->m_bConnectValid = false;
		LeaveCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
	}
}