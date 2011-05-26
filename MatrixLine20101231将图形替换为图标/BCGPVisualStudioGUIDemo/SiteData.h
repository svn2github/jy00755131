#pragma once
#include "instrumentlist.h"
#include "ChannelList.h"
#include "RoutList.h"

#include "LineList.h"
#include "PointCodeList.h"
#include "SensorList.h"

#include "MarkerList.h"
#include "AuxList.h"
#include "BlastMachineList.h"
#include "DetourList.h"
#include "MuteList.h"

#include "SpreadAbsoluteList.h"
#include "SpreadGenericList.h"

#include "TestBaseList.h"
#include "TestLimitList.h"

#include "TestProjectList.h"

#include "FormLineList.h"

#include "LookSetupData.h"

#include "GraphInstrumentList.h"
#include "GraphSensorList.h"

#include "SocketInterfaceCmdRev.h"
#include "SocketInterfaceCmdSend.h"
#include "SocketInterfaceDataRev.h"

/**
现场数据和测线设置数据类
*/
class CSiteData
{
public:
	CSiteData(void);
	~CSiteData(void);

public: // 属性
	/** SegdCode队列*/
	CList<CString, CString> m_olsSegdCode;
	/** BoxType队列*/
	CList<CString, CString> m_olsBoxType;
	/** Gain队列*/
	CList<CString, CString> m_olsGain;
	/** Orientation队列*/
	CList<CString, CString> m_olsOrientation;
	/** 滤波器类型队列*/
	CList<CString, CString> m_olsFilterType;
	/** ADC类型队列*/
	CList<CString, CString> m_olsADCType;
	/** DAC类型队列*/
	CList<CString, CString> m_olsDACType;
	/** 复合测试名称队列*/
	CList<CString, CString> m_olsMultipleTestName;

	/** 测试类型队列，仪器*/
	CList<CString, CString> m_olsTestTypeInstrument;
	/** 测试类型队列，检波器*/
	CList<CString, CString> m_olsTestTypeSensor;
	/** 测试类型队列，所有*/
	CList<CString, CString> m_olsTestTypeAll;
	/** 测试类型索引表，字符串*/
	CMapStringToPtr m_oTestTypeMapString;
	/** 测试类型索引表，字符串*/
	CMap<unsigned int, unsigned int, CString, CString> m_oTestTypeMapID;

	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;

	/** 仪器队列仪器总数*/
	unsigned int m_uiInstrumentCountAll;
	/** 仪器队列*/
	CInstrumentList m_oInstrumentList;
	/** 参加测试仪器队列*/
	CList<unsigned int, unsigned int> m_olsIPInstrument;

	/** 测道总数*/
	unsigned int m_uiChannelCountAll;
	/** 测道队列*/
	CChannelList m_oChannelList;
	/** 参加测试检波器队列*/
	CList<unsigned int, unsigned int> m_olsIPSensor;
	/** 参加噪声监测检波器队列*/
	CList<unsigned int, unsigned int> m_olsIPSeismonitor;

	/** 路由队列*/
	CRoutList m_oRoutList;

	/** 检波器图形视图检波器图形单元队列*/
	CGraphSensorList m_oGraphSensorList;
	/** 仪器图形视图仪器图形单元队列*/
	CGraphInstrumentList m_oGraphInstrumentList;

	/** 测线设置队列*/
	CLineList m_oLineList;
	/** 点代码设置队列*/
	CPointCodeList m_oPointCodeList;
	/** 检波器设置队列*/
	CSensorList m_oSensorList;

	/** 标记点设置队列*/
	CMarkerList m_oMarkerList;
	/** 辅助道设置队列*/
	CAuxList m_oAuxList;
	/** 爆炸机控制器设置队列*/
	CBlastMachineList m_oBlastMachineList;
	/** 迂回道设置队列*/
	CDetourList m_oDetourList;
	/** 哑点设置队列*/
	CMuteList m_oMuteList;

	/** 绝对排列设置队列*/
	CSpreadAbsoluteList m_oSpreadAbsoluteList;
	/** 相对排列设置队列*/
	CSpreadGenericList m_oSpreadGenericList;

	/** 测试基本设置对象，仪器*/
	CTestBaseList m_oTestBaseListInstrument;
	/** 测试基本设置对象，检波器*/
	CTestBaseList m_oTestBaseListSensor;

