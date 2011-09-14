#include "StdAfx.h"

#include "InstrumentList.h"


CInstrumentList::CInstrumentList(void)
{
}

CInstrumentList::~CInstrumentList(void)
{
}

// 初始化
void CInstrumentList::OnInit()
{	
	m_pArrayInstrument = new CInstrument[m_uiCountAll];	// 生成仪器数组	
	m_uiCountFree = m_uiCountAll;	// 空闲仪器数量	
	m_uiCountUsed = 0;	// 使用中仪器数量
	m_pFirstMainCross = NULL;	// 首个主交叉站指针
}
// 关闭
void CInstrumentList::OnClose()
{
	// 删除仪器数组
	delete[] m_pArrayInstrument;
	m_oSNInstrumentMap.OnClose();	// SN仪器索引表	
	m_oIPInstrumentMap.OnClose();	// IP地址仪器索引表
}

// 重置
void CInstrumentList::OnReset()
{	
	m_uiCountFree = m_uiCountAll;	// 空闲仪器数量	
	m_uiCountUsed = 0;	// 使用中仪器数量
	m_pFirstMainCross = NULL;	// 首个主交叉站指针
	m_oSNInstrumentMap.OnReset();	// SN仪器索引表	
	m_oIPInstrumentMap.OnReset();	// IP地址仪器索引表
	m_oSNInstrumentCrossMap.OnReset();	// SN仪器索引表，交叉站
	m_oSNInstrumentPowerMap.OnReset();	// SN仪器索引表，电源站
}

// 加载现场仪器数据队列
void CInstrumentList::LoadSiteData()
{
	CString strFileName = "..\\data\\FileInstrument.dat";
	CFile oFile;
	BOOL bData;
	bData = oFile.Open(strFileName, CFile::modeRead | CFile::typeBinary);
	ULONGLONG iByteCount = oFile.GetLength();
	unsigned int iInstrumentSize = sizeof(CInstrument);
	if(iByteCount != iInstrumentSize * m_uiCountAll)
	{
		oFile.Close();
		return;
	}
	iByteCount = oFile.Read(m_pArrayInstrument, iInstrumentSize * m_uiCountAll);
	oFile.Close();

	m_uiCountUsed = 0;	// 使用中仪器数量
	CInstrument* pInstrument = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pInstrument = &m_pArrayInstrument[i];
		if(true == pInstrument->m_bInUsed)
		{
			if(0 == i)
			{
				m_pFirstMainCross = pInstrument;
			}
			m_oSNInstrumentMap.AddInstrument(pInstrument->m_uiSN, pInstrument);	// 加入SN索引表
			if(1 == pInstrument->m_uiInstrumentType)	// 交叉站
			{
				m_oSNInstrumentCrossMap.AddInstrument(pInstrument->m_uiSN, pInstrument);	// 加入SN索引表，交叉站
			}
			if(2 == pInstrument->m_uiInstrumentType)	// 电源站
			{
				m_oSNInstrumentPowerMap.AddInstrument(pInstrument->m_uiSN, pInstrument);	// 加入SN索引表，电源站
			}
			if(pInstrument->m_uiIP > 0)
			{
				m_oIPInstrumentMap.AddInstrument(pInstrument->m_uiIP, pInstrument);	// 加入IP索引表
			}			
			m_uiCountUsed++;	// 使用中仪器数量
		}
	}	
	m_uiCountFree = m_uiCountAll - m_uiCountUsed;	// 空闲仪器数量
	// 设置交叉站在测区中的位置
	SetInstrumentCrossSurveryPosition();
	// 设置电源站在测区中的位置
	SetInstrumentPowerSurveryPosition();
}

// 得到首个仪器位置
POSITION CInstrumentList::GetFirstInstrumentPosition()
{
	return m_oSNInstrumentMap.GetFirstInstrumentPosition();
}

// 得到下一个仪器
void CInstrumentList::GetNextInstrumentByPosition(POSITION &pos, CInstrument* &pInstrument)
{
	return m_oSNInstrumentMap.GetNextInstrumentByPosition(pos, pInstrument);
}

// 设置交叉站在测区中的位置
void CInstrumentList::SetInstrumentCrossSurveryPosition()
{
	CInstrument* pInstrument = NULL;
	POSITION pos = m_oSNInstrumentCrossMap.GetFirstInstrumentPosition();
	while(NULL != pos)
	{
		m_oSNInstrumentCrossMap.GetNextInstrumentByPosition(pos, pInstrument);
		// 设置仪器在测区中的位置
		SetInstrumentSurveryPosition(pInstrument);
	}
}

