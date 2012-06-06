
#ifndef _CommProtocol_H
#define _CommProtocol_H
/**
* @brief 常量设置
*/
/** 仪器SN位数*/
#define InstrumentBytes						4
/** 仪器表网格大小，仪器SN为0则表示无仪器*/
#define InstrumentTableWindowSize			169
/** 测试结果位数*/
#define TestResultBytes						1
/** 网络传输的数据位数*/
#define TestDataBytes						4
/** 数据表网格大小*/
#define DataTableWindowSize					270
/** C/S接收缓冲区大小*/
#define ServerRecBufferSize					8192000
/** C/S发送缓冲区大小*/
#define ServerSndBufferSize					8192000
/** 最多连接的客户端个数*/
#define ListenClientMaxNum					5
/** 网络接收缓冲区大小*/
#define ServerRecBufSize					8192000
/** 最大接收帧结构体数*/
#define FrameStructNumMax					5800
/** 线程关闭的等待时间*/
#define CloseClientCommThreadTimes			800
/** 线程执行的间隔时间*/
#define ClientCommThreadWaitTime			50
/** 线程执行的间隔次数*/
#define ClientCommThreadWaitTimes			10
/** 发送帧延时等待次数*/
#define SndFrameWaitTimes					5
/** 发送帧最多发送次数*/
#define SndFrameMaxNum						5
/** 客户端验证时间次数*/
#define ClientCheckCountNum					50
/** 客户端活跃时间间隔次数*/
#define ClientActiveCountNum				50
/** 数据处理缓冲区大小（20000站*（线号+点号+仪器指针地址））*/
#define ProcBufferSize						240000
/** 两次FieldOn时间间隔*/
#define FieldOnWaitForTime					120000
/** 自定义关闭客户端消息*/
#define CloseClientMsg						(WM_USER + 100)
/**
* @brief 命令字和通讯协议/n
* 查询某个区域客户端只需发送行号和区域号/n
* 服务端则在区域号和行号后再加入仪器属性（SN（4个字节)后跟IP（4个字节）)或数值（数值(4个字节)后跟测试结果（1个字节））/n
* 查询仪器列表某个区域（4个字节的行号和4个字节的区域序号）
*/

/** 应答命令位（区别应答，进行与操作判别）*/
#define CmdReturnBit						(1 << 15)
/** 命令类型为设置命令*/
#define CmdTypeSet							0x01
/** 命令类型为应答*/
#define CmdTypeReturn						0x02
/** TCP/IP帧长限制*/
#define FrameSizeLimit						1400
/** 帧头长度*/
#define FrameHeadSize						4
/** 帧长长度*/
#define FrameLengthSize						2
/** 帧头信息长度*/
#define FrameHeadInfoSize					27
/** 帧尾长度*/
#define FrameTailSize						1
/** 帧内容限制*/
#define FrameInfoSizeLimit					(FrameSizeLimit-FrameHeadSize-FrameLengthSize-FrameHeadInfoSize-FrameTailSize)
/** 帧头*/
#define FrameHead1							0xaa
/** 帧头*/
#define FrameHead2							0xa5
/** 帧头*/
#define FrameHead3							0x5a
/** 帧头*/
#define FrameHead4							0x55
/** 帧尾*/
#define FrameTail							0xff
/** 帧命令执行结果-等待执行*/
#define CmdResultWait						0x00
/** 帧命令执行结果-执行成功*/
#define CmdResultSuccess					0x01
/** 帧命令执行结果-执行失败*/
#define CmdResultFail						0x02
/** 帧命令执行结果-需要延时执行*/
#define CmdResultDelay						0x03
/** 帧命令执行结果-该命令已经执行*/
#define CmdResultDone						0x04
/** 测试结果-合格*/
#define TestResultQualified					0x01
/** 测试结果-不合格*/
#define TestResultFailure					0x02
/** 测试结果-无数据*/
#define TestResultNull						0x03
/** 客户端向服务端发送验证码*/
#define CommCheck							"天津海泰光电科技有限公司"

