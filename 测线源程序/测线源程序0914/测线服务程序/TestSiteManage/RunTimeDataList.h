#pragma once

#include "RunTimeData.h"

/**
*@brief 运行日志信息数据队列类
*/
class CRunTimeDataList
{
public:
	CRunTimeDataList();
	~CRunTimeDataList();

public:	// 属性
	/** 日志文件记录要求，记录运行日志*/
	bool m_bLogRequestLog;
	/** 日志文件记录要求，记录错误信息*/
	bool m_bLogRequestError;
	/** 日志文件记录要求，记录调试信息*/
	bool m_bLogRequestDebug;
	/** 日志文件记录要求，记录其它信息*/
	bool m_bLogRequestElse;
	/** 队列最大记录数*/
	int m_iRecordCountMax;
	/** 运行信息队列*/
	CList<CRunTimeData, CRunTimeData> m_olsRunTimeData;
	/** 线程同步对象*/
	CCriticalSection m_oCriticalSectionLog;	

	CRunTimeData* m_pRunTimeData;

public:	// 方法
	// 设置运行状态日志数据
	void Set(int iType, CString strObject, CString strPosition, CString strDesc);
	// 得到运行状态日志数据
	CString Get();
};
