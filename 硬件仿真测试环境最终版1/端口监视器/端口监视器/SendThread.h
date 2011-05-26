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
	// 线程结束事件
	HANDLE m_hSendThreadClose;
	//CPortMonitoringDlg* m_pTDoc;
	// 初始化
	void Init();
	bool m_close;
	void OnClose();
	void OnReset();
	// 数据接收缓冲
	unsigned char udp_buf[8][256];
	unsigned char udp_count;
	// 首包计数
	unsigned int m_uiHeadFrameNum;
	// IP地址设置计数
	unsigned int m_uiIPSetReturnNum;
	// 尾包计数
	unsigned int m_uiTailFrameNum;
	// 尾包时刻查询应答计数
	unsigned int m_uiTailTimeReturnNum;
	// 时延设置应答计数
	unsigned int m_uiDelayTimeReturnNum;
	// ADC设置应答计数
	unsigned int m_uiADCSetReturnNum;
	// ADC数据采集帧个数
	unsigned int m_uiADCDataRecNum;
	// 显示首包帧，包含SN和首包时刻
	CString m_csHeadFrameShow;
	// 显示IP地址设置应答帧，包含SN和设置的IP地址
	CString m_csIPSetReturnShow;
	// 显示尾包帧，包含SN
	CString m_csTailFrameShow;
	// 显示尾包时刻查询应答帧，包含IP地址和查询结果
	CString m_csTailTimeReturnShow;
	// 显示时延设置应答帧，包含IP地址和设置内容
	CString m_csDelayTimeReturnShow;
	// 显示ADC采样数据帧，包含IP地址、帧计数和采样数据帧个数
	CString m_csADCDataRecShow;
	// 显示状态查询应答帧，包含本地时间、TB时间高位、TB时间低位、TC时间高位、TC时间低位、TE时间高位、TE时间低位、3D命令
	CString m_csGetStatusReturnShow;
	// 接收帧数
	unsigned int m_uiRecFrameNum;
	// ADC数据采集上一帧的数据指针偏移量
	unsigned short m_usADCLastDataPoint;
};


