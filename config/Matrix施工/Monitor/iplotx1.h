#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CIplotx1 包装类

class CIplotx1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIplotx1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1791C036, 0x8981, 0x492A, { 0xBD, 0x28, 0xF2, 0x33, 0x1B, 0xC9, 0xB7, 0xC7 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:
enum
{
    iptPlot = 0,
    iptPlotXY = 1
}TxiPlotType;
enum
{
    poPortrait = 0,
    poLandscape = 1
}TxPrinterOrientation;
enum
{
    ibsNone = 0,
    ibsRaised = 1,
    ibsLowered = 2
}TxiBevelStyle;
enum
{
    mbLeft = 0,
    mbRight = 1,
    mbMiddle = 2
}TxMouseButton;
enum
{
    ipsSignificantDigits = 0,
    ipsFixedDecimalPoints = 1
}TxiPrecisionStyle;
enum
{
    iptfValue = 0,
    iptfExponent = 1,
    iptfPrefix = 2,
    iptfDateTime = 3,
    iptfPrice32nds = 4,
    iptfThousands = 5
}TxiPlotTextFormat;
enum
{
    ipstLinear = 0,
    ipstLog10 = 1
}TxiPlotScaleType;
enum
{
    iptsScaleMinMax = 0,
    iptsScaleMax = 1,
    iptsScaleMin = 2,
    iptsScrollSmooth = 3,
    iptsScrollPage = 4,
    iptsExpandCollapse = 5,
    iptsExpandCollapseFast = 6
}TxiPlotTrackingStyle;
enum
{
    ipafsMin = 0,
    ipafsMax = 1,
    ipafsAuto = 2,
    ipafsNone = 3
}TxiPlotAlignFirstStyle;
enum
{
    iplsSolid = 0,
    iplsDash = 1,
    iplsDot = 2,
    iplsDashDot = 3,
    iplsDashDotDot = 4
}TxiPlotLineStyle;
enum
{
    ipmsCircle = 0,
    ipmsSquare = 1,
    ipmsDiamond = 2,
    ipmsCross = 3,
    ipmsPlus = 4,
    ipmsTriangleUp = 5,
    ipmsTriangleDown = 6,
    ipmsTriangleLeft = 7,
    ipmsTriangleRight = 8,
    ipmsVerticalLine = 9,
    ipmsHorizontalLine = 10,
    ipmsCharacter = 11
}TxiPlotMarkerStyle;
enum
{
    psSolid = 0,
    psDash = 1,
    psDot = 2,
    psDashDot = 3,
    psDashDotDot = 4,
    psClear = 5,
    psInsideFrame = 6
}TxPenStyle;
enum
{
    bsSolid = 0,
    bsClear = 1,
    bsHorizontal = 2,
    bsVertical = 3,
    bsFDiagonal = 4,
    bsBDiagonal = 5,
    bsCross = 6,
    bsDiagCross = 7
}TxBrushStyle;
enum
{
    ipglsSolid = 0,
    ipglsDash = 1,
    ipglsDot = 2
}TxiPlotGridLineStyle;
enum
{
    ipasText = 0,
    ipasLine = 1,
    ipasRectangle = 2,
    ipasLineX = 3,
    ipasLineY = 4,
    ipasImage = 5,
    ipasTextRectangle = 6,
    ipasEllipse = 7,
    ipasTextBox = 8
}TxiPlotAnnotationStyle;
enum
{
    iprtDataView = 0,
    iprtChannel = 1,
    iprtXChannelYDataView = 2,
    iprtXDataViewYChannel = 3
}TxiPlotReferenceType;
enum
{
    ipdrScale = 0,
    ipdrPercent = 1
}TxiPlotDigitalReferenceStyle;
enum
{
    ipdpsValid = 0,
    ipdpsNull = 1,
    ipdpsEmpty = 2
}TxiPlotDataPointStatus;
enum
{
    ipcsValueXY = 0,
    ipcsValueX = 1,
    ipcsValueY = 2,
    ipcsDeltaX = 3,
    ipcsDeltaY = 4,
    ipcsInverseDeltaX = 5
}TxiPlotDataCursorStyle;
enum
{
    iosBottomRight = 0,
    iosTopLeft = 1
}TxiOrientationSide;
enum
{
    iplsLineX = 0,
    iplsLineY = 1,
    iplsBandX = 2,
    iplsBandY = 3,
    iplsPolyBandX = 4,
    iplsPolyBandY = 5
}TxiPlotLimitStyle;
enum
{
    ira000 = 0,
    ira090 = 1,
    ira180 = 2,
    ira270 = 3
}TxiRotationAngle;
enum
{
    iahCenter = 0,
    iahLeft = 1,
    iahRight = 2
}TxiAlignmentHorizontal;
enum
{
    ipistStraightLine = 0,
    ipistCubicSpline = 1,
    ipistPolynomial = 2,
    ipistRational = 3,
    ipistDifferential = 4,
    ipistDifferentialTerminated = 5
}TxiPlotInterpolationStyle;
enum
{
    iopcxvsOPCServerTimeStamp = 0,
    iopcxvsSystemTimeStamp = 1,
    iopcxvsElapsedTime = 2,
    iopcxvsElapsedSeconds = 3
}TxiOPCXValueSourceStyle;
enum
{
    ipfsModal = 0,
    ipfsStayOnTop = 1
}TxiPlotFormStyle;
enum
{
    ipefMetaFile = 0,
    ipefBitmap = 1,
    ipefJPEG = 2
}TxiPlotExportFormat;
enum
{
    ipcsNone = 0,
    ipcsMaster = 1,
    ipcsChild = 2
}TxiPlotCartesianStyle;
enum
{
    ipdsStandard = 0,
    ipdsCompact = 1,
    ipdsCompactInterval = 2,
    ipdsFullFeature = 3
}TxiPlotDataStyle;
enum
{
    iavCenter = 0,
    iavTop = 1,
    iavBottom = 2
}TxiAlignmentVertical;
enum
{
    ifdTopBottom = 0,
    ifdBottomTop = 1,
    ifdLeftRight = 2,
    ifdRightLeft = 3,
    ifdTopLeftBottomRight = 4,
    ifdBottomRightTopLeft = 5,
    ifdBottomLeftTopRight = 6,
    ifdTopRightBottomLeft = 7,
    ifdRectangleOut = 8,
    ifdRectangleIn = 9,
    ifdCircleOut = 10,
    ifdCircleIn = 11
}TxiFillDirection;
enum
{
    htKeyword = 0,
    htContext = 1
}TxHelpType;
enum
{
    isccDC = 0,
    isccAC = 1,
    isccGround = 2
}TxiScopeChannelCoupling;
enum
{
    ipttMajor = 0,
    ipttMinor = 1
}TxiPlotTickType;
enum
{
    ipacsXAxis = 0,
    ipacsYAxis = 1,
    ipacsBoth = 2
}TxiPlotAxesControlStyle;
enum
{
    ipfcsTab = 0,
    ipdfcsComma = 1
}TxiPlotDataFileColumnSeparator;
enum
{
    iplcncsFont = 0,
    iplcncsChannelColor = 1,
    iplcncsXAxisFontColor = 2,
    iplcncsYAxisFontColor = 3
}TxiPlotLegendChannelNameColorStyle;
enum
{
    ipdffText = 0,
    ipdffCompactInterval = 1
}TxiPlotDataFileFormat;
enum
{
    iphlsOpenClose = 0,
    iphlsCandleStick = 1
}TxiPlotHighLowStyle;
enum
{
    ipamScroll = 0,
    ipamZoom = 1
}TxiPlotAxisMode;
enum
{
    iptbbtResume = 0,
    iptbbtPause = 1,
    iptbbtScrollAxesMode = 2,
    iptbbtZoomAxesMode = 3,
    iptbbtSelect = 4,
    iptbbtZoomBox = 5,
    iptbbtZoomOut = 6,
    iptbbtZoomIn = 7,
    iptbbtCursor = 8,
    iptbbtEdit = 9,
    iptbbtCopy = 10,
    iptbbtSave = 11,
    iptbbtPrint = 12,
    iptbbtPreview = 13
}TxiPlotToolBarButtonType;


// 操作
public:

// IiPlotX

// Functions
//

