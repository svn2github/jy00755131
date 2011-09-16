
#include "stdafx.h"
#include "SiteData.h"

CSiteData::CSiteData(void)
{
	m_uiInstrumentCountAll = 20000;	// 仪器队列仪器总数	
	m_uiChannelCountAll = 20000;	// 测道总数

	// 和内部网络通讯的本机IP地址
	m_strIPForInterface = "192.168.0.11";
	// 用于客户端内部网络命令接收的信宿端口号
	m_uiPortForCmdRev = 0x6200;
	// 用于客户端内部网络命令发送的信源端口号
	m_uiPortForCmdSend = 0x6400;
	// 用于服务器内部网络命令发送的信宿端口号，测线服务器
	m_uiPortForCmdSendToServer = 0x6000;
	// 用于服务器内部网络命令发送的信宿端口号，施工客户端
	m_uiPortForCmdSendToOperation = 0xC010;
	// 用于客户端内部网络数据接收的信宿端口号
	m_uiPortForDataRev = 0x6300;
}

CSiteData::~CSiteData(void)
{
}

/**
* 初始化
* @param void
* @return void
*/
void CSiteData::OnInit()
{
	// 初始化SegdCode队列
	InitSegdCodeList();
	// 初始化BoxType队列
	InitBoxTypeList();
	// 初始化Gain队列
	InitGainList();
	// 初始化Orientation队列
	InitOrientationList();
	// 初始化FilterType队列
	InitFilterTypeList();
	// 初始化ADCType队列
	InitADCTypeList();
	// 初始化DACType队列
	InitDACTypeList();

	//加载测线服务器程序设置数据
	LoadLineServerAppSetupData();

	// 现场仪器数据队列
	m_oInstrumentList.m_uiCountAll = m_uiInstrumentCountAll;	// 仪器队列仪器总数	
	m_oInstrumentList.OnInit();	// 初始化

	// 路由队列初始化
	m_oRoutList.OnInit();	// 初始化

	// 现场测道数据队列初始化
	m_oChannelList.m_uiCountAll = m_uiChannelCountAll;	// 测道总数
	m_oChannelList.OnInit();	// 初始化

	// 仪器图形视图仪器图形单元队列
	m_oGraphInstrumentList.m_uiCountAll = m_uiChannelCountAll;	// 仪器总数
	m_oGraphInstrumentList.m_pInstrumentList = &m_oInstrumentList;
	m_oGraphInstrumentList.OnInit();	// 初始化

	// 检波器图形视图检波器图形单元队列
	m_oGraphSensorList.m_uiCountAll = m_uiChannelCountAll;	// 测道总数
	m_oGraphSensorList.m_pChannelList = &m_oChannelList;
	m_oGraphSensorList.OnInit();	// 初始化

	// 由FTP服务器得到Matrix设置文件
	GetMatrixIniFileFromFTPServer();

	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 测线设置队列
		m_oLineList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oLineList.OnInit();	// 初始化
		// 点代码设置队列
		m_oPointCodeList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oPointCodeList.OnInit();	// 初始化
		// 检波器设置队列
		m_oSensorList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oSensorList.OnInit();	// 初始化

		// 标记点设置队列
		m_oMarkerList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oMarkerList.OnInit();	// 初始化
		// 辅助道设置队列
		m_oAuxList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oAuxList.OnInit();	// 初始化
		// 爆炸机控制器设置队列
		m_oBlastMachineList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oBlastMachineList.OnInit();	// 初始化
		// 迂回道设置队列
		m_oDetourList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oDetourList.OnInit();	// 初始化
		// 哑点设置队列
		m_oMuteList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oMuteList.OnInit();	// 初始化

		// 绝对排列设置队列
		m_oSpreadAbsoluteList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oSpreadAbsoluteList.OnInit();	// 初始化
		// 相对排列设置队列
		m_oSpreadGenericList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oSpreadGenericList.OnInit();	// 初始化

		// 炮点设置索引
		m_oShotPointMap.m_pXMLDOMDocument = &m_oXMLDOMDocument;

		// 仪器测试基本参数队列，仪器
		m_oTestBaseListInstrument.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestBaseListInstrument.m_uiAim = 1;	// 测试对象	1-仪器；2-检波器；3-全部
		m_oTestBaseListInstrument.OnInit();
		// 检波器测试基本参数队列，检波器
		m_oTestBaseListSensor.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestBaseListSensor.m_uiAim = 2;	// 测试对象	1-仪器；2-检波器；3-全部
		m_oTestBaseListSensor.OnInit();

		// 仪器测试判据参数队列，仪器
		m_oTestLimitListInstrument.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestLimitListInstrument.m_uiAim = 1;	// 测试对象	1-仪器；2-检波器
		m_oTestLimitListInstrument.OnInit();
		// 检波器测试判据参数队列，检波器
		m_oTestLimitListSensor.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestLimitListSensor.m_uiAim = 2;	// 测试对象	1-仪器；2-检波器
		m_oTestLimitListSensor.OnInit();

		// 测试设置队列，仪器测试
		m_oTestProjectListInstrument.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestProjectListInstrument.m_uiProjectType = 1;	 // 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		m_oTestProjectListInstrument.OnInit();
		// 测试设置队列，检波器测试
		m_oTestProjectListSensor.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestProjectListSensor.m_uiProjectType = 2;	 // 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		m_oTestProjectListSensor.OnInit();
		// 测试设置队列，检波器测试
		m_oTestProjectListMultiple.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestProjectListMultiple.m_uiProjectType = 3;	 // 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		m_oTestProjectListMultiple.OnInit();
		// 测试设置队列，检波器测试
		m_oTestProjectListSeisMonitor.m_pXMLDOMDocument = &m_oXMLDOMDocument;	// XMLDOM文件对象
		m_oTestProjectListSeisMonitor.m_uiProjectType = 4;	 // 测试项目类型 1-仪器测试；2-检波器测试；3-复合测试；4-噪声监测
		m_oTestProjectListSeisMonitor.OnInit();

		// FormLine设置队列
		m_oFormLineList.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oFormLineList.OnInit();	// 初始化

		// Look设置对象
		m_oLookSetupData.m_pXMLDOMDocument = &m_oXMLDOMDocument;
		m_oLookSetupData.OnInit();	// 初始化
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 初始化TestType队列
	InitTestTypeList();
	// 初始化复合测试名称队列
	InitMultipleTestNameList();


	// 建立客户端内部网络命令发送处理网络对象
	m_oSocketInterfaceCmdSend.OnInit(m_strIPForInterface, m_uiPortForCmdSend, m_uiPortForCmdSendToServer, m_uiPortForCmdSendToOperation);	
	m_oSocketInterfaceCmdSend.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小	

	// 建立客户端内部网络命令帧接收和处理对象	
	m_oSocketInterfaceCmdRev.OnInit(m_strIPForInterface, m_uiPortForCmdRev);	
	m_oSocketInterfaceCmdRev.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小

	// 建立客户端内部网络数据帧接收和处理对象	
	m_oSocketInterfaceDataRev.OnInit(m_strIPForInterface, m_uiPortForDataRev);	
	m_oSocketInterfaceDataRev.SetBufferSize(65536 * 10);	// 初始化网络端口接收缓冲区大小
}

