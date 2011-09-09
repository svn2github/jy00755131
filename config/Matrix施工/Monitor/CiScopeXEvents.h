// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\WINDOWS\\system32\\iPlotLibrary.ocx" no_namespace
// CiScopeXEvents 包装类

class CiScopeXEvents : public COleDispatchDriver
{
public:
	CiScopeXEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CiScopeXEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CiScopeXEvents(const CiScopeXEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// IiScopeXEvents 方法
public:
	void OnClick()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnDblClick()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnMouseDown(short Button, short Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnMouseMove(short Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Shift, X, Y);
	}
	void OnMouseUp(short Button, short Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnKeyPress(short * Key)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Key);
	}
	STDMETHOD(OnKeyDown)(short * Key, short Shift)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PI2 VTS_I2 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Key, Shift);
		return result;
	}
	STDMETHOD(OnKeyUp)(short * Key, short Shift)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PI2 VTS_I2 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Key, Shift);
		return result;
	}

	// IiScopeXEvents 属性
public:

};
