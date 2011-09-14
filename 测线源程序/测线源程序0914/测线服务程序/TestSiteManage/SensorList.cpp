#include "StdAfx.h"
#include "SensorList.h"

CSensorList::CSensorList()
{
}

CSensorList::~CSensorList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CSensorList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CSensorList::OnReload()
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
void CSensorList::OnClose()
{
	// 检波器总数大于0
	if(m_uiCountAll > 0)
	{
		// 检波器指针
		CSensorData* pSensor;
		while(FALSE == m_olsSensor.IsEmpty())
		{
			// 移除队列
			pSensor = m_olsSensor.RemoveHead();
			// 删除检波器
			delete pSensor;
		}
		// 清空检波器索引表
		m_oSensorMap.RemoveAll();
	}
	m_uiCountAll = 0;
}

/**
* 处理配置文件更改	SurveySetup的ApplySensor
* @param void
* @return void
*/
void CSensorList::OnSetupDataChangeForSurveySetupApplySensor()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到检波器设置区
	strKey = "SensorSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到检波器总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CSensorData oSensor;
	CSensorData* pSensor = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		oSensor.ParseXML(&oElement);

		pSensor = NULL;
		pSensor = GetSensor(oSensor.m_uiNb);
		if(NULL != pSensor)
		{
			*pSensor = oSensor;
		}
	}
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CSensorList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到检波器设置区
	strKey = "SensorSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到检波器总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CSensorData* pSensor = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pSensor = new CSensorData;
		pSensor->ParseXML(&oElement);
		// 增加检波器
		m_olsSensor.AddTail(pSensor);
		// 加入检波器索引表
		m_oSensorMap.SetAt(pSensor->m_uiNb, pSensor);
	}

	// 添加一个为跳过道准备的空检波器
	m_uiCountAll++;	// 检波器总数加1
	pSensor = new CSensorData;
	pSensor->m_uiSegdCode = 0;
	pSensor->m_strLabel = "Jumped";
	pSensor->m_fContinuityMin = 0.0;
	pSensor->m_fContinuityMax = 0.0;
	pSensor->m_fTilt = 0.0;
	pSensor->m_fNoise = 0.0;
	pSensor->m_fLeaKage = 0.0;
	pSensor->m_uiNb = 0;
	// 增加检波器
	m_olsSensor.AddTail(pSensor);
	// 加入检波器索引表
	m_oSensorMap.SetAt(pSensor->m_uiNb, pSensor);
}

/**
* 根据索引号，得到检波器指针
* @param unsigned int uiNb 检波器号
* @return CSensorData* 检波器指针 NULL:空指针
*/
CSensorData* CSensorList::GetSensor(unsigned int uiNb)
{
	CSensorData* pSensor = NULL;
	// 查找检波器索引表
	if(TRUE == m_oSensorMap.Lookup(uiNb, pSensor))
	{
		return pSensor;
	}
	return NULL;
}