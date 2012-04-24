#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建仪器通讯信息结构体
m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	pCommInfo->m_pLogOutPut = NULL;
	pCommInfo->m_strDllXMLFilePath = "";
	pCommInfo->m_strLineXMLFilePath = "";
	pCommInfo->m_strOptXMLFilePath = "";
	// 初始化ADC参数设置信息
	OnInitXMLADCSetupData(&pCommInfo->m_oXMLADCSetupData);
	InitializeCriticalSection(&pCommInfo->m_oSecCommInfo);
	return pCommInfo;
}
// 初始化ADC参数设置信息
void OnInitXMLADCSetupData(m_oXMLADCSetupDataStruct* pXMLADCSetupData)
{
	pXMLADCSetupData->m_cpSetADCSetSine = NULL;
	pXMLADCSetupData->m_cpSetADCStopSample = NULL;
	pXMLADCSetupData->m_cpSetADCOpenTBPowerLow = NULL;
	pXMLADCSetupData->m_cpSetADCOpenTBPowerHigh = NULL;
	pXMLADCSetupData->m_cpSetADCOpenSwitchTBLow = NULL;
	pXMLADCSetupData->m_cpSetADCOpenSwitchTBHigh = NULL;
	pXMLADCSetupData->m_cpSetADCRegisterRead = NULL;
	pXMLADCSetupData->m_cpSetADCRegisterWrite = NULL;
	pXMLADCSetupData->m_cpSetADCTBSwitchOpen = NULL;
	pXMLADCSetupData->m_cpSetADCSample = NULL;
	pXMLADCSetupData->m_cpSetADCReadContinuous = NULL;
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
//加载IP地址设置数据
void LoadIPSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	CString csSrcIP = _T("");
	CString csDstIP = _T("");
	string strConv = "";
	try
	{
		EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
		// 找到IP地址设置区
		strKey = _T("LineServerAppIPSetup");
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 和现场仪器通讯的本机IP地址
		strKey = _T("IPForInstrument");
		csSrcIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ConvertCStrToStr(csSrcIP, &strConv);
		pCommInfo->m_oXMLIPSetupData.m_uiSrcIP = inet_addr(strConv.c_str());
		// LCI的IP地址
		strKey = _T("IPLCI");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ConvertCStrToStr(csDstIP, &strConv);
		pCommInfo->m_oXMLIPSetupData.m_uiAimIP = inet_addr(strConv.c_str());
		// ADC数据返回地址
		strKey = _T("IPForADCData");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ConvertCStrToStr(csDstIP, &strConv);
		pCommInfo->m_oXMLIPSetupData.m_uiADCDataReturnAddr = inet_addr(strConv.c_str());
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
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
}
//加载端口设置数据
void LoadPortSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	try
	{
		EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
		// 找到IP地址设置区
		strKey = _T("LineServerAppPortSetup");
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = _T("PortLCI");
		pCommInfo->m_oXMLPortSetupData.m_usAimPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeartBeat");
		pCommInfo->m_oXMLPortSetupData.m_usHeartBeatReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeadFrame");
		pCommInfo->m_oXMLPortSetupData.m_usHeadFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForIPSet");
		pCommInfo->m_oXMLPortSetupData.m_usIPSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailFrame");
		pCommInfo->m_oXMLPortSetupData.m_usTailFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailTimeFrame");
		pCommInfo->m_oXMLPortSetupData.m_usTailTimeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTimeSet");
		pCommInfo->m_oXMLPortSetupData.m_usTimeDelayReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCSet");
		pCommInfo->m_oXMLPortSetupData.m_usADCSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForErrorCode");
		pCommInfo->m_oXMLPortSetupData.m_usErrorCodeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCData");
		pCommInfo->m_oXMLPortSetupData.m_usADCDataReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
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
}
//加载ADC参数设置数据
void LoadADCSetData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey = _T("");
	CString strValue = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	try
	{
		EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
		// 找到ADC参数设置区
		strKey = _T("ADCSetSetup");
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// ADC设置正弦波命令大小
		strKey = _T("SetADCSetSineSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCSetSineSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置正弦波命令
		strKey = _T("SetADCSetSine");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCSetSine, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCSetSineSize, strValue);

		// ADC设置停止采样命令大小
		strKey = _T("SetADCStopSampleSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置停止采样命令
		strKey = _T("SetADCStopSample");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCStopSample, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCStopSampleSize, strValue);

		// ADC设置打开TB电源低位大小
		strKey = _T("SetADCOpenTBPowerLowSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenTBPowerLowSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB电源低位
		strKey = _T("SetADCOpenTBPowerLow");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenTBPowerLowSize, strValue);
	
		// ADC设置打开TB电源高位大小
		strKey = _T("SetADCOpenTBPowerHighSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenTBPowerHighSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB电源高位
		strKey = _T("SetADCOpenTBPowerHigh");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenTBPowerHighSize, strValue);
	
		// ADC设置打开TB开关低位大小
		strKey = _T("SetADCOpenSwitchTBLowSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBLowSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关低位
		strKey = _T("SetADCOpenSwitchTBLow");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBLowSize, strValue);
	
		// ADC设置打开TB开关高位大小
		strKey = _T("SetADCOpenSwitchTBHighSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBHighSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关高位
		strKey = _T("SetADCOpenSwitchTBHigh");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBHighSize, strValue);
	
		// ADC设置读寄存器大小
		strKey = _T("SetADCRegisterReadSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCRegisterReadSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置读寄存器
		strKey = _T("SetADCRegisterRead");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCRegisterRead, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCRegisterReadSize, strValue);

		// ADC设置写寄存器大小
		strKey = _T("SetADCRegisterWriteSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCRegisterWriteSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置写寄存器
		strKey = _T("SetADCRegisterWrite");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCRegisterWrite, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCRegisterWriteSize, strValue);

		// ADC设置打开TB开关大小
		strKey = _T("SetADCTBSwitchOpenSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCTBSwitchOpenSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关
		strKey = _T("SetADCTBSwitchOpen");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCTBSwitchOpenSize, strValue);
		
		// ADC采样设置大小
		strKey = _T("SetADCSampleSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCSampleSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC采样设置
		strKey = _T("SetADCSample");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCSample, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCSampleSize, strValue);
		
		// ADC设置连续采样大小
		strKey = _T("SetADCReadContinuousSize");
		pCommInfo->m_oXMLADCSetupData.m_iSetADCReadContinuousSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置连续采样
		strKey = _T("SetADCReadContinuous");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_oXMLADCSetupData.m_cpSetADCReadContinuous, 
			pCommInfo->m_oXMLADCSetupData.m_iSetADCReadContinuousSize, strValue);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
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
}
//加载测线服务器程序设置数据
void LoadServerAppSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strDllXMLFilePath))
	{
		//加载IP地址设置数据
		LoadIPSetupData(pCommInfo);
		//加载端口设置数据
		LoadPortSetupData(pCommInfo);
		//加载ADC参数设置数据
		LoadADCSetData(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
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
	LoadServerAppSetupData(pCommInfo);
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
// 释放ADC参数设置信息结构体缓冲区
void OnFreeXMLADCSetupData(m_oXMLADCSetupDataStruct* pXMLADCSetupData)
{
	if (pXMLADCSetupData == NULL)
	{
		return;
	}
	if (pXMLADCSetupData->m_cpSetADCSetSine != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCSetSine;
		pXMLADCSetupData->m_cpSetADCSetSine = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCStopSample != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCStopSample;
		pXMLADCSetupData->m_cpSetADCStopSample = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCOpenTBPowerLow != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCOpenTBPowerLow;
		pXMLADCSetupData->m_cpSetADCOpenTBPowerLow = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCOpenTBPowerHigh != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCOpenTBPowerHigh;
		pXMLADCSetupData->m_cpSetADCOpenTBPowerHigh = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCOpenSwitchTBLow != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCOpenSwitchTBLow;
		pXMLADCSetupData->m_cpSetADCOpenSwitchTBLow = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCOpenSwitchTBHigh != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCOpenSwitchTBHigh;
		pXMLADCSetupData->m_cpSetADCOpenSwitchTBHigh = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCRegisterRead != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCRegisterRead;
		pXMLADCSetupData->m_cpSetADCRegisterRead = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCRegisterWrite != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCRegisterWrite;
		pXMLADCSetupData->m_cpSetADCRegisterWrite = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCTBSwitchOpen != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCTBSwitchOpen;
		pXMLADCSetupData->m_cpSetADCTBSwitchOpen = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCSample != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCSample;
		pXMLADCSetupData->m_cpSetADCSample = NULL;
	}
	if (pXMLADCSetupData->m_cpSetADCReadContinuous != NULL)
	{
		delete[] pXMLADCSetupData->m_cpSetADCReadContinuous;
		pXMLADCSetupData->m_cpSetADCReadContinuous = NULL;
	}
}
// 释放仪器通讯信息结构体
void OnFreeInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	OnFreeXMLADCSetupData(&pCommInfo->m_oXMLADCSetupData);
	DeleteCriticalSection(&pCommInfo->m_oSecCommInfo);
	delete pCommInfo;
}

