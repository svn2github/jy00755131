// 心跳帧发送创建端口
#define HeartBeatRec		0x7000
// 首包接收端口
#define HeadFramePort		0x8000
// 用于接收IP地址设置应答端口
#define IPSetPort			0x9100
// 尾包接收端口
#define TailFramePort		0x9000
// 用于接收仪器尾包时刻查询的端口号
#define TailTimeFramePort	0x9200
// 用于发送仪器时延设置的端口号
#define TimeSetPort			0x9300
// 数据采样设置应答帧端口
#define ADSetReturnPort		0x8204
// AD数据采集接收端口
#define ADRecPort			0x8300
// 采集站本地时刻查询端口
#define CollectSysTimePort	0x8205
// 状态查询帧应答端口
#define	GetStatusPort		0x8206
// ADC设置广播端口
#define ADCSetBroadcastPort	0x6666
// IP广播地址
#define IPBroadcastAddr	"255.255.255.255"
// 最多连接的设备数
#define InstrumentMaxCount	49	
// 首包计数
#define HeadFrameCount		5
// 尾包计数
#define TailFrameCount		5
// 绘图控件个数
#define GraphViewNum	48
// 接收数据包的大小
#define ReceiveDataSize		74
// ADC数据文件文件头行数
#define ADCFileHeadRow	3
// ADC数据小数点后位数
#define DecimalPlaces	9
// ADC数据帧重发次数
#define ADCFrameRetransmissionNum	1
// ADC数据缓冲区大小
#define ADCBufSize	655360
// 端口监视缓冲区大小
#define PortMonitoringBufSize	655360
// 串口转UDP端口缓冲区大小
#define UartToUDPBufSize	655360
// 心跳发送缓冲区大小
#define HeartBeatSndBufSize	3
// 接收的网络数据包大小
#define RcvFrameSize	256
// 发送的网络数据包大小
#define SndFrameSize	128

// 发送尾包时刻查询帧计数
extern unsigned int m_uiSendTailTimeFrameCount;
// 接收到的尾包时刻查询帧计数
extern unsigned int m_uiRecTailTimeFrameCount;
// extern HANDLE	hCom;				// 准备打开的串口的句柄