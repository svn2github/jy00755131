#include "StdAfx.h"
#include "Operation.h"
#include "OperationParseXML.h"
#include "../common/LineStruct.h"

/**
 * @brief COperationParseXML构造函数
 */
COperationParseXML::COperationParseXML(void)
{
}
/**
 * @brief COperationParseXML析构函数
 */
COperationParseXML::~COperationParseXML(void)
{

}

/**
 * @brief 读客户端的系统配置
 * @note  从配置文件中读取客户端的主机IP等信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 解析是否成功
 
bool COperationParseXML::ParseClientSetup(LPCTSTR pstrXMLFilePath)
{
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	try
	{	// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到放炮表区域
		strKey =_T("OperationClient");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);

		// 得到客户端IP地址，即本机IP地址
		strKey =_T("HostIP");
		strTemp = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);		
		// theApp.m_dwHostIP = StringIPToDWORDIP(strTemp);
		theApp.m_strHostIP = strTemp;
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}
}*/
/**
 * @brief 解析采集站配置信息
 * @note  从XML配置文件中解析出采集站的基本配置信息，主要
 包括：前置放大、模拟门选择、5373模式、DA衰减、采样率、IIR选择、FIR选择
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CFDUConfig* pFDUConfig，配置对象
 * @return 读取的对象个数

bool COperationParseXML::ParseFDUConfig(LPCTSTR pstrXMLFilePath,CFDUConfig* pFDUConfig)
{
	CString strTemp;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;	

	if(!pFDUConfig)
		return false;
	try
	{	// 创建XML对象
		strTemp =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strTemp, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到入口
		strKey =_T("samplerate");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwSampleRateIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("FilterFIR");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwFilterFIRIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("FilterIIR");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwFilterIIRIndex = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Gain3301");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwGain3301Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Mux3301");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwMux3301Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Mode5373");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwMode5373Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 找到入口
		strKey =_T("Attr5373");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到索引值
		strKey =_T("Index");
		pFDUConfig->m_dwAttr5373Index = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);

		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}
	catch (CException* )
	{
		return false;
	}

} */
/**
 * @brief解析单条施工注释记录
 * @note  解析单条施工注释记录。
 * @param CXMLDOMElement* pElement，XML节点对象指针
 * @param CShotComment* pComment，注释对象
 * @return 成功返回1，失败返回0
 */
int COperationParseXML::ParseCommentRecord(CXMLDOMElement* pElement,CShotComment* pComment)
{
	CString strKey;
	try
	{
		strKey =_T("Nb");
		pComment->m_dwCommentNb= CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);

		strKey =_T("Label");
		pComment->m_strLabel= CXMLDOMTool::GetElementAttributeString(pElement, strKey);

		strKey =_T("Comment");
		pComment->m_strComment= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	}	
	catch (CException* )
	{
		return 0;
	}

	return 1;	
}
/**
 * @brief 保存单条施工注释记录
 * @note  保存单条施工注释记录保存到XML的配置文件中。
 * @param CXMLDOMElement* pElement，XML节点对象指针
 * @param CShotComment* pComment，注释对象
 * @return 成功返回1
 */
int COperationParseXML::SaveCommentRecord(CXMLDOMElement* pElement,CShotComment* pComment)
{
	CString strKey;	// 键名	
	COleVariant   oVariant;
	VARIANT   vt;
	if(NULL==pComment)
		return 0;
	try
	{
		// Nb
		strKey = _T("Nb");
		vt.vt = VT_UI4;
		vt.ulVal = pComment->m_dwCommentNb;
		oVariant = vt;
		pElement->setAttribute(strKey, oVariant);

		// Label
		strKey =_T("Label");
		oVariant = pComment->m_strLabel;
		pElement->setAttribute(strKey, oVariant);

		// Comments
		strKey =_T("Comment");
		oVariant = pComment->m_strComment;
		pElement->setAttribute(strKey, oVariant);
	}	
	catch (CException* )
	{
		return 0;
	}
	return 1;
}
/**
 * @brief 解析施工注释记录表
 * @note  在XML的配置文件中解析施工注释记录。
 * @param LPCTSTR pstrXMLFilePath，XML文件
 * @param CShotComments* pComments，注释对象
 * @return 成功返回记录条数
 */
int  COperationParseXML::ParseComments(LPCTSTR pstrXMLFilePath,CShotComments* pComments)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;
	try
	{
		//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到施工注释区域
		strKey =_T("OperationComment");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CShotComment* pComment = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pComment = new CShotComment;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 得到炮号
			ParseCommentRecord(&oElement, pComment);		
			// 加入放炮表
			pComments->Add(pComment);
		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();	
	}	
	catch (CException* )
	{
		return 0;
	}
	return dwCount;
}
/**
 * @brief 保存施工注释表
 * @note  将注释表对象pComments的数据保存到XML的配置文件中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotComments* pComments，注释表对象，也是施工注释的集合对象
 * @return 存储的对象个数
 */
