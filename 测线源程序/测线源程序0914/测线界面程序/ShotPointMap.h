#pragma once
#include <map>
using namespace std;
class CShotPointMap
{
public:
	CShotPointMap(void);
	~CShotPointMap(void);
public: //属性
	/** XMLDOM文件指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;
	/** 总数*/
	unsigned int m_uiCountAll;
	// 炮点索引表
	// 关键字为炮点号，后一个参数为ID号
	map<unsigned int, unsigned int> m_oShotPointMap;

public: //方法
	// 重新加载
	void OnReload();
	// 解析配置文件
	void ParseXML();
};
