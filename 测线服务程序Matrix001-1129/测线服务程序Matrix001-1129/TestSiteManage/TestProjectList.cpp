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
* 重新加载仪器测试配置文件
* @param void
* @return void
*/
void CTestProjectList::OnReloadTestSetupDataForApplyInstrument()
{
	// 删除测试项目，根据测试类型
	DeleteTestProjectByProjectType(1);
	// 解析XML，仪器测试
	ParseXMLForInstrument();
}

/**
* 重新加载检波器测试配置文件
* @param void
* @return void
*/
void CTestProjectList::OnReloadTestSetupDataForApplySensor()
{
	// 删除测试项目，根据测试类型
	DeleteTestProjectByProjectType(2);
	// 解析XML，检波器测试
	ParseXMLForSensor();
}

/**
* 重新加载复合测试测试配置文件
* @param void
* @return void
*/
void CTestProjectList::OnReloadTestSetupDataForApplyMultiple()
{
	// 删除测试项目，根据测试类型
	DeleteTestProjectByProjectType(3);
	// 解析XML，复合测试
	ParseXMLForMultiple();
}

/**
* 重新加载噪声监测配置文件
* @param void
* @return void
*/
void CTestProjectList::OnReloadTestSetupDataForApplySeisMonitor()
{
	// 删除测试项目，根据测试类型
	DeleteTestProjectByProjectType(4);
	// 解析XML，噪声监测
	ParseXMLForSeisMonitor();
}

/**
* 根据测试项目类型，删除测试项目
* @param unsigned int uiProjectType 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
* @return void
*/
void CTestProjectList::DeleteTestProjectByProjectType(unsigned int uiProjectType)
{
	if(0 == m_uiCountAll)
	{
		return;
	}
	
	CTestProject* pTestProject = NULL;
	POSITION pos = NULL;
	for(int i = (int)(m_uiCountAll - 1); i >= 0; i--)
	{
		pos = m_olsTestProject.FindIndex(i);
		pTestProject = m_olsTestProject.GetAt(pos);
		if(uiProjectType == 1)
		{
			if((pTestProject->m_uiTestNb > 1000) && (pTestProject->m_uiTestNb < 2000))
			{
				m_olsTestProject.RemoveAt(pos);
				m_oTestProjectMap.RemoveKey(pTestProject->m_uiTestNb);
				pTestProject->OnClose();
				delete pTestProject;
			}
		}
		else if(uiProjectType == 2)
		{
			if((pTestProject->m_uiTestNb > 2000) && (pTestProject->m_uiTestNb < 3000))
			{
				m_olsTestProject.RemoveAt(pos);
				m_oTestProjectMap.RemoveKey(pTestProject->m_uiTestNb);
				pTestProject->OnClose();
				delete pTestProject;
			}
		}
		else if(uiProjectType == 3)
		{
			if((pTestProject->m_uiTestNb > 3000) && (pTestProject->m_uiTestNb < 4000))
			{
				m_olsTestProject.RemoveAt(pos);
				m_oTestProjectMap.RemoveKey(pTestProject->m_uiTestNb);
				pTestProject->OnClose();
				delete pTestProject;
			}
		}
		else if(uiProjectType == 4)
		{
			if(pTestProject->m_uiTestNb == 4000)
			{
				m_olsTestProject.RemoveAt(pos);
				m_oTestProjectMap.RemoveKey(pTestProject->m_uiTestNb);
				pTestProject->OnClose();
				delete pTestProject;
			}
		}
	}
	m_uiCountAll = m_olsTestProject.GetCount();	// 数量
}

