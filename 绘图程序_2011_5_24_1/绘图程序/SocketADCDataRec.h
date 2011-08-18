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
public:
	// 界面指针
	CWnd* m_pwnd;
	// 接收到的帧数
	unsigned int* m_uipRecFrameNum;
	// 绘X轴坐标参考点的仪器序号
	unsigned int m_uiDrawPointXNb;
	// 记录X轴坐标点信息指针
	vector <double>	m_DrawPoint_X;
	// 参与ADC数据采集的采集站设备数
	unsigned int m_uiInstrumentADCNum;
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
};


