#pragma once
#include "ClientCommFrame.h"
#include "ClientSocket.h"
class CClientSndFrame : public CClientCommFrame
{
public:
	CClientSndFrame(void);
	~CClientSndFrame(void);
public:
	// 客户端Socket类指针
	CClientSocket* m_pClientSocket;
public:
	// 生成应答帧
	void MakeReturnFrame(m_oCommFrameStructPtr ptrFrame);
	// 生成发送帧
	void MakeSendFrame(m_oCommFrameStructPtr ptrFrame);
};

