#pragma once

/**
*@brief Look设置类
*/
class CLookSetupData
{
public:
	CLookSetupData();
	~CLookSetupData();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 是否自动查找*/
	UINT m_uiLookAuto;	
	/** 新仪器是否测试Resistance*/
	UINT m_uiTestResistance;	
	/** 新仪器是否测试Tilt*/
	UINT m_uiTestTilt;	
	/** 新仪器是否测试Leakage*/
	UINT m_uiTestLeakage;	
public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
};
