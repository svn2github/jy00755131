#include "StdAfx.h"
#include "ClientCommFrame.h"


CClientCommFrame::CClientCommFrame(void)
{
	m_uiPacketIndex = 0;
	m_uiCmdIndex = 0;
}


CClientCommFrame::~CClientCommFrame(void)
{
}

// 重置接收帧结构体
void CClientCommFrame::OnResetFrameStruct(m_oCommFrameStructPtr pFrameStruct)
{
	memset(pFrameStruct, 0, sizeof(m_oCommFrameStruct));
}
// 初始化
void CClientCommFrame::OnInit(void)
{
	m_uiCountFree = FrameStructNumMax;
	m_olsCommFrameFree.clear();
	InitializeCriticalSection(&m_oSecClientFrame);
	m_olsCommWorkFrame.clear();
	m_oSndFrameMap.clear();
	for(unsigned int i = 0; i < FrameStructNumMax; i++)
	{
		// 重置接收帧结构体
		OnResetFrameStruct(&m_oCommFrameArray[i]);
		// 仪器加在空闲接收帧结构体队列尾部
		m_olsCommFrameFree.push_back(&m_oCommFrameArray[i]);
	}
}

// 得到一个空闲接收帧结构体
m_oCommFrameStructPtr CClientCommFrame::GetFreeFrameStruct(void)
{
	m_oCommFrameStructPtr pFrameStruct = NULL;
	list <m_oCommFrameStructPtr>::iterator iter;
	if(m_uiCountFree > 0)	//有空闲接收帧结构体
	{
		// 从空闲队列中得到一个接收帧结构体
		iter = m_olsCommFrameFree.begin();
		pFrameStruct = *iter;
		m_olsCommFrameFree.pop_front();	
		m_uiCountFree--;	// 空闲接收帧结构体总数减1
	}
	else
	{
		// 写入错误日志
	}
	return pFrameStruct;
}
// 增加一个空闲接收帧结构体
void CClientCommFrame::AddFreeFrameStruct(m_oCommFrameStructPtr pFrameStruct)
{
	//初始化接收帧结构体
	OnResetFrameStruct(pFrameStruct);
	//加入空闲队列
	m_olsCommFrameFree.push_back(pFrameStruct);
	m_uiCountFree++;	// 空闲接收帧结构体总数加1
}
// 关闭
void CClientCommFrame::OnClose(void)
{
	m_olsCommFrameFree.clear();
	m_olsCommWorkFrame.clear();
	m_oSndFrameMap.clear();
	DeleteCriticalSection(&m_oSecClientFrame);
}

// 判断索引号是否已加入索引表
BOOL CClientCommFrame::IfFramePtrExistInSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
	unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap)
{
	if (pMap == NULL)
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	m_oSndFrameKey Key(m_usCmd, m_uiServerTimeStep, m_uiPacketIndex);
	map<m_oSndFrameKey, m_oCommFrameStructPtr>::iterator iter;
	iter = pMap->find(Key);
	if (iter != pMap->end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}
// 增加对象到索引表
void CClientCommFrame::AddFramePtrToSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
	unsigned int m_uiPacketIndex, m_oCommFrameStructPtr pFrameStruct, 
	map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap)
{
	if ((pFrameStruct == NULL) || (pMap == NULL))
	{
		return;
	}
	m_oSndFrameKey Key(m_usCmd, m_uiServerTimeStep, m_uiPacketIndex);
	if (false == IfFramePtrExistInSndMap(m_usCmd, m_uiServerTimeStep, m_uiPacketIndex, pMap))
	{
		pMap->insert(map<m_oSndFrameKey, m_oCommFrameStructPtr>::value_type (Key, pFrameStruct));
	}
}
// 根据输入索引号，由索引表得到指针
m_oCommFrameStructPtr CClientCommFrame::GetFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
	unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap)
{
	if (pMap == NULL)
	{
		return NULL;
	}
	m_oSndFrameKey Key(m_usCmd, m_uiServerTimeStep, m_uiPacketIndex);
	map<m_oSndFrameKey, m_oCommFrameStructPtr>::iterator iter;
	iter = pMap->find(Key);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return iter->second;
}
// 从索引表删除索引号指向的仪器指针
BOOL CClientCommFrame::DeleteFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
	unsigned int m_uiPacketIndex, map<m_oSndFrameKey, m_oCommFrameStructPtr>* pMap)
{
	if (pMap == NULL)
	{
		return FALSE;
	}
	BOOL bResult = FALSE;
	m_oSndFrameKey Key(m_usCmd, m_uiServerTimeStep, m_uiPacketIndex);
	map<m_oSndFrameKey, m_oCommFrameStructPtr>::iterator iter;
	iter = pMap->find(Key);
	if (iter != pMap->end())
	{
		pMap->erase(iter);
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}