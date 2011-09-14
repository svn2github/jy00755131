#pragma once

/**
*@brief 测试判据设置类
*/
class CTestLimitData
{
public:
	CTestLimitData();
	~CTestLimitData();

public: //属性
	/** 测试设置目标 1：仪器；2：检波器*/
	unsigned int m_uiTestAim;
	/** 索引号*/
	unsigned int m_uiNb;
	/** 描述*/
	CString m_strDesc;
	/** 单位*/
	CString m_strUnit;
	/** 测试类型*/
	unsigned int m_uiType;
	/** 判据*/
	float m_fLimit;

public: //方法
	// 解析XML
	void ParseXML(CXMLDOMElement* pElement);
};