int  COperationParseXML::SaveComments(LPCTSTR pstrXMLFilePath,CShotComments* pComments)
{
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString			strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElementParent,oElementChild;
	LPDISPATCH		lpDispatch;
	long			i,lCount;
	try
	{	//CoInitialize(NULL);
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到放炮表区域
		strKey =_T("OperationComment");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 设置点代码总数
		strKey = _T("Count");
		lCount = (long)(pComments->GetCount());
		oVariant = lCount;
		oElementParent.setAttribute(strKey, oVariant);

		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		// 增加新节点
		CShotComment *pComment =NULL;
		for( i = 0; i < lCount; i++)
		{
			pComment = pComments->GetShotComment(i);

			lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t"));
			oElementParent.appendChild(lpDispatch);

			lpDispatch = oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);		
			//保存单条放炮记录
			SaveCommentRecord(&oElementChild,pComment);	
			oElementParent.appendChild(lpDispatch);

		}
		lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t"));
		oElementParent.appendChild(lpDispatch);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
		theApp.SaveXMLToFTPServer();
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		theApp.SaveXMLToFTPServer();
	}	
	catch (CException* )
	{
		return 0;
	}
	return lCount;
}
/**
 * @brief 解析放炮表
 * @note  从XML的配置文件中读取出整个放炮表，放入放炮表对象pShotPoints中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotPoints* pShotPoints，放炮表对象，也是存储炮号的集合对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseOperationTable(LPCTSTR pstrXMLFilePath,CShotPoints* pShotPoints)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strOLEObject =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到放炮表区域
	strKey =_T("OperationTable");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CShotPoint* pVP = NULL;
	for(unsigned int i = 0; i < dwCount; i++)
	{
		pVP = new CShotPoint;
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		// 得到炮号
		ParseOperationTableRecord(&oElement, pVP);		
		// 加入放炮表
		pShotPoints->Add(pVP);		
	}
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	//CoUninitialize();
	return dwCount;
}
/**
 * @brief 保存放炮表
 * @note  将放炮表对象pShotPoints的数据保存到XML的配置文件中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotPoints* pShotPoints，放炮表对象，也是存储炮号的集合对象
 * @return 存储的对象个数
 */
int COperationParseXML::SaveOperationTable(LPCTSTR pstrXMLFilePath,CShotPoints* pShotPoints)
{	
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString			strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElementParent,oElementChild;
	LPDISPATCH		lpDispatch;
	long			i,lCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strKey =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到放炮表区域
	strKey =_T("OperationTable");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	
	// 设置点代码总数
	strKey = _T("Count");
	lCount = (long)(pShotPoints->GetCount());
	oVariant = lCount;
	oElementParent.setAttribute(strKey, oVariant);

	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}
	// 增加新节点
	CShotPoint *pShotPoint =NULL;
	for( i = 0; i < lCount; i++)
	{
		pShotPoint = pShotPoints->GetShotPoint(i);

		lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t"));
		oElementParent.appendChild(lpDispatch);

		lpDispatch = oXMLDOMDocument.createElement(_T("Record"));
		oElementChild.AttachDispatch(lpDispatch);		
		//保存单条放炮记录
		SaveOperationTableRecord(&oElementChild,pShotPoint);	
		oElementParent.appendChild(lpDispatch);
				
	}
	lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t"));
	oElementParent.appendChild(lpDispatch);

	oVariant = pstrXMLFilePath;
	oXMLDOMDocument.save(oVariant);
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	theApp.SaveXMLToFTPServer();
	return lCount;
}

/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能：

			每条记录的XML格式如下：
			<Record VPStatus="0" ShotID="1" BreakPoint="N" SwathNb ="1" SourcePointIndex ="1"
			SourceLine = "1" SourceReceiver = "1" SpreadType ="Absolute" SpreadSFL ="" SpreadSFN ="" 
			SuperSpread="" ProcessTypeID ="1" Comments ="注释"/>

    修改历史：



*******************************************************************************/
int COperationParseXML::ParseOperationTableRecord(CXMLDOMElement* pElement,CShotPoint* pVP)
{
	CString strKey;	// 键名	
	if(NULL==pVP)
		return 0;
	// 放炮状态
	strKey = _T("VPStatus");
	pVP->m_VPState =(unsigned char)CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 炮号
	strKey =_T("ShotID");
	pVP->m_dwShotNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	//  
	strKey =_T("BreakPoint");
	strKey= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	if(strKey.CompareNoCase(_T("Y")))
		pVP->m_byBreakPoint = 0;
	else
		pVP->m_byBreakPoint = 1;
	// 
	strKey =_T("SwathNb");
	pVP->m_dwSwathNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("SourcePointIndex");
	pVP->m_dwSourcePointIndex= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("SourceLine");
	pVP->m_fSourceLine = CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	// 
	strKey =_T("SourceReceiver");
	pVP->m_fSourceNb= CXMLDOMTool::GetElementAttributeFloat(pElement, strKey);
	// 
	strKey =_T("SpreadSFL");
	pVP->m_dwSpreadSFL = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("SpreadSFN");
	pVP->m_dwSpreadSFN= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	// 
	strKey =_T("SpreadType");
	pVP->m_dwSpreadNb= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	/*
	strKey =_T("SpreadType");
	strKey= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	if(strKey.CompareNoCase(_T("Absolute")))
		pVP->m_bySpreadType= SPREADTYPE_GENERIC;
	else
		pVP->m_byBreakPoint = SPREADTYPE_ABSOLUTE;
	*/
	// 
	strKey =_T("ProcessTypeID");
	pVP->m_dwProcessNb= CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 
	strKey =_T("Comment");
	pVP->m_szComments= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	return 1;
}
/**
 * @brief 保存单条放炮记录
 * @note  将放炮对象pShotPoint的数据保存到XML的配置文件中。
 * @param CXMLDOMElement* pElement，XML节点对象指针
 * @param CShotPoint* pShotPoint，放炮对象
 * @return 成功返回1
 */
