#include "StdAfx.h"
#include "MuteList.h"

CMuteList::CMuteList()
{
}

CMuteList::~CMuteList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CMuteList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CMuteList::OnReload()
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
void CMuteList::OnClose()
{
	// 哑道总数大于0
	if(m_uiCountAll > 0)
	{
		// 哑道指针
		CMuteData* pMute;
		while(FALSE == m_olsMute.IsEmpty())
		{
			// 移除队列
			pMute = m_olsMute.RemoveHead();
			// 删除哑道
			delete pMute;
		}
	}
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CMuteList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到哑道设置区
	strKey = "MuteSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到哑道总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CMuteData* pMute = NULL;

	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);

		pMute = new CMuteData;
		pMute->ParseXML(&oElement);

		// 增加哑道
		m_olsMute.AddTail(pMute);
	}
}

/**
* 根据顺序索引号，得到哑点
* @param unsigned int uiIndex 哑点索引号
* @return CMuteData* 哑点指针 NULL:空指针
*/
CMuteData* CMuteList::GetMute(unsigned int uiIndex)
{
	POSITION pos = m_olsMute.FindIndex(uiIndex);
	CMuteData* pMute = m_olsMute.GetAt(pos);
	return pMute;
}
