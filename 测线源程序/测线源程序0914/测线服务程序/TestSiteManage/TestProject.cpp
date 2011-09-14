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
* 重置
* @param void
* @return void
*/
void CTestProject::OnReset()
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
	// 总数大于0
	if(m_uiCountAll > 0)
	{
		// 指针
		CTestRequest* pTestRequest;
		while(FALSE == m_olsTestRequest.IsEmpty())
		{
			// 移除队列
			pTestRequest = m_olsTestRequest.RemoveHead();
			// 删除
			delete pTestRequest;
		}
	}
	m_uiCountAll = 0;
	m_olsTestRequestTemp.RemoveAll();	// 测试请求队列，临时
	m_olsIP.RemoveAll();	// 参加测试仪器的IP地址队列
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
	m_uiTestNb = m_uiTestNb + 1000;
	strKey = "Recorded";
	m_uiRecordResult = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 是否记录测试数据  0-不记录；1-记录
	strKey = "RecordLength";
	m_uiRecordLength = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试记录时间（毫秒）
	strKey = "AuxiliaryDescr";
	m_strAuxiliaryDescr = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 辅助道定义
	strKey = "AbsoluteSpread";
	m_strAbsoluteSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 绝对排列定义
	m_uiDelayBetweenTest = 1000;	// 每次测试之间的间隔（毫秒）
	m_uiProjectType = 1;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测

	// 解析测试请求
	CTestRequest* pTestRequest = new CTestRequest;
	// 解析XML，仪器测试
	pTestRequest->ParseXMLForInstrument(pElement);
	m_olsTestRequestTemp.AddTail(pTestRequest);	// 增加到临时队列
	// 整理要求一次测试全部项目的测试请求，仪器测试
	ReArrangeByTestType(pTestRequest);
	m_uiCountAll = m_olsTestRequest.GetCount();	// 测试请求数量
	delete pTestRequest;
	m_olsTestRequestTemp.RemoveAll();
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
	m_uiTestNb = m_uiTestNb + 2000;
	strKey = "Recorded";
	m_uiRecordResult = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 是否记录测试数据  0-不记录；1-记录	
	strKey = "AbsoluteSpread";
	m_strAbsoluteSpread = CXMLDOMTool::GetElementAttributeString(pElement, strKey);	// 绝对排列定义
	m_strAuxiliaryDescr = "";	// 辅助道定义
	m_uiRecordLength = 1000;	// 测试记录时间（毫秒）
	m_uiDelayBetweenTest = 1000;	// 每次测试之间的间隔（毫秒）
	m_uiProjectType = 2;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测

	// 解析测试请求
	CTestRequest* pTestRequest = new CTestRequest;
	// 解析XML，检波器测试
	pTestRequest->ParseXMLForSensor(pElement);	
	m_olsTestRequestTemp.AddTail(pTestRequest);	// 增加到临时队列
	// 整理要求一次测试全部项目的测试请求，检波器测试
	ReArrangeByTestType(pTestRequest);
	m_uiCountAll = m_olsTestRequest.GetCount();	// 测试请求数量
	delete pTestRequest;
	m_olsTestRequestTemp.RemoveAll();
}

