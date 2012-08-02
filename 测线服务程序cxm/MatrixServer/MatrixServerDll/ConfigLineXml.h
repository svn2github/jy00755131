
#include <list>
#include <map>
using std::list;
using std::map;
/**
* @struct Survery_Struct
* @brief Survery SETUP结构体
*/
typedef struct Survery_Struct
{
	/** 测线号*/
	unsigned int m_uiLine;
	/** 长度*/
	unsigned short m_usReceiverSectionSize;
	/** 接收区域+检波器类型，如100-103p1*/
	char* m_pcReceiverSection;
}m_oSurveryStruct;

/**
* @struct PointCode_Struct
* @brief Point Code SETUP结构体
*/
typedef struct PointCode_Struct
{
	/** 点代码编号*/
	unsigned int m_uiNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签，如PointCode1*/
	char* m_pcLabel;
	/** 长度*/
	unsigned short m_usSensorTypeSize;
	/** 检波器类型，如s1+cs*/
	char* m_pcSensorType;
}m_oPointCodeStruct;

/**
* @struct Sensor_Struct
* @brief Sensor SETUP结构体
*/
typedef struct Sensor_Struct
{
	/** 检波器号*/
	unsigned int m_uiNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签，如Sensor1*/
	char* m_pcLabel;
	/** 检波器阻抗的下限*/
	float m_fContinuityMin;
	/** 检波器阻抗的上限*/
	float m_fContinuityMax;
	/** 最大陆上检波器倾斜度百分比*/
	float m_fTilt;
	/** 最大RMS噪声电平*/
	float m_fNoise;
	/** 漏电测试测量地震道与地面之间的整个漏电电阻*/
	float m_fLeakage;
	/** SEGD编码*/
	unsigned int m_uiSEGDCode;
}m_oSensorStruct;

/**
* @struct Marker_Struct
* @brief Marker SETUP结构体
*/
typedef struct Marker_Struct
{
	/** 仪器类型，1-交叉站，2-电源站，3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN*/
	unsigned int m_uiSN;
	/** 测线号*/
	unsigned int m_uiLineName;
	/** 测点号*/
	unsigned int m_uiPointNb;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 标记点增量*/
	unsigned int m_uiMarkerIncr;
	/** 翻转标记，0-不翻转，1-翻转*/
	unsigned int m_uiReversed;
}m_oMarkerStruct;

/**
* @struct Aux_Struct
* @brief Aux SETUP结构体
*/
typedef struct Aux_Struct
{
	/** 索引号，与Instrument测试中的Auxiliary Descr对应*/
	unsigned int m_uiNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签，如pilot*/
	char* m_pcLabel;
	/** 仪器类型，1-交叉站，2-电源站，3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN*/
	unsigned int m_uiSN;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 增益，1600mv或400mv*/
	unsigned int m_uiGain;
	/** DPG编号*/
	unsigned int m_uiDpgNb;
	/** 长度*/
	unsigned short m_usCommentsSize;
	/** 注释，如Comments1*/
	char* m_pcComments;
}m_oAuxStruct;

/**
* @struct Detour_Struct
* @brief Detour SETUP结构体
*/
typedef struct Detour_Struct
{
	/** 低端仪器类型，1-交叉站，2-电源站，3-采集站*/
	unsigned int m_uiLowBoxType;
	/** 低端仪器SN*/
	unsigned int m_uiLowSN;
	/** 低端测道号*/
	unsigned int m_uiLowChanNb;
	/** 高端仪器类型，1-交叉站，2-电源站，3-采集站*/
	unsigned int m_uiHighBoxType;
	/** 高端仪器SN*/
	unsigned int m_uiHighSN;
	/** 高端测道号*/
	unsigned int m_uiHighChanNb;
	/** 停止标记，0-继续标记，1-停止标记*/
	unsigned int m_uiStopMarking;
}m_oDetourStruct;

/**
* @struct Mute_Struct
* @brief Mute SETUP结构体
*/
typedef struct Mute_Struct
{
	/** 测线号*/
	unsigned int m_uiLineName;
	/** 测点号*/
	unsigned int m_uiPointNb;
}m_oMuteStruct;

/**
* @struct BlastMachine_Struct
* @brief BlastMachine SETUP结构体
*/
typedef struct BlastMachine_Struct
{
	/** 索引号*/
	unsigned int m_uiNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签，如pilot*/
	char* m_pcLabel;
	/** 仪器类型，1-交叉站，2-电源站，3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN*/
	unsigned int m_uiSN;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 增益，1600mv或400mv*/
	unsigned int m_uiGain;
	/** DPG编号*/
	unsigned int m_uiDpgNb;
	/** 长度*/
	unsigned short m_usCommentsSize;
	/** 注释，如Comments1*/
	char* m_pcComments;
}m_oBlastMachineStruct;

/**
* @struct Absolute_Struct
* @brief Absolute Spread SETUP结构体
*/
typedef struct Absolute_Struct
{
	/** 索引号*/
	unsigned int m_uiNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签，如Absolute1*/
	char* m_pcLabel;
	/** 长度*/
	unsigned short m_usAbsoluteSpreadSize;
	/** 绝对排列，如1:1-127（测线名：起始测点号-终止测点号）*/
	char* m_pcAbsoluteSpread;
}m_oAbsoluteStruct;

