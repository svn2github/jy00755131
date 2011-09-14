#pragma once

#include "BlastMachineData.h"

/**
*@brief 爆炸机控制器数据队列类
*/
class CBlastMachineList
{
public:
	CBlastMachineList();
	~CBlastMachineList();

public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 爆炸机控制器队列*/
	CList<CBlastMachineData, CBlastMachineData> m_olsBlastMachine;
	/** 爆炸机控制器总数*/
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
