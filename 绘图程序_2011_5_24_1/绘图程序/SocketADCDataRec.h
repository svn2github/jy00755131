#pragma once
#include "Parameter.h"
#include "ParameterSet.h"
// CSocketADCDataRec 命令目标

class CSocketADCDataRec : public CSocket
{
public:
	CSocketADCDataRec();
	virtual ~CSocketADCDataRec();
	virtual void OnReceive(int nErrorCode);
private:
	// 设置ADC数据帧接收缓冲区
	unsigned char m_oADCRecFrameBuf[ADCRecFrameBufSize];
	// 设置ADC数据图形显示设置帧缓冲区
	unsigned char m_oADCGraphSetFrameBuf[ADCSendFrameBufSize];
	// 接收到ADC设置信息帧标志位
	BOOL m_bRecADCSetInfoFrame;
public:
	// 设备标记点序号数组指针
	unsigned int m_uiInstrumentNb[InstrumentMaxCount];
	// 接收到的帧序号记录
	unsigned int m_uiRecFrameNb[InstrumentMaxCount][ADCFrameNum];
	// 接收到得帧数记录
	unsigned int m_uiRecFrameNum[InstrumentMaxCount];
	// 参与绘图的数据的起始帧数
	unsigned int m_uiDrawFrameBeginNum[InstrumentMaxCount];
	// 接收到的ADC数据的值
	double m_dbFduData[InstrumentMaxCount][ADCRecBufSize];
	// 图形显示的ADC数据的值
	double m_dbFduShow[InstrumentMaxCount][ShowLinePointsNumNow];
	// x轴显示的坐标值
	double m_DrawPoint_X[ShowLinePointsNumNow];
	// 参与ADC数据采集的采集站设备数
	unsigned int m_uiInstrumentADCNum;
	// 抽样率
	unsigned int m_uiSamplingRate;
	// 参数设置界面
	CParameterSet* m_pParameterSet;
private:
	// 单个帧处理
	void ProcFrameOne(void);
	// ADC数据绘图准备
	void OnPrepareToShow(unsigned short usInstrumentNum);
public:
	// 得到当前ADC数据接收帧起始序号中的最大值，并返回
	unsigned int GetRecFrameBeginMaxNb(void);
	// 得到当前ADC数据接收帧最后一个序号中的最小值，并返回
	unsigned int GetRecFrameEndMinNb(void);
	// 得到要清除的仪器个数并返回
	unsigned int GetRecFrameBeginToEndNum(unsigned int uiInstrumentNb, unsigned int uiRecFrameEndMinNb);
	// 发送采样参数设置命令
	void OnMakeAndSendSetFrame(unsigned short usSetOperation);
	// 准备开始图形显示标志位
	bool m_bPrepareToShow;
	// 得到每个设备数据接收缓冲区开始绘图的帧数
	void GetDrawFrameBeginNum(unsigned int uiRecFrameBeginMaxNb);
};


