#pragma once

// CSocketShotServer 命令目标
/******************************************************************************
    类    型： 
        
    功    能： 与放炮服务器通信的Socket，主要功能：
	            请求放炮服务；发送放炮命令；接收放炮状态

    修改历史：

*******************************************************************************/
class CSocketShotServer : public CSocket
{
public:
	CSocketShotServer();
	virtual ~CSocketShotServer();
	bool CreateSocket(UINT wPort, LPCTSTR strHostIP);
	virtual void OnReceive(int nErrorCode);
};


