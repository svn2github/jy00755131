#include "StdAfx.h"
#include "AdminParseXML.h"

CAdminParseXML::CAdminParseXML(void)
{
}

CAdminParseXML::~CAdminParseXML(void)
{

}
/**
 * @brief 解析配置程序中用户信息
 * @note  解析配置程序中用户信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CUserInfo*  pUserInfo, 用户对象
 * @return 解析是否成功
 */
bool  CAdminParseXML::ParseUserInfo(CXMLDOMElement* pElement,CUserInfo*  pUserInfo)
{	
	try
	{	
		CString strKey,strTemp;	// 键名	
		if(NULL==pUserInfo)
			return false;
		// 用户名
		strKey =_T("UserName");
		pUserInfo->m_strUserName = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		// 密码 
		strKey =_T("Password");
		pUserInfo->m_strPassword= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		// 角色
		strKey =_T("UserRole");
		strTemp = CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		if(strTemp.CompareNoCase(_T("Guest"))==0)
		{
			pUserInfo->m_nRole = USERINFO_ROLE_GUEST;
		}
		else if(strTemp.CompareNoCase(_T("Observer"))==0)
		{
			pUserInfo->m_nRole = USERINFO_ROLE_OBSERVER;
		}
		else if(strTemp.CompareNoCase(_T("Senior Observer"))==0)
		{
			pUserInfo->m_nRole = USERINFO_ROLE_SENIOROBSERVER;
		}
		else
		{
			pUserInfo->m_nRole = USERINFO_ROLE_GUEST;
		}
		// 注册日期
		strKey =_T("RegistrationDate");
		strTemp= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strTemp.Replace(':',' ');
		strTemp.Replace('-',' ');
		_stscanf_s((LPCTSTR)strTemp,_T("%d %d %d %d %d %d"),&pUserInfo->m_tmRegister.wYear,
			&pUserInfo->m_tmRegister.wMonth,&pUserInfo->m_tmRegister.wDay,&pUserInfo->m_tmRegister.wHour,
			&pUserInfo->m_tmRegister.wMinute,&pUserInfo->m_tmRegister.wSecond);
		// 过期后处理方式
		strKey =_T("ExpiredMode");
		pUserInfo->m_nExpiredMode = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
		// 失效日期
		strKey =_T("ExpiryDate");
		strTemp= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strTemp.Replace(':',' ');
		strTemp.Replace('-',' ');
		_stscanf_s((LPCTSTR)strTemp,_T("%d %d %d %d %d %d"),&pUserInfo->m_tmExpiryDate.wYear,
			&pUserInfo->m_tmExpiryDate.wMonth,&pUserInfo->m_tmExpiryDate.wDay,&pUserInfo->m_tmExpiryDate.wHour,
			&pUserInfo->m_tmExpiryDate.wMinute,&pUserInfo->m_tmExpiryDate.wSecond);
		// 是否自动删除
		strKey =_T("AutoDelete");
		pUserInfo->m_bAutoDelete = CXMLDOMTool::GetElementAttributeInt(pElement, strKey);
		// 登录日期
		strKey =_T("LastUsed");
		strTemp= CXMLDOMTool::GetElementAttributeString(pElement, strKey);
		strTemp.Replace(':',' ');
		strTemp.Replace('-',' ');
		_stscanf_s((LPCTSTR)strTemp,_T("%d %d %d %d %d %d"),&pUserInfo->m_tmLastUsed.wYear,
			&pUserInfo->m_tmLastUsed.wMonth,&pUserInfo->m_tmLastUsed.wDay,&pUserInfo->m_tmLastUsed.wHour,
			&pUserInfo->m_tmLastUsed.wMinute,&pUserInfo->m_tmLastUsed.wSecond);
		return true;
	}
	catch (CException* )
	{
		return false;
	}
}
bool  CAdminParseXML::ParseHostConfig(LPCTSTR pstrXMLFilePath,CString& strIP)
{
	try
	{
		CString strOLEObject;
		COleException oError;
		COleVariant oVariant;

		CXMLDOMDocument oXMLDOMDocument;
		CString strKey;
		CXMLDOMNodeList oNodeList;
		CXMLDOMElement oElement;
		LPDISPATCH lpDispatch;

		//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// --------------------------------
		// 找到AdminServer
		strKey =_T("AdminServer");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到IP
		strKey =_T("IP");
		strIP = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
		
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
}
/**
 * @brief 解析用户账号属性
 * @note  从XML的配置文件中读取出所有用户的账号信息，放入用户对象集合CUserInfos。
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径
 * @param CUserInfos*  pUserInfos，用户对象集合
 * @return bool，成功则返回true，失败返回false
 */
bool  CAdminParseXML::ParseAllUserInfo(LPCTSTR pstrXMLFilePath,CUserInfos*  pUserInfos)
{
	try
	{
		CString strOLEObject;
		COleException oError;
		COleVariant oVariant;

		CXMLDOMDocument oXMLDOMDocument;
		CString strKey;
		CXMLDOMNodeList oNodeList;
		CXMLDOMElement oElement;
		LPDISPATCH lpDispatch;
		ULONG		dwCount;

		//CoInitialize(NULL);
		// 创建XML对象
		strOLEObject =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);
	
		// -------------------------------------
		// 找到Users
		strKey =_T("Users");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElement.AttachDispatch(lpDispatch);
		// 得到总数
		strKey =_T("Count");
		dwCount = CXMLDOMTool::GetElementAttributeInt(&oElement, strKey);
		// 得到队列
		lpDispatch = oElement.get_childNodes();
		oNodeList.AttachDispatch(lpDispatch);

		CUserInfo* pUserInfo = NULL;
		for(unsigned int i = 0; i < dwCount; i++)
		{
			pUserInfo = new CUserInfo;
			// 得到XML元素
			lpDispatch = oNodeList.get_item(i);
			oElement.AttachDispatch(lpDispatch);
			// 得到User
			ParseUserInfo(&oElement, pUserInfo);		
			// 加入放炮表
			pUserInfos->Add(pUserInfo);		
		}
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
	
}
/**
 * @brief 保存一条用户信息
 * @note  向XML配置文件中写入一条用户信息
 * @param CXMLDOMElement* pElement，XML指针
 * @param CUserInfo*  pUserInfo, 所有的用户信息参数
 * @return 写入是否成功
 */
bool  CAdminParseXML::WriteUserInfo(CXMLDOMElement* pElement,CUserInfo*  pUserInfo)
{
	CString strKey,strTemp;	
	COleVariant   oVariant;
	VARIANT   vt;
	if(NULL==pUserInfo)
		return 0;
	try
	{// 用户名	
		strKey = _T("UserName");
		oVariant = pUserInfo->m_strUserName;
		pElement->setAttribute(strKey, oVariant);
		// 密码
		strKey = _T("Password");	
		oVariant = pUserInfo->m_strPassword;	
		pElement->setAttribute(strKey, oVariant);
		// 用户角色
		switch(pUserInfo->m_nRole)	
		{
		case USERINFO_ROLE_GUEST:		// guest
			oVariant = _T("Guest");
			break;
		case USERINFO_ROLE_OBSERVER:	// Observer
			oVariant = _T("Observer");
			break;
		case USERINFO_ROLE_SENIOROBSERVER:// Senior Observer
			oVariant = _T("Senior Observer");
			break;
		default:
			oVariant = _T("Guest");
			break;
		}
		// 角色
		strKey = _T("UserRole");
// 		vt.vt = VT_UI4;
// 		vt.ulVal = pUserInfo->m_nRole;
// 		oVariant = vt;
		pElement->setAttribute(strKey, oVariant);
		// 注册日期
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),pUserInfo->m_tmRegister.wYear,pUserInfo->m_tmRegister.wMonth
			,pUserInfo->m_tmRegister.wDay,pUserInfo->m_tmRegister.wHour,
			pUserInfo->m_tmRegister.wMinute	,pUserInfo->m_tmRegister.wSecond);
		strKey = _T("RegistrationDate");
		oVariant =strTemp;
		pElement->setAttribute(strKey, oVariant);
		// 失效模式
		strKey = _T("ExpiredMode");
		vt.vt = VT_UI4;
		vt.ulVal = pUserInfo->m_nExpiredMode;
		oVariant = vt;
		pElement->setAttribute(strKey, oVariant);
		// 失效日期
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),pUserInfo->m_tmExpiryDate.wYear,pUserInfo->m_tmExpiryDate.wMonth
			,pUserInfo->m_tmExpiryDate.wDay,pUserInfo->m_tmExpiryDate.wHour,
			pUserInfo->m_tmExpiryDate.wMinute	,pUserInfo->m_tmExpiryDate.wSecond);
		strKey = _T("ExpiryDate");
		oVariant =strTemp;
		pElement->setAttribute(strKey, oVariant);
		// AutoDelete
		strKey = _T("AutoDelete");
		vt.vt = VT_UI4;
		vt.ulVal = pUserInfo->m_bAutoDelete;
		oVariant = vt;
		pElement->setAttribute(strKey, oVariant);
		// 最后登录日期
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),pUserInfo->m_tmLastUsed.wYear,pUserInfo->m_tmLastUsed.wMonth
			,pUserInfo->m_tmLastUsed.wDay,pUserInfo->m_tmLastUsed.wHour,
			pUserInfo->m_tmLastUsed.wMinute	,pUserInfo->m_tmLastUsed.wSecond);
		strKey = _T("LastUsed");
		oVariant =strTemp;
		pElement->setAttribute(strKey, oVariant);
		return true;
	}
	catch (CException* )
	{
		return false;
	}
	
}
/**
 * @brief 保存所有的用户信息
 * @note  向XML配置文件中写入用户信息
 * @param LPCTSTR pstrXMLFilePath，XML配置文件路径 
 * @param CUserInfos*  pUserInfos, 所有的用户信息参数
 * @return 写入是否成功
 */
