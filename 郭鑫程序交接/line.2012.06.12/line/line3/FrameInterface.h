#pragma once

class CFrameInterface
{

// 帧定义：帧头{0xEB90}(2) + 帧长度(2) + 帧计数(4) + 帧类型(1) + 命令(2) + 命令字个数(2) + 数据区( ) + 校验位(1) + 帧尾{0x146F}(2)
// 帧长度为帧头到帧尾总字节数
public:
	CFrameInterface();
	~CFrameInterface();

public: // 属性
	/** 帧数据*/
	byte m_pFrameData[65536];

	/** 源IP地址*/
	CString m_strIPSource;
	/** 源端口号*/
	unsigned int  m_uiPortSource;
	/** 目标IP地址*/
	CString m_strIPAim;
	/** 目标端口号*/
	unsigned int  m_uiPortAim;

	/** 帧长度*/
	unsigned short m_usFrameSize;
	/** 帧计数*/
	unsigned int m_uiFrameCount;
	/** 帧类型 0x01-命令帧，不要求回令帧；0x02-命令帧，要求回令帧；0x03-数据帧，不要求回令帧；0x04-数据帧，要求回令帧；0xFF-回令帧*/
	byte m_byFrameType;
	/** 命令*/
	unsigned short m_usCommand;
	/** 命令字个数*/
	unsigned short m_usCommandCount;
	/** 数据区长度*/
	unsigned short m_usDataSize;
	/** 数据区*/
	byte* m_pData;

	// 命令	1-Field ON/OFF
	// 命令 101-Server Field ON/OFF
	/**	FieldON/OFF 0：OFF；1：ON*/
	byte m_byFieldOperation;

	// 命令	2-FormLine	
	unsigned int m_uiSN;	// 仪器设备号

	// 命令	3-Look
	UINT m_uiLookAuto;	// 是否自动查找
	UINT m_uiTestResistance;	// 新仪器是否测试Resistance
	UINT m_uiTestTilt;	// 新仪器是否测试Tilt
	UINT m_uiTestLeakage;	// 新仪器是否测试Leakage

	// 命令	4-Test From InstrumentTestSetup
	// 命令	5-Test From SensorTestSetup
	// 命令	6-Test From MultipleTestSetup
	// 命令	7-Test From SeismonitorTestSetup
	/** 测试索引号*/
	UINT m_uiTestNb;

	// 命令	8-Test From InstrumentView 来自仪器视图的“GO”按钮
	// 命令	9-Test From SensorTestView	来自检波器视图的“GO”按钮	
	/** 测试对象 1-仪器；2-检波器*/
	unsigned int m_uiTestAim;
	/** 测试类型*/
	unsigned int m_uiTestType;
	// 命令	10-Test From SeismonitorView
	/** 野外噪声监测操作 1-启动测试；0-停止测试*/
	byte m_byTestOperation;
	/** 绝对排列定义*/
	CString m_strTestAbsoluteSpread;
	//////////////////////////////////////////////////////////////////////////
	// 命令	20-测线配置文件更改命令
	// 更改的目标	1-SurveySetup的ApplyAll；2-SurveySetup的ApplySensor
	// 更改的目标	11-LayoutSetup的ApplyBlast；12-LayoutSetup的ApplyMarkers；13-LayoutSetup的ApplyAux；14-LayoutSetup的ApplyDetour；15-LayoutSetup的ApplyMute
	// 更改的目标	21-LookSetup的Apply
	// 更改的目标	31-TestSetup的ApplyInstrument；32-TestSetup的ApplySensor；33-TestSetup的ApplyMultiple；34-TestSetup的ApplySeisMonitor
	// 更改的目标	41-TestBaseSetup的ApplyInstrumentTestBase；42-TestBaseSetup的ApplySensorTestBase
	// 更改的目标	51-TestBaseSetup的ApplyInstrumentTestLimit；52-TestBaseSetup的ApplySensorTestLimit
	// 更改的目标	61-SpreadSetup的ApplySpreadSetupDataForAbsolute；62-SpreadSetup的ApplySpreadSetupDataForGeneric
	/** 更改的目标*/
	unsigned int m_uiChangeAim;


public: // 方法
	// 重置对象
	void Reset();
	// 解析帧头
	bool ParseFrameHead();
	// 解析命令字
	bool ParseCommandWord();
	// 生成应答帧
	void MakeReplyFrame(CFrameInterface* m_pFrameInterface);

	// 生成现场数据输出通知命令帧
	void MakeSiteDataOutputCmdFrame(unsigned int uiChangeAim);

	// 生成Field On/Off帧
	void MakeFieldFrame();

	// 生成FormLine帧
	void MakeFormLineFrame(unsigned int uiSN);

	// 生成测试命令帧，测试设置对话框GO按钮
	void MakeTestCmdFrameForTestSetup(unsigned short usCommand, UINT uiTestNb);

	// 生成测试命令帧，响应仪器、检波器图形视图测试GO按钮
	void MakeTestCmdFrameForGraphViewGOButton(unsigned int uiTestAim, unsigned int uiTestType, CString strTestAbsoluteSpread);
	// 生成测试命令帧，响应噪声监测图形视图Seismonitor按钮
	void MakeTestCmdFrameForGraphViewSeismonitorButton(byte byTestOperation, CString strTestAbsoluteSpread);
};
