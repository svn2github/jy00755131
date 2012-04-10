#include "StdAfx.h"
#include "ClientRecFrame.h"


CClientRecFrame::CClientRecFrame(void)
{
}


CClientRecFrame::~CClientRecFrame(void)
{
}
// 重置接收帧结构体
void CClientRecFrame::OnResetRecFrameStruct(m_oCommFrameStructPtr pFrameStruct)
{
	memset(pFrameStruct, 0, sizeof(m_oCommFrameStruct));
}
// 初始化
void CClientRecFrame::OnInit(void)
{
	m_uiCountFree = RecFrameStructNumMax;
	m_olsCommFrameStructFree.clear();
	InitializeCriticalSection(&m_oSecClientRecFrame);
	EnterCriticalSection(&m_oSecClientRecFrame);
	m_olsCommRecFrame.clear();
	LeaveCriticalSection(&m_oSecClientRecFrame);
	for(unsigned int i = 0; i < RecFrameStructNumMax; i++)
	{
		// 重置接收帧结构体
		OnResetRecFrameStruct(&m_oCommFrameArray[i]);
		// 仪器加在空闲接收帧结构体队列尾部
		m_olsCommFrameStructFree.push_back(&m_oCommFrameArray[i]);
	}
}

// 得到一个空闲接收帧结构体
m_oCommFrameStructPtr CClientRecFrame::GetFreeRecFrameStruct(void)
{
	m_oCommFrameStructPtr pFrameStruct = NULL;
	list <m_oCommFrameStructPtr>::iterator iter;
	if(m_uiCountFree > 0)	//有空闲接收帧结构体
	{
		// 从空闲队列中得到一个接收帧结构体
		iter = m_olsCommFrameStructFree.begin();
		pFrameStruct = *iter;
		m_olsCommFrameStructFree.pop_front();	
		m_uiCountFree--;	// 空闲接收帧结构体总数减1
	}
	return pFrameStruct;
}
// 增加一个空闲接收帧结构体
void CClientRecFrame::AddFreeRecFrameStruct(m_oCommFrameStructPtr pFrameStruct)
{
	//初始化接收帧结构体
	OnResetRecFrameStruct(pFrameStruct);
	//加入空闲队列
	m_olsCommFrameStructFree.push_back(pFrameStruct);
	m_uiCountFree++;	// 空闲接收帧结构体总数加1
}
// 关闭
void CClientRecFrame::OnClose(void)
{
	m_olsCommFrameStructFree.clear();
	EnterCriticalSection(&m_oSecClientRecFrame);
	m_olsCommRecFrame.clear();
	LeaveCriticalSection(&m_oSecClientRecFrame);
}

// 解析帧
void CClientRecFrame::PhraseFrame(char* cpFrame, unsigned short usSize)
{
	int iPos = 0;
	unsigned short usCmd = 0;
	m_oCommFrameStructPtr pFrameStruct = NULL;
	pFrameStruct = GetFreeRecFrameStruct();
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
	pFrameStruct->m_usCmd = usCmd & (!CmdReturnBit);
	// 服务端时间戳
	memcpy(&pFrameStruct->m_uiServerTimeStep, &cpFrame[iPos], 4);
	iPos += 4;
	// 客户端时间戳
	memcpy(&pFrameStruct->m_uiClientTimeStep, &cpFrame[iPos], 4);
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
	memcpy(&pFrameStruct->m_pcFrameInfo, &cpFrame[iPos], usSize - 27);
	iPos += 27;
	EnterCriticalSection(&m_oSecClientRecFrame);
	m_olsCommRecFrame.push_back(pFrameStruct);
	LeaveCriticalSection(&m_oSecClientRecFrame);
}