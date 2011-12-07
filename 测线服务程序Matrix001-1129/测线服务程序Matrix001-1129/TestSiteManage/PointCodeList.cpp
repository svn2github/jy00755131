#include "StdAfx.h"
#include "PointCodeList.h"

CPointCodeList::CPointCodeList()
{
}

CPointCodeList::~CPointCodeList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CPointCodeList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CPointCodeList::OnReload()
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
void CPointCodeList::OnClose()
{
	// 点代码总数大于0
	if(m_uiCountAll > 0)
	{
		// 点代码指针
		CPointCodeData* pPointCode;
		while(FALSE == m_olsPointCode.IsEmpty())
		{
			// 移除队列
			pPointCode = m_olsPointCode.RemoveHead();
			// 删除点代码
			delete pPointCode;
		}
		// 清空点代码索引表
		m_oPointCodeMap.RemoveAll();
	}
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CPointCodeList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到点代码设置区
	strKey = "PointCodeSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到点代码总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CPointCodeData* pPointCode = NULL;

	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);

		pPointCode = new CPointCodeData;
		pPointCode->ParseXML(&oElement);
		pPointCode->ParseSensorType(m_pSensorList);

		// 增加点代码
		m_olsPointCode.AddTail(pPointCode);
		// 加入点代码索引表
		m_oPointCodeMap.SetAt(pPointCode->m_uiNb, pPointCode);
	}
}

/**
* 根据标识号，得到检波器
* @param unsigned int uiNb 标识号
* @return CSensorData* 检波器指针 NULL：没有检波器
*/
CPointCodeData* CPointCodeList::GetPointCode(unsigned int uiNb)
{
	CPointCodeData* pPointCode = NULL;
	// 查找点代码索引表
	if(TRUE == m_oPointCodeMap.Lookup(uiNb, pPointCode))
	{
		return pPointCode;
	}
	return NULL;
}