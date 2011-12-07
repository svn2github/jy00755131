#include "StdAfx.h"
#include "TestSetupData.h"

CTestSetupData::CTestSetupData()
{
}

CTestSetupData::~CTestSetupData()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CTestSetupData::OnInit()
{
	// 仪器测试基本参数队列
	m_oTestBaseListInstrument.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oTestBaseListInstrument.m_uiAim = 1;	// 测试对象	1-仪器；2-检波器
	m_oTestBaseListInstrument.OnInit();
	// 检波器测试基本参数队列
	m_oTestBaseListSensor.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oTestBaseListSensor.m_uiAim = 2;	// 测试对象	1-仪器；2-检波器
	m_oTestBaseListSensor.OnInit();

	// 仪器测试判据参数队列
	m_oTestLimitListInstrument.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oTestLimitListInstrument.m_uiAim = 1;	// 测试对象	1-仪器；2-检波器
	m_oTestLimitListInstrument.OnInit();
	// 检波器测试判据参数队列
	m_oTestLimitListSensor.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oTestLimitListSensor.m_uiAim = 2;	// 测试对象	1-仪器；2-检波器
	m_oTestLimitListSensor.OnInit();

	// 测试项目队列对象
	m_oTestProjectList.m_pXMLDOMDocument = m_pXMLDOMDocument;	// XMLDOM文件对象
	m_oTestProjectList.m_pTestBaseListInstrument = &m_oTestBaseListInstrument;	// 仪器测试基本参数队列指针
	m_oTestProjectList.m_pTestBaseListSensor = &m_oTestBaseListSensor;	// 检波器测试基本参数队列指针
	m_oTestProjectList.OnInit();
	
	// 测试单元队列，基本测试
	m_oTestElementListBase.m_uiCountAll = m_uiTestElementCountAll;	// 测试单元数组总数
	m_oTestElementListBase.OnInit();
	// 测试单元队列，噪声监测
	m_oTestElementListNoise.m_uiCountAll = m_uiTestElementCountAll;	// 测试单元数组总数
	m_oTestElementListNoise.OnInit();
	// 测试数据缓冲区，基本测试
	m_oTestDataBufferBase.m_uiTestElementCountAll = m_uiTestElementCountAll;	// 测试单元数组总数
	m_oTestDataBufferBase.OnInit();
	// 测试数据缓冲区，噪声监测
	m_oTestDataBufferNoise.m_uiTestElementCountAll = m_uiTestElementCountAll;	// 测试单元数组总数
	m_oTestDataBufferNoise.OnInit();

	// 测试项目队列对象，基本测试
	m_oTestProjectBase.m_pTestBaseListInstrument = &m_oTestBaseListInstrument;
	m_oTestProjectBase.m_pTestBaseListSensor = &m_oTestBaseListSensor;
	// 测试项目队列对象，噪声监测
	m_oTestProjectNoise.m_pTestBaseListInstrument = &m_oTestBaseListInstrument;
	m_oTestProjectNoise.m_pTestBaseListSensor = &m_oTestBaseListSensor;
}	

/**
* 重置
* @param void
* @return void
*/
void CTestSetupData::OnReset()
{
	// 测试项目队列对象，噪声监测
	m_oTestProjectNoise.OnReset();
}

/**
* 重新加载
* @param void
* @return void
*/
void CTestSetupData::OnReload()
{
	// 仪器测试基本参数队列
	m_oTestBaseListInstrument.OnReload();
	// 检波器测试基本参数队列
	m_oTestBaseListSensor.OnReload();

	// 仪器测试判据参数队列
	m_oTestLimitListInstrument.OnReload();
	// 检波器测试判据参数队列
	m_oTestLimitListSensor.OnReload();

	// 测试项目队列对象
	m_oTestProjectList.OnReload();
	// 测试单元队列，基本测试
	m_oTestElementListBase.OnReset();
	// 测试单元队列，噪声监测
	m_oTestElementListNoise.OnReset();
	// 测试数据缓冲区，基本测试
	m_oTestDataBufferBase.OnReset();
	// 测试数据缓冲区，噪声监测
	m_oTestDataBufferNoise.OnReset();
}

