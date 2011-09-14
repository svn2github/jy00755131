#pragma once

#include "MuteData.h"

/**
*@brief 哑点队列类
*/
class CMuteList
{
public:
	CMuteList();
	~CMuteList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 哑道队列*/
	CList<CMuteData, CMuteData> m_olsMute;
	/** 哑道总数*/
	unsigned int m_uiCountAll;

	/**  Tab键数量*/
	unsigned int m_uiTabCount;
	/** 写格式化XML文件用*/
	CString m_strTabChild;
	/** 写格式化XML文件用*/
	CString m_strTabParent;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 保存配置文件
	void OnSave();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
};
