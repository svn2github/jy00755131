#pragma once
#include "Instrument.h"
#include "Parameter.h"
class CInstrumentList
{
public:
	CInstrumentList(void);
	~CInstrumentList(void);
public:
	CInstrument* m_pInstrumentArray;

protected:
	// 空闲仪器数量
	unsigned int m_uiCountFree;
	// 空闲仪器队列
	CList<CInstrument*, CInstrument*> m_olsInstrumentFree;
public:
	// 仪器SN索引表
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMap;
	// 采集站设备未连接显示的图标指针
	HICON m_iconFDUDisconnected;
	// 交叉站设备未连接显示的图标指针
	HICON m_iconLAUXDisconnected;
	// 界面指针
	CWnd* m_pwnd;
	// 采集站设备Button控件ID
	int m_iButtonIDFDU[InstrumentNum];
	// 采集站设备仪器选择Check控件ID
	int m_iCheckIDInstrumentFDU[InstrumentNum];
protected:
	// 根据IP地址显示设备断开连接的图标
	void OnShowDisconnectedIcon(unsigned int uiIPAddress);
	// 防止程序在循环中运行无法响应消息
	void ProcessMessages(void);
public:
	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 开始
	void OnOpen(void);
	// 停止
	void OnStop(void);
	// 得到一个空闲仪器
	CInstrument* GetFreeInstrument(void);
	// 将一个仪器加入索引表
	void AddInstrumentToMap(unsigned int uiIndex, CInstrument* pInstrument);
	// 判断仪器索引号是否已加入索引表
	BOOL IfIndexExistInMap(unsigned int uiIndex);
	// 根据输入索引号，由索引表得到仪器指针
	BOOL GetInstrumentFromMap(unsigned int uiIndex, CInstrument* &pInstrument);
	// 由索引号从索引表中删除一个仪器
	void DeleteInstrumentFromMap(unsigned int uiIndex);
	// 设备根据首包时刻排序
	void SetInstrumentLocation(CInstrument* pInstrumentAdd);
	// 删除采集站尾包之后的仪器
	void TailFrameDeleteInstrument(CInstrument* pInstrumentDelete);
	// 清理过期的尾包时刻查询结果
	void ClearExperiedTailTimeResult(void);
	// 删除所有仪器
	void DeleteAllInstrument(void);
};
