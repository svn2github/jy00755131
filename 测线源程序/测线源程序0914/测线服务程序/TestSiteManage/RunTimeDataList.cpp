#include "StdAfx.h"
#include "RunTimeDataList.h"

CRunTimeDataList::CRunTimeDataList()
{
	m_iRecordCountMax = 1000;	// 队列最大记录数

	m_bLogRequestLog = true;	// 日志文件记录要求，记录运行日志
	m_bLogRequestError = true;	// 日志文件记录要求，记录错误信息
	m_bLogRequestDebug = true;	// 日志文件记录要求，记录调试信息
	m_bLogRequestElse = true;	// 日志文件记录要求，记录其它信息
}

CRunTimeDataList::~CRunTimeDataList()
{
}

/**
* 设置运行状态日志数据，加入运行信息队列
* @param int iType 类型	1-运行日志；2-异常；3-调试；4-其它
* @param CString strObject  对象
* @param CString strPosition 位置
* @param CString strDesc 描述
* @return void
*/
void CRunTimeDataList::Set(int iType, CString strObject, CString strPosition, CString strDesc)
{
	m_oCriticalSectionLog.Lock();
		if(m_olsRunTimeData.GetCount() < m_iRecordCountMax)	// 队列最大记录数
		{
			if(((true == m_bLogRequestLog) && (1 == iType))
				|| ((true == m_bLogRequestError) && (2 == iType))
				|| ((true == m_bLogRequestDebug) && (3 == iType))
				|| ((true == m_bLogRequestElse) && (4 == iType)))
			{
				CRunTimeData oRunTimeData;
				oRunTimeData.Set(iType, strObject, strPosition, strDesc);
				m_olsRunTimeData.AddTail(oRunTimeData);	// 运行信息队列
			}
		}
	m_oCriticalSectionLog.Unlock();
}

/**
* 得到运行状态日志数据
* @param void
* @return CString 运行状态日志数据 ""：无数据
*/
CString CRunTimeDataList::Get()
{
	m_oCriticalSectionLog.Lock();
		CString strData = "";	
		if(m_olsRunTimeData.GetCount() > 0)
		{
			CRunTimeData oRunTimeData = m_olsRunTimeData.RemoveHead();
			strData = oRunTimeData.Get();
		}
	m_oCriticalSectionLog.Unlock();
	return strData;
}