int COperationParseXML::SaveOperationTableRecord(CXMLDOMElement* pElement,CShotPoint* pShotPoint)
{
	CString strKey;	// 键名	
	COleVariant   oVariant;
	VARIANT   vt;
	if(NULL==pShotPoint)
		return 0;
	// 放炮状态	
	strKey = _T("VPStatus");
	oVariant = pShotPoint->m_VPState;
	pElement->setAttribute(strKey, oVariant);
	// 炮号
	strKey = _T("ShotID");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwShotNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);	
	 
	// BreakPoint
	strKey = _T("BreakPoint");
	if(pShotPoint->m_byBreakPoint)
		oVariant = _T("Y"); 
	else
		oVariant = _T("N");
	pElement->setAttribute(strKey, oVariant);
	// SwathNb
	strKey =_T("SwathNb");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwSwathNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);	
	// SourcePointIndex
	strKey =_T("SourcePointIndex");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwSourcePointIndex;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// SourceLine
	strKey =_T("SourceLine");
	oVariant = pShotPoint->m_fSourceLine;
	pElement->setAttribute(strKey, oVariant);	
	// SourceReceiver
	strKey =_T("SourceReceiver");
	oVariant = pShotPoint->m_fSourceNb;
	pElement->setAttribute(strKey, oVariant);	
	// SpreadSFL
	strKey =_T("SpreadSFL");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwSpreadSFL;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);	
	// SpreadSFN
	strKey =_T("SpreadSFN");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwSpreadSFN;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);	
	// SpreadType
	strKey =_T("SpreadType");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwSpreadNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// ProcessTypeID
	strKey =_T("ProcessTypeID");
	vt.vt = VT_UI4;
	vt.ulVal = pShotPoint->m_dwProcessNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// Comments
	strKey =_T("Comment");
	oVariant = pShotPoint->m_szComments;
	pElement->setAttribute(strKey, oVariant);
	return 1;
}
/**
 * @brief 解析震源表
 * @note  从XML的配置文件中读取出震源表，放入对象pShotSrcs中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSources* pShotSrcs，震源表对象，也是存储震源的集合对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseSourceType(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{

	return ParseSourceExplo(pstrXMLFilePath,pShotSrcs)+ParseSourceVibro(pstrXMLFilePath,pShotSrcs);
}
int COperationParseXML::SaveSourceType(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	
	int nRet = SaveSourceExplo(pstrXMLFilePath,pShotSrcs)+SaveSourceVibro(pstrXMLFilePath,pShotSrcs);
	theApp.SaveXMLToFTPServer();
	return nRet;
}
/******************************************************************************
    函    数：ParseSourceExplo
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析脉冲爆炸震源

    修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceExplo(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;
	try
	{
		//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到脉冲震源
		strKey =_T("SourceExplo");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CShotSourceExplo* pSrc = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pSrc = new CShotSourceExplo;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 解析震源
			ParseSourceExploRecord(&oElement, pSrc);		
			// 加入震源表
			pShotSrcs->Add(pSrc);

		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
	}	
	catch (CException* )
	{
		return 0;
	}
	return dwCount;

}
int COperationParseXML::SaveSourceExplo(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString			strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElementParent,oElementChild;
	LPDISPATCH		lpDispatch;
	long			i,lCount;

	//CoInitialize(NULL);
	// 创建XML对象
	strKey =_T("msxml2.domdocument");
	BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);

	oVariant = pstrXMLFilePath;
	bData = oXMLDOMDocument.load(oVariant);

	// 找到脉冲震源表区域
	strKey =_T("SourceExplo");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);

	// 设置点代码总数
	strKey = _T("Count");
	lCount = (long)(pShotSrcs->GetExploCount());
	oVariant = lCount;
	oElementParent.setAttribute(strKey, oVariant);

	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}
	// 增加新节点
	lCount = pShotSrcs->GetCount();
	CShotSource *pShotSource =NULL;	
	for( i = 0; i < lCount; i++)
	{
		pShotSource = pShotSrcs->GetShotSource(i);
		if(pShotSource==NULL || pShotSource->m_dwSourceType!=SHOTTYPE_EXPLO)
			continue;
		lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t\t"));
		oElementParent.appendChild(lpDispatch);

		lpDispatch = oXMLDOMDocument.createElement(_T("Record"));
		oElementChild.AttachDispatch(lpDispatch);		
		//保存单条放炮记录
		SaveSourceExploRecord(&oElementChild,(CShotSourceExplo*)pShotSource);
		oElementParent.appendChild(lpDispatch);

	}
	lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t"));
	oElementParent.appendChild(lpDispatch);

	oVariant = pstrXMLFilePath;
	oXMLDOMDocument.save(oVariant);
	// 释放对象
	oXMLDOMDocument.DetachDispatch();
	return lCount;
}

/******************************************************************************
    函    数：ParseSourceExploRecord
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析单条震源记录

				<Record SourceStatus="0" SourceNb ="1" Label="label1" 
				ShooterNb="123456" IncrNb="1" Comment="SourceExplo震源描述" />
    修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceExploRecord(CXMLDOMElement* pElement,CShotSourceExplo* pExploSrc)
{
	CString strKey;	// 键名	
	if(NULL==pExploSrc)
		return 0;
	// 状态
	strKey = _T("SourceStatus");
	pExploSrc->m_bySourceState =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 编号
	strKey = _T("SourceNb");
	pExploSrc->m_dwSourceNb =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pExploSrc->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 所连采集站SN
	strKey = _T("ShooterNb");
	pExploSrc->m_dwShooterNb =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 递增步幅，可以为负
	strKey = _T("IncrNb");
	pExploSrc->m_lStep =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// Comment
	strKey = _T("Comment");
	pExploSrc->m_strComment =CXMLDOMTool::GetElementAttributeString(pElement, strKey);	
	return 1;
}
int COperationParseXML::SaveSourceExploRecord(CXMLDOMElement* pElement,CShotSourceExplo* pExploSrc)
{
	CString		strKey;	// 键名	
	COleVariant oVariant;
	VARIANT		vt;
	if(NULL==pExploSrc)
		return 0;
	// 状态	
	strKey = _T("SourceStatus");
	oVariant = pExploSrc->m_bySourceState;
	pElement->setAttribute(strKey, oVariant);
	// SourceNb
	strKey = _T("SourceNb");
	vt.vt = VT_UI4;
	vt.ulVal = pExploSrc->m_dwSourceNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);		
	// Label
	strKey =_T("Label");	
	oVariant = pExploSrc->m_strLabel;
	pElement->setAttribute(strKey, oVariant);	
	// ShooterNb
	strKey =_T("ShooterNb");
	vt.vt = VT_UI4;
	vt.ulVal = pExploSrc->m_dwShooterNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// IncrNb
	strKey =_T("IncrNb");
	oVariant = pExploSrc->m_lStep;
	pElement->setAttribute(strKey, oVariant);	
	// Comment
	strKey =_T("Comment");
	oVariant = pExploSrc->m_strComment;
	pElement->setAttribute(strKey, oVariant);	
	return 1;
}
/******************************************************************************
    函    数：ParseSourceVibro
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析可控震源

    修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceVibro(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;
	try
	{	//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到脉冲震源
		strKey =_T("SourceVibro");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CShotSourceVibro* pSrc = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pSrc = new CShotSourceVibro;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 解析震源
			ParseSourceVibroRecord(&oElement, pSrc);		
			// 加入震源表
			pShotSrcs->Add(pSrc);

		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
	}	
	catch (CException* )
	{
		return 0;
	}
	return dwCount;
}
int COperationParseXML::SaveSourceVibro(LPCTSTR pstrXMLFilePath,CShotSources* pShotSrcs)
{
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString			strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElementParent,oElementChild;
	LPDISPATCH		lpDispatch;
	long			i,lCount;
	try
	{
		//CoInitialize(NULL);
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到脉冲震源表区域
		strKey =_T("SourceVibro");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 设置点代码总数
		strKey = _T("Count");
		lCount = (long)(pShotSrcs->GetVibroCount());
		oVariant = lCount;
		oElementParent.setAttribute(strKey, oVariant);

		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		// 增加新节点
		lCount = pShotSrcs->GetCount();
		CShotSource *pShotSource =NULL;	
		for( i = 0; i < lCount; i++)
		{
			pShotSource = pShotSrcs->GetShotSource(i);
			if(pShotSource==NULL || pShotSource->m_dwSourceType!=SHOTTYPE_VIBRO)
				continue;
			lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t\t"));
			oElementParent.appendChild(lpDispatch);

			lpDispatch = oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);		
			//保存单条放炮记录
			SaveSourceVibroRecord(&oElementChild,(CShotSourceVibro*)pShotSource);
			oElementParent.appendChild(lpDispatch);

		}
		lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t"));
		oElementParent.appendChild(lpDispatch);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
	}	
	catch (CException* )
	{
		return 0;
	}
	return lCount;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析单条可控震源的记录

	<Record SourceStatus="0" SourceNb ="1" Label="label1" ShooterNb="12345" 
	Moving="1" Step="1" WorkByAcq="0" ClusterNb="1" Comment="SourceVibro震源描述" />

	修改历史：

*******************************************************************************/
int COperationParseXML::ParseSourceVibroRecord(CXMLDOMElement* pElement,CShotSourceVibro* pVibroSrc)
{
	CString strKey;	// 键名	
	if(NULL==pVibroSrc)
		return 0;
	// 状态
	strKey = _T("SourceStatus");
	pVibroSrc->m_bySourceState =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 编号
	strKey = _T("SourceNb");
	pVibroSrc->m_dwSourceNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pVibroSrc->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 所连采集站SN
	strKey = _T("ShooterNb");
	pVibroSrc->m_dwShooterNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 递增步幅，可以为负
	strKey = _T("Step");
	pVibroSrc->m_lStep =CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// Comment
	strKey = _T("Comment");
	pVibroSrc->m_strComment =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 递增步幅，可以为负
	strKey = _T("Moving");
	pVibroSrc->m_byMoving =(unsigned char)CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	// 叠加的多次采集
	strKey = _T("WorkByAcq");
	if(CXMLDOMTool::GetElementAttributeInt(pElement, strKey))
	    pVibroSrc->m_bWorkByAcq = TRUE;
	else
		pVibroSrc->m_bWorkByAcq = FALSE;
	// 同时双震源作业
	strKey = _T("ClusterNb");
	pVibroSrc->m_dwClusterNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	
	return 1;
}
/**
 * @brief 保存可控震源单条记录
 * @note  将一条可控震源记录写入XML
 * @param CXMLDOMElement* pElement，XML元素对象指针
 * @param CShotSourceVibro* pVibroSrc，可控震源
 * @return 成功返回1，失败返回0
 */
