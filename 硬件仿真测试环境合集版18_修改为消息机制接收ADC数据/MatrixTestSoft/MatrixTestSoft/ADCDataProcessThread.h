#pragma once
#include "Parameter.h"
#include <afxmt.h>
#include "OScopeCtrl.h"
#include "ADCDataSaveToFileThread.h"

// CADCDataProcessThread

class CADCDataProcessThread : public CWinThread
{
	DECLARE_DYNCREATE(CADCDataProcessThread)

public:
	CADCDataProcessThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CADCDataProcessThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 初始化
	void OnInit(void);
	// 打开
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 关闭
	void OnClose(void);
	virtual int Run();
	// 线程关闭标志
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hADCDataProcessThreadClose;
	// 线程工作事件
	HANDLE m_hADCDataProcessThreadWork;
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 记录前一个接收到的ADC数据的指针偏移量                  
	unsigned short m_usDataPointPrevious[GraphViewNum];
	// 记录每次处理实际接收ADC数据包个数
	unsigned int m_uiADCDataFrameRecCount[GraphViewNum];
	// 记录每次需要处理的ADC数据包个数
	unsigned int m_uiADCDataFrameProcCount[GraphViewNum];
	// 记录应收采集到的ADC数据帧的个数
	unsigned int m_uiADCDataFrameProcNum[GraphViewNum];
	// ADC数据接收缓冲，1个指针偏移量+74个数据
	double m_dADCDataRecBuf[GraphViewNum][ADCDataProcBufSize];
	// ADC数据处理缓冲区
	double m_dADCDataProcBuf[GraphViewNum][ADCDataProcBufSize];
	// ADC数据用于文件存储的临时缓冲区
	double m_dADCDataNeedToSaveBuf[GraphViewNum][ADCDataNeedToSaveBufSize];
	// 将临时数据拷贝到文件存储缓冲区的次数
	unsigned int m_uiCopySaveBufNum[GraphViewNum];
	// 需要重发的ADC数据信息结构
	typedef struct m_structADC
	{
		unsigned int uiDataCount;		// 重发帧所对应的帧位置（个数）
		unsigned short uiRetransmissionNum;	// 重发计数
	};
	// 需要重发的ADC索引表
	CMap<unsigned int, unsigned int, m_structADC, m_structADC&> m_oADCLostMap[GraphViewNum];
	// 定义临界区变量
	CCriticalSection m_Sec;
	// ADC数据重发帧
	unsigned char m_ucADCRetransmission[SndFrameSize];
	// 源IP地址
	CString m_csIPSource;
	// 发送端口
	unsigned int m_uiSendPort;
	// ADC数据接收Socket
	CSocket m_ADCRetransmissionSocket;
	// ADC数据存储线程指针
	CADCDataSaveToFileThread* m_pADCDataSaveToFileThread;
	// ADC数据处理
	void OnProcess(void);
	// 检查接收帧是否为重发帧
	BOOL OnCheckFrameIsRetransmission(unsigned int uiInstrumentNb, unsigned int uiADCDataFrameProcNb);
	// 保存ADC数据帧到写文件临时缓冲区
	void OnSaveADCFrameToFileBuf(unsigned int uiInstrumentNb, BOOL bADCRetransimission, unsigned int uiADCDataFrameProcNb);
	// 处理一帧ADC数据
	void OnProcessOneFrame(unsigned int uiInstrumentNb, unsigned int uiADCDataFrameProcNb);
	// 记录重发帧的位置
	unsigned int m_uiADCRetransmissionNb;
	// 图形模板指针
	COScopeCtrl* m_pOScopeCtrl;
	// ADC图形显示设备IP
	unsigned int m_uiADCGraphIP;
	// 选择仪器对象做噪声监测
	int* m_pSelectObjectNoise;
	// 从索引表中清除过期数据
	void OnClearOutdatedFromMap(unsigned int uiInstrumentNb);
	// 从索引表中发送ADC数据查询帧
	void OnSendADCFrameFromMap(void);
	// 生成ADC数据重发帧
	void MakeADCDataRetransmissionFrame(unsigned short usDataPointNb, unsigned int uiIPAim);
	// 将临时缓冲区文件拷贝到文件存储缓冲区
	void OnCopyToFileBuf(unsigned int uiInstrumentNb);
};


