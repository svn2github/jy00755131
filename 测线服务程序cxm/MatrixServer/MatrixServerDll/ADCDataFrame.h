#include "CommandInfo.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCDataFrame_H
#define   _ADCDataFrame_H
// ADC数据接收
typedef struct ADCDataFrame_Struct
{
	// ADC数据接收帧资源同步对象
	CRITICAL_SECTION m_oSecADCDataFrame;
	// 网络端口接收缓冲区大小
	unsigned int m_uiRcvBufferSize;
	// 接收帧缓冲区
	char* m_pcRcvFrameData;
	// 应答帧命令
	m_oInstrumentCommandStruct* m_pCommandStructReturn;
	// 网络端口发送缓冲区大小
	unsigned int m_uiSndBufferSize;
	// 发送帧缓冲区
	char* m_pcSndFrameData;
	// 发送帧命令
	m_oInstrumentCommandStruct* m_pCommandStructSet;
	// ADC数据接收和重发Socket套接字
	SOCKET m_oADCDataFrameSocket;
}m_oADCDataFrameStruct;
#endif