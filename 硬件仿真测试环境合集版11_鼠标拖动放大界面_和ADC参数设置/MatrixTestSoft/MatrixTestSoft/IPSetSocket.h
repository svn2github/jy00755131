#pragma once
#include "InstrumentList.h"
#include "ADCSet.h"
// CIPSetSocket 命令目标

class CIPSetSocket : public CSocket
{
public:
	CIPSetSocket();
	virtual ~CIPSetSocket();
	virtual void OnReceive(int nErrorCode);
public:
	// 仪器串号
	DWORD m_uiSN;
	// 仪器的IP地址
	DWORD m_uiIPAddress;
	// 仪器列表指针
	CInstrumentList* m_pInstrumentList;
	// IP地址设置应答缓冲
	byte m_pIPSetReturnFrameData[256];
	// 单个IP地址设置应答帧处理
	void ProcIPSetReturnFrameOne(void);
	// 解析IP地址设置应答帧
	BOOL ParseIPSetReturnFrame(void);
	// 采集站设备连接上的图标指针
	HICON m_iconFDUConnected;
	// 交叉站设备连接上的图标指针
	HICON m_iconLAUXConnected;
	// 界面指针
	CWnd* m_pwnd;
	// ADC设置类指针
	CADCSet* m_pADCSet;
	// 目标端口
	unsigned int m_uiSendPort;
	// 选择仪器对象
	int* m_pSelectObject;
	// 显示设备连接图标
	void OnShowConnectedIcon(unsigned int uiIPAddress);
	// 采集站设备Button控件ID
	int m_iButtonIDFDU[GraphViewNum];
	// 采集站设备仪器选择Check控件ID
	int m_iCheckIDInstrumentFDU[GraphViewNum];
};


