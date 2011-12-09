#pragma once
#include "Parameter.h"
/**
*@brief 服务器和客户端之间信息交互网络帧类
*
@note 帧定义：帧头{0xEB90}(2) + 帧长度(2) + 帧计数(4) + 帧类型(1) + 命令(2) + 命令字个数(2) + 数据区( ) + 校验位(1) + 帧尾{0x146F}(2)
@note 帧长度为帧头到帧尾总字节数
@note 帧类型：0x01-命令帧，不要求回令帧；0x02-命令帧，要求回令帧；0x03-数据帧，不要求回令帧；0x04-数据帧，要求回令帧；0xFF-回令帧
@note 
@note 来自客户端的命令：
@note 命令 1-Field ON/OFF，数据个数为1
@note 命令 2-FormLine，数据个数为1
@note 命令 3-Look，数据个数为1
@note 命令 4-Test From InstrumentTestSetup，数据个数为1
@note 命令 5-Test From SensorTestSetup，数据个数为1
@note 命令 6-Test From MultipleTestSetup，数据个数为1
@note 命令 7-Test From SeismonitorTestSetup，数据个数为1
@note 命令 8-Test From InstrumentView 来自仪器视图的“GO”按钮，数据个数为1
@note 命令 9-Test From SensorTestView	来自检波器视图的“GO”按钮，数据个数为1
@note 命令 10-Test From SeismonitorView，数据个数为1
@note 命令 20-测线配置文件更改命令，数据个数为1
@note 更改的目标 1-SurveySetup的ApplyAll；2-SurveySetup的ApplySensor
@note 更改的目标 11-LayoutSetup的ApplyBlast；12-LayoutSetup的ApplyMarkers；13-LayoutSetup的ApplyAux；14-LayoutSetup的ApplyDetour；15-LayoutSetup的ApplyMute
@note 更改的目标 21-LookSetup的Apply
@note 更改的目标 31-TestSetup的ApplyInstrument；32-TestSetup的ApplySensor；33-TestSetup的ApplyMultiple；34-TestSetup的ApplySeisMonitor
@note 更改的目标 41-TestSetup的ApplyInstrumentTestBase；42-TestSetup的ApplySensorTestBase
@note 更改的目标 51-TestBaseSetup的ApplyInstrumentTestLimit；52-TestBaseSetup的ApplySensorTestLimit
@note 更改的目标 61-SpreadSetup的ApplySpreadSetupDataForAbsolute；62-SpreadSetup的ApplySpreadSetupDataForGeneric
@note 
@note 发往客户端的命令：
@note 命令 100-现场数据输出更新，数据个数为0
@note 命令 101-Server Field ON/OFF，数据个数为1
@note 命令 110-现场数据输出更新，数据个数为0
@note 命令 (200+测试类型)-现场测试数据输出，数据个数为N；测试类型为1-22
*/
class CFrameInterface
{


public:
	CFrameInterface();
	~CFrameInterface();

public: // 属性
	/** 帧数据*/
	byte m_pFrameData[NetInterfaceBufSize];

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
	/** 仪器设备号*/
	unsigned int m_uiSN;

	// 命令	3-Look
	/** 是否自动查找*/
	UINT m_uiLookAuto;
	/** 新仪器是否测试Resistance*/
	UINT m_uiTestResistance;
	/** 新仪器是否测试Tilt*/
	UINT m_uiTestTilt;
	/** 新仪器是否测试Leakage*/
	UINT m_uiTestLeakage;

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

	// 命令	20-测线配置文件更改命令
	// 更改的目标	1-SurveySetup的ApplyAll；2-SurveySetup的ApplySensor
	// 更改的目标	11-LayoutSetup的ApplyBlast；12-LayoutSetup的ApplyMarkers；13-LayoutSetup的ApplyAux；14-LayoutSetup的ApplyDetour；15-LayoutSetup的ApplyMute
	// 更改的目标	21-LookSetup的Apply
	// 更改的目标	31-TestSetup的ApplyInstrument；32-TestSetup的ApplySensor；33-TestSetup的ApplyMultiple；34-TestSetup的ApplySeisMonitor
	// 更改的目标	41-TestSetup的ApplyInstrumentTestBase；42-TestSetup的ApplySensorTestBase
	// 更改的目标	51-TestSetup的ApplyInstrumentTestLimit；52-TestSetup的ApplySensorTestLimit
	// 更改的目标	61-SpreadSetup的ApplySpreadSetupDataForAbsolute；62-SpreadSetup的ApplySpreadSetupDataForGeneric
	/** 更改的目标*/
	unsigned int m_uiChangeAim;

public: // 方法
	// 解析帧
	bool ParseFrame();
	// 解析帧头
	int ParseFrameHead();
	// 解析命令字
	bool ParseCommandWord(int iPos);
	// 生成应答帧
	void MakeReplyFrame(CFrameInterface* m_pFrameInterface);

	// 生成现场数据输出通知命令帧
	void MakeSiteDataOutputCmdFrame();
	// 生成Server Field On/Off帧
	void MakeServerFieldFrame();
// 	// 生成测试数据帧
// 	void CreateOutputDataFrameTest(unsigned int uiTestType, unsigned int uiDataSize, unsigned short usDataCount, byte* pData);

};
