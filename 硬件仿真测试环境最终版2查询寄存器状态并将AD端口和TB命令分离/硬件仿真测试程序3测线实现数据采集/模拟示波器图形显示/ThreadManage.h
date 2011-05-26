#pragma once
#include "ADDataThread.h"
#include "HeartBeatThread.h"
#include "HeadFrameThread.h"
#include "InstrumentList.h"
#include "TailFrameThread.h"
class CThreadManage
{
public:
	CThreadManage(void);
	~CThreadManage(void);
public:
	CHeartBeatThread	m_oHeartBeatThread;
	CHeadFrameThread	m_oHeadFrameThread;
	CADDataThread		m_oADDataThread;
	CInstrumentList		m_oInstrumentList;
	CTailFrameThread	m_oTailFrameThread;
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
};
