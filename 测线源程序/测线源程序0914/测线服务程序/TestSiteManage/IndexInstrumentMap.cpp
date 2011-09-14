#include "StdAfx.h"

#include "IndexInstrumentMap.h"

CIndexInstrumentMap::CIndexInstrumentMap()
{
}

CIndexInstrumentMap::~CIndexInstrumentMap()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CIndexInstrumentMap::OnInit()
{

}

/**
* 关闭
* @param void
* @return void
*/
void CIndexInstrumentMap::OnClose()
{
	// 删除所有仪器
	DeleteInstrumentAll();
}

/**
* 重置
* @param void
* @return void
*/
void CIndexInstrumentMap::OnReset()
{
	// 删除所有仪器
	DeleteInstrumentAll();
}

/**
* 增加一个仪器
* @param unsigned int uiIndex 索引号
* @param CInstrument* pInstrument	仪器指针
* @return void
*/
void CIndexInstrumentMap::AddInstrument(unsigned int uiIndex, CInstrument* pInstrument)
{
	m_oInstrumentMap.SetAt(uiIndex, pInstrument);
}

/**
* 仪器索引号是否已加入索引表
* @param unsigned int uiIndex 索引号
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CIndexInstrumentMap::IfIndexExistInMap(unsigned int uiIndex)
{
	CInstrument* pInstrument = NULL;
	return m_oInstrumentMap.Lookup(uiIndex, pInstrument);
}

/**
* 根据输入索引号，由索引表得到一个仪器指针
* @param unsigned int uiIndex 索引号
* @param CInstrument* pInstrument	仪器指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CIndexInstrumentMap::GetInstrument(unsigned int uiIndex, CInstrument* &pInstrument)
{
	return m_oInstrumentMap.Lookup(uiIndex, pInstrument);
}

/**
* 根据输入索引号，从索引表中删除一个仪器
* @param unsigned int uiIndex 索引号
* @return void
*/
void CIndexInstrumentMap::DeleteInstrument(unsigned int uiIndex)
{
	m_oInstrumentMap.RemoveKey(uiIndex);
}

/**
* 删除索引表中所有仪器
* @param void
* @return void
*/
void CIndexInstrumentMap::DeleteInstrumentAll()
{
	m_oInstrumentMap.RemoveAll();
}