/**
* 关闭
* @param void
* @return void
*/
void CTestSetupData::OnClose()
{
	// 仪器测试基本参数队列
	m_oTestBaseListInstrument.OnClose();
	// 检波器测试基本参数队列
	m_oTestBaseListSensor.OnClose();

	// 仪器测试判据参数队列
	m_oTestLimitListInstrument.OnClose();
	// 检波器测试判据参数队列
	m_oTestLimitListSensor.OnClose();

	// 测试项目队列对象
	m_oTestProjectList.OnClose();
	// 测试单元队列，基本测试
	m_oTestElementListBase.OnClose();
	// 测试单元队列，噪声监测
	m_oTestElementListNoise.OnClose();
	// 测试数据缓冲区，基本测试
	m_oTestDataBufferBase.OnClose();
	// 测试数据缓冲区，噪声监测
	m_oTestDataBufferNoise.OnClose();
}

/**
* 设置基本测试项目，来自测试设置的测试任务
* @param unsigned int uiTestNb 测试号
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestProjectForTestBaseFromTestSetup(unsigned int uiTestNb)
{
	// 测试项目不存在
	if(FALSE == m_oTestProjectList.IfIndexExistInMap(uiTestNb))
	{
		return false;
	}
	// 得到测试项目
	CTestProject* pTestProject = NULL;
	m_oTestProjectList.GetTestProject(uiTestNb, pTestProject);
	// 复制测试项目
	m_oTestProjectBase.OnReset();	// 测试项目队列对象，基本测试
	m_oTestProjectBase.Clone(pTestProject);

	return true;
}

/**
* 设置基本测试项目，来自测试图形的测试任务
* @param unsigned int uiTestAim 测试项目类型 1-仪器测试；2-检波器测试
* @param unsigned int uiTestType 测试类型
* @param unsigned CString strTestAbsoluteSpread 绝对排列
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestProjectForTestBaseFromTestView(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread)
{
	// 复制测试项目
	m_oTestProjectBase.OnReset();	// 测试项目队列对象，基本测试
	m_oTestProjectBase.m_uiProjectType = uiTestAim;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	if(1 == uiTestAim)
	{
		m_oTestProjectBase.m_uiTestNb = 1000;
	}
	else if(2 == uiTestAim)
	{
		m_oTestProjectBase.m_uiTestNb = 2000;
	}	
	m_oTestProjectBase.m_uiRecordResult = 0;	// 是否记录测试数据  0-不记录；1-记录
	m_oTestProjectBase.m_uiRecordLength  = 0;	// 测试记录时间（毫秒）
	m_oTestProjectBase.m_strAuxiliaryDescr = "";	// 辅助道定义
	m_oTestProjectBase.m_strAbsoluteSpread = strTestAbsoluteSpread;	// 绝对排列定义
	m_oTestProjectBase.m_uiDelayBetweenTest = 0;	// 每次测试之间的间隔（毫秒）

	// 设置测试项目，来自测试视图
	m_oTestProjectBase.SetupTestProjectFromView(uiTestAim, uiTestType);

	return true;
}

/**
* 设置基本测试项目，处理新加检波器的初始测试
* @param UINT uiTestResistance 新仪器是否测试Resistance
* @param UINT uiTestTilt 新仪器是否测试Tilt
* @param UINT uiTestLeakage 新仪器是否测试Leakage
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestProjectForSensorTestOnField(UINT uiTestResistance, UINT uiTestTilt, UINT uiTestLeakage)
{
	// 复制测试项目
	m_oTestProjectBase.OnReset();	// 测试项目队列对象，基本测试
	m_oTestProjectBase.m_uiProjectType = 2;	// 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
	m_oTestProjectBase.m_uiTestNb = 2000;	
	m_oTestProjectBase.m_uiRecordResult = 0;	// 是否记录测试数据  0-不记录；1-记录
	m_oTestProjectBase.m_uiRecordLength  = 0;	// 测试记录时间（毫秒）
	m_oTestProjectBase.m_strAuxiliaryDescr = "";	// 辅助道定义
	m_oTestProjectBase.m_strAbsoluteSpread = "";	// 绝对排列定义
	m_oTestProjectBase.m_uiDelayBetweenTest = 1000;	// 每次测试之间的间隔（毫秒）

	// 设置测试项目，处理新加检波器的初始测试
	m_oTestProjectBase.SetupTestProjectForSensorTestOnField(uiTestResistance, uiTestTilt, uiTestLeakage);

	return true;
}

/**
* 设置噪声监测项目
* @param void
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestProjectForTestNoise()
{
	// 测试项目不存在
	if(FALSE == m_oTestProjectList.IfIndexExistInMap(4000))
	{
		return false;
	}
	// 得到测试项目
	CTestProject* pTestProject = NULL;
	m_oTestProjectList.GetTestProject(4000, pTestProject);
	// 复制测试项目
	m_oTestProjectNoise.OnReset();	// 测试项目队列对象
	m_oTestProjectNoise.Clone(pTestProject);

	return true;
}

/**
* 设置测试单元，基本测试
* @param void
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestElementBase()
{
	m_oTestElementListBase.OnReset();

	int iIPCount;	// IP地址数量
	unsigned int uiIP;	// IP地址
	int iIndex = 0;
	iIPCount = m_oTestProjectBase.m_olsIP.GetCount();
	m_oTestElementListBase.m_uiCountUsed = iIPCount;
	while(false == m_oTestProjectBase.m_olsIP.IsEmpty())
	{
		uiIP = m_oTestProjectBase.m_olsIP.RemoveHead();
		m_oTestElementListBase.m_pArrayTestElementData[iIndex].m_uiIP = uiIP;
		m_oTestElementListBase.m_pArrayTestElementData[iIndex].m_bInUsed = true;
		// 增加一个单元到索引表
		m_oTestElementListBase.AddTestElementDataToMap(uiIP, &m_oTestElementListBase.m_pArrayTestElementData[iIndex]);
		iIndex++;
	}
	return true;
}

/**
* 设置测试单元，噪声监测
* @param void
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestElementNoise()
{
	m_oTestElementListNoise.OnReset();

	int iIPCount;	// IP地址数量
	unsigned int uiIP;	// IP地址
	int iIndex = 0;
	iIPCount = m_oTestProjectNoise.m_olsIP.GetCount();
	m_oTestElementListNoise.m_uiCountUsed = iIPCount;
	while(false == m_oTestProjectNoise.m_olsIP.IsEmpty())
	{
		uiIP = m_oTestProjectNoise.m_olsIP.RemoveHead();
		m_oTestElementListNoise.m_pArrayTestElementData[iIndex].m_uiIP = uiIP;
		m_oTestElementListNoise.m_pArrayTestElementData[iIndex].m_bInUsed = true;
		// 增加一个单元到索引表
		m_oTestElementListNoise.AddTestElementDataToMap(uiIP, &m_oTestElementListNoise.m_pArrayTestElementData[iIndex]);
		iIndex++;
	}
	return true;
}

/**
* 设置测试数据缓冲区，基本测试
* @param void
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestDataBufferBase()
{
	m_oTestDataBufferBase.OnReset();
	// 测试设置
	m_oTestDataBufferBase.OnTestSetup(m_oTestElementListBase.m_uiCountUsed);
	return true;
}

/**
* 设置测试数据缓冲区，噪声监测
* @param void
* @return bool true：成功；false：失败
*/
bool CTestSetupData::OnSetupTestDataBufferNoise()
{
	m_oTestDataBufferNoise.OnReset();
	// 测试设置
	m_oTestDataBufferNoise.OnTestSetup(m_oTestElementListNoise.m_uiCountUsed);
	return true;
}