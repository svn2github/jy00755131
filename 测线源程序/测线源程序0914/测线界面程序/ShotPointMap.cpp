#include "stdafx.h"
#include "ShotPointMap.h"
CShotPointMap::CShotPointMap(void)
{
	m_uiCountAll = 0;	// 总数
	m_oShotPointMap.clear();
}

CShotPointMap::~CShotPointMap(void)
{
}
/**
* 重新加载
* @param void
* @return void
*/
void CShotPointMap::OnReload(void)
{
	m_uiCountAll = 0;	// 总数
	m_oShotPointMap.clear();	// 清空索引表
	ParseXML();	// 解析配置文件
}
/**
* 解析配置文件
* @param void
* @return void
*/
void CShotPointMap::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int i;
	unsigned int uiShotPoint = 0;
	typedef pair<unsigned int, unsigned int> value_type;

	// 找到设置区
	strKey = "AbsoluteSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	for(i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		strKey = "ShotPoint";
		uiShotPoint = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		value_type p(uiShotPoint, i);
		m_oShotPointMap.insert(p); 
	}
}