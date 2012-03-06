#include "LogOutPut.h"
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef    _XML_H
#define    _XML_H

// 测线网络配置XML文件相对路径
#define CommXMLFilePath				_T("..\\parameter\\MatrixLineApp.XML")

// 从XML文件中解析得到的信息
typedef struct InstrumentCommInfo_Struct
{
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;
	// 源地址
	unsigned int m_uiSrcIP;
	// LCI的IP地址
	unsigned int m_uiAimIP;
	// 自动数据返回地址
	unsigned int m_uiADCDataReturnAddr;
	// LCI接收的端口号
	unsigned short m_usAimPort;
	// 心跳帧返回端口
	unsigned short m_usHeartBeatReturnPort;
	// 首包接收端口
	unsigned short m_usHeadFramePort;
	// IP地址设置返回端口
	unsigned short m_usIPSetReturnPort;
	// 尾包接收端口
	unsigned short m_usTailFramePort;
	// 尾包时刻查询接收端口
	unsigned short m_usTailTimeReturnPort;
	// 时统设置应答端口
	unsigned short m_usTimeDelayReturnPort;
	// ADC参数设置应答端口
	unsigned short m_usADCSetReturnPort;
	// 误码查询返回端口
	unsigned short m_usErrorCodeReturnPort;
	// ADC数据返回端口
	unsigned short m_usADCDataReturnPort;
	// 输出日志指针
	m_oLogOutPutStruct* m_pLogOutPut;
}m_oInstrumentCommInfoStruct;
#endif