/** 客户端登陆验证（帧内容为验证码*/
#define CmdClientConnect					0x0001
/** 查询接收区域（帧内容为行数（4个字节）+列数（4个字节））*/
#define CmdQueryRevSection					0x0002
/** 客户端心跳命令（帧内容为空）*/
#define CmdSetHeartBeat						0x0003
/** 查询Whole Table（帧内容为行号+区域号+ 仪器SN+仪器IP，SN = 0表明无仪器）*/
#define CmdQueryWholeTable					0x0004
/** 查询Update Table（帧内容为行号+区域号+ 仪器SN+仪器IP，SN = 0表明无仪器）*/
#define CmdQueryUpdateTable					0x0005
/** 上电（命令字后帧内容为空）*/
#define CmdSetFieldOn						0x0006
/** 断电（命令字后帧内容为空）*/
#define CmdSetFieldOff						0x0007
/** 查询所选仪器全部信息（帧内容为行号+区域号）*/
#define CmdQueryInstrumentInfo				0x0008
/** 查询全部仪器的全部信息（帧内容为仪器结构体）*/
#define CmdQueryInstrumentInfoAll			0x0009
/** Field On需要等待的时间（帧内容为执行FieldOn剩余时间，为0表示无需等待）*/
#define CmdFieldOnWaitTime					0x000a

