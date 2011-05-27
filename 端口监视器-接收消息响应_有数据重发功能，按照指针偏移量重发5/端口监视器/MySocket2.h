#pragma once

// CMySocket2 命令目标
class CPortMonitoringDlg;
#define InstrumentForTest	75	// 定义虚拟仪器数
#define ClearListFrameCount	10	// 达到指定帧后清理列表中过期重发帧
#define ADCRetransmissionCmd	0x0003	// ADC数据重发命令
#define SendADCFrameCountMax		75	// 最多一次发送ADC数据查询帧数
#define ADRecPort				0x8300	// AD数据采集接收端口
class CMySocket2 : public CSocket
{
public:
	CMySocket2(CPortMonitoringDlg* pTDoc);
	virtual ~CMySocket2();
public:
	virtual void OnReceive(int nErrorCode);

protected:
	unsigned char udp_buf[8][256];
	unsigned char udp_count;
	CPortMonitoringDlg* m_pTDoc;
	// UDP接收缓冲区
	unsigned char m_ucUdpBuf[16384];
	// ADC数据重发帧
	unsigned char m_ucADCRetransmission[128];
public:
	// 需要重发的ADC数据信息结构
	typedef struct m_structADC
	{
		unsigned int	uiIPAim;				// 目标IP地址
		unsigned short	uiRetransmissionNum;	// 接收到的重发计数
		unsigned int	uiFrameCount;			// 程序转发ADC数据帧计数
	};
	// 接收重发的ADC索引表
	CMap<unsigned int, unsigned int, m_structADC, m_structADC&> m_oADCLostMap;
	// 需要重发的ADC索引表
	CMap<unsigned int, unsigned int, m_structADC, m_structADC&> m_oADCSendMap;
	// 设置ADC数据重发
	void OnADCDataRetransmission(unsigned int uiIPAim, unsigned short usDataPointNb);
	// 生成ADC数据重发帧
	void MakeADCDataRetransmissionFrame(unsigned int uiIPAim, unsigned short usDataPointNb);
	// 队列中的每个结构体的帧计数加一
	void OnListCountAdd(void);
	// ADC接收帧计数
	unsigned int m_uiADCFrameCount;
	// 发送ADC数据重发帧
	void SendADCDataRetransmissionFrame(void);
	// 发送重发帧个数
	unsigned int m_uiSendADCRetransmissionFramNum;
	// UDP接收帧指针偏移量
	unsigned int m_uiUdpCount;
	// 目标2发送的端口
	unsigned short m_uiPortNb[18];
};


