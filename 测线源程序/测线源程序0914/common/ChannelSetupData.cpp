#include "StdAfx.h"
#include "ChannelSetupData.h"

CChannelSetupData::CChannelSetupData(void)
{
}

CChannelSetupData::~CChannelSetupData(void)
{
}

// 根据测线号、测点号、测道号，生成IP地址
unsigned int CChannelSetupData::CreateIP(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	// IP地址
	return  uiNbLine * 1000000 + uiNbPoint * 10 + uiNbChannel;
}