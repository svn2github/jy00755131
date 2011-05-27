#pragma once
#include "Parameter.h"
class CADCDataSaveToFile
{
public:
	CADCDataSaveToFile(void);
	~CADCDataSaveToFile(void);
protected:
	// 文件打开标志位
	BOOL m_bOpenADCSaveFile;
	// 保存文件的文件指针
	FILE * m_pFileSave;
	// 被保存的ADC数据个数
	unsigned int m_uiADCDataToSaveNum;
public:
	// 保存ADC数据文件个数
	unsigned int m_uiADCSaveFileNum;
	// ADC数据保存文件尺寸
	unsigned int m_uiADCFileLength;
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
public:
	// 将ADC数据保存到文件中
	int OnSaveADCToFile(double(* dpADCDataBuf)[ADCDataTempDataSize], unsigned int* uiADCDataNum,
		int* pSelectObject, int* pSelectObjectNoise, bool bFinish);
protected:
	// 创建并打开ADC保存数据文件
	void OnOpenADCSaveFile(void);
	// 被选择仪器的ADC数据个数的最小值
	unsigned int OnADCRecDataMinNum(unsigned int* uiADCDataNum, int* pSelectObject, int* pSelectObjectNoise);
	// 关闭ADC保存数据文件
	void OnCloseADCSaveFile(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 被选择仪器的ADC数据个数的最大值
	unsigned int OnADCRecDataMaxNum(unsigned int* upADCDataNum, int* pSelectObject, int* pSelectObjectNoise);
};
