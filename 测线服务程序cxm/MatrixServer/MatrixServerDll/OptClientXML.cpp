#include "stdafx.h"
#include "MatrixServerDll.h"

// 初始化施工客户程序设置信息
void OnInitOptClientXMLSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
// 	pCommInfo->m_oLineSetupData.m_oSeisMonitor.m_pcAbsoluteSpread = NULL;
	// 重置施工客户端信息
	OnResetOptClientXMLSetupData(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置炮点队列
void OnResetOptSourceShotList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oSourceShotStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsSourceShot.begin(); 
		iter != pCommInfo->m_oOptSetupData.m_olsSourceShot.end(); iter++)
	{
		if (iter->m_pcComments != NULL)
		{
			delete[] iter->m_pcComments;
			iter->m_pcComments = NULL;
		}
	}
	pCommInfo->m_oOptSetupData.m_olsSourceShot.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置Explo震源类型队列
void OnResetOptExploList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oSourceExploStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsExploStruct.begin(); 
		iter != pCommInfo->m_oOptSetupData.m_olsExploStruct.end(); iter++)
	{
		if (iter->m_pcComments != NULL)
		{
			delete[] iter->m_pcComments;
			iter->m_pcComments = NULL;
		}
		if (iter->m_pcLabel != NULL)
		{
			delete[] iter->m_pcLabel;
			iter->m_pcLabel = NULL;
		}
	}
	pCommInfo->m_oOptSetupData.m_olsExploStruct.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置Vibro震源类型队列
void OnResetOptVibroList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oSourceVibroStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsVibroStruct.begin(); 
		iter != pCommInfo->m_oOptSetupData.m_olsVibroStruct.end(); iter++)
	{
		if (iter->m_pcComments != NULL)
		{
			delete[] iter->m_pcComments;
			iter->m_pcComments = NULL;
		}
		if (iter->m_pcLabel != NULL)
		{
			delete[] iter->m_pcLabel;
			iter->m_pcLabel = NULL;
		}
	}
	pCommInfo->m_oOptSetupData.m_olsVibroStruct.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置处理类型Aux队列
void OnResetOptProcessAuxList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oProcessAuxStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsProcessAuxStruct.begin(); 
		iter != pCommInfo->m_oOptSetupData.m_olsProcessAuxStruct.end(); iter++)
	{
		if (iter->m_pcAuxProcessing != NULL)
		{
			delete[] iter->m_pcAuxProcessing;
			iter->m_pcAuxProcessing = NULL;
		}
	}
	pCommInfo->m_oOptSetupData.m_olsProcessAuxStruct.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置处理类型Acq队列
void OnResetOptProcessAcqList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	pCommInfo->m_oOptSetupData.m_olsProcessAcqStruct.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置处理类型队列
void OnResetOptProcessTypeList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oProcessTypeStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsProcessTypeStruct.begin(); 
		iter != pCommInfo->m_oOptSetupData.m_olsProcessTypeStruct.end(); iter++)
	{
		if (iter->m_pcLabel != NULL)
		{
			delete[] iter->m_pcLabel;
			iter->m_pcLabel = NULL;
		}
	}
	pCommInfo->m_oOptSetupData.m_olsProcessTypeStruct.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置注释队列
