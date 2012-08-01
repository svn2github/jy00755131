#include "stdafx.h"
#include "MatrixCommDll.h"

CCommHeartBeatThread::CCommHeartBeatThread(void)
{
	m_pCommSndFrame = NULL;
}


CCommHeartBeatThread::~CCommHeartBeatThread(void)
{
}

// ´¦Àíº¯Êý
void CCommHeartBeatThread::OnProc(void)
{
	m_pCommSndFrame->MakeSetFrame(CmdSetHeartBeat, NULL, 0);
}