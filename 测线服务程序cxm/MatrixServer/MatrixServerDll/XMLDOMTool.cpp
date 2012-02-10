#include "StdAfx.h"
#include "XMLDOMTool.h"

CXMLDOMTool::CXMLDOMTool(void)
{
}

CXMLDOMTool::~CXMLDOMTool(void)
{
}

// 解析属性，返回字符串
CString CXMLDOMTool::GetElementAttributeString(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	CString strData = oVariant;
	return strData;
}

// 解析属性，返回整数
int CXMLDOMTool::GetElementAttributeInt(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_INT);
	int iData = oVariant.intVal;
	return iData;
}

// 解析属性，返回无符号整数
unsigned int CXMLDOMTool::GetElementAttributeUnsignedInt(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_UINT);
	unsigned int uiData = oVariant.uintVal;
	return uiData;
}
// 解析属性，返回无符号整数
unsigned short CXMLDOMTool::GetElementAttributeUnsignedShort(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_UI2);
	unsigned short uiData = oVariant.uiVal;
	return uiData;
}

// 解析属性，返回单精度浮点数
float CXMLDOMTool::GetElementAttributeFloat(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_R4);
	float fData = oVariant.fltVal;
	return fData;
}

// 解析属性，返回双精度浮点数
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

// 字符串转换为无符号整数
unsigned int CXMLDOMTool::ConvertStringToUnsignedInt(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_UINT);
	unsigned int uiData = oVariant.uintVal;
	return uiData;
}

// 字符串转换为单精度浮点数
float CXMLDOMTool::ConvertStringToFloat(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_R4);
	float fData = oVariant.fltVal;
	return fData;
}

// 字符串转换为双精度浮点数
double CXMLDOMTool::ConvertStringToDouble(CString strData)
{
	COleVariant oVariant = strData;
	oVariant.ChangeType(VT_R8);
	double dData = oVariant.dblVal;
	return dData;
}
// 解析属性，返回字符
byte CXMLDOMTool::GetElementAttributeByte(CXMLDOMElement* pElement, CString strKey)
{
	COleVariant oVariant = pElement->getAttribute(strKey);
	oVariant.ChangeType(VT_UI1);
	byte ucData = oVariant.bVal;
	return ucData;
}
