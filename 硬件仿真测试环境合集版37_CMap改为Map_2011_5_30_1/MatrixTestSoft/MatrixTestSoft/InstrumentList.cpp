#include "StdAfx.h"
#include "InstrumentList.h"
#include "Parameter.h"
#include "Resource.h"
CInstrumentList::CInstrumentList(void)
: m_pInstrumentArray(NULL)
, m_uiCountFree(0)
, m_pWnd(NULL)
, m_pLogFile(NULL)
, m_pInstrumentGraph(NULL)
, m_uiLCISn(0)
{
}

CInstrumentList::~CInstrumentList(void)
{
}

// 初始化
//************************************
// Method:    OnInit
// FullName:  CInstrumentList::OnInit
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::OnInit(void)
{
	m_pInstrumentArray = new CInstrument[InstrumentMaxCount];
	m_uiCountFree = InstrumentMaxCount;
// 	//初始化哈希表，并指定其大小
// 	m_oInstrumentMap.InitHashTable(InstrumentMaxCount);
}

// 关闭
//************************************
// Method:    OnClose
// FullName:  CInstrumentList::OnClose
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::OnClose(void)
{
	// 清空空闲仪器队列
	m_olsInstrumentFree.clear();
	if (m_pInstrumentArray != NULL)
	{
		delete[] m_pInstrumentArray;
	}
	// 删除索引表中所有仪器
	m_oInstrumentSNMap.clear();
	m_oInstrumentIPMap.clear();
	m_oRoutAddrMap.clear();
}

// 得到一个空闲仪器
//************************************
// Method:    GetFreeInstrument
// FullName:  CInstrumentList::GetFreeInstrument
// Access:    public 
// Returns:   CInstrument*
// Qualifier:
// Parameter: void
//************************************
CInstrument* CInstrumentList::GetFreeInstrument(void)
{
	CInstrument* pInstrument = NULL;
	list <CInstrument*>::iterator iter;
	if(m_uiCountFree > 0)	//有空闲仪器
	{
		// 从空闲队列中得到一个仪器
		iter = m_olsInstrumentFree.begin();
		pInstrument = *iter;
		m_olsInstrumentFree.pop_front();	
		pInstrument->m_bInUsed = true;	// 设置仪器为使用中
		m_uiCountFree--;	// 空闲仪器总数减1
	}
	else
	{
		AfxMessageBox(_T("已无空闲仪器！"));
		m_pLogFile->OnWriteLogFile(_T("CInstrumentList::GetFreeInstrument"), _T("已无空闲仪器！"), ErrorStatus);
	}
	return pInstrument;
}

