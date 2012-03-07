#include "StdAfx.h"
#include "CLogbase.h"

UINT WM_NEWLOG = RegisterWindowMessage(MSG_LOG_NEW);
/**
 * @brief CLogBase构造函数
 */
CLogBase::CLogBase(void)
{
		
}
/**
 * @brief CLogBase析构函数
 */
CLogBase::~CLogBase(void)
{

}
void CLogBase::SetHostAndProgram(LPCTSTR strIP,DWORD dwProgramNum)
{
	USES_CONVERSION;
	LPSTR  str = T2A(strIP);
	unsigned long dwIP = inet_addr(str);
	SetHostAndProgram(dwIP,dwProgramNum);
}
/** 
 * @brief 设置日志信息
 * @note  设置一条日志的信息：位置、描述、类型、是否显示给用户等
 * @param LPCTSTR strPosition,日志产生的位置
 * @param LPCTSTR strDesc,日志描述
 * @param WORD wType,日志类型：正常、警告、异常、调试及其他
 * @param BOOL bShowed，是否显示给用户
 * @return void
*/
void  CLogBase::SetLog(LPCTSTR strPosition,LPCTSTR strDesc,WORD wType,BOOL bShowed)
{	
	// 生成日志信息的时间	
	GetLocalTime(&m_sysTime);	
	// 位置
	_tcscpy_s(m_strPosition,_countof(m_strPosition),strPosition);
	// 说明
	_tcscpy_s(m_strDescription,_countof(m_strDescription),strDesc);
	// 类型
	m_wType = wType;
	m_bShowed = bShowed;
}
/** 
 * @brief 生成用于显示的字符串
 * @note  生成用于显示的字符串，仅包含时间和描述
 * @param CString& strLog,输入输出参数，生成一条文本的日志记录
 * @return void
*/
void  CLogBase::GetShowString(CString& strLog)
{
	strLog.Format(_T("%02d:%02d:%02d [%d] %s"),m_sysTime.wHour,m_sysTime.wMinute,m_sysTime.wSecond,m_dwProgramNum,m_strDescription);
}
/** 
 * @brief 生成日志记录文本
 * @note  将所有的日志按照固定格式组合，生成一条文本的日志记录
 * @param CString& strLog,输入输出参数，生成一条文本的日志记录
 * @return void
*/
void  CLogBase::GetAllAttributes(CString& strLog)
{
	CString   strTemp;
	strLog.Format(_T("%04d-%02d-%02d\t%02d:%02d:%02d\t%02d\t"),
		m_sysTime.wYear,m_sysTime.wMonth,m_sysTime.wDay,m_sysTime.wHour,m_sysTime.wMinute,m_sysTime.wSecond,m_wType);
	if(m_bShowed)
	{
		strLog+=_T("true\t");
	}
	else
	{
		strLog+=_T("false\t");
	}
	strTemp.Format(_T("0x%08x\t"),m_dwHostIP);
	strLog+=strTemp;
	strTemp.Format(_T("0x%08x\t"),m_dwProgramNum);
	strLog+=strTemp;
	strTemp.Format(_T("%s\t\t"),m_strPosition);
	strLog+=strTemp;
	strLog+=m_strDescription;
}
/** 
 * @brief 计算日志网络帧的字节数
 * @note  计算发送一条日志记录的网络帧字节大小，在组帧之前调用该函数，获取字节大小，再动态分配缓冲区。
 * @param void
 * @return int ,日志网络帧字节数
*/
int CLogBase::GetFrameByteSize(void)
{
	int nSize = GetObjectByteSize();
	// 16为网络帧帧头、帧长等字节数
	return 16 + nSize;
}
/** 
 * @brief 写入缓冲区
 * @note 将日志信息按照写入缓冲区
 * @param BYTE* pBuf,数据缓冲区
 * @return int ,返回字节长度
*/
int CLogBase::WriteToBuf(BYTE* pBuf)
{
	if(!pBuf)
		return 0;
	// 时间信息
	int nLen = 0;
	pBuf[nLen++] = LOBYTE(m_sysTime.wYear);
	pBuf[nLen++] = HIBYTE(m_sysTime.wYear);
	pBuf[nLen++] = LOBYTE(m_sysTime.wMonth);
	pBuf[nLen++] = HIBYTE(m_sysTime.wMonth);
	pBuf[nLen++] = LOBYTE(m_sysTime.wDay);
	pBuf[nLen++] = HIBYTE(m_sysTime.wDay);
	pBuf[nLen++] = LOBYTE(m_sysTime.wHour);
	pBuf[nLen++] = HIBYTE(m_sysTime.wHour);
	pBuf[nLen++] = LOBYTE(m_sysTime.wMinute);
	pBuf[nLen++] = HIBYTE(m_sysTime.wMinute);
	pBuf[nLen++] = LOBYTE(m_sysTime.wSecond);
	pBuf[nLen++] = HIBYTE(m_sysTime.wSecond);
	// 类型
	pBuf[nLen++] = LOBYTE(m_wType);
	pBuf[nLen++] = HIBYTE(m_wType);
	memcpy_s(pBuf+nLen,sizeof(BOOL),&m_bShowed,sizeof(BOOL));
	nLen+=sizeof(BOOL);
	// 源计算机IP地址
	memcpy_s(pBuf+nLen,sizeof(DWORD),&m_dwHostIP,sizeof(DWORD));
	nLen+=sizeof(DWORD);
	// 产生日志的程序代号
	memcpy_s(pBuf+nLen,sizeof(DWORD),&m_dwProgramNum,sizeof(DWORD));
	nLen+=sizeof(DWORD);
	// 源程序位置说明，先转为wchar，再发送
	LPWSTR		wstr = T2W(m_strPosition);
	memcpy_s(pBuf+nLen,_countof(m_strPosition)*2,wstr,_countof(m_strPosition)*2);
	nLen+=_countof(m_strPosition)*2;
	// 日志信息，先转为wchar，再发送
	LPWSTR		wstrDesc = T2W(m_strDescription);
	memcpy_s(pBuf+nLen,_countof(m_strDescription)*2,wstrDesc,_countof(m_strDescription)*2);
	nLen+=_countof(m_strDescription)*2;
	return nLen;
}
/** 
 * @brief 写入日志网络帧
 * @note 将日志信息按照日志网络通信协议，写入缓冲区，形成日志网络帧。
 * @param BYTE* pBuf
 * @return int ,返回字节长度
*/
int CLogBase::WriteToSocketFrame(BYTE* pBuf)
{
	if(!pBuf)
		return 0;
	// int     nLen;
	WORD	wFrmLen;
	// 帧头
	pBuf[0]= 0xEB;
	pBuf[1]= 0x90;
	// 帧长
	wFrmLen = (WORD)GetFrameByteSize();
	pBuf[2] = LOBYTE(wFrmLen);
	pBuf[3] = HIBYTE(wFrmLen);
	// 帧计数
	memset(pBuf+4,0,4);
	// 帧类型	
	if(m_dwProgramNum==MATRIX_ADMINSERVER)
		pBuf[8] = 0xFF;
	else
		pBuf[8] = 0x01;	
	// 命令码
	pBuf[9] = 0xFF;
	pBuf[10] = 0xFF;
	// 数据个数
	pBuf[11]= 1;
	pBuf[12]= 0;
	// 写入缓冲区
	WriteToBuf(pBuf+13);
	
	// 帧校验
	pBuf[wFrmLen-3]= 0x00;
	// 帧尾
	pBuf[wFrmLen-2]= 0x14;
	pBuf[wFrmLen-1]= 0x6F;	
	return wFrmLen;
}
/** 
 * @brief 从缓冲区读出属性
 * @note 从缓冲区读出属性
 * @param BYTE* pBuf，指向日志对象属性缓冲区
 * @return int ,返回字节长度
*/
int CLogBase::ReadFromBuf(BYTE* pBuf)
{
	// 日志属性
	// 时间信息
	int nLen = 0;
	m_sysTime.wYear = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_sysTime.wMonth = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_sysTime.wDay = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_sysTime.wHour = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_sysTime.wMinute = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;
	m_sysTime.wSecond = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;		
	// 类型
	m_wType = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);
	nLen+=2;	
	// 是否显示
	memcpy_s(&m_bShowed,sizeof(BOOL),pBuf+nLen,sizeof(BOOL));
	nLen+=sizeof(BOOL);
	// 源计算机IP地址
	memcpy_s(&m_dwHostIP,sizeof(DWORD),pBuf+nLen,sizeof(DWORD));
	nLen+=sizeof(DWORD);
	// 产生日志的程序代号
	memcpy_s(&m_dwProgramNum,sizeof(DWORD),pBuf+nLen,sizeof(DWORD));
	nLen+=sizeof(DWORD);
	// 源程序位置说明，接收的字符串为wchar，先转化为TCHAR类型，再处理
	LPTSTR		strPos = W2T(LPTSTR(pBuf+nLen));
	memcpy_s(m_strPosition,_countof(m_strPosition)*sizeof(TCHAR),strPos,_countof(m_strPosition)*sizeof(TCHAR));
	nLen+=_countof(m_strPosition)*2;
	// 日志信息，接收的字符串为wchar，先转化为TCHAR类型，再处理
	LPTSTR		strDesc = W2T(LPTSTR(pBuf+nLen));
	memcpy_s(m_strDescription,_countof(m_strDescription)*sizeof(TCHAR),strDesc,_countof(m_strDescription)*sizeof(TCHAR));
	nLen+=_countof(m_strDescription)*2;		
	return nLen+3;
}
/** 
 * @brief 从日志网络帧读出日志对象
 * @note 从日志网络帧读出日志对象
 * @param BYTE* pBuf，指向日志网络帧的地址
 * @return int ,返回字节长度
*/
int CLogBase::ReadFromSocketFrame(BYTE* pBuf)
{
	if(!pBuf)
		return 0;	
	// 日志属性,在网络帧中保存日志属性的数据区从第十三个字节开始
	return ReadFromBuf(pBuf+13);	
}

