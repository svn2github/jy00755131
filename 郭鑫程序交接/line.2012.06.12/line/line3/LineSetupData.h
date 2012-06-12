#pragma once

/**
*@brief 测线数据类
*/
class CLineSetupData
{
public:
	CLineSetupData(void);
	~CLineSetupData(void);

public: // 属性	
	/** 测线号*/
	unsigned int m_uiNbLine;
	/** 测线上的测点定义*/
	CString m_strReceiverSection;

public: // 方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
