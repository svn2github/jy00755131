#include "StdAfx.h"
#include "ClientRecThread.h"


CClientRecThread::CClientRecThread(void)
{
	m_pClientRecFrame = NULL;
	m_pClientSndFrame = NULL;
	m_bCheckConnected = false;
}


CClientRecThread::~CClientRecThread(void)
{
}

// 处理函数
void CClientRecThread::OnProc(void)
{
	int iFrameNum = 0;
	m_oCommFrameStructPtr ptrFrameRec = NULL;
	m_oCommFrameStructPtr ptrFrameSnd = NULL;
	EnterCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
	EnterCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
	iFrameNum = m_pClientRecFrame->m_olsCommWorkFrame.size();
	for (int i=0; i<iFrameNum; i++)
	{
		ptrFrameRec = *m_pClientRecFrame->m_olsCommWorkFrame.begin();
		// 如果为设置帧
		if (ptrFrameRec->m_cCmdType == CmdTypeSet)
		{
			// 如果为新命令则清除未完成的旧命令待处理任务
			if (m_pClientRecFrame->m_uiCmdIndex < ptrFrameRec->m_uiCmdIndex)
			{
				m_pClientRecFrame->m_uiCmdIndex = ptrFrameRec->m_uiCmdIndex;
				m_pClientRecFrame->OnResetProcBuf();
			}
			// 将帧内容加入待处理任务中
			else if (m_pClientRecFrame->m_uiCmdIndex == ptrFrameRec->m_uiCmdIndex)
			{
				SaveRecFrameToTask(ptrFrameRec);
			}
			// 如果为过期命令则不处理只返回应答
			else
			{
			}
			// 发送应答帧
			m_pClientSndFrame->MakeReturnFrame(ptrFrameRec);
		}
		// 如果为应答帧
		else if (ptrFrameRec->m_cCmdType == CmdTypeReturn)
		{
			// 在已发送帧索引中找到该帧并删除
			if (TRUE == m_pClientSndFrame->IfFramePtrExistInSndMap(ptrFrameRec->m_usCmd, 
				ptrFrameRec->m_uiServerTimeStep, ptrFrameRec->m_uiPacketIndex, 
				&m_pClientSndFrame->m_oSndFrameMap))
			{
				ptrFrameSnd = m_pClientSndFrame->GetFramePtrFromSndMap(ptrFrameRec->m_usCmd, 
					ptrFrameRec->m_uiServerTimeStep, ptrFrameRec->m_uiPacketIndex, 
					&m_pClientSndFrame->m_oSndFrameMap);
				m_pClientSndFrame->DeleteFramePtrFromSndMap(ptrFrameRec->m_usCmd, 
					ptrFrameRec->m_uiServerTimeStep, ptrFrameRec->m_uiPacketIndex, 
					&m_pClientSndFrame->m_oSndFrameMap);
				m_pClientSndFrame->AddFreeFrameStruct(ptrFrameSnd);
			}
		}
		m_pClientRecFrame->m_olsCommWorkFrame.pop_front();
		m_pClientRecFrame->AddFreeFrameStruct(ptrFrameRec);
	}
	LeaveCriticalSection(&m_pClientSndFrame->m_oSecClientFrame);
	LeaveCriticalSection(&m_pClientRecFrame->m_oSecClientFrame);
}
// 将帧内容加入待处理任务中
void CClientRecThread::SaveRecFrameToTask(m_oCommFrameStructPtr ptrFrame)
{
	unsigned int uiSaveBegin = 0;
	uiSaveBegin = (ptrFrame->m_uiFrameIndex - 1) * FrameInfoSizeLimit;
	memcpy(&m_pClientRecFrame->m_cProcBuf[uiSaveBegin], ptrFrame->m_pcFrameInfo, ptrFrame->m_usFrameInfoSize);
	// 命令帧接收完全
	if (ptrFrame->m_uiFrameIndex == ptrFrame->m_uiFrameNum)
	{
		// 接收数据长度
		m_pClientRecFrame->m_uiProcBufSize = uiSaveBegin + ptrFrame->m_usFrameInfoSize;
		// 进行命令处理
		OnProcRecCmd(ptrFrame->m_usCmd, m_pClientRecFrame->m_cProcBuf, m_pClientRecFrame->m_uiProcBufSize);
	}
}

// 处理接收命令函数
void CClientRecThread::OnProcRecCmd(unsigned short usCmd, char* pChar, unsigned int uiSize)
{
	// 客户端登陆验证（帧内容为验证码）
	if (usCmd == CmdClientConnect)
	{
		// 与连接校验码相匹配
		if (0 == strcmp(pChar, CommCheck))
		{
			m_bCheckConnected = true;
		}
	}
	else
	{
		if (m_bCheckConnected == false)
		{
			return;
		}
		switch (usCmd)
		{
			// 客户端心跳命令（帧内容为空）
		case CmdSetHeartBeat:
			break;
			// 查询整个仪器表（帧内容为行号+区域号+仪器SN，SN = 0表明无仪器）
		case CmdQueryWholeTable:
			break;
		default:
			break;
		}
	}
}