// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\WINDOWS\\system32\\iPlotLibrary.ocx" no_namespace
// CiScopeToolBarX 包装类

class CiScopeToolBarX : public COleDispatchDriver
{
public:
	CiScopeToolBarX(){} // 调用 COleDispatchDriver 默认构造函数
	CiScopeToolBarX(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CiScopeToolBarX(const CiScopeToolBarX& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// IiScopeToolBarX 方法
public:
	BOOL get_CursorActive()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void DoButtonClickCursor()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoButtonClickEdit()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoButtonClickCopy()
	{
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoButtonClickSave()
	{
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoButtonClickPrint()
	{
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// IiScopeToolBarX 属性
public:

};
