#include "StdAfx.h"
#include "DetourList.h"

CDetourList::CDetourList()
{
}

CDetourList::~CDetourList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CDetourList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CDetourList::OnReload()
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
void CDetourList::OnClose()
{
	// 判断：迂回道总数大于0
	if(m_uiCountAll > 0)
	{
		// 迂回道指针
		CDetourData* pDetour = NULL;
		while(FALSE == m_olsDetour.IsEmpty())
		{
			// 移除队列
			pDetour = m_olsDetour.RemoveHead();
			// 删除迂回道
			delete pDetour;
		}
	}
	m_uiCountAll = 0;	// 数量
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CDetourList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到迂回道设置区
	strKey = "DetourSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到迂回道总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CDetourData* pDetour = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);

		pDetour = new CDetourData;
		pDetour->ParseXML(&oElement);

		// 增加迂回道
		m_olsDetour.AddTail(pDetour);
	}
}

/**
* 根据顺序索引号，得到迂回道
* @param unsigned int uiIndex 迂回道索引号
* @return CDetourData* 迂回道指针 NULL:空指针
*/
CDetourData* CDetourList::GetDetour(unsigned int uiIndex)
{
	CDetourData* pDetour = NULL;
	POSITION pos = m_olsDetour.FindIndex(uiIndex);
	if(NULL != pos)
	{
		pDetour = m_olsDetour.GetAt(pos);
	}	
	return pDetour;
}