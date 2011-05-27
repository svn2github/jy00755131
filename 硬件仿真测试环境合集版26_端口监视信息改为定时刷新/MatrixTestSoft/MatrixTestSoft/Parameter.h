//**************************************************
//		通讯端口设置
//**************************************************
// 心跳帧发送创建端口
#define HeartBeatRec					0x7000
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
#define ADSetReturnPort			0x8204
// AD数据采集接收端口
#define ADRecPort						0x8300
// 采集站本地时刻查询端口
#define CollectSysTimePort		0x8205
// 状态查询帧应答端口
#define GetStatusPort					0x8206
// 查询硬件设备出错计数返回端口
#define QueryErrorCodePort		0x8207 
// ADC设置广播端口
#define ADCSetBroadcastPort	0x66666666

//**************************************************
//		缓冲区设置
//**************************************************
// 最多连接的设备数
#define InstrumentMaxCount			49	
// 绘图控件个数
#define InstrumentNum					48
// 接收数据包的大小
#define ReceiveDataSize				74
// 缓冲区个数
#define RcvBufNum						8
// 串口转端口中端口缓冲区大小
#define UartToUdpRcvSize			1024
// ADC数据缓冲区大小
#define ADCBufSize						655360
// 端口监视缓冲区大小
#define PortMonitoringBufSize		655360
// 串口转UDP端口缓冲区大小
#define UartToUDPBufSize			655360
// UDP通讯ADC采样接收缓冲区大小
#define ADCDataBufSize				655360
// 心跳发送缓冲区大小
#define HeartBeatSndBufSize		3
// 接收的网络数据包大小
#define RcvFrameSize					256
// 发送的网络数据包大小
#define SndFrameSize						128
// 发送帧缓冲区初值设定
#define SndFrameBufSet				0x00

//**************************************************
//		常量设置
//**************************************************
// 首包计数
#define HeadFrameCount						4
// IP地址重设次数
#define IPAddrResetTimes						3
// 尾包计数
#define TailFrameCount							15
// ADC数据文件文件头行数
#define ADCFileHeadRow						3
// ADC数据小数点后位数
#define DecimalPlaces							9
// ADC数据帧重发次数
#define ADCFrameRetransmissionNum	1
// 线程关闭等待时间
#define WaitForThreadCloseTime			550
// IP广播地址
#define IPBroadcastAddr		"255.255.255.255"
// XML配置文件名
#define XMLFileName			"MatrixTestSoft.xml"

//**************************************************
//		命令字设置
//**************************************************
// 发送设置命令
#define SendSetCmd								0x0001
// 发送查询命令
#define SendQueryCmd							0x0002
// 发送ADC采样数据重发命令
#define SendADCRetransmissionCmd	0x0003
//**************************************************
//		帧格式设置
//**************************************************
// 帧头长度
#define FrameHeadSize	16
// 帧头第一个字节
#define FrameHead0		0x11
// 帧头第二个字节
#define FrameHead1		0x22
// 帧头第三个字节
#define FrameHead2		0x33
// 帧头第四个字节
#define FrameHead3		0x44
// CRC校验值字节数
#define CRCSize			2
// CRC校验预留字节数
#define CRCCheckSize	4
//**************************************************
//		全局变量设置
//**************************************************
// 发送尾包时刻查询帧计数
extern unsigned int m_uiSendTailTimeFrameCount;
// 接收到的尾包时刻查询帧计数
extern unsigned int m_uiRecTailTimeFrameCount;
// CRC16校验
extern unsigned short get_crc_16(unsigned char* buf, int buf_size);
