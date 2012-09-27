#include "stdafx.h"
#include "MatrixServerDll.h"

// 初始化服务程序设置信息
void OnInitServerXMLSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	InitializeCriticalSection(&pServerSetupData->m_oSecCommInfo);
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	pServerSetupData->m_strServerXMLFilePath = "..\\parameter\\MatrixServer.XML";
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSetSine = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterRead = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterWrite = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_cpSetADCReadContinuous = NULL;
	pServerSetupData->m_oXMLADCSetupData.m_iSampleRate = 1000;	// 默认1K采样率
	pServerSetupData->m_oXMLADCSetupData.m_bHPFOpen = true;	// 高通滤波器默认开启
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 创建服务端通讯信息结构体
m_oServerSetupDataStruct* OnCreateServerAppSetupData(void)
{
	m_oServerSetupDataStruct* pServerSetupData = NULL;
	pServerSetupData = new m_oServerSetupDataStruct;
	// 初始化服务程序设置信息
	OnInitServerXMLSetupData(pServerSetupData);
	return pServerSetupData;
}
// 打开服务程序配置文件
BOOL OpenServerXMLFile(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return FALSE;
	}
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;
	CString str = _T("");
	strOLEObject = _T("msxml2.domdocument");
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	// 初始化COM库
	CoInitialize(NULL);
	if (FALSE == pServerSetupData->m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError))
	{
		AfxMessageBox(_T("XML结构创建失败！"));
		LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
		return FALSE;
	}
	str = pServerSetupData->m_strServerXMLFilePath.c_str();
	oVariant = str;
	if (false == IfFileExist(str))
	{
		AfxMessageBox(_T("文件不存在"));
		LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
		return FALSE;
	}
	if (FALSE == pServerSetupData->m_oXMLDOMDocument.load(oVariant))
	{
		LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
		return FALSE;
	}
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
	return TRUE;
}
// 关闭服务程序配置文件
void CloseServerXMLFile(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	pServerSetupData->m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载IP地址设置数据
void LoadServerIP(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
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
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	try
	{
		// 找到IP地址设置区
		strKey = _T("ServerAppIPSetup");
		lpDispatch = pServerSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 和现场仪器通讯的本机IP地址
		strKey = _T("ServerIP");
		csSrcIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		strConv = (CStringA)csSrcIP;
		pServerSetupData->m_oXMLIPSetupData.m_uiSrcIP = inet_addr(strConv.c_str());
		// LCI的IP地址
		strKey = _T("IPLCI");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		strConv = (CStringA)csDstIP;
		pServerSetupData->m_oXMLIPSetupData.m_uiAimIP = inet_addr(strConv.c_str());
		// ADC数据返回地址
		strKey = _T("IPForADCDataReturn");
		csDstIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		strConv = (CStringA)csDstIP;
		pServerSetupData->m_oXMLIPSetupData.m_uiADCDataReturnAddr = inet_addr(strConv.c_str());
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
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载端口设置数据
void LoadServerPort(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	try
	{
		// 找到IP地址设置区
		strKey = _T("ServerAppPortSetSetup");
		lpDispatch = pServerSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = _T("PortLCI");
		pServerSetupData->m_oXMLPortSetupData.m_usAimPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeartBeatSet");
		pServerSetupData->m_oXMLPortSetupData.m_usHeartBeatReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForHeadFrameSet");
		pServerSetupData->m_oXMLPortSetupData.m_usHeadFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForIPSetSet");
		pServerSetupData->m_oXMLPortSetupData.m_usIPSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailFrameSet");
		pServerSetupData->m_oXMLPortSetupData.m_usTailFramePort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTailTimeFrameSet");
		pServerSetupData->m_oXMLPortSetupData.m_usTailTimeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForTimeSetSet");
		pServerSetupData->m_oXMLPortSetupData.m_usTimeDelayReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCSetSet");
		pServerSetupData->m_oXMLPortSetupData.m_usADCSetReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForErrorCodeSet");
		pServerSetupData->m_oXMLPortSetupData.m_usErrorCodeReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
		strKey = _T("PortForADCDataSet");
		pServerSetupData->m_oXMLPortSetupData.m_usADCDataReturnPort = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
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
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载ADC参数设置数据
void LoadServerADCSet(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	CString strKey = _T("");
	CString strValue = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	try
	{
		// 找到ADC参数设置区
		strKey = _T("ServerAppADCSetSetup");
		lpDispatch = pServerSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// ADC设置正弦波命令大小
		strKey = _T("SetADCSetSineSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCSetSineSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置正弦波命令
		strKey = _T("SetADCSetSine");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSetSine, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCSetSineSize, strValue);

		// ADC设置停止采样命令大小
		strKey = _T("SetADCStopSampleSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置停止采样命令
		strKey = _T("SetADCStopSample");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCStopSampleSize, strValue);

		// ADC设置打开TB电源低位大小
		strKey = _T("SetADCOpenTBPowerLowSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenTBPowerLowSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB电源低位
		strKey = _T("SetADCOpenTBPowerLow");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenTBPowerLowSize, strValue);

		// ADC设置打开TB电源高位大小
		strKey = _T("SetADCOpenTBPowerHighSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenTBPowerHighSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB电源高位
		strKey = _T("SetADCOpenTBPowerHigh");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenTBPowerHighSize, strValue);

		// ADC设置打开TB开关低位大小
		strKey = _T("SetADCOpenSwitchTBLowSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBLowSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关低位
		strKey = _T("SetADCOpenSwitchTBLow");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBLowSize, strValue);

		// ADC设置打开TB开关高位大小
		strKey = _T("SetADCOpenSwitchTBHighSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBHighSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关高位
		strKey = _T("SetADCOpenSwitchTBHigh");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCOpenSwitchTBHighSize, strValue);

		// ADC设置读寄存器大小
		strKey = _T("SetADCRegisterReadSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCRegisterReadSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置读寄存器
		strKey = _T("SetADCRegisterRead");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterRead, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCRegisterReadSize, strValue);

		// ADC设置写寄存器大小
		strKey = _T("SetADCRegisterWriteSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCRegisterWriteSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置写寄存器
		strKey = _T("SetADCRegisterWrite");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterWrite, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCRegisterWriteSize, strValue);

		// ADC设置打开TB开关大小
		strKey = _T("SetADCTBSwitchOpenSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCTBSwitchOpenSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置打开TB开关
		strKey = _T("SetADCTBSwitchOpen");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCTBSwitchOpenSize, strValue);

		// ADC采样设置大小
		strKey = _T("SetADCSampleSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCSampleSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC采样设置
		strKey = _T("SetADCSample");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCSampleSize, strValue);

		// ADC设置连续采样大小
		strKey = _T("SetADCReadContinuousSize");
		pServerSetupData->m_oXMLADCSetupData.m_iSetADCReadContinuousSize = 
			CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

		// ADC设置连续采样
		strKey = _T("SetADCReadContinuous");
		strValue = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		ParseCStringToArray(&pServerSetupData->m_oXMLADCSetupData.m_cpSetADCReadContinuous, 
			pServerSetupData->m_oXMLADCSetupData.m_iSetADCReadContinuousSize, strValue);
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
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载服务端参数设置数据
void LoadServerParameter(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	CString strKey;
	CString str = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	int nYear,nMonth,nDay,nHour,nMin,nSec;
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	try
	{
		// 找到参数设置区
		strKey = _T("ServerAppParameterSetup");
		lpDispatch = pServerSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = _T("FieldOffTime");
		str = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		_stscanf_s(str, _T("%d.%d.%d %d:%d:%d"), &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
		CTime time(nYear, nMonth, nDay, nHour, nMin, nSec);
		pServerSetupData->m_oXMLParameterSetupData.m_oTimeFieldOff = time;
		strKey = _T("NetRcvPortMove");
		pServerSetupData->m_oXMLParameterSetupData.m_usNetRcvPortMove = CXMLDOMTool::GetElementAttributeUnsignedShort(&oElement, strKey);
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
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 保存服务端参数设置数据
void SaveServerParameter(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	CString strKey;
	CString str = _T("");
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	COleVariant oVariant;
	CTime time;
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	try
	{
		// 找到参数设置区
		strKey = _T("ServerAppParameterSetup");
		lpDispatch = pServerSetupData->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = _T("FieldOffTime");
		time = pServerSetupData->m_oXMLParameterSetupData.m_oTimeFieldOff;
		str.Format(_T("%d.%d.%d %d:%d:%d"), time.GetYear(), time.GetMonth(), time.GetDay(),
			time.GetHour(), time.GetMinute(), time.GetSecond());
		oVariant = str;
		oElement.setAttribute(strKey, oVariant);
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
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载IP地址设置数据
void LoadServerIPSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenServerXMLFile(pServerSetupData))
	{
		// 加载IP地址设置数据
		LoadServerIP(pServerSetupData);
	}
	// 关闭程序配置文件
	CloseServerXMLFile(pServerSetupData);
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载端口设置数据
void LoadServerPortSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenServerXMLFile(pServerSetupData))
	{
		// 加载端口设置数据
		LoadServerPort(pServerSetupData);
	}
	// 关闭程序配置文件
	CloseServerXMLFile(pServerSetupData);
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载ADC参数设置数据
void LoadServerADCSetSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenServerXMLFile(pServerSetupData))
	{
		// 加载ADC参数设置数据
		LoadServerADCSet(pServerSetupData);
	}
	// 关闭程序配置文件
	CloseServerXMLFile(pServerSetupData);
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载服务器端参数设置数据
void LoadServerParameterSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenServerXMLFile(pServerSetupData))
	{
		// 加载服务端参数设置数据
		LoadServerParameter(pServerSetupData);
	}
	// 关闭程序配置文件
	CloseServerXMLFile(pServerSetupData);
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 保存服务器端参数设置数据
void SaveServerParameterSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	COleVariant oVariant;
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenServerXMLFile(pServerSetupData))
	{
		// 保存服务器端参数设置数据
		SaveServerParameter(pServerSetupData);
	}
	oVariant = (CString)(pServerSetupData->m_strServerXMLFilePath.c_str());
	pServerSetupData->m_oXMLDOMDocument.save(oVariant);
	// 关闭程序配置文件
	CloseServerXMLFile(pServerSetupData);
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
}
// 加载服务器程序设置数据
void LoadServerAppSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	// 加载IP地址设置数据
	LoadServerIPSetupData(pServerSetupData);
	// 加载端口设置数据
	LoadServerPortSetupData(pServerSetupData);
	// 加载ADC参数设置数据
	LoadServerADCSetSetupData(pServerSetupData);
	// 加载服务器端参数设置数据
	LoadServerParameterSetupData(pServerSetupData);
}
// 释放服务端参数设置信息结构体缓冲区
void OnFreeServerXMLSetupData(m_oServerSetupDataStruct* pServerSetupData)
{
	if (pServerSetupData == NULL)
	{
		return;
	}
	EnterCriticalSection(&pServerSetupData->m_oSecCommInfo);
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSetSine != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSetSine;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSetSine = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCStopSample = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerLow = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenTBPowerHigh = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBLow = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCOpenSwitchTBHigh = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterRead != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterRead;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterRead = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterWrite != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterWrite;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCRegisterWrite = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCTBSwitchOpen = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCSample = NULL;
	}
	if (pServerSetupData->m_oXMLADCSetupData.m_cpSetADCReadContinuous != NULL)
	{
		delete[] pServerSetupData->m_oXMLADCSetupData.m_cpSetADCReadContinuous;
		pServerSetupData->m_oXMLADCSetupData.m_cpSetADCReadContinuous = NULL;
	}
	LeaveCriticalSection(&pServerSetupData->m_oSecCommInfo);
	DeleteCriticalSection(&pServerSetupData->m_oSecCommInfo);
	delete pServerSetupData;
	pServerSetupData = NULL;
}