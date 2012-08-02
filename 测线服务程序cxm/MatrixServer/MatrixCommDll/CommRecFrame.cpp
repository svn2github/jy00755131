#include "stdafx.h"
#include "MatrixCommDll.h"

CCommRecFrame::CCommRecFrame(void)
{
}


CCommRecFrame::~CCommRecFrame(void)
{
}

// 解析帧
void CCommRecFrame::PhraseFrame(char* cpFrame, unsigned short usSize)
{
	int iPos = 0;
	unsigned short usCmd = 0;
	m_oCommFrameStructPtr pFrameStruct = NULL;
	EnterCriticalSection(&m_oSecClientFrame);
	pFrameStruct = GetFreeFrameStruct();
	// 帧内容长度
	pFrameStruct->m_usFrameInfoSize = usSize;
	memcpy(&usCmd, &cpFrame[iPos], 2);
	iPos += 2;
	// 帧命令类型
	if (usCmd & CmdReturnBit)
	{
		pFrameStruct->m_cCmdType = CmdTypeReturn;
	}
	else
	{
		pFrameStruct->m_cCmdType = CmdTypeSet;
	}
	// 帧命令字，表明帧的功能
	pFrameStruct->m_usCmd = usCmd & (~CmdReturnBit);
	// 服务端时间戳
	memcpy(&pFrameStruct->m_uiSrcTimeStep, &cpFrame[iPos], 4);
	iPos += 4;
	// 客户端时间戳
	memcpy(&pFrameStruct->m_uiDstTimeStep, &cpFrame[iPos], 4);
	iPos += 4;
	// 包流水号
	memcpy(&pFrameStruct->m_uiPacketIndex, &cpFrame[iPos], 4);
	iPos += 4;
	// 命令流水号
	memcpy(&pFrameStruct->m_uiCmdIndex, &cpFrame[iPos], 4);
	iPos += 4;
	// 总帧数
	memcpy(&pFrameStruct->m_uiFrameNum, &cpFrame[iPos], 4);
	iPos += 4;
	// 帧序号
	memcpy(&pFrameStruct->m_uiFrameIndex, &cpFrame[iPos], 4);
	iPos += 4;
	// 帧命令执行结果
	memcpy(&pFrameStruct->m_cResult, &cpFrame[iPos], 1);
	iPos += 1;
	// 帧内容,如果为查询命令则帧内容的前两个字节分别定义行号和区域号
	memcpy(&pFrameStruct->m_pcFrameInfo, &cpFrame[iPos], usSize);
	iPos += FrameHeadInfoSize;
	m_olsCommWorkFrame.push_back(pFrameStruct);
	LeaveCriticalSection(&m_oSecClientFrame);
}