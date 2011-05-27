#pragma once


// CSendThread

class CSendThread : public CWinThread
{

	DECLARE_DYNCREATE(CSendThread)

public:
	CSendThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSendThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSocket m_hSocket2;
	//CPortMonitoringDlg* m_pTDoc;
	// 初始化
	void Init();
	bool m_close;
	void OnClose();
	unsigned char udp_buf[8][256];
	unsigned char udp_count;
	// ADC数据重发帧
	unsigned char m_ucADCRetransmission[128];
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
};


