#include "StdAfx.h"
#include "TestLimitList.h"

CTestLimitList::CTestLimitList()
{
}

CTestLimitList::~CTestLimitList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestLimitList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CTestLimitList::OnReload()
{
	// 关闭
	OnClose();
	// 初始化
	OnInit();
}

/**
* 关闭
* @param void
* @return void
*/
void CTestLimitList::OnClose()
{
	m_oTestLimitDataMap.RemoveAll();
	m_olsTestLimit.RemoveAll();
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CTestLimitList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到设置区
	if(1 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "InstrumentTestLimit";
	}
	else if(2 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "SensorTestLimit";
	}
	else
	{
		return;
	}
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	lpDispatch = oNodeList.nextNode();
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestLimitData oTestLimitData;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		oTestLimitData.ParseXML(&oElement);
		// 增加到队列
		m_olsTestLimit.AddTail(oTestLimitData);
		// 增加到测试类型索引表
		m_oTestLimitDataMap.SetAt(oTestLimitData.m_uiNb, oTestLimitData);
	}
}

/**
* 保存配置文件
* @param void
* @return void
*/
void CTestLimitList::OnSave()
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到设置区
	if(1 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "InstrumentTestLimit";
	}
	else if(2 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "SensorTestLimit";
	}
	else
	{
		return;
	}
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 得到队列
	lpDispatch = oElementParent.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	// 设置节点
	POSITION pos = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElementChild.AttachDispatch(lpDispatch);

		pos = m_olsTestLimit.FindIndex(i);
		CTestLimitData& oTestLimitData = m_olsTestLimit.GetAt(pos);

		oTestLimitData.AddToXML(&oElementChild);
	}
}