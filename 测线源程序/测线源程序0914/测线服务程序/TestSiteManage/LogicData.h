#pragma once

#include "SurveySetupData.h"
#include "LayoutSetupData.h"
#include "LookSetupData.h"
#include "TestSetupData.h"

/**
*@brief 测线设置类
*/
class CLogicData
{
public:
	CLogicData();
	~CLogicData();

public: //属性
	/** 配置文件名，包含全部路径*/
	CString m_strIniFile;
	/** XMLDOM文件对象*/
	CXMLDOMDocument m_oXMLDOMDocument;

	/** FTP服务器IP地址*/
	CString m_strIPFTPServer;

	/** 测区设置对象*/
	CSurveySetupData m_oSurveySetupData;
	/** 布局设置对象*/
	CLayoutSetupData m_oLayoutSetupData;
	/** Look设置对象*/
	CLookSetupData m_oLookSetupData;
	/** 测试设置对象*/
	CTestSetupData m_oTestSetupData;

	/** 测线数组总数*/
	unsigned int m_uiLineCountAll;
	/** 测点数组总数*/
	unsigned int m_uiPointCountAll;
	/** 测道数组总数*/
	unsigned int m_uiChannelCountAll;
	/** 测试单元数组总数*/
	unsigned int m_uiTestElementCountAll;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void OnReset();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 打开配置文件
	void OpenIniXMLFile();
	// 关闭配置文件
	void CloseIniXMLFile();

	// 由FTP服务器得到Matrix设置文件
	bool GetMatrixIniFileFromFTPServer();

	// 设置哑点到测点和测道
	void SetMuteToCSurveySetupData(bool bValue);

	// 设置基本测试项目，来自测试设置的测试任务
	bool OnSetupTestProjectForTestBaseFromTestSetup(unsigned int uiTestNb);
	// 设置基本测试项目，来自测试视图的测试任务
	bool OnSetupTestProjectForTestBaseFromTestView(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread);
	// 设置基本测试项目，处理新加检波器的初始测试
	bool OnSetupTestProjectForSensorTestOnField();
	// 设置噪声监测测试项目，来自测试设置的测试任务
	bool OnSetupTestProjectForTestNoiseFromTestSetup();
	// 设置噪声监测测试项目，来自噪声监测视图的测试任务
	bool OnSetupTestProjectForTestNoiseFromTestView(CString strTestAbsoluteSpread);

	// 清除被删除仪器对应测道的仪器SN号
	void ClearSNInChannelWhenInstrumentDelete(unsigned int uiIP);

	// 处理配置文件更改	SurveySetup的ApplyAll
	void OnSetupDataChangeForSurveySetupApplyAll();
	// 处理配置文件更改	SurveySetup的ApplySensor
	void OnSetupDataChangeForSurveySetupApplySensor();
	// 处理配置文件更改	LayoutSetup的ApplyBlast
	void OnSetupDataChangeForLayoutSetupApplyBlast();
	// 处理配置文件更改	LayoutSetup的ApplyAux
	void OnSetupDataChangeForLayoutSetupApplyAux();
	// 处理配置文件更改	LayoutSetup的ApplyDetour
	void OnSetupDataChangeForLayoutSetupApplyDetour();
	// 处理配置文件更改	LayoutSetup的ApplyMarkers
	void OnSetupDataChangeForLayoutSetupApplyMarkers();
	// 处理配置文件更改	LayoutSetup的ApplyMute
	void OnSetupDataChangeForLayoutSetupApplyMute();
	// 处理配置文件更改	LookSetup的Apply
	void OnSetupDataChangeForLookSetupApply();
	// 处理配置文件更改	TestSetup的ApplyInstrument
	void OnSetupDataChangeForTestSetupApplyInstrument();
	// 处理配置文件更改	TestSetup的ApplySensor
	void OnSetupDataChangeForTestSetupApplySensor();
	// 处理配置文件更改	TestSetup的ApplyMultiple
	void OnSetupDataChangeForTestSetupApplyMultiple();
	// 处理配置文件更改	TestSetup的ApplySeisMonitor
	void OnSetupDataChangeForTestSetupApplySeisMonitor();

	// 处理配置文件更改	TestSetup的ApplyInstrumentTestBase
	void OnSetupDataChangeForTestSetupApplyInstrumentTestBase();
	// 处理配置文件更改	TestSetup的ApplySensorTestBase
	void OnSetupDataChangeForTestSetupApplySensorTestBase();

	// 处理配置文件更改	TestSetup的ApplyInstrumentTestLimit
	void OnSetupDataChangeForTestSetupApplyInstrumentTestLimit();
	// 处理配置文件更改	TestSetup的ApplySensorTestLimit
	void OnSetupDataChangeForTestSetupApplySensorTestLimit();

	// 设置测试单元队列判据
	void SetTestElementListLimit(unsigned int uiType);
	// 设置测试单元队列判据，仪器测试，仪器噪声
	void SetTestElementListLimitInstrumentNoise();
	// 设置测试单元队列判据，仪器测试，仪器畸变
	void SetTestElementListLimitInstrumentDistortion();
	// 设置测试单元队列判据，仪器测试，仪器串音
	void SetTestElementListLimitInstrumentCrosstalk();
	// 设置测试单元队列判据，仪器测试，仪器增益和相位
	void SetTestElementListLimitInstrumentGainAndPhase();
	// 设置测试单元队列判据，仪器测试，仪器共模
	void SetTestElementListLimitInstrumentCMRR();
	// 设置测试单元队列判据，检波器测试，野外电阻
	void SetTestElementListLimitISensorResistance();
	// 设置测试单元队列判据，检波器测试，野外漏电
	void SetTestElementListLimitISensorLeakage();
	// 设置测试单元队列判据，检波器测试，野外噪声
	void SetTestElementListLimitISensorNoise();
	// 设置测试单元队列判据，检波器测试，野外倾斜度
	void SetTestElementListLimitISensorTilt();
	// 设置测试单元队列判据，检波器测试，野外倾斜度模式
	void SetTestElementListLimitISensorTiltModel();

	// 设置测试结果，基本测试
	void SetTestResultBase(int iTestType, CTestElementList* pTestElementList, bool bProcSensorTestOnField);
};