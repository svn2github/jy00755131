#include "StdAfx.h"

#include "InstrumentList.h"


CInstrumentList::CInstrumentList()
{
}

CInstrumentList::~CInstrumentList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CInstrumentList::OnInit()
{
	// 清空SN仪器索引表
	m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	m_oIPInstrumentMap.clear();
	// 生成仪器数组
	m_pArrayInstrument = new CInstrument[m_uiCountAll];
	// 生成仪器数组，为现场数据输出准备
	m_pArrayInstrumentOutput = new CInstrument[m_uiCountAll];
	// 空闲仪器数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 仪器在仪器数组中的位置
		m_pArrayInstrument[i].m_uiIndex = i;
		// 重置仪器
		m_pArrayInstrument[i].OnReset();
		// 仪器加在空闲仪器队列尾部
		m_olsInstrumentFree.AddTail(&m_pArrayInstrument[i]);
	}
}

/**
* 关闭
* @param void
* @return void
*/
void CInstrumentList::OnClose()
{
	// 清空SN仪器索引表
	m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	m_olsInstrumentFree.RemoveAll();
	// 删除仪器数组
	delete[] m_pArrayInstrument;
	// 删除仪器数组，为现场数据输出准备
	delete[] m_pArrayInstrumentOutput;
}

/**
* 重置
* @param void
* @return void
*/
void CInstrumentList::OnReset()
{
	// 清空SN仪器索引表
	m_oSNInstrumentMap.clear();
	// 清空IP地址仪器索引表
	m_oIPInstrumentMap.clear();
	// 清空空闲仪器队列
	m_olsInstrumentFree.RemoveAll();
	// 空闲仪器数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置仪器
		m_pArrayInstrument[i].OnReset();
		// 仪器加在空闲仪器队列尾部
		m_olsInstrumentFree.AddTail(&m_pArrayInstrument[i]);
	}

}

/**
* 得到一个空闲仪器
* @param void
* @return CInstrument* 空闲仪器 NULL：没有空闲仪器
*/
CInstrument* CInstrumentList::GetFreeInstrument()
{
	CInstrument* pInstrument = NULL;

	if(m_uiCountFree > 0)	//有空闲仪器
	{
		pInstrument = m_olsInstrumentFree.RemoveHead();	// 从空闲队列中得到一个仪器
		pInstrument->m_bInUsed = true;	// 设置仪器为使用中
		m_uiCountFree--;	// 空闲仪器总数减1
	}
	return pInstrument;
}

/**
* 增加一个空闲仪器
* @param CInstrument* pInstrument 仪器指针
* @return void
*/
void CInstrumentList::AddFreeInstrument(CInstrument* pInstrument)
{
	//初始化仪器
	pInstrument->OnReset();
	//加入空闲队列
	m_olsInstrumentFree.AddTail(pInstrument);
	m_uiCountFree++;	// 空闲仪器总数加1
}

/**
* 复制仪器数组到输出数组
* @param void
* @return void
*/
void CInstrumentList::CopyInstrumentArrayForOutput()
{
	memcpy(m_pArrayInstrumentOutput, m_pArrayInstrument, sizeof(CInstrument) * m_uiCountAll);
}
void CInstrumentList::AddInstrumentToSNMap(unsigned int uiIndex, CInstrument* pInstrument)
{
	m_oSNInstrumentMap.insert(hash_map<unsigned int, CInstrument*>::value_type (uiIndex, pInstrument));
}

// 判断仪器索引号是否已加入索引表
//************************************
// Method:    IfIndexExistInMap
// FullName:  CInstrumentList::IfIndexExistInMap
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: unsigned int uiIndex
//************************************
BOOL CInstrumentList::IfIndexExistInSNMap(unsigned int uiIndex)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, CInstrument*>::iterator iter;
	iter = m_oSNInstrumentMap.find(uiIndex);
	if (iter != m_oSNInstrumentMap.end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

// 根据输入索引号，由索引表得到仪器指针
//************************************
// Method:    GetInstrumentFromMap
// FullName:  CInstrumentList::GetInstrumentFromMap
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: unsigned int uiIndex
// Parameter: CInstrument *  & pInstrument
//************************************
BOOL CInstrumentList::GetInstrumentFromSNMap(unsigned int uiIndex, CInstrument* &pInstrument)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, CInstrument*>::iterator iter;
	iter = m_oSNInstrumentMap.find(uiIndex);
	if (iter != m_oSNInstrumentMap.end())
	{
		pInstrument = iter->second;
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

// 将一个仪器加入IP地址索引表
//************************************
// Method:    AddInstrumentToIPMap
// FullName:  CInstrumentList::AddInstrumentToIPMap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned int uiIndex
// Parameter: CInstrument * pInstrument
//************************************
void CInstrumentList::AddInstrumentToIPMap(unsigned int uiIndex, CInstrument* pInstrument)
{
	m_oIPInstrumentMap.insert(hash_map<unsigned int, CInstrument*>::value_type (uiIndex, pInstrument));
}

// 判断仪器索引号是否已加入IP地址索引表
//************************************
// Method:    IfIndexExistInIPMap
// FullName:  CInstrumentList::IfIndexExistInIPMap
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: unsigned int uiIndex
//************************************
BOOL CInstrumentList::IfIndexExistInIPMap(unsigned int uiIndex)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, CInstrument*>::iterator iter;
	iter = m_oIPInstrumentMap.find(uiIndex);
	if (iter != m_oIPInstrumentMap.end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}

// 根据输入IP地址索引号，由IP地址索引表得到仪器指针
//************************************
// Method:    GetInstrumentFromIPMap
// FullName:  CInstrumentList::GetInstrumentFromIPMap
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: unsigned int uiIndex
// Parameter: CInstrument *  & pInstrument
//************************************
BOOL CInstrumentList::GetInstrumentFromIPMap(unsigned int uiIndex, CInstrument* &pInstrument)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, CInstrument*>::iterator iter;
	iter = m_oIPInstrumentMap.find(uiIndex);
	if (iter != m_oIPInstrumentMap.end())
	{
		pInstrument = iter->second;
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}