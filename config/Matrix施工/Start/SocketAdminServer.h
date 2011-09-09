#pragma once

// CSocketAdminServer 命令目标
/**
 * @class CSocketAdminServer
 * @brief 与管理服务器程序通信的Socket类，主要完成用户登录
 *
 * 
 */
#include "..\common\UserInfo.h"
#include "..\common\SocketParam.h"
class CStartDlg;
class CSocketAdminServer : public CSocket
{
public:
	CSocketAdminServer();
	virtual ~CSocketAdminServer();
public:
	CStartDlg*	m_pDlg;
public:
	// 创建与AdminServer程序通信的Socket端口
	bool CreateSocket(UINT wPort, LPCTSTR strHostIP);
	// 发送用户登录数据帧
	int SendFrm_UserLogin(LPCTSTR strUserName,LPCTSTR strPassword,LPCTSTR strServerIP);
	// 发送用户退出数据帧
	int SendFrm_UserLogout(CUserInfo* pUserInfo);
	virtual void OnReceive(int nErrorCode);
};