/**
* 重置
* @param void
* @return void
*/
void CSiteData::OnReset()
{
	m_oInstrumentList.OnReset();
	m_oRoutList.OnReset();
	m_oChannelList.OnReset();

	m_oGraphInstrumentList.OnReset();
	m_oGraphSensorList.OnReset();
}

/**
* 关闭
* @param void
* @return void
*/
void CSiteData::OnClose()
{
	// 清除SegdCode队列
	CleanSegdCodeList();
	// 清除BoxType队列
	CleanBoxTypeList();
	// 清除Gain队列
	CleanGainList();
	// 清除Orientation队列
	CleanOrientationList();
	// 清除FilterType队列
	CleanFilterTypeList();
	// 清除ADCType队列
	CleanADCTypeList();
	// 清除DACType队列
	CleanDACTypeList();
	// 清除TestType队列
	CleanTestTypeList();
	// 清除MultipleTestName队列
	CleanMultipleTestNameList();

	m_oInstrumentList.OnClose();
	m_oRoutList.OnClose();
	m_oChannelList.OnClose();

	m_oGraphInstrumentList.OnClose();
	m_oGraphSensorList.OnClose();

	// 测线设置队列
	m_oLineList.OnClose();
	// 点代码设置队列
	m_oPointCodeList.OnClose();	
	// 检波器设置队列
	m_oSensorList.OnClose();

	// 标记点设置队列
	m_oMarkerList.OnClose();
	// 辅助道设置队列
	m_oAuxList.OnClose();
	// 爆炸机控制器设置队列
	m_oBlastMachineList.OnClose();
	// 迂回道设置队列
	m_oDetourList.OnClose();
	// 哑点设置队列
	m_oMuteList.OnClose();

	// 绝对排列设置队列
	m_oSpreadAbsoluteList.OnClose();
	// 相对排列设置队列
	m_oSpreadGenericList.OnClose();

	// 仪器测试基本参数队列，仪器
	m_oTestBaseListInstrument.OnClose();
	// 检波器测试基本参数队列，检波器
	m_oTestBaseListSensor.OnClose();

	// 测试设置队列，仪器测试
	m_oTestProjectListInstrument.OnClose();
	// 测试设置队列，检波器测试
	m_oTestProjectListSensor.OnClose();
	// 测试设置队列，检波器测试
	m_oTestProjectListMultiple.OnClose();
	// 测试设置队列，检波器测试
	m_oTestProjectListSeisMonitor.OnClose();

	// FormLine设置队列
	m_oFormLineList.OnClose();
}

/**
* 初始化SegdCode队列
* @param void
* @return void
*/
void CSiteData::InitSegdCodeList()
{
	CString strSegdCode;

	strSegdCode = "0:Not defined";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "1:Hydrophone";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "2:Geophone vertical";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "3:Geophone horizontal inline";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "4:Geophone horizontal cross-inline";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "5:Geophone horizontal other";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "6:Accelerometer vertical";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "7:Accelerometer horizontal inline";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "8:Accelerometer horizontal cross-inline";
	m_olsSegdCode.AddTail(strSegdCode);

	strSegdCode = "9:Accelerometer horizontal other";
	m_olsSegdCode.AddTail(strSegdCode);
}

/**
* 清除SegdCode队列
* @param void
* @return void
*/
void CSiteData::CleanSegdCodeList()
{
	m_olsSegdCode.RemoveAll();
}

/**
* 得到SegdCode字符串
* @param unsigned int uiSegdCode SegdCode
* @return CString SegdCode字符串
*/
CString CSiteData::GetSegdCodeStringByID(unsigned int uiSegdCode)
{
	POSITION pos;
	CString strSegdCode = "";
	pos = m_olsSegdCode.FindIndex(uiSegdCode);
	if(NULL != pos)
	{
		strSegdCode = m_olsSegdCode.GetAt(pos);
	}

	return strSegdCode;
}

/**
* 得到SegdCodeID
* @param CString strSegdCode SegdCode字符串
* @return unsigned int SegdCodeID 0：无对应SegdCodeID
*/
unsigned int CSiteData::GetSegdCodeIDByString(CString strSegdCode)
{
	unsigned int uiSegdCode = 0;
	POSITION pos;
	pos = m_olsSegdCode.Find(strSegdCode);
	if(NULL != pos)
	{
		CString strSegdCode = m_olsSegdCode.GetAt(pos);
		strSegdCode = strSegdCode.Left(1);
		uiSegdCode = CXMLDOMTool::ConvertStringToUnsignedInt(strSegdCode);
	}
	return uiSegdCode;
}

/**
* 初始化BoxType队列
* @param void
* @return void
*/
void CSiteData::InitBoxTypeList()
{
	CString strBoxType;

	strBoxType = "1:LAUX";
	m_olsBoxType.AddTail(strBoxType);

	strBoxType = "2:LAUL";
	m_olsBoxType.AddTail(strBoxType);

	strBoxType = "3:FDU";
	m_olsBoxType.AddTail(strBoxType);
}

/**
* 清除BoxType队列
* @param void
* @return void
*/
void CSiteData::CleanBoxTypeList()
{
	m_olsBoxType.RemoveAll();
}

/**
* 得到BoxType字符串
* @param unsigned int uiBoxType BoxType
* @return CString BoxType字符串
*/
CString CSiteData::GetBoxTypeStringByID(unsigned int uiBoxType)
{
	POSITION pos;
	CString strBoxType = "";
	pos = m_olsBoxType.FindIndex(uiBoxType - 1);
	if(NULL != pos)
	{
		strBoxType = m_olsBoxType.GetAt(pos);
	}

	return strBoxType;
}

/**
* 得到BoxTypeID
* @param CString strBoxType BoxType字符串
* @return unsigned int BoxTypeID 0：无对应BoxTypeID
*/
unsigned int CSiteData::GetBoxTypeIDByString(CString strBoxType)
{
	unsigned int uiBoxType = 0;
	POSITION pos;
	pos = m_olsBoxType.Find(strBoxType);
	if(NULL != pos)
	{
		CString strBoxType = m_olsBoxType.GetAt(pos);
		strBoxType = strBoxType.Left(1);
		uiBoxType = CXMLDOMTool::ConvertStringToUnsignedInt(strBoxType);
	}
	return uiBoxType;
}

/**
* 初始化Gain队列
* @param void
* @return void
*/
void CSiteData::InitGainList()
{
	CString strGain;

	strGain = "1600: g1";
	m_olsGain.AddTail(strGain);

	strGain = "400: g2";
	m_olsGain.AddTail(strGain);
}

