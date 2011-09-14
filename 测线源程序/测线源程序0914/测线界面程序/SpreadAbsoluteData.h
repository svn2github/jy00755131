#pragma once

/**
*@brief 绝对排列数据类
*/
class CSpreadAbsoluteData
{
public:
	CSpreadAbsoluteData();
	~CSpreadAbsoluteData();

public: //属性
	/** 索引号*/
	unsigned int m_uiNb;
	/** 标签*/
	CString m_strLabel;
	/** 排列*/
	CString m_strSpread;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
