#include "StdAfx.h"
#include "MarkerList.h"

CMarkerList::CMarkerList()
{
}

CMarkerList::~CMarkerList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CMarkerList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CMarkerList::OnReload()
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
void CMarkerList::OnClose()
{
	// 标记点总数大于0
	if(m_uiCountAll > 0)
	{
		// 标记点指针
		CMarkerData* pMarker;
		while(FALSE == m_olsMarker.IsEmpty())
		{
			// 移除队列
			pMarker = m_olsMarker.RemoveHead();
			// 删除标记点
			delete pMarker;
		}
	}
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CMarkerList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到标记点设置区
	strKey = "MarkerSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到标记点总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CMarkerData* pMarker = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pMarker = new CMarkerData;
		pMarker->ParseXML(&oElement);
		// 增加标记点
		m_olsMarker.AddTail(pMarker);
	}
}

/**
* 根据顺序索引号，得到标记点
* @param unsigned int uiIndex 标记点索引号
* @return CMarkerData* 标记点指针 NULL:空指针
*/
CMarkerData* CMarkerList::GetMarker(unsigned int uiIndex)
{
	POSITION pos = m_olsMarker.FindIndex(uiIndex);
	CMarkerData* pMarker = m_olsMarker.GetAt(pos);
	return pMarker;
}