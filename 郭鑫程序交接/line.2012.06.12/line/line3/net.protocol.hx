#pragma once

// 测试任务
#define InstrumentNoiseTestType			0x00000001
#define InstrumentDistortionTestType	(InstrumentNoiseTestType << 1)
#define InstrumentCrosstalkTestType		(InstrumentNoiseTestType << 2)
#define InstrumentCMRRTestType			(InstrumentNoiseTestType << 3)
#define InstrumentGainPhaseTestType		(InstrumentNoiseTestType << 4)
#define SensorResistanceTestType		(InstrumentNoiseTestType << 5)
#define SensorLeakageTestType			(InstrumentNoiseTestType << 6)
#define SensorNoiseTestType				(InstrumentNoiseTestType << 7)
#define SensorTiltTestType				(InstrumentNoiseTestType << 8)
#define SensorTiltModelTestType			(InstrumentNoiseTestType << 9)
#define SeisMonitorTestType				(InstrumentNoiseTestType << 10)

// 命令字
// 上电
#define CmdFieldOn					0x0001
// 断电
#define CmdFieldOff					0x0002
// 增加仪器（后跟仪器结构体）
#define CmdAddInstrument			0x0003
// 修改仪器属性（后跟仪器结构体）
#define CmdChangeInstrument			0x0004
// 删除仪器（后跟仪器SN+IP）
#define CmdDeleteInstrument			0x0005
// 得到单个仪器全部信息（后跟仪器SN+IP）
#define CmdGetInstrumentInfo		0x0006
// 得到全部仪器的全部信息
#define CmdGetInstrumentInfoAll		0x0007
// 全部仪器开始测试（后跟测试类型，测试类型占4个字节）
#define CmdStartTestAll				0x0008
// 部分仪器开始测试（测试类型+所有参与采集的仪器SN+IP）
#define CmdStarTestSelect			0x0009
// 全部仪器停止测试（后跟测试类型，测试类型占4个字节）
#define CmdStopTestAll				0x000a
// 部分仪器停止测试（测试类型+所有停止采集的仪器SN+IP）
#define	CmdStopTestSelect			0x000b

// SurveyXML 文件信息（后跟信息结构体）
#define CmdSurveyXMLInfo			0x1001
// PointCode XML文件信息（后跟信息结构体）
#define CmdPointCodeXMLInfo			0x1002
// Sensor XML文件信息（后跟信息结构体）
#define CmdSensorXMLInfo			0x1003
// Marker XML文件信息（后跟信息结构体）
#define CmdMarkerXMLInfo			0x1004
// Aux XML文件信息（后跟信息结构体）
#define CmdAuxXMLInfo				0x1005
// Detour XML文件信息（后跟信息结构体）
#define CmdDetourXMLInfo			0x1006
// Mute XML文件信息（后跟信息结构体）
#define CmdMuteXMLInfo				0x1007
// BlastMachine XML文件信息（后跟信息结构体）
#define CmdBlastMachineXMLInfo		0x1008
// Absolute XML文件信息（后跟信息结构体）
#define CmdAbsoluteXMLInfo			0x1009
// Generic XML文件信息（后跟信息结构体）
#define CmdGenericXMLInfo			0x100a
// Look XML文件信息（后跟信息结构体）
#define CmdLookXMLInfo				0x100b
// InstrumentTestBase XML文件信息（后跟信息结构体）
#define CmdInstrumentTestBaseXMLInfo	0x100c
// SensorTestBase XML文件信息（后跟信息结构体）
#define	CmdSensorTestBaseXMLInfo		0x100d
// InstrumentTestLimit XML文件信息（后跟信息结构体）
#define CmdInstrumentTestLimitXMLInfo	0x100e
// SensorTestLimit XML文件信息（后跟信息结构体）
#define CmdSensorTestLimitXMLInfo		0x100f
// InstrumentTest XML文件信息（后跟信息结构体）
#define CmdInstrumentTestXMLInfo		0x1010
// SensorTest XML文件信息（后跟信息结构体）
#define CmdSensorTestXMLInfo			0x1011
// MultipleTest XML文件信息（后跟信息结构体）
#define CmdMultipleTestXMLInfo			0x1012
// SeisMonitorTest XML文件信息（后跟信息结构体）
#define CmdSeisMonitorTestXMLInfo		0x1013

