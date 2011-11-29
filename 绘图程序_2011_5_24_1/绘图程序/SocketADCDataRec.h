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
// 	// 绘X轴坐标参考点的仪器序号
// 	unsigned int m_uiDrawPointXNb;
public:
	// 设备标记点序号数组指针
	unsigned int* m_uiInstrumentNb;
	// 接收到的帧序号记录
	vector<unsigned int>* m_uipRecFrameNb;
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
};


