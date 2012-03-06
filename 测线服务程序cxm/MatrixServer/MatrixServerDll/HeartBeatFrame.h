#include "CommandInfo.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _HeartBeatFrame_H
#define   _HeartBeatFrame_H

// 心跳
typedef struct HeartBeatFrame_Struct
{
	// 心跳帧资源同步对象
	CRITICAL_SECTION m_oSecHeartBeat;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// 心跳命令字集合
	char* m_pcCommandWord;
	// 心跳命令字个数
	unsigned short m_usCommandWordNum;
	// 心跳帧命令
	m_oInstrumentCommandStruct* m_pCommandStruct;
	// 心跳Socket套接字
	SOCKET m_oHeartBeatSocket;
}m_oHeartBeatFrameStruct;

#endif