	/** 测试判据设置对象，仪器*/
	CTestLimitList m_oTestLimitListInstrument;
	/** 测试判据设置对象，检波器*/
	CTestLimitList m_oTestLimitListSensor;

	/** 测试设置对象，仪器测试*/
	CTestProjectList m_oTestProjectListInstrument;
	/** 测试设置对象，检波器测试*/
	CTestProjectList m_oTestProjectListSensor;
	/** 测试设置对象，复合测试*/
	CTestProjectList m_oTestProjectListMultiple;
	/** 测试设置对象，噪声监测*/
	CTestProjectList m_oTestProjectListSeisMonitor;

	/** FormLine设置队列*/
	CFormLineList m_oFormLineList;

	/** Look设置对象*/
	CLookSetupData m_oLookSetupData;

	/** 客户端内部网络命令接收处理网络对象*/
	CSocketInterfaceCmdRev m_oSocketInterfaceCmdRev;
	/** 客户端内部网络命令发送处理网络对象*/
	CSocketInterfaceCmdSend m_oSocketInterfaceCmdSend;
	/** 客户端内部网络数据接收处理网络对象*/
	CSocketInterfaceDataRev m_oSocketInterfaceDataRev;
	/** 和内部网络通讯的本机IP地址*/
	CString m_strIPForInterface;
	/** 用于客户端内部网络命令接收的信宿端口号*/
	UINT m_uiPortForCmdRev;
	/** 用于客户端内部网络命令发送的信源端口号*/
	UINT m_uiPortForCmdSend;
	/** 用于服务器内部网络命令发送的信宿端口号*/
	UINT m_uiPortForCmdSendTo;
	/** 用于客户端内部网络数据接收的信宿端口号*/
	UINT m_uiPortForDataRev;

	/** FTP服务器IP地址*/
	CString m_strIPFTPServer;

public: // 方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 关闭
	void OnClose();

	// 初始化SegdCode队列
	void InitSegdCodeList();
	// 清除SegdCode队列
	void CleanSegdCodeList();
	// 得到SegdCode字符串
	CString GetSegdCodeStringByID(unsigned int uiSegdCode);
	// 得到SegdCodeID
	unsigned int GetSegdCodeIDByString(CString strSegdCode);

	// 初始化BoxType队列
	void InitBoxTypeList();
	// 清除BoxType队列
	void CleanBoxTypeList();
	// 得到BoxType字符串
	CString GetBoxTypeStringByID(unsigned int uiBoxType);
	// 得到BoxTypeID
	unsigned int GetBoxTypeIDByString(CString strBoxType);

	// 初始化Gain队列
	void InitGainList();
	// 清除Gain队列
	void CleanGainList();
	// 得到Gain字符串
	CString GetGainStringByGainData(unsigned int uiGain);
	// 得到Gain
	unsigned int GetGainByString(CString strGain);

	// 初始化Orientation队列
	void InitOrientationList();
	// 清除Orientation队列
	void CleanOrientationList();
	// 得到Orientation字符串
	CString GetOrientationStringByID(unsigned int uiOrientation);
	// 得到OrientationID
	unsigned int GetOrientationIDByString(CString strOrientation);

	// 初始化FilterType队列
	void InitFilterTypeList();
	// 清除FilterType队列
	void CleanFilterTypeList();
	// 得到FilterType字符串
	CString GetFilterTypeStringByID(unsigned int uiFilterType);
	// 得到FilterTypeID
	unsigned int GetFilterTypeIDByString(CString strFilterType);

	// 初始化ADCType队列
	void InitADCTypeList();
	// 清除ADCType队列
	void CleanADCTypeList();
	// 得到ADCType字符串
	CString GetADCTypeStringByID(unsigned int uiADCType);
	// 得到ADCTypeID
	unsigned int GetADCTypeIDByString(CString strADCType);

	// 初始化DACType队列
	void InitDACTypeList();
	// 清除DACType队列
	void CleanDACTypeList();
	// 得到DACType字符串
	CString GetDACTypeStringByID(unsigned int uiDACType);
	// 得到DACTypeID
	unsigned int GetDACTypeIDByString(CString strDACType);