/**
* 解析XML，复合测试
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CTestProject::ParseXMLForMultiple(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "TestNb";
	m_uiTestNb = CXMLDOMTool::GetElementAttributeUnsignedInt(pElement, strKey);	// 测试项目索引号
	m_uiTestNb = m_uiTestNb + 3000;
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
	m_uiProjectType = 3;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测

	// 解析测试请求
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	// 得到测试请求总数
	strKey = "Count";
	unsigned int uiCountAll = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);	// 测试请求数量
	// 得到队列
	lpDispatch = pElement->get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);
	CTestRequest* pTestRequest = NULL;
	for(unsigned int i = 0; i < uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pTestRequest = new CTestRequest;
		pTestRequest->ParseXMLForMultiple(&oElement);
		m_olsTestRequestTemp.AddTail(pTestRequest);	// 增加到临时队列
	}
	// 整理复合测试的测试请求
	ReArrangeForMultiple();
	m_uiCountAll = m_olsTestRequest.GetCount();	// 测试请求数量
	while(FALSE == m_olsTestRequestTemp.IsEmpty())
	{		
		pTestRequest = m_olsTestRequestTemp.RemoveHead();	// 移除队列		
		delete pTestRequest;	// 删除
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
	m_uiTestNb = 4000;	// 测试项目索引号
	m_strAuxiliaryDescr = "";	// 辅助道定义
	m_uiDelayBetweenTest = 0;	// 每次测试之间的间隔（毫秒）
	m_uiRecordResult = 0;	// 是否记录测试数据  0-不记录；1-记录
	m_uiRecordLength = 86400000;	// 测试记录时间（毫秒）	24小时
	m_uiProjectType = 4;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测

	// 解析测试请求
	CTestRequest* pTestRequest = new CTestRequest;
	pTestRequest->m_uiIndex = 1;	// 索引号	
	pTestRequest->m_uiAim = 2;	// 测试对象	1-仪器；2-检波器；3-全部
	pTestRequest->m_uiType = 22;	// 测试类型
	pTestRequest->m_uiGain = 1600;	// 增益
	pTestRequest->m_uiLoopNb = 0;	// 跳转到的索引号
	pTestRequest->m_uiLoopCount = 0;	// 循环次数
	// 增加到队列
	m_olsTestRequest.AddTail(pTestRequest);
	m_uiCountAll = 1;	// 测试请求数量
}

/**
* 整理要求一次测试全部项目的测试请求
* @param CTestRequest* pTestRequest 测试请求指针
* @return void
*/
void CTestProject::ReArrangeByTestType(CTestRequest* pTestRequest)
{
	if(30 == pTestRequest->m_uiType)	// 30 仪器全部测试项目
	{
		CTestRequest* pTestRequestNew = NULL;
		for(unsigned int i = 0; i < m_pTestBaseListInstrument->m_uiCountAll; i++)
		{
			pTestRequestNew = new CTestRequest;
			*pTestRequestNew = *pTestRequest;
			pTestRequestNew->m_uiType = m_pTestBaseListInstrument->GetTestBaseDataByIndex(i)->m_uiType;
			pTestRequest->m_uiAim =1;
			m_olsTestRequest.AddTail(pTestRequestNew);
		}
	}
	else if(40 == pTestRequest->m_uiType)	// 40 检波器全部测试项目
	{
		CTestRequest* pTestRequestNew = NULL;
		for(unsigned int i = 0; i < (m_pTestBaseListSensor->m_uiCountAll - 1); i++)
		{
			pTestRequestNew = new CTestRequest;
			*pTestRequestNew = *pTestRequest;
			pTestRequestNew->m_uiType = m_pTestBaseListSensor->GetTestBaseDataByIndex(i)->m_uiType;
			pTestRequest->m_uiAim =2;
			m_olsTestRequest.AddTail(pTestRequestNew);
		}
	}
	else if(50 == pTestRequest->m_uiType)	// 50 仪器和检波器全部测试项目
	{
		CTestRequest* pTestRequestNew = NULL;
		unsigned int i;
		for(i = 0; i < m_pTestBaseListInstrument->m_uiCountAll; i++)
		{
			pTestRequestNew = new CTestRequest;
			*pTestRequestNew = *pTestRequest;
			pTestRequestNew->m_uiType = m_pTestBaseListInstrument->GetTestBaseDataByIndex(i)->m_uiType;
			pTestRequest->m_uiAim =1;
			m_olsTestRequest.AddTail(pTestRequestNew);
		}
		
		for(i = 0; i < (m_pTestBaseListSensor->m_uiCountAll - 1); i++)
		{
			pTestRequestNew = new CTestRequest;
			*pTestRequestNew = *pTestRequest;
			pTestRequestNew->m_uiType = m_pTestBaseListSensor->GetTestBaseDataByIndex(i)->m_uiType;
			pTestRequest->m_uiAim =1;
			m_olsTestRequest.AddTail(pTestRequestNew);
		}
	}
	else if(0 == pTestRequest->m_uiLoopNb)	// 非控制请求
	{
		CTestRequest* pTestRequestNew = NULL;
		pTestRequestNew = new CTestRequest;
		*pTestRequestNew = *pTestRequest;
		// 设置测试请求的测试目标 1-仪器；2-检波器
		SetTestRequestAim(pTestRequestNew);
		m_olsTestRequest.AddTail(pTestRequestNew);
	}
}

