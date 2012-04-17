#pragma once
#include "ClientCommFrame.h"
class CClientSndFrame : public CClientCommFrame
{
public:
	CClientSndFrame(void);
	~CClientSndFrame(void);
public:


public:
	// Éú³ÉÓ¦´ðÖ¡
	void MakeReturnFrame(m_oCommFrameStructPtr ptrFrame);
};

