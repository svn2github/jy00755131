#pragma once
#include "OScopeCtrl.h"
#include "Parameter.h"
#include "ADCDataSaveToFile.h"
#include <afxmt.h>
using namespace std;

// CADCDataRecThread

class CADCDataRecThread : public CWinThread
{
	DECLARE_DYNCREATE(CADCDataRecThread)

public:
	CADCDataRecThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CADCDataRecThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
public:
	// 线程结束事件
	HANDLE m_hADCDataThreadClose;
	// ADC数据接收Socket
	CSocket* m_pADCDataSocket;
	// 线程关闭标志
	bool m_bclose;
	// 选择仪器对象
	int m_iSelectObject[InstrumentNum];
	// 选择仪器对象做噪声监测
	int m_iSelectObjectNoise[InstrumentNum];
	// 图形模板指针
	COScopeCtrl* m_pOScopeCtrl[InstrumentNum];
	// 定义临界区变量
	CCriticalSection m_Sec;
	// 记录前一个接收到的ADC数据的指针偏移量                  
	unsigned short m_usDataPointPrevious[InstrumentNum];
	// 记录应接收ADC数据包个数
	unsigned int m_uiADCDataFrameCount[InstrumentNum];
	// 记录实际接收ADC数据包个数
	unsigned int m_uiADCDataFrameRecCount[InstrumentNum];
	// 判断ADC采样开始标志位，用于丢弃TB之前的无效数据
	BOOL m_bStartSample;
	// ADC数据重发帧
	unsigned char m_ucADCRetransmission[SndFrameSize];
	// 源IP地址
	CString m_csIPSource;
	// 发送端口
	unsigned int m_uiSendPort;
	// 需要重发的ADC数据信息结构
	typedef struct m_structADC
	{
		unsigned int uiDataCount;		// 重发帧所对应的帧位置（个数）
		unsigned short uiRetransmissionNum;	// 重发计数
	};
	// 需要重发的ADC索引表
	CMap<unsigned int, unsigned int, m_structADC, m_structADC&> m_oADCLostMap[InstrumentNum];
	// 有ADC数据重发故先保留不连续的数据
	double m_dSampleTemp[InstrumentNum][ADCDataTempDataSize];
	// 有ADC数据重发故先保留不连续的数据个数
	unsigned int m_uiSampleTempNum[InstrumentNum];
	// ADC数据保存成文件的数据缓冲区
	double m_dADCSave[InstrumentNum][ADCDataTempDataSize];
	// ADC数据保存成文件的数据缓冲区中数据个数
	unsigned int m_uiADCSaveNum[InstrumentNum];
	// ADC数据处理过程缓冲区
	double m_dTemp[ADCDataTempDataSize];
	// ADC数据处理过程缓冲区中数据个数
	unsigned int m_uiTempNum;
	// 测试丢帧情况
	unsigned int m_uiTestADCLost;
	// ADC图形显示设备IP
	unsigned int m_uiADCGraphIP[InstrumentNum];
	// ADC数据界面显示缓冲区
	double m_dADCDataShow[InstrumentNum];
	// ADC接收到的数据重发帧的序号
	unsigned int m_uiADCRetransmissionNb;
	// 数据帧接收缓冲
	unsigned char m_ucudp_buf[RcvBufNum][RcvFrameSize];
	// 缓冲区序号
	unsigned short m_usudp_count;
	// UDP接收帧指针偏移量
	unsigned int m_uiUdpCount;
	// UDP接收缓冲区
	unsigned char m_ucUdpBuf[ADCDataBufSize];
	// 类CADCDataSaveToFile成员变量指针
	CADCDataSaveToFile* m_pADCDataSaveToFile;
protected:
	// 被选择仪器的ADC数据个数的最小值
	unsigned int OnADCRecDataMinNum(void);
	// 被选择仪器的ADC数据个数的最大值
	unsigned int OnADCRecDataMaxNum(void);
public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 接收采样数据
	void ReceiveSampleData(byte* pSampleData);
	// 生成ADC数据重发帧
	void MakeADCDataRetransmissionFrame(unsigned short usDataPointNb, unsigned int uiIPAim);
	// ADC数据需要重新发送帧对应的最小个数
	unsigned int OnADCRetransimissionMinNb(unsigned int uiInstrumentNb);
	// 将ADC数据保存到文件中
	void OnSaveADCToFile(bool bfinish);
	// ADC数据重发
	void OnADCDataRetransmission(void);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 检查接收帧是否为重发帧
	BOOL OnCheckFrameIsRetransmission(unsigned int uiInstrumentNb, unsigned int uiIPAim, unsigned short usDataPointNow);
	// 该帧为最后一个数据重发帧
	void OnRecOkIsRetransimissionFrame(unsigned int uiInstrumentNb, unsigned int uiIPAim, double* pReceiveData);
	// 该帧为普通帧且之前没有重发帧
	void OnRecOkIsNormalFrame(unsigned int uiInstrumentNb, unsigned int uiIPAim, double* pReceiveData);
	// 该帧为重发帧但不是最后一个重发帧
	void OnRecNotOkIsRetransimissionFrame(unsigned int uiInstrumentNb, unsigned int uiIPAim, double* pReceiveData);
	// 该帧为普通帧且之前有重发帧
	void OnRecNotOkIsNormalFrame(unsigned int uiInstrumentNb, double* pReceiveData);
	// 重置变量的值
	void OnReset(void);
	// 数据处理
	void OnProcess(int iCount);
};


