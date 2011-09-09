#pragma once

// CSocketUserInfo 命令目标

/**
 * @class CSocketUserInfo
 * @brief 用于与客户端进行用户管理的Socket类
 * 
 */
class CSocketUserInfo : public CSocket
{
public:
	CSocketUserInfo();
	virtual ~CSocketUserInfo();

private:
	bool  AnalysisCmd_UserLogin(unsigned char* pBuf,int nSize,sockaddr_in* psockaddr);
	bool  AnalysisCmd_UserLogout(unsigned char* pBuf,int nSize);	
	bool  AnalysisCmd_ReqUserInfo(sockaddr_in* psockaddr);
	bool  AnalysisCmd_AllUserInfo(BYTE* pBuf,int nSize);
public:
	// 创建Socket端口
	bool CreateSocket(UINT wPort, LPCTSTR strHostIP);
	// 接收数据
	virtual void OnReceive(int nErrorCode);
};