int COperationParseXML::SaveSourceVibroRecord(CXMLDOMElement* pElement,CShotSourceVibro* pVibroSrc)
{
	CString		strKey;	// 键名	
	COleVariant oVariant;
	VARIANT		vt;
	if(!pVibroSrc || !pElement)
		return 0;
	// 状态	
	strKey = _T("SourceStatus");
	oVariant = pVibroSrc->m_bySourceState;
	pElement->setAttribute(strKey, oVariant);
	// SourceNb
	strKey = _T("SourceNb");
	vt.vt = VT_UI4;
	vt.ulVal = pVibroSrc->m_dwSourceNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);		
	// Label
	strKey =_T("Label");	
	oVariant = pVibroSrc->m_strLabel;
	pElement->setAttribute(strKey, oVariant);	
	// ShooterNb
	strKey =_T("ShooterNb");
	vt.vt = VT_UI4;
	vt.ulVal = pVibroSrc->m_dwShooterNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// Moving	
	strKey = _T("Moving");
	oVariant = pVibroSrc->m_byMoving;
	pElement->setAttribute(strKey, oVariant);
	// Step
	strKey =_T("Step");
	oVariant = pVibroSrc->m_lStep;
	pElement->setAttribute(strKey, oVariant);	
	// WorkByAcq
	strKey =_T("WorkByAcq");
	if(pVibroSrc->m_bWorkByAcq)
		oVariant = 1L;
	else
		oVariant = 0L;
	pElement->setAttribute(strKey, oVariant);
	// ClusterNb
	strKey =_T("ClusterNb");
	vt.vt = VT_UI4;
	vt.ulVal = pVibroSrc->m_dwClusterNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// Comment
	strKey =_T("Comment");
	oVariant = pVibroSrc->m_strComment;
	pElement->setAttribute(strKey, oVariant);	
	return 1;
}
/******************************************************************************
    函    数：
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 读取采集排列 ????????????????????

    修改历史： 20100729: 当前是测试代码，读取10*150的采集排列，后续要修改

*******************************************************************************/
/**
 * @brief 解析采集排列表
 * @note  从XML的配置文件中读取出采集排列表，放入对象CShotSpreads中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @return 读取的对象个数

int COperationParseXML::ParseShotSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pSpreads)
{
	CShotSpread *pSpread = new CShotSpread;

	pSpread->m_dwNb = 1;
	int i,j;
	SHOTSPREAD    ShotSpread;
	for (i=1;i<=10;i++) // 大线数量
	{
		for (j=11;j<=20;j++) // 点号
		{
			ShotSpread.m_dwIP = CIPList::GetCollectIP(i,j,1);
			ShotSpread.m_dwGain = 1;
			pSpread->m_arrSpread.Add(ShotSpread);
		}
	}
	pSpreads->m_AllSpread.Add(pSpread);
	return 1;
} */
/**
 * @brief 解析辅助道记录
 * @note  从配置文件中读取解析辅助道记录信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @return 成功返回1，失败返回0
 */
