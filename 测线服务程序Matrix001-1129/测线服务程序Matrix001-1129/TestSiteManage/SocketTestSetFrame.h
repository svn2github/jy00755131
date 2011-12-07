#pragma once

#include "FrameTestSet.h"
#include "RunTimeDataList.h"
#include "Matrixline.h"
#include "TestElementList.h"
#include "Parameter.h"
/**
*@brief 仪器测试设置帧发送网络接口类
*/
class CSocketTestSetFrame : public CSocket
{
public:
	CSocketTestSetFrame();
	virtual ~CSocketTestSetFrame();

public: // 属性
	/** LCI设备IP地址*/
	CString m_strIPLCI;
	/**  LCI设备IP地址端口号*/
	UINT m_uiPortLCI;

	/** 和现场仪器通讯的本机IP地址*/
	CString m_strIPForInstrument;
	/** 和现场仪器通讯的本机IP地址*/
	UINT m_uiIPForInstrument;
	/** 测试设置端口号*/
	UINT m_uiPortForTestSet;

	/** 帧字节数*/
	int m_iFrameSize;
	/** 帧数据对象*/
	CFrameTestSet m_oFrameTestSet;
	/** 运行日志*/
	CRunTimeDataList *m_oRunTimeDataList;
	/** 采样率 单位微秒*/
	unsigned int m_uiSamplingRate;
	/** 绘图抽样率 */
	unsigned int m_uiSamplingView;
	/** 绘图状态 1开始 0停止 */
	int Viewtype;
	unsigned char * ofccheck;
	char ReceiveBuf[FrameLength*16];
	CString strrecv;
	int adsetcount;
	int adgetcount;
	int gettestcount;
	int getTBcount;
	int checkrecv[ADcheckrecv];

	/** 测试单元队列*/
	CTestElementList* m_pTestElementList;
	// ADC命令设置序号
	unsigned int m_uiADCSetOperationNb;
	// 采集站本地时间
	unsigned int m_uiTnow;
	/** 参加测试IP地址数量*/
	unsigned int m_uiIPCount;
	// UDP接收缓冲区
	unsigned char m_ucUdpBuf[ADCSetReturnBufSize];
	// UDP接收帧指针偏移量
	unsigned int m_uiUdpCount;

public: // 方法
	// 初始化
	BOOL OnInit(CString strIPForInstrument, UINT uiIPForInstrument, UINT uiPortForTestSet, CString strIPLCI, UINT uiPortLCI);
	// 设置发送缓冲区可以发送的帧数量
	BOOL SetBufferSize(int iFrameCount);
	// 生成测试设置帧
	void MakeFrameDataForTestSet();
	// 发送测试设置帧
	void SendTestSetFrame();
	// 设置AD接收端口
	int MakeFrameADListen();
	// 生成测试开始帧
	void MakeFrameDataForTestStart();
	// 生成测试停止帧
	void MakeFrameDataForTestEnd(unsigned int uiInstrumentIP);
	// 发送测试停止帧
	void SendTestEndFrame();
	//设置AD参数
	void MakeFrameADSet();
	//设置帧头
	int MakeFrameHead();
	//设置帧头,非广播
	int MakeFrameHead1();
	void ParseFrameHead(int iCount,char *ReceiveBuf);
	//设置AD采集开始
	void MakeFrameADTestStart();
	//设置AD零漂采集开始
	void MakeFrameADClickTestStart1();
	void MakeFrameADIPTestStart();
	void MakeFrameADClickTestStart2();
	//设置绘图初始帧
	void ViewSet(int itype);
	// 生成AD测试停止
	void MakeFrameADTestSTOP(unsigned int uiInstrumentIP);
	// 补采集数据
	void ReloadTestDataFrame(unsigned int UiIP,int i_idx);
	//设置AD零漂
	void MakeFrameADClick0();
	bool ParseCommandWord(byte  * pBuf);
	bool AppCommandWord(unsigned char * pBuf);
	void resetcheckrecv();
	// ADC参数设置
	void OnADCSet(void);
	// ADC数据采集停止
	void OnADCSampleStop(void);
	// ADC零漂校正
	void OnADCZeroDrift(void);
	// ADC设置TB时刻开始采集
	void OnADCPrepareToSample(void);
	// 发送ADC命令设置帧
	void OnSendADCSetCmd(void);
	// 处理ADC设置应答帧
	void OnProcADCSetReturn(unsigned int uiIP);
	// 检查是否收到所有采集站的ADC命令应答
	BOOL OnCheckADCSetReturn(void);
	// 处理零漂校正查询应答
	void OnProcADCZeroDriftReturn(unsigned int uiIPAim);
	// 接收ADC参数设置应答帧
	void OnReceiveADCSetReturn(int itype);
	// 数据处理
	void OnProcess(int iCount);
	// 处理一帧ADC参数设置应答帧
	void	OnProcessOneADCSetReturn(void);
	// 重置ADC参数设置操作序号
	void OnResetADCOperationNb(void);
	// 清空ADC参数设置接收缓冲区
	void OnClearADCSetReturnBuf(void);
	// 检查ADC参数设置应答帧是否接收完全
	void OnCheckADCSetReturnComplete(void);
	// ADC参数设置操作过程
	void OnADCSetOperation(void);
	// ADC设置TB时刻
	void OnADCSetTBTime(unsigned int tnow);
};