/**
* 关闭
* @param void
* @return void
*/
void CTestProjectList::OnClose()
{
	// 清空索引表
	m_oTestProjectMap.RemoveAll();
	// 总数大于0
	if(m_uiCountAll > 0)
	{
		// 指针
		CTestProject* pTestProject;
		while(FALSE == m_olsTestProject.IsEmpty())
		{
			// 移除队列
			pTestProject = m_olsTestProject.RemoveHead();
			pTestProject->OnReset();
			// 删除
			delete pTestProject;
		}
	}
	m_uiCountAll = 0;	// 数量
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CTestProjectList::ParseXML()
{
	m_uiCountAll = 0;	// 测试请求数量
	// 解析XML，仪器测试
	ParseXMLForInstrument();
	// 解析XML，检波器测试
	ParseXMLForSensor();
	// 解析XML，复合测试
	ParseXMLForMultiple();
	// 解析XML，噪声监测
	ParseXMLForSeisMonitor();
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

	// 找到设置区
	strKey = "InstrumentTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	unsigned int uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	m_uiCountAll = m_uiCountAll + uiCountAll;	// 数量
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestProject = new CTestProject;
		pTestProject->m_pTestBaseListInstrument = m_pTestBaseListInstrument;	// 仪器测试基本参数队列指针
		pTestProject->m_pTestBaseListSensor = m_pTestBaseListSensor;	// 检波器测试基本参数队列指针
		pTestProject->OnInit();
		pTestProject->ParseXMLForInstrument(&oElement);
		// 增加到队列
		m_olsTestProject.AddTail(pTestProject);
		// 增加到索引表
		m_oTestProjectMap.SetAt(pTestProject->m_uiTestNb, pTestProject);
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

	// 找到设置区
	strKey = "SensorTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	unsigned int uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	m_uiCountAll = m_uiCountAll + uiCountAll;	// 数量
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestProject = new CTestProject;
		pTestProject->m_pTestBaseListInstrument = m_pTestBaseListInstrument;	// 仪器测试基本参数队列指针
		pTestProject->m_pTestBaseListSensor = m_pTestBaseListSensor;	// 检波器测试基本参数队列指针
		pTestProject->OnInit();
		pTestProject->ParseXMLForSensor(&oElement);
		// 增加到队列
		m_olsTestProject.AddTail(pTestProject);
		// 增加到索引表
		m_oTestProjectMap.SetAt(pTestProject->m_uiTestNb, pTestProject);
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
	unsigned int uiCountProject = 0;	// 有效复合测试数量

	// 找到设置区
	strKey = "MultipleTest";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到总数
	strKey = "Count";
	unsigned int uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CTestProject* pTestProject = NULL;
	for(unsigned int i = 0; i < uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		// 得到测试请求总数
		strKey = "Count";
		unsigned int uiRequestCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);	// 测试请求数量
		if(0 < uiRequestCount)
		{
			pTestProject = new CTestProject;
			pTestProject->m_pTestBaseListInstrument = m_pTestBaseListInstrument;	// 仪器测试基本参数队列指针
			pTestProject->m_pTestBaseListSensor = m_pTestBaseListSensor;	// 检波器测试基本参数队列指针
			pTestProject->OnInit();
			pTestProject->ParseXMLForMultiple(&oElement);
			// 增加到队列
			m_olsTestProject.AddTail(pTestProject);
			// 增加到索引表
			m_oTestProjectMap.SetAt(pTestProject->m_uiTestNb, pTestProject);
			uiCountProject++;
		}
	}
	m_uiCountAll = m_uiCountAll + uiCountProject;	// 数量
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
	m_uiCountAll = m_uiCountAll + 1;	// 数量

	CTestProject* pTestProject = NULL;
	pTestProject = new CTestProject;
	pTestProject->m_pTestBaseListInstrument = m_pTestBaseListInstrument;	// 仪器测试基本参数队列指针
	pTestProject->m_pTestBaseListSensor = m_pTestBaseListSensor;	// 检波器测试基本参数队列指针
	pTestProject->OnInit();
	pTestProject->ParseXMLForSeisMonitor(&oElement);
	// 增加到队列
	m_olsTestProject.AddTail(pTestProject);
	// 增加到索引表
	m_oTestProjectMap.SetAt(pTestProject->m_uiTestNb, pTestProject);
}

/**
* 测试索引号是否已加入索引表
* @param unsigned int uiIndex 测试索引号
* @return BOOL TRUE：是；FALSE：否
*/
BOOL CTestProjectList::IfIndexExistInMap(unsigned int uiIndex)
{
	CTestProject* pTestProject = NULL;
	return m_oTestProjectMap.Lookup(uiIndex, pTestProject);
}

/**
* 根据输入索引号，由索引表得到测试项目指针
* @param unsigned int uiIndex 索引号
* @param CTestProject* &pTestProject	测试项目指针，引用调用
* @return BOOL TRUE：成功；FALSE：失败
*/
BOOL CTestProjectList::GetTestProject(unsigned int uiIndex, CTestProject* &pTestProject)
{
	return m_oTestProjectMap.Lookup(uiIndex, pTestProject);
}

