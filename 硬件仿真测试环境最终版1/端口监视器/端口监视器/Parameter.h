#include "PortMonitoringDlg.h"

#define RcvPort1	0x8202	// 目标1接收端口
#define SendPort1	0x9002	// 目标1发送端口
#define RcvPort2	0x9001	// 目标2接收端口
#define SendPort2	0x8201	// 目标2发送端口

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
// 状态查询帧应答端口
#define	GetStatusPort		0x8206


class CPortMonitoringDlg;

extern CPortMonitoringDlg* pDlg;
