#include "CommandInfo.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _TailTimeFrame_H
#define   _TailTimeFrame_H
// 尾包时刻
typedef struct TailTimeFrame_Struct
{
	// 尾包时刻帧资源同步对象
	CRITICAL_SECTION m_oSecTailTimeFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 尾包时刻应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// 尾包时刻查询命令字集合
	char* m_pcCommandWord;
	// 尾包时刻查询命令字个数
	unsigned short m_usCommandWordNum;
	// 尾包时刻发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 尾包时刻Socket套接字
	SOCKET m_oTailTimeFrameSocket;
}m_oTailTimeFrameStruct;
#endif