// 将一个仪器加入索引表
//************************************
// Method:    AddInstrumentToMap
// FullName:  CInstrumentList::AddInstrumentToMap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned int uiIndex
// Parameter: CInstrument * pInstrument
//************************************
void CInstrumentList::AddInstrumentToSNMap(unsigned int uiIndex, CInstrument* pInstrument)
{
//	SetInstrumentLocation(pInstrument);
	m_oInstrumentSNMap.insert(hash_map<unsigned int, CInstrument*>::value_type (uiIndex, pInstrument));
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
	iter = m_oInstrumentSNMap.find(uiIndex);
	if (iter != m_oInstrumentSNMap.end())
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
	iter = m_oInstrumentSNMap.find(uiIndex);
	if (iter != m_oInstrumentSNMap.end())
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
	m_oInstrumentIPMap.insert(hash_map<unsigned int, CInstrument*>::value_type (uiIndex, pInstrument));
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
	iter = m_oInstrumentIPMap.find(uiIndex);
	if (iter != m_oInstrumentIPMap.end())
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
	iter = m_oInstrumentIPMap.find(uiIndex);
	if (iter != m_oInstrumentIPMap.end())
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
// 将一个路由加入路由地址索引表
void CInstrumentList::AddRoutToRoutMap(unsigned int uiRoutAddr, unsigned int uiBroadCastPort)
{
	m_oRoutAddrMap.insert(hash_map<unsigned int, unsigned int>::value_type (uiRoutAddr, uiBroadCastPort));
}
// 判断路由是否已加入路由地址索引表
BOOL CInstrumentList::IfRoutExistInRoutMap(unsigned int uiRoutAddr)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, unsigned int>::iterator iter;
	iter = m_oRoutAddrMap.find(uiRoutAddr);
	if (iter != m_oRoutAddrMap.end())
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}
// 根据输入路由地址索引号，由路由地址索引表得到广播端口
BOOL CInstrumentList::GetBroadCastPortFromRoutMap(unsigned int uiRoutAddr, unsigned int &uiBroadCastPort)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, unsigned int>::iterator iter;
	iter = m_oRoutAddrMap.find(uiRoutAddr);
	if (iter != m_oRoutAddrMap.end())
	{
		uiBroadCastPort = iter->second;
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	return bResult;
}
// 设备根据首包时刻排序
//************************************
// Method:    SetInstrumentLocation
// FullName:  CInstrumentList::SetInstrumentLocation
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CInstrument * pInstrument
//************************************
void CInstrumentList::SetInstrumentLocation(CInstrument* pInstrument)
{
	int iLocation = 0;
	unsigned int uiLineRoutLocation = 0;
	unsigned int uiFDULocation = 0;
	int iLXLocation = 0;
	CString str = _T("");
	// hash_map迭代器，效率比map高
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	if (pInstrument->m_uiLineDirection == DirectionCenter)
	{
		pInstrument->m_iLocation = 0;
	} 
	else if ((pInstrument->m_uiLineDirection == DirectionTop)
		|| (pInstrument->m_uiLineDirection == DirectionDown))
	{
		for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
		{
			//		ProcessMessages();
			if (NULL != iter->second)
			{
				if (iter->second->m_uiRoutAddressTop == pInstrument->m_uiRoutAddress)
				{
					iLocation = iter->second->m_iLocation;
					break;
				}
				else if (iter->second->m_uiRoutAddressDown == pInstrument->m_uiRoutAddress)
				{
					iLocation = iter->second->m_iLocation;
					break;
				}
			}
		}
	}
	else
	{
		// 仪器设备左连
		if (pInstrument->m_uiLineDirection == DirectionLeft)
		{
			for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
			{
				//		ProcessMessages();
				if (NULL != iter->second)
				{
					// 路由相同
					if ((iter->second->m_uiRoutAddressLeft == pInstrument->m_uiRoutAddress)
						|| (iter->second->m_uiRoutAddress == pInstrument->m_uiRoutAddress))
					{
						// 计算该仪器的相对位置
						if (pInstrument->m_uiHeadFrameTime > iter->second->m_uiHeadFrameTime)
						{
							iLocation--;
							uiLineRoutLocation++;
							if ((pInstrument->m_uiInstrumentType == InstrumentTypeFDU)
								&& (iter->second->m_uiInstrumentType == InstrumentTypeFDU))
							{
								uiFDULocation++;
							}
						}
						// 记录连接到的交叉站的位置
						if (iter->second->m_uiRoutAddressLeft == pInstrument->m_uiRoutAddress)
						{
							iLXLocation = iter->second->m_iLocation;
						}
					}
				}
			}
			iLocation += iLXLocation;
		} 
		// 仪器设备右连
		else
		{
			for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
			{
				//		ProcessMessages();
				if (NULL != iter->second)
				{
					// 路由相同
					if ((iter->second->m_uiRoutAddressRight == pInstrument->m_uiRoutAddress)
						|| (iter->second->m_uiRoutAddress == pInstrument->m_uiRoutAddress))
					{
						// 计算该仪器的相对位置
						if (pInstrument->m_uiHeadFrameTime > iter->second->m_uiHeadFrameTime)
						{
							iLocation++;
							uiLineRoutLocation++;
							if ((pInstrument->m_uiInstrumentType == InstrumentTypeFDU)
								&& (iter->second->m_uiInstrumentType == InstrumentTypeFDU))
							{
								uiFDULocation++;
							}
						}
						// 记录连接到的交叉站的位置
						if (iter->second->m_uiRoutAddressRight == pInstrument->m_uiRoutAddress)
						{
							iLXLocation = iter->second->m_iLocation;
						}
					}
				}
			}
			iLocation += iLXLocation;
		}
	}
	if (iLocation == pInstrument->m_iLocation)
	{
		// 首包计数器加一
		pInstrument->m_iHeadFrameCount++;
	}
	else
	{
		pInstrument->m_iHeadFrameCount = 0;
		pInstrument->m_iLocation = iLocation;
		pInstrument->m_uiLineRoutLocation = uiLineRoutLocation;
		pInstrument->m_uiFDULocation = uiFDULocation;
		//		pInstrumentAdd->m_uiFDULocation = uiLocation - 1;
	}
	str.Format(_T("仪器SN为0x%x，仪器首包时刻%d，仪器位置%d，采集站位置%d，仪器类型%d！"), pInstrument->m_uiSN, 
		pInstrument->m_uiHeadFrameTime, pInstrument->m_iLocation, pInstrument->m_uiFDULocation, pInstrument->m_uiInstrumentType);
	m_pLogFile->OnWriteLogFile(_T("CInstrumentList::SetInstrumentLocation"), str, SuccessStatus);
}