/**
* @struct Generic_Struct
* @brief Generic Spread SETUP结构体
*/
typedef struct Generic_Struct
{
	/** 索引号*/
	unsigned int m_uiNb;
	/** 长度*/
	unsigned short m_usLabelSize;
	/** 标签，如Generic1*/
	char* m_pcLabel;
	/** 长度*/
	unsigned short m_usLineSize;
	/** 普通测线类型，如50（g1+g2）（50对地震道，第一个地震道增益为0Db,第二个为12dB）*/
	char* m_pcLine;
	/** 长度*/
	unsigned short m_usSpreadSize;
	/** 普通排列，如10(l1+ls)（10对测线，第一条测线为Line中定义的l1类型，第二条被跳过）*/
	char* m_pcSpread;
}m_oGenericStruct;

/**
* @struct Look_Struct
* @brief Look SETUP结构体
*/
typedef struct Look_Struct
{
	/** 自动查看是否连接了新设备，1-自动查看，2-手动查看*/
	unsigned int m_uiAutoLook;
	/** 检波器电阻测试，1-测试，2-不测试*/
	unsigned int m_uiResistance;
	/** 检波器倾斜度测试，1-测试，2-不测试*/
	unsigned int m_uiTilt;
	/** 检波器漏电测试，1-测试，2-不测试*/
	unsigned int m_uiLeakage;
}m_oLookStruct;

/**
* @struct LAULeakage_Struct
* @brief LAULeakage SETUP结构体
*/
typedef struct LAULeakage_Struct
{
	/** 限制值*/
	unsigned int m_uiLimit;
}m_oLAULeakageStruct;

/**
* @struct FormLine_Struct
* @brief Form Line SETUP结构体
*/
typedef struct FormLine_Struct
{
	/** 索引号*/
	unsigned int m_uiNb;
	/** 仪器类型，1-交叉站，2-电源站，3-采集站*/
	unsigned int m_uiBoxType;
	/** 仪器SN*/
	unsigned int m_uiSN;
}m_oFormLineStruct;

/**
* @struct Instrument_SensorTestBase_Struct
* @brief Instrument Sensor Test base SETUP结构体
*/
typedef struct Instrument_SensorTestBase_Struct
{
	unsigned int m_uiTestAim;
	unsigned int m_uiNb;	/**索引号（只读）*/
	unsigned short m_usDescrSize; /**m_pcDescr 大小*/
	char* m_pcDescr; /**测试类型描述，如INSTRUMENT NOISE（只读）*/
	unsigned int m_uiTestType;/**测试类型代码（只读）*/
	unsigned int m_uiADC;/**0-Close，1-Inner，2-Out	*/
	unsigned int m_uiGain;/**增益，1600mv或400mv*/
	unsigned int m_uiDAC;/**0-Close（关闭），1-Inner（连接到内部测试网络），2-Out（连接到检波器的电路输入端）*/	
	unsigned int m_uiFilter;/**滤波器类型，1-0.8LIN，2-0.8MIN*/
	unsigned int m_uiSamplingRate; /**（us）采样率*/
	unsigned int m_uiSamplingLength;/**（ms）采样长度*/
}m_oInstrumentTestBaseStruct, m_oSensorTestBaseStruct;

/**
* @struct Instrument_SensorTestLimit_Struct
* @brief Instrument Sensor Test Limit SETUP结构体
*/
typedef struct Instrument_SensorTestLimit_Struct
{
	unsigned int m_uiNb; /**索引号（只读）*/
	unsigned short m_usDescrSize; /**m_pcDescr 大小*/
	char* m_pcDescr; /**测试类型描述，如INSTRUMENT NOISE（只读）*/
	unsigned short m_usUnitSize; /**	单位 size*/
	char* m_pcUnit;/**	单位（只读），如%*/
	unsigned int m_uiTestAim;
	unsigned int m_uiTestType;/**测试类型代码（只读）*/
	float m_fLimit;/**极限值*/
}m_oInstrumentTestLimitStruct, m_oSensorTestLimitStruct;

/**
* @struct InstrumentTest_Struct
* @brief Instrument Test SETUP结构体
*/
typedef struct InstrumentTest_Struct
{
	unsigned int m_uiNb; /**索引号（只读）*/
	unsigned int m_uiTestType;/**测试类型代码（只读）*/
	unsigned int m_uiGain;/**增益，1600mv或400mv*/
	unsigned m_uiRecordLength; /**记录时长*/
	unsigned m_uiRecorded; /**是否记录测试数据，0-不记录，1-记录*/
	unsigned short m_usAuxiliaryDescrSize; /**辅助道描述 大小*/
	char* m_pcAuxiliaryDescr; /**辅助道描述，如a1-a3*/
	unsigned short m_usAbsoluteSpreadSize; /**绝对排列 大小*/
	char* m_pcAbsoluteSpread; /**绝对排列，如1:10-20*/
}m_oInstrumentTestStruct;

