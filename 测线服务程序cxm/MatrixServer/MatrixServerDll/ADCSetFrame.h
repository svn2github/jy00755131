#include "CommandInfo.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCSetFrame_H
#define   _ADCSetFrame_H
// ADC参数设置
typedef struct ADCSetFrame_Struct
{
	// ADC参数设置帧资源同步对象
	CRITICAL_SECTION m_oSecADCSetFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// ADC参数设置应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// ADC参数设置命令字集合
	char* m_pcCommandWord;
	// ADC参数设置命令字个数
	unsigned short m_usCommandWordNum;
	// ADC参数设置发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// ADC参数设置Socket套接字
	SOCKET m_oADCSetFrameSocket;
}m_oADCSetFrameStruct;
#endif