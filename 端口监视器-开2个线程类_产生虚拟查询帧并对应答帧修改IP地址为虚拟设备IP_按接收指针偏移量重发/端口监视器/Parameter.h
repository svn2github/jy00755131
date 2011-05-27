#include "PortMonitoringDlg.h"

#define RcvPort1	0x8202	// 目标1接收端口
#define SendPort1	0x9002	// 目标1发送端口
#define RcvPort2	0x9001	// 目标2接收端口
#define SendPort2	0x8201	// 目标2发送端口
#define RcvBufSize	655360	// 定义接收缓冲区大小
#define SndBufSize	655360	// 定义发送缓冲区大小
#define InstrumentForTest	8	// 定义虚拟仪器数
#define ClearListFrameCount	100	// 达到指定帧后清理列表中过期重发帧
#define ADCRetransmissionCmd	0x0003	// ADC数据重发命令
class CPortMonitoringDlg;
extern CPortMonitoringDlg* pDlg;
