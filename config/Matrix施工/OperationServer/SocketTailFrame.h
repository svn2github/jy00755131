#pragma once

// CSocketTailFrame 命令目标
///////////////////////////////////////////////////////////////////////////
/**
 * @class CSocketTailFrame
 * @brief 尾包接收
 *
 * 尾包接收对象，接收尾包数据，解析当前测网的时码
 */
class CSocketTailFrame : public CSocket
{
public:
	CSocketTailFrame();
	virtual ~CSocketTailFrame();
	virtual void OnReceive(int nErrorCode);
	bool CreateTailFrameSocket(UINT wPort, LPCTSTR strHostIP);
};


