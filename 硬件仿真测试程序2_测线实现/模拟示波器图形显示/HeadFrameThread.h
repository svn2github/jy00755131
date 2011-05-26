#pragma once
#include "InstrumentList.h"


// CHeadFrameThread

class CHeadFrameThread : public CWinThread
{
	DECLARE_DYNCREATE(CHeadFrameThread)

public:
	CHeadFrameThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CHeadFrameThread();
	virtual int Run();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 初始化
	void OnInit(void);
	// 关闭并结束线程
	void OnClose(void);
	// 关闭线程标志位
	bool m_bclose;
	// 首包接收Socket
	CSocket m_HeadFrameSocket;
	// IP地址设置Socket
	CSocket m_IPSetSocket;
	// 处理单个首包
	void ProcHeadFrameOne();
	// 解析首包
	BOOL ParseHeadFrame(void);
	// 仪器串号
	DWORD m_uiSN;
	// 首包时刻
	DWORD m_uiHeadFrameTime;
	// 路由地址
	DWORD m_uiRoutAddress;
	// 仪器的IP地址
	DWORD m_uiIPAddress;
	// 仪器列表指针
	CInstrumentList* m_oInstrumentList;
	// 生成IP地址设置帧
	void MakeIPSetFrame(CInstrument* pInstrument);
	// 首包接收数据缓冲
	byte m_pHeadFrameData[256];
	// 设置IP地址缓冲
	byte m_pIPSetFrameData[256];
	// IP地址设置应答缓冲
	byte m_pIPSetReturnFrameData[256];
	// 发送IP地址设置帧
	void SendIPSetFrame(void);
	// 单个IP地址设置应答帧处理
	void ProcIPSetReturnFrameOne(void);
	// 解析IP地址设置应答帧
	BOOL ParseIPSetReturnFrame(void);
};