	long get_DataViewZHorz()
	{
		long result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DataViewZHorz(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DataViewZVert()
	{
		long result;
		InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DataViewZVert(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_XYAxesReverse()
	{
		BOOL result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_XYAxesReverse(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OuterMarginLeft()
	{
		long result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OuterMarginLeft(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OuterMarginTop()
	{
		long result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OuterMarginTop(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OuterMarginRight()
	{
		long result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OuterMarginRight(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OuterMarginBottom()
	{
		long result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OuterMarginBottom(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_PrintOrientation()
	{
		long result;
		InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PrintOrientation(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_PrintMarginLeft()
	{
		double result;
		InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_PrintMarginLeft(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_PrintMarginTop()
	{
		double result;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_PrintMarginTop(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_PrintMarginRight()
	{
		double result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_PrintMarginRight(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_PrintMarginBottom()
	{
		double result;
		InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_PrintMarginBottom(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_PrintShowDialog()
	{
		BOOL result;
		InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PrintShowDialog(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_UpdateFrameRate()
	{
		long result;
		InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_UpdateFrameRate(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackGroundColor()
	{
		unsigned long result;
		InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackGroundColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BorderStyle()
	{
		long result;
		InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BorderStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoFrameRate()
	{
		BOOL result;
		InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoFrameRate(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_XAxis(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_YAxis(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_Legend(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_ToolBar(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_DataView(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	void DisableLayoutManager()
	{
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EnableLayoutManager()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DisableAllTracking()
	{
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EnableAllTracking()
	{
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long AddXAxis()
	{
		long result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddYAxis()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddLegend()
	{
		long result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddToolBar()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddDataView()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddChannel()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void RemoveAllXAxes()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RemoveAllYAxes()
	{
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RemoveAllLegends()
	{
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RemoveAllToolBars()
	{
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RemoveAllDataViews()
	{
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RemoveAllChannels()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_XAxisCount()
	{
		long result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_YAxisCount()
	{
		long result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_LegendCount()
	{
		long result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_ToolBarCount()
	{
		long result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_DataViewCount()
	{
		long result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_ChannelCount()
	{
		long result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_HintsShow()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HintsShow(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HintsPause()
	{
		long result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_HintsPause(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HintsHidePause()
	{
		long result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_HintsHidePause(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void DeleteToolBar(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void DeleteLegend(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void DeleteXAxis(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void DeleteYAxis(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void DeleteChannel(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void DeleteDataView(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void ShowPropertyEditor()
	{
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_TitleVisible()
	{
		BOOL result;
		InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TitleVisible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_TitleText()
	{
		CString result;
		InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_TitleText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x38, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_TitleMargin()
	{
		double result;
		InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_TitleMargin(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x39, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_TitleFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_TitleFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x3a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_TitleFontColor()
	{
		unsigned long result;
		InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_TitleFontColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x3b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void RemoveAllAnnotations()
	{
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DeleteAnnotation(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long AddAnnotation()
	{
		long result;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_AnnotationCount()
	{
		long result;
		InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Annotation(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long GetChannelIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetXAxisIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetYAxisIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	BOOL get_UserCanEditObjects()
	{
		BOOL result;
		InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UserCanEditObjects(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x44, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void PrintChart()
	{
		InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CopyToClipBoard()
	{
		InvokeHelper(0x46, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Save()
	{
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH GetSnapShotPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void RepaintAll()
	{
		InvokeHelper(0x49, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void BeginUpdate()
	{
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EndUpdate()
	{
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Lock()
	{
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Unlock()
	{
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ClearAllData()
	{
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddDataArray(double XValue, VARIANT Data)
	{
		static BYTE parms[] = VTS_R8 VTS_VARIANT ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_EMPTY, NULL, parms, XValue, &Data);
	}
	double GetNow()
	{
		double result;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	void SavePropertiesToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadPropertiesFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveDataToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadDataFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	CString get_LogFileName()
	{
		CString result;
		InvokeHelper(0x56, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_LogFileName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x56, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_LogBufferSize()
	{
		long result;
		InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_LogBufferSize(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x57, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void LogActivate(BOOL Append)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x58, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Append);
	}
	void LogDeactivate()
	{
		InvokeHelper(0x59, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_LoggingActive()
	{
		BOOL result;
		InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_ComponentHandle()
	{
		long result;
		InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DataCursor(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long get_DataCursorCount()
	{
		long result;
		InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddDataCursor()
	{
		long result;
		InvokeHelper(0x5e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DeleteDataCursor(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void RemoveAllDataCursors()
	{
		InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Limit(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long get_LimitCount()
	{
		long result;
		InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddLimit()
	{
		long result;
		InvokeHelper(0x61, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DeleteLimit(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x62, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void RemoveAllLimits()
	{
		InvokeHelper(0x63, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SaveImageToBitmap(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveImageToMetaFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BOOL ;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, Compression, Progressive);
	}
	void SaveTranslationsToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadTranslationsFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void RemoveAllTranslations()
	{
		InvokeHelper(0x6a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DeleteTranslation(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long AddTranslation(LPCTSTR OriginalString, LPCTSTR ReplacementString)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x6c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OriginalString, ReplacementString);
		return result;
	}
	long TranslationCount()
	{
		long result;
		InvokeHelper(0x6d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetTranslationOriginalString(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	CString GetTranslationReplacementString(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6f, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get_Labels(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long get_LabelCount()
	{
		long result;
		InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddLabel()
	{
		long result;
		InvokeHelper(0x72, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DeleteLabel(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void RemoveAllLabels()
	{
		InvokeHelper(0x74, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	VARIANT GetBytesJPEG(long Compression, BOOL Progressive)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x1069, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Compression, Progressive);
		return result;
	}
	long GetObjectPointer()
	{
		long result;
		InvokeHelper(0x1068, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_OptionSaveAllProperties()
	{
		BOOL result;
		InvokeHelper(0x106d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_OptionSaveAllProperties(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x106d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_HintsFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x75, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_HintsFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x75, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_HintsFontColor()
	{
		unsigned long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_HintsFontColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x76, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_AnnotationDefaultFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x77, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x77, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_AnnotationDefaultFontColor()
	{
		unsigned long result;
		InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultFontColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AnnotationDefaultBrushStlye()
	{
		long result;
		InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultBrushStlye(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x79, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_AnnotationDefaultBrushColor()
	{
		unsigned long result;
		InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultBrushColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x7a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AnnotationDefaultPenStlye()
	{
		long result;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultPenStlye(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_AnnotationDefaultPenColor()
	{
		unsigned long result;
		InvokeHelper(0x7c, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultPenColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x7c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AnnotationDefaultPenWidth()
	{
		long result;
		InvokeHelper(0x7d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AnnotationDefaultPenWidth(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Width()
	{
		long result;
		InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Width(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Height()
	{
		long result;
		InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Height(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_UserCanAddRemoveChannels()
	{
		BOOL result;
		InvokeHelper(0x80, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UserCanAddRemoveChannels(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x80, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SaveAnnotationsToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadAnnotationsFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x82, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void iPaintToDC(long X, long Y, BOOL Transparent, long DC)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_I4 ;
		InvokeHelper(0x10cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y, Transparent, DC);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x84, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x84, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(0x85, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x85, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EditorFormStyle()
	{
		long result;
		InvokeHelper(0x83, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EditorFormStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x83, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CopyToClipBoardFormat()
	{
		long result;
		InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CopyToClipBoardFormat(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_PrintDocumentName()
	{
		CString result;
		InvokeHelper(0x87, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_PrintDocumentName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x87, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void ImageListClear(long ImageListIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x88, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ImageListIndex);
	}
	void ImageListLoadFromResourceID(long ImageListIndex, unsigned long Instance, long ResID)
	{
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_I4 ;
		InvokeHelper(0x8a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ImageListIndex, Instance, ResID);
	}
	void ImageListLoadFromResourceName(long ImageListIndex, unsigned long Instance, LPCTSTR ResName)
	{
		static BYTE parms[] = VTS_I4 VTS_UI4 VTS_BSTR ;
		InvokeHelper(0x8b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ImageListIndex, Instance, ResName);
	}
	CString get_PrinterName()
	{
		CString result;
		InvokeHelper(0x89, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_PrinterName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x89, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ClipAnnotationsToAxes()
	{
		BOOL result;
		InvokeHelper(0x8c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ClipAnnotationsToAxes(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_BackGroundGradientEnabled()
	{
		BOOL result;
		InvokeHelper(0x8d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_BackGroundGradientEnabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BackGroundGradientDirection()
	{
		long result;
		InvokeHelper(0x8e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BackGroundGradientDirection(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackGroundGradientStartColor()
	{
		unsigned long result;
		InvokeHelper(0x8f, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackGroundGradientStartColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x8f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackGroundGradientStopColor()
	{
		unsigned long result;
		InvokeHelper(0x90, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackGroundGradientStopColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x90, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Table(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x91, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long get_TableCount()
	{
		long result;
		InvokeHelper(0x92, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddTable()
	{
		long result;
		InvokeHelper(0x94, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DeleteTable(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x95, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void RemoveAllTables()
	{
		InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ReCalcLayout()
	{
		InvokeHelper(0x93, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void TransferChannelData(long SourceChannelIndex, long DestinationChannelIndex, long StartIndex, long StopIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SourceChannelIndex, DestinationChannelIndex, StartIndex, StopIndex);
	}
	void ImageListAdd(long ImageListIndex, LPDISPATCH APicture)
	{
		static BYTE parms[] = VTS_I4 VTS_DISPATCH ;
		InvokeHelper(0x98, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ImageListIndex, APicture);
	}
	long get_DataFileColumnSeparator()
	{
		long result;
		InvokeHelper(0x99, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DataFileColumnSeparator(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x99, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DataFileFormat()
	{
		long result;
		InvokeHelper(0x9a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DataFileFormat(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_LastPropertiesFileName()
	{
		CString result;
		InvokeHelper(0x9b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_LastDataFileName()
	{
		CString result;
		InvokeHelper(0x9c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetDataViewIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetToolBarIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetLegendIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetAnnotationIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetDataCursorIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetLabelIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetLimitIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	long GetTableIndexByName(LPCTSTR Value)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetToolBarByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetLegendByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetTableByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetXAxisByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetYAxisByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetDataViewByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xaa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetDataCursorByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xab, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetLimitByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xad, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetLabelByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xae, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	LPDISPATCH GetAnnotationByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xaf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}
	VARIANT GetBytesPNG(long Compression)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xac, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Compression);
		return result;
	}
	void SaveImageToPNG(LPCTSTR FileName, long Compression)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0xb0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, Compression);
	}
	LPDISPATCH get_Channel(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH GetChannelByName(LPCTSTR Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Value);
		return result;
	}

// Properties
//



};
