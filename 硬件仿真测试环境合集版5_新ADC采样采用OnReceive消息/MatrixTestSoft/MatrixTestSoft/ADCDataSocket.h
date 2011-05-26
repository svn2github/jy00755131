#pragma once
#include <vector>
#include "OScopeCtrl.h"
#include "Parameter.h"
#include <afxmt.h>
using namespace std;
// CADCDataSocket 命令目标

class CADCDataSocket : public CSocket
{
public:
	CADCDataSocket();
	virtual ~CADCDataSocket();
public:
	// ADC接收数据缓冲区
	byte ADCData[256];
	// 接收采样数据
	void ReceiveSampleData(byte* pSampleData);
	// 选择仪器对象
	int* m_pSelectObject;
	// 选择仪器对象做噪声监测
	int* m_pSelectObjectNoise;
	// 采集到的数据
	vector <double>* m_pSampleData[GraphViewNum];
	// 图形模板指针
	COScopeCtrl* m_pOScopeCtrl[GraphViewNum];
	// 定义临界区变量
	CCriticalSection m_Sec;
	virtual void OnReceive(int nErrorCode);
	// 记录指针偏移量
	unsigned int m_uiDataPointMove[GraphViewNum][10000];
	// 记录首个指针偏移量
	unsigned int m_uiDataPointLast[GraphViewNum];
	// 采样数据包个数
	unsigned int m_uiDataFrameNum[GraphViewNum];
	// 用于存储ADC数据
	vector <double> m_dADCSaveData[GraphViewNum];
	// 界面指针
//	CWnd* m_pwnd;
};
