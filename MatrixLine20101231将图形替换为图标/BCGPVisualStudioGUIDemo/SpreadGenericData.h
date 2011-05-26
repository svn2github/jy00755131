#pragma once

/**
*@brief 相对排列数据类
*/
class CSpreadGenericData
{
public:
	CSpreadGenericData();
	~CSpreadGenericData();

public: //属性
	/** 索引号*/
	unsigned int m_uiNb;
	/** 标签*/
	CString m_strLabel;
	/** 测线*/
	CString m_strLine;
	/** 排列*/
	CString m_strSpread;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
