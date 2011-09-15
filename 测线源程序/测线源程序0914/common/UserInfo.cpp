#include "stdafx.h"
#include "UserInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////   CUserInfo    ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
CUserInfo::CUserInfo(void)
{
	// 用户名
	m_strUserName=_T("");
	/** 密码 */
	m_strPassword=_T("");
	/** 角色：0为Guest,1为Observer,2为Senior Observer*/
	m_nRole=0;	
	/** 失效模式：0表示密码永不失效，1表示超过指定时间失效 */
	m_nExpiredMode = 0;	
	/** 密码过期处理方式: 0：不自动删除，1为删除 */
	m_bAutoDelete = 0;
	/** 会话ID*/
	m_dwSessionID = 0;
	/** 当前状态，0:不在线；1：在线*/
	m_nState = USERINFO_STATE_OFFLINE;
	/** 当前登录的客户机IP地址*/
	m_dwClientIP = 0x0;
	/** 当前所使用的应用程序*/
	m_nApplication=0;

	/** 密码过期时间 */
	GetLocalTime(&m_tmExpiryDate);
	/** 注册时间 */
	GetLocalTime(&m_tmRegister);
	/** 上次登录时间 */
	GetLocalTime(&m_tmLastUsed);
}

CUserInfo::~CUserInfo(void)
{
}

/**
 * @brief 得到用户信息的字节长度。
 * @note  得到用户所有属性的字节长度
 * @param void
 * @return int,字节大小
 */
int   CUserInfo::GetSizeOfBytes(void)
{
	int nSize=0;
	nSize = m_strUserName.GetLength()*sizeof(TCHAR)+2;
	nSize+=m_strPassword.GetLength()*sizeof(TCHAR)+2;
	nSize+=64;
	return nSize;
}
/**
 * @brief 将用户信息写入缓冲区。
 * @note  将用户信息写入到对应的缓冲区中 。
 * @param unsigned char* pBuf，缓冲区指针
 * @param int nSize，缓冲区大小
 * @return int,实际占用缓冲区大小
 */
