// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

//#import "E:\\Test\\TestSiteManage\\msxml6.dll" no_namespace
// CXMLDOMNode 包装类

class CXMLDOMNode : public COleDispatchDriver
{
public:
	CXMLDOMNode(){} // 调用 COleDispatchDriver 默认构造函数
	CXMLDOMNode(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CXMLDOMNode(const CXMLDOMNode& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// IXMLDOMNode 方法
public:
	CString get_nodeName()
	{
		CString result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	VARIANT get_nodeValue()
	{
		VARIANT result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_nodeValue(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_nodeType()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_parentNode()
	{
		LPDISPATCH result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_childNodes()
	{
		LPDISPATCH result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_firstChild()
	{
		LPDISPATCH result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_lastChild()
	{
		LPDISPATCH result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_previousSibling()
	{
		LPDISPATCH result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_nextSibling()
	{
		LPDISPATCH result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_attributes()
	{
		LPDISPATCH result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH insertBefore(LPDISPATCH newChild, VARIANT& refChild)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_VARIANT ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, newChild, &refChild);
		return result;
	}
	LPDISPATCH replaceChild(LPDISPATCH newChild, LPDISPATCH oldChild)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH VTS_DISPATCH ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, newChild, oldChild);
		return result;
	}
	LPDISPATCH removeChild(LPDISPATCH childNode)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, childNode);
		return result;
	}
	LPDISPATCH appendChild(LPDISPATCH newChild)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, newChild);
		return result;
	}
	BOOL hasChildNodes()
	{
		BOOL result;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ownerDocument()
	{
		LPDISPATCH result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH cloneNode(BOOL deep)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, deep);
		return result;
	}
	CString get_nodeTypeString()
	{
		CString result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_text()
	{
		CString result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_text(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_specified()
	{
		BOOL result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_definition()
	{
		LPDISPATCH result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_nodeTypedValue()
	{
		VARIANT result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_nodeTypedValue(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_dataType()
	{
		VARIANT result;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_dataType(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_xml()
	{
		CString result;
		InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString transformNode(LPDISPATCH stylesheet)
	{
		CString result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, stylesheet);
		return result;
	}
	LPDISPATCH selectNodes(LPCTSTR queryString)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, queryString);
		return result;
	}
	LPDISPATCH selectSingleNode(LPCTSTR queryString)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, queryString);
		return result;
	}
	BOOL get_parsed()
	{
		BOOL result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_namespaceURI()
	{
		CString result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_prefix()
	{
		CString result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_baseName()
	{
		CString result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void transformNodeToObject(LPDISPATCH stylesheet, VARIANT& outputObject)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_VARIANT ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, stylesheet, &outputObject);
	}

	// IXMLDOMNode 属性
public:

};