// 客户端登陆验证
#define CmdClientConnect				0x2001

// TCP/IP帧长限制
#define FrameSizeLimit					1460
// 帧头
#define FrameHead1						0x11
// 帧头
#define FrameHead2						0x22
// 帧头
#define FrameHead3						0x33
// 帧头
#define FrameHead4						0x44
// 帧尾
#define FrameTail						0xaa
// 客户端向服务端发送验证码
#define CommCheck						_T("天津海泰光电科技有限公司")

// 服务端与客户端通讯帧结构
/************************************************************************/
/* 业务逻辑：先找到帧头，帧头校验，成功则继续，失败则重新找帧头；读取帧内
   容总长度，验证其合理性，不合理则重新找帧头；能找到帧尾则继续，否则先不
   处理；验证帧尾，正确则对帧内容进行业务处理，错误则重新找帧头         */
/************************************************************************/
typedef struct CommFrame_Struct
{
	// 帧头
	char m_cFrameHead[4];
	// 帧内容总长度，小于帧长限制
	unsigned short m_usFrameLength;
	// 帧命令字，表明帧的功能
	unsigned short m_usCmd;
	// 总帧数
	unsigned int m_uiFrameNum;
	// 帧序号
	unsigned int m_uiFrameIndex;
	// 帧命令，为1则为设置命令，为2则为查询命令，为3则为应答命令
	char m_cCommand;
	// 应答设置和查询命令是否成功
	char m_cResult;
	// 帧内容长度 = 帧内容总长度 – 2 – 4 – 4 – 1 - 1；
	/** FieldOn命令会应答4个字节表明客户端FieldOn需要等待的时间，
	以秒为单位为0则执行，否则等待)。*/
	unsigned short m_usFrameInfoSize;
	// 帧内容指针
	char* m_pcFrameInfo;
	// 帧尾
	char m_cFrameTail;
}m_oCommFrameStruct;
// SURVEY SETUP结构体
// Survey
typedef struct Survey_Struct
{
	// 测线号
	unsigned int m_uiLine;
	// 接收区域+检波器类型，如100-103p1
	char* m_pcReceiverSection;
	// 长度
	unsigned short m_usReceiverSectionSize;
}m_oSurveyStruct;
// Point Code
typedef struct PointCode_Struct
{
	// 点代码编号
	unsigned int m_uiNb;
	// 标签，如PointCode1
	char* m_pcLabel;
	// 长度
	unsigned short m_usLabelSize;
	// 检波器类型，如s1+cs
	char* m_pcSensorType;
	// 长度
	unsigned short m_usSensorTypeSize;
}m_oPointCodeStruct;
// Sensor
typedef struct Sensor_Struct
{
	// 检波器号
	unsigned int m_uiNb;
	// 标签，如Sensor1
	char* m_pcLabel;
	// 长度
	unsigned short m_usLabelSize;
	// 检波器阻抗的下限
	float m_fContinuityMin;
	// 检波器阻抗的上限
	float m_fContinuityMax;
	// 最大陆上检波器倾斜度百分比
	float m_fTilt;
	// 最大RMS噪声电平
	float m_fNoise;
	// 漏电测试测量地震道与地面之间的整个漏电电阻
	float m_fLeakage;
	// SEGD编码
	unsigned int m_uiSEGDCode;
}m_oSensorStruct;
// LAYOUT SETUP
// Marker
typedef struct Marker_Struct
{
	// 仪器类型，1-交叉站，2-电源站，3-采集站
	unsigned int m_uiBoxType;
	// 仪器SN
	unsigned int m_uiSN;
	// 测线号
	unsigned int m_uiLineName;
	// 测点号
	unsigned int m_uiPointNb;
	// 测道号
	unsigned int m_uiChannelNb;
	// 标记点增量
	unsigned int m_uiMarkerIncr;
	// 翻转标记，0-不翻转，1-翻转
	unsigned int m_uiReversed;
}m_oMarker_Struct;
// Aux
typedef struct Aux_Struct
{
	// 索引号，与Instrument测试中的Auxiliary Descr对应
	unsigned int m_uiNb;
	// 标签，如pilot
	char* m_pcLabel;
	// 长度
	unsigned short m_usLabelSize;
	// 仪器类型，1-交叉站，2-电源站，3-采集站
	unsigned int m_uiBoxType;
	// 仪器SN
	unsigned int m_uiSN;
	// 测道号
	unsigned int m_uiChannelNb;
	// 增益，1600mv或400mv
	unsigned int m_uiGain;
	// DPG编号
	unsigned int m_uiDpgNb;
	// 注释，如Comments1
	char* m_pcComments;
	// 长度
	unsigned short m_usCommentsSize;
}m_oAux_Struct;
// Detour
typedef struct Detour_Struct
{
	// 低端仪器类型，1-交叉站，2-电源站，3-采集站
	unsigned int m_uiLowBoxType;
	// 低端仪器SN
	unsigned int m_uiLowSN;
	// 低端测道号
	unsigned int m_uiLowChanNb;
	// 高端仪器类型，1-交叉站，2-电源站，3-采集站
	unsigned int m_uiHighBoxType;
	// 高端仪器SN
	unsigned int m_uiHighSN;
	// 高端测道号
	unsigned int m_uiHighChanNb;
	// 停止标记，0-继续标记，1-停止标记
	unsigned int m_uiStopMarking;
}m_oDetourStruct;
// Mute
typedef struct Mute_Struct
{
	// 测线号
	unsigned int m_uiLineName;
	// 测点号
	unsigned int m_uiPointNb;
}m_oMuteStruct;
// BlastMachine
typedef struct BlastMachine_Struct
{
	// 索引号
	unsigned int m_uiNb;
	// 标签，如pilot
	char* m_pcLabel;
	// 长度
	unsigned short m_usLabelSize;
	// 仪器类型，1-交叉站，2-电源站，3-采集站
	unsigned int m_uiBoxType;
	// 仪器SN
	unsigned int m_uiSN;
	// 测道号
	unsigned int m_uiChannelNb;
	// 增益，1600mv或400mv
	unsigned int m_uiGain;
	// DPG编号
	unsigned int m_uiDpgNb;
	// 注释，如Comments1
	char* m_pcComments;
	// 长度
	unsigned short m_usCommentsSize;
}m_oBlastMachineStruct;
// Spread Type Setup
// Absolute
typedef struct Absolute_Struct
{
	// 索引号
	unsigned int m_uiNb;
	// 标签，如Absolute1
	char* m_pcLabel;
	// 长度
	unsigned short m_usLabelSize;
	// 绝对排列，如1:1-127（测线名：起始测点号-终止测点号）
	char* m_pcAbsoluteSpread;
	// 长度
	unsigned short m_usAbsoluteSpreadSize;
}m_oAbsoluteStruct;
// Generic
typedef struct Generic_Struct
{
	// 索引号
	unsigned int m_uiNb;
	// 标签，如Generic1
	char* m_pcLabel;
	// 长度
	unsigned short m_usLabelSize;
	// 普通测线类型，如50（g1+g2）（50对地震道，第一个地震道增益为0Db,第二个为12dB）
	char* m_pcLine;
	// 长度
	unsigned short m_usLineSize;
	// 普通排列，如10(l1+ls)（10对测线，第一条测线为Line中定义的l1类型，第二条被跳过）
	char* m_pcSpread;
	// 长度
	unsigned short m_usSpreadSize;
}m_oGenericStruct;
// Look
typedef struct Look_Struct
{
	// 自动查看是否连接了新设备，1-自动查看，2-手动查看
	unsigned int m_uiAutoLook;
	// 检波器电阻测试，1-测试，2-不测试
	unsigned int m_uiResistance;
	// 检波器倾斜度测试，1-测试，2-不测试
	unsigned int m_uiTilt;
	// 检波器漏电测试，1-测试，2-不测试
	unsigned int m_uiLeakage;
}m_oLookStruct;
// Form Line
typedef struct FormLine_Struct
{
	// 索引号
	unsigned int m_uiNb;
	// 仪器类型，1-交叉站，2-电源站，3-采集站
	unsigned int m_uiBoxType;
	// 仪器SN
	unsigned int m_uiSN;
}m_oFormLineStruct;

