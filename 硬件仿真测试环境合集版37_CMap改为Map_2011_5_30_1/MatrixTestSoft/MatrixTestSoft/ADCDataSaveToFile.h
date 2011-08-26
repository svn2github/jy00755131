#pragma once
#include "Parameter.h"
#include "InstrumentList.h"
class CADCDataSaveToFile
{
public:
	CADCDataSaveToFile(void);
	~CADCDataSaveToFile(void);
protected:
	// 文件打开标志位
	BOOL m_bOpenADCSaveFile;
	// 保存文件的文件指针
//	FILE * m_pFileSave;
	CFile m_FileSave;
	// 保存文件
	CArchive* m_arFileSave;
public:
	// 保存ADC数据文件个数
	unsigned int m_uiADCSaveFileNum;
	// ADC数据保存文件尺寸
	unsigned int m_uiADCFileLength;
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
	// 被保存的ADC数据个数
	unsigned int m_uiADCDataToSaveNum;
	// 参与采样的设备个数
	unsigned int m_uiSampleInstrumentNum;
	// 设备类指针
	CInstrumentList* m_pInstrumentList;
public:
	// 将ADC数据保存到文件中
	void OnSaveADCToFile(int(* iADCDataBuf)[ADCDataTempDataSize], unsigned int* uiADCDataNum,
		unsigned int uiADCDataToSaveNum, bool bFinish);
	// 关闭ADC保存数据文件
	void OnCloseADCSaveFile(void);
	// 创建并打开ADC保存数据文件
	void OnOpenADCSaveFile(void);
protected:
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};
