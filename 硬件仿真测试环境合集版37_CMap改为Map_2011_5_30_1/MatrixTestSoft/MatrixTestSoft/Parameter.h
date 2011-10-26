#include <string>
using std::string;
using std::wstring;
//______________选项卡控件位置设置______________
// 采样选项卡位置
#define TabSampleCtl									0
// ADC参数设置选项卡位置
#define TabADCSettingsCtl						1
// ADC接收数据显示选项卡位置
#define TabADCDataShowCtl					2
// 串口转UDP选项卡位置
#define TabUartToUdpCtl							3
// 端口监视选项卡位置
#define TabPortMonitoringCtl					4
// 仪器设备EEPROM选项卡位置
#define TabEepromCtl								5
// 端口监视接收帧选项卡位置
#define TabPortMonitoringRecCtl				0
// 端口监视接收帧选项卡位置
#define TabPortMonitoringSndCtl				1
//____________UDP套接字事件消息设置____________
// 串口转UDP套接字事件
#define WM_UARTTOUDPSOCKET		WM_USER + 1001
// 首包UDP套接字事件
#define WM_HEADFRAMESOCKET		WM_USER + 1002
// IP地址设置UDP套接字事件
#define WM_IPSETSOCKET					WM_USER + 1003
// 尾包UDP套接字事件
#define WM_TAILFRAMESOCKET		WM_USER + 1004
// 尾包时刻查询UDP套接字事件
#define WM_TAILTIMESOCKET			WM_USER + 1005
// 本地时间查询UDP套接字事件
#define WM_SYSTIMESOCKET				WM_USER + 1006
// ADC设置UDP套接字事件
#define WM_ADCSETSOCKET				WM_USER + 1007
// ADC数据图形化显示UDP套接字事件
#define WM_ADCGRAPHSOCKET			WM_USER + 1008
//__________________通讯端口设置__________________
// 心跳帧发送创建端口
#define HeartBeatPort					0x7000
// 首包接收端口
#define HeadFramePort				0x8000
// 用于接收IP地址设置应答端口
#define IPSetPort						0x9100
// 尾包接收端口
#define TailFramePort				0x9000
// 用于接收仪器尾包时刻查询的端口号
#define TailTimeFramePort		0x9200
// 用于发送仪器时延设置的端口号
#define TimeSetPort					0x9300
// 数据采样设置应答帧端口
#define ADSetReturnPort			0x8300
// AD数据采集接收端口
#define ADRecPort						0x8400
// 采集站本地时刻查询端口
#define CollectSysTimePort		0x8500
// 查询硬件设备出错计数返回端口
#define QueryErrorCodePort		0x8600 
// ADC数据图形化显示发送端口
#define ADCGraphShowPort		0x7800
// ADC数据图形化显示设置接收端口
#define ADCGraphSetRecPort	0x9401
// EEPROM读写应答端口
#define EepromRecPort			0x8700
// ADC设置广播端口
#define ADCSetBroadcastPort	0x66666666
// 广播端口
#define BroadCastPort				0xffffffff

//___________________缓冲区设置____________________
// 最多连接的设备数
#define InstrumentMaxCount			300
// 绘图控件个数
#define InstrumentNum					300
// 接收数据包的大小
#define ReceiveDataSize				72
// 缓冲区个数
#define RcvBufNum						4
// 心跳发送缓冲区大小
#define HeartBeatSndBufSize		1280
// 接收的网络数据包大小
#define RcvFrameSize					256
// 发送的网络数据包大小
#define SndFrameSize						128
// 串口转端口中端口缓冲区大小
#define UartToUdpRcvSize			1024
// ADC数据缓冲区大小，4K采样率单个设备一秒钟最多上传56帧数据
#define ADCDataBufSize				10000000
// 端口监视缓冲区大小，ADC数据缓冲区大小的2倍
#define PortMonitoringBufSize		10000000
// 串口转UDP端口缓冲区大小
#define UartToUDPBufSize			65536
// 发送帧缓冲区初值设定
#define SndFrameBufInit				0x00
// ADC处理过程中预设的缓冲区帧数
#define ADCDataTempFrameNum	60
// ADC处理过程中预设的缓冲区大小
#define ADCDataTempDataSize	(ADCDataTempFrameNum * ReceiveDataSize)
// EEPROM缓冲区大小
#define EEPROMCapacity				8192
// 每帧读取EEPROM数据的最大值
#define EEPROMPerFrameReadMax	64
// 每帧写入EEPROM数据的最大值
#define EEPROMPerFrameWriteMax	64
//__________________串口端口定义__________________
#define COMNum	10

