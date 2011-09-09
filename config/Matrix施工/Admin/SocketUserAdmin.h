#pragma once

// CSocketUserAdmin 命令目标
/**
 * @class CSocketUserAdmin
 * @brief 用户管理Socket端口：Admin程序通过该Socket端口查询所有用户信息，注册用户，
 删除用户，修改用户属性等。
 *
 * 
 */
class CSocketUserAdmin : public CSocket
{
public:
	CSocketUserAdmin();
	virtual ~CSocketUserAdmin();

public:
	virtual void OnReceive(int nErrorCode);

	bool CreateSocket(UINT wPort, LPCTSTR strHostIP);
	// 向服务器发送请求所有用户信息的命令帧
	int  SendFrm_ReqUsersInfo();
	// 香服务器发送所有用户信息帧
	bool SendFrm_SendAllUserInfo();
	bool AnalysisCmd_AllUserInfo(BYTE* pBuf,int nSize);
};


