#include "StdAfx.h"
#include "InstrumentList.h"
#include "Parameter.h"
#include "Resource.h"
CInstrumentList::CInstrumentList(void)
: m_pInstrumentArray(NULL)
, m_uiCountAll(0)
, m_uiCountFree(0)
, m_bIPSetAllOK(0)
, m_pwnd(NULL)
{
}

CInstrumentList::~CInstrumentList(void)
{
}

// 初始化
void CInstrumentList::OnInit(void)
{
	m_uiCountAll = InstrumentMaxCount;
	m_pInstrumentArray = new CInstrument[m_uiCountAll];
	m_uiCountFree = m_uiCountAll;
}

// 关闭
void CInstrumentList::OnClose(void)
{
	// 清空空闲仪器队列
	m_olsInstrumentFree.RemoveAll();
	m_pInstrumentArray = NULL;
	delete[] m_pInstrumentArray;
	// 删除索引表中所有仪器
	m_oInstrumentMap.RemoveAll();
}

// 得到一个空闲仪器
CInstrument* CInstrumentList::GetFreeInstrument(void)
{
	CInstrument* pInstrument = NULL;

	if(m_uiCountFree > 0)	//有空闲仪器
	{
		pInstrument = m_olsInstrumentFree.RemoveHead();	// 从空闲队列中得到一个仪器
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
void CInstrumentList::AddInstrumentToMap(unsigned int uiIndex, CInstrument* pInstrument)
{
//	SetInstrumentLocation(pInstrument);
	m_oInstrumentMap.SetAt(uiIndex, pInstrument);
}

// 判断仪器索引号是否已加入索引表
BOOL CInstrumentList::IfIndexExistInMap(unsigned int uiIndex)
{
	BOOL bResult = FALSE;
	CInstrument* pInstrument = NULL;
	bResult = m_oInstrumentMap.Lookup(uiIndex, pInstrument);
	pInstrument = NULL;
	delete pInstrument;
	return bResult;
}

// 根据输入索引号，由索引表得到仪器指针
BOOL CInstrumentList::GetInstrumentFromMap(unsigned int uiIndex, CInstrument* &pInstrument)
{
	return m_oInstrumentMap.Lookup(uiIndex, pInstrument);
}

// 由索引号从索引表中删除一个仪器
void CInstrumentList::DeleteInstrumentFromMap(unsigned int uiIndex)
{
	m_oInstrumentMap.RemoveKey(uiIndex);
}

// 设备根据首包时刻排序
void CInstrumentList::SetInstrumentLocation(CInstrument* pInstrumentAdd)
{
	CInstrument* pInstrument = NULL;	// 仪器对象指针
	POSITION pos = NULL;				// 位置	
	unsigned int uiKey;					// 索引键	
	unsigned int uiLocation = 0;
	pos = m_oInstrumentMap.GetStartPosition();	// 得到索引表起始位置
	while(NULL != pos)
	{
		ProcessMessages();
		pInstrument = NULL;		
		m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);	// 得到仪器对象
		if(NULL != pInstrument)	
		{
			if (pInstrument->m_uiSN != pInstrumentAdd->m_uiSN)
			{
				if (pInstrumentAdd->m_uiHeadFrameTime > pInstrument->m_uiHeadFrameTime)
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
	TRACE2(_T("仪器SN%04x，仪器位置%d\r\n"), pInstrumentAdd->m_uiSN, pInstrumentAdd->m_uiLocation);
	pInstrument = NULL;
	delete pInstrument;
}

// 删除采集站尾包之后的仪器
void CInstrumentList::TailFrameDeleteInstrument(CInstrument* pInstrumentDelete)
{
	CInstrument* pInstrument = NULL;	// 仪器对象指针
	POSITION pos = NULL;				// 位置	
	unsigned int uiKey;					// 索引键	

	pos = m_oInstrumentMap.GetStartPosition();	// 得到索引表起始位置
	while(NULL != pos)
	{
		pInstrument = NULL;		
		m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);	// 得到仪器对象
		if(NULL != pInstrument)	
		{
			ProcessMessages();
			if (pInstrumentDelete->m_uiLocation < pInstrument->m_uiLocation)
			{
				// 显示设备断开连接的图标
				OnShowDisconnectedIcon(pInstrument->m_uiIPAddress);
				// 将仪器从索引表中删除
				DeleteInstrumentFromMap(uiKey);
				// 重置仪器
				pInstrument->OnReset();
				// 仪器加在空闲仪器队列尾部
				m_olsInstrumentFree.AddTail(pInstrument);
				m_uiCountFree++;
			}
		}
	}
	pInstrument = NULL;
	delete pInstrument;
}	

// 清理过期的尾包时刻查询结果
void CInstrumentList::ClearExperiedTailTimeResult(void)
{
	CInstrument* pInstrument = NULL;	// 仪器	
	POSITION pos = NULL;	// 位置	
	unsigned int uiKey;	// 索引键

	// 得到索引表起始仪器位置
	pos = m_oInstrumentMap.GetStartPosition();
	// 当前位置有仪器
	while(NULL != pos)
	{
		ProcessMessages();
		pInstrument = NULL;
		// 得到仪器
		m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);
		if(NULL != pInstrument)	// 得到仪器
		{
			// 发送尾包时刻查询但是在下一个尾包接收之前未回复的设置过期标志
			if (pInstrument->m_bSendTailTimeFrame == true)
			{
				if (pInstrument->m_bTailTimeReturnOK == false)
				{
					pInstrument->m_bTailTimeExpired = true;
				}
			}
			pInstrument->m_bSendTailTimeFrame = false;
			pInstrument->m_bTailTimeReturnOK = false;
		}
	}
	pInstrument = NULL;
	delete pInstrument;
}

// 开始
void CInstrumentList::OnOpen(void)
{
	// 清空空闲仪器队列
	m_olsInstrumentFree.RemoveAll();
	// 删除索引表中所有仪器
	m_oInstrumentMap.RemoveAll();
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 仪器在仪器数组中的位置
		m_pInstrumentArray[i].m_uiIndex = i;
		// 重置仪器
		m_pInstrumentArray[i].OnReset();
		// 仪器加在空闲仪器队列尾部
		m_olsInstrumentFree.AddTail(&m_pInstrumentArray[i]);
	}
}
// 停止
void CInstrumentList::OnStop(void)
{
	// 清空空闲仪器队列
	m_olsInstrumentFree.RemoveAll();
	m_pInstrumentArray = NULL;
	// 删除索引表中所有仪器
	m_oInstrumentMap.RemoveAll();
}

// 删除所有仪器
void CInstrumentList::DeleteAllInstrument(void)
{
	CInstrument* pInstrument = NULL;	// 仪器对象指针
	POSITION pos = NULL;				// 位置	
	unsigned int uiKey;					// 索引键	
	unsigned int icount = m_oInstrumentMap.GetCount();
	if (icount == 0)
	{
		return;
	}
	pos = m_oInstrumentMap.GetStartPosition();	// 得到索引表起始位置
	while(NULL != pos)
	{
		ProcessMessages();
		pInstrument = NULL;		
		m_oInstrumentMap.GetNextAssoc(pos, uiKey, pInstrument);	// 得到仪器对象
		if(NULL != pInstrument)	
		{
			// 显示设备断开连接的图标
			OnShowDisconnectedIcon(pInstrument->m_uiIPAddress);
			// 将仪器从索引表中删除
			DeleteInstrumentFromMap(uiKey);
			// 重置仪器
			pInstrument->OnReset();
			// 仪器加在空闲仪器队列尾部
			m_olsInstrumentFree.AddTail(pInstrument);
			m_uiCountFree++;
		}
	}
	pInstrument = NULL;
	delete pInstrument;
}

// 根据IP地址显示设备断开连接的图标
void CInstrumentList::OnShowDisconnectedIcon(unsigned int uiIPAddress)
{
	CButton* iconbutton = NULL;
	CStatic* iconstatic = NULL;
	CButton* pButton = NULL;

	for (int i=0 ;i<= InstrumentNum; i++)
	{
		ProcessMessages();
		if (uiIPAddress == (71 + i*10))
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
	iconstatic = NULL;
	iconbutton = NULL;
	pButton = NULL;
	delete iconstatic;
	delete iconbutton;
	delete pButton;
}

// 防止程序在循环中运行无法响应消息
void CInstrumentList::ProcessMessages(void)
{
	MSG msg;
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::DispatchMessage(&msg);
}