int COperationParseXML::ParseAuxChannelRecord(CXMLDOMElement* pElement,CAuxChannel* pAuxChannel)
{
	CString strKey;
	if(!pAuxChannel)
		return 0;
	strKey = _T("Nb");
	pAuxChannel->m_dwNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("Label");
	pAuxChannel->m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = _T("BoxType");
	pAuxChannel->m_dwBoxType = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("SN");
	pAuxChannel->m_dwSN = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	strKey = _T("ChannelNb");
	pAuxChannel->m_dwChannelNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("Gain");
	pAuxChannel->m_dwGain = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("DPGNb");
	pAuxChannel->m_dwDPGNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = _T("Comments");
	pAuxChannel->m_strComments = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// IP地址
	pAuxChannel->m_dwIP = CIPList::GetAuxIP(pAuxChannel->m_dwNb);		
	return 1;

}
/**
 * @brief 读客户端的系统配置
 * @note  从配置文件中读取解析辅助道信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
  * @param CAuxChannels* pAllAuxes，辅助道数组对象
 * @return 辅助道总数
 */
int COperationParseXML::ParseAuxChannel(LPCTSTR pstrXMLFilePath,CAuxChannels* pAllAuxes)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;
	try
	{
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到辅助道区域
		strKey =_T("AuxSetup");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CAuxChannel* pAuxChannel = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pAuxChannel = new CAuxChannel;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 得到炮号
			ParseAuxChannelRecord(&oElement, pAuxChannel);		
			// 加入放炮表
			pAllAuxes->Add(pAuxChannel);
		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 解析采集排列表
 * @note  从XML的配置文件中读取出采集排列表，放入对象CShotSpreads中。由于采集排列的定义中
 仅仅说明了线号、点号，没有定义道号；因此需要从2万道的记录中依据线号、点号来找出可能的所有采集道，
 将采集道添加到采集排列中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @param CChannelList* pChannelList，所有的采集道列表
 * @return 读取的对象个数，失败返回-1；
 */
int COperationParseXML::ParseAllAbsoluteSpread(LPCTSTR pstrXMLFilePath,CShotSpreads* pSpreads,CChannelList* pChannelList)
{
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		dwCount;
	try
	{
		//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到脉冲震源
		strKey =_T("AbsoluteSetup");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CShotSpread *pSpread = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pSpread= new CShotSpread;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 解析采集排列
			ParseShotSpreadRecord(&oElement, pSpread,pChannelList);		
			// 加入震源表
			pSpreads->Add(pSpread);

		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
	}	
	catch (CException* )
	{
		return -1;
	}
	return dwCount;
}


int COperationParseXML::ParseShotSpreadRecord(CXMLDOMElement* pElement, CShotSpread *pSpread,CChannelList* pChannelList)
{
	CString			strKey;	// 键名	
	CString         strTemp;
	CString			strSpread;
	LPTSTR			lpStr;
	int				nPos;
	DWORD           dwLineNb,dwPointMin,dwPointMax;
	DWORD			dwGain;
	
	if(NULL==pSpread|| NULL==pChannelList)
		return 0;
	// 编号
	strKey = _T("Nb");
	pSpread->m_dwNb =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pSpread->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 采集排列
	strKey = _T("Spread");
	strSpread =CXMLDOMTool::GetElementAttributeString(pElement, strKey);

	// 解析采集排列，示例为： strSpread="10:101-103g1,106-115g2;20:101-103g1,106-115g2;30:101-103g1,106-115g2"
	while(strSpread!=_T(""))
	{
		nPos = strSpread.Find(_T(";"));
		if(nPos>0)
		{
			strKey = strSpread.Left(nPos);
			strSpread = strSpread.Right(strSpread.GetLength()-nPos-1);
		}
		else
		{
			strKey = strSpread;
			strSpread = _T("");
		}
		
		// 下面解析10:101-103g1,106-115g2
		// 解析出行号		
		dwLineNb = _tcstoul(LPCTSTR(strKey),&lpStr,10);
		// 此时lpStr = :101-103g1,106-115g2
		strKey = lpStr+1;	
		do // 解析完某一条测线的所有点号定义
		{		
			// 解析出点号的最小值
			dwPointMin= _tcstoul(LPCTSTR(strKey),&lpStr,10);
			// lpStr = -103g1,106-115g2
			strKey = lpStr+1;
			// 解析出点号的最大值
			dwPointMax= _tcstoul(LPCTSTR(strKey),&lpStr,10);
			// lpStr = g1,106-115g2
			strTemp = lpStr;	
			// 解析出增益
			strKey = strTemp.Left(2);
			if(!strKey.CompareNoCase(_T("g1")))
			{
				dwGain = 1;
			}
			else
			{
				dwGain = 4;
			}
			// 对该条测线范围dwPointMin - dwPointMax之间测点读入内存
			AppendChannelsToShotSpread(dwLineNb, dwPointMin, dwPointMax,dwGain, pSpread, pChannelList);
			// 判断是否结束
			nPos= strTemp.Find(_T(","));
			// 找不到“，”，则表示该大线已经解析完成，退出解析下一条大线
			if (nPos<=0)
			{	
				break;
			}
			// 获得后面的点号定义，例如 106-115g2
			strKey = strTemp.Right(strTemp.GetLength()-nPos-1);

		} while (true);	
		
	}	
	return 1;	
}
/**
 * @brief 计算采集道属性并加入采集排列对象中
 * @note  计算某一范围的采集道属性，并将采集道SHOTSPREAD加入采集对象CShotSpread中
 * @param DWORD dwLine, 线号
 * @param DWORD dwPointMin,最小点号
 * @param DWORD dwPointMax,最大点号
 * @param DWORD dwGain,增益
 * @param CShotSpread *pSpread,采集排列对象
 * @param CChannelList* pChannelList 
 * @return 解析成功返回true，失败返回false
 */
bool COperationParseXML::AppendChannelsToShotSpread(DWORD dwLine,DWORD dwPointMin,DWORD dwPointMax,DWORD dwGain,
													CShotSpread *pSpread,CChannelList* pChannelList)
{
	DWORD			dwIndex;
	DWORD			dwIP;
	// DWORD           dwTemp;
	SHOTSPREAD		ShotSpread;
	CChannelSetupData* pChnData=NULL;
	if(!pSpread || !pChannelList)
		return false;
	// 防止定义的点号最大值小于最小值
	if(dwPointMin>dwPointMax)
	{	// 将最大值和最小值交换
		dwIndex=dwPointMin;
		dwPointMin = dwPointMax;
		dwPointMax = dwIndex;
	}
	for (dwIndex=dwPointMin;dwIndex<=dwPointMax;dwIndex++)
	{
		// 线号
		ShotSpread.m_dwLineNb = dwLine;
		// 点号
		ShotSpread.m_dwPointNb = dwIndex;
		// 增益
		ShotSpread.m_dwGain = dwGain;
		// 一个采集站下可能有多个道，先找第一道的位置
		dwIP = CChannelSetupData::CreateIP(dwLine,dwIndex,1);
		// 从测网设计的对象中查找相应的采集道
		if(!pChannelList->GetChannelFromMap(dwIP,pChnData))
		{
			continue;
		}
		// 保存第一道的对象，记录编号
		ShotSpread.m_dwChannelNb = 1;
		// IP地址
		ShotSpread.m_dwIP = dwIP;
		// 判断采集道类型
		if(pChnData->m_bMute)
			ShotSpread.m_byType = CHN_MUTE;
		else
			ShotSpread.m_byType = CHN_NORMAL;
		// 如果不是跳过道
		if(!pChnData->m_bJumped)
			pSpread->Append(ShotSpread);
		
		// 寻找是否存在其他采集通道
		while(pChnData!=NULL)
		{
			// 依次向采集道数组后面查找是否相同的测线、测点，而属于不同的道号对象
			pChnData = pChannelList->GetChannelByIndex(pChnData->m_uiIndex+1);
			if (pChnData==NULL)
			{
				break;
			}
			// 找到新的采集道对象，判断测线和点号是否一致
			if(pChnData->m_uiNbLine!=dwLine || pChnData->m_uiNbPoint != dwIndex)
				break;
			// 跳过道
			if(pChnData->m_bJumped)
				continue;
			// 采集道号
			ShotSpread.m_dwChannelNb =pChnData->m_uiNbChannel;
			// IP地址
			ShotSpread.m_dwIP = pChnData->m_uiIP;
			// 判断采集道类型
			if(pChnData->m_bMute)
				ShotSpread.m_byType = CHN_MUTE;
			else
				ShotSpread.m_byType = CHN_NORMAL;
			pSpread->Append(ShotSpread);
		}
	}
	return true;
}
/**
 * @brief 解析数据类型表
 * @note  从XML的配置文件中读取出采集排列表，放入对象CShotSpreads中。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseProcessType(LPCTSTR pstrXMLFilePath,CProcessTypes* pProcesses)
{
	CString			strOLEObject;
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;	
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElement;
	LPDISPATCH		lpDispatch;
	ULONG			dwCount;
	CString			strKey;
	BYTE            byType;	
	try
	{
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到脉冲震源
		strKey =_T("ProcessType");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CProcessType* pProcess = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{		
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			strKey =_T("Type");	// 得到处理对象的类型，根据类型创建不同对象
			byType = (BYTE)CXMLDOMTool::GetElementAttributeInt(&oElement,strKey);
			switch(byType )
			{
			case PROCESS_IMPULSIVE:
				pProcess = new CProcessImpulsive;
				ParseProcessImpulsive(&oElement,(CProcessImpulsive*)pProcess);
				pProcesses->Add(pProcess);// 加入震源表
				break;
			default:
				break;
			}		
		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();		
	}	
	catch (CException* )
	{
		return 0;
	}
	return dwCount;

}

/**
 * @brief 解析辅助道
 * @note  从XML的配置文件中读取出某一个处理类型的辅助道，放入对象CProcessType中。
		解析辅助道
		<Auxiliaries Count="1"  CorreWith = "Pilot" >	
			<Record AuxiNb ="1" AuxiProcessing="Aux1*Aux2"  />
		</Auxiliaries>    

 * @param CXMLDOMElement* pElement，指向父节点ProcessRecord元素的指针
 * @param CProcessType* pProcess，处理类型对象
 * @return 读取的对象个数
 */
int COperationParseXML::ParseProcessAuxi(CXMLDOMElement* pElement,CProcessType* pProcess)
{
	CXMLDOMNodeList oNodeList;	
	CXMLDOMElement	oElement;
	LPDISPATCH		lpDispatch;
	ULONG			dwCount;
	CString			strKey;
	if(NULL==pProcess || NULL==pElement)
		return 0;
	try
	{
		lpDispatch = pElement->get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);
		// CProcessType* pProcess = NULL;
		AUXICHANNEL  AuxiCHN;
		for(unsigned int i = 0; i < dwCount; i++)
		{		
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			AuxiCHN.m_dwNb = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, _T("AuxiNb"));
			strKey = CXMLDOMTool::GetElementAttributeString(&oElement, _T("AuxiProcessing"));
			_tcscpy_s(AuxiCHN.m_szProcessing,_countof(AuxiCHN.m_szProcessing),strKey);
			pProcess->m_arrAuxiChannel.Add(AuxiCHN);
		}
		return dwCount;
	}
	catch (CException* )
	{
		return 0;
	}
}

