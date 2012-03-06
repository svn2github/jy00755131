
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _CommandInfo_H
#define   _CommandInfo_H

// 与设备通讯命令字内容
typedef struct InstrumentCommand_Struct
{
	// LCI的IP地址
	unsigned int m_uiAimIP;
	// LCI接收的端口号
	unsigned short m_usAimPort;
	// 源地址
	unsigned int m_uiSrcIP;
	// 目的地址
	unsigned int m_uiDstIP;
	// 应答端口
	unsigned short m_usReturnPort;
	// 命令，为1则设置命令应答，为2查询命令应答，为3AD采样数据重发
	unsigned short m_usCommand;
	// SN，低8位为仪器类型，0x01为交叉站，0x02为电源站，0x03为采集站
	unsigned int m_uiSN;
	// 首包时刻
	unsigned int m_uiTimeHeadFrame;
	// 仪器IP地址
	unsigned int m_uiInstrumentIP;
	// 本地时间
	unsigned int m_uiSysTime;
	// 本地时间修正高位
	unsigned int m_uiLocalTimeFixedHigh;
	// 本地时间修正低位
	unsigned int m_uiLocalTimeFixedLow;
	// 自动数据返回地址
	unsigned int m_uiADCDataReturnAddr;
	// 自动数据返回端口
	unsigned short m_usADCDataReturnPort;
	// 自动数据返回命令，ad_cmd(7)=1，端口递增；=0，端口不变
	unsigned short m_usADCDataReturnCmd;
	// 当返回命令高位为1时，返回端口自动加1，到高限时返回低限端口
	// 端口递增下限
	unsigned short m_usADCDataReturnPortLimitLow;
	// 端口递增上限
	unsigned short m_usADCDataReturnPortLimitHigh;
	// 设置网络等待端口，指设置接收上位机广播命令的端口
	unsigned int m_uiBroadCastPortSet;
	// 网络数据错误计数
	char m_byFDUErrorCodeDataCount;
	// 命令错误计数
	char m_byFDUErrorCodeCmdCount;
	// 时间报文状态
	char m_byTimeStatus;
	// 控制状态
	char m_byCtrlStatus;
	// TB时刻高位
	unsigned int m_uiTBHigh;
	// TB时刻低位
	unsigned short m_usTBLow;
	// TB控制，0x05启动TB，0x06则AD采集无需TB，0x00停止AD，ad_ctrl(2)=1则LED灯灭
	unsigned short m_usTBCtrl;
	// work_ctrl控制交叉站接收和发送命令的方向
	// 由高到低位每位分别控制发送口交叉线A、B，大线A、B，接收端交叉线A、B，大线A、B
	// =0为开，=1为关
	char m_byLAUXRoutOpenQuery;
	// 路由开关
	// 由高位到低位分别控制开交叉线A、开交叉线B、开大线A、开大线B
	// =0为开，=1为关
	char m_byLAUXRoutOpenSet;
	// 尾包接收时刻，低14位有效
	unsigned short m_usTailRecTime;
	// 尾包发送时刻//交叉站尾包发送时刻，低14位有效
	unsigned short m_usTailSndTime;
	// 广播命令等待端口匹配，必须放在第一个命令字位置，并和0x0a命令中的16位端口匹配才能接收广播命令
	unsigned int m_uiBroadCastPortSeted;
	// 网络时刻
	unsigned int m_uiNetTime;
	// 交叉站大线A尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLineA;
	// 交叉站大线B尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLineB;
	// 交叉站交叉线A尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLAUXLineA;
	// 交叉站交叉线B尾包接收时刻
	unsigned short m_usLAUXTailRecTimeLAUXLineB;
	// 交叉站大线A数据故障
	char m_byLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	char m_byLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	char m_byLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	char m_byLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	char m_byLAUXErrorCodeCmdCount;
	// 交叉站上方路由IP
	unsigned int m_uiRoutIPTop;
	// 交叉站下方路由IP
	unsigned int m_uiRoutIPDown;
	// 交叉站左方路由IP
	unsigned int m_uiRoutIPLeft;
	// 交叉站右方路由IP
	unsigned int m_uiRoutIPRight;
	// 路由IP地址
	unsigned int m_uiRoutIP;
	// 0x18命令数组
	char* m_pcADCSet;
	// 0x18命令数据个数
	int m_iADCSetNum;
	// ADC数据指针偏移量
	unsigned short m_usADCDataPoint;
	// ADC数据采集时仪器本地时间
	unsigned int m_uiADCSampleSysTime;
	// ADC采样数据缓冲区指针
	int* m_pADCData;
}m_oInstrumentCommandStruct;

#endif