// Instrument Sensor Test base
typedef struct Instrument_SensorTestBase_Struct
{
	unsigned int m_uiNb;	//索引号（只读）
	unsigned short m_usDescrSize; //m_pcDescr 大小
	char* m_pcDescr; //测试类型描述，如INSTRUMENT NOISE（只读）
	unsigned int m_uiTestType;//测试类型代码（只读）
	unsigned int m_uiADC;//0-Close，1-Inner，2-Out	
	unsigned int m_uiGain;//增益，1600mv或400mv
	unsigned int m_uiDAC;//0-Close（关闭），1-Inner（连接到内部测试网络），2-Out（连接到检波器的电路输入端）	
	unsigned int m_uifilter;//滤波器类型，1-0.8LIN，2-0.8MIN
	unsigned int m_uiSampingRate; //（us）采样率
	unsigned int m_uiSampingLength;//（ms）采样长度
}m_oInstrumentTestBaseStruct, m_oSensorTestBaseStruct;

//Instrument Limit
typedef struct Instrument_SensorTestLimit_Struct
{
	unsigned int m_uiNb; //索引号（只读）
	unsigned short m_usDescrSize; //m_pcDescr 大小
	char* m_pcDescr; //测试类型描述，如INSTRUMENT NOISE（只读）
	unsigned int m_uiUnitSize; //	单位 size
	char* m_pcUnit;//	单位（只读），如%
	unsigned int m_uiTestType;//测试类型代码（只读）
	float m_fLimit;//极限值
}m_oInstrumentTestLimitStruct, m_oSensorTestLimitStruct;

