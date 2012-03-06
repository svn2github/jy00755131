
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _ADCLostFrame_H
#define   _ADCLostFrame_H
// 丢失帧IP地址和偏移量结构体
typedef struct ADCLostFrameKey_Struct
{
	// 丢帧的指针偏移量
	unsigned short m_usADCFramePointNb;
	// 丢帧的IP地址
	unsigned int m_uiIP;
	bool operator == (const ADCLostFrameKey_Struct& rhs) const
	{
		return ((m_uiIP == rhs.m_uiIP) && (m_usADCFramePointNb == rhs.m_usADCFramePointNb));
	}
	bool operator < (const ADCLostFrameKey_Struct& rhs) const
	{
		if (m_uiIP == rhs.m_uiIP)
		{
			return (m_usADCFramePointNb < rhs.m_usADCFramePointNb);
		}
		else
		{
			return (m_uiIP < rhs.m_uiIP);
		}
	}
}m_oADCLostFrameKeyStruct;
// 丢失帧结构体
typedef struct ADCLostFrame_Struct
{
	// 丢失帧重发次数
	unsigned int m_uiCount;
	// 丢帧在文件内的帧序号，从0开始
	unsigned int m_uiFrameNb;
	// 丢失帧的本地时间
	unsigned int m_uiSysTime;
	// 是否已经收到应答
	bool m_bReturnOk;
}m_oADCLostFrameStruct;
#endif