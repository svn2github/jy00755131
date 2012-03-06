#include "CommandInfo.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _TimeDelayFrame_H
#define   _TimeDelayFrame_H
// 时统设置
typedef struct TimeDelayFrame_Struct
{
	// 时统设置帧资源同步对象
	CRITICAL_SECTION m_oSecTimeDelayFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 时统设置应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// 时统设置命令字集合
	char* m_pcCommandWord;
	// 时统设置命令字个数
	unsigned short m_usCommandWordNum;
	// 时统设置发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// 时统设置Socket套接字
	SOCKET m_oTimeDelayFrameSocket;
}m_oTimeDelayFrameStruct;

#endif