//instrument
typedef struct InstrumentTest_Struct
{
	unsigned int m_uiNb; //索引号（只读）
	unsigned int m_uiTestType;//测试类型代码（只读）
	unsigned int m_uiGain;//增益，1600mv或400mv
	unsigned m_uiRecordLength; //记录时长
	unsigned m_uiRecorded; //是否记录测试数据，0-不记录，1-记录
	unsigned short m_usAuxiliaryDescrSize; //辅助道描述 大小
	char* m_pcAuxiliaryDescr; //辅助道描述，如a1-a3
	unsigned short m_usAbsoluteSpreadSize; //绝对排列 大小
	char* m_pcAbsoluteSpread; //绝对排列，如1:10-20
}m_oInstrumentTestStruct;

//Sensor
typedef struct SensorTest_Struct
{
	unsigned int m_uiNb; //索引号（只读）
	unsigned int m_uiTestType;//测试类型代码（只读）
	unsigned m_uiRecorded; //是否记录测试数据，0-不记录，1-记录
	unsigned short m_usAbsoluteSpreadSize; //绝对排列 大小
	char* m_pcAbsoluteSpread; //绝对排列，如1:10-20
}m_oSensorTestStruct;

//Multple
typedef struct MultpleTest_Struct
{
	unsigned int m_uiNb; //索引号（只读）
	unsigned m_uiTestNameSize;
	char* m_pcTestName;//测试项目名称，如Test1
	unsigned short m_usAuxiliaryDescrSize; //辅助道描述 大小
	char* m_pcAuxiliaryDescr; //辅助道描述，如a1-a3
	unsigned short m_usAbsoluteSpreadSize; //绝对排列 大小
	char* m_pcAbsoluteSpread; //绝对排列，如1:10-20

	unsigned int m_uiDelayBetweenTest;//(ms)每次测试之间的间隔
	unsigned int m_uiRecordResults;//	是否记录测试数据，0-不记录，1-记录
	unsigned int m_uiRecordLength; //(ms)测试记录时长
	unsigned int m_uiTestFileNb;//文件编号
	unsigned int m_uiLineNb;//测线号
	unsigned int m_uiTestType;//测试类型代码
	unsigned int m_uiGain;//增益，1600mv或400mv
	unsigned int m_uiLoopLineNb;//需要重复进行的测线索引号
	unsigned int m_uiNbLoops;//输入循环次数
}m_oMultpleTestStruct;

//SeisMonitor
typedef struct SeisMonitorTest_Struct
{
	unsigned short m_usAbsoluteSpreadSize; //绝对排列 大小
	char* m_pcAbsoluteSpread; //绝对排列，如1:10-20
}m_oSeisMonitorStruct;