// 删除尾包之后的仪器
//************************************
// Method:    TailFrameDeleteInstrument
// FullName:  CInstrumentList::TailFrameDeleteInstrument
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CInstrument * pInstrumentDelete
//************************************
void CInstrumentList::TailFrameDeleteInstrument(CInstrument* pInstrument)
{
	// 删除交叉线上交叉站交叉线尾包之后的仪器
	if ((pInstrument->m_uiLineDirection == DirectionCenter)
		|| (pInstrument->m_uiLineDirection == DirectionTop)
		|| (pInstrument->m_uiLineDirection == DirectionDown))
	{
		// @@@@@@@暂不考虑交叉线上尾包删除情况
		// @@@@@@@因为LCI在交叉线上有仪器的情况下会发尾包
//		TailFrameDeleteInstrumentLXLine(pInstrument);
	}
	// 删除大线方向尾包之后的仪器
	else
	{
		// 删除该站之后IP地址设置成功的站
		TailFrameDeleteInstrumentLine(pInstrument);
	}
}	

// 清理过期的尾包时刻查询结果
//************************************
// Method:    ClearExperiedTailTimeResult
// FullName:  CInstrumentList::ClearExperiedTailTimeResult
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::ClearExperiedTailTimeResult(void)
{
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
	{
//		ProcessMessages();
		if (NULL != iter->second)
		{
			// 发送尾包时刻查询但是在下一个尾包接收之前未回复的设置过期标志
			iter->second->m_bTailTimeReturnOK = false;
		}
	}
}

// 开始
//************************************
// Method:    OnOpen
// FullName:  CInstrumentList::OnOpen
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::OnOpen(void)
{
	// 清空空闲仪器队列
	m_olsInstrumentFree.clear();
	// 删除索引表中所有仪器
	m_oInstrumentSNMap.clear();
	m_oInstrumentIPMap.clear();
	m_oRoutAddrMap.clear();
	if (m_pInstrumentArray != NULL)
	{
		delete []m_pInstrumentArray;
	}
	m_pInstrumentArray = new CInstrument[InstrumentMaxCount];

	for(unsigned int i = 0; i < InstrumentMaxCount; i++)
	{
		// 重置仪器
		m_pInstrumentArray[i].OnReset();
		// 仪器在仪器数组中的位置
		m_pInstrumentArray[i].m_uiIndex = i;
		// 仪器加在空闲仪器队列尾部
		m_olsInstrumentFree.push_back(&m_pInstrumentArray[i]);
	}
	m_uiCountFree = InstrumentMaxCount;
}
// 停止
//************************************
// Method:    OnStop
// FullName:  CInstrumentList::OnStop
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::OnStop(void)
{
	// 清空空闲仪器队列
	m_olsInstrumentFree.clear();
	m_pInstrumentArray = NULL;
	// 删除索引表中所有仪器
	m_oInstrumentSNMap.clear();
	m_oInstrumentIPMap.clear();
	m_oRoutAddrMap.clear();
}