/**
* @struct SensorTest_Struct
* @brief Sensor Test SETUP结构体
*/
typedef struct SensorTest_Struct
{
	unsigned int m_uiNb; /**索引号（只读）*/
	unsigned int m_uiTestType;/**测试类型代码（只读）*/
	unsigned m_uiRecorded; /**是否记录测试数据，0-不记录，1-记录*/
	unsigned short m_usAbsoluteSpreadSize; /**绝对排列 大小*/
	char* m_pcAbsoluteSpread; /**绝对排列，如1:10-20*/
}m_oSensorTestStruct;

/**
* @struct MultipleTestKey_Struct
* @brief Multiple Test SETUP结构体
*/
typedef struct MultipleTestKey_Struct
{
	unsigned int m_uiNb; /**索引号（只读）*/
	unsigned short m_usTestNameSize;
	char* m_pcTestName;/**测试项目名称，如Test1*/
	unsigned short m_usAuxiliaryDescrSize; /**辅助道描述 大小*/
	char* m_pcAuxiliaryDescr; /**辅助道描述，如a1-a3*/
	unsigned short m_usAbsoluteSpreadSize; /**绝对排列 大小*/
	char* m_pcAbsoluteSpread; /**绝对排列，如1:10-20*/
	unsigned int m_uiDelayBetweenTest;/**(ms)每次测试之间的间隔*/
	unsigned int m_uiRecordResults;/**	是否记录测试数据，0-不记录，1-记录*/
	unsigned int m_uiRecordLength; /**(ms)测试记录时长*/
	unsigned int m_uiTestFileNb;/**文件编号*/
	bool operator < (const MultipleTestKey_Struct& rhs) const
	{
		return (m_uiNb < rhs.m_uiNb);
	}
}m_oMultipleTestKeyStruct;

/**
* @struct MultipleTestTask_Struct
* @brief Multiple Test Task结构体
*/
typedef struct MultipleTestTask_Struct
{
	unsigned int m_uiLineNb;/**测线号*/
	unsigned int m_uiTestType;/**测试类型代码*/
	unsigned int m_uiGain;/**增益，1600mv或400mv*/
	unsigned int m_uiLoopLineNb;/**需要重复进行的测线索引号*/
	unsigned int m_uiNbLoops;/**输入循环次数*/
}m_oMultipleTestTaskStruct;

/**
* @struct SeisMonitorTest_Struct
* @brief SeisMonitor Test结构体
*/
typedef struct SeisMonitorTest_Struct
{
	unsigned short m_usAbsoluteSpreadSize; /**绝对排列 大小*/
	char* m_pcAbsoluteSpread; /**绝对排列，如1:10-20*/
}m_oSeisMonitorStruct;

/**
* @struct LineSetupData_Struct
* @brief 测线客户端信息结构体
*/
typedef struct LineSetupData_Struct
{
	/** Survery*/
	list<m_oSurveryStruct> m_olsSurveryStruct;
	/** Point Code*/
	list<m_oPointCodeStruct> m_olsPointCodeStruct;
	/** Sensor*/
	list<m_oSensorStruct> m_olsSensorStruct;
	/** LAYOUT SETUP*/
	/** Marker*/
	list<m_oMarkerStruct> m_olsMarkerStruct;
	/** Aux*/
	list<m_oAuxStruct> m_olsAuxStruct;
	/** Detour*/
	list<m_oDetourStruct> m_olsDetourStruct;
	/** Mute*/
	list<m_oMuteStruct> m_olsMuteStruct;
	/** BlastMachine*/
	list<m_oBlastMachineStruct> m_olsBlastMachineStruct;
	/** 绝对排列和炮点的索引*/
	map<unsigned int, list<m_oAbsoluteStruct>> m_oAbsoluteStructMap;
	/** Generic*/
	list<m_oGenericStruct> m_olsGenericStruct;
	/** Look*/
	m_oLookStruct m_oLook;
	/** LAULeakage*/
	m_oLAULeakageStruct m_oLAULeakage;
	/** Form Line*/
	list<m_oFormLineStruct> m_olsFormLineStruct;
	/** Instrument Test base*/
	list<m_oInstrumentTestBaseStruct> m_olsInstrumentTestBaseStruct;
	/** Sensor Test base*/
	list<m_oSensorTestBaseStruct> m_olsSensorTestBaseStruct;
	/** Instrument Limit*/
	list<m_oInstrumentTestLimitStruct> m_olsInstrumentTestLimitStruct;
	/** Sensor Limit*/
	list<m_oSensorTestLimitStruct> m_olsSensorTestLimitStruct;
	/** Instrument Test*/
	list<m_oInstrumentTestStruct> m_olsInstrumentTestStruct;
	/** Sensor Test*/
	list<m_oSensorTestStruct> m_olsSensorTestStruct;
	/** Multiple Test*/
	map<m_oMultipleTestKeyStruct, list<m_oMultipleTestTaskStruct>> m_oMultpleTestStructMap;
	/** SeisMonitor*/
	m_oSeisMonitorStruct m_oSeisMonitor;
}m_oLineSetupDataStruct;