bool  CAdminParseXML::WriteAllUserInfo(LPCTSTR pstrXMLFilePath,CUserInfos*  pUserInfos)
{
	COleException	oError;
	COleVariant		oVariant;

	CXMLDOMDocument oXMLDOMDocument;
	CString			strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement	oElementParent,oElementChild;
	LPDISPATCH		lpDispatch;
	long			i,lCount;

	try
	{		
		// 创建XML对象
		strKey =_T("msxml2.domdocument");
		BOOL bData = oXMLDOMDocument.CreateDispatch(strKey, &oError);
		oXMLDOMDocument.put_preserveWhiteSpace(TRUE);
		oVariant = pstrXMLFilePath;
		bData = oXMLDOMDocument.load(oVariant);

		// 找到Users区域
		strKey =_T("Users");
		lpDispatch = oXMLDOMDocument.getElementsByTagName(strKey);
		oNodeList.AttachDispatch(lpDispatch);
		// 找到入口
		lpDispatch = oNodeList.get_item(0);
		oElementParent.AttachDispatch(lpDispatch);

		// 设置点代码总数
		strKey = _T("Count");
		lCount = (long)(pUserInfos->GetCount());
		oVariant = lCount;
		oElementParent.setAttribute(strKey, oVariant);

		// 删除所有子节点
		while(TRUE == oElementParent.hasChildNodes())
		{
			lpDispatch = oElementParent.get_firstChild();
			oElementParent.removeChild(lpDispatch);
		}
		// 增加新节点
		CUserInfo *pUserInfo =NULL;
		for( i = 0; i < lCount; i++)
		{
			pUserInfo = pUserInfos->GetUserInfo(i);

			lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t\t"));
			oElementParent.appendChild(lpDispatch);

			lpDispatch = oXMLDOMDocument.createElement(_T("Record"));
			oElementChild.AttachDispatch(lpDispatch);		
			//保存单条记录
			WriteUserInfo(&oElementChild,pUserInfo);
			oElementParent.appendChild(lpDispatch);

		}
		lpDispatch = oXMLDOMDocument.createTextNode(_T("\r\n\t\t"));
		oElementParent.appendChild(lpDispatch);

		oVariant = pstrXMLFilePath;
		oXMLDOMDocument.save(oVariant);
		// 释放对象
		oXMLDOMDocument.DetachDispatch();
		return true;
	}	
	catch (CException* )
	{
		return false;
	}
}
