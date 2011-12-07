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