/**
* 清除Gain队列
* @param void
* @return void
*/
void CSiteData::CleanGainList()
{
	m_olsGain.RemoveAll();
}

/**
* 得到Gain字符串
* @param unsigned int uiGain Gain
* @return CString Gain字符串
*/
CString CSiteData::GetGainStringByGainData(unsigned int uiGain)
{
	POSITION pos;
	CString strGain = "";
	int iIndex = -1;
	if(1600 == uiGain)
	{
		iIndex = 0;
	}
	else if(400 == uiGain)
	{
		iIndex = 1;
	}
	if(-1 != iIndex)
	{
		pos = m_olsGain.FindIndex(iIndex);
		if(NULL != pos)
		{
			strGain = m_olsGain.GetAt(pos);
		}
	}
	return strGain;
}

/**
* 得到Gain
* @param CString strGain Gain字符串
* @return unsigned int Gain 0：无对应Gain
*/
unsigned int CSiteData::GetGainByString(CString strGain)
{
	int iPos = strGain.Find(':');
	CString strData = strGain.Left(iPos);	
	unsigned int uiGain = CXMLDOMTool::ConvertStringToUnsignedInt(strData);
	return uiGain;
}

/**
* 初始化Orientation队列
* @param void
* @return void
*/
void CSiteData::InitOrientationList()
{
	CString strOrientation;

	strOrientation = "1:Top";
	m_olsOrientation.AddTail(strOrientation);

	strOrientation = "2:Down";
	m_olsOrientation.AddTail(strOrientation);

	strOrientation = "3:Left";
	m_olsOrientation.AddTail(strOrientation);
	
	strOrientation = "4:Right";
	m_olsOrientation.AddTail(strOrientation);	
}

/**
* 清除Orientation队列
* @param void
* @return void
*/
void CSiteData::CleanOrientationList()
{
	m_olsOrientation.RemoveAll();
}

/**
* 得到Orientation字符串
* @param unsigned int uiOrientation Orientation
* @return CString Orientation字符串
*/
CString CSiteData::GetOrientationStringByID(unsigned int uiOrientation)
{
	POSITION pos;
	CString strOrientation = "";
	pos = m_olsOrientation.FindIndex(uiOrientation - 1);
	if(NULL != pos)
	{
		strOrientation = m_olsOrientation.GetAt(pos);
	}

	return strOrientation;
}

/**
* 得到OrientationID
* @param CString strOrientation Orientation字符串
* @return unsigned int OrientationID 0：无对应OrientationID
*/
unsigned int CSiteData::GetOrientationIDByString(CString strOrientation)
{
	unsigned int uiOrientation = 0;
	POSITION pos;
	pos = m_olsOrientation.Find(strOrientation);
	if(NULL != pos)
	{
		CString strOrientation = m_olsOrientation.GetAt(pos);
		strOrientation = strOrientation.Left(1);
		uiOrientation = CXMLDOMTool::ConvertStringToUnsignedInt(strOrientation);
	}
	return uiOrientation;
}

/**
* 初始化FilterType队列
* @param void
* @return void
*/
void CSiteData::InitFilterTypeList()
{
	CString strFilterType;

	strFilterType = "1:0.8 LIN";
	m_olsFilterType.AddTail(strFilterType);

	strFilterType = "2:0.8 MIN";
	m_olsFilterType.AddTail(strFilterType);
}

/**
* 清除FilterType队列
* @param void
* @return void
*/
void CSiteData::CleanFilterTypeList()
{
	m_olsFilterType.RemoveAll();
}

/**
* 得到FilterType字符串
* @param unsigned int uiFilterType FilterType
* @return CString FilterType字符串
*/
CString CSiteData::GetFilterTypeStringByID(unsigned int uiFilterType)
{
	POSITION pos;
	CString strFilterType = "";
	pos = m_olsFilterType.FindIndex(uiFilterType - 1);
	if(NULL != pos)
	{
		strFilterType = m_olsFilterType.GetAt(pos);
	}

	return strFilterType;
}

/**
* 得到FilterTypeID
* @param CString strFilterType FilterType字符串
* @return unsigned int FilterTypeID 0：无对应FilterTypeID
*/
unsigned int CSiteData::GetFilterTypeIDByString(CString strFilterType)
{
	unsigned int uiFilterType = 0;
	POSITION pos;
	pos = m_olsFilterType.Find(strFilterType);
	if(NULL != pos)
	{
		CString strFilterType = m_olsFilterType.GetAt(pos);
		strFilterType = strFilterType.Left(1);
		uiFilterType = CXMLDOMTool::ConvertStringToUnsignedInt(strFilterType);
	}
	return uiFilterType;
}

/**
* 初始化ADCType队列
* @param void
* @return void
*/
void CSiteData::InitADCTypeList()
{
	CString strADCType;

	strADCType = "0:Close";
	m_olsADCType.AddTail(strADCType);

	strADCType = "1:Inner";
	m_olsADCType.AddTail(strADCType);
	
	strADCType = "2:Out";
	m_olsADCType.AddTail(strADCType);	
}

/**
* 清除ADCType队列
* @param void
* @return void
*/
void CSiteData::CleanADCTypeList()
{
	m_olsADCType.RemoveAll();
}

/**
* 得到ADCType字符串
* @param unsigned int uiADCType ADCType
* @return CString ADCType字符串
*/
CString CSiteData::GetADCTypeStringByID(unsigned int uiADCType)
{
	POSITION pos;
	CString strADCType = "";
	pos = m_olsADCType.FindIndex(uiADCType);
	if(NULL != pos)
	{
		strADCType = m_olsADCType.GetAt(pos);
	}

	return strADCType;
}

/**
* 得到ADCTypeID
* @param CString strADCType ADCType字符串
* @return unsigned int ADCTypeID 0：无对应ADCTypeID
*/
unsigned int CSiteData::GetADCTypeIDByString(CString strADCType)
{
	unsigned int uiADCType = 0;
	POSITION pos;
	pos = m_olsADCType.Find(strADCType);
	if(NULL != pos)
	{
		CString strADCType = m_olsADCType.GetAt(pos);
		strADCType = strADCType.Left(1);
		uiADCType = CXMLDOMTool::ConvertStringToUnsignedInt(strADCType);
	}
	return uiADCType;
}

/**
* 初始化DACType队列
* @param void
* @return void
*/
void CSiteData::InitDACTypeList()
{
	CString strDACType;

	strDACType = "0:Close";
	m_olsDACType.AddTail(strDACType);

	strDACType = "1:Inner";
	m_olsDACType.AddTail(strDACType);
	
	strDACType = "2:Out";
	m_olsDACType.AddTail(strDACType);	
}

/**
* 清除DACType队列
* @param void
* @return void
*/
void CSiteData::CleanDACTypeList()
{
	m_olsDACType.RemoveAll();
}