// 设置电源站在测区中的位置
void CInstrumentList::SetInstrumentPowerSurveryPosition()
{
	CInstrument* pInstrument = NULL;
	POSITION pos = m_oSNInstrumentPowerMap.GetFirstInstrumentPosition();
	while(NULL != pos)
	{
		m_oSNInstrumentPowerMap.GetNextInstrumentByPosition(pos, pInstrument);
		// 设置仪器在测区中的位置
		SetInstrumentSurveryPosition(pInstrument);
	}
}

// 设置仪器在测区中的位置
void CInstrumentList::SetInstrumentSurveryPosition(CInstrument* &pInstrument)
{
	CInstrument* pInstrumentLeft = pInstrument;
	while(0 != pInstrumentLeft->m_uiSNInstrumentLeft)
	{
		if(TRUE == m_oSNInstrumentMap.GetInstrument(pInstrumentLeft->m_uiSNInstrumentLeft, pInstrumentLeft))
		{
			if(pInstrumentLeft->m_uiChannelNb > 0)
			{
				pInstrument->m_uiLineNb = pInstrumentLeft->m_uiLineNb;
				pInstrument->m_uiPointNb = pInstrumentLeft->m_uiPointNb;
				pInstrument->m_uiChannelNb = pInstrumentLeft->m_uiChannelNb;
				break;
			}
		}
		else
		{
			break;
		}
	}
}

/**
* 得到迂回道中仪器数量
* @param unsigned int uiIndex 迂回道低端仪器索引号
* @return int 迂回道中仪器数量
*/
int CInstrumentList::GetDetourInstrumentCount(unsigned int uiIndex)
{
	int iCount = 0;
	CInstrument* pInstrument = NULL;
	CInstrument* pInstrumentNext = NULL;

	pInstrument = &m_pArrayInstrument[uiIndex];
	if((true == pInstrument->m_bInUsed) && (true == pInstrument->m_bDetourMarkerLow))
	{
//		iCount++;
		if(pInstrument->m_iIndexInstrumentRight > 0)
		{
			pInstrumentNext = &m_pArrayInstrument[pInstrument->m_iIndexInstrumentRight];
			while(true)
			{
				if((true == pInstrumentNext->m_bInUsed) && (true == pInstrumentNext->m_bDetour))
				{
					iCount++;
				}
				else
				{
					break;
				}
				if(pInstrumentNext->m_iIndexInstrumentRight > 0)
				{
					pInstrumentNext = &m_pArrayInstrument[pInstrumentNext->m_iIndexInstrumentRight];
				}
				else
				{
					break;
				}
			}
			if((true == pInstrumentNext->m_bInUsed) && (true == pInstrumentNext->m_bDetourMarkerHigh))
			{
//				iCount++;
			}
		}
	}
	return iCount;
}

/**
* 判断仪器A是否在仪器B左边
* @param CInstrument* pInstrumentA 仪器A
* @param CInstrument* pInstrumentB 仪器B
* @return bool true：是；false：否
*/
bool CInstrumentList::JudgeInstrumentAAtInstrumentBLeft(CInstrument* pInstrumentA, CInstrument* pInstrumentB)
{
	CInstrument* pInstrumentNext = NULL;
	pInstrumentNext = pInstrumentB;
	while(true)
	{
		if(-1 == pInstrumentNext->m_iIndexInstrumentLeft)
		{
			return false;
		}
		pInstrumentNext = &m_pArrayInstrument[pInstrumentNext->m_iIndexInstrumentLeft];
		if(pInstrumentA == pInstrumentNext)
		{
			return true;
		}		
	}
	return true;
}

/**
* 判断仪器A是否在仪器B右边
* @param CInstrument* pInstrumentA 仪器A
* @param CInstrument* pInstrumentB 仪器B
* @return bool true：是；false：否
*/
bool CInstrumentList::JudgeInstrumentAAtInstrumentBRight(CInstrument* pInstrumentA, CInstrument* pInstrumentB)
{
	CInstrument* pInstrumentNext = NULL;
	pInstrumentNext = pInstrumentB;
	while(true)
	{
		if(-1 == pInstrumentNext->m_iIndexInstrumentRight)
		{
			return false;
		}
		pInstrumentNext = &m_pArrayInstrument[pInstrumentNext->m_iIndexInstrumentRight];
		if(pInstrumentA == pInstrumentNext)
		{
			return true;
		}		
	}
	return true;
}