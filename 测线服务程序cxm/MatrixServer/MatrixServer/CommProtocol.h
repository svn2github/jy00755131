
#ifndef _CommProtocol_H
#define _CommProtocol_H
/************************************************************************/
/* 常量设置                                                             */
/************************************************************************/
// 仪器SN位数
#define InstrumentBytes						4
// 仪器表网格大小，仪器SN为0则表示无仪器
#define InstrumentTableWindowSize			338
// 测试结果位数
#define TestResultBytes						1
// 网络传输的数据位数
#define DataBytes							3
// 数据表网格大小
#define DataTableWindowSize					338
// C/S接收缓冲区大小
#define ServerRecBufferSize					8192000
// C/S发送缓冲区大小
#define ServerSndBufferSize					8192000
// 最多连接的客户端个数
#define ListenClientMaxNum					5
// 网络接收缓冲区大小
#define ServerRecBufSize					8192000
// 最大接收帧结构体数
#define FrameStructNumMax					5800
// 线程关闭的等待时间
#define CloseClientCommThreadTimes			500
// 线程执行的间隔时间
#define RunClientCommThreadTimes			100

/************************************************************************/
/* 命令字和通讯协议                                                     */
/************************************************************************/
// 查询某个区域客户端只需发送行号和区域号
// 服务端则在区域号和行号后再加入仪器或数值
// 查询仪器列表某个区域（4个字节的行号和4个字节的区域序号）

// 应答命令位（区别应答，进行与操作判别）
#define CmdReturnBit						(0x0001 << 15)
// 命令类型为设置命令
#define CmdTypeSet							0x01
// 命令类型为应答
#define CmdTypeReturn						0x02
// TCP/IP帧长限制
#define FrameSizeLimit						1400
// 帧头长度
#define FrameHeadSize						4
// 帧长长度
#define FrameLengthSize						2
// 帧头信息长度
#define FrameHeadInfoSize					27
// 帧尾长度
#define FrameTailSize						1
// 帧内容限制
#define FrameInfoSizeLimit					(FrameSizeLimit-FrameHeadSize-FrameLengthSize-FrameHeadInfoSize-FrameTailSize)
// 帧头
#define FrameHead1							0xaa
// 帧头
#define FrameHead2							0xa5
// 帧头
#define FrameHead3							0x5a
// 帧头
#define FrameHead4							0x55
// 帧尾
#define FrameTail							0xff
// 帧命令执行结果-执行成功
#define CmdResultSuccess					0x01
// 帧命令执行结果-执行失败
#define CmdResultFail						0x02
// 帧命令执行结果-需要延时执行
#define CmdResultDelay						0x03
// 帧命令执行结果-该命令已经执行
#define CmdResultDone						0x04
// 测试结果-合格
#define TestResultQualified					0x01
// 测试结果-不合格
#define TestResultFailure					0x02
// 测试结果-无数据
#define TestResultNull						0x03
// 客户端向服务端发送验证码
#define CommCheck							"天津海泰光电科技有限公司"

// 客户端登陆验证（帧内容为验证码）
#define CmdClientConnect					0x0001
// 客户端心跳命令（帧内容为空）
#define CmdSetHeartBeat						0x0002
// 查询整个仪器表（帧内容为行号+区域号+仪器SN，SN = 0表明无仪器）
#define CmdQueryWholeTable					0x0003
// 发送Update Table（帧内容为行号+区域号+仪器SN，SN = 0表明无仪器）
#define CmdSendUpdateTable					0x0004
// 上电（命令字后帧内容为空，返回值为执行FieldOn剩余时间，为0表示无需等待）
#define CmdSetFieldOn						0x0005
// 断电（命令字后帧内容为空）
#define CmdSetFieldOff						0x0006
// 查询所选仪器全部信息（帧内容为仪器SN，每4个字节一个仪器）
#define CmdQueryInstrumentInfo				0x0007
// 查询全部仪器的全部信息（应答帧内容为仪器结构体）
#define CmdQueryInstrumentInfoAll			0x0008

