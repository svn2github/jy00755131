#include "StdAfx.h"

#include "IndexInstrumentMap.h"

CIndexInstrumentMap::CIndexInstrumentMap(void)
{
}

CIndexInstrumentMap::~CIndexInstrumentMap(void)
{
}

// 初始化
void CIndexInstrumentMap::OnInit()
{

}

// 关闭
void CIndexInstrumentMap::OnClose()
{
	// 删除所有仪器
	DeleteInstrumentAll();
}

// 重置
void CIndexInstrumentMap::OnReset()
{
	// 删除所有仪器
	DeleteInstrumentAll();
}

// 增加一个仪器
void CIndexInstrumentMap::AddInstrument(unsigned int uiIndex, CInstrument* pInstrument)
{
	m_oInstrumentMap.SetAt(uiIndex, pInstrument);
}

// 仪器索引号是否已加入索引表
BOOL CIndexInstrumentMap::IfIndexExistInMap(unsigned int uiIndex)
{
	CInstrument* pInstrument = NULL;
	return m_oInstrumentMap.Lookup(uiIndex, pInstrument);
}

// 得到一个仪器
BOOL CIndexInstrumentMap::GetInstrument(unsigned int uiIndex, CInstrument* &pInstrument)
{
	return m_oInstrumentMap.Lookup(uiIndex, pInstrument);
}

// 删除一个仪器
void CIndexInstrumentMap::DeleteInstrument(unsigned int uiIndex)
{
	m_oInstrumentMap.RemoveKey(uiIndex);
}

// 删除所有仪器
void CIndexInstrumentMap::DeleteInstrumentAll(void)
{
	m_oInstrumentMap.RemoveAll();
}

// 得到首个仪器位置
POSITION CIndexInstrumentMap::GetFirstInstrumentPosition()
{
	POSITION pos = m_oInstrumentMap.GetStartPosition();
	return pos;
}

// 得到下一个仪器
void CIndexInstrumentMap::GetNextInstrumentByPosition(POSITION &pos, CInstrument* &pInstrument)
{
	unsigned int uiIP;
	return m_oInstrumentMap.GetNextAssoc(pos, uiIP, pInstrument);
}