// 删除所有仪器
//************************************
// Method:    DeleteAllInstrument
// FullName:  CInstrumentList::DeleteAllInstrument
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::DeleteAllInstrument(void)
{
	unsigned int icount = m_oInstrumentSNMap.size();
	if (icount == 0)
	{
		return;
	}
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	m_oInstrumentIPMap.clear();
	m_oRoutAddrMap.clear();
	// 清除界面仪器和连接线显示
	m_pInstrumentGraph->OnClearAllInstrumentGraph();
	for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end();)
	{
		//		ProcessMessages();
		if (NULL != iter->second)
		{
			// 重置仪器
			iter->second->OnReset();
			// 仪器加在空闲仪器队列尾部
			m_olsInstrumentFree.push_back(iter->second);
			m_uiCountFree++;
			// 将仪器从索引表中删除
			m_oInstrumentSNMap.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	m_pLogFile->OnWriteLogFile(_T("CInstrumentList::DeleteAllInstrument"), _T("未收到尾包，删除所有在线仪器！"), WarningStatus);
}

// 防止程序在循环中运行无法响应消息
//************************************
// Method:    ProcessMessages
// FullName:  CInstrumentList::ProcessMessages
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void CInstrumentList::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}

// 判断仪器连接线号
void CInstrumentList::OnSetInstrumentLineIndex(CInstrument* pInstrument)
{
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
	{
		if (NULL != iter->second)
		{
			if ((iter->second->m_uiInstrumentType == InstrumentTypeLCI)
				|| (iter->second->m_uiInstrumentType == InstrumentTypeLAUX))
			{
				// 仪器在交叉站的上方也为交叉站
				if (iter->second->m_uiRoutAddressTop == pInstrument->m_uiRoutAddress)
				{
					pInstrument->m_iLineIndex = iter->second->m_iLineIndex - 1;
					iter->second->m_pInstrumentTop = pInstrument;
					pInstrument->m_pInstrumentDown = iter->second;
					break;
				}
				else if (iter->second->m_uiRoutAddressDown == pInstrument->m_uiRoutAddress)
				{
					pInstrument->m_iLineIndex = iter->second->m_iLineIndex + 1;
					iter->second->m_pInstrumentDown = pInstrument;
					pInstrument->m_pInstrumentTop = iter->second;
					break;
				}
				else if ((iter->second->m_uiRoutAddressLeft == pInstrument->m_uiRoutAddress)
					|| (iter->second->m_uiRoutAddressRight == pInstrument->m_uiRoutAddress))
				{
					pInstrument->m_iLineIndex = iter->second->m_iLineIndex;
					break;
				}
			}
		}
	}
}

// 建立仪器的连接关系
void CInstrumentList::OnInstrumentConnect(CInstrument* pInstrument)
{
	int iLocation = pInstrument->m_iLocation;
	// 仪器左连找到前一个仪器
	if (pInstrument->m_uiLineDirection == DirectionLeft)
	{
		iLocation++;
	}
	// 仪器右连找到前一个仪器
	else
	{
		iLocation--;
	}
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
	{
		if (NULL != iter->second)
		{
			if (iter->second->m_iLineIndex == pInstrument->m_iLineIndex)
			{
				if (iter->second->m_iLocation == iLocation)
				{
					if (pInstrument->m_uiLineDirection == DirectionLeft)
					{
						iter->second->m_pInstrumentLeft = pInstrument;
						pInstrument->m_pInstrumentRight = iter->second;
					}
					else if (pInstrument->m_uiLineDirection == DirectionRight)
					{
						iter->second->m_pInstrumentRight = pInstrument;
						pInstrument->m_pInstrumentLeft= iter->second;
					}
					break;
				}
			}
		}
	}
}

