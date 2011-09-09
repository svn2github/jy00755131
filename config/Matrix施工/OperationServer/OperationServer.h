// OperationServer.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

//////////////////////////////////////////////////////////////
// SEGD文件编号初始值
#define  SEGDFILENUMBER_INIT	800

// COperationServerApp:
// 有关此类的实现，请参阅 OperationServer.cpp
//
#include "..\common\SocketParam.h"
#include "..\common\OperaStruct.h"
#include "..\common\XMLFileNameParam.h"
#include "..\common\ChannelList.h"
#include "..\common\CLogBase.h"
class COperationServerApp : public CWinApp
{
public:
	COperationServerApp();

// 属性
public:
	// 当前主机的IP地址
	DWORD			 m_dwHostIP;
	// 当前服务器的IP地址，与m_dwHostIP含义相同，仅仅是类型不同
	CString          m_strHostIP;
	// 主交叉站IP，是指物理的真实IP地址
	DWORD		     m_dwMainCrossIP;
	/** 本地XML配置文件（含路径） */
	CString			 m_strLocalXMLFile;
	// 主交叉站端口
	WORD		     m_wMainCrossPort;
	// 采样数据文件存盘路径
	// CString			 m_strFilePath;
	CChannelList	m_ChannelList;
// 属性
private:
	// 对测网系统时间读写同步对象
	CCriticalSection m_critFDUTime;
	// 测网系统时间
	DWORD			 m_dwFDUTime;
	// SEGD文件编号，四组采集线程每次读该变量获得SEGD文件编号
	// 如果存盘的目录存在，则文件编号每次加1，如果存盘目录不存在，则文件编号清零
	DWORD				m_dwFileNumber;
	// SEGD文件编号读写同步对象
	CCriticalSection	m_critFileNumber;
// 重写
public:
	virtual BOOL InitInstance();
	// 设置采集系统本地时间
	void	SetFDULocalTime(DWORD dwFDUTime)
	{
		m_critFDUTime.Lock();
		m_dwFDUTime = dwFDUTime;
		m_critFDUTime.Unlock();
	}
	// 获得采集系统本地时间
	DWORD	GetFDULocalTime(void)
	{
		DWORD  dwFDUTime;
		m_critFDUTime.Lock();
		dwFDUTime = m_dwFDUTime;
		m_critFDUTime.Unlock();
		return dwFDUTime;
	}
/**
 * @brief 获得SEGD文件编号的全局函数
 * @note  获得最新的SEGD文件编号，并将全局文件编号变量加一。
 * @param  void
 * @return SEGD文件编号。
 */
DWORD  GetGlobalFileNumber(BOOL bRefreshed = TRUE)
{
	DWORD   dwTemp;
	m_critFileNumber.Lock();
	dwTemp = m_dwFileNumber;
	if(bRefreshed)
		m_dwFileNumber++;
	m_critFileNumber.Unlock();
	return dwTemp;
}
/**
 * @brief 设置SEGD文件编号编号初值
 * @note  设置SEGD文件编号编号初值。
 * @param  DWORD dwNumber，SEGD文件编号
 * @return void
 */
void SetGlobalFileNumber(DWORD dwNumber)
{
	m_critFileNumber.Lock();
	m_dwFileNumber = dwNumber;
	m_critFileNumber.Unlock();
}
// 实现
public:
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	BOOL ParseXMLFile();
};

extern COperationServerApp theApp;