void OnResetOptCommentList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oOperationCommentStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsComment.begin(); 
		iter != pCommInfo->m_oOptSetupData.m_olsComment.end(); iter++)
	{
		if (iter->m_pcComments != NULL)
		{
			delete[] iter->m_pcComments;
			iter->m_pcComments = NULL;
		}
	}
	pCommInfo->m_oOptSetupData.m_olsComment.clear();
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 重置施工客户端信息
void OnResetOptClientXMLSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	// 重置炮点队列
	OnResetOptSourceShotList(pCommInfo);
	// 重置Explo震源类型队列
	OnResetOptExploList(pCommInfo);
	// 重置Vibro震源类型队列
	OnResetOptVibroList(pCommInfo);
	// 重置处理类型Aux队列
	OnResetOptProcessAuxList(pCommInfo);
	// 重置处理类型Acq队列
	OnResetOptProcessAcqList(pCommInfo);
	// 重置处理类型队列
	OnResetOptProcessTypeList(pCommInfo);
	// 重置注释队列
	OnResetOptCommentList(pCommInfo);
}
// 加载Delay设置数据
void LoadDelay(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	string strConv = "";
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到Operation Delay设置区
		strKey = "OperationDelay";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = "VPSlipTime";
		pCommInfo->m_oOptSetupData.m_oDelay.m_iVPSlipTime = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		strKey = "VPTimeMax";
		pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMax = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		strKey = "VPTimeMin";
		pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMin = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		strKey = "AcqSlipTime";
		pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqSlipTime = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		strKey = "AcqTimeMax";
		pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMax = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		strKey = "AcqTimeMin";
		pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMin = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 加载Delay设置数据
void LoadOptDelaySetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 加载Delay设置数据
		LoadDelay(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存Delay设置数据
void SaveDelay(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	string strConv = "";
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到Delay设置区
		strKey = "OperationDelay";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		strKey = "VPSlipTime";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_oDelay.m_iVPSlipTime;
		oElement.setAttribute(strKey, oVariant);
		strKey = "VPTimeMax";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMax;
		oElement.setAttribute(strKey, oVariant);
		strKey = "VPTimeMin";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMin;
		oElement.setAttribute(strKey, oVariant);
		strKey = "AcqSlipTime";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqSlipTime;
		oElement.setAttribute(strKey, oVariant);
		strKey = "AcqTimeMax";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMax;
		oElement.setAttribute(strKey, oVariant);
		strKey = "AcqTimeMin";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMin;
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存Delay设置数据
void SaveDelaySetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	COleVariant oVariant;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 保存Delay设置数据
		SaveDelay(pCommInfo);
	}
	oVariant = (CString)(pCommInfo->m_strOptXMLFilePath.c_str());
	pCommInfo->m_oXMLDOMDocument.save(oVariant);
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 设置Delay设置数据
void SetDelaySetupData(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo)
{
	unsigned int uiPos = 0;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	while(uiPos < uiSize)
	{
		memcpy(&pCommInfo->m_oOptSetupData.m_oDelay.m_iVPSlipTime, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMax, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMin, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqSlipTime, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMax, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMin, &pChar[uiPos], 4);
		uiPos += 4;
	}
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
	SaveDelaySetupData(pCommInfo);
}
// 查询 Delay XML文件信息
void QueryDelaySetupData(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo)
{
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	memcpy(&cProcBuf[iPos], &pCommInfo->m_oOptSetupData.m_oDelay.m_iVPSlipTime, 4);
	iPos += 4;
	memcpy(&cProcBuf[iPos], &pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMax, 4);
	iPos += 4;
	memcpy(&cProcBuf[iPos], &pCommInfo->m_oOptSetupData.m_oDelay.m_iVPTimeMin, 4);
	iPos += 4;
	memcpy(&cProcBuf[iPos], &pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqSlipTime, 4);
	iPos += 4;
	memcpy(&cProcBuf[iPos], &pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMax, 4);
	iPos += 4;
	memcpy(&cProcBuf[iPos], &pCommInfo->m_oOptSetupData.m_oDelay.m_iAcqTimeMin, 4);
	iPos += 4;
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}

// 加载SourceShot设置数据
void LoadSourceShot(m_oSourceShotStruct* pSourceShotStruct,CXMLDOMElement* pElement)
{
	CString strKey = _T("");
	CString str = _T("");
	string strConv = "";
	try
	{
		strKey = "VPStatus";
		pSourceShotStruct->m_uiVPStatus = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "ShotID";
		pSourceShotStruct->m_uiShotID = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "BreakPoint";
		pSourceShotStruct->m_uiBreakPoint = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "SourceLine";
		pSourceShotStruct->m_fSourceLine = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
		strKey = "SourceReceiver";
		pSourceShotStruct->m_fSourceReceiver = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
		strKey = "SpreadSFL";
		pSourceShotStruct->m_uiSFL = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "SpreadSFN";
		pSourceShotStruct->m_uiSFN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "SpreadType";
		pSourceShotStruct->m_uiSpreadType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "SuperSpread";
		pSourceShotStruct->m_iSuperSpread = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
		strKey = "ProcessTypeID";
		pSourceShotStruct->m_uiProcessType = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "Comments";
		str = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strConv = (CStringA)str;
		pSourceShotStruct->m_usCommentsSize = (unsigned short)(strConv.size() + 1);
		pSourceShotStruct->m_pcComments = new char[pSourceShotStruct->m_usCommentsSize];
		memcpy(pSourceShotStruct->m_pcComments, strConv.c_str(), pSourceShotStruct->m_usCommentsSize);
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
// 加载SourceShot设置队列数据
void LoadSourceShotList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int uiCountAll;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到VPTable设置区
		strKey = "VPTable";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到SourceShot总数
		strKey = "Count";
		uiCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		m_oSourceShotStruct oSourceShotStruct;
		for(unsigned int i = 0; i < uiCountAll; i++)
		{
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			LoadSourceShot(&oSourceShotStruct, &oElement);
			// 增加SourceShot
			pCommInfo->m_oOptSetupData.m_olsSourceShot.push_back(oSourceShotStruct);
		}
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 加载SourceShot设置数据
void LoadSourceShotSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 重置SourceShot
	OnResetOptSourceShotList(pCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 加载SourceShot设置队列数据
		LoadSourceShotList(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存SourceShot设置数据
void SaveSourceShot(m_oSourceShotStruct* pSourceShotStruct,CXMLDOMElement* pElement)
{
	CString strKey = _T("");
	COleVariant oVariant;
	CString str = _T("");
	string strConv = "";
	try
	{
		strKey = "VPStatus";
		oVariant = (long)pSourceShotStruct->m_uiVPStatus;
		pElement->setAttribute(strKey, oVariant);
		strKey = "ShotID";
		oVariant = (long)pSourceShotStruct->m_uiShotID;
		pElement->setAttribute(strKey, oVariant);
		strKey = "BreakPoint";
		oVariant = (long)pSourceShotStruct->m_uiBreakPoint;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SourceLine";
		oVariant = (long)pSourceShotStruct->m_fSourceLine;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SourceReceiver";
		oVariant = (long)pSourceShotStruct->m_fSourceReceiver;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SpreadSFL";
		oVariant = (long)pSourceShotStruct->m_uiSFL;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SpreadSFN";
		oVariant = (long)pSourceShotStruct->m_uiSFN;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SpreadType";
		oVariant = (long)pSourceShotStruct->m_uiSpreadType;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SuperSpread";
		oVariant = (long)pSourceShotStruct->m_iSuperSpread;
		pElement->setAttribute(strKey, oVariant);
		strKey = "ProcessTypeID";
		oVariant = (long)pSourceShotStruct->m_uiProcessType;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Comments";
		strConv = pSourceShotStruct->m_pcComments;
		str = strConv.c_str();
		oVariant = str;
		pElement->setAttribute(strKey, oVariant);
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
// 保存SourceShot设置队列数据
void SaveSourceShotList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;
	unsigned int uiTabCount = 0;
	CString strTabChild = _T("");
	CString strTabParent = _T("");
	list<m_oSourceShotStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到SourceShot设置区
		strKey = "VPTable";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 得到Tab键数量
		strKey = "TabCount";
		uiTabCount = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElementParent, strKey);
		strTabChild = _T("\r\n");
		strTabParent = _T("\r\n");
		for(unsigned int i = 0; i < uiTabCount; i++)
		{
			strTabChild += _T("\t");
			if (i < (uiTabCount - 1))
			{
				strTabParent += _T("\t");
			}
		}
		// 设置SourceShot总数
		strKey = "Count";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_olsSourceShot.size();
		oElementParent.setAttribute(strKey, oVariant);
		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		for (iter = pCommInfo->m_oOptSetupData.m_olsSourceShot.begin();
			iter != pCommInfo->m_oOptSetupData.m_olsSourceShot.end(); iter++)
		{
			lpDispatch = pCommInfo->m_oXMLDOMDocument.createTextNode(strTabChild);
			oElementParent.appendChild(lpDispatch);
			lpDispatch = pCommInfo->m_oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);
			SaveSourceShot(&(*iter), &oElementChild);
			oElementParent.appendChild(lpDispatch);		
		}
		lpDispatch = pCommInfo->m_oXMLDOMDocument.createTextNode(strTabParent);
		oElementParent.appendChild(lpDispatch);
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存SourceShot设置数据
void SaveSourceShotSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	COleVariant oVariant;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 保存SourceShot设置队列数据
		SaveSourceShotList(pCommInfo);
	}
	oVariant = (CString)(pCommInfo->m_strOptXMLFilePath.c_str());
	pCommInfo->m_oXMLDOMDocument.save(oVariant);
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 设置SourceShot设置数据
void SetSourceShotSetupData(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo)
{
	m_oSourceShotStruct oSourceShotStruct;
	unsigned int uiPos = 0;
	OnResetOptSourceShotList(pCommInfo);
	while(uiPos < uiSize)
	{
		memcpy(&oSourceShotStruct.m_uiVPStatus, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_uiShotID, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_uiBreakPoint, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_fSourceLine, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_fSourceReceiver, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_uiSFL, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_uiSFN, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_uiSpreadType, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_iSuperSpread, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_uiProcessType, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceShotStruct.m_usCommentsSize, &pChar[uiPos], 2);
		uiPos += 2;
		oSourceShotStruct.m_pcComments = new char[oSourceShotStruct.m_usCommentsSize];
		memcpy(&oSourceShotStruct.m_pcComments, &pChar[uiPos], oSourceShotStruct.m_usCommentsSize);
		uiPos += oSourceShotStruct.m_usCommentsSize;
		EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
		pCommInfo->m_oOptSetupData.m_olsSourceShot.push_back(oSourceShotStruct);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
	}
	SaveSourceShotSetupData(pCommInfo);
}
// 查询 SourceShot XML文件信息
void QuerySourceShotSetupData(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oSourceShotStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsSourceShot.begin();
		iter != pCommInfo->m_oOptSetupData.m_olsSourceShot.end(); iter++)
	{
		memcpy(&cProcBuf[iPos], &iter->m_uiVPStatus, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiShotID, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiBreakPoint, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_fSourceLine, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_fSourceReceiver, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiSFL, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiSFN, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiSpreadType, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_iSuperSpread, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiProcessType, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_usCommentsSize, 2);
		iPos += 2;
		memcpy(&cProcBuf[iPos], &iter->m_pcComments, iter->m_usCommentsSize);
		iPos += iter->m_usCommentsSize;
	}
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}

// 加载Explo设置数据
void LoadExplo(m_oSourceExploStruct* pSourceExploStruct,CXMLDOMElement* pElement)
{
	CString strKey = _T("");
	CString str = _T("");
	string strConv = "";
	try
	{
		strKey = "SourceStatus";
		pSourceExploStruct->m_uiSourceStatus = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "SourceNb";
		pSourceExploStruct->m_uiSourceNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "Label";
		str = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strConv = (CStringA)str;
		pSourceExploStruct->m_usLabelSize = (unsigned short)(strConv.size() + 1);
		pSourceExploStruct->m_pcLabel = new char[pSourceExploStruct->m_usLabelSize];
		memcpy(pSourceExploStruct->m_pcLabel, strConv.c_str(), pSourceExploStruct->m_usLabelSize);
		strKey = "ShooterSN";
		pSourceExploStruct->m_uiShooterSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "IncrNb";
		pSourceExploStruct->m_iIncrNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
		strKey = "Comments";
		str = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strConv = (CStringA)str;
		pSourceExploStruct->m_usCommentsSize = (unsigned short)(strConv.size() + 1);
		pSourceExploStruct->m_pcComments = new char[pSourceExploStruct->m_usCommentsSize];
		memcpy(pSourceExploStruct->m_pcComments, strConv.c_str(), pSourceExploStruct->m_usCommentsSize);
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
// 加载Explo设置队列数据
void LoadExploList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int uiCountAll;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到Explo设置区
		strKey = "SourceExplo";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到Explo总数
		strKey = "Count";
		uiCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		m_oSourceExploStruct oSourceExploStruct;
		for(unsigned int i = 0; i < uiCountAll; i++)
		{
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			LoadExplo(&oSourceExploStruct, &oElement);
			// 增加Explo
			pCommInfo->m_oOptSetupData.m_olsExploStruct.push_back(oSourceExploStruct);
		}
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 加载Explo设置数据
void LoadExploSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 重置Explo
	OnResetOptExploList(pCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 加载Explo设置队列数据
		LoadExploList(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存Explo设置数据
void SaveExplo(m_oSourceExploStruct* pSourceExploStruct,CXMLDOMElement* pElement)
{
	CString strKey = _T("");
	COleVariant oVariant;
	CString str = _T("");
	string strConv = "";
	try
	{
		strKey = "SourceStatus";
		oVariant = (long)pSourceExploStruct->m_uiSourceStatus;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SourceNb";
		oVariant = (long)pSourceExploStruct->m_uiSourceNb;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Label";
		strConv = pSourceExploStruct->m_pcLabel;
		str = strConv.c_str();
		oVariant = str;
		pElement->setAttribute(strKey, oVariant);
		strKey = "ShooterSN";
		oVariant = (long)pSourceExploStruct->m_uiShooterSN;
		pElement->setAttribute(strKey, oVariant);
		strKey = "IncrNb";
		oVariant = (long)pSourceExploStruct->m_iIncrNb;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Comments";
		strConv = pSourceExploStruct->m_pcComments;
		str = strConv.c_str();
		oVariant = str;
		pElement->setAttribute(strKey, oVariant);
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
// 保存Explo设置队列数据
void SaveExploList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;
	unsigned int uiTabCount = 0;
	CString strTabChild = _T("");
	CString strTabParent = _T("");
	list<m_oSourceExploStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到Explo设置区
		strKey = "SourceExplo";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 得到Tab键数量
		strKey = "TabCount";
		uiTabCount = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElementParent, strKey);
		strTabChild = _T("\r\n");
		strTabParent = _T("\r\n");
		for(unsigned int i = 0; i < uiTabCount; i++)
		{
			strTabChild += _T("\t");
			if (i < (uiTabCount - 1))
			{
				strTabParent += _T("\t");
			}
		}
		// 设置Explo总数
		strKey = "Count";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_olsExploStruct.size();
		oElementParent.setAttribute(strKey, oVariant);
		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		for (iter = pCommInfo->m_oOptSetupData.m_olsExploStruct.begin();
			iter != pCommInfo->m_oOptSetupData.m_olsExploStruct.end(); iter++)
		{
			lpDispatch = pCommInfo->m_oXMLDOMDocument.createTextNode(strTabChild);
			oElementParent.appendChild(lpDispatch);
			lpDispatch = pCommInfo->m_oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);
			SaveExplo(&(*iter), &oElementChild);
			oElementParent.appendChild(lpDispatch);		
		}
		lpDispatch = pCommInfo->m_oXMLDOMDocument.createTextNode(strTabParent);
		oElementParent.appendChild(lpDispatch);
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存Explo设置数据
void SaveExploSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	COleVariant oVariant;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 保存Explo设置队列数据
		SaveExploList(pCommInfo);
	}
	oVariant = (CString)(pCommInfo->m_strOptXMLFilePath.c_str());
	pCommInfo->m_oXMLDOMDocument.save(oVariant);
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 设置Explo设置数据
void SetExploSetupData(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo)
{
	m_oSourceExploStruct oSourceExploStruct;
	unsigned int uiPos = 0;
	OnResetOptExploList(pCommInfo);
	while(uiPos < uiSize)
	{
		memcpy(&oSourceExploStruct.m_uiSourceStatus, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceExploStruct.m_uiSourceNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceExploStruct.m_usLabelSize, &pChar[uiPos], 2);
		uiPos += 2;
		oSourceExploStruct.m_pcLabel = new char[oSourceExploStruct.m_usLabelSize];
		memcpy(&oSourceExploStruct.m_pcLabel, &pChar[uiPos], oSourceExploStruct.m_usLabelSize);
		uiPos += oSourceExploStruct.m_usLabelSize;
		memcpy(&oSourceExploStruct.m_uiShooterSN, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceExploStruct.m_iIncrNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceExploStruct.m_usCommentsSize, &pChar[uiPos], 2);
		uiPos += 2;
		oSourceExploStruct.m_pcComments = new char[oSourceExploStruct.m_usCommentsSize];
		memcpy(&oSourceExploStruct.m_pcComments, &pChar[uiPos], oSourceExploStruct.m_usCommentsSize);
		uiPos += oSourceExploStruct.m_usCommentsSize;
		EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
		pCommInfo->m_oOptSetupData.m_olsExploStruct.push_back(oSourceExploStruct);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
	}
	SaveExploSetupData(pCommInfo);
}
// 查询 Explo XML文件信息
void QueryExploSetupData(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oSourceExploStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsExploStruct.begin();
		iter != pCommInfo->m_oOptSetupData.m_olsExploStruct.end(); iter++)
	{
		memcpy(&cProcBuf[iPos], &iter->m_uiSourceStatus, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiSourceNb, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_usLabelSize, 2);
		iPos += 2;
		memcpy(&cProcBuf[iPos], &iter->m_pcLabel, iter->m_usLabelSize);
		iPos += iter->m_usLabelSize;
		memcpy(&cProcBuf[iPos], &iter->m_uiShooterSN, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_iIncrNb, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_usCommentsSize, 2);
		iPos += 2;
		memcpy(&cProcBuf[iPos], &iter->m_pcComments, iter->m_usCommentsSize);
		iPos += iter->m_usCommentsSize;
	}
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 加载Vibro设置数据
void LoadVibro(m_oSourceVibroStruct* pSourceVibroStruct,CXMLDOMElement* pElement)
{
	CString strKey = _T("");
	CString str = _T("");
	string strConv = "";
	try
	{
		strKey = "SourceStatus";
		pSourceVibroStruct->m_uiSourceStatus = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "SourceNb";
		pSourceVibroStruct->m_uiSourceNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "Label";
		str = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strConv = (CStringA)str;
		pSourceVibroStruct->m_usLabelSize = (unsigned short)(strConv.size() + 1);
		pSourceVibroStruct->m_pcLabel = new char[pSourceVibroStruct->m_usLabelSize];
		memcpy(pSourceVibroStruct->m_pcLabel, strConv.c_str(), pSourceVibroStruct->m_usLabelSize);
		strKey = "FleetNb";
		pSourceVibroStruct->m_uiFleetNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "Moving";
		pSourceVibroStruct->m_uiMoving = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "Step";
		pSourceVibroStruct->m_iStep = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
		strKey = "WorkByAcq";
		pSourceVibroStruct->m_uiWorkByAcq = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "ClusterNb";
		pSourceVibroStruct->m_uiCluster = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
		strKey = "Comments";
		str = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strConv = (CStringA)str;
		pSourceVibroStruct->m_usCommentsSize = (unsigned short)(strConv.size() + 1);
		pSourceVibroStruct->m_pcComments = new char[pSourceVibroStruct->m_usCommentsSize];
		memcpy(pSourceVibroStruct->m_pcComments, strConv.c_str(), pSourceVibroStruct->m_usCommentsSize);
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
// 加载Vibro设置队列数据
void LoadVibroList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int uiCountAll;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到Vibro设置区
		strKey = "SourceVibro";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到Vibro总数
		strKey = "Count";
		uiCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		m_oSourceVibroStruct oSourceVibroStruct;
		for(unsigned int i = 0; i < uiCountAll; i++)
		{
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			LoadVibro(&oSourceVibroStruct, &oElement);
			// 增加Vibro
			pCommInfo->m_oOptSetupData.m_olsVibroStruct.push_back(oSourceVibroStruct);
		}
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 加载Vibro设置数据
void LoadVibroSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 重置Vibro
	OnResetOptVibroList(pCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 加载Vibro设置队列数据
		LoadVibroList(pCommInfo);
	}
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存Vibro设置数据
void SaveVibro(m_oSourceVibroStruct* pSourceVibroStruct,CXMLDOMElement* pElement)
{
	CString strKey = _T("");
	COleVariant oVariant;
	CString str = _T("");
	string strConv = "";
	try
	{
		strKey = "SourceStatus";
		oVariant = (long)pSourceVibroStruct->m_uiSourceStatus;
		pElement->setAttribute(strKey, oVariant);
		strKey = "SourceNb";
		oVariant = (long)pSourceVibroStruct->m_uiSourceNb;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Label";
		strConv = pSourceVibroStruct->m_pcLabel;
		str = strConv.c_str();
		oVariant = str;
		pElement->setAttribute(strKey, oVariant);
		strKey = "FleetNb";
		oVariant = (long)pSourceVibroStruct->m_uiFleetNb;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Moving";
		oVariant = (long)pSourceVibroStruct->m_uiMoving;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Step";
		oVariant = (long)pSourceVibroStruct->m_iStep;
		pElement->setAttribute(strKey, oVariant);
		strKey = "WorkByAcq";
		oVariant = (long)pSourceVibroStruct->m_uiWorkByAcq;
		pElement->setAttribute(strKey, oVariant);
		strKey = "ClusterNb";
		oVariant = (long)pSourceVibroStruct->m_uiCluster;
		pElement->setAttribute(strKey, oVariant);
		strKey = "Comments";
		strConv = pSourceVibroStruct->m_pcComments;
		str = strConv.c_str();
		oVariant = str;
		pElement->setAttribute(strKey, oVariant);
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
// 保存Vibro设置队列数据
void SaveVibroList(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;
	unsigned int uiTabCount = 0;
	CString strTabChild = _T("");
	CString strTabParent = _T("");
	list<m_oSourceVibroStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	try
	{
		// 找到Vibro设置区
		strKey = "SourceVibro";
		lpDispatch = pCommInfo->m_oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 得到Tab键数量
		strKey = "TabCount";
		uiTabCount = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElementParent, strKey);
		strTabChild = _T("\r\n");
		strTabParent = _T("\r\n");
		for(unsigned int i = 0; i < uiTabCount; i++)
		{
			strTabChild += _T("\t");
			if (i < (uiTabCount - 1))
			{
				strTabParent += _T("\t");
			}
		}
		// 设置Explo总数
		strKey = "Count";
		oVariant = (long)pCommInfo->m_oOptSetupData.m_olsVibroStruct.size();
		oElementParent.setAttribute(strKey, oVariant);
		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		for (iter = pCommInfo->m_oOptSetupData.m_olsVibroStruct.begin();
			iter != pCommInfo->m_oOptSetupData.m_olsVibroStruct.end(); iter++)
		{
			lpDispatch = pCommInfo->m_oXMLDOMDocument.createTextNode(strTabChild);
			oElementParent.appendChild(lpDispatch);
			lpDispatch = pCommInfo->m_oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);
			SaveVibro(&(*iter), &oElementChild);
			oElementParent.appendChild(lpDispatch);		
		}
		lpDispatch = pCommInfo->m_oXMLDOMDocument.createTextNode(strTabParent);
		oElementParent.appendChild(lpDispatch);
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
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 保存Vibro设置数据
void SaveVibroSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	COleVariant oVariant;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	// 打开程序配置文件
	if (TRUE == OpenAppXMLFile(pCommInfo, pCommInfo->m_strOptXMLFilePath))
	{
		// 保存Vibro设置队列数据
		SaveVibroList(pCommInfo);
	}
	oVariant = (CString)(pCommInfo->m_strOptXMLFilePath.c_str());
	pCommInfo->m_oXMLDOMDocument.save(oVariant);
	// 关闭程序配置文件
	CloseAppXMLFile(pCommInfo);
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 设置Vibro设置数据
void SetVibroSetupData(char* pChar, unsigned int uiSize, m_oInstrumentCommInfoStruct* pCommInfo)
{
	m_oSourceVibroStruct oSourceVibroStruct;
	unsigned int uiPos = 0;
	OnResetOptVibroList(pCommInfo);
	while(uiPos < uiSize)
	{
		memcpy(&oSourceVibroStruct.m_uiSourceStatus, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_uiSourceNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_usLabelSize, &pChar[uiPos], 2);
		uiPos += 2;
		oSourceVibroStruct.m_pcLabel = new char[oSourceVibroStruct.m_usLabelSize];
		memcpy(&oSourceVibroStruct.m_pcLabel, &pChar[uiPos], oSourceVibroStruct.m_usLabelSize);
		uiPos += oSourceVibroStruct.m_usLabelSize;
		memcpy(&oSourceVibroStruct.m_uiFleetNb, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_uiMoving, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_iStep, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_uiWorkByAcq, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_uiCluster, &pChar[uiPos], 4);
		uiPos += 4;
		memcpy(&oSourceVibroStruct.m_usCommentsSize, &pChar[uiPos], 2);
		uiPos += 2;
		oSourceVibroStruct.m_pcComments = new char[oSourceVibroStruct.m_usCommentsSize];
		memcpy(&oSourceVibroStruct.m_pcComments, &pChar[uiPos], oSourceVibroStruct.m_usCommentsSize);
		uiPos += oSourceVibroStruct.m_usCommentsSize;
		EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
		pCommInfo->m_oOptSetupData.m_olsVibroStruct.push_back(oSourceVibroStruct);
		LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
	}
	SaveVibroSetupData(pCommInfo);
}
// 查询 Vibro XML文件信息
void QueryVibroSetupData(char* cProcBuf, int& iPos, m_oInstrumentCommInfoStruct* pCommInfo)
{
	list<m_oSourceVibroStruct>::iterator iter;
	EnterCriticalSection(&pCommInfo->m_oSecCommInfo);
	for (iter = pCommInfo->m_oOptSetupData.m_olsVibroStruct.begin();
		iter != pCommInfo->m_oOptSetupData.m_olsVibroStruct.end(); iter++)
	{
		memcpy(&cProcBuf[iPos], &iter->m_uiSourceStatus, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiSourceNb, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_usLabelSize, 2);
		iPos += 2;
		memcpy(&cProcBuf[iPos], &iter->m_pcLabel, iter->m_usLabelSize);
		iPos += iter->m_usLabelSize;
		memcpy(&cProcBuf[iPos], &iter->m_uiFleetNb, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiMoving, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_iStep, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiWorkByAcq, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_uiCluster, 4);
		iPos += 4;
		memcpy(&cProcBuf[iPos], &iter->m_usCommentsSize, 2);
		iPos += 2;
		memcpy(&cProcBuf[iPos], &iter->m_pcComments, iter->m_usCommentsSize);
		iPos += iter->m_usCommentsSize;
	}
	LeaveCriticalSection(&pCommInfo->m_oSecCommInfo);
}
// 加载施工客户端程序设置数据
void LoadOptAppSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	// 加载Delay设置数据
	LoadOptDelaySetupData(pCommInfo);
	// 加载SourceShot设置数据
	LoadSourceShotSetupData(pCommInfo);
	// 加载Explo设置数据
	LoadExploSetupData(pCommInfo);
	// 加载Vibro设置数据
	LoadVibroSetupData(pCommInfo);
}

// 保存施工客户端程序设置数据
void SaveOptAppSetupData(m_oInstrumentCommInfoStruct* pCommInfo)
{
	if (pCommInfo == NULL)
	{
		return;
	}
	// 保存Delay设置数据
	SaveDelaySetupData(pCommInfo);
	// 保存SourceShot设置数据
	SaveSourceShotSetupData(pCommInfo);
	// 保存Explo设置数据
	SaveExploSetupData(pCommInfo);
	// 保存Vibro设置数据
	SaveVibroSetupData(pCommInfo);
}