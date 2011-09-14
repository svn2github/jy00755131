#include "StdAfx.h"
#include "BlastMachineList.h"

CBlastMachineList::CBlastMachineList()
{
}

CBlastMachineList::~CBlastMachineList()
{
}

/**
* 初始化
* @param void
* @return void
*/
void CBlastMachineList::OnInit()
{
	// 解析配置文件
	ParseXML();
}

/**
* 重新加载
* @param void
* @return void
*/
void CBlastMachineList::OnReload()
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
void CBlastMachineList::OnClose()
{
	// 爆炸机控制器总数大于0
	if(m_uiCountAll > 0)
	{
		// 爆炸机控制器指针
		CBlastMachineData* pBlastMachine = NULL;
		while(FALSE == m_olsBlastMachine.IsEmpty())
		{
			// 移除队列
			pBlastMachine = m_olsBlastMachine.RemoveHead();
			// 删除爆炸机控制器
			delete pBlastMachine;
		}
	}
	m_uiCountAll = 0;
}

/**
* 解析配置文件
* @param void
* @return void
*/
void CBlastMachineList::ParseXML()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到爆炸机控制器设置区
	strKey = "BlastMachineSetup";
	lpDispatch = m_pXMLDOMDocument->getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);
	// 得到爆炸机控制器总数
	strKey = "Count";
	m_uiCountAll = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
	// 得到队列
	lpDispatch = oElement.get_childNodes();
	oNodeList.AttachDispatch(lpDispatch);

	CBlastMachineData* pBlastMachine = NULL;
	for(unsigned int i = 0; i < m_uiCountAll; i++)
	{
		lpDispatch = oNodeList.get_item(i);
		oElement.AttachDispatch(lpDispatch);
		pBlastMachine = new CBlastMachineData;
		pBlastMachine->ParseXML(&oElement);
		// 增加爆炸机控制器
		m_olsBlastMachine.AddTail(pBlastMachine);
	}
}

/**
* 根据顺序索引号，得到爆炸机控制器
* @param unsigned int uiIndex 爆炸机控制器索引号
* @return CBlastMachineData* 爆炸机控制器指针 NULL:空指针
*/
CBlastMachineData* CBlastMachineList::GetBlastMachine(unsigned int uiIndex)
{
	CBlastMachineData* pBlastMachine = NULL;
	POSITION pos = m_olsBlastMachine.FindIndex(uiIndex);
	if(NULL != pos)
	{
		pBlastMachine = m_olsBlastMachine.GetAt(pos);
	}	
	return pBlastMachine;
}
