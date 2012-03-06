
#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _Instrument_H
#define   _Instrument_H

// 仪器属性结构体
typedef struct Instrument_Struct 
{
	/** 在仪器数组中的位置*/
	unsigned int m_uiIndex;
	/** 仪器是否使用中*/
	bool m_bInUsed;
	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站；4-LCI*/
	int m_iInstrumentType;
	/** 仪器IP地址*/
	unsigned int m_uiIP;
	/** 仪器路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	int m_iRoutDirection;
	/** 路由地址 交叉线方向 上方*/
	unsigned int m_uiRoutIPTop;
	/** 路由地址 交叉线方向 下方*/
	unsigned int m_uiRoutIPDown;
	/** 路由地址 测线线方向 左方*/
	unsigned int m_uiRoutIPLeft;
	/** 路由地址 测线线方向 右方*/
	unsigned int m_uiRoutIPRight;
	/** 路由开关*/
	byte m_byLAUXRoutOpenSet;
	/** 链接的仪器 上方*/
	Instrument_Struct* m_pInstrumentTop;
	/** 链接的仪器 下方*/
	Instrument_Struct* m_pInstrumentDown;
	/** 链接的仪器 左方*/
	Instrument_Struct* m_pInstrumentLeft;
	/** 链接的仪器 右方*/
	Instrument_Struct* m_pInstrumentRight;
	/** 首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	// 	/** 尾包时刻*/
	// 	unsigned int m_uiTailSysTime;
	/** 尾包计数*/
	int m_iTailFrameCount;
	/** 仪器时延*/
	unsigned int m_uiDelayTime;
	/** 仪器本地系统时间*/
	unsigned int m_uiSystemTime;
	/** 仪器网络时间*/
	unsigned int m_uiNetTime;
	/** 仪器网络状态*/
	unsigned int m_uiNetState;
	/** 仪器参数备用1*/
	unsigned int m_uiNetOrder;
	/** 16bits时间，接收时刻低位*/
	unsigned short m_usReceiveTime;	
	/** 16bits时间，发送时刻低位*/
	unsigned short m_usSendTime;
	/** 16bits 测线方向左面尾包接收时刻*/
	unsigned short m_usLineLeftReceiveTime;	
	/** 16bits 测线方向右面尾包接收时刻*/
	unsigned short m_usLineRightReceiveTime;	
	/** 16bits 交叉线方向上面尾包接收时刻*/
	unsigned short m_usCrossTopReceiveTime;	
	/** 16bits 交叉线方下面尾包接收时刻*/
	unsigned short m_usCrossDownReceiveTime;	
	/** 采集站或电源站的广播端口*/
	unsigned int m_uiBroadCastPort;
	/** 测线*/
	int m_iLineNb;
	/** 测点号*/
	int m_iPointIndex;
	/** 标记点号*/
	unsigned int m_uiPointNb;
	// 	/** 测道*/
	// 	unsigned int m_uiChannelNb;
	/** 是否跳过道*/
	bool m_bJumpedChannel;
	/** 是否连接检波器*/
	bool m_bSensor;
	/** 是否辅助道*/
	bool m_bAux;
	/** 是否连接爆炸机*/
	bool m_bBlastMachine;
	/** 是否迂回道*/
	bool m_bDetour;
	/** 是否迂回道低端标记点*/
	bool m_bDetourMarkerLow;
	/** 是否迂回道高端标记点*/
	bool m_bDetourMarkerHigh;
	/** 是否迂回道停止标记*/
	unsigned int m_uiStopMarking;
	/** 是否标记点*/
	bool m_bMarker;
	// 首包位置稳定次数
	int m_iHeadFrameStableNum;
	/** 第几次设置IP地址*/
	int m_iIPSetCount;
	/** IP地址设置是否成功*/
	bool m_bIPSetOK;
	/** 第几次尾包时刻查询*/
	int m_iTailTimeQueryCount;
	/** 尾包时刻查询应答次数*/
	int m_iTailTimeReturnCount;
	/** 尾包时刻查询是否成功*/
	bool m_bTailTimeQueryOK;
	/** 本地时间修正高位*/
	unsigned int m_uiTimeHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiTimeLow;
	/** 第几次设置仪器时间*/
	int m_iTimeSetCount;
	/** 仪器时统成功次数*/
	int m_iTimeSetReturnCount;
	/** 仪器时间设置是否成功*/
	bool m_bTimeSetOK;
	// ADC命令设置是否应答
	bool m_bADCSetReturn;
	// 仪器是否进行了ADC参数设置
	bool m_bADCSet;
	// 仪器开始ADC数据采集设置成功
	bool m_bADCStartSample;
	// 仪器停止ADC数据采集设置成功
	bool m_bADCStopSample;
	// 误码查询发送帧数
	unsigned int m_uiErrorCodeQueryNum;
	// 误码查询应答帧数
	unsigned int m_uiErrorCodeReturnNum;
	// 采集站和电源站网络数据错误计数
	int m_iFDUErrorCodeDataCount;
	// 采集站和电源站命令错误计数
	int m_iFDUErrorCodeCmdCount;
	// 采集站和电源站网络数据错误计数
	char m_byFDUErrorCodeDataCountOld;
	// 采集站和电源站命令错误计数
	char m_byFDUErrorCodeCmdCountOld;

	// 交叉站大线A数据故障
	int m_iLAUXErrorCodeDataLineACount;
	// 交叉站大线B数据故障
	int m_iLAUXErrorCodeDataLineBCount;
	// 交叉站交叉线A数据故障
	int m_iLAUXErrorCodeDataLAUXLineACount;
	// 交叉站交叉线B数据故障
	int m_iLAUXErrorCodeDataLAUXLineBCount;
	// 交叉站命令口故障
	int m_iLAUXErrorCodeCmdCount;

	// 交叉站大线A数据故障
	char m_byLAUXErrorCodeDataLineACountOld;
	// 交叉站大线B数据故障
	char m_byLAUXErrorCodeDataLineBCountOld;
	// 交叉站交叉线A数据故障
	char m_byLAUXErrorCodeDataLAUXLineACountOld;
	// 交叉站交叉线B数据故障
	char m_byLAUXErrorCodeDataLAUXLineBCountOld;
	// 交叉站命令口故障
	char m_byLAUXErrorCodeCmdCountOld;

	// 实际接收ADC数据帧数（含重发帧）
	unsigned int m_uiADCDataActualRecFrameNum;
	// 重发查询帧得到的应答帧数
	unsigned int m_uiADCDataRetransmissionFrameNum;
	// 应该接收ADC数据帧数（含丢帧）
	unsigned int m_uiADCDataShouldRecFrameNum;
	// ADC数据帧的指针偏移量
	unsigned short m_usADCDataFramePoint;
	// ADC数据帧发送时的本地时间
	unsigned int m_uiADCDataFrameSysTime;
	// ADC数据帧起始帧数
	int m_iADCDataFrameStartNum;
}m_oInstrumentStruct;

#endif