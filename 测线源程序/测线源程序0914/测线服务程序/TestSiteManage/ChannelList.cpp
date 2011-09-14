#include "StdAfx.h"
#include "ChannelList.h"

CChannelList::CChannelList()
{
}

CChannelList::~CChannelList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CChannelList::OnInit()
{
	// 生成测道数组
	m_pArrayChannel = new CChannelSetupData[m_uiCountAll];
	// 生成测道数组，为现场数据输出准备
	m_pArrayChannelOutput = new CChannelSetupData[m_uiCountAll];
	// 使用中测道数量
	m_uiCountUsed = 0;
	// 空闲测道数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 测道在测道数组中的位置
		m_pArrayChannel[i].m_uiIndex = i;
		// 重新加载测道
		m_pArrayChannel[i].OnReload();
	}
}

/**
* 重置
* @param void
* @return void
*/
void CChannelList::OnReset()
{
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置测道
		m_pArrayChannel[i].OnReset();
	}
}

/**
* 重新加载
* @param void
* @return void
*/
void CChannelList::OnReload()
{
	// 使用中测线数量
	m_uiCountUsed = 0;
	// 空闲测道数量
	m_uiCountFree = m_uiCountAll;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 重置测道
		m_pArrayChannel[i].OnReload();
	}
	// 清空索引表
	m_oChannelMap.RemoveAll();
}

/**
* 关闭
* @param void
* @return void
*/
void CChannelList::OnClose()
{
	// 删除测道数组
	delete[] m_pArrayChannel;
	// 删除测道数组，为现场数据输出准备
	delete[] m_pArrayChannelOutput;
	// 清空索引表
	m_oChannelMap.RemoveAll();
}

/**
* 得到一个空闲测道
* @param void
* @return CChannelSetupData* 空闲测道 NULL：没有空闲测道
*/
CChannelSetupData* CChannelList::GetFreeChannel()
{
	CChannelSetupData* pChannel = NULL;

	//判断：有空闲测道
	if(m_uiCountFree > 0)
	{
		pChannel = &m_pArrayChannel[m_uiCountUsed];
		pChannel->m_bInUsed = true;	// 设置测道为使用中
		m_uiCountFree--;	// 空闲测道总数减1
		m_uiCountUsed++;	// 使用中测道总数加1
	}
	return pChannel;
}

/**
* 测道加入测道索引表
* @param unsigned int uiIndex 索引号
* @param CChannelSetupData* pChannel 测道指针
* @return void
*/
void CChannelList::AddChannelToMap(unsigned int uiIndex, CChannelSetupData* pChannel)
{
	m_oChannelMap.SetAt(uiIndex, pChannel);
}

/**
* 测道索引号是否已加入索引表
* @param unsigned int uiIndex 索引号
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CChannelList::IfIndexExistInMap(unsigned int uiIndex)
{
	CChannelSetupData* pChannel = NULL;
	return m_oChannelMap.Lookup(uiIndex, pChannel);
}

/**
* 由索引表得到一个测道
* @param unsigned int uiIndex 索引号
* @param CChannelSetupData* pChannel 测道指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CChannelList::GetChannelFromMap(unsigned int uiIndex, CChannelSetupData* &pChannel)
{
	return m_oChannelMap.Lookup(uiIndex, pChannel);
}

/**
* 根据数组索引，在测道数组中得到一个测道
* @param unsigned int uiIndex 数组索引号
* @return CChannelSetupData* 测道
*/
CChannelSetupData* CChannelList::GetChannelByIndex(unsigned int uiIndex)
{
	return &m_pArrayChannel[uiIndex];
}

/**
* 根据测线号、测点号、测道号，在索引表中得到测道
* @param unsigned int uiNbLine 测线号
* @param unsigned int uiNbPoint 测点号
* @param unsigned int uiNbChannel 测道号
* @return CChannelSetupData* 测道 NULL：没有测道
*/
CChannelSetupData* CChannelList::GetChannel(unsigned int uiNbLine, unsigned int uiNbPoint, unsigned int uiNbChannel)
{
	CChannelSetupData* pChannel = NULL;
	unsigned int uiIP = CChannelSetupData::CreateIP(uiNbLine, uiNbPoint, uiNbChannel);
	if(TRUE == GetChannelFromMap(uiIP, pChannel))
	{
		return pChannel;
	}
	return NULL;
}

/**
* 复制测道数组到输出数组
* @param void
* @return void
*/
void CChannelList::CopyChannelArrayForOutput()
{
	memcpy(m_pArrayChannelOutput, m_pArrayChannel, sizeof(CChannelSetupData) * m_uiCountAll);
}