// 查询 SurveyXML 文件信息（帧内容为空）
#define CmdQuerySurveyXMLInfo				0x1001
// 设置 SurveyXML 文件信息（帧内容为信息结构体）
#define CmdSetSurveyXMLInfo					0x1002
// 查询 PointCode XML文件信息（帧内容为空）
#define CmdQueryPointCodeXMLInfo			0x1003
// 设置 PointCode XML文件信息（帧内容为信息结构体）
#define CmdSetPointCodeXMLInfo				0x1004
// 查询 Sensor XML文件信息（帧内容为空）
#define CmdQuerySensorXMLInfo				0x1005
// 设置 Sensor XML文件信息（帧内容为信息结构体）
#define CmdSetSensorXMLInfo					0x1006
// 查询 Marker XML文件信息（帧内容为空）
#define CmdQueryMarkerXMLInfo				0x1007
// 设置 Marker XML文件信息（帧内容为信息结构体）
#define CmdSetMarkerXMLInfo					0x1008
// 查询 Aux XML文件信息（帧内容为空）
#define CmdQueryAuxXMLInfo					0x1009
// 设置 Aux XML文件信息（帧内容为信息结构体）
#define CmdSetAuxXMLInfo					0x100a
// 查询 Detour XML文件信息（帧内容为空）
#define CmdQueryDetourXMLInfo				0x100b
// 设置 Detour XML文件信息（帧内容为信息结构体）
#define CmdSetDetourXMLInfo					0x100c
// 查询 Mute XML文件信息（帧内容为空）
#define CmdQueryMuteXMLInfo					0x100d
// 设置 Mute XML文件信息（帧内容为信息结构体）
#define CmdSetMuteXMLInfo					0x100e
// 查询 BlastMachine XML文件信息（帧内容为空）
#define CmdQueryBlastMachineXMLInfo			0x100f
// 设置 BlastMachine XML文件信息（帧内容为信息结构体）
#define CmdSetBlastMachineXMLInfo			0x1010
// 查询 Absolute XML文件信息（帧内容为空）
#define CmdQueryAbsoluteXMLInfo				0x1011
// 设置 Absolute XML文件信息（帧内容为信息结构体）
#define CmdSetAbsoluteXMLInfo				0x1012
// 查询 Generic XML文件信息（帧内容为空）
#define CmdQueryGenericXMLInfo				0x1013
// 设置 Generic XML文件信息（帧内容为信息结构体）
#define CmdSetGenericXMLInfo				0x1014
// 查询 Look XML文件信息（帧内容为空）
#define CmdQueryLookXMLInfo					0x1015
// 设置 Look XML文件信息（帧内容为信息结构体）
#define CmdSetLookXMLInfo					0x1016
// 查询 InstrumentTestBase XML文件信息（帧内容为空）
#define CmdQueryInstrumentTestBaseXMLInfo	0x1017
// 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）
#define CmdSetInstrumentTestBaseXMLInfo		0x1018
// 查询 SensorTestBase XML文件信息（帧内容为空）
#define	CmdQuerySensorTestBaseXMLInfo		0x1019
// 设置 SensorTestBase XML文件信息（帧内容为信息结构体）
#define	CmdSetSensorTestBaseXMLInfo			0x101a
// 查询 InstrumentTestLimit XML文件信息（帧内容为空）
#define CmdQueryInstrumentTestLimitXMLInfo	0x101b
// 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）
#define CmdSetInstrumentTestLimitXMLInfo	0x101c
// 查询 SensorTestLimit XML文件信息（帧内容为空）
#define CmdQuerySensorTestLimitXMLInfo		0x101d
// 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）
#define CmdSetSensorTestLimitXMLInfo		0x101e
// 查询 InstrumentTest XML文件信息（帧内容为空）
#define CmdQueryInstrumentTestXMLInfo		0x101f
// 设置 InstrumentTest XML文件信息（帧内容为信息结构体）
#define CmdSetInstrumentTestXMLInfo			0x1020
// 查询 SensorTest XML文件信息（帧内容为空）
#define CmdQuerySensorTestXMLInfo			0x1021
// SensorTest XML文件信息（帧内容为信息结构体）
#define CmdSetSensorTestXMLInfo				0x1022
// 查询 MultipleTest XML文件信息（帧内容为空）
#define CmdQueryMultipleTestXMLInfo			0x1023
// 设置 MultipleTest XML文件信息（帧内容为信息结构体）
#define CmdSetMultipleTestXMLInfo			0x1024
// 查询 SeisMonitorTest XML文件信息（帧内容为空）
#define CmdQuerySeisMonitorTestXMLInfo		0x1025
// 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）
#define CmdSetSeisMonitorTestXMLInfo		0x1026