/** 查询 SurveyXML 文件信息（帧内容为空）*/
#define CmdQuerySurveyXMLInfo				0x1001
/** 设置 SurveyXML 文件信息（帧内容为信息结构体）*/
#define CmdSetSurveyXMLInfo					0x1002
/** 查询 PointCode XML文件信息（帧内容为空）*/
#define CmdQueryPointCodeXMLInfo			0x1003
/** 设置 PointCode XML文件信息（帧内容为信息结构体）*/
#define CmdSetPointCodeXMLInfo				0x1004
/** 查询 Sensor XML文件信息（帧内容为空）*/
#define CmdQuerySensorXMLInfo				0x1005
/** 设置 Sensor XML文件信息（帧内容为信息结构体）*/
#define CmdSetSensorXMLInfo					0x1006
/** 查询 Marker XML文件信息（帧内容为空）*/
#define CmdQueryMarkerXMLInfo				0x1007
/** 设置 Marker XML文件信息（帧内容为信息结构体）*/
#define CmdSetMarkerXMLInfo					0x1008
/** 查询 Aux XML文件信息（帧内容为空）*/
#define CmdQueryAuxXMLInfo					0x1009
/** 设置 Aux XML文件信息（帧内容为信息结构体）*/
#define CmdSetAuxXMLInfo					0x100a
/** 查询 Detour XML文件信息（帧内容为空）*/
#define CmdQueryDetourXMLInfo				0x100b
/** 设置 Detour XML文件信息（帧内容为信息结构体）*/
#define CmdSetDetourXMLInfo					0x100c
/** 查询 Mute XML文件信息（帧内容为空）*/
#define CmdQueryMuteXMLInfo					0x100d
/** 设置 Mute XML文件信息（帧内容为信息结构体）*/
#define CmdSetMuteXMLInfo					0x100e
/** 查询 BlastMachine XML文件信息（帧内容为空）*/
#define CmdQueryBlastMachineXMLInfo			0x100f
/** 设置 BlastMachine XML文件信息（帧内容为信息结构体）*/
#define CmdSetBlastMachineXMLInfo			0x1010
/** 查询 Absolute XML文件信息（帧内容为空）*/
#define CmdQueryAbsoluteXMLInfo				0x1011
/** 设置 Absolute XML文件信息（帧内容为信息结构体）*/
#define CmdSetAbsoluteXMLInfo				0x1012
/** 查询 Generic XML文件信息（帧内容为空）*/
#define CmdQueryGenericXMLInfo				0x1013
/** 设置 Generic XML文件信息（帧内容为信息结构体）*/
#define CmdSetGenericXMLInfo				0x1014
/** 查询 Look XML文件信息（帧内容为空）*/
#define CmdQueryLookXMLInfo					0x1015
/** 设置 Look XML文件信息（帧内容为信息结构体）*/
#define CmdSetLookXMLInfo					0x1016
/** 查询 InstrumentTestBase XML文件信息（帧内容为空）*/
#define CmdQueryInstrumentTestBaseXMLInfo	0x1017
/** 设置 InstrumentTestBase XML文件信息（帧内容为信息结构体）*/
#define CmdSetInstrumentTestBaseXMLInfo		0x1018
/** 查询 SensorTestBase XML文件信息（帧内容为空）*/
#define	CmdQuerySensorTestBaseXMLInfo		0x1019
/** 设置 SensorTestBase XML文件信息（帧内容为信息结构体）*/
#define	CmdSetSensorTestBaseXMLInfo			0x101a
/** 查询 InstrumentTestLimit XML文件信息（帧内容为空）*/
#define CmdQueryInstrumentTestLimitXMLInfo	0x101b
/** 设置 InstrumentTestLimit XML文件信息（帧内容为信息结构体）*/
#define CmdSetInstrumentTestLimitXMLInfo	0x101c
/** 查询 SensorTestLimit XML文件信息（帧内容为空）*/
#define CmdQuerySensorTestLimitXMLInfo		0x101d
/** 设置 SensorTestLimit XML文件信息（帧内容为信息结构体）*/
#define CmdSetSensorTestLimitXMLInfo		0x101e
/** 查询 InstrumentTest XML文件信息（帧内容为空）*/
#define CmdQueryInstrumentTestXMLInfo		0x101f
/** 设置 InstrumentTest XML文件信息（帧内容为信息结构体）*/
#define CmdSetInstrumentTestXMLInfo			0x1020
/** 查询 SensorTest XML文件信息（帧内容为空）*/
#define CmdQuerySensorTestXMLInfo			0x1021
/** SensorTest XML文件信息（帧内容为信息结构体）*/
#define CmdSetSensorTestXMLInfo				0x1022
/** 查询 MultipleTest XML文件信息（帧内容为空）*/
#define CmdQueryMultipleTestXMLInfo			0x1023
/** 设置 MultipleTest XML文件信息（帧内容为信息结构体）*/
#define CmdSetMultipleTestXMLInfo			0x1024
/** 查询 SeisMonitorTest XML文件信息（帧内容为空）*/
#define CmdQuerySeisMonitorTestXMLInfo		0x1025
/** 设置 SeisMonitorTest XML文件信息（帧内容为信息结构体）*/
#define CmdSetSeisMonitorTestXMLInfo		0x1026
/** 查询 LAULeakage XML文件信息（帧内容为空）*/
#define CmdQueryLAULeakageXMLInfo			0x1027
/** 设置 LAULeakage XML文件信息（帧内容为信息结构体）*/
#define CmdSetLAULeakageXMLInfo				0x1028
/** 查询 FormLine XML文件信息（帧内容为空）*/
#define CmdQueryFormLineXMLInfo				0x1029
/** 设置 FormLine XML文件信息（帧内容为信息结构体）*/
#define CmdSetFormLineXMLInfo				0x102a

