#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建仪器通讯信息结构体
m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	// 初始化服务程序设置信息
	OnInitServerXMLSetupData(&pCommInfo->m_oServerSetupData);
	// 初始化测线客户程序设置信息
	OnInitLineClientXMLSetupData(&pCommInfo->m_oLineSetupData);
	// 初始化施工客户程序设置信息
	OnInitOptClientXMLSetupData(&pCommInfo->m_oOptSetupData);
	return pCommInfo;
}

// 初始化仪器通讯信息结构体
void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	// 加载服务器程序设置数据
	LoadServerAppSetupData(&pCommInfo->m_oServerSetupData);
	// 加载测线客户端程序设置数据
	LoadLineAppSetupData(&pCommInfo->m_oLineSetupData);
	// 加载施工客户端程序设置数据
	LoadOptAppSetupData(&pCommInfo->m_oOptSetupData);
}
// 释放仪器通讯信息结构体
void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	// 释放服务端参数设置信息结构体缓冲区
	OnFreeServerXMLSetupData(&pCommInfo->m_oServerSetupData);
	// 保存测线客户端程序设置数据
	SaveLineAppSetupData(&pCommInfo->m_oLineSetupData);
	// 重置测线客户端信息
	OnResetLineClientXMLSetupData(&pCommInfo->m_oLineSetupData);
	// 保存施工客户端程序设置数据
	SaveOptAppSetupData(&pCommInfo->m_oOptSetupData);
	// 重置施工客户端信息
	OnResetOptClientXMLSetupData(&pCommInfo->m_oOptSetupData);
	DeleteCriticalSection(&pCommInfo->m_oSecCommInfo);
	delete pCommInfo;
	pCommInfo = NULL;
}