//_________________串口波特率定义_________________
#define BAUD0		300
#define BAUD1		600
#define BAUD2		1200
#define BAUD3		2400
#define BAUD4		4800
#define BAUD5		9600
#define BAUD6		19200
#define BAUD7		38400
#define BAUD8		43000
#define BAUD9		56000
#define BAUD10	57600
#define BAUD11	115200

//_____________________常量设置____________________
// 首包计数
#define HeadFrameCount						4
// IP地址重设次数
#define IPAddrResetTimes						3
// 交叉站尾包计数
#define TailFrameLAUXCount				5
// 交叉站尾包稳定计数
#define TailFrameStableLAUXCount		8
// 采集站尾包计数
#define TailFrameFDUCount					8
// 采集站尾包稳定计数
#define TailFrameStableFDUCount		10
// 尾包查询计数
#define TailTimeFrameCount					8
// 仪器类型-采集站
#define InstrumentTypeFDU					3
// 仪器类型-电源站
#define InstrumentTypeLAUL				2 
// 仪器类型-交叉站
#define InstrumentTypeLAUX				1
// ADC数据文件文件头行数
#define ADCFileHeadRow						3
// ADC数据小数点后位数
#define DecimalPlaces							8
// ADC数据间隔
#define ADCDataPlaces							2
// ADC数据帧重发次数
#define ADCFrameRetransmissionNum	5
// 硬件存储ADC数据个数缓冲区大小
#define HardwareSaveADCNum			2000
// 线程关闭等待时间
#define WaitForThreadCloseTime			550
// IP地址设置的起始地址
#define IPSetAddrStart							71
// IP地址设置的间隔
#define IPSetAddrInterval						10
// ADC数据帧指针翻转周期
#define ADCFramePointLength				2048
// 交叉站上方路由设置
#define RoutIPTop									0x55555555
// 交叉站下方路由设置
#define RoutIPDown								0x66666666
// 交叉站左方路由设置
#define RoutIPLeft									0x77777777
// 交叉站右方路由设置
#define RoutIPRight								0x88888888
// 点击横向滚动条左侧按钮的移动量
#define ScrollBarLineLeftMove				5
// 点击横向滚动条右侧按钮的移动量
#define ScrollBarLineRightMove			5
// 横向滚动条向左翻页的移动量
#define ScrollBarPageLeftMove			30
// 横向滚动条向右翻页的移动量
#define ScrollBarPageRightMove			30
// ADC零漂校正需要写寄存器的位数
#define ADCZeroDriftWritebytesNum	6
// 查询设备误码种类个数
#define QueryErrorCodeNum					4
// ADC操作过程延时
#define ADCOperationSleepTime						100
// ADC零漂校正过程连续采集延时
#define ADCReadContinuousSleepTime			500
// 端口监视发送线程Sleep时间
#define PortMonitoringSendThreadSleepTime	50
// 端口监视接收线程Sleep时间
#define PortMonitoringRecThreadSleepTime	50
// 心跳线程Sleep时间
#define HeartBeatThreadSleepTime					500
// 设备仪器监视线程Sleep时间
#define InstrumentMonitorThreadSleepTime		1000
// ADC数据接收线程Sleep时间
#define ADCDataRecThreadSleepTime				50
// TB设置延时高位
#define TBSleepTimeHigh									0xa000
// TB设置延时低位
#define TBSleepTimeLow									0x00fa
// ADC数据显示界面每行显示的个数
#define ADCDataShowPerLineNum					6
// IP广播地址
#define IPBroadcastAddr		0xFFFFFFFF
// XML配置文件名
#define XMLFileName			_T("MatrixTestSoft.xml")
// ADC数据转换
#define ADCDataConvert									1
// ADC数据不转换
#define ADCDataNoConvert								0
// 取最小值操作
#define OptMinValue											1
// 取最大值操作
#define OptMaxValue											2
// 判断是否相等操作
#define OptCheckEqule										3
// 比较值后相等的返回值
#define CheckEquleReturn									-1
// 比较值后不相等的返回值
#define CheckNotEquleReturn							-2
// EEPROM列表加入操作
#define EEPROMListAdd									1
// EEPROM列表删除操作
#define EEPROMListDelete								2
//____________________命令字设置__________________
// 发送设置命令
#define SendSetCmd								0x0001
// 发送查询命令
#define SendQueryCmd							0x0002
// 发送ADC采样数据重发命令
#define SendADCCmd							0x0003
// TB开始采集开关控制命令(TB_L高8位)
#define CmdTBCtrl									0x010000
// 串号
#define CmdSn										0x01
// 首包时间
#define CmdHeadFrameTime					0x02
// 本地IP地址
#define CmdLocalIPAddr						0x03
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
#define CmdSetBroadCastPort				0x0a
// 系统硬件状态拷贝
#define CmdFDUErrorCode					0x0b
// TB时刻高位
#define CmdTBHigh								0x0c
// TB时刻低位
#define CmdTbLow								0x0d
// 尾包接收\发送时刻低位
#define CmdTailRecSndTimeLow			0x16
// 广播命令等待端口匹配
#define CmdBroadCastPortSet				0x17
// 设置ADC控制命令命令字
#define CmdADCSet								0x18
// 网络时刻
#define CmdNetTime								0x19
// 交叉站大线尾包接收时刻
#define CmdTailRecTimeLAUX			0x1b
// 交叉站故障1
#define CmdLAUXErrorCode1				0x1d
// 交叉站故障2
#define CmdLAUXErrorCode2				0x1e
// 交叉站路由分配
#define CmdLAUXSetRout					0x1f
// ADC数据图形化显示抽样率
#define CmdADCDataSamplingRate		0x20
// 返回路由
#define CmdReturnRout							0x3f
// ADC数据开始图形化显示标志位
#define StartGraphShow							3
// ADC数据停止图形化显示标志位
#define StopGraphShow							4
// 读EEPROM命令
#define ReadEEPROMCmd						1
// 写EEPROM命令
#define WriteEEPROMCmd						2
//__________________ADC设置命令__________________
// ADC设置正弦波命令
#define SetADCSetSine							{0xb2, 0x00, 0x80, 0x00}
// ADC设置停止采样命令
#define SetADCStopSample					{0x81, 0x11, 0x00, 0x00}
// ADC设置打开TB电源低位
#define SetADCOpenTBPowerLow		{0xa3, 0x00, 0x30, 0x00}
// ADC设置打开TB电源高位
#define SetADCOpenTBPowerHigh		{0xa3, 0x00, 0x30, 0x40}
// ADC设置打开TB开关低位
#define SetADCOpenSwitchTBLow		{0xa3, 0x00, 0xf0, 0x00}
// ADC设置打开TB开关高位
#define SetADCOpenSwitchTBHigh		{0xa3, 0x00, 0xf0, 0x40}
// ADC设置读寄存器
#define SetADCRegisterRead					{0x82, 0x20, 0x0a, 0x00}
// ADC设置写寄存器
#define SetADCRegisterWrite				{0x8d, 0x40, 0x0a, 0x00, 0x52, 0x08, 0x32, 0x03, 0x6f, 0x0c, 0xff, 0x7d, 0x52, 0x40, 0x00, 0x00}
// ADC设置连续采样
#define SetADCReadContinuous			{0x81, 0x10, 0x00, 0x00}
// ADC设置打开零漂校正电源
#define SetADCOpenPowerZeroDrift	{0xa3, 0x05, 0xf8, 0x00}
// ADC设置查询ADC寄存器
#define SetADCRegisterQuery				{0x00, 0x00, 0x00, 0x00}
// ADC设置数据采集同步
#define SetADCSampleSynchronization	{0x81, 0x04, 0x00, 0x00}
// ADC设置零漂偏移矫正
#define SetADCZeroDriftCorrection		{0x81, 0x60, 0x00, 0x00}
// ADC设置打开TB开关
#define SetADCTBSwitchOpen				{0xa3, 0x05, 0xf8, 0x40}
// ADC设置零漂
#define SetADCZeroDriftSetFromIP		{0x87, 0x40, 0x04, 0x00, 0x52, 0x08, 0x32, 0x03}