	// 初始化TestType队列
	void InitTestTypeList();
	// 清除TestType队列
	void CleanTestTypeList();
	// 得到TestType字符串
	CString GetTestTypeStringByID(unsigned int uiTestType);
	// 得到TestTypeID
	unsigned int GetTestTypeIDByString(CString strTestType);
	// 得到仪器TestType判据字符串
	CString GetInsarumentTestTypeLimitStringByID(unsigned int uiTestType);
	// 得到检波器TestType判据字符串
	CString GetSensorTestTypeLimitStringByID(unsigned int uiTestType);
	// 得到噪声监测判据
	float GetSeismonitorLimit();

	// 初始化复合测试名称队列
	void InitMultipleTestNameList();
	// 清除MultipleTestName队列
	void CleanMultipleTestNameList();

	// 打开程序配置文件
	void OpenAppIniXMLFile();
	// 关闭程序配置文件
	void CloseAppIniXMLFile();

	// 打开配置文件
	void OpenMatrixIniXMLFile();
	// 关闭配置文件
	void CloseMatrixIniXMLFile();
	// 保存配置文件
	void SaveMatrixIniXMLFile();

	//加载测线服务器程序设置数据
	void LoadLineServerAppSetupData();
	//加载IP地址设置数据
	void LoadIPSetupData();
	//加载端口设置数据
	void LoadPortSetupData();
	//加载数据缓冲区大小数据
	void LoadBufferSizeSetupData();

	// 由FTP服务器得到Matrix设置文件
	bool GetMatrixIniFileFromFTPServer();
	// 保存Matrix设置文件到FTP服务器
	bool SaveMatrixIniFileToFTPServer();

	// 由FTP服务器得到数据文件
	bool GetDataFileFromFTPServer();
	// 由FTP服务器得到数据文件
	bool GetDataFileFromFTPServer(CFtpConnection* pConnection, CString strFile);

	// 加载现场数据
	void LoadSiteData();
	// 处理命令	100-现场数据输出更新通知命令
	void ProcInterSiteDataOutputCMD();

	// 加载地形设置数据
	void OnLoadSurveySetupData();
	// 保存地形设置数据；ApplyAll
	void OnApplyAllSurveySetupData();
	// 保存地形设置数据；ApplySensor
	void OnApplySensorSurveySetupData();
	// 发送地形设置ApplyAll通知命令帧
	void OnSendCMDFrameForApplyAllSurveySetupData();
	// 发送地形设置ApplySensor通知命令帧
	void OnSendCMDFrameForApplySensorSurveySetupData();

	// 加载布局设置数据
	void OnLoadLayoutSetupData();
	// 加载标记点设置数据
	void OnLoadLayoutSetupDataOfMarker();
	// 加载辅助道设置数据
	void OnLoadLayoutSetupDataOfAux();
	// 加载爆炸机控制器设置数据
	void OnLoadLayoutSetupDataOfBlastMachine();
	// 加载迂回道设置数据
	void OnLoadLayoutSetupDataOfDetour();
	// 加载哑点设置数据
	void OnLoadLayoutSetupDataOfMute();	

	// Apply标记点设置数据
	void OnApplyLayoutSetupDataOfMarker();
	// Apply辅助道设置数据
	void OnApplyLayoutSetupDataOfAux();
	// Apply爆炸机控制器设置数据
	void OnApplyLayoutSetupDataOfBlastMachine();
	// Apply迂回道设置数据
	void OnApplyLayoutSetupDataOfDetour();
	// Apply哑点设置数据
	void OnApplyLayoutSetupDataOfMute();	

	// 发送标记点设置数据变化通知命令帧
	void OnSendCMDFrameForApplyLayoutSetupDataOfMarker();
	// 发送辅助道设置数据变化通知命令帧
	void OnSendCMDFrameForApplyLayoutSetupDataOfAux();
	// 发送爆炸机控制器设置数据变化通知命令帧
	void OnSendCMDFrameForApplyLayoutSetupDataOfBlastMachine();
	// 发送迂回道设置数据变化通知命令帧
	void OnSendCMDFrameForApplyLayoutSetupDataOfDetour();
	// 发送哑点设置数据变化通知命令帧
	void OnSendCMDFrameForApplyLayoutSetupDataOfMute();	

	// 加载排列设置数据
	void OnLoadSpreadSetupData();
	// 加载绝对排列设置数据
	void OnLoadSpreadSetupDataForAbsolute();
	// 加载相对排列设置数据
	void OnLoadSpreadSetupDataForGeneric();

