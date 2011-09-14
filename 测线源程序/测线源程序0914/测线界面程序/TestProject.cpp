#include "StdAfx.h"
#include "TestProject.h"

CTestProject::CTestProject()
{
	OnInit();
}

CTestProject::~CTestProject()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestProject::OnInit()
{
	m_uiProjectType = 0;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测

	m_uiTestNb = 0;	// 测试项目索引号
	m_strAuxiliaryDescr = "";	// 辅助道定义
	m_strAbsoluteSpread = "";	// 绝对排列定义
	m_uiDelayBetweenTest = 0;	// 每次测试之间的间隔（毫秒）
	m_uiRecordResult = 0;	// 是否记录测试数据  0-不记录；1-记录
	m_uiRecordLength = 0;	// 测试记录时间（毫秒）
	m_uiTestFileNb = 0;	// 测试文件号
	m_uiCountAll = 0;	// 测试请求数量
}

/**
* 重新加载
* @param void
* @return void
*/
void CTestProject::OnReload()
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
void CTestProject::OnClose()
{
	m_olsTestRequest.RemoveAll();
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CTestProject::ParseXML(CXMLDOMElement* pElement)
{
	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	switch(m_uiProjectType)
	{
	case 1:
		// 解析XML，仪器测试
		ParseXMLForInstrument(pElement);
		break;
	case 2:
		// 解析XML，检波器测试
		ParseXMLForSensor(pElement);
		break;
	case 3:
		// 解析XML，复合测试
		ParseXMLForMultiple(pElement);
		break;
	case 4:
		// 解析XML，噪声监测
		ParseXMLForSeisMonitor(pElement);
		break;
	}
}

/**
* 解析XML，仪器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::ParseXMLForInstrument(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestNb";
	m_uiTestNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试项目索引号
	strKey = "Recorded";
	m_uiRecordResult = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 是否记录测试数据  0-不记录；1-记录
	strKey = "RecordLength";
	m_uiRecordLength = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试记录时间（毫秒）
	strKey = "AuxiliaryDescr";
	m_strAuxiliaryDescr = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 辅助道定义
	strKey = "AbsoluteSpread";
	m_strAbsoluteSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 绝对排列定义

	// 解析测试请求
	CTestRequest oTestRequest;
	// 解析XML，仪器测试
	oTestRequest.ParseXMLForInstrument(pElement);
	m_olsTestRequest.AddTail(oTestRequest);	// 增加到队列
	m_uiCountAll = 1;
}

/**
* 解析XML，检波器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::ParseXMLForSensor(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestNb";
	m_uiTestNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试项目索引号
	strKey = "Recorded";
	m_uiRecordResult = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 是否记录测试数据  0-不记录；1-记录	
	strKey = "AbsoluteSpread";
	m_strAbsoluteSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 绝对排列定义

	// 解析测试请求
	CTestRequest oTestRequest;
	// 解析XML，检波器测试
	oTestRequest.ParseXMLForSensor(pElement);	
	m_olsTestRequest.AddTail(oTestRequest);	// 增加到队列
	m_uiCountAll = 1;
}

/**
* 解析XML，复合测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::ParseXMLForMultiple(CXMLDOMElement* pElement)
{
	// 解析测试请求
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int i;
	CString strKey;

	strKey = "TestNb";
	m_uiTestNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试项目索引号
	strKey = "TestName";
	m_strTestName = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 测试项目名称
	strKey = "DelayBetweenTest";
	m_uiDelayBetweenTest = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	//  每次测试之间的间隔（毫秒）
	strKey = "RecordResults";
	m_uiRecordResult = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 是否记录测试数据  0-不记录；1-记录
	strKey = "RecordLength";
	m_uiRecordLength = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试记录时间（毫秒）
	strKey = "AuxiliaryDescr";
	m_strAuxiliaryDescr = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 辅助道定义
	strKey = "AbsoluteSpread";
	m_strAbsoluteSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 绝对排列定义
	strKey = "TestFileNb";
	m_uiTestFileNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试文件号
	// 得到测试请求总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);	// 测试请求数量
	// 得到Tab键数量
	strKey = "TabCount";
	m_uiTabCount = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);

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
	lpDispatch = pElement->get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);
	CTestRequest oTestRequest;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		oTestRequest.ParseXMLForMultiple(&oElement);
		m_olsTestRequest.AddTail(oTestRequest);	// 增加到队列
	}
}

/**
* 解析XML，噪声监测
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::ParseXMLForSeisMonitor(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "AbsoluteSpread";
	m_strAbsoluteSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 绝对排列定义
	m_uiCountAll = 0;	// 测试请求数量
}

/**
* 加入配置文件
* @param void
* @return void
*/
void CTestProject::AddToXML(CXMLDOMElement* pElement, CXMLDOMDocument* pXMLDOMDocument)
{
	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	switch(m_uiProjectType)
	{
	case 1:
		// 加入XML，仪器测试
		AddToXMLForInstrument(pElement);
		break;
	case 2:
		// 加入XML，检波器测试
		AddToXMLForSensor(pElement);
		break;
	case 3:
		// 加入XML，复合测试
		AddToXMLForMultiple(pElement, pXMLDOMDocument);
		break;
	case 4:
		// 加入XML，噪声监测
		AddToXMLForSeisMonitor(pElement);
		break;
	}
}

/**
* 加入XML，仪器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::AddToXMLForInstrument(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "TestNb";
	oVariant = (long)m_uiTestNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Recorded";
	oVariant = (long)m_uiRecordResult;
	pElement->setAttribute(strKey, oVariant);

	strKey = "RecordLength";
	oVariant = (long)m_uiRecordLength;
	pElement->setAttribute(strKey, oVariant);

	strKey = "AuxiliaryDescr";
	oVariant = m_strAuxiliaryDescr;
	pElement->setAttribute(strKey, oVariant);

	strKey = "AbsoluteSpread";
	oVariant = m_strAbsoluteSpread;
	pElement->setAttribute(strKey, oVariant);

	// 加入测试请求
	CTestRequest oTestRequest = m_olsTestRequest.RemoveHead();
	oTestRequest.AddToXMLForInstrument(pElement);
}

/**
* 加入XML，检波器测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::AddToXMLForSensor(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "TestNb";
	oVariant = (long)m_uiTestNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "Recorded";
	oVariant = (long)m_uiRecordResult;
	pElement->setAttribute(strKey, oVariant);

	strKey = "AbsoluteSpread";
	oVariant = m_strAbsoluteSpread;
	pElement->setAttribute(strKey, oVariant);

	// 加入测试请求
	CTestRequest oTestRequest = m_olsTestRequest.RemoveHead();
	oTestRequest.AddToXMLForSensor(pElement);
}

/**
* 加入XML，复合测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::AddToXMLForMultiple(CXMLDOMElement* pElement, CXMLDOMDocument* pXMLDOMDocument)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "Count";
	oVariant = (long)m_uiCountAll;
	pElement->setAttribute(strKey, oVariant);

	strKey = "TestNb";
	oVariant = (long)m_uiTestNb;
	pElement->setAttribute(strKey, oVariant);

	strKey = "DelayBetweenTest";
	oVariant = (long)m_uiDelayBetweenTest;
	pElement->setAttribute(strKey, oVariant);

	strKey = "RecordResults";
	oVariant = (long)m_uiRecordResult;
	pElement->setAttribute(strKey, oVariant);

	strKey = "RecordLength";
	oVariant = (long)m_uiRecordLength;
	pElement->setAttribute(strKey, oVariant);

	strKey = "AuxiliaryDescr";
	oVariant = m_strAuxiliaryDescr;
	pElement->setAttribute(strKey, oVariant);

	strKey = "AbsoluteSpread";
	oVariant = m_strAbsoluteSpread;
	pElement->setAttribute(strKey, oVariant);

	strKey = "TestFileNb";
	oVariant = (long)m_uiTestFileNb;
	pElement->setAttribute(strKey, oVariant);

	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementChild;
	LPDISPATCH lpDispatch;
	// 删除所有子节点
	while(TRUE == pElement->hasChildNodes())
	{
		lpDispatch = pElement->get_firstChild();
		pElement->removeChild(lpDispatch);
	}

	// 增加新节点
	CTestRequest oTestRequest;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		oTestRequest = m_olsTestRequest.RemoveHead();

		lpDispatch = pXMLDOMDocument->createTextNode(m_strTabChild);
		pElement->appendChild(lpDispatch);

		lpDispatch = pXMLDOMDocument->createElement("Record");
		oElementChild.AttachDispatch(lpDispatch);
		oTestRequest.AddToXMLForMultiple(&oElementChild);
		pElement->appendChild(lpDispatch);

		if(i == (m_uiCountAll - 1))
		{
			lpDispatch = pXMLDOMDocument->createTextNode(m_strTabParent);
			pElement->appendChild(lpDispatch);
		}		
	}
}

/**
* 加入XML，噪声监测
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::AddToXMLForSeisMonitor(CXMLDOMElement* pElement)
{
	CString strKey;	// 键名
	COleVariant oVariant;

	strKey = "AbsoluteSpread";
	oVariant = m_strAbsoluteSpread;
	pElement->setAttribute(strKey, oVariant);
}