/**
 * @brief 保存辅助道
 * @note 某一个处理类型的辅助道保存到XML的配置文件中
		解析辅助道
		<Auxiliaries Count="1"  CorreWith = "Pilot" >	
			<Record AuxiNb ="1" AuxiProcessing="Aux1*Aux2"  />
		</Auxiliaries>    

 * @param CXMLDOMElement* pElement，指向父节点ProcessRecord元素的指针
 * @param CProcessType* pProcess，处理类型对象
 * @return 读取的对象个数
 */
int COperationParseXML::SaveProcessAuxi(CXMLDOMDocument* pXMLDOMDocument,CXMLDOMElement* pElement,CProcessType* pProcess)
{
	CXMLDOMNodeList oNodeList;	
	CXMLDOMElement	oElement;		// Auxiliaries 节点
	CXMLDOMElement	oElementChild;	// 指向Auxi 的Record节点
	LPDISPATCH		lpDispatch;
	long			lCount;
	CString			strKey;
	COleVariant     oVariant;
	VARIANT			vt;
	if(!pXMLDOMDocument || !pElement || !pProcess)
		return 0;

	lpDispatch = pXMLDOMDocument->createTextNode(_T("\r\n\t\t\t\t\t"));
	pElement->appendChild(lpDispatch);

	// 添加 Auxiliaries 节点
	lpDispatch = pXMLDOMDocument->createElement(_T("Auxiliaries"));
	oElement.AttachDispatch(lpDispatch);
	pElement->appendChild(lpDispatch);

	// 设置点代码总数
	strKey = _T("Count");
	lCount = (long)(pProcess->m_arrAuxiChannel.GetCount());
	oVariant = lCount;
	oElement.setAttribute(strKey, oVariant);
	
	// 增加新节点
	// lCount = pShotSrcs->GetCount();
	// CShotSource *pShotSource =NULL;	
	for(int i = 0; i < lCount; i++)
	{		
		lpDispatch = pXMLDOMDocument->createTextNode(_T("\r\n\t\t\t\t\t\t"));
		oElement.appendChild(lpDispatch);

		lpDispatch = pXMLDOMDocument->createElement(_T("Record"));
		oElementChild.AttachDispatch(lpDispatch);		
		//保存单条放炮记录
		//SaveSourceVibroRecord(&oElementChild,(CShotSourceVibro*)pShotSource);
		// SourceNb
		strKey = _T("AuxiNb");
		vt.vt = VT_UI4;
		vt.ulVal = pProcess->m_arrAuxiChannel[i].m_dwNb;
		oVariant = vt;
		oElementChild.setAttribute(strKey, oVariant);		
		// Label
		strKey =_T("AuxiProcessing");		
		oVariant = pProcess->m_arrAuxiChannel[i].m_szProcessing;
		oElementChild.setAttribute(strKey, oVariant);	
		oElement.appendChild(lpDispatch);

	}
	lpDispatch = pXMLDOMDocument->createTextNode(_T("\r\n\t\t\t\t\t"));
	oElement.appendChild(lpDispatch);
	lpDispatch = pXMLDOMDocument->createTextNode(_T("\r\n\t\t\t\t"));
	pElement->appendChild(lpDispatch);
	return lCount;
}
/******************************************************************************
    函    数：ParseProcessImpulsive
    
    输    入：
    
    输    出：
  
    返 回 值：
    
    功    能： 解析脉冲类型, 其中 类型 Type ="0" 

	<ProcessRecord Nb="1"  Label ="record" Type ="0" ProcessSetup = "0" SerialNb = "12345"  RecordLength="7" RefractingDelay="1000" TBWindow="200">      		
		<Auxiliaries Count="1"  CorreWith = "Pilot" >	
			<Record AuxiNb ="1" AuxiProcessing="Aux1*Aux2"  />
		</Auxiliaries>      	        	   	
	</ProcessRecord>

    修改历史：

*******************************************************************************/
int COperationParseXML::ParseProcessImpulsive(CXMLDOMElement* pElement,CProcessImpulsive* pProcess)
{
	CString strKey;	// 键名	
	if(NULL==pProcess)
		return 0;
	// 编号
	strKey = _T("Nb");
	pProcess->m_dwProcessNb=CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 类型
	strKey = _T("Type");
	pProcess->m_byProcessType =(BYTE)CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 标签
	strKey = _T("Label");
	pProcess->m_strLabel =CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	// 设置：标准、高级
	strKey = _T("ProcessSetup");
	pProcess->m_ProcessSetup =(BYTE)CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 爆炸机控制器所连采集站SN
	strKey = _T("SerialNb");
	pProcess->m_dwFDUSN =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 记录长度
	strKey = _T("RecordLength");
	pProcess->m_dwRecordLen =CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// Refraction Delay  折射延迟
	strKey = _T("RefractingDelay");
	pProcess->m_dwRefractionDelay=CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
    // TB Window
	strKey = _T("TBWindow");
	pProcess->m_dwTBWindow=CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);
	// 辅助道
	ParseProcessAuxi( pElement,pProcess);
	return 1;
}
/**
 * @brief 保存脉冲对象
 * @note 某一个处理类型的辅助道保存到XML的配置文件中
 
 <ProcessRecord Nb="1" Label="record" Type="0" ProcessSetup="0" SerialNb="12345" RecordLength="20000" RefractingDelay="1000" TBWindow="2000">
	<Auxiliaries Count="1" CorreWith="Pilot">
		<Record AuxiNb="1" AuxiProcessing="Aux1*Aux2"/>
	</Auxiliaries>
 </ProcessRecord>   

 * @param CXMLDOMDocument* pXMLDOMDocument, XML文档对象
 * @param CXMLDOMElement* pElement，指向父节点ProcessRecord元素的指针
 * @param CProcessType* pProcess，处理类型对象
 * @return 读取的对象个数
 */
