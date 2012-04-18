#include "StdAfx.h"
#include "ClientSndFrame.h"


CClientSndFrame::CClientSndFrame(void)
{
	m_pClientSocket = NULL;
}


CClientSndFrame::~CClientSndFrame(void)
{
}


// 生成应答帧
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
// 生成发送帧
void CClientSndFrame::MakeSendFrame(m_oCommFrameStructPtr ptrFrame)
{
	char* pChar = m_pClientSocket->m_cSndBuf;
	int iPos = 0;
	unsigned short usCmd = 0;
	memset(pChar, 0, FrameSizeLimit);
	pChar[iPos] = FrameHead1;
	iPos++;
	pChar[iPos] = FrameHead2;
	iPos++;
	pChar[iPos] = FrameHead3;
	iPos++;
	pChar[iPos] = FrameHead4;
	iPos++;
 	// 帧长度
 	memcpy(&pChar[iPos], &ptrFrame->m_usFrameInfoSize, 2);
	iPos += 2;
	// 帧命令字，表明帧的功能
	usCmd = ptrFrame->m_usCmd;
	if (ptrFrame->m_cCmdType == CmdTypeReturn)
	{
		usCmd |= CmdReturnBit;
	}
	memcpy(&pChar[iPos], &usCmd, 2);
	iPos += 2;
}
