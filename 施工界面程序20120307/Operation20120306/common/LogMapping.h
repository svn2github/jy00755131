#pragma once

#include "CLogBase.h"
///////////////////////////////////////////////////////////////////////////////////////////////
// 日志内存区中共享配置参数区的字节大小
#define     MEMMAP_CONFIGPARAMSIZE	1024
///////////////////////////////////////////////////////////////////////////////////////////////
// 日志内存映射区保存的日志记录最大数量。每条日志的最大长度为 SOCKET_LOGFRM_MAXLEN 宏定义
#define		LOGRECORD_MAXNUM		256
//////////////////////////////////////////////////////////////////////////
// 共享内存映射区的全局名称
#define     LOGMEMMAPPING_NAME		(_T("日志内存_MATRIX2011"))
//////////////////////////////////////////////////////////////////////////
// 共享内存区同步对象的全局名称
#define     LOGMEMMUTEX_NAME		(_T("日志内存同步_MATRIX2011"))
/**
 * @class CLogMapping
 * @brief 
                ______                     __________________              _____________ 
               |_____1024字节共享配置参数区______|___________共享日志记录区_____________|

 * @brief 日志内存映射区，共享内存区分为两部分：共享配置参数区和日志记录固定长度，
 假设认为每条日志不超过800字节，允许最大 LOGRECORD_MAXNUM 个日志记录。
 共享配置参数区的大小为1024字节。
 * @brief 
          共享配置参数区配置的参数依次为：
		  客户机IP地址（4字节整数）+ 管理服务器(运行AdminServer程序的计算机)IP地址(4字节整数)
 *
 * 
 */
class CLogMapping
{
public:
	CLogMapping(void);
	~CLogMapping(void);

protected:	
	// 自定义更新消息
	//UINT	  m_dwMessage;
	// 多进程访问同步对象
	CMutex*   m_pMutex;
	// 内存映射区全局名称
	// CString   m_strMemName;
	//内存映射区句柄
	HANDLE    m_hMemMap;

	// 指向内存映射区首地址
	BYTE*     m_pMemData;	
	// 指向共享日志数据区的首地址
	BYTE*	  m_pLogData;	
	// 内存映射区字节大小
	ULONG     m_dwSize;
		
	/** 写日志记录，从0开始，超过最大后清0*/
	int       m_nWriteIndex;

public:
	static DWORD	StringIPToDWORDIP(LPCTSTR strHostIP);
	static void     DWORDIPToStringIP(DWORD dwIP,CString&  strIP);
	// 创建内存映射区
	bool Create(void);
	/// 打开内存映射区
	bool Open(void);
	// 释放内存映射区
	bool Destroy(void);	
	/// 写一条日志记录
	int WriteRecord(CLogBase* pLogRecord);
	/// 读指定位置日志记录
	bool ReadRecord(int nIndex,CLogBase* pLogRecord);

	/// 读运行Start程序的客户机IP地址
	DWORD	ReadClientHostIP(void);
	/// 写运行Start程序的客户机IP地址
	bool	WriteClientHostIP(DWORD    dwIP);
	bool	WriteClientHostIP(LPCTSTR  strIP);
	/// 读运行AdminServer程序的服务器IP地址
	DWORD	ReadAdminServerHostIP(void);
	/// 写运行AdminServer程序的服务器IP地址	
	bool	WriteAdminServerHostIP(DWORD    dwIP);
	bool	WriteAdminServerHostIP(LPCTSTR  strIP);
	/// 读运行FTP的服务器IP地址
	DWORD	ReadFTPServerHostIP(void);
	/// 写运行FTP服务器IP地址	
	bool	WriteFTPServerHostIP(DWORD    dwIP);
	bool	WriteFTPServerHostIP(LPCTSTR  strIP);
};
