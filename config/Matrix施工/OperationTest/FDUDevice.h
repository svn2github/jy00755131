#pragma once

//#include "FDUSocket.h"

class CFDUDevice
{
public:
	CFDUDevice(void);
	~CFDUDevice(void);


	DWORD	m_dwIP;
	unsigned char m_nIndex;
	char    m_pBuf[8][256];

	int WriteData(char *pBuf,int nSize)
	{
		memcpy(m_pBuf[m_nIndex],pBuf,nSize);
		m_nIndex = ((m_nIndex+1)>7) ? 0: (m_nIndex+1);
		return 0;
	}

};
