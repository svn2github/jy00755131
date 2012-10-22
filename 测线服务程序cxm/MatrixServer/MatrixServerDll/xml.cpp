#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建仪器通讯信息结构体
m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	// 创建服务端通讯信息结构体
	pCommInfo->m_pServerSetupData = OnCreateServerAppSetupData();
	// 创建测线客户端通讯信息结构体
	pCommInfo->m_pLineSetupData = OnCreateLineAppSetupData();
	// 创建施工客户端通讯信息结构体
	pCommInfo->m_pOptSetupData = OnCreateOptAppSetupData();
	// 创建Pcap程序信息结构体
	pCommInfo->m_pPcapSetupData = OnCreatePcapAppSetupData();
	return pCommInfo;
}

// 初始化仪器通讯信息结构体
void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	ASSERT(pCommInfo != NULL);
	// 加载服务器程序设置数据
	LoadServerAppSetupData(pCommInfo->m_pServerSetupData);
	// 加载测线客户端程序设置数据
	LoadLineAppSetupData(pCommInfo->m_pLineSetupData);
	// 加载施工客户端程序设置数据
	LoadOptAppSetupData(pCommInfo->m_pOptSetupData);
	// 加载Pcap程序设置数据
	LoadPcapAppSetupData(pCommInfo->m_pPcapSetupData);
}
// 释放仪器通讯信息结构体
void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	ASSERT(pCommInfo != NULL);
	// 释放服务端参数设置信息结构体缓冲区
	OnFreeServerXMLSetupData(pCommInfo->m_pServerSetupData);
	// 保存测线客户端程序设置数据
	SaveLineAppSetupData(pCommInfo->m_pLineSetupData);
	// 释放测线客户端参数设置信息结构体缓冲区
	OnFreeLineXMLSetupData(pCommInfo->m_pLineSetupData);
	// 保存施工客户端程序设置数据
	SaveOptAppSetupData(pCommInfo->m_pOptSetupData);
	// 释放施工客户端参数设置信息结构体缓冲区
	OnFreeOptXMLSetupData(pCommInfo->m_pOptSetupData);
	// 释放Pcap程序设置信息结构体缓冲区
	OnFreePcapXMLSetupData(pCommInfo->m_pPcapSetupData);
	delete pCommInfo;
	pCommInfo = NULL;
}

