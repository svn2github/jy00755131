#pragma once
//#include "Parameter.h"
//#include "FDUADSet.h"
//#include "..\common\OperaStruct.h"
//#include "..\common\SocketParam.h"
// 定义 FDU 最大数量
#define  FDU_MAXINUM	   256
// 接收数据包的大小
#define ADDataSize			3
//采样速率
#define  SamplingRate	   1000
// 定义每个数据采样帧的采样数据字节数
#define  SAMPFRM_DATALEN   216
//最大帧数
#define  MaxrevFramePoint	   32*SamplingRate/1000
//每帧采集点数
#define ReceiveDataPoint	72	
#define  MaxrevFrameSize	   MaxrevFramePoint*ReceiveDataPoint*ADDataSize		//64*72*3=13824
//采集接收缓冲区
#define  RecvBuf	MaxrevFrameSize

class CADCDataSaveToFile
{
public:
	CADCDataSaveToFile(void);
	~CADCDataSaveToFile(void);
protected:
	// 文件打开标志位
	BOOL m_bOpenADCSaveFile;
public:
	// 保存文件的文件指针
	//FILE * m_pFileSave;
	CFile m_pFileSave;
	// 保存ADC数据文件个数
	unsigned int m_uiADCSaveFileNum;
	// ADC数据保存文件尺寸
	unsigned int m_uiADCFileLength;
	// 保存数据到文件的存储路径
	//CString m_csSaveFilePath;
	// 被保存的ADC数据个数
	unsigned int m_uiADCDataToSaveNum;
	// 文件序号
	unsigned int m_uiSaveNum;
	// 定义临界区全局变量
	CCriticalSection m_Sec;
	CArchive *arP;
	int iline;
	unsigned char m_RevBuf[FDU_MAXINUM*RecvBuf];

public:
	// 将ADC数据保存到文件中
	//void OnSaveADCToFile(double(* dpADCDataBuf)[ADCDataTempDataSize], unsigned int* uiADCDataNum,
	//	unsigned int uiADCDataToSaveNum, bool bFinish);
	//void OnSaveADCToFile(unsigned char  *m_RevBuf, int (*upADCDataNum)[MaxrevFfameSize+ReceiveDataPoint],int *Revidx,int SpreadCount, bool bFinish,int type);
	void OnSaveADCToFile(unsigned char  *m_RevBuf, int *upADCDataNum,int *Revidx,int SpreadCount, bool bFinish,int type,int lineCount);
	// 关闭ADC保存数据文件
	void OnCloseADCSaveFile(void);
	// 创建并打开ADC保存数据文件
	void OnOpenADCSaveFile(int);
protected:
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
};
