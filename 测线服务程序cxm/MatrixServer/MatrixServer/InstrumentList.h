#pragma once

#include "Instrument.h"
#include <list>
#include <hash_map>

using std::list;
using stdext::hash_map;
/**
*@brief 仪器队列类
*/
class CInstrumentList
{
public:
	CInstrumentList();
	~CInstrumentList();

public: //属性
	/** 仪器数组指针*/
	CInstrument* m_pArrayInstrument;
	/** 仪器数组指针，为现场数据输出准备*/
	CInstrument* m_pArrayInstrumentOutput;
	/** 空闲仪器队列*/
	list<CInstrument*> m_olsInstrumentFree;
	/** 设置IP地址的仪器队列*/
	hash_map<unsigned int, CInstrument*> m_oIPSetMap;
	// 仪器SN索引表
	hash_map<unsigned int, CInstrument*> m_oSNInstrumentMap;
	// 仪器IP地址索引表
	hash_map<unsigned int, CInstrument*> m_oIPInstrumentMap;
	/** 仪器总数*/
	unsigned int m_uiCountAll;
	/** 空闲仪器数量*/
	unsigned int m_uiCountFree;

public: //方法
	// 初始化
	void OnInit();
	// 关闭
	void OnClose();
	// 重置
	void OnReset();
	// 得到一个空闲仪器
	CInstrument* GetFreeInstrument();
	// 增加一个空闲仪器
	void AddFreeInstrument(CInstrument* pInstrument);
	// 增加一个IP地址设置仪器
	void AddInstrumentToIPSetMap(unsigned int uiIndex, CInstrument* pInstrument);
	// 复制仪器数组到输出数组
	void CopyInstrumentArrayForOutput();
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
};