/** 查询所选区域仪器噪声测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQueryInstrNoiseTestArea			0x2001
/** 查询全部仪器的仪器噪声测试数据和测试结果（帧内容为空）*/
#define CmdQueryInstrNoiseTestAll			0x2002
/** 查询所选区域仪器失真测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQueryInstrDistortionTestArea		0x2003
/** 查询全部仪器失真测试数据和测试结果（帧内容为空）*/
#define CmdQueryInstrDistortionTestAll		0x2004
/** 查询所选区域仪器串扰测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQueryInstrCrosstalkTestArea		0x2005
/** 查询全部仪器串扰测试数据和测试结果（帧内容为空）*/
#define CmdQueryInstrCrosstalkTestAll		0x2006
/** 查询所选区域仪器共模抑制比测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQueryInstrCMRRTestArea			0x2007
/** 查询全部仪器共模抑制比测试数据和测试结果（帧内容为空）*/
#define CmdQueryInstrCMRRTestAll			0x2008
/** 查询所选区域仪器增益相位测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQueryInstrGainPhaseTestArea		0x2009
/** 查询全部仪器增益相位测试数据和测试结果（帧内容为空）*/
#define CmdQueryInstrGainPhaseTestAll		0x200a
/** 查询所选区域检波器阻抗测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQuerySensorResistanceTestArea	0x200b
/** 查询全部检波器阻抗测试数据和测试结果（帧内容为空）*/
#define CmdQuerySensorResistanceTestAll		0x200c
/** 查询所选区域检波器漏电测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQuerySensorLeakageTestArea		0x200d
/** 查询全部检波器漏电测试数据和测试结果（帧内容为空）*/
#define CmdQuerySensorLeakageTestAll		0x200e
/** 查询所选区域检波器噪声测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQuerySensorNoiseTestArea			0x200f
/** 查询全部检波器噪声测试数据和测试结果（帧内容为空）*/
#define CmdQuerySensorNoiseTestAll			0x2010
/** 查询所选区域检波器倾斜度测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQuerySensorTiltTestArea			0x2011
/** 查询全部检波器倾斜度测试数据和测试结果（帧内容为空）*/
#define CmdQuerySensorTiltTestAll			0x2012
/** 查询所选区域检波器倾斜度模式测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQuerySensorTiltModelTestArea		0x2013
/** 查询全部检波器倾斜度模式测试数据和测试结果（帧内容为空）*/
#define CmdQuerySensorTiltModelTestAll		0x2014
/** 查询所选区域地震监测测试数据和测试结果（帧内容为行号+区域号）*/
#define CmdQuerySeisMonitorTestArea			0x2015
/** 查询全部地震监测测试数据和测试结果（帧内容为空）*/
#define CmdQuerySeisMonitorTestAll			0x2016

/**
* @brief 服务端与客户端通讯帧结构\n
* 业务逻辑：先找到帧头，帧头校验，成功则继续，失败则重新找帧头；读取帧内\n
* 容总长度，验证其合理性，不合理则重新找帧头；能找到帧尾则继续，否则先不\n
* 处理；验证帧尾，正确则对帧内容进行业务处理，错误则重新找帧头。
*/
typedef struct CommFrame_Struct
{
// 	/** 帧头*/
// 	char m_cFrameHead[4];
// 	/** 从帧长之后到帧尾（包含帧尾）为帧内容总长度，需要小于帧长限制*/
// 	unsigned short m_usFrameLength;
	/** 帧命令字、时间戳、包流水号均相同，则认为是同一帧*/
	/** 帧命令类型，为1则为命令，为3则为应答*/
	char m_cCmdType;
	/** 帧命令字，表明帧的功能*/
	unsigned short m_usCmd;
	/** 服务端时间戳*/
	unsigned int m_uiServerTimeStep;
	/** 客户端时间戳*/
	unsigned int m_uiClientTimeStep;
	/** 包流水号*/
	unsigned int m_uiPacketIndex;
	/** 命令流水号*/
	unsigned int m_uiCmdIndex;
	/** 总帧数*/
	unsigned int m_uiFrameNum;
	/** 帧序号（从1开始）*/
	unsigned int m_uiFrameIndex;
	/** 帧命令执行结果*/
	char m_cResult;
	/** 帧内容长度 = 帧内容总长度 – 2 – 4 – 4 – 4 - 1；*/
	/** FieldOn命令会应答4个字节表明客户端FieldOn需要等待的时间，
	以秒为单位为0则执行，否则等待)。*/
	unsigned short m_usFrameInfoSize;
	/** 帧内容指针，如果为查询命令则帧内容的前两个字节分别定义行号和区域号*/
	char m_pcFrameInfo[FrameInfoSizeLimit];
// 	/** 帧尾*/
// 	char m_cFrameTail;
	/** 超时计数*/
	unsigned int m_uiTimeOutCount;
	/** 发送次数计数*/
	unsigned int m_uiSndCount;
}m_oCommFrameStruct, *m_oCommFrameStructPtr;

/**
* @brief 仪器属性结构体
*/
typedef struct InstrumentNet_Struct 
{
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器IP*/
	unsigned int m_uiIP;
}m_oInstrumentNetStruct;

#endif