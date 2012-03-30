#include "stdafx.h"
#include "MatrixServerDll.h"

// 创建仪器通讯信息结构体
m_oInstrumentCommInfoStruct* OnCreateInstrumentCommInfo(void)
{
	m_oInstrumentCommInfoStruct* pCommInfo = NULL;
	pCommInfo = new m_oInstrumentCommInfoStruct;
	pCommInfo->m_pLogOutPut = NULL;
	pCommInfo->m_cpSetADCSetSine = NULL;
	pCommInfo->m_cpSetADCStopSample = NULL;
	pCommInfo->m_cpSetADCOpenTBPowerLow = NULL;
	pCommInfo->m_cpSetADCOpenTBPowerHigh = NULL;
	pCommInfo->m_cpSetADCOpenSwitchTBLow = NULL;
	pCommInfo->m_cpSetADCOpenSwitchTBHigh = NULL;
	pCommInfo->m_cpSetADCRegisterRead = NULL;
	pCommInfo->m_cpSetADCRegisterWrite = NULL;
	pCommInfo->m_cpSetADCTBSwitchOpen = NULL;
	pCommInfo->m_cpSetADCSample = NULL;
	pCommInfo->m_cpSetADCReadContinuous = NULL;
	InitializeCriticalSection(&pCommInfo->m_oSecCommInfo);
	return pCommInfo;
}
// 打开程序配置文件
BOOL OpenAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo,
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
		pCommInfo->m_uiSrcIP = inet_addr(strConv.c_str());
		// LCI的IP地址
		strKey = _T("IPLCI");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ConvertCStrToStr(csDstIP, &strConv);
		pCommInfo->m_uiAimIP = inet_addr(strConv.c_str());
		// ADC数据返回地址
		strKey = _T("IPForADCData");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ConvertCStrToStr(csDstIP, &strConv);
		pCommInfo->m_uiADCDataReturnAddr = inet_addr(strConv.c_str());
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
		pCommInfo->m_usAimPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeartBeat");
		pCommInfo->m_usHeartBeatReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeadFrame");
		pCommInfo->m_usHeadFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForIPSet");
		pCommInfo->m_usIPSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailFrame");
		pCommInfo->m_usTailFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailTimeFrame");
		pCommInfo->m_usTailTimeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTimeSet");
		pCommInfo->m_usTimeDelayReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCSet");
		pCommInfo->m_usADCSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForErrorCode");
		pCommInfo->m_usErrorCodeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCData");
		pCommInfo->m_usADCDataReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
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
		pCommInfo->m_iSetADCSetSineSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置正弦波命令
		strKey = _T("SetADCSetSine");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCSetSine, pCommInfo->m_iSetADCSetSineSize, strValue);

		// ADC设置停止采样命令大小
		strKey = _T("SetADCStopSampleSize");
		pCommInfo->m_iSetADCStopSampleSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置停止采样命令
		strKey = _T("SetADCStopSample");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCStopSample, pCommInfo->m_iSetADCStopSampleSize, strValue);

		// ADC设置打开TB电源低位大小
		strKey = _T("SetADCOpenTBPowerLowSize");
		pCommInfo->m_iSetADCOpenTBPowerLowSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB电源低位
		strKey = _T("SetADCOpenTBPowerLow");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCOpenTBPowerLow, pCommInfo->m_iSetADCOpenTBPowerLowSize, strValue);
	
		// ADC设置打开TB电源高位大小
		strKey = _T("SetADCOpenTBPowerHighSize");
		pCommInfo->m_iSetADCOpenTBPowerHighSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB电源高位
		strKey = _T("SetADCOpenTBPowerHigh");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCOpenTBPowerHigh, pCommInfo->m_iSetADCOpenTBPowerHighSize, strValue);
	
		// ADC设置打开TB开关低位大小
		strKey = _T("SetADCOpenSwitchTBLowSize");
		pCommInfo->m_iSetADCOpenSwitchTBLowSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关低位
		strKey = _T("SetADCOpenSwitchTBLow");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCOpenSwitchTBLow, pCommInfo->m_iSetADCOpenSwitchTBLowSize, strValue);
	
		// ADC设置打开TB开关高位大小
		strKey = _T("SetADCOpenSwitchTBHighSize");
		pCommInfo->m_iSetADCOpenSwitchTBHighSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关高位
		strKey = _T("SetADCOpenSwitchTBHigh");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCOpenSwitchTBHigh, pCommInfo->m_iSetADCOpenSwitchTBHighSize, strValue);
	
		// ADC设置读寄存器大小
		strKey = _T("SetADCRegisterReadSize");
		pCommInfo->m_iSetADCRegisterReadSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置读寄存器
		strKey = _T("SetADCRegisterRead");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCRegisterRead, pCommInfo->m_iSetADCRegisterReadSize, strValue);

		// ADC设置写寄存器大小
		strKey = _T("SetADCRegisterWriteSize");
		pCommInfo->m_iSetADCRegisterWriteSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置写寄存器
		strKey = _T("SetADCRegisterWrite");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCRegisterWrite, pCommInfo->m_iSetADCRegisterWriteSize, strValue);

		// ADC设置打开TB开关大小
		strKey = _T("SetADCTBSwitchOpenSize");
		pCommInfo->m_iSetADCTBSwitchOpenSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关
		strKey = _T("SetADCTBSwitchOpen");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCTBSwitchOpen, pCommInfo->m_iSetADCTBSwitchOpenSize, strValue);
		
		// ADC采样设置大小
		strKey = _T("SetADCSampleSize");
		pCommInfo->m_iSetADCSampleSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC采样设置
		strKey = _T("SetADCSample");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCSample, pCommInfo->m_iSetADCSampleSize, strValue);
		
		// ADC设置连续采样大小
		strKey = _T("SetADCReadContinuousSize");
		pCommInfo->m_iSetADCReadContinuousSize = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置连续采样
		strKey = _T("SetADCReadContinuous");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pCommInfo->m_cpSetADCReadContinuous, pCommInfo->m_iSetADCReadContinuousSize, strValue);
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
void LoadLineServerAppSetupData(m_oInstrumentCommInfoStruct* pCommInfo,
	string strXMLFilePath)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppIniXMLFile(pCommInfo, strXMLFilePath))
	{
		//加载IP地址设置数据
		LoadIPSetupData(pCommInfo);
		//加载端口设置数据
		LoadPortSetupData(pCommInfo);
		//加载ADC参数设置数据
		LoadADCSetData(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppIniXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 初始化仪器通讯信息结构体
void OnInitInstrumentCommInfo(m_oInstrumentCommInfoStruct* pCommInfo, 
	string strXMLFilePath, m_oLogOutPutStruct* pLogOutPut)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	pCommInfo->m_pLogOutPut = pLogOutPut;
	LoadLineServerAppSetupData(pCommInfo, strXMLFilePath);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 关闭程序配置文件
void CloseAppIniXMLFile(m_oInstrumentCommInfoStruct* pCommInfo)
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
	if (pCommInfo->m_cpSetADCSetSine != NULL)
	{
		delete[] pCommInfo->m_cpSetADCSetSine;
		pCommInfo->m_cpSetADCSetSine = NULL;
	}
	if (pCommInfo->m_cpSetADCStopSample != NULL)
	{
		delete[] pCommInfo->m_cpSetADCStopSample;
		pCommInfo->m_cpSetADCStopSample = NULL;
	}
	if (pCommInfo->m_cpSetADCOpenTBPowerLow != NULL)
	{
		delete[] pCommInfo->m_cpSetADCOpenTBPowerLow;
		pCommInfo->m_cpSetADCOpenTBPowerLow = NULL;
	}
	if (pCommInfo->m_cpSetADCOpenTBPowerHigh != NULL)
	{
		delete[] pCommInfo->m_cpSetADCOpenTBPowerHigh;
		pCommInfo->m_cpSetADCOpenTBPowerHigh = NULL;
	}
	if (pCommInfo->m_cpSetADCOpenSwitchTBLow != NULL)
	{
		delete[] pCommInfo->m_cpSetADCOpenSwitchTBLow;
		pCommInfo->m_cpSetADCOpenSwitchTBLow = NULL;
	}
	if (pCommInfo->m_cpSetADCOpenSwitchTBHigh != NULL)
	{
		delete[] pCommInfo->m_cpSetADCOpenSwitchTBHigh;
		pCommInfo->m_cpSetADCOpenSwitchTBHigh = NULL;
	}
	if (pCommInfo->m_cpSetADCRegisterRead != NULL)
	{
		delete[] pCommInfo->m_cpSetADCRegisterRead;
		pCommInfo->m_cpSetADCRegisterRead = NULL;
	}
	if (pCommInfo->m_cpSetADCRegisterWrite != NULL)
	{
		delete[] pCommInfo->m_cpSetADCRegisterWrite;
		pCommInfo->m_cpSetADCRegisterWrite = NULL;
	}
	if (pCommInfo->m_cpSetADCTBSwitchOpen != NULL)
	{
		delete[] pCommInfo->m_cpSetADCTBSwitchOpen;
		pCommInfo->m_cpSetADCTBSwitchOpen = NULL;
	}
	if (pCommInfo->m_cpSetADCSample != NULL)
	{
		delete[] pCommInfo->m_cpSetADCSample;
		pCommInfo->m_cpSetADCSample = NULL;
	}
	if (pCommInfo->m_cpSetADCReadContinuous != NULL)
	{
		delete[] pCommInfo->m_cpSetADCReadContinuous;
		pCommInfo->m_cpSetADCReadContinuous = NULL;
	}
	DeleteCriticalSection(&pCommInfo->m_oSecCommInfo);
	delete pCommInfo;
}

