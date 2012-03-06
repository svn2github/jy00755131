#include "Instrument.h"

#ifdef MATRIXSERVERDLL_EXPORTS
#define MATRIXSERVERDLL_API __declspec(dllexport)
#define MatrixServerDll_API extern "C" __declspec(dllexport)
#else
#define MATRIXSERVERDLL_API __declspec(dllimport)
#define MatrixServerDll_API extern "C" __declspec(dllimport)
#endif

#ifndef   _Rout_H
#define   _Rout_H
// 路由属性结构体
typedef struct Rout_Struct
{
	/** 在路由数组中的位置*/
	unsigned int m_uiIndex;
	/** 路由是否使用中*/
	bool m_bInUsed;
	/** 路由IP地址*/
	unsigned int m_uiRoutIP;
	/** 路由方向 1-上；2-下；3-左；4右*/
	int m_iRoutDirection;
	/** 路由头仪器*/
	Instrument_Struct* m_pHead;
	/** 路由尾仪器*/
	Instrument_Struct* m_pTail;
	/** 路由时刻*/
	unsigned int m_uiRoutTime;
	/** 路由是否为交叉线路由*/
	bool m_bRoutLaux;
	/** 路由是否接收到ADC设置参数应答*/
	bool m_bADCSetReturn;
	// 路由是否发送ADC参数设置帧
	bool m_bADCSetRout;
	// 该路由方向上仪器的个数
	unsigned int m_uiInstrumentNum;
}m_oRoutStruct;
#endif