// 查询所选区域仪器噪声测试数据和测试结果（帧内容为行号+区域号）
#define CmdQueryInstrNoiseTestArea			0x2001
// 查询全部仪器的仪器噪声测试数据和测试结果（帧内容为空）
#define CmdQueryInstrNoiseTestAll			0x2002
// 查询所选区域仪器失真测试数据和测试结果（帧内容为行号+区域号）
#define CmdQueryInstrDistortionTestArea		0x2003
// 查询全部仪器失真测试数据和测试结果（帧内容为空）
#define CmdQueryInstrDistortionTestAll		0x2004
// 查询所选区域仪器串扰测试数据和测试结果（帧内容为行号+区域号）
#define CmdQueryInstrCrosstalkTestArea		0x2005
// 查询全部仪器串扰测试数据和测试结果（帧内容为空）
#define CmdQueryInstrCrosstalkTestAll		0x2006
// 查询所选区域仪器共模抑制比测试数据和测试结果（帧内容为行号+区域号）
#define CmdQueryInstrCMRRTestArea			0x2007
// 查询全部仪器共模抑制比测试数据和测试结果（帧内容为空）
#define CmdQueryInstrCMRRTestAll			0x2008
// 查询所选区域仪器增益相位测试数据和测试结果（帧内容为行号+区域号）
#define CmdQueryInstrGainPhaseTestArea		0x2009
// 查询全部仪器增益相位测试数据和测试结果（帧内容为空）
#define CmdQueryInstrGainPhaseTestAll		0x200a
// 查询所选区域检波器阻抗测试数据和测试结果（帧内容为行号+区域号）
#define CmdQuerySensorResistanceTestArea	0x200b
// 查询全部检波器阻抗测试数据和测试结果（帧内容为空）
#define CmdQuerySensorResistanceTestAll		0x200c
// 查询所选区域检波器漏电测试数据和测试结果（帧内容为行号+区域号）
#define CmdQuerySensorLeakageTestArea		0x200d
// 查询全部检波器漏电测试数据和测试结果（帧内容为空）
#define CmdQuerySensorLeakageTestAll		0x200e
// 查询所选区域检波器噪声测试数据和测试结果（帧内容为行号+区域号）
#define CmdQuerySensorNoiseTestArea			0x200f
// 查询全部检波器噪声测试数据和测试结果（帧内容为空）
#define CmdQuerySensorNoiseTestAll			0x2010
// 查询所选区域检波器倾斜度测试数据和测试结果（帧内容为行号+区域号）
#define CmdQuerySensorTiltTestArea			0x2011
// 查询全部检波器倾斜度测试数据和测试结果（帧内容为空）
#define CmdQuerySensorTiltTestAll			0x2012
// 查询所选区域检波器倾斜度模式测试数据和测试结果（帧内容为行号+区域号）
#define CmdQuerySensorTiltModelTestArea		0x2013
// 查询全部检波器倾斜度模式测试数据和测试结果（帧内容为空）
#define CmdQuerySensorTiltModelTestAll		0x2014
// 查询所选区域地震监测测试数据和测试结果（帧内容为行号+区域号）
#define CmdQuerySeisMonitorTestArea			0x2015
// 查询全部地震监测测试数据和测试结果（帧内容为空）
#define CmdQuerySeisMonitorTestAll			0x2016
			
// 服务端与客户端通讯帧结构
/************************************************************************/
/* 业务逻辑：先找到帧头，帧头校验，成功则继续，失败则重新找帧头；读取帧内
   容总长度，验证其合理性，不合理则重新找帧头；能找到帧尾则继续，否则先不
   处理；验证帧尾，正确则对帧内容进行业务处理，错误则重新找帧头         */
/************************************************************************/
typedef struct CommFrame_Struct
{
// 	// 帧头
// 	char m_cFrameHead[4];
// 	// 从帧长之后到帧尾（包含帧尾）为帧内容总长度，需要小于帧长限制
// 	unsigned short m_usFrameLength;
	/** 帧命令字、时间戳、包流水号均相同，
	则认为是同一帧*/
	// 帧命令类型，为1则为命令，为3则为应答
	char m_cCmdType;
	// 帧命令字，表明帧的功能
	unsigned short m_usCmd;
	// 服务端时间戳
	unsigned int m_uiServerTimeStep;
	// 客户端时间戳
	unsigned int m_uiClientTimeStep;
	// 包流水号
	unsigned int m_uiPacketIndex;
	// 命令流水号
	unsigned int m_uiCmdIndex;
	// 总帧数
	unsigned int m_uiFrameNum;
	// 帧序号
	unsigned int m_uiFrameIndex;
	// 帧命令执行结果
	char m_cResult;
	// 帧内容长度 = 帧内容总长度 – 2 – 4 – 4 – 4 - 1；
	/** FieldOn命令会应答4个字节表明客户端FieldOn需要等待的时间，
	以秒为单位为0则执行，否则等待)。*/
	unsigned short m_usFrameInfoSize;
	// 帧内容指针，如果为查询命令则帧内容的前两个字节分别定义行号和区域号
	char m_pcFrameInfo[FrameInfoSizeLimit];
// 	// 帧尾
// 	char m_cFrameTail;
}m_oCommFrameStruct, *m_oCommFrameStructPtr;
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
typedef struct InstrumentNet_Struct 
{
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 测线*/
	int m_iLineIndex;
	/** 测点号*/
	int m_iPointIndex;
	/** 标记线号*/
	unsigned int m_uiLineNb;
	/** 标记点号*/
	unsigned int m_uiPointNb;
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
	bool m_bStopMarking;
	/** 是否标记点*/
	bool m_bMarker;
	/** 是否哑道*/
	bool m_bMute;
}m_oInstrumentNetStruct;

#endif