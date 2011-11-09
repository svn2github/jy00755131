#include "StdAfx.h"
#include "Instrument.h"

CInstrument::CInstrument(void)
: m_uiIPAddress(0)
, m_uiSN(0)
, m_uiRoutAddress(0)
, m_uiRoutAddressTop(0)
, m_uiRoutAddressDown(0)
, m_uiRoutAddressLeft(0)
, m_uiRoutAddressRight(0)
, m_uiHeadFrameTime(0)
, m_uiSystemTime(0)
, m_iHeadFrameCount(0)
, m_iTailFrameCount(0)
, m_uiIndex(0)
, m_bInUsed(false)
, m_uiInstrumentType(0)
, m_bIPSetOK(false)
, m_iLocation(0)
, m_iLineIndex(0)
, m_uiLineDirection(0)
, m_uiNetTime(0)
, m_usTailRecTime(0)
, m_usTailSendTime(0)
, m_bTailTimeReturnOK(false)
, m_bTailTimeExpired(false)
, m_bSendTailTimeFrame(false)
, m_bADCSet(false)
, m_uiIPResetTimes(0)
, m_uiADCSetOperationNb(0)
, m_iSelectObject(0)
, m_iSelectObjectNoise(0)
, m_uiUnCheckedNum(0)
, m_uiFDULocation(0)
, m_pInstrumentTop(NULL)
, m_pInstrumentDown(NULL)
, m_pInstrumentLeft(NULL)
, m_pInstrumentRight(NULL)
, m_uiBroadCastPort(0)
{
}

CInstrument::~CInstrument(void)
{
}

// 重置成员变量
void CInstrument::OnReset(void)
{ 
	m_uiIPAddress = 0;
	m_uiSN = 0;
	m_uiRoutAddress = 0;
	m_uiRoutAddressTop = 0;
	m_uiRoutAddressDown = 0;
	m_uiRoutAddressLeft = 0;
	m_uiRoutAddressRight = 0;
	m_uiHeadFrameTime = 0;
	m_uiSystemTime = 0;
	m_iHeadFrameCount = 0;
	m_iTailFrameCount = 0;
	m_bInUsed = false;
	m_uiInstrumentType = 0;
	m_bIPSetOK = false;
	m_iLocation = 0;
	m_iLineIndex = 0;
	m_uiLineDirection = 0;
	m_uiNetTime = 0;
	m_usTailRecTime = 0;
	m_usTailSendTime = 0;
	m_bTailTimeReturnOK = false;
	m_bTailTimeExpired = false;
	m_bSendTailTimeFrame = false;
	m_bADCSet = false;
	m_uiIPResetTimes = 0;
	m_uiADCSetOperationNb = 0;
	m_iSelectObject = 0;
	m_iSelectObjectNoise = 0;
	m_uiUnCheckedNum = 0;
	m_uiFDULocation = 0;
	m_pInstrumentTop = NULL;
	m_pInstrumentDown = NULL;
	m_pInstrumentLeft = NULL;
	m_pInstrumentRight = NULL;
	m_uiBroadCastPort = 0;
}
