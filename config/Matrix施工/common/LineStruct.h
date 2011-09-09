#pragma once

class CIPList
{
public:
		CIPList();
		~CIPList();

public:
		static unsigned int GetCollectIP(unsigned int uiLineNb, unsigned int uiPointNb, unsigned int uiChannelNb);
		static unsigned int GetBlastMachineIP(unsigned int uiNb);
		static unsigned int GetAuxIP(unsigned int uiNb);
		static DWORD GetLocalIP(void);
};

