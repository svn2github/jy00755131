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
	// 清空索引表
	m_oTestLimitMap.RemoveAll();
	// 总数大于0
	if(m_uiCountAll > 0)
	{
		// 指针
		CTestLimitData* pTestLimitData;
		while(FALSE == m_olsTestLimit.IsEmpty())
		{
			// 移除队列
			pTestLimitData = m_olsTestLimit.RemoveHead();
			// 删除
			delete pTestLimitData;
		}
	}
	m_uiCountAll = 0;	// 数量
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

	CTestLimitData* pTestLimitData = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestLimitData = new CTestLimitData;
		pTestLimitData->ParseXML(&oElement);
		// 增加到队列
		m_olsTestLimit.AddTail(pTestLimitData);
		// 增加到索引表
		m_oTestLimitMap.SetAt(pTestLimitData->m_uiNb, pTestLimitData);
	}
}

/**
* 根据索引号，得到测试判据设置
* @param unsigned int uiIndex 索引号
* @return CTestLimitData* 测试判据设置指针 NULL：无
*/
CTestLimitData* CTestLimitList::GetTestLimitDataByIndex(unsigned int uiIndex)
{
	CTestLimitData* pTestLimitData = NULL;
	POSITION pos = NULL;
	pos = m_olsTestLimit.FindIndex(uiIndex);
	if(NULL != pos)
	{
		pTestLimitData = m_olsTestLimit.GetAt(pos);
	}
	return pTestLimitData;
}

/**
* 根据索引号，得到测试判据
* @param unsigned int uiIndex 索引号
* @return float 测试判据
*/
float CTestLimitList::GetTestLimitValueByIndex(unsigned int uiIndex)
{
	CTestLimitData* pTestLimitData = NULL;
	if(TRUE == m_oTestLimitMap.Lookup(uiIndex, pTestLimitData))
	{
		return pTestLimitData->m_fLimit;
	}
	return 0.0;	
}

/**
* 重新加载测试配置文件
* @param void
* @return void
*/
void CTestLimitList::OnReloadTestSetupDataForApplyTestLimit()
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

	CTestLimitData oTestLimitData;
	CTestLimitData* pTestLimitData = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		oTestLimitData.ParseXML(&oElement);
		pTestLimitData = GetTestLimitDataByIndex(i);
		*pTestLimitData = oTestLimitData;
	}
}