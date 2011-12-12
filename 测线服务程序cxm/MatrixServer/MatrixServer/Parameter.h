#pragma once
// 一次休眠的时间
#define OneSleepTime						50
// 心跳线程发送心跳帧延时次数
#define HertBeatSleepTimes					5
// 首包线程接收首包延时次数
#define HeadFrameSleepTimes					1

// 等待所有线程关闭的延时次数
#define CloseAllThreadSleepTimes			4
// Field On野外测线服务启动
#define	FieldOn								1
// Field On野外测线服务关闭
#define FieldOff							0

// 接收的网络数据帧帧长度
#define RcvFrameSize						256
// 发送的网络数据帧帧长度
#define SndFrameSize						128

// 服务器和客户端之间信息交互网络帧缓冲区大小
#define NetInterfaceBufSize					65536

// 仪器类型-交叉站
#define InstrumentTypeLAUX					1
// 仪器类型-电源站
#define InstrumentTypeLAUL					2
// 仪器类型-采集站
#define InstrumentTypeFDU					3
// 仪器类型-LCI
#define InstrumentTypeLCI					4
// 方向上方
#define DirectionTop						1
// 方向上方
#define DirectionDown						2
// 方向上方
#define DirectionLeft						3
// 方向上方
#define DirectionRight						4
// 方向正中
#define DirectionCenter						0

//________________服务器与客户端命令字设置_________________
// 命令1-Field ON/OFF
#define FieldCmd							0x0001
// 命令2-FormLine
#define FormLineCmd							0x0002
// 命令3-Look
#define LookCmd								0x0003
// 命令4-Test From InstrumentTestSetup
#define InstrumentTestCmd					0x0004
// 命令5-Test From SensorTestSetup
#define SensorTestCmd						0x0005
// 命令6-Test From MultipleTestSetup
#define MultipleTestCmd						0x0006
// 命令7-Test From SeismonitorTestSetup
#define SeismonitorTestCmd					0x0007
// 命令8-Test From InstrumentView 来自仪器视图的“GO”按钮
#define InstrumentViewTestCmd				0x0008
// 命令9-Test From SensorTestView	来自检波器视图的“GO”按钮
#define SensorTestViewTestCmd				0x0009
// 命令	10-Test From SeismonitoView
#define SeismonitoViewTestCmd				0x000a
// 帧头命令字高位
#define FrameHeadHigh						0xEB
// 帧头命令字低位
#define FrameHeadLow						0x90
// 帧尾命令字高位
#define FrameTailHigh						0x14
// 帧尾命令字低位
#define FrameTailLow						0x6F
// 帧类型 0xFF-回令帧
#define FrameTypeReturnCmd					0xFF
// 帧类型 0x01-命令帧，不要求回令帧
#define FrameTypeNoReturnCmd				0x01
// 生成现场数据输出通知命令
#define CmdGenerateOutputData				0x64
// 命令 101-Server Field ON/OFF
#define CmdServerFieldStatus				0x65

//_________________服务器与设备命令字设置__________________
// 发送设置命令
#define SendSetCmd									0x0001
// 发送查询命令
#define SendQueryCmd							0x0002
// 发送ADC采样数据重发命令
#define SendADCCmd								0x0003
// TB开始采集开关控制命令(TB_L高8位)
#define CmdTBCtrl									0x050000
// 串号
#define CmdSn											0x01
// 首包时间
#define CmdHeadFrameTime					0x02
// 本地IP地址
#define CmdLocalIPAddr							0x03
// 本地系统时间
#define CmdLocalSysTime						0x04
// 本地时间修正高位
#define CmdLocalFixedTimeHigh			0x05
// 本地时间修正低位
#define CmdLocalFixedTimeLow			0x06
// 自动数据返回地址
#define CmdADCDataReturnAddr			0x07
// 自动数据返回端口和命令
#define CmdADCDataReturnPort			0x08
// 端口递增下限和上限
#define CmdADCDataReturnPortLimit	0x09
// 设置网络等待端口和命令
#define CmdSetBroadCastPort					0x0a
// 系统硬件状态拷贝
#define CmdFDUErrorCode						0x0b
// TB时刻高位
#define CmdTBHigh									0x0c
// TB时刻低位
#define CmdTbLow									0x0d
// work_ctrl 交叉站方向
#define CmdLAUXRoutOpenQuery		0x0e
// 路由开关
#define CmdLAUXRoutOpenSet			0x0f
// 尾包接收\发送时刻低位
#define CmdTailRecSndTimeLow			0x16
// 广播命令等待端口匹配
#define CmdBroadCastPortSet					0x17
// 设置ADC控制命令命令字
#define CmdADCSet								0x18
// 网络时刻
#define CmdNetTime								0x19
// 交叉站大线尾包接收时刻
#define CmdLineTailRecTimeLAUX		0x1b
// 交叉站交叉线尾包接收时刻
#define CmdLAUTailRecTimeLAUX		0x1c
// 交叉站故障1
#define CmdLAUXErrorCode1				0x1d
// 交叉站故障2
#define CmdLAUXErrorCode2				0x1e
// 交叉站路由分配
#define CmdLAUXSetRout						0x1f
// ADC数据图形化显示抽样率
#define CmdADCDataSamplingRate		0x20
// 返回路由
#define CmdReturnRout							0x3f

//___________________帧格式设置___________________
// 帧头长度
#define FrameHeadSize				16
// 帧头校验字长度
#define FrameHeadCheckSize		4
// 帧头第一个字节
#define FrameHeadCheck0			0x11
// 帧头第二个字节
#define FrameHeadCheck1			0x22
// 帧头第三个字节
#define FrameHeadCheck2			0x33
// 帧头第四个字节
#define FrameHeadCheck3			0x44
// 命令字长度1字节
#define FrameCmdSize1B			1
// 命令包长度2字节
#define FramePacketSize2B			2
// 命令包长度4字节
#define FramePacketSize4B			4
// ADC数据所占字节数
#define ADCDataSize3B				3
// 发送帧缓冲区初值设定
#define SndFrameBufInit					0x00
// CRC校验值字节数
#define CRCSize							2
// CRC校验预留字节数
#define CRCCheckSize					4
