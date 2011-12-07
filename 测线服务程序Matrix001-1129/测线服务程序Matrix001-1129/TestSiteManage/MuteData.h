#pragma once

/**
*@brief 哑点数据类
*/
class CMuteData
{
public:
	CMuteData();
	~CMuteData();

public: //属性
	/** 测线号*/
	unsigned int m_uiLineName;
	/** 测点号*/
	unsigned int m_uiPointNb;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
};
