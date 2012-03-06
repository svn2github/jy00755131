
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCDataBuf_H
#define   _ADCDataBuf_H

// 数据存储缓冲区结构体
typedef struct ADCDataBuf_Struct
{
	// 缓冲区是否使用中
	bool m_bInUsed;
	// ADC数据存储缓冲区
	int* m_pADCDataBuf;
	// 数据存储帧序号，从0开始
	unsigned int m_uiFrameNb;
	// 采样仪器SN
	unsigned int m_uiSN;
	// 帧的本地时间
	unsigned int m_uiSysTime;
	// 缓冲区序号
	unsigned int m_uiIndex;
}m_oADCDataBufStruct;
#endif