#include "StdAfx.h"
#include "TestProjectList.h"

CTestProjectList::CTestProjectList()
{

}

CTestProjectList::~CTestProjectList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestProjectList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CTestProjectList::OnReload()
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
void CTestProjectList::OnClose()
{
	// 总数大于0
	if(m_uiCountAll > 0)
	{
		// 指针
		CTestProject* pTestProject;
		while(FALSE == m_olsTestProject.IsEmpty())
		{
			// 移除队列
			pTestProject = m_olsTestProject.RemoveHead();
			pTestProject->OnClose();
			// 删除
			delete pTestProject;
		}
	}
	m_uiCountAll = 0;	// 数量
}

// 保存配置文件
void CTestProjectList::OnSave()
{
	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	switch(m_uiProjectType)
	{
	case 1:
		// 加入XML，仪器测试
		AddToXMLForInstrument();
		break;
	case 2:
		// 加入XML，检波器测试
		AddToXMLForSensor();
		break;
	case 3:
		// 加入XML，复合测试
		AddToXMLForMultiple();
		break;
	case 4:
		// 加入XML，噪声监测
		AddToXMLForSeisMonitor();
		break;
	}
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CTestProjectList::ParseXML()
{
	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	switch(m_uiProjectType)
	{
	case 1:
		// 解析XML，仪器测试
		ParseXMLForInstrument();
		break;
	case 2:
		// 解析XML，检波器测试
		ParseXMLForSensor();
		break;
	case 3:
		// 解析XML，复合测试
		ParseXMLForMultiple();
		break;
	case 4:
		// 解析XML，噪声监测
		ParseXMLForSeisMonitor();
		break;
	}
}

/**
* 解析配置文件，仪器测试
* @param void
* @return void
*/
void CTestProjectList::ParseXMLForInstrument()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int i;

	// 找到设置区
	strKey = "InstrumentTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到Tab键数量
	strKey = "TabCount";
	m_uiTabCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

	m_strTabChild = "\r\n";
	m_strTabParent = "\r\n";
	for(i = 0; i < m_uiTabCount; i++)
	{
		m_strTabChild = m_strTabChild + "\t";
	}
	for(i = 0; i < m_uiTabCount - 1; i++)
	{
		m_strTabParent = m_strTabParent + "\t";
	}

	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestProject = new CTestProject;
		pTestProject->OnInit();
		pTestProject->m_uiProjectType = 1;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		pTestProject->ParseXML(&oElement);
		// 增加到队列
		m_olsTestProject.AddTail(pTestProject);
	}
}

/**
* 解析配置文件，检波器测试
* @param void
* @return void
*/
void CTestProjectList::ParseXMLForSensor()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int i;

	// 找到设置区
	strKey = "SensorTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到Tab键数量
	strKey = "TabCount";
	m_uiTabCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

	m_strTabChild = "\r\n";
	m_strTabParent = "\r\n";
	for(i = 0; i < m_uiTabCount; i++)
	{
		m_strTabChild = m_strTabChild + "\t";
	}
	for(i = 0; i < m_uiTabCount - 1; i++)
	{
		m_strTabParent = m_strTabParent + "\t";
	}

	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestProject = new CTestProject;
		pTestProject->OnInit();
		pTestProject->m_uiProjectType = 2;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		pTestProject->ParseXML(&oElement);
		// 增加到队列
		m_olsTestProject.AddTail(pTestProject);
	}
}

/**
* 解析配置文件，复合测试
* @param void
* @return void
*/
void CTestProjectList::ParseXMLForMultiple()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int i;

	// 找到设置区
	strKey = "MultipleTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到Tab键数量
	strKey = "TabCount";
	m_uiTabCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

	m_strTabChild = "\r\n";
	m_strTabParent = "\r\n";
	for(i = 0; i < m_uiTabCount; i++)
	{
		m_strTabChild = m_strTabChild + "\t";
	}
	for(i = 0; i < m_uiTabCount - 1; i++)
	{
		m_strTabParent = m_strTabParent + "\t";
	}

	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestProject = new CTestProject;
		pTestProject->OnInit();
		pTestProject->m_uiProjectType = 3;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		pTestProject->ParseXML(&oElement);
		// 增加到队列
		m_olsTestProject.AddTail(pTestProject);
	}
}

