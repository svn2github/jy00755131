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
// ADC数据查询发送Socket接收端口
#define ADCFrameRetransmissionPort	0x8207
// IP广播地址
#define IPBroadcastAddr	"255.255.255.255"
// 最多连接的设备数
#define InstrumentMaxCount	4	
// 首包计数
#define HeadFrameCount		5
// 尾包计数
#define TailFrameCount		5
// 绘图控件个数
#define GraphViewNum	3
// 接收数据包的大小
#define ReceiveDataSize		74
// ADC数据文件文件头行数
#define ADCFileHeadRow	3
// ADC数据小数点后位数
#define DecimalPlaces	9
// XML配置文件名
#define XMLFileName		"MatrixTestSoft.xml"
// ADC数据帧重发次数
#define ADCFrameRetransmissionNum	1
// ADC数据端口接收缓冲区大小
#define ADCBufSize	655360
// ADC接收缓冲区个数
#define ADCRecBufNum	8
// 记录ADC信息数据的个数
#define ADCFrameInfoSize	1
// ADC数据保存帧个数
#define ADCFrameNumNeedToSave	10
// ADC数据处理缓冲区大小
#define ADCDataProcBufSize	((ReceiveDataSize + ADCFrameInfoSize) * ADCFrameNumNeedToSave)
// ADC数据用于临时文件的缓冲区个数
#define ADCDataNeedToSaveBufNum	2
// ADC数据用于临时文件的缓冲区大小
#define ADCDataNeedToSaveBufSize	(ReceiveDataSize * ADCFrameNumNeedToSave * ADCDataNeedToSaveBufNum)
// ADC数据文件存储缓冲区个数
#define ADCDataSaveToFileBufNum	10	
// ADC数据文件存储缓冲区大小
#define ADCDataSaveToFileBufSize	(ReceiveDataSize * ADCFrameNumNeedToSave * ADCDataSaveToFileBufNum)	
// 端口监视端口缓冲区大小
#define PortMonitoringBufSize	655360
// 串口转UDP端口缓冲区大小
#define UartToUDPBufSize	655360
// ADC数据重发端口缓冲区大小
#define ADCRetransmissionBufSize	655360
// UDP通讯ADC采样接收缓冲区大小
#define ADCDataBufSize			4096
// 心跳发送缓冲区大小
#define HeartBeatSndBufSize	3
// 接收的网络数据包大小
#define RcvFrameSize	256
// 发送的网络数据包大小
#define SndFrameSize	128
// 发送设置命令
#define SendSetCmd	0x0001
// 发送查询命令
#define SendQueryCmd	0x0002
// 发送ADC采样数据重发命令
#define SendADCRetransmissionCmd	0x03
// CRC校验值字节数
#define CRCSize			2
// CRC校验预留字节数
#define CRCCheckSize	4
// 帧头长度
#define FrameHeadSize	16

// 发送尾包时刻查询帧计数
extern unsigned int m_uiSendTailTimeFrameCount;
// 接收到的尾包时刻查询帧计数
extern unsigned int m_uiRecTailTimeFrameCount;
// CRC16校验
extern unsigned short get_crc_16(unsigned char* buf, int buf_size);
// extern HANDLE	hCom;				// 准备打开的串口的句柄