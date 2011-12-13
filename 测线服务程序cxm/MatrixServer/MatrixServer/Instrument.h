#pragma once

/**
*@brief 仪器类
*/
class CInstrument
{
public:
	CInstrument();
	~CInstrument();

public: //属性
	/** 在仪器数组中的位置*/
	unsigned int m_uiIndex;

	/** 仪器是否使用中*/
	bool m_bInUsed;

	/** 仪器设备号*/
	unsigned int m_uiSN;
	/** 仪器类型 1-交叉站；2-电源站；3-采集站*/
	unsigned int m_uiInstrumentType;

	/** 仪器IP地址*/
	unsigned int m_uiIP;
	/** 仪器路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	unsigned int m_uiRoutDirection;
	/** 路由地址 交叉线方向 上方*/
	unsigned int m_uiRoutIPTop;
	/** 路由地址 交叉线方向 下方*/
	unsigned int m_uiRoutIPDown;
	/** 路由地址 测线线方向 左方*/
	unsigned int m_uiRoutIPLeft;
	/** 路由地址 测线线方向 右方*/
	unsigned int m_uiRoutIPRight;

// 	/** 链接的下一个主交叉站*/
// 	CInstrument* m_pMainCrossNext;
// 	/** 链接的上一个主交叉站*/
// 	CInstrument* m_pMainCrossPreview;
// 	/** 链接的交叉站 上方*/
// 	CInstrument* m_pCrossTop;
// 	/** 链接的交叉站 下方*/
// 	CInstrument* m_pCrossDown;
	/** 链接的仪器 上方*/
	CInstrument* m_pInstrumentTop;
	/** 链接的仪器 下方*/
	CInstrument* m_pInstrumentDown;
	/** 链接的仪器 左方*/
	CInstrument* m_pInstrumentLeft;
	/** 链接的仪器 右方*/
	CInstrument* m_pInstrumentRight;

// 	/** 链接的下一个主交叉站*/
// 	unsigned int m_uiSNMainCrossNext;
// 	/** 链接的上一个主交叉站*/
// 	unsigned int m_uiSNMainCrossPreview;
// 	/** 链接的交叉站 上方*/
// 	unsigned int m_uiSNCrossTop;
// 	/** 链接的交叉站 下方*/
// 	unsigned int m_uiSNCrossDown;
// 	/** 链接的仪器 左方*/
// 	unsigned int m_uiSNInstrumentLeft;
// 	/** 链接的仪器 右方*/
// 	unsigned int m_uiSNInstrumentRight;

// 	/** 链接的下一个主交叉站*/
// 	int m_iIndexMainCrossNext;
// 	/** 链接的上一个主交叉站*/
// 	int m_iIndexMainCrossPreview;
// 	/** 链接的交叉站 上方*/
// 	int m_iIndexCrossTop;
// 	/** 链接的交叉站 下方*/
// 	int m_iIndexCrossDown;
// 	/** 链接的仪器 左方*/
// 	int m_iIndexInstrumentLeft;
// 	/** 链接的仪器 右方*/
// 	int m_iIndexInstrumentRight;

	/** 首包时刻*/
	unsigned int m_uiTimeHeadFrame;
	/** 尾包计数*/
	unsigned int m_uiTailFrameCount;
	/** 仪器时延*/
	unsigned int m_uiTimeDelay;
	/** 仪器本地系统时间*/
	unsigned int m_uiTimeSystem;
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

	// 采集站或电源站的广播端口
	unsigned int m_uiBroadCastPort;
// 	/** 测线*/
// 	unsigned int m_uiLineNb;
// 	/** 测点*/
// 	unsigned int m_uiPointNb;
// 	/** 测道*/
// 	unsigned int m_uiChannelNb;

	/** 是否跳过道*/
	bool m_bJumpedChannel;
	/** 是否连接检波器*/
	bool m_bSensor;

// 	/** 仪器IP地址分配时间*/
// 	unsigned int m_uiIPDistributeTime;	

