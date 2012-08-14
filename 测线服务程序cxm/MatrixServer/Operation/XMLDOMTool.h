#pragma once

class CXMLDOMTool
{
public:
	CXMLDOMTool(void);
	~CXMLDOMTool(void);

public: //方法
	// 解析属性，返回字符串
	static CString GetElementAttributeString(CXMLDOMElement* pElement, CString strKey);
	// 解析属性，返回整数
	static int GetElementAttributeInt(CXMLDOMElement* pElement, CString strKey);
	// 解析属性，返回无符号整数
	static unsigned int GetElementAttributeUnsignedInt(CXMLDOMElement* pElement, CString strKey);
	// 解析属性，返回无符号2字节整数
	static unsigned short GetElementAttributeUnsignedShort(CXMLDOMElement* pElement, CString strKey);
	// 解析属性，返回单精度浮点数
	static float GetElementAttributeFloat(CXMLDOMElement* pElement, CString strKey);
	// 解析属性，返回双精度浮点数
	static double GetElementAttributeDouble(CXMLDOMElement* pElement, CString strKey);
	// 字符串转换为整数
	static int ConvertStringToInt(CString strData);
	// 字符串转换为无符号整数
	static unsigned int ConvertStringToUnsignedInt(CString strData);
	// 字符串转换为单精度浮点数
	static float ConvertStringToFloat(CString strData);
	// 字符串转换为双精度浮点数
	static double ConvertStringToDouble(CString strData);
	// 解析属性，返回字符
	static byte GetElementAttributeByte(CXMLDOMElement* pElement, CString strKey);
};