//___________________帧格式设置___________________
// 帧头长度
#define FrameHeadSize				16
// 帧头校验字长度
#define FrameHeadCheckSize	4
// 帧头第一个字节
#define FrameHeadCheck0		0x11
// 帧头第二个字节
#define FrameHeadCheck1		0x22
// 帧头第三个字节
#define FrameHeadCheck2		0x33
// 帧头第四个字节
#define FrameHeadCheck3		0x44
// 命令字长度1字节
#define FrameCmdSize1B			1
// 命令包长度2字节
#define FramePacketSize2B		2
// 命令包长度4字节
#define FramePacketSize4B		4
// ADC数据所占字节数
#define ADCDataSize3B			3

// CRC校验值字节数
#define CRCSize			2
// CRC校验预留字节数
#define CRCCheckSize	4
//__________________绘图参数设置___________________
// 设置Y轴下限
#define DrawGraphYAxisLower							-2.5
// 设置Y轴上限
#define DrawGraphYAxisUpper								2.5
// 设置Y轴坐标轴标签
#define DrawGraphYAxisLabel								_T("Y")
// 设置X轴坐标轴标签
#define DrawGraphXAxisLabel								_T("X")
// 设置背景颜色
#define DrawGraphSetBackgroundColor				RGB(0, 0, 64)
// 设置栅格颜色
#define DrawGraphSetGridColor							RGB(192, 192, 255)
// 设置绘点颜色
#define DrawGraphSetPlotColor							RGB(255, 255, 255)
//___________________定时器设置____________________
// TabUartToUdp界面串口接收定时器序号
#define TabUartToUdpRecTimerNb						1
// TabUartToUdp界面串口接收定时器延时设置
#define TabUartToUdpRecTimerSet						100
// TabSample界面开始采样定时器序号
#define TabSampleStartSampleTimerNb				1
// TabSample界面开始采样定时器延时设置
#define TabSampleStartSampleTimerSet				1000
// TabSample界面停止采样定时器序号
#define TabSampleStopSampleTimerNb				2
// TabSample界面停止采样定时器延时设置
#define TabSampleStopSampleTimerSet				3000
// TabSample界面查询硬件设备出错计数定时器序号
#define TabSampleQueryErrorCountTimerNb		3
// TabSample界面查询硬件设备出错计数定时器延时设置
#define TabSampleQueryErrorCountTimerSet		1000
// TabSample界面尾包监测定时器序号
#define TabSampleTailMonitorTimerNb				4
// TabSample界面尾包监测定时器延时设置
#define TabSampleTailMonitorTimerSet				1000
// TabSample界面IP地址设置监测定时器序号
#define TabSampleIPSetMonitorTimerNb				5
// TabSample界面IP地址设置监测定时器延时设置
#define TabSampleIPSetMonitorTimerSet				8000
// TabSample界面初始化仪器本地时间的定时器序号
#define TabSampleInitSysTimeTimerNb				6
// TabSample界面初始化仪器本地时间的定时器延时设置
#define TabSampleInitSysTimeTimerSet				1000
// TabSample界面做时统设置的定时器序号
#define TabSampleTimeCalTimerNb						7
// TabSample界面做时统设置的定时器延时设置
#define TabSampleTimeCalTimerSet						2300
// TabSample界面做ADC参数设置的定时器序号
#define TabSampleADCSetTimerNb						8
// TabSample界面做ADC参数设置的定时器延时设置
#define TabSampleADCSetTimerSet						2500
// TabSample界面做ADC零漂校正的定时器序号
#define TabSampleADCZeroDriftTimerNb			9
// TabSample界面做ADC零漂校正的定时器延时设置
#define TabSampleADCZeroDriftTimerSet			2000
// TabSample界面做ADC命令设置应答监视的定时器序号
#define TabSampleADCSetReturnTimerNb			10
// TabSample界面做ADC命令设置应答监视的定时器延时设置
#define TabSampleADCSetReturnTimerSet			1000