/**
* 整理复合测试的测试请求
* @param void
* @return void
*/
void CTestProject::ReArrangeForMultiple()
{
	unsigned int uiCountAll = 0;
	unsigned int uiNbStart = 0;
	CTestRequest* pTestRequest = NULL;
	bool bStartFound = false;
	bool bLoopContinue = false;
	POSITION pos;

	uiCountAll = m_olsTestRequestTemp.GetCount();	// 测试请求数量
	pTestRequest = m_olsTestRequestTemp.GetHead();
	uiNbStart = pTestRequest->m_uiIndex;
	while(true)
	{
		for(unsigned int i = 0; i < uiCountAll; i++)
		{
			pos = m_olsTestRequestTemp.FindIndex(i);
			pTestRequest = m_olsTestRequestTemp.GetAt(pos);
			if((false == bStartFound) && (uiNbStart != pTestRequest->m_uiIndex))
			{
				continue;
			}
			else
			{
				bStartFound = true;
			}
			if((pTestRequest->m_uiLoopNb > 0) && (pTestRequest->m_uiLoopCount > 0))
			{
				uiNbStart = pTestRequest->m_uiLoopNb;
				pTestRequest->m_uiLoopCount--;
				bStartFound = false;
				bLoopContinue = true;
				break;
			}
			// 整理要求一次测试全部项目的测试请求，复合测试
			ReArrangeByTestType(pTestRequest);
			bLoopContinue = false;
		}
		if(false == bLoopContinue)
		{
			break;
		}
	}
}

/**
* 复制测试项目
* @param CTestProject* pTestProject 测试项目指针
* @return void
*/
void CTestProject::Clone(CTestProject* pTestProject)
{
	m_uiProjectType = pTestProject->m_uiProjectType;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	m_uiTestNb = pTestProject->m_uiTestNb;	// 测试项目索引号
	m_strAuxiliaryDescr = pTestProject->m_strAuxiliaryDescr;	// 辅助道定义
	m_strAbsoluteSpread = pTestProject->m_strAbsoluteSpread;	// 绝对排列定义
	m_uiDelayBetweenTest = pTestProject->m_uiDelayBetweenTest;	// 每次测试之间的间隔（毫秒）
	m_uiRecordResult = pTestProject->m_uiRecordResult;	// 是否记录测试数据  0-不记录；1-记录
	m_uiRecordLength = pTestProject->m_uiRecordLength;	// 测试记录时间（毫秒）
	m_uiTestFileNb = pTestProject->m_uiTestFileNb;	// 测试文件号
	m_uiCountAll = pTestProject->m_uiCountAll;	// 测试请求数量

	CTestRequest* pTestRequestNew = NULL;
	CTestRequest* pTestRequest = NULL;
	POSITION pos;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pos = pTestProject->m_olsTestRequest.FindIndex(i);
		pTestRequest = pTestProject->m_olsTestRequest.GetAt(pos);
		pTestRequestNew = new CTestRequest;
		*pTestRequestNew = *pTestRequest;
		m_olsTestRequest.AddTail(pTestRequestNew);
	}
}

/**
* 设置测试请求的测试目标 1-仪器；2-检波器
* @param CTestRequest* pTestRequest 测试请求指针
* @return void
*/
void CTestProject::SetTestRequestAim(CTestRequest* pTestRequest)
{
	if(NULL != m_pTestBaseListInstrument->GetTestBaseDataByTestType(pTestRequest->m_uiType))
	{
		pTestRequest->m_uiAim = 1;
		return;
	}
	if(NULL != m_pTestBaseListSensor->GetTestBaseDataByTestType(pTestRequest->m_uiType))
	{
		pTestRequest->m_uiAim = 2;
		return;
	}
}

/**
* 根据测试类型，得到测试测试基本设置指针
* @param unsigned int uiType 测试类型
* @return CTestBaseData* 测试基本设置指针 NULL：无
*/
CTestBaseData* CTestProject::GetTestBaseDataByTestType(unsigned int uiType)
{
	CTestBaseData* pTestBaseData = NULL;
	pTestBaseData = m_pTestBaseListInstrument->GetTestBaseDataByTestType(uiType);
	if(NULL != pTestBaseData)
	{
		return pTestBaseData;
	}
	pTestBaseData = m_pTestBaseListSensor->GetTestBaseDataByTestType(uiType);
	if(NULL != pTestBaseData)
	{
		return pTestBaseData;
	}
	return NULL;
}

