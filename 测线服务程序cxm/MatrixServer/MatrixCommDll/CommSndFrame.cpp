#include "stdafx.h"
#include "MatrixCommDll.h"

CCommSndFrame::CCommSndFrame(void)
{
	m_pCommSocket = NULL;
	m_oSndFrameMap.clear();
}


CCommSndFrame::~CCommSndFrame(void)
{
}


// 生成应答帧
void CCommSndFrame::MakeReturnFrame(m_oCommFrameStructPtr ptrFrame)
{
	m_oCommFrameStructPtr pFrameStruct = NULL;
	EnterCriticalSection(&m_oSecClientFrame);
	pFrameStruct = GetFreeFrameStruct();
	pFrameStruct->m_cCmdType = CmdTypeReturn;
	pFrameStruct->m_usCmd = ptrFrame->m_usCmd;
	pFrameStruct->m_uiServerTimeStep = GetTickCount();
	pFrameStruct->m_uiClientTimeStep = ptrFrame->m_uiClientTimeStep;
	pFrameStruct->m_uiPacketIndex = ptrFrame->m_uiPacketIndex;
	pFrameStruct->m_uiCmdIndex = ptrFrame->m_uiCmdIndex;
	pFrameStruct->m_uiFrameNum = ptrFrame->m_uiFrameNum;
	pFrameStruct->m_uiFrameIndex = ptrFrame->m_uiFrameIndex;
	pFrameStruct->m_usFrameInfoSize = ptrFrame->m_usFrameInfoSize;
	memcpy(&pFrameStruct->m_pcFrameInfo, &ptrFrame->m_pcFrameInfo, 
		pFrameStruct->m_usFrameInfoSize);
	m_olsCommWorkFrame.push_back(pFrameStruct);
	LeaveCriticalSection(&m_oSecClientFrame);
}
// 生成设置帧
void CCommSndFrame::MakeSetFrame(unsigned short usCmd, char* pChar, unsigned int uiSize)
{
	m_oCommFrameStructPtr pFrameStruct = NULL;
	unsigned int uiFrameNum = 0;
	uiFrameNum = uiSize / FrameInfoSizeLimit;
	if (uiSize % FrameInfoSizeLimit != 0)
	{
		uiFrameNum++;
	}
	EnterCriticalSection(&m_oSecClientFrame);
	m_uiCmdIndex++;
	for (unsigned int i=0; i<uiFrameNum; i++)
	{
		pFrameStruct = GetFreeFrameStruct();
		pFrameStruct->m_cCmdType = CmdTypeSet;
		pFrameStruct->m_usCmd = usCmd;
		pFrameStruct->m_uiServerTimeStep = GetTickCount();
		pFrameStruct->m_uiClientTimeStep = 0;
		m_uiPacketIndex++;
		pFrameStruct->m_uiPacketIndex = m_uiPacketIndex;
		pFrameStruct->m_uiCmdIndex = m_uiCmdIndex;
		pFrameStruct->m_uiFrameNum = uiFrameNum;
		pFrameStruct->m_uiFrameIndex = i + 1;
		if (uiFrameNum == (i + 1))
		{
			pFrameStruct->m_usFrameInfoSize = (unsigned short)(uiSize - i * FrameInfoSizeLimit);
		}
		else
		{
			pFrameStruct->m_usFrameInfoSize = FrameInfoSizeLimit;
		}
		memcpy(&pFrameStruct->m_pcFrameInfo, &pChar[i * FrameInfoSizeLimit], 
			pFrameStruct->m_usFrameInfoSize);
		m_olsCommWorkFrame.push_back(pFrameStruct);
	}
	LeaveCriticalSection(&m_oSecClientFrame);
}
// 生成发送帧
void CCommSndFrame::MakeSendFrame(m_oCommFrameStructPtr ptrFrame)
{
	char* pChar = m_pCommSocket->m_cSndBuf;
	int iPos = 0;
	unsigned short usCmd = 0;
	unsigned short usFrameLength = 0;
	memset(pChar, 0, FrameSizeLimit);
	pChar[iPos] = FrameHead1;
	iPos++;
	pChar[iPos] = FrameHead2;
	iPos++;
	pChar[iPos] = FrameHead3;
	iPos++;
	pChar[iPos] = FrameHead4;
	iPos++;
	// 帧长度
	usFrameLength = ptrFrame->m_usFrameInfoSize + FrameTailSize + FrameHeadInfoSize;
	memcpy(&pChar[iPos], &usFrameLength, 2);
	iPos += 2;
	// 帧命令字，表明帧的功能
	usCmd = ptrFrame->m_usCmd;
	if (ptrFrame->m_cCmdType == CmdTypeReturn)
	{
		usCmd |= CmdReturnBit;
	}
	memcpy(&pChar[iPos], &usCmd, 2);
	iPos += 2;
	// 服务端时间戳
	memcpy(&pChar[iPos], &ptrFrame->m_uiServerTimeStep, 4);
	iPos += 4;
	// 客户端时间戳
	memcpy(&pChar[iPos], &ptrFrame->m_uiClientTimeStep, 4);
	iPos += 4;
	// 包流水号
	memcpy(&pChar[iPos], &ptrFrame->m_uiPacketIndex, 4);
	iPos += 4;
	// 命令流水号
	memcpy(&pChar[iPos], &ptrFrame->m_uiCmdIndex, 4);
	iPos += 4;
	// 总帧数
	memcpy(&pChar[iPos], &ptrFrame->m_uiFrameNum, 4);
	iPos += 4;
	// 帧序号
	memcpy(&pChar[iPos], &ptrFrame->m_uiFrameIndex, 4);
	iPos += 4;
	// 帧命令执行结果
	memcpy(&pChar[iPos], &ptrFrame->m_cResult, 1);
	iPos += 1;
	memcpy(&pChar[iPos], &ptrFrame->m_pcFrameInfo, ptrFrame->m_usFrameInfoSize);
	iPos += ptrFrame->m_usFrameInfoSize;
	pChar[iPos] = FrameTail;
	iPos++;
	m_pCommSocket->Send(pChar, 
		ptrFrame->m_usFrameInfoSize + FrameHeadSize + FrameLengthSize + FrameTailSize + FrameHeadInfoSize);
}


// 重发过期帧
bool CCommSndFrame::OnReSendFrame(void)
{
	bool bReturn = true;
	map<m_oSndFrameKey, m_oCommFrameStructPtr>::iterator iter;
	for (iter = m_oSndFrameMap.begin(); iter != m_oSndFrameMap.end(); iter++)
	{
		// 超过重发次数则跳出循环并关闭该客户端连接
		if (iter->second->m_uiSndCount > SndFrameMaxNum)
		{
			bReturn = false;
			break;
		}
		else
		{
			// 超过延时时间则重发该帧
			if (iter->second->m_uiTimeOutCount > SndFrameWaitTimes)
			{
				iter->second->m_uiTimeOutCount = 0;
				iter->second->m_uiSndCount++;
				MakeSendFrame(iter->second);
			}
			else
			{
				iter->second->m_uiTimeOutCount++;
			}
		}
	}
	return bReturn;
}

// 判断索引号是否已加入索引表
BOOL CCommSndFrame::IfFramePtrExistInSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
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
void CCommSndFrame::AddFramePtrToSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
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
m_oCommFrameStructPtr CCommSndFrame::GetFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
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
BOOL CCommSndFrame::DeleteFramePtrFromSndMap(unsigned short m_usCmd, unsigned int m_uiServerTimeStep, 
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