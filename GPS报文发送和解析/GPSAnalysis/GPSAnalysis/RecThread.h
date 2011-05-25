#pragma once

#define IPSource "127.0.0.1"
#define RecGPSPort 0x8210

// CRecThread

class CRecThread : public CWinThread
{
	DECLARE_DYNCREATE(CRecThread)

public:
	CRecThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CRecThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();


protected:
	DECLARE_MESSAGE_MAP()
public:
	// GPS帧发送Socket
	CSocket m_Socket;
	// 线程关闭标志位
	bool m_bclose;
	// 线程结束事件
	HANDLE m_hRecThreadClose;
	// 接收到的GPS帧
	byte m_pRecFrameData[256]; 
	// 帧头变量
	CString m_csHeadFrame;
	// UTC时间变量
	double m_dUTCTime;
	// 状态变量
	CString m_csStatus;
	// 纬度值变量
	double m_dLatitudeValue;
	// 纬度变量
	CString m_csLatitude;
	// 经度值变量
	double m_dLongitudeValue;
	// 经度变量
	CString m_csLongitude;
	// 速度变量
	double m_dSpeed;
	// 方位角变量
	double m_dAzimuth;
	// UTC日期变量
	CString m_csUTCData;
	// 磁偏角变量
	double m_dMagneticdeclination;
	// 磁偏角方向
	CString m_csMagneticdeclination_Direction;
	// 模式变量
	CString m_csMode;
	// 校验和
	char m_cCheckSum;
public:
	
	// 线程初始化
	void Init(void);
	// 线程关闭
	void OnClose(void);
	// 处理单个GPS帧
	void ProcGPSFrameOne(void);
	// 重置
	void OnReset(void);
};


