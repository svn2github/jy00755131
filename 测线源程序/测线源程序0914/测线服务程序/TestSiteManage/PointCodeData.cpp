#include "StdAfx.h"
#include "PointCodeData.h"

CPointCodeData::CPointCodeData()
{
}

CPointCodeData::~CPointCodeData()
{
}

/**
* 解析XML
* @param CXMLDOMElement* pElement XML文档节点指针
* @return void
*/
void CPointCodeData::ParseXML(CXMLDOMElement* pElement)
{
	CString strKey;
	strKey = "Nb";
	m_uiNb = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
	strKey = "Label";
	m_strLabel = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
	strKey = "SensorType";
	m_strSensorType = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
}

/**
* 解析检波器类型
* @param CSensorList* pSensorList 检波器队列指针
* @return void
*/
void CPointCodeData::ParseSensorType(CSensorList* pSensorList)
{
	// 转换为小写
	m_strSensorType.MakeLower();
	// 跳过道 cs 用 p0 替换
	m_strSensorType.Replace(_T("cs"), _T("s0"));
	// 去掉标识符 s
	m_strSensorType.Replace(_T("s"), _T(""));

	CString strPart;	// 检波器Nb字符串
	int iSensorType;	// 检波器Nb
	CSensorData* pSensorData;	// 检波器指针
	POSITION pos;	// 位置
	int iPos, iPosStart, iPosStartEnd;	// 位置
	int iCount, i;
	CList<int, int> lsPos;	// 位置队列

	lsPos.AddTail(-1);
	iPosStart = 0;
	while(true)
	{
		// 查找分割标记"+"
		iPos = m_strSensorType.Find('+', iPosStart);
		if(-1 == iPos)
		{
			break;
		}
		lsPos.AddTail(iPos);
		iPosStart = iPos + 1;
	}
	lsPos.AddTail(m_strSensorType.GetAllocLength());

	// 得到标记点 "+" 数量
	iCount = lsPos.GetCount();
	for(i = 0; i < iCount - 1; i++)
	{
		pos = lsPos.FindIndex(i);
		iPosStart = lsPos.GetAt(pos) + 1;
		pos = lsPos.FindIndex(i + 1);
		iPosStartEnd = lsPos.GetAt(pos);
		// 得到检波器Nb字符串
		strPart = m_strSensorType.Mid(iPosStart, iPosStartEnd - iPosStart);
		// 得到检波器Nb 
		iSensorType = CXMLDOMTool::ConvertStringToInt(strPart);
		// 得到检波器队列指针
		pSensorData = pSensorList->GetSensor(iSensorType);
		if(NULL != pSensorData)
		{
			// 加入检波器队列尾部
			m_olsSensor.AddTail(pSensorData);
		}
	}
	// 点代码对应得检波器数量
	m_iSensorCount = m_olsSensor.GetCount();
}

/**
* 根据索引号，得到检波器
* @param int iIndex 队列顺序索引号
* @return CSensorData* 检波器指针 NULL：没有检波器
*/
CSensorData* CPointCodeData::GetSensorData(int iIndex)
{
	CSensorData* pSensorData = NULL;
	POSITION pos;

	pos = m_olsSensor.FindIndex(iIndex);
	if(NULL != pos)
	{
		pSensorData = m_olsSensor.GetAt(pos);
	}
	return pSensorData;
}