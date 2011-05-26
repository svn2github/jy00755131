
//__________________串口端口定义__________________
#define COM0 "端口1"
#define COM1 "端口2"
#define COM2 "端口3"
#define COM3 "端口4"
#define COM4 "端口5"
#define COM5 "端口6"
#define COM6 "端口7"
#define COM7 "端口8"
#define COM8 "端口9"

//__________________串口波特率定义________________
#define BAUD0 "300"
#define BAUD1 "600"
#define BAUD2 "1200"
#define BAUD3 "2400"
#define BAUD4 "4800"
#define BAUD5 "9600"
#define BAUD6 "19200"
#define BAUD7 "38400"
#define BAUD8 "43000"
#define BAUD9 "56000"
#define BAUD10 "57600"
#define BAUD11 "115200"

//____________________端口定义_____________________
// UDP接收端口
#define ReceivePort		0x8202
// AD数据采集接收端口
#define ADRecPort		0x8300
// AD数据采集命令发送端口
#define ADCmdSend		0x6000
// AD数据采集命令应答端口
#define ADCmdRec		0x6500
// 心跳帧发送创建端口
#define HeartBeatRec	0x7000
// 发送端口
#define SendPort		0x8202
// 首包接收端口
#define HeadFramePort	0x8000
// 用于接收IP地址设置应答端口
#define IPSetPort		0x9100
// 尾包接收端口
#define TailFramePort	0x9000
// 用于接收仪器尾包时刻查询的端口号
#define TailTimeFramePort	0x9200
// 用于发送仪器时延设置的端口号
#define TimeSetPort		0x9300
// 本机IP地址
#define IPSource		_T("127.0.0.1")
//串口接收从UDP发送的端口数
#define UDPPortNb	6