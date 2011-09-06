#pragma once
#include "Parameter.h"
#include <vector>
using namespace std;
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
	// 接收到ADC设置信息帧标志位
	BOOL m_bRecADCSetInfoFrame;
	// 设置ADC数据图形显示设置帧缓冲区
	unsigned char m_oADCGraphSetFrameBuf[ADCSendFrameBufSize];
public:
	// 接收到的帧数
	unsigned int* m_uipRecFrameNum;
	// 设备标记点序号数组指针
	unsigned int* m_uiInstrumentNb;
	// 绘X轴坐标参考点的仪器序号
	unsigned int m_uiDrawPointXNb;
	// 记录X轴坐标点信息指针
	vector <double>	m_DrawPoint_X;
	// 参与ADC数据采集的采集站设备数
	unsigned int m_uiInstrumentADCNum;
	// 抽样率
	unsigned int m_uiSamplingRate;
	// 单个采集站接收帧数
	unsigned int m_uiInstrumentRecFrameNum;
private:
	// 单个帧处理
	void ProcFrameOne(void);
	// ADC数据绘图准备
	void OnPrepareToShow(unsigned short usInstrumentNum);
public:
	// 得到当前ADC数据接收帧最小帧数，并返回仪器序号
	unsigned int GetRecFrameNumMin(void);
	// 得到当前ADC数据接收帧最大帧数，并返回仪器序号
	unsigned int GetRecFrameNumMax(void);
	// 发送采样参数设置命令
	void OnMakeAndSendSetFrame(unsigned short usSetOperation);
};


