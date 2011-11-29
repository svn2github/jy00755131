#pragma once
#include "OScopeCtrl.h"
#include "Parameter.h"
#include "ADCDataSaveToFile.h"
#include "ADCFrameInfo.h"
#include "LogFile.h"
#include "InstrumentList.h"
#include <hash_map>

using stdext::hash_map;
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
	// ADC数据接收sockaddr_in
	sockaddr_in m_RecvAddr;
	// ADC数据重发sockaddr_in
	sockaddr_in m_SendToAddr;
	// ADC数据图形化显示发送sockaddr_in
	sockaddr_in m_SendADCGraphToAddr;
	// ADC数据接收和发送Socket套接字
	SOCKET m_ADCDataSocket;
	// ADC数据图形化显示Socket套接字
	SOCKET m_ADCGraphShowSocket;
	// 图形模板指针
	COScopeCtrl* m_pOScopeCtrl[InstrumentMaxCount];
	// 记录应接收ADC数据包个数
	unsigned int m_uiADCDataFrameCount[InstrumentMaxCount];
	// 判断ADC采样开始标志位，用于丢弃TB之前的无效数据
	BOOL m_bStartSample;
	// 源IP地址
	unsigned int m_uiIPSource;
	// ADC图形显示设备IP
	unsigned int m_uiADCGraphIP[InstrumentMaxCount];
	// ADC数据界面显示缓冲区
	unsigned int m_uiADCDataShow[InstrumentMaxCount];
	// 类CADCDataSaveToFile成员变量指针
	CADCDataSaveToFile* m_pADCDataSaveToFile;
	// 类CADCFrameInfo成员变量指针
	CADCFrameInfo* m_pADCFrameInfo;
	// 日志类指针
	CLogFile* m_pLogFile;
	// ADC数据采样采样率设置
	unsigned int m_uispsSelect;
	// 设备类指针
	CInstrumentList* m_pInstrumentList;

private:
	// 线程关闭标志
	bool m_bclose;
	// 记录前一个接收到的ADC数据的指针偏移量                  
	unsigned short m_usDataPointPrevious[InstrumentMaxCount];
	// 记录实际接收ADC数据包个数
	unsigned int m_uiADCDataFrameRecCount[InstrumentMaxCount];
	// ADC数据重发帧
	unsigned char m_ucADCRetransmission[SndFrameSize];
	// 需要重发的ADC数据信息结构
	typedef struct m_structADC
	{
		unsigned int uiDataCount;		// 重发帧所对应的帧位置（个数）
		unsigned short uiRetransmissionNum;	// 重发计数
	};
	// 需要重发的ADC索引表
	hash_map<unsigned int, m_structADC> m_oADCLostMap[InstrumentMaxCount];
	// 有ADC数据重发故先保留不连续的数据
	int m_iSampleTemp[InstrumentMaxCount][ADCDataTempDataSize];
	// 有ADC数据重发故先保留不连续的数据个数
	unsigned int m_uiSampleTempNum[InstrumentMaxCount];
	// ADC数据保存成文件的数据缓冲区
	int m_iADCSave[InstrumentMaxCount][ADCDataTempDataSize];
	// ADC数据保存成文件的数据缓冲区中数据个数
	unsigned int m_uiADCSaveNum[InstrumentMaxCount];
	// ADC数据处理过程缓冲区
	int m_iTemp[ADCDataTempDataSize];
	// 测试丢帧情况
	unsigned int m_uiTestADCLost;
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
	// ADC数据图形化显示设置接收缓冲区
	byte m_pADCGraphSetFrameData[RcvFrameSize];
	// ADC数据图形化显示抽样率
	unsigned int m_uiSamplingRate;
	// 开始ADC数据图形化显示标志位
	BOOL m_bStartGraphShow;
	// 监测参与采样的设备应收ADC数据包个数是否一致
	BOOL m_bCheckADCDataFrameCountEqule;
	// ADC数据图形化显示设置缓冲区
	unsigned char m_ucADCGraphShowSetFrame[RcvFrameSize];
	// ADC数据图形化显示数据缓冲区
	unsigned char m_ucADCGraphShowDataFrame[InstrumentMaxCount][RcvFrameSize];
	// ADC数据图形化显示抽样点数
	unsigned int m_uiADCGraphShowSamplingNum[InstrumentMaxCount];
	// ADC数据图形化显示数据源指针坐标
	unsigned int m_uiADCGraphShowSourcePoint[InstrumentMaxCount];
private:
	// 接收采样数据
	void ReceiveSampleData(byte* pSampleData);
	// 被选择仪器的数值比较
	int OnADCRecDataCheckOpt(unsigned short usOperation, unsigned int* pData);
	// 生成ADC数据重发帧
	void MakeADCDataRetransmissionFrame(unsigned short usDataPointNb, unsigned int uiIPAim);
	// ADC数据需要重新发送帧对应的最小个数
	unsigned int OnADCRetransimissionMinNb(unsigned int uiInstrumentNb);
	// ADC数据重发
	void OnADCDataRetransmission(void);
	// ADC数据重发
	void OnADCDataRetransmission(unsigned int uiInstrumentNb);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
	// 检查接收帧是否为重发帧
	DWORD OnCheckFrameIsRetransmission(unsigned int uiInstrumentNb, unsigned int uiIP, unsigned short usDataPointNow);
	// 该帧为最后一个数据重发帧
	void OnRecOkIsRetransimissionFrame(unsigned int uiInstrumentNb, int* piReceiveData);
	// 该帧为普通帧且之前没有重发帧
	void OnRecOkIsNormalFrame(unsigned int uiInstrumentNb, int* piReceiveData);
	// 该帧为重发帧但不是最后一个重发帧
	void OnRecNotOkIsRetransimissionFrame(unsigned int uiInstrumentNb, int* piReceiveData);
	// 该帧为普通帧且之前有重发帧
	void OnRecNotOkIsNormalFrame(unsigned int uiInstrumentNb, int* piReceiveData);
	// 数据处理
	void OnProcess(int iCount);
	// 生成ADC数据图形化显示帧
	void OnMakeADCGraphShowFrame(unsigned short usCommand);
	// ADC图形化显示设置帧处理函数
	void ProcADCGraphSetFrame(void);
public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 重置变量的值
	void OnReset(void);
	// 关闭UDP套接字
	void OnCloseUDP(void);
	// ADC数据图形化显示设置接收函数
	void OnReceive(void);
	// 将ADC数据保存到文件中
	void OnSaveADCToFile(bool bfinish);
};