// 仪器属性结构体
typedef struct Instrument_Struct 
{
	/** 在仪器数组中的位置*/
	unsigned int m_uiIndex;
	/** 仪器是否使用中*/
	bool m_bInUsed;
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站；4-LCI*/
	int m_iInstrumentType;
	/** 仪器IP地址*/
	unsigned int m_uiIP;
	/** 仪器路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	int m_iRoutDirection;
	/** 路由地址 交叉线方向 上方*/
	unsigned int m_uiRoutIPTop;
	/** 路由地址 交叉线方向 下方*/
	unsigned int m_uiRoutIPDown;
	/** 路由地址 测线线方向 左方*/
	unsigned int m_uiRoutIPLeft;
	/** 路由地址 测线线方向 右方*/
	unsigned int m_uiRoutIPRight;
	/** 路由开关*/
	char m_cLAUXRoutOpenSet;
	/** 链接的仪器 上方*/
	Instrument_Struct* m_pInstrumentTop;
	/** 链接的仪器 下方*/
	Instrument_Struct* m_pInstrumentDown;
	/** 链接的仪器 左方*/
	Instrument_Struct* m_pInstrumentLeft;
	/** 链接的仪器 右方*/
	Instrument_Struct* m_pInstrumentRight;
	/** 首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	/** 尾包计数*/
	int m_iTailFrameCount;
	/** 仪器时延*/
	unsigned int m_uiDelayTime;
	/** 仪器本地系统时间*/
	unsigned int m_uiSystemTime;
	/** 仪器网络时间*/
	unsigned int m_uiNetTime;
	/** 仪器网络状态*/
	unsigned int m_uiNetState;
	/** 仪器参数备用1*/
	unsigned int m_uiNetOrder;
	/** 16bits时间，接收时刻低位*/
	unsigned short m_usReceiveTime;	
	/** 16bits时间，发送时刻低位*/
	unsigned short m_usSendTime;
	/** 16bits 测线方向左面尾包接收时刻*/
	unsigned short m_usLineLeftReceiveTime;	
	/** 16bits 测线方向右面尾包接收时刻*/
	unsigned short m_usLineRightReceiveTime;	
	/** 16bits 交叉线方向上面尾包接收时刻*/
	unsigned short m_usCrossTopReceiveTime;	
	/** 16bits 交叉线方下面尾包接收时刻*/
	unsigned short m_usCrossDownReceiveTime;	
	/** 采集站或电源站的广播端口*/
	unsigned int m_uiBroadCastPort;
	/** 测线*/
	int m_iLineIndex;
	/** 测点号*/
	int m_iPointIndex;
	/** 标记线号*/
	unsigned int m_uiLineNb;
	/** 标记点号*/
	unsigned int m_uiPointNb;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 是否跳过道*/
	bool m_bJumpedChannel;
	/** 是否连接检波器*/
	bool m_bSensor;
	/** 是否辅助道*/
	bool m_bAux;
	/** 是否连接爆炸机*/
	bool m_bBlastMachine;
	/** 是否迂回道*/
	bool m_bDetour;
	/** 是否迂回道低端标记点*/
	bool m_bDetourMarkerLow;
	/** 是否迂回道高端标记点*/
	bool m_bDetourMarkerHigh;
	/** 是否迂回道停止标记*/
	unsigned int m_uiStopMarking;
	/** 是否标记点*/
	bool m_bMarker;
	/** 是否哑道*/
	bool m_bMute;
	// 首包位置稳定次数
	int m_iHeadFrameStableNum;
	/** 第几次设置IP地址*/
	int m_iIPSetCount;
	/** IP地址设置是否成功*/
	bool m_bIPSetOK;
	/** 第几次尾包时刻查询*/
	int m_iTailTimeQueryCount;
	/** 尾包时刻查询应答次数*/
	int m_iTailTimeReturnCount;
	/** 尾包时刻查询是否成功*/
	bool m_bTailTimeQueryOK;
	/** 本地时间修正高位*/
	unsigned int m_uiTimeHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiTimeLow;
	/** 第几次设置仪器时间*/
	int m_iTimeSetCount;
	/** 仪器时统成功次数*/
	int m_iTimeSetReturnCount;
	/** 仪器时间设置是否成功*/
	bool m_bTimeSetOK;
	// ADC命令设置是否应答
	bool m_bADCSetReturn;
	// 仪器是否进行了ADC参数设置
	bool m_bADCSet;
	// 仪器开始ADC数据采集设置成功
	bool m_bADCStartSample;
	// 仪器停止ADC数据采集设置成功
	bool m_bADCStopSample;
	// 误码查询发送帧数
	unsigned int m_uiErrorCodeQueryNum;
	// 误码查询应答帧数
	unsigned int m_uiErrorCodeReturnNum;
	// 采集站和电源站网络数据错误计数
	int m_iFDUErrorCodeDataCount;
	// 采集站和电源站命令错误计数
	int m_iFDUErrorCodeCmdCount;
	// 采集站和电源站网络数据错误计数
	char m_cFDUErrorCodeDataCountOld;
	// 采集站和电源站命令错误计数
	char m_cFDUErrorCodeCmdCountOld;

	// 交叉站大线A数据故障
	int m_iLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	int m_iLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	int m_iLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	int m_iLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	int m_iLAUXErrorCodeCmdCount;

	// 交叉站大线A数据故障
	char m_cLAUXErrorCodeDataLineACountOld;
	// 交叉站大线B数据故障
	char m_cLAUXErrorCodeDataLineBCountOld;
	// 交叉站交叉线A数据故障
	char m_cLAUXErrorCodeDataLAUXLineACountOld;
	// 交叉站交叉线B数据故障
	char m_cLAUXErrorCodeDataLAUXLineBCountOld;
	// 交叉站命令口故障
	char m_cLAUXErrorCodeCmdCountOld;

	// 实际接收ADC数据帧数（含重发帧）
	unsigned int m_uiADCDataActualRecFrameNum;
	// 重发查询帧得到的应答帧数
	unsigned int m_uiADCDataRetransmissionFrameNum;
	// 应该接收ADC数据帧数（含丢帧）
	unsigned int m_uiADCDataShouldRecFrameNum;
	// ADC数据帧的指针偏移量
	unsigned short m_usADCDataFramePoint;
	// ADC数据帧发送时的本地时间
	unsigned int m_uiADCDataFrameSysTime;
	// ADC数据帧起始帧数
	int m_iADCDataFrameStartNum;
}m_oInstrumentStruct;