/**
* 得到DACType字符串
* @param unsigned int uiDACType DACType
* @return CString DACType字符串
*/
CString CSiteData::GetDACTypeStringByID(unsigned int uiDACType)
{
	POSITION pos;
	CString strDACType = "";
	pos = m_olsDACType.FindIndex(uiDACType);
	if(NULL != pos)
	{
		strDACType = m_olsDACType.GetAt(pos);
	}

	return strDACType;
}

/**
* 得到DACTypeID
* @param CString strDACType DACType字符串
* @return unsigned int DACTypeID 0：无对应DACTypeID
*/
unsigned int CSiteData::GetDACTypeIDByString(CString strDACType)
{
	unsigned int uiDACType = 0;
	POSITION pos;
	pos = m_olsDACType.Find(strDACType);
	if(NULL != pos)
	{
		CString strDACType = m_olsDACType.GetAt(pos);
		strDACType = strDACType.Left(1);
		uiDACType = CXMLDOMTool::ConvertStringToUnsignedInt(strDACType);
	}
	return uiDACType;
}

/**
* 初始化TestType队列
* @param void
* @return void
*/
void CSiteData::InitTestTypeList()
{
	CString strTestType;
	unsigned int uiTestType;
	POSITION pos = NULL;
	CTestBaseData oTestBaseData;
	unsigned int i;
	for(i = 0; i < m_oTestBaseListInstrument.m_uiCountAll; i++)
	{
		pos = m_oTestBaseListInstrument.m_olsTestBase.FindIndex(i);
		oTestBaseData = m_oTestBaseListInstrument.m_olsTestBase.GetAt(pos);
		strTestType = oTestBaseData.m_strDesc;
		uiTestType = oTestBaseData.m_uiType;
		m_olsTestTypeInstrument.AddTail(strTestType);
		m_olsTestTypeAll.AddTail(strTestType);
		m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
		m_oTestTypeMapID.SetAt(uiTestType, strTestType);
	}
	strTestType = "ALL INSTRUMENT TEST";
	uiTestType = 30;
	m_olsTestTypeInstrument.AddTail(strTestType);
	m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
	m_oTestTypeMapID.SetAt(uiTestType, strTestType);
	for(i = 0; i < (m_oTestBaseListSensor.m_uiCountAll - 1); i++)
	{
		pos = m_oTestBaseListSensor.m_olsTestBase.FindIndex(i);
		oTestBaseData = m_oTestBaseListSensor.m_olsTestBase.GetAt(pos);
		strTestType = oTestBaseData.m_strDesc;
		uiTestType = oTestBaseData.m_uiType;
		m_olsTestTypeSensor.AddTail(strTestType);
		m_olsTestTypeAll.AddTail(strTestType);
		m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
		m_oTestTypeMapID.SetAt(uiTestType, strTestType);
	}
	pos = m_oTestBaseListSensor.m_olsTestBase.FindIndex(m_oTestBaseListSensor.m_uiCountAll - 1);
	oTestBaseData = m_oTestBaseListSensor.m_olsTestBase.GetAt(pos);
	strTestType = oTestBaseData.m_strDesc;
	uiTestType = oTestBaseData.m_uiType;
	m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
	m_oTestTypeMapID.SetAt(uiTestType, strTestType);

	strTestType = "ALL SENSOR TEST";
	uiTestType = 40;
	m_olsTestTypeSensor.AddTail(strTestType);
	m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
	m_oTestTypeMapID.SetAt(uiTestType, strTestType);

	strTestType = "ALL INSTRUMENT TEST";
	uiTestType = 30;
	m_olsTestTypeAll.AddTail(strTestType);
	strTestType = "ALL SENSOR TEST";
	uiTestType = 40;
	m_olsTestTypeAll.AddTail(strTestType);
	strTestType = "ALL TEST";
	uiTestType = 50;
	m_olsTestTypeAll.AddTail(strTestType);
	m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
	m_oTestTypeMapID.SetAt(uiTestType, strTestType);
	strTestType = "CONTROL LOOP";
	uiTestType = 60;
	m_olsTestTypeAll.AddTail(strTestType);
	m_oTestTypeMapString.SetAt(strTestType, (void*)uiTestType);
	m_oTestTypeMapID.SetAt(uiTestType, strTestType);
}

/**
* 清除TestType队列
* @param void
* @return void
*/
void CSiteData::CleanTestTypeList()
{
	m_olsTestTypeInstrument.RemoveAll();
	m_olsTestTypeSensor.RemoveAll();
	m_olsTestTypeAll.RemoveAll();
	m_oTestTypeMapString.RemoveAll();
	m_oTestTypeMapID.RemoveAll();
}

/**
* 得到TestType字符串
* @param unsigned int uiTestType TestType
* @return CString TestType字符串
*/
CString CSiteData::GetTestTypeStringByID(unsigned int uiTestType)
{
	CString strTestType;
	if(TRUE == m_oTestTypeMapID.Lookup(uiTestType, strTestType))
	{
		return strTestType;
	}
	return "";
}

/**
* 得到TestTypeID
* @param CString strTestType TestType字符串
* @return unsigned int TestTypeID 0：无对应TestTypeID
*/
unsigned int CSiteData::GetTestTypeIDByString(CString strTestType)
{
	void* uiTestType;
	if(TRUE == m_oTestTypeMapString.Lookup(strTestType, uiTestType))
	{
		return (unsigned int)uiTestType;
	}
	return 0;
}

