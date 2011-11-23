#pragma once
#include "Instrument.h"
#include "Parameter.h"
#include <hash_map>
#include <list>
#include "LogFile.h"
#include "InstrumentGraph.h"

using stdext::hash_map;
using std::list;
class CInstrumentList
{
public:
	CInstrumentList(void);
	~CInstrumentList(void);
public:
	CInstrument* m_pInstrumentArray;
	// 日志类指针
	CLogFile* m_pLogFile;
protected:
	// 空闲仪器数量
	unsigned int m_uiCountFree;
	// 空闲仪器队列
	list<CInstrument*> m_olsInstrumentFree;
public:
	// 仪器SN索引表
	hash_map<unsigned int, CInstrument*> m_oInstrumentSNMap;
	// 仪器IP地址索引表
	hash_map<unsigned int, CInstrument*> m_oInstrumentIPMap;
	// 关键字为路由，对应的内容为广播端口
	hash_map<unsigned int, unsigned int> m_oRoutAddrMap;
	// 界面指针
	CWnd* m_pWnd;
	// 类CInstrumentGraph的指针
	CInstrumentGraph* m_pInstrumentGraph;
protected:
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
	// 将一个仪器加入SN索引表
	void AddInstrumentToSNMap(unsigned int uiIndex, CInstrument* pInstrument);
	// 判断仪器SN索引号是否已加入SN索引表
	BOOL IfIndexExistInSNMap(unsigned int uiIndex);
	// 根据输入SN索引号，由SN索引表得到仪器指针
	BOOL GetInstrumentFromSNMap(unsigned int uiIndex, CInstrument* &pInstrument);
	// 将一个仪器加入IP地址索引表
	void AddInstrumentToIPMap(unsigned int uiIndex, CInstrument* pInstrument);
	// 判断仪器索引号是否已加入IP地址索引表
	BOOL IfIndexExistInIPMap(unsigned int uiIndex);
	// 根据输入IP地址索引号，由IP地址索引表得到仪器指针
	BOOL GetInstrumentFromIPMap(unsigned int uiIndex, CInstrument* &pInstrument);
	// 将一个路由加入路由地址索引表
	void AddRoutToRoutMap(unsigned int uiRoutAddr, unsigned int uiBroadCastPort);
	// 判断路由是否已加入路由地址索引表
	BOOL IfRoutExistInRoutMap(unsigned int uiRoutAddr);
	// 根据输入路由地址索引号，由路由地址索引表得到广播端口
	BOOL GetBroadCastPortFromRoutMap(unsigned int uiRoutAddr, unsigned int &uiBroadCastPort);
	// 设备根据首包时刻排序
	void SetInstrumentLocation(CInstrument* pInstrument);
	// 删除尾包之后的仪器
	void TailFrameDeleteInstrument(CInstrument* pInstrument);
	// 清理过期的尾包时刻查询结果
	void ClearExperiedTailTimeResult(void);
	// 删除所有仪器
	void DeleteAllInstrument(void);
	// 判断仪器连接线号
	void OnSetInstrumentLineIndex(CInstrument* pInstrument);
	// 建立仪器的连接关系
	void OnInstrumentConnect(CInstrument* pInstrument);
	// 清除相同路由仪器的尾包计数
	void OnClearSameRoutTailCount(CInstrument* pInstrument);
	// 删除大线方向尾包之后的仪器
	void TailFrameDeleteInstrumentLine(CInstrument* pInstrument);
	// 删除交叉线上交叉站交叉线尾包之后的仪器
	void TailFrameDeleteInstrumentLXLine(CInstrument* pInstrument);
	// LCI设备的SN号
	unsigned int m_uiLCISn;
	// 检查尾包时刻查询应答是否全部接收
	bool OnCheckTailTimeReturn(void);
	// 得到采集站设备序号
	void OnGetFduIndex(void);
	// 按照仪器路由删除该路由方向上的仪器
	void TailFrameDeleteInstrumentRout(unsigned int uiRoutAddr);
};