// 网络通讯中的仪器属性结构体
typedef struct InstrumentNet_Struct 
{
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站；4-LCI*/
	int m_iInstrumentType;
	/** 仪器IP地址*/
	unsigned int m_uiIP;
	/** 仪器路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	int m_iRoutDirection;
	/** 测线*/
	int m_iLineIndex;
	/** 测点号*/
	int m_iPointIndex;
	/** 标记线号*/
	unsigned int m_uiLineNb;
	/** 标记点号*/
	unsigned int m_uiPointNb;
	/** 测道号*/
	unsigned int m_uiChannelNb;
	/** 是否跳过道*/
	bool m_bJumpedChannel;
	/** 是否连接检波器*/
	bool m_bSensor;
	/** 是否辅助道*/
	bool m_bAux;
	/** 是否连接爆炸机*/
	bool m_bBlastMachine;
	/** 是否迂回道*/
	bool m_bDetour;
	/** 是否迂回道低端标记点*/
	bool m_bDetourMarkerLow;
	/** 是否迂回道高端标记点*/
	bool m_bDetourMarkerHigh;
	/** 是否迂回道停止标记*/
	unsigned int m_uiStopMarking;
	/** 是否标记点*/
	bool m_bMarker;
	/** 是否哑道*/
	bool m_bMute;
	/** IP地址设置是否成功*/
	bool m_bIPSetOK;
	/** 仪器时间设置是否成功*/
	bool m_bTimeSetOK;
	// 仪器是否进行了ADC参数设置
	bool m_bADCSet;
}m_oInstrumentNetStruct;
/**	test type
INSTRUMENT:
	INSTRUMENT NOISE
	INSTRUMENT DISTORTION
	INSTRUMENT CROSSTALK
	INSTRUMENT CMRR
	INSTRUMENT GAIN/PHASE

SENSOR:
	SENSOR RESISTANCE
	SENSOR LEAKAGE
	SENSOR NOISE
	SENSOR TILT
	SENSOR TILT MODEL

MONITOR:
	SENSOR MONITOR	
*/
//common instrument/sensor test
typedef struct CommonDevice_Struct
{
	unsigned int m_uiSN;// 仪器设备号
	unsigned int m_uiIP;// 仪器IP地址
}m_oCommonDeviceStruct;

//common test result
typedef struct CommonDeviceTestResult_Struct
{
	m_oCommonDeviceStruct m_oDevice;
	unsigned int m_uiTestType; //测试类型
	float m_fTestValue;//测试结果
	char m_cTestCheck; //测试检查位
}m_oCommonDeviceTestResultStruct;