int COperationParseXML::SaveProcessImpulsive(CXMLDOMDocument* pXMLDOMDocument,CXMLDOMElement* pElement,CProcessImpulsive* pProcess)
{
	CString		strKey;	// 键名	
	COleVariant oVariant;
	VARIANT		vt;
	if(!pProcess || !pElement || !pXMLDOMDocument)
		return 0;
	
	// Nb
	strKey = _T("Nb");
	vt.vt = VT_UI4;
	vt.ulVal = pProcess->m_dwProcessNb;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);		
	// Type
	strKey =_T("Type");	
	oVariant = pProcess->m_byProcessType;
	pElement->setAttribute(strKey, oVariant);	
	// Label
	strKey =_T("Label");
	oVariant = pProcess->m_strLabel;
	pElement->setAttribute(strKey, oVariant);
	// ProcessSetup	
	strKey = _T("ProcessSetup");
	oVariant = pProcess->m_ProcessSetup;
	pElement->setAttribute(strKey, oVariant);
	// SerialNb
	strKey =_T("SerialNb");
	vt.vt = VT_UI4;
	vt.ulVal = pProcess->m_dwFDUSN;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	
	// RecordLength
	strKey =_T("RecordLength");
	vt.vt = VT_UI4;
	vt.ulVal = pProcess->m_dwRecordLen;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);
	// RefractingDelay
	strKey =_T("RefractingDelay");
	vt.vt = VT_UI4;
	vt.ulVal = pProcess->m_dwRefractionDelay;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);	
	// TBWindow
	strKey =_T("TBWindow");
	vt.vt = VT_UI4;
	vt.ulVal = pProcess->m_dwTBWindow;
	oVariant = vt;
	pElement->setAttribute(strKey, oVariant);

	SaveProcessAuxi(pXMLDOMDocument, pElement,(CProcessType*)pProcess);
	return 1;
}

