#include "stdafx.h"
#include "MatrixCommDll.h"

CCommRecThread::CCommRecThread(void)
{
	m_pCommRecFrame = NULL;
	m_pCommSndFrame = NULL;
	m_pCommClient = NULL;
	m_uiClientActiveCount = 0;
	m_uiClientCheckCount = 0;
	m_oInstrumentWholeTableMap.clear();
	m_oInstrumentUpdataTableMap.clear();
}


CCommRecThread::~CCommRecThread(void)
{
	m_oInstrumentWholeTableMap.clear();
	m_oInstrumentUpdataTableMap.clear();
}
// 处理函数
void CCommRecThread::OnProc(void)
{
	int iFrameNum = 0;
	bool bActive = false;
	m_oCommFrameStructPtr ptrFrameRec = NULL;
	m_oCommFrameStructPtr ptrFrameSnd = NULL;
	EnterCriticalSection(&m_pCommRecFrame->m_oSecClientFrame);
	EnterCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
	iFrameNum = m_pCommRecFrame->m_olsCommWorkFrame.size();
	if (iFrameNum != 0)
	{
		bActive = true;
	}
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrameRec = *m_pCommRecFrame->m_olsCommWorkFrame.begin();
		// 如果为设置帧
		if (ptrFrameRec->m_cCmdType == CmdTypeSet)
		{
			// 发送应答帧
			m_pCommSndFrame->MakeReturnFrame(ptrFrameRec);
			// 如果为新命令则清除未完成的旧命令待处理任务
			if (m_pCommRecFrame->m_uiCmdIndex < ptrFrameRec->m_uiCmdIndex)
			{
				m_pCommRecFrame->m_uiCmdIndex = ptrFrameRec->m_uiCmdIndex;
				m_pCommRecFrame->OnResetProcBuf();
			}
			// 将帧内容加入待处理任务中
			if (m_pCommRecFrame->m_uiCmdIndex == ptrFrameRec->m_uiCmdIndex)
			{
				SaveRecFrameToTask(ptrFrameRec);
			}
			// 如果为过期命令则不处理只返回应答
			else
			{
			}
		}
		// 如果为应答帧
		else if (ptrFrameRec->m_cCmdType == CmdTypeReturn)
		{
			// 在已发送帧索引中找到该帧并删除
			if (TRUE == m_pCommSndFrame->IfFramePtrExistInSndMap(ptrFrameRec->m_usCmd, 
				ptrFrameRec->m_uiDstTimeStep, ptrFrameRec->m_uiPacketIndex, 
				&m_pCommSndFrame->m_oSndFrameMap))
			{
				ptrFrameSnd = m_pCommSndFrame->GetFramePtrFromSndMap(ptrFrameRec->m_usCmd, 
					ptrFrameRec->m_uiDstTimeStep, ptrFrameRec->m_uiPacketIndex, 
					&m_pCommSndFrame->m_oSndFrameMap);
				m_pCommSndFrame->DeleteFramePtrFromSndMap(ptrFrameRec->m_usCmd, 
					ptrFrameRec->m_uiDstTimeStep, ptrFrameRec->m_uiPacketIndex, 
					&m_pCommSndFrame->m_oSndFrameMap);
				m_pCommSndFrame->AddFreeFrameStruct(ptrFrameSnd);
			}
		}
		m_pCommRecFrame->m_olsCommWorkFrame.pop_front();
		m_pCommRecFrame->AddFreeFrameStruct(ptrFrameRec);
	}
	LeaveCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
	LeaveCriticalSection(&m_pCommRecFrame->m_oSecClientFrame);
	// 监视客户端是否活跃
	MonitorClientActive(bActive);
}
// 将帧内容加入待处理任务中
void CCommRecThread::SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame)
{
	unsigned int uiSaveBegin = 0;
	uiSaveBegin = (ptrFrame->m_uiFrameIndex - 1) * FrameInfoSizeLimit;
	memcpy(&m_pCommRecFrame->m_cProcBuf[uiSaveBegin], ptrFrame->m_pcFrameInfo, ptrFrame->m_usFrameInfoSize);
	// 命令帧接收完全
	if (ptrFrame->m_uiFrameIndex == ptrFrame->m_uiFrameNum)
	{
		// 进行命令处理
		OnProcRecCmd(ptrFrame->m_usCmd, m_pCommRecFrame->m_cProcBuf, uiSaveBegin + ptrFrame->m_usFrameInfoSize);
	}
}
// 处理接收命令函数
void CCommRecThread::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize)
{
	(*m_oProcRecCmdCallBack)(usCmd, pChar, uiSize, this);
}
// 判断仪器位置索引号是否已加入索引表
BOOL CCommRecThread::IfLocationExistInMap(int iLineIndex, int iPointIndex, 
	map<m_oLocationStruct, unsigned int>* pMap)
{
	BOOL bResult = FALSE;
	m_oLocationStruct Location(iLineIndex, iPointIndex);
	map<m_oLocationStruct, unsigned int>::iterator iter;
	iter = pMap->find(Location);
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
void CCommRecThread::AddLocationToMap(int iLineIndex, int iPointIndex, unsigned int uiSN, 
	map<m_oLocationStruct, unsigned int>* pMap)
{
	m_oLocationStruct Location(iLineIndex, iPointIndex);
	if (false == IfLocationExistInMap(iLineIndex, iPointIndex, pMap))
	{
		pMap->insert(map<m_oLocationStruct, unsigned int>::value_type (Location, uiSN));
	}
}

// 根据输入索引号，由索引表得到仪器指针
unsigned int* CCommRecThread::GetSnPtrFromLocationMap(int iLineIndex, int iPointIndex, 
	map<m_oLocationStruct, unsigned int>* pMap)
{
	m_oLocationStruct Location(iLineIndex, iPointIndex);
	map<m_oLocationStruct, unsigned int>::iterator iter;
	iter = pMap->find(Location);
	if (iter == pMap->end())
	{
		return NULL;
	}
	return &(iter->second);
}


/** 监视客户端是否活跃*/
void CCommRecThread::MonitorClientActive(bool bActive)
{
	bool bClose = false;
	if (bActive == true)
	{
		m_uiClientActiveCount = 0;
	}
	else
	{
		m_uiClientActiveCount++;
		if (m_uiClientActiveCount >= ClientActiveCountNum)
		{
			bClose = true;
		}
	}
	if (m_pCommClient->m_bCheckConnected == false)
	{
		m_uiClientCheckCount++;
		if (m_uiClientCheckCount >= ClientCheckCountNum)
		{
			bClose = true;
		}
	}
	if (bClose == true)
	{
		EnterCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
		m_pCommSndFrame->m_bConnectValid = false;
		LeaveCriticalSection(&m_pCommSndFrame->m_oSecClientFrame);
	}
}
