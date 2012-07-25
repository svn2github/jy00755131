#include "stdafx.h"
#include "MatrixCommDll.h"

CCommMonitorThread::CCommMonitorThread(void)
{
	m_pComClientMap = NULL;
	m_olsClientClose.clear();
}

CCommMonitorThread::~CCommMonitorThread(void)
{
	m_olsClientClose.clear();
}

// ´¦Àíº¯Êý
void CCommMonitorThread::OnProc(void)
{
	hash_map<SOCKET, CCommClient*>::iterator iter;
	list<CCommClient*>::iterator iterList;
	bool bConnectValid = true;
	for (iter = m_pComClientMap->begin(); iter != m_pComClientMap->end(); iter++)
	{
		EnterCriticalSection(&iter->second->m_oSndFrame.m_oSecClientFrame);
		bConnectValid = iter->second->m_oSndFrame.m_bConnectValid;
		LeaveCriticalSection(&iter->second->m_oSndFrame.m_oSecClientFrame);
		if (bConnectValid == false)
		{
			m_olsClientClose.push_back(iter->second);
		}
	}
	for (iterList = m_olsClientClose.begin(); iterList != m_olsClientClose.end(); iterList++)
	{
		(*iterList)->OnClose();
	}
	m_olsClientClose.clear();
}