/**
* 得到仪器TestType判据字符串
* @param unsigned int uiTestType TestType
* @return CString TestType字符串
*/
CString CSiteData::GetInsarumentTestTypeLimitStringByID(unsigned int uiTestType)
{
	CString strTestType = "";
	CTestLimitData oTestLimitData;
	unsigned int uiTestLimitNb = 0;
	switch(uiTestType)
	{
	case 4:	// 仪器噪声
		uiTestLimitNb = 400;
		break;
	case 5:	// 仪器畸变
		uiTestLimitNb = 500;
		break;
	case 7:	// 仪器串音
		uiTestLimitNb = 700;
		break;
	case 8:	// 仪器共模
		uiTestLimitNb = 800;
		break;
	}
	if(TRUE == m_oTestLimitListInstrument.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
	{
		strTestType.Format("%3.2f%s", oTestLimitData.m_fLimit, oTestLimitData.m_strUnit);
		return strTestType;
	}

	// 判断：增益/相位测试
	if(6 == uiTestType)
	{
		CString strTestTypeGain = "";
		CString strTestTypePhase = "";
		uiTestLimitNb = 600;
		if(TRUE == m_oTestLimitListInstrument.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
		{
			strTestTypeGain.Format("%s:%3.2f%s", oTestLimitData.m_strDesc, oTestLimitData.m_fLimit, oTestLimitData.m_strUnit);
		}
		uiTestLimitNb = 610;
		if(TRUE == m_oTestLimitListInstrument.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
		{
			strTestTypePhase.Format("%s:%3.2f%s", oTestLimitData.m_strDesc, oTestLimitData.m_fLimit, oTestLimitData.m_strUnit);
		}
		strTestType.Format("%s  %s", strTestTypeGain, strTestTypePhase);
		return strTestType;
	}

	return "";
}

/**
* 得到检波器TestType判据字符串
* @param unsigned int uiTestType TestType
* @return CString TestLimit字符串
*/
CString CSiteData::GetSensorTestTypeLimitStringByID(unsigned int uiTestType)
{
	CString strTestLimit = "";
	CTestLimitData oTestLimitData;
	unsigned int uiTestLimitNb = 0;
	switch(uiTestType)
	{
	case 14:	// 检波器漏电
		uiTestLimitNb = 1400;
		break;
	case 1:	// 检波器噪声
		uiTestLimitNb = 100;
		break;
	case 20:	// 检波器倾斜度
		uiTestLimitNb = 2000;
		break;
	case 21:	// 检波器倾斜度模型
		uiTestLimitNb = 2100;
		break;
	case 22:	// 检波器噪声监测
		uiTestLimitNb = 2200;
		break;
	}
	if(TRUE == m_oTestLimitListSensor.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
	{
		strTestLimit.Format("%3.2f%s", oTestLimitData.m_fLimit, oTestLimitData.m_strUnit);
		return strTestLimit;
	}
	// 判断：检波器电阻
	if(15 == uiTestType)
	{
		CString strTestLimitLow = "";
		CString strTestLimitHight = "";
		uiTestLimitNb = 1500;
		if(TRUE == m_oTestLimitListSensor.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
		{
			strTestLimitLow.Format("%3.2f", oTestLimitData.m_fLimit);
		}
		uiTestLimitNb = 1501;
		if(TRUE == m_oTestLimitListSensor.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
		{
			strTestLimitHight.Format("%3.2f%s", oTestLimitData.m_fLimit, oTestLimitData.m_strUnit);
			strTestLimit.Format("%s-%s", strTestLimitLow, strTestLimitHight);
		}
		return strTestLimit;
	}
	return "";
}

/**
* 得到噪声监测判据
* @param void
* @return float 噪声监测判据
*/
float CSiteData::GetSeismonitorLimit()
{
	float fTestLimit = -66.0;
	CTestLimitData oTestLimitData;
	if(TRUE == m_oTestLimitListSensor.m_oTestLimitDataMap.Lookup(2200, oTestLimitData))
	{
		fTestLimit = oTestLimitData.m_fLimit;
	}
	return fTestLimit;
}

/**
* 得到仪器TestType单位字符串
* @param unsigned int uiTestType TestType
* @param unsigned int iSubItem 测试项
* @return CString TestType单位字符串
*/
CString CSiteData::GetInsarumentTestTypeUnitStringByID(unsigned int uiTestType, int iSubItem)
{
	CString strTestUnit = "";
	CTestLimitData oTestLimitData;
	unsigned int uiTestLimitNb = 0;
	switch(uiTestType)
	{
	case 4:	// 仪器噪声
		uiTestLimitNb = 400;
		break;
	case 5:	// 仪器畸变
		uiTestLimitNb = 500;
		break;
	case 7:	// 仪器串音
		uiTestLimitNb = 700;
		break;
	case 6:	// 仪器增益/相位
		// 判断：增益
		if(1 == iSubItem)
		{
			uiTestLimitNb = 600;
		}
		// 判断：相位
		else if(2 == iSubItem)
		{
			uiTestLimitNb = 610;
		}
		break;
	case 8:	// 仪器共模
		uiTestLimitNb = 800;
		break;
	}
	if(TRUE == m_oTestLimitListInstrument.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
	{
		strTestUnit = oTestLimitData.m_strUnit;
	}
	return strTestUnit;
}

/**
* 得到检波器TestType单位字符串
* @param unsigned int uiTestType TestType
* @return CString TestType单位字符串
*/
CString CSiteData::GetSensorTestTypeUnitStringByID(unsigned int uiTestType)
{
	CString strTestUnit = "";
	CTestLimitData oTestLimitData;
	unsigned int uiTestLimitNb = 0;
	switch(uiTestType)
	{
	case 15:	// 检波器电阻
		uiTestLimitNb = 1500;
		break;
	case 14:	// 检波器漏电
		uiTestLimitNb = 1400;
		break;
	case 1:	// 检波器噪声
		uiTestLimitNb = 100;
		break;
	case 20:	// 检波器倾斜度
		uiTestLimitNb = 2000;
		break;
	case 21:	// 检波器倾斜度模型
		uiTestLimitNb = 2100;
		break;
	case 22:	// 检波器噪声监测
		uiTestLimitNb = 2200;
		break;
	}
	if(TRUE == m_oTestLimitListSensor.m_oTestLimitDataMap.Lookup(uiTestLimitNb, oTestLimitData))
	{
		strTestUnit = oTestLimitData.m_strUnit;
	}
	return strTestUnit;
}

// 初始化复合测试名称队列
void CSiteData::InitMultipleTestNameList()
{
	POSITION pos = NULL;
	CTestProject* pTestProject;
	for(unsigned int i = 0; i < m_oTestProjectListMultiple.m_uiCountAll; i++)
	{
		pos = m_oTestProjectListMultiple.m_olsTestProject.FindIndex(i);
		pTestProject = m_oTestProjectListMultiple.m_olsTestProject.GetAt(pos);
		m_olsMultipleTestName.AddTail(pTestProject->m_strTestName);
	}
}

// 清除MultipleTestName队列
void CSiteData::CleanMultipleTestNameList()
{
	m_olsMultipleTestName.RemoveAll();
}

/**
* 由FTP服务器得到Matrix设置文件
* @param void
* @return bool true：成功；false：失败
*/
bool CSiteData::GetMatrixIniFileFromFTPServer()
{
	bool bReturn = false;
	BOOL bData;
	int iCount = 0;
	CInternetSession oSession;
	CFtpConnection* pConnection = oSession.GetFtpConnection(m_strIPFTPServer);
	
	while(true)
	{
		bData = pConnection->GetFile("\\parameter\\Matrix.xml", "..\\parameter\\Matrix.xml", FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD);
		if(TRUE == bData)
		{
			bReturn = true;
			break;
		}
		iCount++;
		if(4 <= iCount)
		{
			break;
		}
		Sleep(50);
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}

/**
* 保存Matrix设置文件到FTP服务器
* @param void
* @return bool true：成功；false：失败
*/
bool CSiteData::SaveMatrixIniFileToFTPServer()
{
	bool bReturn = false;
	BOOL bData;
	int iCount = 0;
	CInternetSession oSession;
	CFtpConnection* pConnection = oSession.GetFtpConnection(m_strIPFTPServer);
	
	while(true)
	{
		bData = pConnection->PutFile("..\\parameter\\Matrix.xml", "parameter\\Matrix.xml");
		if(TRUE == bData)
		{
			bReturn = true;
			break;
		}
		iCount++;
		if(4 <= iCount)
		{
			break;
		}
		Sleep(50);
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}

/**
* 由FTP服务器得到数据文件
* @param void
* @return bool true：成功；false：失败
*/
bool CSiteData::GetDataFileFromFTPServer()
{
	bool bReturn = false;
	CInternetSession oSession;
	BOOL bData;
	CFtpConnection* pConnection = oSession.GetFtpConnection(m_strIPFTPServer);

	bData = GetDataFileFromFTPServer(pConnection, "\\data\\FileInstrument.dat", "..\\data\\FileInstrument.dat");
	if(TRUE == bData)
	{
		bData = GetDataFileFromFTPServer(pConnection, "\\data\\FileRout.dat", "..\\data\\FileRout.dat");
		if(TRUE == bData)
		{
			bData = GetDataFileFromFTPServer(pConnection, "\\data\\FileChannel.dat", "..\\data\\FileChannel.dat");
			if(TRUE == bData)
			{
				bReturn = true;
			}
		}
	}
	pConnection->Close();
	delete pConnection;
	return bReturn;
}

/**
* 由FTP服务器得到数据文件
* @param CFtpConnection* pConnection FTP链接
* @param CString strFileSource 源文件名
* @param CString strFileAim 目的文件名
* @return bool true：成功；false：失败
*/
bool CSiteData::GetDataFileFromFTPServer(CFtpConnection* pConnection, CString strFileSource, CString strFileAim)
{
	BOOL bData;
	int iCount = 0;
	while(true)
	{
		bData = pConnection->GetFile(strFileSource, strFileAim, FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD);
		if(TRUE == bData)
		{
			return true;
		}
		iCount++;
		if(4 <= iCount)
		{
			return false;
		}
		Sleep(50);
	}
	return false;
}

/**
* 加载现场数据
* @param void
* @return void
*/
void CSiteData::LoadSiteData()
{
	// 由FTP服务器得到数据文件
	GetDataFileFromFTPServer();

	// 加载现场仪器数据队列
	m_oInstrumentList.LoadSiteData();
	// 加载路由队列
	m_oRoutList.LoadSiteData();
	// 加载现场测道数据队列
	m_oChannelList.LoadSiteData();

	// 加载仪器图形视图仪器图形单元队列
	m_oGraphInstrumentList.LoadSiteData();
	// 加载检波器图形视图检波器图形单元队列
	m_oGraphSensorList.LoadSiteData();
}

/**
* 处理命令	100-现场数据输出更新通知命令
* @param void
* @return void
*/
void CSiteData::ProcInterSiteDataOutputCMD()
{
	OnReset();

	// 由FTP服务器得到数据文件
	GetDataFileFromFTPServer();

	// 加载现场仪器数据队列
	m_oInstrumentList.LoadSiteData();
	// 加载路由队列
	m_oRoutList.LoadSiteData();
	// 加载现场测道数据队列
	m_oChannelList.LoadSiteData();

	// 加载仪器图形视图仪器图形单元队列
	m_oGraphInstrumentList.LoadSiteData();
	// 加载检波器图形视图检波器图形单元队列
	m_oGraphSensorList.LoadSiteData();
}

/**
* 打开程序配置文件
* @param void
* @return void
*/
void CSiteData::OpenAppIniXMLFile()
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = "msxml2.domdocument";
	BOOL bData = m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = "..\\parameter\\MatrixLineApp.xml";
	bData = m_oXMLDOMDocument.load(oVariant);
}

/**
* 关闭程序配置文件
* @param void
* @return void
*/
void CSiteData::CloseAppIniXMLFile()
{
	m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}

/**
* 打开配置文件
* @param void
* @return void
*/
void CSiteData::OpenMatrixIniXMLFile()
{
	// 初始化COM库
	CoInitialize(NULL);

	CString strOLEObject;
	COleException oError;
	COleVariant oVariant;

	strOLEObject = "msxml2.domdocument";
	BOOL bData = m_oXMLDOMDocument.CreateDispatch(strOLEObject, &oError);

	oVariant = "..\\parameter\\Matrix.xml";
	bData = m_oXMLDOMDocument.load(oVariant);
}

/**
* 关闭配置文件
* @param void
* @return void
*/
void CSiteData::CloseMatrixIniXMLFile()
{
	m_oXMLDOMDocument.DetachDispatch();
	// 释放COM库
	CoUninitialize();
}

/**
* 保存配置文件
* @param void
* @return void
*/
void CSiteData::SaveMatrixIniXMLFile()
{
	COleVariant oVariant;
	oVariant = "..\\parameter\\Matrix.xml";
	m_oXMLDOMDocument.save(oVariant);
}

/**
* 加载测线服务器程序设置数据
* @param void
* @return void
*/
void CSiteData::LoadLineServerAppSetupData()
{
	// 打开程序配置文件
	OpenAppIniXMLFile();

	//加载IP地址设置数据
	LoadIPSetupData();
	//加载端口设置数据
	LoadPortSetupData();
	//加载数据缓冲区大小数据
	LoadBufferSizeSetupData();

	// 关闭程序配置文件
	CloseAppIniXMLFile();
}

/**
* 加载IP地址设置数据
* @param void
* @return void
*/
void CSiteData::LoadIPSetupData()
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到IP地址设置区
	strKey = "LineClientAppIPSetup";
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = "IPFTPServer";
	m_strIPFTPServer = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);

	strKey = "IPForInterface";
	m_strIPForInterface = CXMLDOMTool::GetElementAttributeString(&oElement, strKey);
}

/**
* 加载端口设置数据
* @param void
* @return void
*/
void CSiteData::LoadPortSetupData(void)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到IP地址设置区
	strKey = "LineClientAppPortSetup";
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = "PortForCmdRev";
	m_uiPortForCmdRev = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "PortForCmdSend";
	m_uiPortForCmdSend = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "PortForCmdSendToServer";
	m_uiPortForCmdSendToServer = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "PortForCmdSendToOperation";
	m_uiPortForCmdSendToOperation = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "PortForDataRev";
	m_uiPortForDataRev = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}

/**
* 加载数据缓冲区大小数据
* @param void
* @return void
*/
void CSiteData::LoadBufferSizeSetupData(void)
{
	CString strKey;
	CXMLDOMNodeList oNodeList;
	CXMLDOMElement oElement;
	LPDISPATCH lpDispatch;

	// 找到数据缓冲区大小设置区
	strKey = "LineClientAppBufferSizeSetup";
	lpDispatch = m_oXMLDOMDocument.getElementsByTagName(strKey);
	oNodeList.AttachDispatch(lpDispatch);
	// 找到入口
	lpDispatch = oNodeList.get_item(0);
	oElement.AttachDispatch(lpDispatch);

	strKey = "InstrumentCountAll";
	m_uiInstrumentCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
	strKey = "ChannelCountAll";
	m_uiChannelCountAll = CXMLDOMTool::GetElementAttributeUnsignedInt(&oElement, strKey);
}

/**
* 加载地形设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadSurveySetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 测线设置队列
		m_oLineList.OnReload();
		// 点代码设置队列
		m_oPointCodeList.OnReload();
		// 检波器设置队列
		m_oSensorList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 保存地形设置数据；ApplyAll
* @param void
* @return void
*/
void CSiteData::OnApplyAllSurveySetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 测线设置队列
		m_oLineList.OnSave();
		// 点代码设置队列
		m_oPointCodeList.OnSave();
		// 检波器设置队列
		m_oSensorList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 保存地形设置数据；ApplySensor
* @param void
* @return void
*/
void CSiteData::OnApplySensorSurveySetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
	// 检波器设置队列
	m_oSensorList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送地形设置ApplyAll通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyAllSurveySetupData()
{
	// 命令	20-测线配置文件更改命令；更改的目标	1-SurveySetup的ApplyAll
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(1);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送地形设置ApplySensor通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplySensorSurveySetupData()
{
	// 命令	20-测线配置文件更改命令；更改的目标	2-SurveySetup的ApplySensor
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(2);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载布局设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLayoutSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 标记点设置队列
		m_oMarkerList.OnReload();
		// 辅助道设置队列
		m_oAuxList.OnReload();
		// 爆炸机控制器设置队列
		m_oBlastMachineList.OnReload();
		// 迂回道设置队列
		m_oDetourList.OnReload();
		// 哑点设置队列
		m_oMuteList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载标记点设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLayoutSetupDataOfMarker()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 标记点设置队列
		m_oMarkerList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载辅助道设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLayoutSetupDataOfAux()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 辅助道设置队列
		m_oAuxList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载爆炸机控制器设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLayoutSetupDataOfBlastMachine()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 爆炸机控制器设置队列
		m_oBlastMachineList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载迂回道设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLayoutSetupDataOfDetour()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 迂回道设置队列
		m_oDetourList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载哑点设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLayoutSetupDataOfMute()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 哑点设置队列
		m_oMuteList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* Apply标记点设置数据
* @param void
* @return void
*/
void CSiteData::OnApplyLayoutSetupDataOfMarker()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 标记点设置队列
		m_oMarkerList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply辅助道设置数据
* @param void
* @return void
*/
void CSiteData::OnApplyLayoutSetupDataOfAux()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 辅助道设置队列
		m_oAuxList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();

	::PostMessage(HWND_BROADCAST, WM_NEWXMLFILE , MATRIX_LINE, MATRIX_LINE_AUXI);
}

/**
* Apply爆炸机控制器设置数据
* @param void
* @return void
*/
void CSiteData::OnApplyLayoutSetupDataOfBlastMachine()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 爆炸机控制器设置队列
		m_oBlastMachineList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply迂回道设置数据
* @param void
* @return void
*/
void CSiteData::OnApplyLayoutSetupDataOfDetour()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 迂回道设置队列
		m_oDetourList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply哑点设置数据
* @param void
* @return void
*/
void CSiteData::OnApplyLayoutSetupDataOfMute()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 哑点设置队列
		m_oMuteList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送标记点设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyLayoutSetupDataOfMarker()
{
	// 命令	20-测线配置文件更改命令；更改的目标	12-LayoutSetup的ApplyMarkers
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(12);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送辅助道设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyLayoutSetupDataOfAux()
{
	// 命令	20-测线配置文件更改命令；更改的目标	13-LayoutSetup的ApplyAux
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(13);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送爆炸机控制器设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyLayoutSetupDataOfBlastMachine()
{
	// 命令	20-测线配置文件更改命令；更改的目标	11-LayoutSetup的ApplyBlast
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(11);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送迂回道设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyLayoutSetupDataOfDetour()
{
	// 命令	20-测线配置文件更改命令；更改的目标	14-LayoutSetup的ApplyDetour
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(14);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送哑点设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyLayoutSetupDataOfMute()
{
	// 命令	20-测线配置文件更改命令；更改的目标	15-LayoutSetup的ApplyMute
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(15);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载排列设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadSpreadSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 绝对排列设置队列
		m_oSpreadAbsoluteList.OnReload();
		// 相对排列设置队列
		m_oSpreadGenericList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}
/**
* 加载炮点和排列的对应关系数据
* @param void
* @return void
*/
void CSiteData::OnLoadShotPointSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 炮点设置索引
		m_oShotPointMap.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}
/**
* 加载绝对排列设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadSpreadSetupDataForAbsolute()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 绝对排列设置队列
		m_oSpreadAbsoluteList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载相对排列设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadSpreadSetupDataForGeneric()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 相对排列设置队列
		m_oSpreadGenericList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* Apply绝对排列设置数据
* @param void
* @return void
*/
void CSiteData::OnApplySpreadSetupDataForAbsolute()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 绝对排列设置队列
		m_oSpreadAbsoluteList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();

	::PostMessage(HWND_BROADCAST, WM_NEWXMLFILE , MATRIX_LINE, MATRIX_LINE_SPREAD);
}

/**
* Apply相对排列设置数据
* @param void
* @return void
*/
void CSiteData::OnApplySpreadSetupDataForGeneric()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 相对排列设置队列
		m_oSpreadGenericList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();

	::PostMessage(HWND_BROADCAST, WM_NEWXMLFILE , MATRIX_LINE, MATRIX_LINE_SPREAD);
}

/**
* 发送测试绝对排列设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplySpreadSetupDataForAbsolute()
{
	// 命令	20-测线配置文件更改命令；更改的目标	61-SpreadSetup的ApplySpreadSetupDataForAbsolute
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(61);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送测试相对排列设置数据变化通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplySpreadSetupDataForGeneric()
{
	// 命令	20-测线配置文件更改命令；更改的目标	62-SpreadSetup的ApplySpreadSetupDataForGeneric
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(62);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载测试基本设置数据，仪器
* @param void
* @return void
*/
void CSiteData::OnLoadTestBaseSetupDataForInstrument()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestBaseListInstrument.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载测试基本设置数据，检波器
* @param void
* @return void
*/
void CSiteData::OnLoadTestBaseSetupDataForSensor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestBaseListSensor.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* Apply测试基本设置数据，仪器
* @param void
* @return void
*/
void CSiteData::OnApplyTestBaseSetupDataForInstrument()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestBaseListInstrument.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply测试基本设置数据，检波器
* @param void
* @return void
*/
void CSiteData::OnApplyTestBaseSetupDataForSensor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestBaseListSensor.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送测试基本设置数据变化通知命令帧，仪器
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestBaseSetupDataOfInstrument()
{
	// 命令	20-测线配置文件更改命令；更改的目标	41-TestSetup的ApplyInstrumentTestBase
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(41);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送测试基本设置数据变化通知命令帧，检波器
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestBaseSetupDataOfSensor()
{
	// 命令	20-测线配置文件更改命令；更改的目标	42-TestSetup的ApplySensorTestBase
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(42);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载测试判据设置数据，仪器
* @param void
* @return void
*/
void CSiteData::OnLoadTestLimitSetupDataForInstrument()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestLimitListInstrument.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载测试判据设置数据，检波器
* @param void
* @return void
*/
void CSiteData::OnLoadTestLimitSetupDataForSensor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestLimitListSensor.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* Apply测试判据设置数据，仪器
* @param void
* @return void
*/
void CSiteData::OnApplyTestLimitSetupDataForInstrument()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestLimitListInstrument.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply测试判据设置数据，检波器
* @param void
* @return void
*/
void CSiteData::OnApplyTestLimitSetupDataForSensor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestLimitListSensor.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送测试判据设置数据变化通知命令帧，仪器
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestLimitSetupDataOfInstrument()
{
	// 命令	20-测线配置文件更改命令；更改的目标	41-TestSetup的ApplyInstrumentTestLimit
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(51);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送测试判据设置数据变化通知命令帧，检波器
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestLimitSetupDataOfSensor()
{
	// 命令	20-测线配置文件更改命令；更改的目标	42-TestSetup的ApplySensorTestLimit
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(52);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载测试设置数据，仪器测试
* @param void
* @return void
*/
void CSiteData::OnLoadTestSetupDataForInstrument()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListInstrument.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载测试设置数据，检波器测试
* @param void
* @return void
*/
void CSiteData::OnLoadTestSetupDataForSensor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListSensor.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载测试设置数据，复合测试
* @param void
* @return void
*/
void CSiteData::OnLoadTestSetupDataForMultiple()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListMultiple.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 加载测试设置数据，噪声监测
* @param void
* @return void
*/
void CSiteData::OnLoadTestSetupDataForSeisMonitor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListSeisMonitor.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* Apply测试设置数据，仪器测试
* @param void
* @return void
*/
void CSiteData::OnApplyTestSetupDataForInstrument()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListInstrument.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply测试设置数据，检波器测试
* @param void
* @return void
*/
void CSiteData::OnApplyTestSetupDataForSensor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListSensor.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply测试设置数据，复合测试
* @param void
* @return void
*/
void CSiteData::OnApplyTestSetupDataForMultiple()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListMultiple.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* Apply测试设置数据，噪声监测
* @param void
* @return void
*/
void CSiteData::OnApplyTestSetupDataForSeisMonitor()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		m_oTestProjectListSeisMonitor.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送测试设置数据变化通知命令帧，仪器测试
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestSetupDataOfInstrument()
{
	// 命令	20-测线配置文件更改命令；更改的目标	31-TestSetup的ApplyInstrument
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(31);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送测试设置数据变化通知命令帧，检波器测试
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestSetupDataOfSensor()
{
	// 命令	20-测线配置文件更改命令；更改的目标	32-TestSetup的ApplySensor
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(32);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送测试设置数据变化通知命令帧，复合测试
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestSetupDataOfMultiple()
{
	// 命令	20-测线配置文件更改命令；更改的目标	33-TestSetup的ApplyMultiple
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(33);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送测试设置数据变化通知命令帧，噪声监测
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyTestSetupDataOfSeisMonitor()
{
	// 命令	20-测线配置文件更改命令；更改的目标	34-TestSetup的ApplySeisMonitor
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(34);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 发送TestSetup操作GO通知命令帧
* @param unsigned short usCommand 命令 4-Test From InstrumentTestSetup；5-Test From SensorTestSetup；6-Test From MultipleTestSetup；7-Test From SeismonitorTestSetup
* @param UINT uiTestNb 测试索引号
* @return void
*/
void CSiteData::OnSendCMDFrameForTestSetupGO(unsigned short usCommand, UINT uiTestNb)
{
	// 命令 4-Test From InstrumentTestSetup；5-Test From SensorTestSetup；6-Test From MultipleTestSetup；7-Test From SeismonitorTestSetup
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeTestCmdFrameForTestSetup(usCommand, uiTestNb);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载FormLine设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadFormLineSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 重新加载
		m_oFormLineList.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 保存FormLine设置数据；Apply
* @param void
* @return void
*/
void CSiteData::OnApplyFormLineSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 保存FormLine设置
		m_oFormLineList.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送FormLine操作GO通知命令帧
* @param unsigned int uiSN 仪器SN号
* @return void
*/
void CSiteData::OnSendFormLineCMDFrameGO(unsigned int uiSN)
{
	// 命令	2-FormLine
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeFormLineFrame(uiSN);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 加载Look设置数据
* @param void
* @return void
*/
void CSiteData::OnLoadLookSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 重新加载
		m_oLookSetupData.OnReload();
	// 关闭配置文件
	CloseMatrixIniXMLFile();
}

/**
* 保存Look设置数据；Apply
* @param void
* @return void
*/
void CSiteData::OnApplyLookSetupData()
{
	// 打开配置文件
	OpenMatrixIniXMLFile();
		// 保存Look设置
		m_oLookSetupData.OnSave();
	// 保存配置文件
	SaveMatrixIniXMLFile();
	// 关闭配置文件
	CloseMatrixIniXMLFile();

	// 保存Matrix设置文件到FTP服务器
	SaveMatrixIniFileToFTPServer();
}

/**
* 发送Look设置Apply通知命令帧
* @param void
* @return void
*/
void CSiteData::OnSendCMDFrameForApplyLookSetupData()
{
	// 命令	20-测线配置文件更改命令；更改的目标	21-LookSetup的Apply
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeSiteDataOutputCmdFrame(21);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 响应仪器、检波器图形视图测试GO按钮
* @param unsigned unsigned int uiTestAim 测试对象	1-仪器；2-检波器
* @param unsigned unsigned int uiTestType 测试类型
* @param unsigned CString strTestAbsoluteSpread 绝对排列定义
* @return void
*/
void CSiteData::OnSendCMDFrameForGraphViewInstrumentOrSensorTestGOButton(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread)
{
	// 生成测试命令帧，响应仪器、检波器图形视图测试GO按钮
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeTestCmdFrameForGraphViewGOButton(uiTestAim, uiTestType, strTestAbsoluteSpread);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}

/**
* 响应噪声监测图形视图Seismonitor按钮
* @param byte byTestOperation 操作 1-启动测试；0-停止测试
* @param unsigned CString strTestAbsoluteSpread 绝对排列定义
* @return void
*/
void CSiteData::OnSendCMDFrameForGraphViewNoiseSeismonitorButton(byte byTestOperation, CString strTestAbsoluteSpread)
{
	// 生成测试命令帧，响应噪声监测图形视图Seismonitor按钮
	m_oSocketInterfaceCmdSend.m_oFrameInterface.MakeTestCmdFrameForGraphViewSeismonitorButton(byTestOperation, strTestAbsoluteSpread);
	m_oSocketInterfaceCmdSend.SendCommandFrame();
}