#include "StdAfx.h"
#include "AuxList.h"

CAuxList::CAuxList()
{
}

CAuxList::~CAuxList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CAuxList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CAuxList::OnReload()
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
void CAuxList::OnClose()
{
	// 判断：辅助道总数大于0
	if(m_uiCountAll > 0)
	{
		// 辅助道指针
		CAuxData* pAux = NULL;
		while(FALSE == m_olsAux.IsEmpty())
		{
			// 移除队列
			pAux = m_olsAux.RemoveHead();
			// 删除辅助道
			delete pAux;
		}
	}
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CAuxList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到辅助道设置区
	strKey = "AuxSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到辅助道总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CAuxData* pAux = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pAux = new CAuxData;
		pAux->ParseXML(&oElement);
		// 增加辅助道
		m_olsAux.AddTail(pAux);
	}
}

/**
* 根据顺序索引号，得到辅助道
* @param unsigned int uiIndex 辅助道索引号
* @return CAuxData* 辅助道指针 NULL:空指针
*/
CAuxData* CAuxList::GetAux(unsigned int uiIndex)
{
	CAuxData* pAux = NULL;
	POSITION pos = m_olsAux.FindIndex(uiIndex);
	if(NULL != pos)
	{
		pAux = m_olsAux.GetAt(pos);
	}	
	return pAux;
}

/**
* 根据标识号，得到辅助道
* @param unsigned int uiNb 辅助道标识号
* @return CAuxData* 辅助道指针 NULL:空指针
*/
CAuxData* CAuxList::GetAuxByNb(unsigned int uiNb)
{
	int iCount = m_olsAux.GetCount();
	POSITION pos = NULL;
	CAuxData* pAux = NULL;
	for(int i = 0; i < iCount; i++)
	{
		pos = m_olsAux.FindIndex(i);
		pAux = m_olsAux.GetAt(pos);
		if(pAux->m_uiNb == uiNb)
		{
			return pAux;
		}
	}
	return NULL;
}