/**
 * @brief 保存数据类型表
 * @note  向XML的配置文件中写入采集排列表对象CShotSpreads。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CShotSpreads* pSpreads，采集排列表对象，也是存储采集排列的集合对象
 * @return 对象个数
 */
int COperationParseXML::SaveProcessType(LPCTSTR pstrXMLFilePath,CProcessTypes* pProcesses)
{
	CString			strOLEObject;
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;	
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElementParent,oElementChild;
	LPDISPATCH		lpDispatch;
	long			lCount;
	CString			strKey;
		
	try
	{
		//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到脉冲震源
		strKey =_T("ProcessType");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 设置点代码总数
		strKey = _T("Count");
		lCount = (long)(pProcesses->GetCount());
		oVariant = lCount;
		oElementParent.setAttribute(strKey, oVariant);
		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		CProcessType* pProcess = NULL;
		for( int i = 0; i < lCount; i++)
		{		
			lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t\t"));
			oElementParent.appendChild(lpDispatch);

			lpDispatch = oXMLDOMDocument.createElement(_T("ProcessRecord"));
			oElementChild.AttachDispatch(lpDispatch);		
			//保存单条放炮记录
			pProcess = pProcesses->GetProcessType(i);
			switch(pProcess->m_byProcessType )
			{
			case PROCESS_IMPULSIVE:
				SaveProcessImpulsive(&oXMLDOMDocument,&oElementChild,(CProcessImpulsive*)pProcess);
				break;
			default:
				break;
			}	

			oElementParent.appendChild(lpDispatch);	
		}
		lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t"));
		oElementParent.appendChild(lpDispatch);

		oVariant = pstrXMLFilePath;	
		oXMLDOMDocument.save(oVariant);
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		theApp.SaveXMLToFTPServer();
	}	
	catch (CException* )
	{
		return 0;
	}
	return lCount;

}