/**
* 解析配置文件，噪声监测
* @param void
* @return void
*/
void CTestProjectList::ParseXMLForSeisMonitor()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到设置区
	strKey = "SeisMonitorTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	m_uiCountAll = 1;	// 数量

	CTestProject* pTestProject = NULL;
	pTestProject = new CTestProject;
	pTestProject->OnInit();
	pTestProject->m_uiProjectType = 4;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	pTestProject->ParseXML(&oElement);
	// 增加到队列
	m_olsTestProject.AddTail(pTestProject);
}

/**
* 加入配置文件，仪器测试
* @param void
* @return void
*/
void CTestProjectList::AddToXMLForInstrument()
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到设置区
	strKey = "InstrumentTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 设置总数
	strKey = "Count";
	oVariant = (long)m_uiCountAll;
	oElementParent.setAttribute(strKey, oVariant);

	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}

	// 增加新节点
	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pTestProject = m_olsTestProject.RemoveHead();

		lpDispatch = m_pXMLDOMDocument->createTextNode(m_strTabChild);
		oElementParent.appendChild(lpDispatch);

		lpDispatch = m_pXMLDOMDocument->createElement("Record");
		oElementChild.AttachDispatch(lpDispatch);
		pTestProject->AddToXML(&oElementChild, m_pXMLDOMDocument);
		delete pTestProject;
		oElementParent.appendChild(lpDispatch);

		if(i == (m_uiCountAll - 1))
		{
			lpDispatch = m_pXMLDOMDocument->createTextNode(m_strTabParent);
			oElementParent.appendChild(lpDispatch);
		}		
	}
}

/**
* 加入配置文件，检波器测试
* @param void
* @return void
*/
void CTestProjectList::AddToXMLForSensor()
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到设置区
	strKey = "SensorTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 设置总数
	strKey = "Count";
	oVariant = (long)m_uiCountAll;
	oElementParent.setAttribute(strKey, oVariant);

	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}

	// 增加新节点
	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pTestProject = m_olsTestProject.RemoveHead();

		lpDispatch = m_pXMLDOMDocument->createTextNode(m_strTabChild);
		oElementParent.appendChild(lpDispatch);

		lpDispatch = m_pXMLDOMDocument->createElement("Record");
		oElementChild.AttachDispatch(lpDispatch);
		pTestProject->AddToXML(&oElementChild, m_pXMLDOMDocument);
		delete pTestProject;
		oElementParent.appendChild(lpDispatch);

		if(i == (m_uiCountAll - 1))
		{
			lpDispatch = m_pXMLDOMDocument->createTextNode(m_strTabParent);
			oElementParent.appendChild(lpDispatch);
		}		
	}
}

/**
* 加入配置文件，复合测试测试
* @param void
* @return void
*/
void CTestProjectList::AddToXMLForMultiple()
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到设置区
	strKey = "MultipleTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 得到队列
	lpDispatch = oElementParent.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	// 设置节点属性
	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElementChild.AttachDispatch(lpDispatch);
		POSITION pos = m_olsTestProject.FindIndex(i);
		pTestProject = m_olsTestProject.GetAt(pos);
		pTestProject->AddToXML(&oElementChild, m_pXMLDOMDocument);
	}
}

/**
* 加入配置文件，噪声监测
* @param void
* @return void
*/
void CTestProjectList::AddToXMLForSeisMonitor()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到设置区
	strKey = "SeisMonitorTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = m_olsTestProject.GetHead();
	pTestProject->AddToXML(&oElement, m_pXMLDOMDocument);
}