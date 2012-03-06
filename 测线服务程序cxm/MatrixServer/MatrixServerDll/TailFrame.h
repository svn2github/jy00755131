#include "CommandInfo.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _TailFrame_H
#define   _TailFrame_H

// 尾包
typedef struct TailFrame_Struct
{
	// 尾包帧资源同步对象
	CRITICAL_SECTION m_oSecTailFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 尾包帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 尾包Socket套接字
	SOCKET m_oTailFrameSocket;
}m_oTailFrameStruct;
#endif