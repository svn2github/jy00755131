
#pragma once
/******************************************************************************
  
    功    能：用户数据结构

    修改历史：

*******************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 用户属性定义
// 用户角色定义
#define USERINFO_ROLE_GUEST					0
#define USERINFO_ROLE_OBSERVER				1
#define USERINFO_ROLE_SENIOROBSERVER		2

// 用户密码失效模式
#define USERINFO_EXPIREDMODE_NEVER			0		// 永不失效
#define USERINFO_EXPIREDMODE_DATE			1		// 指定日期后失效
//////////////////////////////////////////////////////////////////////////
// 用户状态
#define USERINFO_STATE_OFFLINE				0		// 离线
#define USERINFO_STATE_ALINE				1		// 在线
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 传送用户管理的通信帧最大长度
#define     SOCKET_USERINFOFRM_MAXLEN		11264
/////////////////////帧命令码/////////////////////////////////////////////////////////////////////////
//  表示用户登录
#define     USERINFOFRM_CMDCODE_LOGIN		0x2001
//  表示用户注销
#define     USERINFOFRM_CMDCODE_LOGOUT		0x2002
//  表示用户信息注册
#define     USERINFOFRM_CMDCODE_NEW			0x2003
//  表示用户信息更新
#define     USERINFOFRM_CMDCODE_MODIFY		0x2004
//  表示删除用户
#define     USERINFOFRM_CMDCODE_DELETE		0x2005
//  查询全部用户信息
#define     USERINFOFRM_CMDCODE_QUERY		0x20FF

// 服务程序发送给客户端时
//  表示认证成功；
#define     USERINFOFRM_CMDCODE_AUTHENOK	0x2101
// 表示认证失败；
#define     USERINFOFRM_CMDCODE_AUTHENFAIL	0x2102
// 表示发送所有用户信息
#define     USERINFOFRM_CMDCODE_ALLUSERS	0x21FF

// 日志管理，是日志网络帧，在start程序中与用户管理在同一个网络端口处理
#define     LOGINFOFRM_CMDCODE_LOG			0xFFFF

/**
 * @class CUserInfo
 * @brief 用户属性描述，包括用户名、密码、角色、密码过期时间、过期处理方式、
          注册时间、最后登录时间
 *
 * 
 */
class CUserInfo
{
public:
	CUserInfo(void);
	~CUserInfo(void);
	// 属性
public:
	/** 用户名 */
	CString		m_strUserName;
	/** 密码 */
	CString     m_strPassword;
	/** 角色：0为Guest,1为Observer,2为Senior Observer*/
	int         m_nRole;
	/** 注册时间 */
	SYSTEMTIME	m_tmRegister;
	/** 失效模式：0表示密码永不失效，1表示超过指定时间失效 */
	int			m_nExpiredMode;
	/** 密码过期时间 */
	SYSTEMTIME	m_tmExpiryDate;
	/** 密码过期处理方式: 0：不自动删除，1为删除 */
	BOOL		m_bAutoDelete;
	/** 最后登录时间 */
	SYSTEMTIME	m_tmLastUsed;
	
	/** 会话ID*/
	DWORD      m_dwSessionID;
	/** 当前状态，0:不在线；1：在线*/
	int        m_nState;
	/** 当前登录的客户机IP地址*/
	DWORD	   m_dwClientIP;
	/** 当前所使用的应用程序*/
	int        m_nApplication;
	// 方法
public:
	int		GetSizeOfBytes(void);
	int		Write(unsigned char* pBuf,int nSize);
	int		Read(unsigned char* pBuf,int nSize);

};
/**
 * @class CUserInfos
 * @brief 用户集合类
 * 用动态数组管理所有的用户。
 */
class CUserInfos
{
	//属性
public:
	/** 用于计算用户Session的公共计数器 */
	DWORD		m_dwSession;
private:
	CArray<CUserInfo*,CUserInfo*>	m_arrUserInfo;
public:
	CUserInfos(void);
	~CUserInfos(void);

	// 计算用户总数
	int		GetCount(void)	{return m_arrUserInfo.GetCount();};	
	// 计算在线用户总数
	int		GetAliveCount(void)	;	
	// 删除单个用户
	void    RemoveAt(int nIndex);
	// 删除所有用户
	void    RemoveAll(void);
	// 得到指定索引的对象，索引从0开始
	CUserInfo* GetUserInfo(int iIndex);	
	// 添加一个对象
	int Add(CUserInfo* pUserInfo);
	// 通过用户名查找用户信息
	CUserInfo* FindUserByName(LPCTSTR strUserName);
	// 匹配用户名称和密码
	CUserInfo*	FindUserByNameAndPassWord(LPCTSTR strName,LPCTSTR strPassword);
	
};
