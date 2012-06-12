#include "StdAfx.h"
#include "LineList.h"

CLineList::CLineList(void)
{
}

CLineList::~CLineList(void)
{
}

/**
* 初始化
* @param void
* @return void
*/
void CLineList::OnInit()
{	
	m_uiCountAll = 0;	// 使用中测线数量
	ParseXML();	// 解析配置文件
}

/**
* 重新加载
* @param void
* @return void
*/
void CLineList::OnReload()
{	
	m_uiCountAll = 0;	// 测线数量	
	m_olsLine.RemoveAll();	// 清空队列
	ParseXML();	// 解析配置文件
}

/**
* 关闭
* @param void
* @return void
*/
void CLineList::OnClose()
{
	m_uiCountAll = 0;	// 使用中测线数量	
	m_olsLine.RemoveAll();	// 清空队列
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CLineList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;
	unsigned int i;

	// 找到测线设置区
	strKey = "SurverySetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到测线总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到Tab键数量
	strKey = "TabCount";
	m_uiTabCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);

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
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CLineSetupData oLineSetupData;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		// 得到XML元素
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);

		oLineSetupData.ParseXML(&oElement);
		// 加入测线队列
		m_olsLine.AddTail(oLineSetupData);
	}
}

/**
* 保存配置文件
* @param void
* @return void
*/
void CLineList::OnSave()
{
	CString strKey;
	COleVariant oVariant;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElementParent, oElementChild;
	LPDISPATCH lpDispatch;

	// 找到测线设置区
	strKey = "SurverySetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElementParent.AttachDispatch(lpDispatch);
	// 设置总数
	strKey = "Count";
	oVariant = (long)m_uiCountAll;
	oElementParent.setAttribute(strKey, oVariant);

	// 删除所有子节点
	while(TRUE == oElementParent.hasChildNodes())
	{
		lpDispatch = oElementParent.get_firstChild();
		oElementParent.removeChild(lpDispatch);
	}
	// 增加新节点
	CLineSetupData oLineSetupData;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		oLineSetupData = m_olsLine.RemoveHead();

		lpDispatch = m_pXMLDOMDocument->createTextNode(m_strTabChild);
		oElementParent.appendChild(lpDispatch);

		lpDispatch = m_pXMLDOMDocument->createElement("Record");
		oElementChild.AttachDispatch(lpDispatch);
		oLineSetupData.AddToXML(&oElementChild);
		oElementParent.appendChild(lpDispatch);
		
		if(i == (m_uiCountAll - 1))
		{
			lpDispatch = m_pXMLDOMDocument->createTextNode(m_strTabParent);
			oElementParent.appendChild(lpDispatch);
		}		
	}
}

