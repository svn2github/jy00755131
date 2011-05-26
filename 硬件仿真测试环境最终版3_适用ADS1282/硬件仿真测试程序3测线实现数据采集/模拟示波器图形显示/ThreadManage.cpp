#include "StdAfx.h"
#include "ThreadManage.h"

CThreadManage::CThreadManage(void)
{
}

CThreadManage::~CThreadManage(void)
{
}

// 初始化
void CThreadManage::OnInit(void)
{
	m_oHeartBeatThread.OnInit();
	m_oHeartBeatThread.CreateThread();
	m_oADDataThread.OnInit();
	m_oADDataThread.CreateThread();
	m_oHeadFrameThread.OnInit();
	m_oHeadFrameThread.CreateThread();
	m_oTailFrameThread.OnInit();
	m_oTailFrameThread.CreateThread();
	m_oInstrumentList.OnInit();
	m_oHeadFrameThread.m_oInstrumentList = &m_oInstrumentList;
	m_oTailFrameThread.m_oInstrumentList = &m_oInstrumentList; 
}

// 关闭并结束线程
void CThreadManage::OnClose(void)
{
	int iResult = 0;
	m_oHeartBeatThread.OnClose();
	iResult = ::WaitForSingleObject(m_oHeartBeatThread.m_hHeartBeatThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("心跳线程在100ms内未能正常结束！"));
	}
	m_oHeadFrameThread.OnClose();
	iResult = ::WaitForSingleObject(m_oHeadFrameThread.m_hHeadFrameThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("首包线程在100ms内未能正常结束！"));
	}
	m_oADDataThread.OnClose();
	iResult = ::WaitForSingleObject(m_oADDataThread.m_hADDDataThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("数据采集线程在100ms内未能正常结束！"));
	}
	m_oTailFrameThread.OnClose();
	iResult = ::WaitForSingleObject(m_oTailFrameThread.m_hTailFrameThreadClose,100);
	if (iResult != WAIT_OBJECT_0)
	{
		AfxMessageBox(_T("尾包线程在100ms内未能正常结束！"));
	}
	m_oInstrumentList.OnClose();
}
