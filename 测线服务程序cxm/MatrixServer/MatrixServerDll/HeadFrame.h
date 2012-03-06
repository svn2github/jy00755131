#include "CommandInfo.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _HeadFrame_H
#define   _HeadFrame_H
// 首包
typedef struct HeadFrame_Struct
{
	// 首包帧资源同步对象
	CRITICAL_SECTION m_oSecHeadFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 首包帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 首包Socket套接字
	SOCKET m_oHeadFrameSocket;
}m_oHeadFrameStruct;
#endif