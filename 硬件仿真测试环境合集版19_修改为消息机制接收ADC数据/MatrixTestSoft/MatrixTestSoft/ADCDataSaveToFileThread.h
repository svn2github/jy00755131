#pragma once
#include "Parameter.h"


// CADCDataSaveToFileThread

class CADCDataSaveToFileThread : public CWinThread
{
	DECLARE_DYNCREATE(CADCDataSaveToFileThread)

public:
	CADCDataSaveToFileThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CADCDataSaveToFileThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	// 初始化
	void OnInit(void);
	// 开始
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 关闭
	void OnClose(void);
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hADCDataSaveToFileThreadClose;
	// 线程开始工作事件
	HANDLE m_hADCDataSaveToFileThreadWork;
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 将ADC数据保存到文件
	void OnProcess(void);
	// ADC数据保存接收缓冲区
	double m_dADCSaveToFileRecBuf[GraphViewNum][ADCDataSaveToFileBufSize];
	// ADC数据保存处理缓冲区
	double m_dADCSaveToFileProcBuf[GraphViewNum][ADCDataSaveToFileBufSize];
	// ADC数据保存帧接收的个数
	unsigned int m_uiADCFrameSaveToFileRecNum[GraphViewNum];
	// ADC数据保存帧处理的个数
	unsigned int m_uiADCFrameSaveToFileProcNum[GraphViewNum];
	// ADC数据保存剩余帧个数
	unsigned int m_uiADCFrameRemain[GraphViewNum];
	// ADC完成存储的帧数
	unsigned int m_uiADCFrameSaved[GraphViewNum];
	// 选择仪器对象
	int* m_pSelectObject;
	// 选择仪器对象做噪声监测
	int* m_pSelectObjectNoise;
	// 保存文件的文件指针
	FILE * m_pFileSave;
	// 文件打开标志位
	BOOL m_bOpenADCSaveFile;
	// 被保存的ADC数据个数
	unsigned int m_uiADCDataToSaveNum;
	// ADC数据保存文件尺寸
	unsigned int m_uiADCFileLength;
	// 保存数据到文件的存储路径
	CString m_csSaveFilePath;
	// 保存ADC数据文件个数
	unsigned int m_uiADCSaveFileNum;
	// 选中的仪器对象名称
	char* m_cSelectObjectName[GraphViewNum];
	// 判断ADC采样停止标志位，用于保存剩余的ADC采样数据
	BOOL m_bStopSample;
	// 保存一次从临时文件缓冲区中得到的数据
	void OnSaveADCDataFramesToFile(void);
	// 创建并打开ADC保存数据文件
	void OnOpenADCSaveFile(void);
	// 关闭ADC保存数据文件
	void OnCloseADCSaveFile(void);

	// 计算出最小的待处理个数
	unsigned int OnCalMinProcNum(void);
	// 计算出最大的待处理个数
	unsigned int OnCalMaxNeedToSaveDataNum(void);
	// 将ADC数据保存到文件中
	void OnSaveADCToFile(unsigned int uiADCSaveDataNum);
	// 将ADC数据保存到文件中
	void OnSaveADCToFile(unsigned int uiADCSaveDataNum, unsigned int uiADCSaveDataPointNb);
	// 清理保存完的数据
	void OnClearProcData(unsigned int uiOutDataedProcNum);
};


