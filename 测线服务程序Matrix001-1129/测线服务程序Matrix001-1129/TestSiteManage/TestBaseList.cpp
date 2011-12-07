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
	// 清空索引表
	m_oTestBaseMap.RemoveAll();
	// 总数大于0
	if(m_uiCountAll > 0)
	{
		// 指针
		CTestBaseData* pTestBaseData;
		while(FALSE == m_olsTestBase.IsEmpty())
		{
			// 移除队列
			pTestBaseData = m_olsTestBase.RemoveHead();
			// 删除
			delete pTestBaseData;
		}
	}
	m_uiCountAll = 0;	// 数量
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
	long lData = oNodeList.get_length();
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

	CTestBaseData* pTestBaseData = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestBaseData = new CTestBaseData;
		pTestBaseData->ParseXML(&oElement);
		// 增加到队列
		m_olsTestBase.AddTail(pTestBaseData);
		// 增加到索引表
		m_oTestBaseMap.SetAt(pTestBaseData->m_uiType, pTestBaseData);
	}
}

/**
* 根据索引号，得到测试基本设置
* @param unsigned int uiIndex 索引号
* @return CTestBaseData* 测试基本设置指针 NULL：无
*/
CTestBaseData* CTestBaseList::GetTestBaseDataByIndex(unsigned int uiIndex)
{
	CTestBaseData* pTestBaseData = NULL;
	POSITION pos = NULL;
	pos = m_olsTestBase.FindIndex(uiIndex);
	if(NULL != pos)
	{
		pTestBaseData = m_olsTestBase.GetAt(pos);
	}
	return pTestBaseData;
}

/**
* 根据测试类型，得到测试基本设置
* @param unsigned int uiTestType 测试类型
* @return CTestBaseData* 测试基本设置指针 NULL：无
*/
CTestBaseData* CTestBaseList::GetTestBaseDataByTestType(unsigned int uiTestType)
{
	CTestBaseData* pTestBaseData = NULL;
	m_oTestBaseMap.Lookup(uiTestType, pTestBaseData);
	return pTestBaseData;
}

/**
* 重新加载测试配置文件
* @param void
* @return void
*/
void CTestBaseList::OnReloadTestSetupDataForApplyTestBase()
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
	long lData = oNodeList.get_length();
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
	CTestBaseData* pTestBaseData = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		oTestBaseData.ParseXML(&oElement);
		pTestBaseData = GetTestBaseDataByIndex(i);
		*pTestBaseData = oTestBaseData;
	}
}