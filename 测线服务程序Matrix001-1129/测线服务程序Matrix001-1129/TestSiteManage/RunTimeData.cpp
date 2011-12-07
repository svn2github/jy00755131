#include "StdAfx.h"
#include "RunTimeData.h"

CRunTimeData::CRunTimeData()
{
}

CRunTimeData::~CRunTimeData()
{
}

/**
* 设置运行状态日志数据
* @param int iType 类型	1-运行日志；2-异常；3-调试；4-其它
* @param CString strObject  对象
* @param CString strPosition 位置
* @param CString strDesc 描述
* @return void
*/
void CRunTimeData::Set(int iType, CString strObject, CString strPosition, CString strDesc)
{
	SYSTEMTIME oSystemTime;
	GetLocalTime(&oSystemTime);
	m_strDate.Format("%d-%d-%d", oSystemTime.wYear, oSystemTime.wMonth, oSystemTime.wDay);	// 日期
	m_strTime.Format("%d:%d:%d.%d", oSystemTime.wHour, oSystemTime.wMinute, oSystemTime.wSecond, oSystemTime.wMilliseconds);	// 时间
	if(1 == iType)	// 类型	1-运行日志；2-异常；3-调试；4-其它
	{
		m_strType = "Log";
	}
	else if(2 == iType)	// 类型	1-运行日志；2-异常；3-调试；4-其它
	{
		m_strType = "Error";
	}
	else if(3 == iType)	// 类型	1-运行日志；2-异常；3-调试；4-其它
	{
		m_strType = "Debug";
	}
	else if(4 == iType)	// 类型	1-运行日志；2-异常；3-调试；4-其它
	{
		m_strType = "Else";
	}
	m_strObject = strObject;	// 对象
	m_strPosition = strPosition;	// 位置 
	m_strDesc = strDesc;	// 描述
}

/**
* 得到运行状态日志数据
* @param void
* @return CString 运行状态日志数据
*/
CString CRunTimeData::Get()
{
	CString strData = "";
	strData.Format("<Log Date=\"%s\" Time=\"%s\" Type=\"%s\" Object=\"%s\" Position=\"%s\" strDesc=\"%s\"/>\r\n",
		m_strDate, m_strTime, m_strType, m_strObject, m_strPosition, m_strDesc);
	return strData;
}