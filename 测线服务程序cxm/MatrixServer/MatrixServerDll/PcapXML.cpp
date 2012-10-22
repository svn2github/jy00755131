#include "stdafx.h"
#include "MatrixServerDll.h"
// 初始化Pcap程序配置信息结构体
void OnInitPcapXMLSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	InitializeCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	pPcapSetupData->m_strPcapXMLFilePath = "..\\parameter\\MatrixServer.XML";
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 创建Pcap程序信息结构体
m_oNetPcapSetupDataStruct* OnCreatePcapAppSetupData(void)
{
	m_oNetPcapSetupDataStruct* pPcapSetupData = NULL;
	pPcapSetupData = new m_oNetPcapSetupDataStruct;
	// 初始化Pcap程序信息结构体
	OnInitPcapXMLSetupData(pPcapSetupData);
	return pPcapSetupData;
}
// 打开Pcap程序配置文件
BOOL OpenPcapXMLFile(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;
	CString str = _T("");
	strOLEObject = _T("msxml2.domdocument");
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	// 初始化COM库
	CoInitialize(NULL);
	if (FALSE == pPcapSetupData->m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError))
	{
		AfxMessageBox(_T("XML结构创建失败！"));
		LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
		return FALSE;
	}
	str = pPcapSetupData->m_strPcapXMLFilePath.c_str();
	oVariant = str;
	if (false == IfFileExist(str))
	{
		AfxMessageBox(_T("文件不存在"));
		LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
		return FALSE;
	}
	if (FALSE == pPcapSetupData->m_oXMLDOMDocument.load(oVariant))
	{
		LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
		return FALSE;
	}
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	return TRUE;
}
// 关闭Pcap程序配置文件
void ClosePcapXMLFile(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	pPcapSetupData->m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载IP设置数据
void LoadPcapIP(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	CString strKey;
	string strConv = "";
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	try
	{
		// 找到NetIPSetup设置区
		strKey = "NetIPSetup";
		lpDispatch = pPcapSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = "LowIP";
		pPcapSetupData->m_oXMLIPSetupData.m_strLowIP = (CStringA)CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		strKey = "HighIP";
		pPcapSetupData->m_oXMLIPSetupData.m_strHighIP = (CStringA)CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
	}
	catch (CMemoryException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载PcapIP设置数据
void LoadPcapIPSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenPcapXMLFile(pPcapSetupData))
	{
		// 加载IP设置数据
		LoadPcapIP(pPcapSetupData);
	}
	// 关闭程序配置文件
	ClosePcapXMLFile(pPcapSetupData);
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载端口设置数据
void LoadPcapPort(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	CString strKey;
	string strConv = "";
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	try
	{
		// 找到NetPortSetup设置区
		strKey = "NetPortSetup";
		lpDispatch = pPcapSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = "NetDownStreamSrcPort";
		pPcapSetupData->m_oXMLPortSetupData.m_usDownStreamPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = "NetUpStreamSrcPort";
		pPcapSetupData->m_oXMLPortSetupData.m_usUpStreamPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
	}
	catch (CMemoryException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载Pcap端口设置数据
void LoadPcapPortSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenPcapXMLFile(pPcapSetupData))
	{
		// 加载端口设置数据
		LoadPcapPort(pPcapSetupData);
	}
	// 关闭程序配置文件
	ClosePcapXMLFile(pPcapSetupData);
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载参数设置数据
void LoadPcapParam(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	CString strKey;
	string strConv = "";
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	try
	{
		// 找到NetParameterSetup设置区
		strKey = "NetParameterSetup";
		lpDispatch = pPcapSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = "NetAppPath";
		pPcapSetupData->m_oPcapParamSetupData.m_strPath = (CStringA)CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		strKey = "NetCardId";
		pPcapSetupData->m_oPcapParamSetupData.m_usNetCardID = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = "WinpcapBufSize";
		pPcapSetupData->m_oPcapParamSetupData.m_uiPcapBufSize = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "DownStreamSndBufSize";
		pPcapSetupData->m_oPcapParamSetupData.m_uiDownStreamSndBufSize = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "UpStreamSndBufSize";
		pPcapSetupData->m_oPcapParamSetupData.m_uiUpStreamSndBufSize = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "MaxPackageSize";
		pPcapSetupData->m_oPcapParamSetupData.m_uiPcapMaxPackageSize = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "PcapTimeOut";
		pPcapSetupData->m_oPcapParamSetupData.m_uiPcapTimeOut = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "PcapSndWaitTime";
		pPcapSetupData->m_oPcapParamSetupData.m_uiPcapSndWaitTime = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "PcapRcvWaitTime";
		pPcapSetupData->m_oPcapParamSetupData.m_uiPcapRcvWaitTime = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
		strKey = "PcapQueueSize";
		pPcapSetupData->m_oPcapParamSetupData.m_uiPcapQueueSize = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	}
	catch (CMemoryException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_MEMORY_EXCEPTION);
	}
	catch (CFileException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_FILE_EXCEPTION);
	}
	catch (CException* e)
	{
		e->ReportError(MB_OK, IDS_ERR_OTHER_EXCEPTION);
	}
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载Pcap参数设置数据
void LoadPcapParamSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
	EnterCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenPcapXMLFile(pPcapSetupData))
	{
		// 加载参数设置数据
		LoadPcapParam(pPcapSetupData);
	}
	// 关闭程序配置文件
	ClosePcapXMLFile(pPcapSetupData);
	LeaveCriticalSection(&pPcapSetupData->m_oSecCommInfo);
}
// 加载Pcap程序配置信息
void LoadPcapAppSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	ASSERT(pPcapSetupData != NULL);
 	// 加载IP数据
 	LoadPcapIPSetupData(pPcapSetupData);
  	// 加载端口数据
  	LoadPcapPortSetupData(pPcapSetupData);
  	// 加载参数数据
  	LoadPcapParamSetupData(pPcapSetupData);
}

// 释放Pcap程序配置信息结构体缓冲区
void OnFreePcapXMLSetupData(m_oNetPcapSetupDataStruct* pPcapSetupData)
{
	DeleteCriticalSection(&pPcapSetupData->m_oSecCommInfo);
	delete pPcapSetupData;
	pPcapSetupData = NULL;
}