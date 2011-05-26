#pragma once
#include "Instrument.h"
class CInstrumentList
{
public:
	CInstrumentList(void);
	~CInstrumentList(void);
public:
	CInstrument* m_pInstrumentArray;

	// 初始化
	void OnInit(void);
	// 关闭
	void OnClose(void);
	// 仪器总数
	unsigned int m_uiCountAll;
	// 空闲仪器数量
	unsigned int m_uiCountFree;
	// 空闲仪器队列
	CList<CInstrument*, CInstrument*> m_olsInstrumentFree;
	// 仪器SN索引表
	CMap<unsigned int, unsigned int, CInstrument*, CInstrument*> m_oInstrumentMap;
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
	// 仪器的IP地址都设置成功
	int m_bIPSetAllOK;
	// 设备根据首包时刻排序
	void SetInstrumentLocation(CInstrument* pInstrumentAdd);
	// 删除采集站尾包之后的仪器
	void TailFrameDeleteInstrument(CInstrument* pInstrumentDelete);
	// 清理过期的尾包时刻查询结果
	void ClearExperiedTailTimeResult(void);
};