	/** 是否辅助道*/
	bool m_bAux;
// 	/** 辅助道标记时间*/
// 	unsigned int m_uiAuxTime;

	/** 是否连接爆炸机*/
	bool m_bBlastMachine;
// 	/** 爆炸机标记时间*/
// 	unsigned int m_uiBlastMachineTime;

	/** 是否迂回道*/
	bool m_bDetour;
	/** 是否迂回道低端标记点*/
	bool m_bDetourMarkerLow;
	/** 是否迂回道高端标记点*/
	bool m_bDetourMarkerHigh;
	/** 是否迂回道停止标记*/
	unsigned int m_uiStopMarking;
// 	/** 迂回道标记时间*/
// 	unsigned int m_uiDetourLogoTime;
// 	/** 迂回道标记时间*/
// 	unsigned int m_uiDetourTime;

	/** 是否标记点*/
	bool m_bMarker;
// 	/** 标记点标记时间*/
// 	unsigned int m_uiMarkerLogoTime;
// 	/** 标记点标记时间*/
// 	unsigned int m_uiMarkerTime;
	// 首包位置稳定次数
	int m_iHeadFrameStableNum;

	/** 第几次设置IP地址*/
	int m_iIPSetCount;
	/** IP地址设置是否成功*/
	bool m_bIPSetOK;

	/** 第几次尾包时刻查询*/
	int m_iTailTimeQueryCount;
	/** 尾包时刻查询是否成功*/
	bool m_bTailTimeQueryOK;

	/** 本地时间修正高位*/
	unsigned int m_uiTimeHigh;
	/** 本地时间修正低位*/
	unsigned int m_uiTimeLow;
	/** 第几次设置仪器时间*/
	int m_iTimeSetCount;
	/** 仪器时间设置是否成功*/
	bool m_bTimeSetOK;

// 	/** 仪器被删除时的删除方向 1-上；2-下；3-左；4右*/
// 	unsigned int m_uiDeleteDirection;

	/** 仪器噪声测试值*/
	float m_fNoiseValue;
	/** 仪器噪声测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byNoiseCheck;

	/** 仪器畸变测试值*/
	float m_fDistortionValue;
	/** 仪器畸变测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byDistortionCheck;

	/** 仪器串音测试值*/
	float m_fCrosstalkValue;
	/** 仪器串音测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byCrosstalkCheck;

	/** 仪器增益测试值*/
	float m_fGainValue;
	/** 仪器增益测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byGainCheck;
	/** 仪器相位测试值*/
	float m_fPhaseValue;
	/** 仪器相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byPhaseCheck;
	/** 仪器增益和相位测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byGainAndPhaseCheck;

	/** 仪器共模测试值*/
	float m_fCMRRValue;
	/** 仪器共模测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byCMRRCheck;

	/** 仪器测试结果 0：未测试；1：测试合格；2：测试不合格；3：无信号；4：未判读*/
	byte m_byCheck;

public: //方法
	// 重置
	void OnReset();
	// 得到连接的下一个仪器
	CInstrument* GetNextInstrument(unsigned int uiRoutDirection);
// 	// 得到采集站
// 	CInstrument* GetInstrumentCollect(unsigned int uiRoutDirection);
// 	// 重新设置交叉站尾包接收时刻
// 	void ResetInstrumentCrossTailFrameReceiveTime(unsigned int uiRoutDirection);
// 	// 重新设置尾包接收、发送时刻，屏蔽高2位
// 	void ResetInstrumentTailFrameTime();
// 	// 根据方向得到交叉站路由地址
// 	unsigned int GetRoutIPByRoutDirection(unsigned int uiRoutDirection);
// 	// 根据方向设置交叉站路由地址
// 	void SetRoutIPByRoutDirection(unsigned int uiRoutIP, unsigned int uiRoutDirection);

	// 检查测试结果
	void CheckResultAll();
};