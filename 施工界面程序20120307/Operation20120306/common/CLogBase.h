#pragma once

#include "MatrixDef.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
// 用于传送一条日志记录帧最大长度，同时也是日志内存映射区保存一条日志记录的最大长度
#define SOCKET_LOGFRM_MAXLEN		832
extern UINT WM_NEWLOG;
//////////////////////////////////////////////////////////////////////////
// 自定义的日志更新消息
#define		MSG_LOG_NEW				(_T("新日志消息"))
//////////////////////////////////////////////////////////////////////////
// 日志类型
#define     LOGTYPE_NORMAL			0	// 正常信息，一般显示给用户
#define     LOGTYPE_WARNING			1	// 警告信息，可以显示也可以不显示
#define     LOGTYPE_ERROR			2	// 错误信息，可以显示也可以不显示
#define     LOGTYPE_DEBUG			3	// 调试信息，一般不显示，记录程序运行状态
#define     LOGTYPE_OTHER			4	// 其他信息

/**
 * @class CLogBase
 * @brief 日志基类
 *
 * @note 一条程序运行日志的基类。所有的程序都采用相同的日志基类。
 * 
 */
class CLogBase
{
	// 信息
private:	
	/** 产生日志的时间 */
	SYSTEMTIME	m_sysTime;	
	/** 日志的类型 : 正常、警告、异常、调试、其他*/
	WORD		m_wType;
	/** 是否显示给用户 */
	BOOL		m_bShowed;
	/** 产生日志的源计算机IP地址 */
	DWORD		m_dwHostIP;
	/** 产生日志的程序代号*/
	DWORD		m_dwProgramNum;
	/** 产生日志的源程序位置说明 */
	TCHAR		m_strPosition[128];
	/** 日志信息 */
	TCHAR		m_strDescription[256];
public:
	CLogBase(void);
	~CLogBase(void);
	/// 是否需要显示给用户
	BOOL  IsShowed(void)	{ return m_bShowed; };
	///  得到显示的字符串
	void  GetShowString(CString& strLog);
	/// 得到日志记录
	virtual void  GetAllAttributes(CString& strLog);
	/// 设置日志信息
	void  SetHostAndProgram(DWORD dwIP,DWORD dwProgramNum)	{ m_dwHostIP = dwIP; m_dwProgramNum = dwProgramNum;};
	/// 设置日志信息
	void  SetHostAndProgram(LPCTSTR strIP,DWORD dwProgramNum);
	/// 设置日志信息
	void  SetLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType=LOGTYPE_NORMAL,BOOL bShowed=FALSE);
	/// 得到对象属性的字节大小
	int	 GetObjectByteSize(void)	{return 18+sizeof(BOOL)+sizeof(DWORD)*2
										+_countof(m_strPosition)*2+_countof(m_strDescription)*2;};
	/// 传送一条日志的网络帧所需的字节大小
	int GetFrameByteSize(void);
	/// 写入缓冲区
	int WriteToBuf(BYTE* pBuf);
	/// 从缓冲区读出属性
	int ReadFromBuf(BYTE* pBuf);
	/// 组成网络帧
	int WriteToSocketFrame(BYTE* pBuf);
	/// 从网络帧中读出对象属性
	int ReadFromSocketFrame(BYTE* pBuf);
};


