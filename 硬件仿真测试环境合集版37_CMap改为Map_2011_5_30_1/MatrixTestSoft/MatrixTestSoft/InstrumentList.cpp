#include "StdAfx.h"
#include "InstrumentList.h"
#include "Parameter.h"
#include "Resource.h"
CInstrumentList::CInstrumentList(void)
: m_pInstrumentArray(NULL)
, m_uiCountFree(0)
, m_pwnd(NULL)
{
}

CInstrumentList::~CInstrumentList(void)
{
	if (m_pInstrumentArray != NULL)
	{
		m_pInstrumentArray = NULL;
		delete[] m_pInstrumentArray;
	}
	if (m_pwnd != NULL)
	{
		m_pwnd = NULL;
		delete m_pwnd;
	}
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
		m_pInstrumentArray = NULL;
		delete[] m_pInstrumentArray;
	}
	// 删除索引表中所有仪器
	m_oInstrumentMap.clear();
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
void CInstrumentList::AddInstrumentToMap(unsigned int uiIndex, CInstrument* pInstrument)
{
//	SetInstrumentLocation(pInstrument);
	m_oInstrumentMap.insert(hash_map<unsigned int, CInstrument*>::value_type (uiIndex, pInstrument));
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
BOOL CInstrumentList::IfIndexExistInMap(unsigned int uiIndex)
{
	BOOL bResult = FALSE;
	CInstrument* pInstrument = NULL;
	hash_map<unsigned int, CInstrument*>::iterator iter;
	iter = m_oInstrumentMap.find(uiIndex);
	if (iter != m_oInstrumentMap.end())
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
BOOL CInstrumentList::GetInstrumentFromMap(unsigned int uiIndex, CInstrument* &pInstrument)
{
	BOOL bResult = FALSE;
	hash_map<unsigned int, CInstrument*>::iterator iter;
	iter = m_oInstrumentMap.find(uiIndex);
	if (iter != m_oInstrumentMap.end())
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

// 由索引号从索引表中删除一个仪器
//************************************
// Method:    DeleteInstrumentFromMap
// FullName:  CInstrumentList::DeleteInstrumentFromMap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned int uiIndex
//************************************
void CInstrumentList::DeleteInstrumentFromMap(unsigned int uiIndex)
{
	m_oInstrumentMap.erase(uiIndex);
}

// 设备根据首包时刻排序
//************************************
// Method:    SetInstrumentLocation
// FullName:  CInstrumentList::SetInstrumentLocation
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CInstrument * pInstrumentAdd
//************************************
void CInstrumentList::SetInstrumentLocation(CInstrument* pInstrumentAdd)
{
	POSITION pos = NULL;				// 位置	
	unsigned int uiKey = 0;					// 索引键	
	unsigned int uiLocation = 0;
	// hash_map迭代器，效率比map高
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentMap.begin(); iter!=m_oInstrumentMap.end(); iter++)
	{
		ProcessMessages();
		if (NULL != iter->second)
		{
			if (iter->second->m_uiSN != pInstrumentAdd->m_uiSN)
			{
				if (pInstrumentAdd->m_uiHeadFrameTime > iter->second->m_uiHeadFrameTime)
				{
					uiLocation++;
				}
			}
		}
	}
	if (uiLocation == pInstrumentAdd->m_uiLocation)
	{
		// 首包计数器加一
		pInstrumentAdd->m_iHeadFrameCount++;
	}
	else
	{
		pInstrumentAdd->m_iHeadFrameCount = 0;
		pInstrumentAdd->m_uiLocation = uiLocation;
	}
	TRACE(_T("仪器SN%04x,"), pInstrumentAdd->m_uiSN);
	TRACE(_T("仪器位置%d\r\n"), pInstrumentAdd->m_uiLocation);
}

// 删除采集站尾包之后的仪器
//************************************
// Method:    TailFrameDeleteInstrument
// FullName:  CInstrumentList::TailFrameDeleteInstrument
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CInstrument * pInstrumentDelete
//************************************
void CInstrumentList::TailFrameDeleteInstrument(CInstrument* pInstrumentDelete)
{
	POSITION pos = NULL;				// 位置	
	unsigned int uiKey = 0;					// 索引键	
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentMap.begin(); iter!=m_oInstrumentMap.end(); iter++)
	{
		ProcessMessages();
		if (NULL != iter->second)
		{
			ProcessMessages();
			if (pInstrumentDelete->m_uiLocation < iter->second->m_uiLocation)
			{
				// 显示设备断开连接的图标
				OnShowDisconnectedIcon(iter->second->m_uiIPAddress);
				// 将仪器从索引表中删除
				DeleteInstrumentFromMap(uiKey);
				// 重置仪器
				iter->second->OnReset();
				// 仪器加在空闲仪器队列尾部
				m_olsInstrumentFree.push_back(iter->second);
				m_uiCountFree++;
			}
		}
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
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey = 0;	// 索引键

	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentMap.begin(); iter!=m_oInstrumentMap.end(); iter++)
	{
		ProcessMessages();
		if (NULL != iter->second)
		{
			// 发送尾包时刻查询但是在下一个尾包接收之前未回复的设置过期标志
			if (iter->second->m_bSendTailTimeFrame == true)
			{
				if (iter->second->m_bTailTimeReturnOK == false)
				{
					iter->second->m_bTailTimeExpired = true;
				}
			}
			iter->second->m_bSendTailTimeFrame = false;
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
	m_oInstrumentMap.clear();
	for(unsigned int i = 0; i < InstrumentMaxCount; i++)
	{
		// 仪器在仪器数组中的位置
		m_pInstrumentArray[i].m_uiIndex = i;
		// 重置仪器
		m_pInstrumentArray[i].OnReset();
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
	m_oInstrumentMap.clear();
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
	POSITION pos = NULL;				// 位置	
	unsigned int uiKey = 0;					// 索引键	
	unsigned int icount = m_oInstrumentMap.size();
	if (icount == 0)
	{
		return;
	}
	// hash_map迭代器
	hash_map<unsigned int, CInstrument*>::iterator  iter;
	for(iter=m_oInstrumentMap.begin(); iter!=m_oInstrumentMap.end(); iter++)
	{
		ProcessMessages();
		if (NULL != iter->second)
		{
			// 显示设备断开连接的图标
			OnShowDisconnectedIcon(iter->second->m_uiIPAddress);
			// 将仪器从索引表中删除
			DeleteInstrumentFromMap(uiKey);
			// 重置仪器
			iter->second->OnReset();
			// 仪器加在空闲仪器队列尾部
			m_olsInstrumentFree.push_back(iter->second);
			m_uiCountFree++;
		}
	}
}

// 根据IP地址显示设备断开连接的图标
//************************************
// Method:    OnShowDisconnectedIcon
// FullName:  CInstrumentList::OnShowDisconnectedIcon
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: unsigned int uiIPAddress
//************************************
void CInstrumentList::OnShowDisconnectedIcon(unsigned int uiIPAddress)
{
	CButton* iconbutton = NULL;
	CStatic* iconstatic = NULL;
	CButton* pButton = NULL;

	for (int i=0 ;i<= InstrumentNum; i++)
	{
		ProcessMessages();
		if (uiIPAddress == (IPSetAddrStart + i * IPSetAddrInterval))
		{
			if (i == 0)
			{
				iconstatic =(CStatic*)m_pwnd->GetDlgItem(IDC_STATIC_LAUX);
				iconstatic->SetIcon(m_iconLAUXDisconnected);
			}
			else
			{
				iconbutton = (CButton*)m_pwnd->GetDlgItem(m_iButtonIDFDU[i-1]);
				iconbutton->SetIcon(m_iconFDUDisconnected);
				pButton = (CButton*)m_pwnd->GetDlgItem(m_iCheckIDInstrumentFDU[i-1]);
				pButton->SetCheck(0);
			}
			break;
		}
	}
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