// TabPortMonitoring界面刷新接收发送帧数的定时器序号
#define TabPortMonitoringFrameNumTimerNb	1
// TabPortMonitoring界面刷新接收发送帧数的定时器延时设置
#define TabPortMonitoringFrameNumTimerSet	1000
// TabADCDataShow界面刷新接收的ADC数值的定时器序号
#define TabADCDataShowADCTimerNb				1
// TabADCDataShow界面刷新接收的ADC数值的定时器延时设置
#define TabADCDataShowADCTimerSet				3000
// PortMonitoringRec界面刷新接收帧的定时器序号
#define PortMonitoringRecTimerNb						1
// PortMonitoringRec界面刷新接收帧的定时器延时设置
#define PortMonitoringRecTimerSet						1000
// PortMonitoringSend界面刷新接收帧的定时器序号
#define PortMonitoringSendTimerNb						1
// PortMonitoringSend界面刷新接收帧的定时器延时设置
#define PortMonitoringSendTimerSet					1000
//__________________日志记录状态__________________
// 正常工作状态
#define SuccessStatus												1
// 警告状态
#define WarningStatus											2
// 出错状态
#define ErrorStatus													3
//__________________全局变量设置___________________
// 发送尾包时刻查询帧计数
extern unsigned int m_uiSendTailTimeFrameCount;
// 接收到的尾包时刻查询帧计数
extern unsigned int m_uiRecTailTimeFrameCount;
// CRC16校验
extern unsigned short get_crc_16(unsigned char* buf, int buf_size);
// 宽字节字符串转换为多字节字符串
extern string WideCharToMultiChar(wstring str);
// 将含有中文的字符串写入文件
// extern void WriteCHToCFile(CFile *file, CString str);
extern void WriteCHToCFile(CArchive *ar, CString str);
// CString转换为const char*
extern const char* ConvertCStringToConstCharPointer(CString str);
// 
extern char* Convert(char* ch,int dec,int sign);