int   CUserInfo::Write(unsigned char* pBuf,int nSize)
{	
	if(nSize<GetSizeOfBytes())
		return 0;
	int   nLen = 0;		// 当前写缓冲区的位置
	int   nTemp;
	// 用户名
	nTemp = m_strUserName.GetLength()*sizeof(TCHAR);
	memcpy_s(pBuf+nLen,nTemp, LPCTSTR(m_strUserName),nTemp);	
	pBuf[nLen+nTemp]='\0';
	pBuf[nLen+nTemp+1]='\0';
	nLen += nTemp+2;
	// 密码
	nTemp = m_strPassword.GetLength()*sizeof(TCHAR);
	memcpy_s(pBuf+nLen,nTemp, LPCTSTR(m_strPassword),nTemp);	
	pBuf[nLen+nTemp]='\0';
	pBuf[nLen+nTemp+1]='\0';
	nLen += nTemp+2;
	// 角色
	memcpy_s(pBuf+nLen,sizeof(int),&m_nRole,sizeof(int));
	nLen+=sizeof(int);
	// 失效模式
	memcpy_s(pBuf+nLen,sizeof(int),&m_nExpiredMode,sizeof(int));
	nLen+=sizeof(int);
	// 是否自动删除
	memcpy_s(pBuf+nLen,sizeof(BOOL),&m_bAutoDelete,sizeof(BOOL));
	nLen+=sizeof(BOOL);
	//  会话ID
	memcpy_s(pBuf+nLen,sizeof(int),&m_dwSessionID,sizeof(int));
	nLen+=sizeof(int);
	//  当前状态
	memcpy_s(pBuf+nLen,sizeof(int),&m_nState,sizeof(int));
	nLen+=sizeof(int);

	// 当前登录的客户机IP地址
	memcpy_s(pBuf+nLen,sizeof(int),&m_dwClientIP,sizeof(int));
	nLen+=sizeof(int);
	// 当前所使用的应用程序
	memcpy_s(pBuf+nLen,sizeof(int),&m_nApplication,sizeof(int));
	nLen+=sizeof(int);

	// 注册时间
	pBuf[nLen++] = LOBYTE(m_tmRegister.wYear);
	pBuf[nLen++] = HIBYTE(m_tmRegister.wYear);
	pBuf[nLen++] = LOBYTE(m_tmRegister.wMonth);
	pBuf[nLen++] = HIBYTE(m_tmRegister.wMonth);
	pBuf[nLen++] = LOBYTE(m_tmRegister.wDay);
	pBuf[nLen++] = HIBYTE(m_tmRegister.wDay);
	pBuf[nLen++] = LOBYTE(m_tmRegister.wHour);
	pBuf[nLen++] = HIBYTE(m_tmRegister.wHour);
	pBuf[nLen++] = LOBYTE(m_tmRegister.wMinute);
	pBuf[nLen++] = HIBYTE(m_tmRegister.wMinute);
	pBuf[nLen++] = LOBYTE(m_tmRegister.wSecond);
	pBuf[nLen++] = HIBYTE(m_tmRegister.wSecond);	
	// 密码过期时间
	pBuf[nLen++] = LOBYTE(m_tmExpiryDate.wYear);
	pBuf[nLen++] = HIBYTE(m_tmExpiryDate.wYear);
	pBuf[nLen++] = LOBYTE(m_tmExpiryDate.wMonth);
	pBuf[nLen++] = HIBYTE(m_tmExpiryDate.wMonth);
	pBuf[nLen++] = LOBYTE(m_tmExpiryDate.wDay);
	pBuf[nLen++] = HIBYTE(m_tmExpiryDate.wDay);
	pBuf[nLen++] = LOBYTE(m_tmExpiryDate.wHour);
	pBuf[nLen++] = HIBYTE(m_tmExpiryDate.wHour);
	pBuf[nLen++] = LOBYTE(m_tmExpiryDate.wMinute);
	pBuf[nLen++] = HIBYTE(m_tmExpiryDate.wMinute);
	pBuf[nLen++] = LOBYTE(m_tmExpiryDate.wSecond);
	pBuf[nLen++] = HIBYTE(m_tmExpiryDate.wSecond);
	/** 上次登录时间 */
	pBuf[nLen++] = LOBYTE(m_tmLastUsed.wYear);
	pBuf[nLen++] = HIBYTE(m_tmLastUsed.wYear);
	pBuf[nLen++] = LOBYTE(m_tmLastUsed.wMonth);
	pBuf[nLen++] = HIBYTE(m_tmLastUsed.wMonth);
	pBuf[nLen++] = LOBYTE(m_tmLastUsed.wDay);
	pBuf[nLen++] = HIBYTE(m_tmLastUsed.wDay);
	pBuf[nLen++] = LOBYTE(m_tmLastUsed.wHour);
	pBuf[nLen++] = HIBYTE(m_tmLastUsed.wHour);
	pBuf[nLen++] = LOBYTE(m_tmLastUsed.wMinute);
	pBuf[nLen++] = HIBYTE(m_tmLastUsed.wMinute);
	pBuf[nLen++] = LOBYTE(m_tmLastUsed.wSecond);
	pBuf[nLen++] = HIBYTE(m_tmLastUsed.wSecond);
	
	return nLen;
}
/**
 * @brief 从缓冲区读出用户信息。
 * @note  与Write函数对应，从缓冲区中读出用户信息。
 * @param unsigned char* pBuf，缓冲区指针
 * @param int nSize，缓冲区大小
 * @return int,实际占用缓冲区大小
 */
