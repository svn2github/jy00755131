#pragma once

/**
*@brief 点代码数据类
*/
class CPointCodeData
{
public:
	CPointCodeData(void);
	~CPointCodeData(void);

public: //属性
	/** 索引号*/
	unsigned int m_uiNb;
	/** 标签*/
	CString m_strLabel;
	/** 检波器类型*/
	CString m_strSensorType;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
	// 加入XML
	void AddToXML(CXMLDOMElement* pElement);
};
