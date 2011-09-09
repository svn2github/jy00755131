// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\WINDOWS\\system32\\iPlotLibrary.ocx" no_namespace
// CiXYPlotXEvents 包装类

class CiXYPlotXEvents : public COleDispatchDriver
{
public:
	CiXYPlotXEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CiXYPlotXEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CiXYPlotXEvents(const CiXYPlotXEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 属性
public:

	// 操作
public:


	// IiXYPlotXEvents 方法
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
	STDMETHOD(OnDataCursorChange)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnXAxisSpanChange)(long Index, double OldValue, double NewValue)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldValue, NewValue);
		return result;
	}
	STDMETHOD(OnXAxisMinChange)(long Index, double OldValue, double NewValue)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldValue, NewValue);
		return result;
	}
	STDMETHOD(OnYAxisSpanChange)(long Index, double OldValue, double NewValue)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldValue, NewValue);
		return result;
	}
	STDMETHOD(OnYAxisMinChange)(long Index, double OldValue, double NewValue)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldValue, NewValue);
		return result;
	}
	STDMETHOD(OnXAxisCustomizeLabel)(long Index, double Value, BSTR * ALabel)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_PBSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Value, ALabel);
		return result;
	}
	STDMETHOD(OnYAxisCustomizeLabel)(long Index, double Value, BSTR * ALabel)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_PBSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Value, ALabel);
		return result;
	}
	STDMETHOD(OnBeforePrint)()
	{
		HRESULT result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAfterPrint)()
	{
		HRESULT result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnClickDataPoint)(long ChannelIndex, long DataIndex)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, ChannelIndex, DataIndex);
		return result;
	}
	STDMETHOD(OnClickAnnotation)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusChannel)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusXAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusYAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusDataCursor)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusDataView)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusLegend)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusChannel)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusXAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusYAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusDataCursor)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusDataView)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusLegend)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLimitLine1PositionChange)(long Index, double OldValue, double NewValue)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldValue, NewValue);
		return result;
	}
	STDMETHOD(OnLimitLine2PositionChange)(long Index, double OldValue, double NewValue)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldValue, NewValue);
		return result;
	}
	STDMETHOD(OnBeforeLoadProperties)()
	{
		HRESULT result;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAfterLoadProperties)()
	{
		HRESULT result;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnBeforeZoomBox)(long * Left, long * Top, long * Right, long * Bottom, BOOL * Cancel)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PBOOL ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Left, Top, Right, Bottom, Cancel);
		return result;
	}
	STDMETHOD(OnAnnotationCoordinatesChange)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnKeyPress)(short * Key)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Key);
		return result;
	}
	STDMETHOD(OnKeyDown)(short * Key, short Shift)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PI2 VTS_I2 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Key, Shift);
		return result;
	}
	STDMETHOD(OnKeyUp)(short * Key, short Shift)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PI2 VTS_I2 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Key, Shift);
		return result;
	}
	STDMETHOD(OnAnnotationCoordinatesChangeFinished)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnGotFocusAnnotation)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnLostFocusAnnotation)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnPopupMenuChannel)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuXAxis)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuYAxis)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuDataCursor)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuDataView)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuLegend)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuAnnotation)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnPopupMenuLimit)(long Index, BOOL * Cancel, long ScreenX, long ScreenY)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Cancel, ScreenX, ScreenY);
		return result;
	}
	STDMETHOD(OnDataCursorCustomizeHint)(long Index, BSTR * AText)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, AText);
		return result;
	}
	STDMETHOD(OnClickXAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnClickYAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnClickDataView)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnClickLegend)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnClickChannel)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnClickLimit)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnClickDataCursor)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickXAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickYAxis)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickDataView)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickLegend)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickChannel)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickLimit)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickDataCursor)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnDblClickDataPoint)(long ChannelIndex, long DataIndex)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, ChannelIndex, DataIndex);
		return result;
	}
	STDMETHOD(OnDblClickAnnotation)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnXAxisMinSpanChange)(long Index, double OldMin, double OldSpan, double NewMin, double NewSpan)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldMin, OldSpan, NewMin, NewSpan);
		return result;
	}
	STDMETHOD(OnYAxisMinSpanChange)(long Index, double OldMin, double OldSpan, double NewMin, double NewSpan)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, OldMin, OldSpan, NewMin, NewSpan);
		return result;
	}
	STDMETHOD(OnAfterSaveProperties)()
	{
		HRESULT result;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAfterLoadData)()
	{
		HRESULT result;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAfterSaveData)()
	{
		HRESULT result;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAfterOpenEditor)()
	{
		HRESULT result;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAfterCloseEditor)()
	{
		HRESULT result;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnBeforeDataCursorUpdate)(long Index)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index);
		return result;
	}
	STDMETHOD(OnXAxisCustomizeHint)(long Index, BSTR * AText)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, AText);
		return result;
	}
	STDMETHOD(OnYAxisCustomizeHint)(long Index, BSTR * AText)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, AText);
		return result;
	}
	STDMETHOD(OnToolBarCustomizeHint)(long Index, BSTR * AText)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, AText);
		return result;
	}
	STDMETHOD(OnToolBarButtonClick)(long Index, long ButtonType)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, ButtonType);
		return result;
	}
	STDMETHOD(OnMouseDownXAxis)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownYAxis)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownDataView)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownLegend)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownChannel)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownLimit)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownDataCursor)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseDownAnnotation)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpXAxis)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x56, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpYAxis)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x57, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpDataView)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x58, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpLegend)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x59, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpChannel)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5a, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpLimit)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5b, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpDataCursor)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5c, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseUpAnnotation)(long Index, short Button, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5d, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Button, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveXAxis)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5e, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveYAxis)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5f, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveDataView)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveLegend)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x61, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveChannel)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x62, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveLimit)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x63, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveDataCursor)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x64, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}
	STDMETHOD(OnMouseMoveAnnotation)(long Index, short Shift, long X, long Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x65, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Index, Shift, X, Y);
		return result;
	}

	// IiXYPlotXEvents 属性
public:

};