int   CUserInfo::Read(unsigned char* pBuf,int nSize)
{	
	if(nSize<GetSizeOfBytes())
		return 0 ;
	
	int nLen = 0;		// 当前读缓冲区的位置
	int nTemp;
	// 用户名
	m_strUserName = LPCTSTR(pBuf+nLen);
	nTemp = m_strUserName.GetLength()*sizeof(TCHAR)+2;
	nLen+=nTemp;
	//密码
	m_strPassword = LPCTSTR(pBuf+nLen);
	nTemp = m_strPassword.GetLength()*sizeof(TCHAR)+2;
	nLen+=nTemp;
	// 角色
	memcpy_s(&m_nRole,sizeof(int),pBuf+nLen,sizeof(int));
	nLen+=sizeof(int);
	// 失效模式
	memcpy_s(&m_nExpiredMode,sizeof(int),pBuf+nLen,sizeof(int));
	nLen+=sizeof(int);
	// 是否自动删除
	memcpy_s(&m_bAutoDelete,sizeof(BOOL),pBuf+nLen,sizeof(BOOL));
	nLen+=sizeof(BOOL);
	//  会话ID
	memcpy_s(&m_dwSessionID,sizeof(int),pBuf+nLen,sizeof(int));
	nLen+=sizeof(int);
	//  当前状态
	memcpy_s(&m_nState,sizeof(int),pBuf+nLen,sizeof(int));
	nLen+=sizeof(int);

	// 当前登录的客户机IP地址
	memcpy_s(&m_dwClientIP,sizeof(int),pBuf+nLen,sizeof(int));
	nLen+=sizeof(int);
	// 当前所使用的应用程序
	memcpy_s(&m_nApplication,sizeof(int),pBuf+nLen,sizeof(int));
	nLen+=sizeof(int);

	// 注册时间
	m_tmRegister.wYear = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmRegister.wMonth = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmRegister.wDay = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmRegister.wHour = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmRegister.wMinute = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmRegister.wSecond = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;	
	// 密码过期时间
	m_tmExpiryDate.wYear = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmExpiryDate.wMonth = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmExpiryDate.wDay = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmExpiryDate.wHour = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmExpiryDate.wMinute = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmExpiryDate.wSecond = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;	
	// 上次登录时间 
	m_tmLastUsed.wYear = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmLastUsed.wMonth = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmLastUsed.wDay = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmLastUsed.wHour = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmLastUsed.wMinute = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_tmLastUsed.wSecond = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;	
	return nLen;
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////   CUserInfos    ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
CUserInfos::CUserInfos(void)
: m_dwSession(0)
{
}

CUserInfos::~CUserInfos(void)
{
	RemoveAll();
}
/**
 * @brief 删除所有的用户对象
 * @note  从动态数组中释放所有的用户对象，并清空数组。
 * @param void
 * @param void
 * @return void
 */
void    CUserInfos::RemoveAll(void)
{
	int i,nCount = m_arrUserInfo.GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_arrUserInfo[i])
		{
			delete m_arrUserInfo[i];
			m_arrUserInfo[i] = NULL;
		}	
	}
	m_arrUserInfo.RemoveAll();
}
/**
 * @brief 删除单个的用户对象
 * @note  从动态数组中释放用户对象，删除指定位置的用户信息。
 * @param int nIndex,删除单个用户信息
 * @return void
 */
void    CUserInfos::RemoveAt(int nIndex)
{
	CUserInfo* pUser = GetUserInfo(nIndex);
	if(!pUser)
		return;
	delete pUser;
	m_arrUserInfo.RemoveAt(nIndex);
}
// 得到指定索引的对象，索引从0开始
CUserInfo* CUserInfos::GetUserInfo(int iIndex)
{
	int nCount = GetCount();
	if(iIndex>=nCount)
		return NULL;
	return m_arrUserInfo[iIndex];
}
/**
 * @brief 计算在线用户的总数
 * @note  计算在线用户的总数
 * @param void
 * @param void
 * @return 返回在线用户的总数
 */
int	CUserInfos::GetAliveCount(void)	
{
	CUserInfo* pUser = NULL;
	int  nAliveCount = 0;
	int  nCount = GetCount();
	for (int i=0;i<nCount;i++)
	{
		pUser = GetUserInfo(i);
		if(!pUser)
			continue;
		if(pUser->m_nState == USERINFO_STATE_ALINE)
			nAliveCount++;
	}
	return nAliveCount;
}
/**
 * @brief 添加一个对象
 * @note  向动态数组中增加一个用户对象
 * @param void
 * @param void
 * @return 返回在数组中的位置
 */
int CUserInfos::Add(CUserInfo* pUserInfo)
{
	return m_arrUserInfo.Add(pUserInfo);
}
/**
 * @brief  通过用户名查找用户信息
 * @note   通过用户名查找用户信息
 * @param void
 * @param void
 * @return 找到用户则返回在数组中的位置；没有找到对应的用户则返回NULL
 */
CUserInfo* CUserInfos::FindUserByName(LPCTSTR strUserName)
{
	int i,nCount = GetCount();
	for (i=0;i<nCount;i++)
	{
		if(m_arrUserInfo[i]->m_strUserName.CompareNoCase(strUserName)==0)
			return m_arrUserInfo[i];
	}
	return NULL;

}

/**
 * @brief 验证用户
 * @note  根据用户密码验证用户
 * @param LPCTSTR strName,
 * @param LPCTSTR strPassword
 * @return 验证成功则返回该用户对象的指针，失败则返回NULL
 */
CUserInfo*	CUserInfos::FindUserByNameAndPassWord(LPCTSTR strName,LPCTSTR strPassword)
{
	int i,nCount = GetCount();
	CUserInfo* pUserInfo =NULL;
	for (i=0;i<nCount;i++)
	{
		pUserInfo= FindUserByName(strName);
		if (pUserInfo && pUserInfo->m_strPassword == strPassword)
		{
			return pUserInfo;
		}
	}
	return NULL;
}