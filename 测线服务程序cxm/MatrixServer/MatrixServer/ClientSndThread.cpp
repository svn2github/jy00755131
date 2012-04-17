#include "StdAfx.h"
#include "ClientSndThread.h"


CClientSndThread::CClientSndThread(void)
{
	m_pClientSndFrame = NULL;
	m_pClientSocket = NULL;
}


CClientSndThread::~CClientSndThread(void)
{
}

// ´¦Àíº¯Êı
void CClientSndThread::OnProc(void)
{
	char* test = "askdfjasif65utyjtjtjttyutututyh";
	m_pClientSocket->Send(test, strlen(test)+1);
}