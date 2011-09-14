#pragma once

/**
*@brief 运行日志信息数据类
*/
class CRunTimeData
{
public:
	CRunTimeData();
	~CRunTimeData();

public:	// 属性
	/** 日期*/
	CString m_strDate;
	/** 时间*/
	CString m_strTime;
	/** 类型	1-运行日志；2-异常；3-调试；4-其它*/
	CString m_strType;
	/** 对象*/
	CString m_strObject;
	/** 位置*/
	CString m_strPosition;
	/** 描述*/
	CString m_strDesc;

public:	// 方法
	// 设置运行状态数据
	void Set(int iType, CString strObject, CString strPosition, CString strDesc);
	// 得到运行状态数据
	CString Get();
};
