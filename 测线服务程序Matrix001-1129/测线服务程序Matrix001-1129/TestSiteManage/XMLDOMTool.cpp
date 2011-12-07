#include "StdAfx.h"
#include "XMLDOMTool.h"

CXMLDOMTool::CXMLDOMTool()
{
}

CXMLDOMTool::~CXMLDOMTool()
{
}

/**
* 解析属性，返回字符串
* @param CXMLDOMElement* pElement XML文档节点指针
* @param CString strKey 属性名
* @return CString 属性值
*/
CString CXMLDOMTool::GetElementAttributeString(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	CString strData = oVariant;
	return strData;
}

/**
* 解析属性，返回整数
* @param CXMLDOMElement* pElement XML文档节点指针
* @param CString strKey 属性名
* @return int 整数
*/
int CXMLDOMTool::GetElementAttributeInt(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_INT);
	int iData = oVariant.intVal;
	return iData;
}

/**
* 解析属性，返回无符号整数
* @param CXMLDOMElement* pElement XML文档节点指针
* @param CString strKey 属性名
* @return unsigned int 无符号整数
*/
unsigned int CXMLDOMTool::GetElementAttributeUnsignedInt(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_UINT);
	unsigned int uiData = oVariant.uintVal;
	return uiData;
}

/**
* 解析属性，返回单精度浮点数
* @param CXMLDOMElement* pElement XML文档节点指针
* @param CString strKey 属性名
* @return float 单精度浮点数
*/
float CXMLDOMTool::GetElementAttributeFloat(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_R4);
	float fData = oVariant.fltVal;
	return fData;
}

/**
* 解析属性，返回双精度浮点数
* @param CXMLDOMElement* pElement XML文档节点指针
* @param CString strKey 属性名
* @return double 双精度浮点数
*/
double CXMLDOMTool::GetElementAttributeDouble(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_R8);
	double dData = oVariant.dblVal;
	return dData;
}

// 字符串转换为整数
int CXMLDOMTool::ConvertStringToInt(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_INT);
	int iData = oVariant.intVal;
	return iData;
}

/**
* 字符串转换为无符号整数
* @param CString strData 字符串
* @return unsigned int 无符号整数
*/
unsigned int CXMLDOMTool::ConvertStringToUnsignedInt(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_UINT);
	unsigned int uiData = oVariant.uintVal;
	return uiData;
}

/**
* 字符串转换为单精度浮点数
* @param CString strData 字符串
* @return float 单精度浮点数
*/
float CXMLDOMTool::ConvertStringToFloat(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_R4);
	float fData = oVariant.fltVal;
	return fData;
}

/**
* 字符串转换为双精度浮点数
* @param CString strData 字符串
* @return double 双精度浮点数
*/
double CXMLDOMTool::ConvertStringToDouble(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_R8);
	double dData = oVariant.dblVal;
	return dData;
}