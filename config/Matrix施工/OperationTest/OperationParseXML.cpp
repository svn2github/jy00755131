#include "StdAfx.h"
#include "OperationParseXML.h"

COperationParseXML::COperationParseXML(void)
{
}

COperationParseXML::~COperationParseXML(void)
{
}

int COperationParseXML::ParseOperationTable(LPCTSTR pstrXMLFilePath)
{
	/*
	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	ULONG		m_dwCount;
try
{

	CoInitialize(NULL);
	// 创建XML对象
	strOLEObject ="msxml2.domdocument";
	BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = "Matrix.xml";
	bData = oXMLDOMDocument.load(oVariant);

	// 找到放炮表区域
	strKey =_T("SurverySetup");
	lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey =_T("Count");
	m_dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);


	// 释放对象
	oXMLDOMDocument.DetachDispatch();
*/
	return 0;
}
catch (COleException* pe)
{
	TRACE (_T("COleException caught. SCODE = %x\n"), pe->m_sc);
	pe->Delete();

	return 0;
	
}
catch (CException* e)
{	
	return 0;
}
}

int COperationParseXML::ParseRecord(CXMLDOMElement* pElement)
{
// 	CString strKey;	// 键名	
// 	// 得到测线号
// 	strKey = "LineName";
// 	m_uiNbLine = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
// 	// 得到测线的测点定义
// 	strKey = "ReceiverSection";
// 	m_strReceiverSection = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	return 0;
}
