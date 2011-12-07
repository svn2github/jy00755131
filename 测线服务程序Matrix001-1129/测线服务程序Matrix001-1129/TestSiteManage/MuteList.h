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
	CList<CMuteData*, CMuteData*> m_olsMute;
	/** 哑道总数*/
	unsigned int m_uiCountAll;

	CMuteData* m_pMuteData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
	// 根据索引号，得到哑道
	CMuteData* GetMute(unsigned int uiIndex);
};