// 清除相同路由仪器的尾包计数
void CInstrumentList::OnClearSameRoutTailCount(CInstrument* pInstrument)
{
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
	{
		if (NULL != iter->second)
		{
			if (iter->second->m_uiRoutAddress == pInstrument->m_uiRoutAddress)
			{
				if (iter->second->m_uiLineRoutLocation < pInstrument->m_uiLineRoutLocation)
				{
					iter->second->m_iTailFrameCount = 0;
				}
			}
		}
	}
}

// 删除大线方向尾包之后的仪器
void CInstrumentList::TailFrameDeleteInstrumentLine(CInstrument* pInstrument)
{
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	hash_map<unsigned int, CInstrument*>::iterator  iter2;
	CString str = _T("");
	// 删除大线方向尾包之后的仪器
	for(iter=m_oInstrumentIPMap.begin(); iter!=m_oInstrumentIPMap.end();)
	{
		if ((NULL != iter->second)
			&& (iter->second->m_bIPSetOK == true)
			&& (iter->second->m_iLineIndex == pInstrument->m_iLineIndex))
		{
			if (((pInstrument->m_uiLineDirection == DirectionLeft)
				&& (pInstrument->m_iLocation > iter->second->m_iLocation))
				|| ((pInstrument->m_uiLineDirection == DirectionRight)
				&& (pInstrument->m_iLocation < iter->second->m_iLocation)))
			{
				str.Format(_T("清理尾包之后的仪器IP为%d"), iter->first);
				m_pLogFile->OnWriteLogFile(_T("CInstrumentList::TailFrameDeletRoutInstrument"), str, WarningStatus);
				// 将仪器从IP索引表中删除
				m_oInstrumentIPMap.erase(iter++);
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}
	// 删除大线方向尾包之后的仪器
	for(iter2=m_oInstrumentSNMap.begin(); iter2!=m_oInstrumentSNMap.end();)
	{
		if ((NULL != iter2->second)
			&& (iter2->second->m_bIPSetOK == true)
			&& (iter2->second->m_iLineIndex == pInstrument->m_iLineIndex))
		{
			if (((pInstrument->m_uiLineDirection == DirectionLeft)
				&& (pInstrument->m_iLocation > iter2->second->m_iLocation))
				|| ((pInstrument->m_uiLineDirection == DirectionRight)
				&& (pInstrument->m_iLocation < iter2->second->m_iLocation)))
			{
				m_pInstrumentGraph->DrawUnit(iter2->second->m_iLocation, iter2->second->m_iLineIndex, 
					iter2->second->m_uiLineDirection, iter2->second->m_uiInstrumentType, iter2->second->m_uiSN, 
					GraphInstrumentOffLine, true);
				// 重置仪器
				iter2->second->OnReset();
				// 仪器加在空闲仪器队列尾部
				m_olsInstrumentFree.push_back(iter2->second);
				m_uiCountFree++;
				// 将仪器从SN索引表中删除
				m_oInstrumentSNMap.erase(iter2++);
				if (pInstrument->m_uiLineDirection == DirectionLeft)
				{
					pInstrument->m_pInstrumentLeft = NULL;
				}
				else if (pInstrument->m_uiLineDirection == DirectionRight)
				{
					pInstrument->m_pInstrumentRight = NULL;
				}
			}
			else
			{
				iter2++;
			}
		}
		else
		{
			iter2++;
		}
	}
}

// 删除交叉线上交叉站交叉线尾包之后的仪器
void CInstrumentList::TailFrameDeleteInstrumentLXLine(CInstrument* pInstrument)
{
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	hash_map<unsigned int, CInstrument*>::iterator  iter2;
	CString str = _T("");
	if (pInstrument->m_pInstrumentTop != NULL)
	{
		// 删除大线方向尾包之后的仪器
		for(iter=m_oInstrumentIPMap.begin(); iter!=m_oInstrumentIPMap.end();)
		{
			if ((NULL != iter->second)
				&& (iter->second->m_bIPSetOK == true)
				&& (iter->second->m_iLineIndex < pInstrument->m_iLineIndex))
			{
				str.Format(_T("清理尾包之后的仪器IP为%d"), iter->first);
				m_pLogFile->OnWriteLogFile(_T("CInstrumentList::TailFrameDeleteInstrumentLXLine"), str, WarningStatus);
				// 将仪器从IP索引表中删除
				m_oInstrumentIPMap.erase(iter++);
			}
			else
			{
				iter++;
			}
		}
		// 删除大线方向尾包之后的仪器
		for(iter2=m_oInstrumentSNMap.begin(); iter2!=m_oInstrumentSNMap.end();)
		{
			if ((NULL != iter2->second)
				&& (iter2->second->m_bIPSetOK == true)
				&& (iter2->second->m_iLineIndex < pInstrument->m_iLineIndex))
			{
				m_pInstrumentGraph->DrawUnit(iter2->second->m_iLocation, iter2->second->m_iLineIndex, 
					iter2->second->m_uiLineDirection, iter2->second->m_uiInstrumentType, iter2->second->m_uiSN,
					GraphInstrumentOffLine, true);

				// 重置仪器
				iter2->second->OnReset();
				// 仪器加在空闲仪器队列尾部
				m_olsInstrumentFree.push_back(iter2->second);
				m_uiCountFree++;
				// 将仪器从SN索引表中删除
				m_oInstrumentSNMap.erase(iter2++);
				pInstrument->m_pInstrumentTop = NULL;
			}
			else
			{
				iter2++;
			}
		}
	}
	else if (pInstrument->m_pInstrumentDown != NULL)
	{
		// 删除大线方向尾包之后的仪器
		for(iter=m_oInstrumentIPMap.begin(); iter!=m_oInstrumentIPMap.end();)
		{
			if ((NULL != iter->second)
				&& (iter->second->m_bIPSetOK == true)
				&& (iter->second->m_iLineIndex > pInstrument->m_iLineIndex))
			{
				str.Format(_T("清理尾包之后的仪器IP为%d"), iter->first);
				m_pLogFile->OnWriteLogFile(_T("CInstrumentList::TailFrameDeleteInstrumentLXLine"), str, WarningStatus);
				// 将仪器从IP索引表中删除
				m_oInstrumentIPMap.erase(iter++);
			}
			else
			{
				iter++;
			}
		}
		// 删除大线方向尾包之后的仪器
		for(iter2=m_oInstrumentSNMap.begin(); iter2!=m_oInstrumentSNMap.end();)
		{
			if ((NULL != iter2->second)
				&& (iter2->second->m_bIPSetOK == true)
				&& (iter2->second->m_iLineIndex > pInstrument->m_iLineIndex))
			{
				m_pInstrumentGraph->DrawUnit(iter2->second->m_iLocation, iter2->second->m_iLineIndex, 
					iter2->second->m_uiLineDirection, iter2->second->m_uiInstrumentType, iter2->second->m_uiSN,
					GraphInstrumentOffLine, true);

				// 重置仪器
				iter2->second->OnReset();
				// 仪器加在空闲仪器队列尾部
				m_olsInstrumentFree.push_back(iter2->second);
				m_uiCountFree++;
				// 将仪器从SN索引表中删除
				m_oInstrumentSNMap.erase(iter2++);
				pInstrument->m_pInstrumentDown = NULL;
			}
			else
			{
				iter2++;
			}
		}
	}
}

// 检查尾包时刻查询应答是否全部接收
bool CInstrumentList::OnCheckTailTimeReturn(void)
{
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
	{
		//		ProcessMessages();
		if (NULL != iter->second)
		{
			if (iter->second->m_bTailTimeReturnOK == false)
			{
				return false;
			}
		}
	}
	m_pLogFile->OnWriteLogFile(_T("CInstrumentList::OnCheckTailTimeReturn"), _T("尾包时刻查询帧接收完成！"), SuccessStatus);
	return true;
}

// 得到采集站设备序号
void CInstrumentList::OnGetFduIndex(void)
{
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	hash_map<unsigned int, unsigned int>::iterator  iter2;
	CInstrumentGraph::m_oGraph oGraph;
	POSITION pos = NULL;
	list<unsigned int> oRoutList;
	unsigned int uiRout = 0;
	unsigned int uiSize = 0;
	unsigned int uiCount = 0;
	unsigned int uiFDUNum = 0;
	oRoutList.clear();
	for(iter2=m_oRoutAddrMap.begin(); iter2!=m_oRoutAddrMap.end(); iter2++)
	{
		oRoutList.push_back(iter2->first);
	}
	oRoutList.sort();
	uiSize = oRoutList.size();
	for (unsigned int i=0; i<uiSize; i++)
	{
		uiRout = oRoutList.front();
		oRoutList.pop_front();
		for(iter=m_oInstrumentSNMap.begin(); iter!=m_oInstrumentSNMap.end(); iter++)
		{
			if (NULL != iter->second)
			{
				if ((iter->second->m_uiInstrumentType == InstrumentTypeFDU)
					&& (iter->second->m_uiRoutAddress == uiRout))
				{
					uiCount++;
					iter->second->m_uiFDUIndex = iter->second->m_uiFDULocation + uiFDUNum;
					oGraph.iUnitIndex = iter->second->m_iLocation;
					oGraph.iLineIndex = iter->second->m_iLineIndex;
					pos = m_pInstrumentGraph->m_oInstrumentGraphRectList.Find(oGraph);
					if (NULL != pos)
					{
						oGraph = m_pInstrumentGraph->m_oInstrumentGraphRectList.GetAt(pos);
						oGraph.uiFDUIndex = iter->second->m_uiFDUIndex;
						m_pInstrumentGraph->m_oInstrumentGraphRectList.SetAt(pos, oGraph);
					}
				}
			}
		}
		uiFDUNum += uiCount;
		uiCount = 0;
	}
	oRoutList.clear();
}
// 按照仪器路由删除该路由方向上的仪器
void CInstrumentList::TailFrameDeleteInstrumentRout(unsigned int uiRoutAddr)
{
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	hash_map<unsigned int, CInstrument*>::iterator  iter2;
	CString str = _T("");
	// 删除大线方向尾包之后的仪器
	for(iter=m_oInstrumentIPMap.begin(); iter!=m_oInstrumentIPMap.end();)
	{
		if ((NULL != iter->second)
			&& (iter->second->m_uiRoutAddress == uiRoutAddr))
		{
			str.Format(_T("清理尾包之后的仪器IP为%d"), iter->first);
			m_pLogFile->OnWriteLogFile(_T("CInstrumentList::TailFrameDeletRoutInstrument"), str, WarningStatus);
			// 将仪器从IP索引表中删除
			m_oInstrumentIPMap.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	// 删除大线方向尾包之后的仪器
	for(iter2=m_oInstrumentSNMap.begin(); iter2!=m_oInstrumentSNMap.end();)
	{
		if ((NULL != iter2->second)
			&& (iter2->second->m_uiRoutAddress == uiRoutAddr))
		{
			m_pInstrumentGraph->DrawUnit(iter2->second->m_iLocation, iter2->second->m_iLineIndex, 
				iter2->second->m_uiLineDirection, iter2->second->m_uiInstrumentType, iter2->second->m_uiSN, 
				GraphInstrumentOffLine, true);
			// 重置仪器
			iter2->second->OnReset();
			// 仪器加在空闲仪器队列尾部
			m_olsInstrumentFree.push_back(iter2->second);
			m_uiCountFree++;
			// 将仪器从SN索引表中删除
			m_oInstrumentSNMap.erase(iter2++);
		}
		else
		{
			iter2++;
		}
	}
}