#pragma once

#include "AuxData.h"

/**
*@brief 辅助道数据队列类
*/
class CAuxList
{
public:
	CAuxList();
	~CAuxList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 辅助道队列*/
	CList<CAuxData*, CAuxData*> m_olsAux;
	/** 辅助道总数*/
	unsigned int m_uiCountAll;

	/** 辅助道数据指针*/
	CAuxData* m_pAuxData;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();
	// 根据索引号，得到辅助道
	CAuxData* GetAux(unsigned int uiIndex);
	// 根据标识号，得到辅助道
	CAuxData* GetAuxByNb(unsigned int uiNb);
};
