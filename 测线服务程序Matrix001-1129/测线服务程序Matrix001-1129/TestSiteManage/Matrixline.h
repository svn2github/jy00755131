#define FrameADCount		72			//每帧AD数据长度
#define FrameADCount72		72			//每帧AD数据长度
#define ADoffset			2048		//偏移量循环
#define NetSendStep			2000		//网络单步字节数
#define MaxFDUCount			1024		//最大采集个数
#define MaxSample			1000		//最大采样率
#define UDPbufCount			3000		//UDP缓存帧个数
#define UDPRcvBufSize		256000		// 定义接收缓冲区大小
#define PortADListen		0x66666666	//AD采集广播端口
#define PortTimeListen		0x66666666	//尾包时刻查询端口
//采集端口 PortForTestDataFrameBase="20480" 0x5000
#define FrameLength			256			//帧长度
#define FrameSetLength			128			//帧长度
//#define ArrayTestDataOfOne  MaxSample*MaxFDUCount*30	//采集一次 一个测试单元的；74 * 4 * 28= 8288字节数
#define ArrayTestDataOfOne  FrameADCount*MaxFDUCount*32	//采集一次 一个测试单元的；74 * 4 * 28字节数
#define ADdata0				0xFFFFFF		//比较的AD初始值
#define ADcheckrecv			100			//AD发送-应答帧个数
//4个站采集30秒 74*4*4*30=35520,48个站=426240
#define TLOffset		16384			//AD发送-应答帧个数
#define ViewIPStr		"192.168.0.19"		//绘图IP地址
#define ViewPort		0x7800				//绘图端口
//#define ViewGain		10					//绘图抽样率
#define NoiseGain		30				//噪声增益
