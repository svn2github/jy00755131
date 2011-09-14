#include "StdAfx.h"
#include "TestBaseList.h"

CTestBaseList::CTestBaseList()
{
}

CTestBaseList::~CTestBaseList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestBaseList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CTestBaseList::OnReload()
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
void CTestBaseList::OnClose()
{
	m_oTestBaseDataMap.RemoveAll();
	m_olsTestBase.RemoveAll();
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CTestBaseList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到设置区
	if(1 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "InstrumentTestBase";
	}
	else if(2 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "SensorTestBase";
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

	CTestBaseData oTestBaseData;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		oTestBaseData.ParseXML(&oElement);
		// 增加到队列
		m_olsTestBase.AddTail(oTestBaseData);
		// 增加到测试类型索引表
		m_oTestBaseDataMap.SetAt(oTestBaseData.m_uiType, oTestBaseData);
	}
}

/**
* 保存配置文件
* @param void
* @return void
*/
void CTestBaseList::OnSave()
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到设置区
	if(1 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "InstrumentTestBase";
	}
	else if(2 == m_uiAim)	// 测试对象	1-仪器；2-检波器
	{
		strKey = "SensorTestBase";
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

		pos = m_olsTestBase.FindIndex(i);
		CTestBaseData& oTestBaseData = m_olsTestBase.GetAt(pos);

		oTestBaseData.AddToXML(&oElementChild);
	}
}