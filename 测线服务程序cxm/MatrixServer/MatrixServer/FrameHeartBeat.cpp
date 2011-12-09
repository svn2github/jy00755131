#include "StdAfx.h"
#include "FrameHeartBeat.h"

CFrameHeartBeat::CFrameHeartBeat()
{
}

CFrameHeartBeat::~CFrameHeartBeat()
{
}

/**
* ÷ÿ÷√∂‘œÛ
* @param void
* @return void
*/
void CFrameHeartBeat::MakeHeartBeatFrame()
{
	memset(m_pFrameData, SndFrameBufInit, SndFrameSize);
	m_pFrameData[0] = FrameHeadCheck0;
	m_pFrameData[1] = FrameHeadCheck1;
	m_pFrameData[2] = FrameHeadCheck2;
	m_pFrameData[3] = FrameHeadCheck3;
}