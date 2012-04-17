#include "StdAfx.h"
#include "ClientSndFrame.h"


CClientSndFrame::CClientSndFrame(void)
{
}


CClientSndFrame::~CClientSndFrame(void)
{
}


// Éú³ÉÓ¦´ðÖ¡
void CClientSndFrame::MakeReturnFrame(m_oCommFrameStructPtr ptrFrame)
{
	m_oCommFrameStructPtr pFrameStruct = NULL;
	pFrameStruct = GetFreeFrameStruct();
	pFrameStruct->m_cCmdType = CmdTypeReturn;
	pFrameStruct->m_usCmd = ptrFrame->m_usCmd;
	pFrameStruct->m_uiServerTimeStep = GetTickCount();
	pFrameStruct->m_uiClientTimeStep = ptrFrame->m_uiClientTimeStep;
	pFrameStruct->m_uiPacketIndex = ptrFrame->m_uiPacketIndex;
	pFrameStruct->m_uiCmdIndex = ptrFrame->m_uiCmdIndex;
	pFrameStruct->m_uiFrameNum = ptrFrame->m_uiFrameNum;
	pFrameStruct->m_uiFrameIndex = ptrFrame->m_uiFrameIndex;
	pFrameStruct->m_cResult = CmdResultWait;
	pFrameStruct->m_usFrameInfoSize = ptrFrame->m_usFrameInfoSize;
	memcpy(&pFrameStruct->m_pcFrameInfo, &ptrFrame->m_pcFrameInfo, 
		pFrameStruct->m_usFrameInfoSize - FrameHeadInfoSize);
	EnterCriticalSection(&m_oSecClientFrame);
	m_olsCommWorkFrame.push_back(pFrameStruct);
	LeaveCriticalSection(&m_oSecClientFrame);
}