/**
* 设置测试项目，来自测试视图
* @param unsigned int uiTestAim 测试对象	1-仪器；2-检波器；3-全部
* @param unsigned int uiTestType 测试类型
* @return void
*/
void CTestProject::SetupTestProjectFromView(unsigned int uiTestAim, unsigned int uiTestType)
{
	// 解析测试请求
	CTestRequest* pTestRequest = new CTestRequest;
	pTestRequest->OnReset();

	pTestRequest->m_uiIndex = 1;	// 索引号	
	pTestRequest->m_uiAim = uiTestAim;	// 测试对象	1-仪器；2-检波器；3-全部
	pTestRequest->m_uiType = uiTestType;	// 测试类型

	m_olsTestRequestTemp.AddTail(pTestRequest);	// 增加到临时队列
	// 整理要求一次测试全部项目的测试请求
	ReArrangeByTestType(pTestRequest);
	m_uiCountAll = m_olsTestRequest.GetCount();	// 测试请求数量
	delete pTestRequest;
	pTestRequest = NULL;
	m_olsTestRequestTemp.RemoveAll();

	POSITION pos;
	CTestBaseData* pTestBaseData = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pos = m_olsTestRequest.FindIndex(i);
		pTestRequest = m_olsTestRequest.GetAt(pos);
		if(1 == uiTestAim)	// 测试对象	1-仪器；2-检波器；3-全部
		{
			pTestBaseData = m_pTestBaseListInstrument->GetTestBaseDataByTestType(pTestRequest->m_uiType);
		}
		else if(2 == uiTestAim)	// 测试对象	1-仪器；2-检波器；3-全部
		{
			pTestBaseData = m_pTestBaseListSensor->GetTestBaseDataByTestType(pTestRequest->m_uiType);
		}		
		pTestRequest->m_uiGain = pTestBaseData->m_uiGain;
	}
}

/**
* 设置基本测试项目，处理新加检波器的初始测试
* @param UINT uiTestResistance 新仪器是否测试Resistance
* @param UINT uiTestTilt 新仪器是否测试Tilt
* @param UINT uiTestLeakage 新仪器是否测试Leakage
* @return void
*/
void CTestProject::SetupTestProjectForSensorTestOnField(UINT uiTestResistance, UINT uiTestTilt, UINT uiTestLeakage)
{
	CTestRequest* pTestRequestNew = NULL;
	if(1 == uiTestResistance)
	{
		pTestRequestNew = new CTestRequest;
		pTestRequestNew->m_uiIndex = 1;	// 索引号	
		pTestRequestNew->m_uiAim = 2;	// 测试对象	1-仪器；2-检波器；3-全部
		pTestRequestNew->m_uiType = 15;	// 测试类型
		m_olsTestRequest.AddTail(pTestRequestNew);
	}
	if(1 == uiTestTilt)
	{
		pTestRequestNew = new CTestRequest;
		pTestRequestNew->m_uiIndex = 1;	// 索引号	
		pTestRequestNew->m_uiAim = 2;	// 测试对象	1-仪器；2-检波器；3-全部
		pTestRequestNew->m_uiType = 20;	// 测试类型
		m_olsTestRequest.AddTail(pTestRequestNew);
	}
	if(1 == uiTestLeakage)
	{
		pTestRequestNew = new CTestRequest;
		pTestRequestNew->m_uiIndex = 1;	// 索引号	
		pTestRequestNew->m_uiAim = 2;	// 测试对象	1-仪器；2-检波器；3-全部
		pTestRequestNew->m_uiType = 14;	// 测试类型
		m_olsTestRequest.AddTail(pTestRequestNew);
	}
	m_uiCountAll = m_olsTestRequest.GetCount();	// 测试请求数量

	POSITION pos;
	CTestBaseData* pTestBaseData = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		pos = m_olsTestRequest.FindIndex(i);
		pTestRequestNew = m_olsTestRequest.GetAt(pos);
		pTestBaseData = m_pTestBaseListSensor->GetTestBaseDataByTestType(pTestRequestNew->m_uiType);		
		pTestRequestNew->m_uiGain = pTestBaseData->m_uiGain;
	}
}