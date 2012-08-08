#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建仪器通讯信息结构体
m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	pCommInfo->m_pLogOutPut = NULL;
	pCommInfo->m_strServerXMLFilePath = "..\\parameter\\MatrixServer.XML";
	pCommInfo->m_strLineXMLFilePath = "..\\parameter\\MatrixLine.XML";
	pCommInfo->m_strOptXMLFilePath = "..\\parameter\\MatrixOperation.XML";

	InitializeCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 初始化服务程序设置信息
	OnInitServerXMLSetupData(pCommInfo);
	// 初始化测线客户程序设置信息
	OnInitLineClientXMLSetupData(pCommInfo);
	// 初始化施工客户程序设置信息
	OnInitOptClientXMLSetupData(pCommInfo);
	return pCommInfo;
}

// 打开程序配置文件
BOOL OpenAppXMLFile(m_oInstrumentCommInfoStruct* pCommInfo,
	string strXMLFilePath)
{
	if (pCommInfo == NULL)
	{
		return FALSE;
	}
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;
	CString str = _T("");
	strOLEObject = _T("msxml2.domdocument");
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 初始化COM库
	CoInitialize(NULL);
	if (FALSE == pCommInfo->m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError))
	{
		AddMsgToLogOutPutList(pCommInfo->m_pLogOutPut, "OpenAppIniXMLFile", "CreateDispatch",
			ErrorType, IDS_ERR_XMLINTERFACE);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
		return FALSE;
	}
	str = strXMLFilePath.c_str();
	oVariant = str;
	if (false == IfFileExist(str))
	{
		AddMsgToLogOutPutList(pCommInfo->m_pLogOutPut, "OpenAppIniXMLFile", "",
			ErrorType, IDS_ERR_FILE_NOEXIST);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
		return FALSE;
	}
	if (FALSE == pCommInfo->m_oXMLDOMDocument.load(oVariant))
	{
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
		return FALSE;
	}
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
	return TRUE;
}

// 初始化仪器通讯信息结构体
void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo, 
	m_oLogOutPutStruct* pLogOutPut)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	pCommInfo->m_pLogOutPut = pLogOutPut;
	// 加载服务器程序设置数据
	LoadServerAppSetupData(pCommInfo);
	// 加载测线客户端程序设置数据
	LoadLineAppSetupData(pCommInfo);
	// 加载施工客户端程序设置数据
	LoadOptAppSetupData(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 关闭程序配置文件
void CloseAppXMLFile(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	pCommInfo->m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 释放仪器通讯信息结构体
void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	// 释放服务端参数设置信息结构体缓冲区
	OnFreeServerXMLSetupData(pCommInfo);
	// 保存测线客户端程序设置数据
	SaveLineAppSetupData(pCommInfo);
	// 重置测线客户端信息
	OnResetLineClientXMLSetupData(pCommInfo);
	// 保存施工客户端程序设置数据
	SaveOptAppSetupData(pCommInfo);
	// 重置施工客户端信息
	OnResetOptClientXMLSetupData(pCommInfo);
	DeleteCriticalSection(&pCommInfo->m_oSecCommInfo);
	delete pCommInfo;
	pCommInfo = NULL;
}

