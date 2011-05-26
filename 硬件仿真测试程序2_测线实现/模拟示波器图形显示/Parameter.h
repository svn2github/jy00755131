#include "HardwareTestDlg.h"

// 绘图控件个数
#define GraphViewNum	8
// 绘图控件ID号
#define GraphViewId		8
// 采样数据数据处理后结果显示控件ID个数
#define MathValueIdNum	4 
// 开始采集命令
#define StartSampleCmd	81
// 接收数据包的大小
#define ReceiveDataSize	74
// AD数据采集接收端口
#define ADRecPort		0x8300
// AD数据采集命令发送端口
#define ADCmdSend		0x6000
// AD数据采集命令应答端口
#define ADCmdRec		0x6500
// IP广播地址
#define IPBroadcastAddr	"127.0.0.1"
// 源IP地址
#define IPSource		"127.0.0.1"
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
// 最多连接的设备数
#define InstrumentMaxCount	5	
// 首包计数
#define HeadFrameCount		5
// 尾包计数
#define TailFrameCount		5
class CHardwareTestDlg;
extern CHardwareTestDlg* pHardwareTestDlg;