	// Apply绝对排列设置数据
	void OnApplySpreadSetupDataForAbsolute();
	// Apply相对排列设置数据
	void OnApplySpreadSetupDataForGeneric();

	// 发送测试绝对排列设置数据变化通知命令帧
	void OnSendCMDFrameForApplySpreadSetupDataForAbsolute();	
	// 发送测试相对排列设置数据变化通知命令帧
	void OnSendCMDFrameForApplySpreadSetupDataForGeneric();	

	// 加载测试基本设置数据，仪器
	void OnLoadTestBaseSetupDataForInstrument();
	// 加载测试基本设置数据，检波器
	void OnLoadTestBaseSetupDataForSensor();

	// Apply测试基本设置数据，仪器
	void OnApplyTestBaseSetupDataForInstrument();
	// Apply测试基本设置数据，检波器
	void OnApplyTestBaseSetupDataForSensor();

	// 发送测试基本设置数据变化通知命令帧，仪器
	void OnSendCMDFrameForApplyTestBaseSetupDataOfInstrument();	
	// 发送测试基本设置数据变化通知命令帧，检波器
	void OnSendCMDFrameForApplyTestBaseSetupDataOfSensor();	

	// 加载测试判据设置数据，仪器
	void OnLoadTestLimitSetupDataForInstrument();
	// 加载测试判据设置数据，检波器
	void OnLoadTestLimitSetupDataForSensor();

	// Apply测试判据设置数据，仪器
	void OnApplyTestLimitSetupDataForInstrument();
	// Apply测试判据设置数据，检波器
	void OnApplyTestLimitSetupDataForSensor();

	// 发送测试判据设置数据变化通知命令帧，仪器
	void OnSendCMDFrameForApplyTestLimitSetupDataOfInstrument();	
	// 发送测试判据设置数据变化通知命令帧，检波器
	void OnSendCMDFrameForApplyTestLimitSetupDataOfSensor();	

	// 加载测试设置数据，仪器测试
	void OnLoadTestSetupDataForInstrument();
	// 加载测试设置数据，检波器测试
	void OnLoadTestSetupDataForSensor();
	// 加载测试设置数据，复合测试
	void OnLoadTestSetupDataForMultiple();
	// 加载测试设置数据，噪声监测
	void OnLoadTestSetupDataForSeisMonitor();		

	// Apply测试设置数据，仪器测试
	void OnApplyTestSetupDataForInstrument();
	// Apply测试设置数据，检波器测试
	void OnApplyTestSetupDataForSensor();
	// Apply测试设置数据，复合测试
	void OnApplyTestSetupDataForMultiple();
	// Apply测试设置数据，噪声监测
	void OnApplyTestSetupDataForSeisMonitor();	

	// 发送测试设置数据变化通知命令帧，仪器测试
	void OnSendCMDFrameForApplyTestSetupDataOfInstrument();
	// 发送测试设置数据变化通知命令帧，检波器测试
	void OnSendCMDFrameForApplyTestSetupDataOfSensor();
	// 发送测试设置数据变化通知命令帧，复合测试
	void OnSendCMDFrameForApplyTestSetupDataOfMultiple();
	// 发送测试设置数据变化通知命令帧，噪声监测
	void OnSendCMDFrameForApplyTestSetupDataOfSeisMonitor();
	// 发送TestSetup操作GO通知命令帧
	void OnSendCMDFrameForTestSetupGO(unsigned short usCommand, UINT uiTestNb);

	// 加载FormLine设置数据
	void OnLoadFormLineSetupData();
	// 保存FormLine设置数据；Apply
	void OnApplyFormLineSetupData();
	// 发送FormLine操作GO通知命令帧
	void OnSendFormLineCMDFrameGO(unsigned int uiSN);

	// 加载Look设置数据
	void OnLoadLookSetupData();
	// 保存Look设置数据；Apply
	void OnApplyLookSetupData();
	// 发送Look设置Apply通知命令帧
	void OnSendCMDFrameForApplyLookSetupData();

	// 响应仪器、检波器图形视图测试GO按钮
	void OnSendCMDFrameForGraphViewInstrumentOrSensorTestGOButton(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread);
	// 响应噪声监测图形视图Seismonitor按钮
	void OnSendCMDFrameForGraphViewNoiseSeismonitorButton(byte byTestOperation, CString strTestAbsoluteSpread);
};
